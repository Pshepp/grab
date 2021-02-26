/**
 * @file node.h
 * @brief Something about this node class and its structure.
 *
 * More info can go here.
 *
 *	Notes:
 *		- Replace our std::vector labels with an unsorted set if we do not need to track
 *			label insertion order
 *
 *
 *	TODO:
 *		- Figure out how to properly creating our node index
 *		- Eventually use unorded_sets to increase speed
 *		- Figure out if our labels are truly a LIFO structure
 *
 * @author Preston
 * @date 22 Feb 2021
 */

#ifndef INC_NODE_H_
#define INC_NODE_H_

/************************************************
 *  BEGIN TO REMOVE IN FINAL, USED FOR BUG TESTING
 ***********************************************/

#include <iostream>

/************************************************
 *  END TO REMOVE IN FINAL
 ***********************************************/

#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>

#include "graph.h"
#include "edge.h"


template<class T>
class Node: public std::enable_shared_from_this<Node<T> >
{
public:

	Node()
	{
		defaultInit();
	}

	Node(std::string inputName)
	{
		defaultInit(inputName);
	}

	Node(std::string inputName, std::vector<std::string> inputLabels)
	{
		defaultInit(inputName, inputLabels);
	}


	Node(std::string inputName, std::vector<std::string> inputLabels, std::weak_ptr<Graph<T>> parentGraph)
	{
		defaultInit(inputName, labelsToAdd, parentGraph);
	}

	~Node()
	{
		this->deleteNode();
	}

	void setLabels(std::vector<std::string> inputLabels);
	std::vector<std::string> getLabels();
	bool constainsLabel(std::string possibleLabel);

	void setName(std::string inputName);
	std::string getName();

	void setIsVisited(bool isVisited);
	bool getIsVisited();

	void setIsBridge(bool isBridge);
	bool getIsBridge();

	void setIsLeaf(bool isLeaf);
	bool getIsLeaf();

	void setIndex(unsigned short int index);
	short int getIndex();

	bool isNeighbor(Node<T> possibleNeighbor);
	bool isChild(Node<T> possibleChild);
	bool isParent(Node<T> possibleParent);

	//delete from specific graph and all graphs
	void deleteNode();
	void deleteNode(std::weak_ptr<Graph<T>>);

	void deleteEdge(std::unique_ptr<Edge<T>> outEdge);
	void deleteEdge(Edge<T> *inEdge);
	void deleteAllEdges();

	void addNeighbor(std::shared_ptr<Node<T>> destNode, std::string inputName);
	void deleteNeighbor(std::shared_ptr<Node<T>> neighborToDelete);
	void deleteNeighbor(std::shared_ptr<Node<T>> neighborToDelete, std::weak_ptr<Graph <T>> containingGraph);

	void addGraphOwnership(std::weak_ptr<Graph <T>> graphToAdd);


	std::vector<std::shared_ptr<Node<T>>> getNeighbors();

private:
	unsigned short int index;

	std::string name;
	std::vector<std::string> labels;

	bool isVisited;
	bool isBridge;
	bool isLeaf;

	std::vector<std::unique_ptr<Edge<T> > > outEdges;
	std::vector<Edge<T>*> inEdges;

	std::vector<std::weak_ptr<Graph<T> > > parentGraphs;

	void defaultInit();
	void defaultInit(std::string name);
	void defaultInit(std::string name, std::vector<std::string> initialLabels);
	void Node<T>::defaultInit(std::string inputName,
			std::vector<std::string> labelsToAdd, std::weak_ptr<Graph<T>> parentGraph);

};
//Node

/************************************************
 *  STRUCTURE CHECKS
 ***********************************************/

template<typename T>
bool Node<T>::isNeighbor(Node<T> possibleNeighbor)
{
	return (isChild(possibleNeighbor) || isParent(possibleNeighbor));
}

template<typename T>
bool Node<T>::isChild(Node<T> possibleChild)
{
	for (auto const &edgePtr : this->outEdges)
		if (edgePtr->destNode == possibleChild)
			return true;
	return false;
}

template<typename T>
bool Node<T>::isParent(Node<T> possibleParent)
{
	for (auto const &edgePtr : this->inEdges)
		if (edgePtr->sourceNode == possibleParent)
			return true;
	return false;
}

template<typename T>
bool Node<T>::constainsLabel(std::string possibleLabel)
{
	return (std::find(this->labels.begin(), this->labels.end(), possibleLabel)
			!= this->labels.end());
}

template<typename T>
std::vector<std::shared_ptr<Node<T>>> Node<T>::getNeighbors()
{
	std::vector<std::shared_ptr<Node<T>>> neighbors;

	for (auto &edge : this->outEdges)
	{
		neighbors.push_back(edge->destNode.lock()); //maybe need check
	}

	for (auto & edge : this->inEdges)
	{
		neighbors.push_back(edge->sourceNode.lock());
	}

	return neighbors;
}

/************************************************
 *  GETTERS
 ***********************************************/

template<typename T>
short int Node<T>::getIndex()
{
	return this->index;
}

template<typename T>
std::vector<std::string> Node<T>::getLabels()
{
	return this->labels;
}

template<typename T>
bool Node<T>::getIsBridge()
{
	return isBridge;
}

template<typename T>
bool Node<T>::getIsVisited()
{
	return isVisited;
}

template<typename T>
bool Node<T>::getIsLeaf()
{
	return isLeaf;
}

template<typename T>
std::string Node<T>::getName()
{
	return name;
}

/************************************************
 *  SETTERS
 ***********************************************/

template<typename T>
void Node<T>::setIndex(unsigned short int index)
{
	this->index = index;
}

// TODO: Check that this properly replaces the member vector
template<typename T>
void Node<T>::setLabels(std::vector<std::string> inputLabels)
{
	this->labels = inputLabels;
}

template<typename T>
void Node<T>::setIsVisited(bool status)
{
	this->isVisited = status;
}

template<typename T>
void Node<T>::setIsBridge(bool status)
{
	this->isBridge = status;
}

template<typename T>
void Node<T>::setIsLeaf(bool status)
{
	this->isLeaf = status;
}

template<typename T>
void Node<T>::setName(std::string inputName)
{
	this->name = inputName;
}

/************************************************
 *  MUTATORS
 ***********************************************/

//DATA ADDITION
template<typename T>
void Node<T>::addNeighbor(std::shared_ptr<Node<T>> destNode, std::string inputName)
{
//NOTE: We are creating a directed link between this node and the one passed

	//creates our edge and puts it into our outedges vec
	this->outEdges.push_back(std::unique_ptr<Edge<T>> (new Edge<T>(inputName, this->Node<T>, destNode <T>)));
	Edge<T> * rawBackPtr = this->outEdges.back().get();
	destNode->inEdges.push_back(rawBackPtr);
}

template<typename T>
void Node<T>::addLabels(std::vector<std::string> inputLabels)
{
	this->labels.insert(this->labels.end(), inputLabels.begin(),
			inputLabels.end());
}

template<typename T>
void Node<T>::addLabel(std::string inputLabel)
{
	this->labels.push_back(inputLabel);
}

//want to use unordered set here, prevent dupes
template<typename T>
void Node<T>::addGraphOwnership(std::weak_ptr<Graph <T>> graphToAdd)
{
	this->parentGraphs.push_back(graphToAdd);
}


//DATA REMOVAL

//void deleteNeighbor(std::shared_ptr<Node<T>> neighborToDelete);

//deletes neighbor in ALL shared graph structures
template<typename T>
void Node<T>::deleteNeighbor(std::shared_ptr<Node<T>> neighborToDelete)
{
	//TODO: faster
	for (auto& callingNodeGraph : this->parentGraphs)
	{
		for (auto& toDeleteNodeGraph : neighborToDelete->parentGraphs)
		{
			if (callingNodeGraph.lock().get() == toDeleteNodeGraph.lock().get())
				neighborToDelete->deleteNode(toDeleteNodeGraph);
		}
	}
}

//delete neighbor in specific shared graph structure
template<typename T>
void Node<T>::deleteNeighbor(std::shared_ptr<Node<T>> neighborToDelete, std::weak_ptr<Graph <T>> containingGraph)
{
	if (!this->isNeighbor(neighborToDelete))
	{
		cout << "DEATH ON LINE, NOT NEIGHBOR! " << __LINE__ << endl;
		return;
	}
	for (auto& callingNodeGraph : this->parentGraphs)
	{
		if (callingNodeGraph.lock().get() == containingGraph.lock().get())
			neighborToDelete->deleteNode(callingNodeGraph);
	}

}


//need to find a better name, delete from all graphs
template<typename T>
void Node<T>::deleteNode()
{
	for (auto & parentGraph : this->parentGraphs)
		deleteNode(parentGraph);
}

//delete from specified graph
template<typename T>
void Node<T>::deleteNode(std::weak_ptr<Graph<T>> parentGraph)
{
	this->deleteAllEdges(); //ensure only shared ptr to this is now from our graph structures, need to pass parent graph func
	//std::vector<std::weak_ptr<Graph<T> > > parentGraphs;

	if (!parentGraph.lock())
	{
		std::cout << "\n************************************************";
		std::cout << "WARNING CANNOT REMOVE SELF FROM PARENT GRAPHS LINE"
				<< __LINE__;
		std::cout << "************************************************";
	}
	else
	{
		std::shared_ptr<Graph<T>> sharedParentGraph = parentGraph.lock();

		sharedParentGraph->nodesInGraph.erase(
				std::remove(sharedParentGraph->nodesInGraph.begin(),
						sharedParentGraph->nodesInGraph.end(), this->Node<T>),
				sharedParentGraph->nodesInGraph.end());
	}
	this->parentGraphs.erase(
			std::remove(this->parentGraphs.begin(), this->parentGraphs.end(),
					parentGraph), this->parentGraphs.end());

}

//Ensure logic, should delete all edges.
template<typename T>
void Node<T>::deleteAllEdges()
{
	for (auto &outEdge : this->outEdges)
	{
		deleteEdge(outEdge);
	}

	for (auto &inEdge : this->inEdges)
	{
		deleteEdge(inEdge);
	}
}

/*
 * TODO: Desketti
 */
template<typename T>
void Node<T>::deleteEdge(std::unique_ptr<Edge<T>> outEdge)
{
	for (auto &rawEdge : outEdge->destNode->inEdges)
	{
		//Removes all our raw pointers from the destination nodes vector, should only be hit once
		if (rawEdge == outEdge.get())
			outEdge->destNode->inEdges.erase(
					std::remove(outEdge->destNode->inEdges.begin(),
							outEdge->destNode->inEdges.end(), rawEdge),
					outEdge->destNode->inEdges.end());
	}
	//hopefully destroys the unique ptr
	this->outEdges.erase(
			std::remove(this->outEdges.begin(), this->outEdges.end(), outEdge),
			this->outEdges.end());
}

//to help show ownership we delete from the outEdge ptr, we just translate to our unique our edge ptr
template<typename T>
void Node<T>::deleteEdge(Edge<T> *inEdge)
{
	for (auto &uniqueEdge : inEdge->sourceNode->outEdges)
	{
		if (uniqueEdge.get() == inEdge)
			deleteEdge(uniqueEdge);
	}
}

/************************************************
 *  PRIVATE FUNCTIONS
 ***********************************************/

// CONSTRUCTION HELPERS
template<typename T>
void Node<T>::defaultInit()
{
	this->setIndex(1);
	this->setName("DEFAULT_NODE_NAME");
	this->setIsVisited(false);
	this->setIsLeaf(false);
	this->setIsBridge(false);
}

template<typename T>
void Node<T>::defaultInit(std::string inputName)
{
	this->setIndex(1);
	this->setName(inputName);
	this->setIsVisited(false);
	this->setIsLeaf(false);
	this->setIsBridge(false);
}

template<typename T>
void Node<T>::defaultInit(std::string inputName,
		std::vector<std::string> labelsToAdd)
{
	this->setIndex(1);
	this->setName(inputName);
	// TODO: Check if memory leak occurs
	this->labels.swap(labelsToAdd);
	this->setIsVisited(false);
	this->setIsLeaf(false);
	this->setIsBridge(false);
}

template<typename T>
void Node<T>::defaultInit(std::string inputName,
		std::vector<std::string> labelsToAdd, std::weak_ptr<Graph<T>> parentGraph)
{
	this->setIndex(1);
	this->setName(inputName);
	// TODO: Check if memory leak occurs
	this->labels.swap(labelsToAdd);
	this->setIsVisited(false);
	this->setIsLeaf(false);
	this->setIsBridge(false);
	this->addGraphOwnership(parentGraph);
}

#endif /* INC_NODE_H_ */
