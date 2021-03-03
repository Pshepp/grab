/**
 * @file graph.h
 * @author Preston Sheppard
 * @date 22 Feb 2021
 * @brief This graph class is to be used as a part of our total graph abstraction "toolkit".
 *
 * The use of this is to act as a "structure" that contains our nodes, etc. within the graph. We
 * will only be having pointers to our Nodes within the graph structure, recall how edges are owned by
 * the nodes.
 *
 * To allow multiple different graph structures to share the same nodes by memory we will have to map
 * our data memebers of node that create our structure to a graph (i.e. graph = key, members = value)
 * Will have to wrap members
 *
 * Since our graph class is what "holds" our structure (well, holds nodes which create the structure) we
 * will want to run our algos on this.
 *
 *  ADD MORE INFO/CLARIFICATION AS NEEDED
 */

#ifndef INC_GRAPH_H_
#define INC_GRAPH_H_

#include <string>
#include <vector>
#include <memory>

//may be able to switch this to a forward declare
#include "node.h"

template<class T>
class Graph
{
public:
	Graph() //should never be called (maybe), honestly it is fine being called
	{
		this->setName("DEFAULT_GRAPH_NAME");
		this->setIndex(1);
	}

	/************************************************
	 *  GETTER/SETTER PAIRS
	 ***********************************************/
	void setIndex(unsigned short int index);
	unsigned short int getIndex() const;

	void setName(std::string name);
	std::string getName() const;

	void setLabels(std::vector<std::string> labels);
	std::vector<std::string> getLabels();

	std::vector<std::vector<std::shared_ptr<Node<T>>>> makeAdjacencyList(); //adj list built when called

	std::vector<std::shared_ptr<Node<T>>> getNodes();

	bool containsNode(std::shared_ptr<Node<T>> possibleNode);

	void lazyPrintAdjList();

	/************************************************
	 *  MUTATORS
	 ***********************************************/
	void addLabel(std::string label);
	void addLabel(std::vector<std::string> labels);

	void addNode(std::shared_ptr<Node<T>> nodeToAdd);
	//void addNode(std::vector<std::shared_ptr<Node<T>>> nodesToAdd);

	void deleteNode(std::shared_ptr<Node<T>> nodeToDelete);

private:
	unsigned short int index;
	std::string name;
	std::vector<std::string> labels;

	//TODO: Keep us from having dupes
	std::vector<std::shared_ptr<Node<T>>> nodesWithin;

};

/************************************************
 *  GETTER/SETTER PAIRS
 ***********************************************/

template<class T>
void Graph<T>::setIndex(unsigned short int index)
{
	this->index = index;
}

template<class T>
unsigned short int Graph<T>::getIndex() const
{
	return this->index;
}

template<class T>
void Graph<T>::setName(std::string name)
{
	this->name = name;
}

template<class T>
std::string Graph<T>::getName() const
{
	return this->name;
}

template<class T>
void Graph<T>::setLabels(std::vector<std::string> labels)
{
	this->labels = labels;
}

template<class T>
std::vector<std::string> Graph<T>::getLabels()
{
	return this->labels;
}

template<class T>
std::vector<std::vector<std::shared_ptr<Node<T> > > > Graph<T>::makeAdjacencyList()
{
	std::vector<std::vector<std::shared_ptr<Node<T>>>> adjList;
	for (std::shared_ptr<Node<T>> currNode : this->nodesWithin)
		adjList.push_back(currNode.get()->getNeighbors());
	return adjList;
}

template<class T>
std::vector<std::shared_ptr<Node<T> > > Graph<T>::getNodes()
{
	return this->nodesWithin;
}

/************************************************
 *  MUTATORS
 ***********************************************/

template<class T>
void Graph<T>::addLabel(std::string label)
{
	this->labels.push_back(label);
}

template<class T>
void Graph<T>::addLabel(std::vector<std::string> labels)
{
	for (std::string currLabel : labels)
		this->addLabel(currLabel);
}

template<class T>
void Graph<T>::addNode(std::shared_ptr<Node<T> > nodeToAdd)
{
	this->nodesWithin.push_back(nodeToAdd);
}

template<class T>
bool Graph<T>::containsNode(std::shared_ptr<Node<T> > possibleNode)
{
	for (std::shared_ptr<Node<T>> currNode : this->nodesWithin)
	{
		if (currNode.get() == possibleNode.get())
			return true;
	}
	return false;
}

//doesnt print the actual node we are looking at
template<class T>
void Graph<T>::lazyPrintAdjList()
{
	std::vector<std::vector<Node<T>>> tempList = this->makeAdjacencyList();
	std::cout << std::endl << "==== ADJ LIST ====";
	for (std::vector<std::shared_ptr<Node<T>>> nodeNeighborList : tempList)
	{
		std::cout << std::endl;
		for (std::shared_ptr<Node<T>> nodeNeighbor : nodeNeighborList)
			std::cout << " -> " << nodeNeighbor.get()->getName() << ", ";
	}
}

template<class T>
void Graph<T>::deleteNode(std::shared_ptr<Node<T> > nodeToDelete)
{
	if (!(this->containsNode(nodeToDelete)))
	{
		badBehavior(__LINE__, __func__);
		std::cout << "Doesn't contain node" << std::endl;
		return;
	}
	//delete all our edges which only leaves the shared pointer from our graph structure. This allows for a floating node in our structures.
	nodeToDelete.get()->deleteAllEdges();
	//sorry, just want to delete by index because lazy for now. This is bad due to the face that it is not defined in 100% of cases. Small edge cases plus very inneficient
	for (int currIndex = 0; currIndex < this->nodesWithin.size(); currIndex++)
	{
		//found node now remove from final place
		if (this->nodesWithin[currIndex].get() == nodeToDelete.get())
			this->nodesWithin.erase(this->nodesWithin.begin() + currIndex);
	}

	if (this->containsNode(nodeToDelete))
	{
		badBehavior(__LINE__, __func__);
		std::cout << "Node did not delete" << std::endl;
	}
}

//LAZY ERROR USE FOLLOWING TO CALL badBehavior(__LINE__, __func__);
void badBehavior(int lineBroke, const char *funcNameBroke)
{
	std::cout << "****************************************" << std::endl
			<< "\tBORKED" << "****************************************"
			<< std::endl;
	std::cout << "Borked Function Name: " << funcNameBroke << std::endl;
	std::cout << "Line Number: " << lineBroke << std::endl << std::endl;
}

#endif /* INC_GRAPH_H_ */

