/**
 * @file node.h
 * @author Preston Sheppard
 * @date 22 Feb 2021
 * @brief This node class is to be used as a part of our total graph abstraction "toolkit".
 *
 * We are defining a node object that utilizes smart pointers to not only increase efficiency but
 * to also help denote "ownership". Our node has a unique_ptr to each connected edge, thus our node
 * owns the edge. This is needed due to smart pointers needing some form of ownership hierarchy in
 * order to allow for proper use. Please note our node pointer structure. The reasoning behind a node
 * owning an edge is due to if a node is deleted all edges must be deleted but not necessarily vice-versa.
 *
 * Please note that not only are our nodes "owned" by the edges, but also by the over-arching graph
 * structure itself. This way we are able to allow nodes to be within the graph structure yet have no
 * edges.
 *
 * ADD MORE INFO/CLARIFICATION AS NEEDED
 */

/*	TODO: The following are in no particular order:
 * 			- Increase our speed using unordered_set, etc. Please keep in mind that we will have to ensure our iterator is correct.
 * 			- Properly track our node index and figure out the pipeline the adjacency matrix/list should use.
 * 					- Fix: Nest our set of edge pointers within a hashmap where key = graph ptr or something
 * 						robustly hashable and value is to our vector of ptrs for our nodes :o)
 * 			- Fix node presence in multiple graph structures, this will take adding a "layer" to our edges that correlates to each structure
 * 				- The following will be within the new "layer" due to their implications in the graph structure itself:
 * 						- leaf, bridge, visited, inEdges, outEdges
 * 			- Possibly implement a relationship check (i.e Node A == Node B) for hunting and raw ptr checks. Will make finding a specific
 * 				node to delete much easier.
 * 			- If memory footprint is too large, begin passing by reference for some of our functions i.e.
 * 					- label addition, etc.
 * 					- returning of our neighbors
 * 			- Algos (mostly worried about how we want to call them, should become clear once all are clean):
 * 				- Leaf detection
 * 				- Bridge detection
 * 			- Good error throwing instead of cout laziness
 */

#ifndef INC_NODE_H_
#define INC_NODE_H_

//TODO: Eventually delete
#include <iostream>
#include "lazyPrints.h"
const bool nodeDebug = true;

#include <vector>
#include <string>
#include <memory>
#include <algorithm>

#include "edge.h"

template<class T>
class Node: public std::enable_shared_from_this<Node<T>>
{
public:
	/************************************************
	 *  CONSTRUCTORS
	 ***********************************************/
	Node()
	{
		this->setName("DEFAULT_NODE_NAME");
		this->setIndex(1);
		this->setIsLeaf(false);
		this->setIsBridge(false);
		this->setIsVisited(false);
		if (nodeDebug)
		{
			std::string conMsg =
					"Warning, created a node with no name. This should not have been called.";
			badBehavior(__LINE__, __func__, conMsg);
		}
	}

	Node(std::string name)
	{
		this->setName(name);
		this->setIndex(1);
		this->setIsLeaf(false);
		this->setIsBridge(false);
		this->setIsVisited(false);
		if (nodeDebug)
		{
			std::string conMsg = "Constructed a node with name: "
					+ this->getName();
			lazyInfo(__LINE__, __func__, conMsg);
		}
	}

	~Node()
	{
		if (nodeDebug)
		{
			//TODO: Node destructor information msg
		}
	}

	/************************************************
	 *  GETTER/SETTER PAIRS
	 ***********************************************/
	void setIndex(unsigned short int index);
	unsigned short int getIndex() const;

	void setName(std::string name);
	std::string getName() const;

	void setLabels(std::vector<std::string> labels);
	std::vector<std::string> getLabels() const;

	void setIsLeaf(bool leaf);
	bool getIsLeaf() const;

	void setIsBridge(bool bridge);
	bool getIsBridge() const;

	void setIsVisited(bool visited);
	bool getIsVisited() const;

	std::weak_ptr<Node<T>> getNeighborByName(std::string name);
	std::weak_ptr<Node<T>> getNeighborByIndex(unsigned short index);

	std::vector<std::weak_ptr<Node<T>>> getNeighbors();
	std::vector<std::weak_ptr<Node<T>>> getChildren();
	std::vector<std::weak_ptr<Node<T>>> getParents();

	/************************************************
	 *  MUTATORS
	 ***********************************************/
	void addLabel(std::string label);
	void addLabel(std::vector<std::string> labels);

	void addNeighbor(std::shared_ptr<Node<T>> neighborToAdd,
			std::string edgeName);
	void deleteNeighbor(std::shared_ptr<Node<T>> neighborToRemove);

	void deleteEdge(std::shared_ptr<Node<T>> secondNode);
	void deleteAllEdges();

	void switchEdgeDirection(std::shared_ptr<Node<T>> nodeB);

	/************************************************
	 *  STRUCTURAL/RELATIONSHIP CHECKS
	 ***********************************************/
	bool isNeighbor(std::shared_ptr<Node<T>> possibleNeighbor);
	bool isChild(std::shared_ptr<Node<T>> possibleParent);
	bool isParent(std::shared_ptr<Node<T>> possibleChild);

	bool containsLabel(std::string labelToCheck);
	bool containsLabel(std::vector<std::string> labelToCheck);

	//TODO: Get rid of this garbage
	Edge<T>* getConnectingEdge(std::shared_ptr<Node<T>> nodeB);

private:
	//TODO: Remove these lazy checks that are used to keep an eye on things

	//gets our edge array sizes, prints them out, then gets the contents
	void edgeCheck()
	{
		std::cout << "========= EDGE CHECK =========" << std::endl;
		std::cout << "In Edges size: " << this->inEdges.size() << std::endl;
		for (auto const &inEdge : this->inEdges)
		{
			std::cout << inEdge->getName() << " , ";
		}
		//all in edges printed
		std::cout << std::endl << "Out Edges size:" << this->outEdges.size()
				<< std::endl;
		//please note that I went ahead and explicitly declared our reference in order to help with understanding
		for (std::unique_ptr<Edge<T>> const &outEdge : this->outEdges)
		{
			std::cout << outEdge.get()->getName() << ", ";
		}
		std::cout << std::endl;
		//all out edges printed, check if we break when removing "&"
	}

	/************************************************
	 *  IDENTIFIERS
	 ***********************************************/
	unsigned short int index;
	std::string name;
	std::vector<std::string> labels;

	/************************************************
	 *  STRUCTURAL ATTRIBUTES
	 ***********************************************/
	bool leaf;
	bool bridge;

	/************************************************
	 *  ALGORITHM TRAVERSAL ATTRIBUTES
	 ***********************************************/
	bool visited;

	/************************************************
	 *  STRUCTURAL OWNERSHIP
	 ***********************************************/
	std::vector<std::unique_ptr<Edge<T>>> outEdges;
	std::vector<Edge<T>*> inEdges;

	/************************************************
	 *  HELPER FUNCTIONS
	 ***********************************************/
	void deleteInEdge(Edge<T> *inEdgeToDelete);
	void deleteOutEdge(Edge<T> *outEdgeToDelete);

	bool hasInEdge(Edge<T> *possibleInEdge);
	bool hasOutEdge(Edge<T> *possibleOutEdge);

};

/************************************************
 *  GETTER/SETTER PAIRS
 ***********************************************/

template<class T>
void Node<T>::setIndex(unsigned short int index)
{
	this->index = index;
}

template<class T>
unsigned short int Node<T>::getIndex() const
{
	return this->index;
}

template<class T>
void Node<T>::setName(std::string name)
{
	this->name = name;
}

template<class T>
std::string Node<T>::getName() const
{
	return this->name;
}

template<class T>
void Node<T>::setLabels(std::vector<std::string> labels)
{
	this->labels = labels;
}

template<class T>
std::vector<std::string> Node<T>::getLabels() const
{
	return this->labels;
}

template<class T>
void Node<T>::setIsLeaf(bool leaf)
{
	this->leaf = leaf;
}

template<class T>
bool Node<T>::getIsLeaf() const
{
	return this->leaf;
}

template<class T>
void Node<T>::setIsBridge(bool bridge)
{
	this->bridge = bridge;
}

template<class T>
bool Node<T>::getIsBridge() const
{
	return this->bridge;
}

template<class T>
void Node<T>::setIsVisited(bool visited)
{
	this->visited = visited;
}

template<class T>
bool Node<T>::getIsVisited() const
{
	return this->visited;
}

//TODO: Get rid of this extra garbage, please note that this raw ptr being returned may not even exist after a deletion
template<class T>
Edge<T>* Node<T>::getConnectingEdge(std::shared_ptr<Node<T>> nodeB)
{
	if (!(this->isNeighbor(nodeB)))
	{
		badBehavior(__LINE__, __func__,
				"Warning, these nodes are NOT neighbors!");
		return NULL;
	}
	lazyInfo(__LINE__, __func__, "Starting");
	edgeCheck();
	nodeB.get()->edgeCheck();
	//get the edge if nodeB is parent, checking in callings in list
	Edge<T> *ptrFromCallingIn = NULL;
	//get the edge if nodeB is child, checking in callings out list
	Edge<T> *ptrFromCallingOut = NULL;
	//get the edge if calling is parent, checking the callees in list
	Edge<T> *ptrFromCalleeIn = NULL;
	Edge<T> *ptrFromCalleeOut = NULL;
	int count = 0;
	//Going to EXTREMELY hard check our logic to ensure everything behaves correctly.
	std::string borkedMsg =
			"We encountered an issue with getting our connecting edge.";
	for (auto const &inEdge : this->inEdges)
	{
		//behaving as if our calling is a child, so we must get parent
		if (inEdge->getSourceNode().get() == nodeB.get())
		{
			count++;
			ptrFromCallingIn = inEdge;
		}
	}
	if (count > 1)
	{
		badBehavior(__LINE__, __func__, borkedMsg + "Count value too high!");
		return NULL;
	}
	else
		count = 0;
	for (auto const &inEdge : nodeB.get()->inEdges)
	{
		//behaving as if our calling is a child, so we must get parent
		if (inEdge->getSourceNode().get() == this)
		{
			lazyInfo(__LINE__, __func__,
					"Found parent of calling node: " + this->getName());
			count++;
			ptrFromCalleeIn = inEdge;
		}
	}
	if (count > 1)
	{
		badBehavior(__LINE__, __func__, borkedMsg + "Count value too high!");
		return NULL;
	}
	else
		count = 0;
	for (std::unique_ptr<Edge<T>> const &outEdge : this->outEdges)
	{
		if (outEdge.get()->getSinkNode().get() == nodeB.get())
		{
			lazyInfo(__LINE__, __func__,
					"Found a child from calling node: " + this->getName());
			count++;
			ptrFromCallingOut = outEdge.get();
		}
	}
	if (count > 1)
	{
		badBehavior(__LINE__, __func__, borkedMsg + "Count value too high!");
		return NULL;
	}
	else
		count = 0;
	for (std::unique_ptr<Edge<T>> const &outEdge : nodeB.get()->outEdges)
	{
		if (outEdge.get()->getSinkNode().get() == this)
		{
			lazyInfo(__LINE__, __func__, "Found a child from our 2nd node");
			count++;
			ptrFromCalleeOut = outEdge.get();
		}
	}
	if (count > 1)
	{
		badBehavior(__LINE__, __func__, borkedMsg + "Count value too high!");
		return NULL;
	}
	else
		count = 0;
	if (nodeDebug)
	{
		std::cout << std::endl
				<< "AGGGGGGGGGGGGGGGGHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
				<< ptrFromCalleeIn << std::endl;
		std::cout << std::endl << "CALLING IN: " << ptrFromCallingIn
				<< std::endl << "CALLING OUT: " << ptrFromCallingOut
				<< std::endl << "OKAY COOL";
		std::cout << std::endl << "CALLEE IN: " << ptrFromCalleeIn << std::endl
				<< "CALLEE OUT: " << ptrFromCalleeOut << std::endl;
	}
	//disgusting check to make sure edges are being nice, use xor to make sure only 1 is true
	if (((ptrFromCallingIn == ptrFromCalleeOut)
			|| (ptrFromCallingOut == ptrFromCalleeIn))
			//ensure not all nulls
			&& ((ptrFromCallingIn != NULL) || (ptrFromCallingOut != NULL))
			//after making sure the pointers are related in the way they should be, we check for odd stuff. Making sure that our edge does not pt to self
			&& (ptrFromCallingIn != ptrFromCallingOut)
			&& (ptrFromCalleeIn != ptrFromCalleeOut)
			&& (ptrFromCallingIn != ptrFromCalleeIn)
			&& (ptrFromCallingOut != ptrFromCalleeOut))
	{

		Edge<T> *toRet =
				(ptrFromCallingIn == 0) ? ptrFromCallingOut : ptrFromCallingIn;
		if (nodeDebug)
		{
			std::cout << std::endl << std::endl
					<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
					<< std::endl << toRet << std::endl
					<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
					<< std::endl;
		}

		return toRet;
	}
	else
	{
		badBehavior(__LINE__, __func__,
				borkedMsg + "\n~~~DISGUSTING CHECK FAILED~~~");
		return NULL;
	}

}

template<class T>
std::weak_ptr<Node<T>> Node<T>::getNeighborByName(std::string name)
{
	std::vector<std::weak_ptr<Node<T>>> neighbors = this->getNeighbors();
	int count = 0;
	std::weak_ptr<Node<T>> tempReturn;
	for (std::weak_ptr<Node<T>> &curNeighbor : neighbors)
	{
		if (curNeighbor.lock())
		{
			std::shared_ptr<Node<T>> temp = curNeighbor.lock();
			if (temp.get()->getName() == name)
			{
				count++;
				tempReturn = temp;
			}
		}
		else
		{
			badBehavior(__LINE__, __func__, "Couldnt lock!");
		}
	}
	if (count > 1)
		badBehavior(__LINE__, __func__, "Multiple neighbor with same name");
	return tempReturn;

}

template<class T>
std::weak_ptr<Node<T>> Node<T>::getNeighborByIndex(unsigned short index)
{
	std::vector<std::weak_ptr<Node<T>>> neighbors = this->getNeighbors();
	int count = 0;
	std::weak_ptr<Node<T>> tempReturn;
	for (std::weak_ptr<Node<T>> &curNeighbor : neighbors)
	{
		if (curNeighbor.lock())
		{
			std::shared_ptr<Node<T>> temp = curNeighbor.lock();
			if (temp.get()->getIndex() == index)
			{
				count++;
				tempReturn = temp;
			}
		}
		else
		{
			badBehavior(__LINE__, __func__, "Couldnt lock!");
		}
	}
	if (count > 1)
		badBehavior(__LINE__, __func__, "Multiple neighbor with same index");
	return tempReturn;
}

/* TODO: How would we like to pass this, in current use it creates a vector of shared
 * 			ptrs that increments the ownership count by 1. As of now let us leave this be.
 *			We must be careful regarding exposing the ptr to the object itself. This will
 *			cause us to break our ownership hierarchy. As of now, returning weak_ptrs to our
 *			smart_ptrs should be enough.
 */
template<class T>
std::vector<std::weak_ptr<Node<T>>> Node<T>::getNeighbors()
{
	std::vector<std::weak_ptr<Node<T>>> children = this->getChildren();
	std::vector<std::weak_ptr<Node<T>>> parents = this->getParents();
	children.insert(children.end(), parents.begin(), parents.end());
	return children;
}

template<class T>
inline std::vector<std::weak_ptr<Node<T> > > Node<T>::getChildren()
{
	std::vector<std::weak_ptr<Node<T>>> children;
	for (std::unique_ptr<Edge<T>> const &outEdge : this->outEdges)
	{
		children.push_back(outEdge->getSinkNode());
	}
	return children;
}

template<class T>
std::vector<std::weak_ptr<Node<T> > > Node<T>::getParents()
{
	std::vector<std::weak_ptr<Node<T>>> parents;
	for (auto const &inEdge : this->inEdges)
	{
		parents.push_back(inEdge->getSourceNode());
	}
	return parents;
}

/************************************************
 *  STRUCTURAL/RELATIONSHIP CHECKS
 ***********************************************/

template<class T>
bool Node<T>::isNeighbor(std::shared_ptr<Node<T>> possibleNeighbor)
{
	/* Eventually use this, currently done a different way to keep track on input
	 * return (this->isChild(possibleNeighbor) ^ this->isNeighbor(possibleNeighbor));
	 */
	bool verboseIsChild = this->isChild(possibleNeighbor);
	bool verboseIsParent = this->isParent(possibleNeighbor);
	if (nodeDebug)
	{
		std::string isNeighborMsg = "DEFAULT_MSG";
		if (verboseIsParent && verboseIsChild)
		{
			isNeighborMsg = this->getName() + " points to itself";
		}
		else if (verboseIsChild ^ verboseIsParent)
		{
			isNeighborMsg = this->getName() + " is neighbor of "
					+ possibleNeighbor.get()->getName();
		}
		else if (!(verboseIsParent & verboseIsChild))
		{
			isNeighborMsg = this->getName() + " is not neighbor of "
					+ possibleNeighbor.get()->getName();
		}
		lazyInfo(__LINE__, __func__, isNeighborMsg);
	}
	return (verboseIsChild ^ verboseIsParent);
}

/* Checks if our calling node is a child of input, i.e. possible parent owns our edge
 * TODO: add our check to make sure that our child is "seen" as a child by the parent and vice-versa.
 */
template<class T>
bool Node<T>::isChild(std::shared_ptr<Node<T>> possibleParent)
{
	unsigned int count = 0;
	for (auto const &inEdge : this->inEdges)
	{
		if (inEdge->getSourceNode().get() == possibleParent.get())
		{
			count++;
			if (nodeDebug)
			{
				std::string foundMsg = this->getName() + " is child of "
						+ possibleParent.get()->getName() + " count = "
						+ std::to_string(count);
				lazyInfo(__LINE__, __func__, foundMsg);
			}
		}
	}
	//ensure only child one time, prevent multiple edges to same
	if (nodeDebug)
	{
		if (count > 1)
		{
			std::string badMsg = "Our parent is seen <" + std::to_string(count)
					+ "> times";
			badBehavior(__LINE__, __func__, badMsg);
		}
		std::string isChildMsg = this->getName() + " -is child of- "
				+ possibleParent.get()->getName();
		lazyInfo(__LINE__, __func__, isChildMsg);
		std::cout << "Childs edges:\n";
		edgeCheck();
		std::cout << "Parents edges:\n";
		possibleParent.get()->edgeCheck();
	}
	return (count == 1) ? true : false;
}

/* Checks if calling node is parent of input i.e. calling node owns the edge
 * TODO: add our check to make sure that our child is "seen" as a child by the parent and vice-versa.
 */
template<class T>
bool Node<T>::isParent(std::shared_ptr<Node<T>> possibleChild)
{
	unsigned int count = 0;
	//just forcing to make sure I am understanding that this is what auto const& becomes
	for (std::unique_ptr<Edge<T>> const &outEdge : this->outEdges)
	{
		if (outEdge.get()->getSinkNode().get() == possibleChild.get())
		{
			count++;
			if (nodeDebug)
			{
				std::string foundMsg = this->getName() + " is parent of "
						+ possibleChild.get()->getName() + " count = "
						+ std::to_string(count);
				lazyInfo(__LINE__, __func__, foundMsg);
			}
		}
	}
	if (nodeDebug)
	{
		if (count > 1)
		{
			std::string badMsg = "Our child is seen <" + std::to_string(count)
					+ "> times";
			badBehavior(__LINE__, __func__, badMsg);
		}
		std::string isParentMsg = this->getName() + " -is parent of- "
				+ possibleChild.get()->getName();
		lazyInfo(__LINE__, __func__, isParentMsg);
		std::cout << "~ Parents edges:\n";
		edgeCheck();
		std::cout << "~ Childs edges:\n";
		possibleChild.get()->edgeCheck();
		edgeCheck();
	}
	return (count == 1) ? true : false;
}

template<class T>
bool Node<T>::containsLabel(std::string labelToCheck)
{
	return (std::find(this->labels.begin(), this->labels.end(), labelToCheck)
			!= this->labels.end()) ? true : false;
}

template<class T>
bool Node<T>::containsLabel(std::vector<std::string> labelsToCheck)
{
	for (std::string currLabel : labelsToCheck)
		return this->containsLabel(currLabel);
	return false;
}

/************************************************
 *  MUTATORS
 ***********************************************/

template<class T>
void Node<T>::addLabel(std::string label)
{
	this->labels.push_back(label);
}

template<class T>
void Node<T>::addLabel(std::vector<std::string> labels)
{
	for (std::string currLabel : labels)
		this->addLabel(currLabel);
}

/************************************************
 *  GETTER/SETTER PAIRS
 ***********************************************/

template<class T>
void Node<T>::addNeighbor(std::shared_ptr<Node<T> > neighborToAdd,
		std::string edgeName)
{
	if (this->isNeighbor(neighborToAdd))
	{
		badBehavior(__LINE__, __func__, "Warning, already neighbors!");
		return;
	}
	else if (this == neighborToAdd.get())
	{
		badBehavior(__LINE__, __func__,
				"Warning, trying to add self as neighbor!");
		return;
	}
	this->outEdges.push_back(
			std::make_unique<Edge<T>>(edgeName, this->shared_from_this(),
					neighborToAdd));
	neighborToAdd.get()->inEdges.push_back(this->outEdges.back().get());

}

template<class T>
void Node<T>::deleteNeighbor(std::shared_ptr<Node<T>> neighborToRemove)
{
	std::cout << "NOT IMPLEMENTED PROBABLY DONT NEED" << std::endl;
}

template<class T>
void Node<T>::deleteEdge(std::shared_ptr<Node<T> > secondNode)
{
	//once we make sure our structure is always maintained we will just use the source & sink ptr from our edge!
	Edge<T> *connectingEdge = this->getConnectingEdge(secondNode);
	lazyInfo(__LINE__, __func__,
			"ENTERING OUR DELETE EDGE ~~~~~~~~~~~~~~~~~~~");
	edgeCheck();
	if (connectingEdge != NULL)
	{
		//delete our sink that is goign to our node, prevent dangling ptr
		connectingEdge->getSinkNode().get()->deleteInEdge(connectingEdge);
		//after ensure no dangle we now delete other side which kills whole edge
		connectingEdge->getSourceNode().get()->deleteOutEdge(connectingEdge);
	}
	else
	{
		std::string badMsg = "We could not find connecting edge between node ("
				+ this->getName() + ") and node ("
				+ secondNode.get()->getName();
		badBehavior(__LINE__, __func__, badMsg);
	}

}

//TODO: Make more fragile to enforce checks, could just get neighbors, get edges between, call delete on list of edges
template<class T>
void Node<T>::deleteAllEdges()
{

	/*
	 * for (auto const &inEdge : this->inEdges)
	{
		if (possibleInEdge == inEdge.get)
			count++;
	}
	 */
	std::vector<std::weak_ptr<Node<T>>> allNeighbors = this->getNeighbors();
	for (auto& neigh : allNeighbors)
	{
		if (neigh.lock())
		{
			this->deleteEdge(neigh.lock());
		}
		else
		{
			badBehavior(__LINE__, __func__, "COULDNT LOCK DELETING ALL");
		}
	}
	//deletes all the edges to our parents
	lazyInfo(__LINE__, __func__, "Beginning");
	edgeCheck();

	/*
	std::cout << std::endl << "god have mercy seg faulting" <<std::endl;
	for (auto &inEdge: this->inEdges)
	{

		//should be "this" node
		inEdge->getSinkNode().get()->deleteInEdge(inEdge);
		//should be "those" nodes
		inEdge->getSourceNode().get()->deleteOutEdge(inEdge);
	}
	std::cout << "\n\tAfter deleting in our edges for node: " << this->getName() <<std::endl;
	edgeCheck();
	//deletes all edges to our children
	//now gives a segfault
	for (std::unique_ptr<Edge<T>> &outEdge : this->outEdges)
	{
		//WARNING SEG FAULT
		//trying to brute force
		Edge<T>* tempEdge = outEdge.get();
		std::cout << "EGHHH: " << tempEdge << std::endl;
		std::cout << "EGHHH: " << tempEdge->getSourceNode().get() << std::endl;
		std::cout << "EGHHH: " << tempEdge->getSinkNode().get() << std::endl<<std::endl;
		tempEdge->getSinkNode().get()->deleteInEdge(tempEdge);
		std::shared_ptr<Node<T>> tempSourceNode = tempEdge->getSourceNode();
		//tempSourceNode.get()->deleteOutEdge(tempEdge);
		//tempEdge->getSourceNode().get()->deleteOutEdge(tempEdge);
		//outEdge.get()->getSinkNode().get()->deleteInEdge(outEdge.get());
		//outEdge.get()->getSourceNode().get()->deleteOutEdge(outEdge.get());
		//outEdge.get()->getSinkNode()
		//outEdge.get()->getSinkNode().get()->deleteInEdge(outEdge.get());
		//outEdge.get()->getSourceNode().get()->deleteOutEdge(outEdge.get());
	}
	std::cout << "\n\tAfter deleting out our edges for node: " << this->getName() <<std::endl;
	edgeCheck();
	*/
}

template<class T>
void Node<T>::switchEdgeDirection(std::shared_ptr<Node<T> > nodeB)
{


}

/************************************************
 *  HELPER FUNCTIONS
 ***********************************************/

//delete an edge in the node inEdges dealio. TODO: Warning - we must delete our in edge (raw ptr) before the out edge (unique_ptr)
template<class T>
void Node<T>::deleteInEdge(Edge<T> *inEdgeToDelete)
{
	lazyInfo(__LINE__, __func__, "Removing an incomming edge");
	edgeCheck();
	this->inEdges.erase(
			std::remove(this->inEdges.begin(), this->inEdges.end(),
					inEdgeToDelete), this->inEdges.end());
	//this->inEdges.erase(
	//		std::remove(this->inEdges.begin(), this->inEdges.end(),
	//				inEdgeToDelete), this->inEdges.end);
	std::cout << "After removal of in edges\n";
	edgeCheck();
}

template<class T>
void Node<T>::deleteOutEdge(Edge<T> *outEdgeToDelete)
{
	lazyInfo(__LINE__, __func__, "Removing an outgoing edge");
	edgeCheck();
	//hopefully this works https://www.fluentcpp.com/2018/09/18/how-to-remove-pointers-from-a-vector-in-cpp/
	//this->outEdges.erase(std::remove_if(this->outEdges.begin(), this->outEdges.end(), [](std::unique_ptr<Edge<T>> const &itEdge)
	//		{})
//https://stackoverflow.com/questions/33202941/removing-a-unique-ptr-of-an-object-from-a-vector-by-an-attribute-value
	/*	this->outEdges.erase(
	 std::remove_if(this->outEdges.begin(), this->outEdges.end(),
	 [](std::unique_ptr<Edge<T>> const &itEdge)
	 {
	 if (itEdge.get() == outEdgeToDelete)
	 {
	 return true;
	 }
	 else
	 return false;
	 }), this->outEdges.end());
	 */
	//this->outEdges.erase(std::remove_if(this->outEdges.begin(), this->outEdges.end(),[](std::unique_ptr<Edge<T>> const &itEdge) -> bool
	//		{
	//possibly use https://stackoverflow.com/questions/15125631/best-way-to-delete-a-stdunique-ptr-from-a-vector-with-a-raw-pointer
	//		return itEdge.get() == outEdgeToDelete;
	//		})),  this->outEdges.end());
	//cant care less at this point, we are going to move all to null ptr after a lazy check to make sure theerea are not null ptrs then we remove the null ptrs. Dunno if this will work, need to try using an iterator but gets grumpy
	for (auto &curPtr : this->outEdges)
	{
		if (curPtr.get() == outEdgeToDelete)
		{
			curPtr.reset();
		}
	}
	this->outEdges.erase(
			std::remove(this->outEdges.begin(), this->outEdges.end(), nullptr),
			this->outEdges.end());
	std::cout << "After removal of outEdge\n";
	edgeCheck();
}

//eventually get rid of these stupid checks
template<class T>
bool Node<T>::hasInEdge(Edge<T> *possibleInEdge)
{
	int count = 0;
	for (auto const &inEdge : this->inEdges)
	{
		if (possibleInEdge == inEdge.get)
			count++;
	}
	//lazy check
	if (count > 1)
	{
		badBehavior(__LINE__, __func__,
				"Warning, our in edge count is higher than 1!");
		return false;
	}
	return (count == 1) ? true : false;
}

template<class T>
bool Node<T>::hasOutEdge(Edge<T> *possibleOutEdge)
{
	/*
	 * std::unique_ptr<Edge<T>> const &outEdge
	 */
	int count = 0;
	for (auto const &outEdge : this->outEdges)
	{
		if (possibleOutEdge == outEdge.get())
			count++;
	}
	//lazy check
	if (count > 1)
	{
		badBehavior(__LINE__, __func__,
				"Warning, our out edge count is higher than 1!");
		return false;
	}
	return (count == 1) ? true : false;
}

#endif /* INC_NODE_H_ */

