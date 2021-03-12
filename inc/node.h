/*
 * TODO:
 * 		- use unordered set
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
			std::string delMsg = "Removing node with name: " + this->getName();
			lazyInfo(__LINE__, __func__, delMsg);
		}
		this->deleteAllEdges();

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

	//add neighbor as a child
	void addNeighbor(std::shared_ptr<Node<T>> neighborToAdd,
			std::string edgeName);

	//delete a specific edge, will want to take in an edge ptr
	void deleteEdge(std::shared_ptr<Node<T>> secondNode, Edge<T> *edgeToRemove);

	//delete all edges from our calling node to nodeb
	void deleteAllEdgesToChild(std::shared_ptr<Node<T>> nodeB);
	//delete all edges to our calling node from nodeb, i.e. nodeB is parent
	void deleteAllEdgesToParent(std::shared_ptr<Node<T>> nodeB);
	//delete all edges between 2 nodes
	void deleteEdges(std::shared_ptr<Node<T>> nodeB);

	//delete all edges to and from calling node
	void deleteAllEdges();

	/************************************************
	 *  STRUCTURAL/RELATIONSHIP CHECKS
	 ***********************************************/
	bool isNeighbor(std::shared_ptr<Node<T>> possibleNeighbor);
	bool isChild(std::shared_ptr<Node<T>> possibleParent);
	bool isParent(std::shared_ptr<Node<T>> possibleChild);

	bool containsLabel(std::string labelToCheck);
	bool containsLabel(std::vector<std::string> labelToCheck);

	//TODO: Get rid of this garbage
	std::vector<Edge<T>*> getConnectingEdges(std::shared_ptr<Node<T>> nodeB);
	std::vector<Edge<T>*> getInConnectingEdges(std::shared_ptr<Node<T>> nodeB);
	std::vector<Edge<T>*> getOutConnectingEdges(std::shared_ptr<Node<T>> nodeB);

	void printEdges();

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
		std::cout << std::endl <<std::endl;
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

	//Warning O(nlogn) to run this each time, check our 2 vecs have the same contents order doesnt matter

	bool equalEdgeContents(std::vector<Edge<T>*> vec1,
			std::vector<Edge<T>*> vec2)
	{
		std::sort(vec1.begin(), vec1.end());
		std::sort(vec2.begin(), vec2.end());
		return vec1 == vec2;
	}
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

template<class T>
std::weak_ptr<Node<T>> Node<T>::getNeighborByName(std::string name)
{
}

template<class T>
std::weak_ptr<Node<T>> Node<T>::getNeighborByIndex(unsigned short index)
{
}

template<class T>
std::vector<std::weak_ptr<Node<T>>> Node<T>::getNeighbors()
{
	//since this is neighbors we want to remove our dupes. Dont care about ownership
	std::vector<std::weak_ptr<Node<T>>> children = this->getChildren();
	std::vector<std::weak_ptr<Node<T>>> parents = this->getParents();
	if (false)
	{
		if (children.size() <= 0)
			badBehavior(__LINE__, __func__, "Children vec size is 0");
		if (parents.size() <= 0)
			badBehavior(__LINE__, __func__, "Parent vec size is 0");
	}

	children.insert(children.end(), parents.begin(), parents.end());
	return children;
}

template<class T>
inline std::vector<std::weak_ptr<Node<T> > > Node<T>::getChildren()
{
	std::vector<std::weak_ptr<Node<T>>> children;
	for (std::unique_ptr<Edge<T>> const &outEdge : this->outEdges)
		children.push_back(outEdge->getSinkNode());
	return children;
}

template<class T>
std::vector<std::weak_ptr<Node<T> > > Node<T>::getParents()
{
	std::vector<std::weak_ptr<Node<T>>> parents;
	for (auto const &inEdge : this->inEdges)
		parents.push_back(inEdge->getSourceNode());
	return parents;
}

template<class T>
bool Node<T>::isNeighbor(std::shared_ptr<Node<T>> possibleNeighbor)
{
	return (this->isChild(possibleNeighbor) || this->isParent(possibleNeighbor));
}

template<class T>
bool Node<T>::isChild(std::shared_ptr<Node<T>> possibleParent)
{
	for (auto const &inEdge : this->inEdges)
	{
		if (inEdge->getSourceNode().get() == possibleParent.get())
			return true;
	}
	return false;
}

template<class T>
bool Node<T>::isParent(std::shared_ptr<Node<T>> possibleChild)
{
	for (std::unique_ptr<Edge<T>> const &outEdge : this->outEdges)
	{
		if (outEdge.get()->getSinkNode().get() == possibleChild.get())
			return true;
	}
	return false;
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

template<class T>
void Node<T>::addNeighbor(std::shared_ptr<Node<T> > neighborToAdd,
		std::string edgeName)
{
	if (this == neighborToAdd.get())
	{
		badBehavior(__LINE__, __func__,
				"Warning trying to add self as neighbor");
		return;
	}
	this->outEdges.push_back(
			std::make_unique<Edge<T>>(edgeName, this->shared_from_this(),
					neighborToAdd));
	neighborToAdd.get()->inEdges.push_back(this->outEdges.back().get());
}

//delete all our edges that are going in and out of calling node
template<class T>
void Node<T>::deleteAllEdges()
{
	if (nodeDebug)
		lazyInfo(__LINE__, __func__);
	std::vector<std::weak_ptr<Node<T>>> allNeighbors = this->getNeighbors();
	if (!(allNeighbors.size() > 0))
	{
		badBehavior(__LINE__, __func__, "Neighbors has size < 0");
		return;
	}
	for (auto &neigh : allNeighbors)
	{
		if (neigh.lock())
		{
			std::shared_ptr<Node<T>> tempNeigh = neigh.lock();
			if (nodeDebug)
			{
				std::string infoMsg =
						"Note that we do not care about direction in this case\n\tDeleting edge connecting node ("
								+ this->getName() + ") and node ("
								+ tempNeigh.get()->getName() + ")";
				lazyInfo(__LINE__, __func__, infoMsg);
			}
			this->deleteEdges(tempNeigh);
			//this->deleteEdges(tempNeigh);
		}
		else
		{
			badBehavior(__LINE__, __func__,
					"Warning, could not lock neighbor in deleting all");
		}
	}
}

template<class T>
inline void Node<T>::deleteEdge(std::shared_ptr<Node<T> > secondNode,
		Edge<T> *edgeToRemove)
{
}

//delete all edges between our two nodes
template<class T>
void Node<T>::deleteEdges(std::shared_ptr<Node<T> > nodeB)
{
	if (this->isNeighbor(nodeB))
	{
		this->deleteAllEdgesToChild(nodeB);
		this->deleteAllEdgesToParent(nodeB);
	}
	else
	{
		std::string badMsg = "Warning: our two nodes (" + this->getName()
				+ ") and (" + nodeB.get()->getName() + ") are not neighbors.";
		badBehavior(__LINE__, __func__, badMsg);

		//prints current edges
		std::cout << "\nNode (" + this->getName() + ") current edges\n";
		this->edgeCheck();
		std::cout << "\nNode (" + nodeB.get()->getName() + ") current edges\n";
		nodeB.get()->edgeCheck();
		std::cout << std::endl;
	}
}

//calling is parent, nodeB is child
template<class T>
void Node<T>::deleteAllEdgesToChild(std::shared_ptr<Node<T> > nodeB)
{
	std::vector<Edge<T>*> outConEdges = this->getOutConnectingEdges(nodeB);
	//TODO: Possibly bad
	std::vector<Edge<T>*> inConNodeB = nodeB.get()->getInConnectingEdges(
			this->shared_from_this());
	//ensure our 2 vectors are the same
	bool nice = true;
	for (auto const &outEdge : outConEdges)
	{
		if (std::find(inConNodeB.begin(), inConNodeB.end(), outEdge)
				== inConNodeB.end())
			nice = false;
	}

	//TODO: Ensure this works all the time
	//bool otherNice = equalEdgeContents(outConEdges, inConNodeB);

	if (!nice)
	{
		std::string badMsg = "Our connecting out and in edges dont match!";
		badBehavior(__LINE__, __func__);
	}
	else //nice
	{
		for (auto const &toDelete : outConEdges)
		{
			nodeB.get()->deleteInEdge(toDelete);
			this->deleteOutEdge(toDelete);
		}
	}

}

template<class T>
void Node<T>::deleteAllEdgesToParent(std::shared_ptr<Node<T> > nodeB)
{
	std::vector<Edge<T>*> inConEdges = this->getInConnectingEdges(nodeB);
	std::vector<Edge<T>*> outConNodeB = nodeB.get()->getOutConnectingEdges(
			this->shared_from_this());
	//ensure our 2 vectors are the same, these checks are redundant
	bool nice = true;
	for (auto const &inEdge : inConEdges)
	{
		if (std::find(outConNodeB.begin(), outConNodeB.end(), inEdge)
				== outConNodeB.end())
			nice = false;
	}
	if (!nice)
	{
		std::string badMsg = "Our connecting out and in edges dont match!";
		badBehavior(__LINE__, __func__, badMsg);
	}
	else //nice
	{
		for (auto const &toDelete : outConNodeB)
		{
			this->deleteInEdge(toDelete);
			nodeB.get()->deleteOutEdge(toDelete);
		}
	}
}

//previously we only assumed we had one edge, now we allow for multiple between two nodes.
template<class T>
std::vector<Edge<T>*> Node<T>::getConnectingEdges(
		std::shared_ptr<Node<T>> nodeB)
{
	std::vector<Edge<T>*> inThis = this->getInConnectingEdges(nodeB);
	std::vector<Edge<T>*> outThis = this->getOutConnectingEdges(nodeB);
	std::vector<Edge<T>*> inNodeB = nodeB.get()->getInConnectingEdges(
			this->shared_from_this());
	std::vector<Edge<T>*> outNodeB = nodeB.get()->getOutConnectingEdges(
			this->shared_from_this());
	//doing checks to make sure our nodes know what is attached to them
	bool niceOne = true;
	for (auto const &inThisEdge : inThis)
	{
		if (std::find(outNodeB.begin(), outNodeB.end(), inThisEdge)
				== outNodeB.end())
			niceOne = false;
	}
	if (!niceOne)
	{
		std::string badMsg = "Our connecting out and in edges dont match!";
		badBehavior(__LINE__, __func__, badMsg);
	}
	//other way
	bool nice1 = true;
	for (auto const &outThisEdge : outThis)
	{
		if (std::find(inNodeB.begin(), inNodeB.end(), outThisEdge)
				== inNodeB.end())
			nice1 = false;
	}
	if (!nice1)
	{
		std::string badMsg = "Our connecting out and in edges dont match!";
		badBehavior(__LINE__, __func__, badMsg);
	}

	if (nice1 && niceOne) //nice
	{

	}

}

//gets all connecting edges owned by callee, so we are checking all edges FROM nodeB
template<class T>
std::vector<Edge<T>*> Node<T>::getInConnectingEdges(
		std::shared_ptr<Node<T>> nodeB)
{
	std::vector<Edge<T>*> inConEdges;
	for (auto const &inEdge : this->inEdges)
	{
		if (inEdge->getSourceNode().get() == nodeB.get())
			inConEdges.push_back(inEdge);
	}
	return inConEdges;
}

//gets all connecting edges owned by our caller, so we are checking all edges FROM our caller TO our callee
template<class T>
std::vector<Edge<T>*> Node<T>::getOutConnectingEdges(
		std::shared_ptr<Node<T>> nodeB)
{
	std::vector<Edge<T>*> outConEdges;
	for (std::unique_ptr<Edge<T>> const &outEdge : this->outEdges)
	{
		if (outEdge.get()->getSinkNode().get() == nodeB.get())
			outConEdges.push_back(outEdge.get());
	}
	return outConEdges;
}

template<class T>
void Node<T>::printEdges()
{
	this->edgeCheck();
}

/************************************************
 *  HELPER FUNCTIONS
 ***********************************************/

//deletes an edge witin this nodes inEdge list
template<class T>
void Node<T>::deleteInEdge(Edge<T> *inEdgeToDelete)
{
	if (this->hasInEdge(inEdgeToDelete))
	{
		this->inEdges.erase(
				std::remove(this->inEdges.begin(), this->inEdges.end(),
						inEdgeToDelete), this->inEdges.end());
	}
	else
	{
		std::string badMsg = "Warning: inEdge (" + inEdgeToDelete->getName()
				+ ") does not exist in node: " + this->getName();
		badBehavior(__LINE__, __func__, badMsg);
	}
}

//deletes an edge within this nodes outEdges list. NOTE THAT YOU MUST DELETE THIS EDGE FROM THE SINK NODES INEDGES LIST BEFORE DELETING THE EDGE FROM THE OUTEDGE LIST DUE TO UNIQUE PTR
template<class T>
void Node<T>::deleteOutEdge(Edge<T> *outEdgeToDelete)
{
	if (this->hasOutEdge(outEdgeToDelete))
	{
		//first clear out our unique ptr aka destruct edge
		for (auto &outEdge : this->outEdges)
			if (outEdge.get() == outEdgeToDelete)
				outEdge.reset();
		this->outEdges.erase(
				std::remove(this->outEdges.begin(), this->outEdges.end(),
						nullptr), this->outEdges.end());
	}
	else
	{
		std::string badMsg = "Warning: outEdge (" + outEdgeToDelete->getName()
				+ ") does not exist in node: " + this->getName();
		badBehavior(__LINE__, __func__, badMsg);
	}
}

template<class T>
bool Node<T>::hasInEdge(Edge<T> *possibleInEdge)
{
	//used to make sure we have only one of the edges in our vec
	int count = 0;
	for (auto const &inEdge : this->inEdges)
	{
		if (possibleInEdge == inEdge)
			count++; //we have a hit, if this was unorded set we would use diff method
	}
	if (count > 1)
	{
		std::string badMsg = "Warning: inEdge (" + possibleInEdge->getName()
				+ ") present multiple times in (" + this->getName() + ")";
		badBehavior(__LINE__, __func__, badMsg);
		return false;
	}
	return (count == 1) ? true : false;
}

template<class T>
bool Node<T>::hasOutEdge(Edge<T> *possibleOutEdge)
{
	//used to make sure we have only one of the edges in our vec
	int count = 0;
	for (auto const &outEdge : this->outEdges)
	{
		if (possibleOutEdge == outEdge.get())
			count++; //we have a hit, if this was unorded set we would use diff method
	}
	if (count > 1)
	{
		std::string badMsg = "Warning: outEdge (" + possibleOutEdge->getName()
				+ ") present multiple times in (" + this->getName() + ")";
		badBehavior(__LINE__, __func__, badMsg);
		return false;
	}
	return (count == 1) ? true : false;
}

#endif /* INC_NODE_H_ */

