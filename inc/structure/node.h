/* NOTE: We need to know our "owning" graphs in order to delete ourself from them when we completely get removed
 * 			How do we know our "parents"? Know up? Raw ptr makes sense but needs so much more logic....
 *
 */

#ifndef INC_STRUCTURE_NODE_H_
#define INC_STRUCTURE_NODE_H_

#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>

#include "../lazyPrints.h"

//probably gonna need to full include
template<class T> class Edge;
template<class T, class E> class Graph;

const bool nodeDebug = true;

//switching that will let us use more verbose checks to ensure our structure is maintained
const bool nodeVerbose = true;

//TODO: Figure out how to proper and quickly do our hashing
int graphHash = 100;

template<class T>
class Node: public std::enable_shared_from_this<Node<T>>
{
public:
	/************************************************
	 *  CONSTRUCTORS/DESTRUCTORS
	 ***********************************************/
	Node();
	Node(std::string name);

	~Node();

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

	std::vector<std::weak_ptr<Node<T>>> getNeighbors();
	std::vector<std::weak_ptr<Node<T>>> getChildren();
	std::vector<std::weak_ptr<Node<T>>> getParents();

	/************************************************
	 *  MUTATORS
	 ***********************************************/
	void addLabel(std::string label);
	void addLabel(std::vector<std::string> labels);

	void addChild(std::string edgeName, std::shared_ptr<Node<T>> freshChild);
	void addParent(std::string edgeName, std::shared_ptr<Node<T>> freshParent);

	void deleteEdgesToChild(std::shared_ptr<Node<T>> child);
	void deleteEdgesToParent(std::shared_ptr<Node<T>> parent);
	void deleteEdges(std::shared_ptr<Node<T>> nodeB);

	void deleteEdges();

	/************************************************
	 *  STRUCTURAL/RELATIONSHIP CHECKS/CHANGES/GETS
	 ***********************************************/
	bool isChild(std::shared_ptr<Node<T>> possibleParent);
	bool isParent(std::shared_ptr<Node<T>> possibleChild);
	bool isNeighbor(std::shared_ptr<Node<T>> possibleNeighbor);

	bool containsLabel(std::string labelToCheck);
	bool containsLabel(std::vector<std::string> labelToCheck); //Do we need?

	//POSSIBLY CHANGE THIS TO PRIVATE HELPER FUNCTIONS. AS OF NOW KEEP PUBLIC AND TRY NOT TO USE OUTSIDE
	std::vector<Edge<T>*> getConnectingEdges(std::shared_ptr<Node<T>> nodeB);
	std::vector<Edge<T>*> getInConnectingEdges(std::shared_ptr<Node<T>> nodeB);
	std::vector<Edge<T>*> getOutConnectingEdges(std::shared_ptr<Node<T>> nodeB);

	/************************************************
	 *  HELPING FUNCTIONS
	 ***********************************************/
	std::string edgesAsString();

	/* BELOW ARE FUNCTIONS THAT HAVE BEEN REMOVED BUT MAY BE ADDED AGAIN
	 *
	 */
	//std::weak_ptr<Node<T>> getNeighborByName(std::string name);
	//std::weak_ptr<Node<T>> getNeighborByIndex(unsigned short index);
	//void deleteEdge(std::shared_ptr<Node<T>> secondnode, Edge<T> *edgeToRemove); //delete edge to specific neighbor
private:
	/************************************************
	 *  IDENTIFIERS/INFORMATION
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

	/* TODO: How do we know about our owning graph? The issue
	 * 			is I do not want to have to deal with a template
	 * 			with 2 types instead fo 1. Seems impossible as of now.
	 *
	 * 			Below is how I am hoping to implement our multi-graph ownership.
	 * 			Will be uncommented and filled out in a different branch.
	 *
	 * 			We must know what graph structures we belong to in order to properly delete
	 * 			ourself. This way we do not have to do any checks like previously done.
	 */
	//std::vector<std::weak_ptr<int>> owningGraphs; //do not use weak_ptr, other options?
	//std::unordered_map<std::weak_ptr<int>, std::vector<std::unique_ptr<Edge<T>>>> graphSpecifcOutEdges; //this is an unordered map using some type of value that signifies the graph, note that every graph in here must be present in owningGraphs but not vice versa.
	//std::unordered_map<std::weak_ptr<int>, std::vector<Edge<T>* >> graphSpecificInEdges;
	/************************************************
	 *  HELPER FUNCTIONS
	 ***********************************************/
	void deleteInEdge(Edge<T> *inEdgeToDelete);
	void deleteOutEdge(Edge<T> *outEdgeToDelete);

	bool hasInEdge(Edge<T> *possibleInEdge);
	bool hasOutEdge(Edge<T> *possibleOutEdge);

	//Currently testing in order to ensure efficacy
	bool equalEdgeContents(std::vector<Edge<T>*> vec1,
			std::vector<Edge<T>*> vec2)
	{
		std::sort(vec1.begin(), vec1.end());
		std::sort(vec2.begin(), vec2.end());
		return vec1 == vec2;
	}
};

/************************************************
 *  CONSTRUCTORS/DESTRUCTORS
 ***********************************************/

template<class T>
Node<T>::Node()
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

template<class T>
Node<T>::Node(std::string name)
{
	this->setName(name);
	this->setIndex(1);
	this->setIsLeaf(false);
	this->setIsBridge(false);
	this->setIsVisited(false);
	if (nodeDebug)
	{
		std::string conMsg = "Constructed a node with name: " + this->getName();
		lazyInfo(__LINE__, __func__, conMsg);
	}
}

template<class T>
Node<T>::~Node()
{
	if (nodeDebug)
	{
		std::string delMsg = "Removing node with name: " + this->getName();
		lazyInfo(__LINE__, __func__, delMsg);
	}
	//We will have to delete ourself from all of the graphs we are contained within if we are explicitly deleted, this will come in future
	this->deleteEdges();
}

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
	return index;
}

template<class T>
void Node<T>::setName(std::string name)
{
	this->name = name;
}

template<class T>
std::string Node<T>::getName() const
{
	return name;
}

template<class T>
void Node<T>::setLabels(std::vector<std::string> labels)
{
	this->labels = labels;
}

template<class T>
std::vector<std::string> Node<T>::getLabels() const
{
	return labels;
}

template<class T>
void Node<T>::setIsLeaf(bool leaf)
{
	this->leaf = leaf;
}

template<class T>
bool Node<T>::getIsLeaf() const
{
	return leaf;
}

template<class T>
void Node<T>::setIsBridge(bool bridge)
{
	this->bridge = bridge;
}

template<class T>
bool Node<T>::getIsBridge() const
{
	return bridge;
}

template<class T>
void Node<T>::setIsVisited(bool visited)
{
	this->visited = visited;
}

template<class T>
bool Node<T>::getIsVisited() const
{
	return visited;
}

template<class T>
std::vector<std::weak_ptr<Node<T> > > Node<T>::getNeighbors()
{
	std::vector<std::weak_ptr<Node<T>>> children = this->getChildren();
	std::vector<std::weak_ptr<Node<T>>> parents = this->getParents();
	children.insert(children.end(), parents.begin(), parents.end());
	if (nodeDebug && (children.size() == 0))
		lazyInfo(__LINE__, __func__,
				"Returned vector of neighbors is of size 0");
	return children;
}

template<class T>
std::vector<std::weak_ptr<Node<T> > > Node<T>::getChildren()
{
	std::vector<std::weak_ptr<Node<T>>> children;
	for (std::unique_ptr<Edge<T>> const &outEdge : this->outEdges)
		children.push_back(outEdge->getSinkNode());
	if (nodeDebug && (children.size() == 0))
		lazyInfo(__LINE__, __func__,
				"Returned vector of children is of size 0");
	return children;
}

template<class T>
std::vector<std::weak_ptr<Node<T> > > Node<T>::getParents()
{
	std::vector<std::weak_ptr<Node<T>>> parents;
	for (Edge<T> *inEdge : this->inEdges)
		parents.push_back(inEdge->getSourceNode());
	if (nodeDebug && (parents.size() == 0))
		lazyInfo(__LINE__, __func__, "Returned vector of parents is of size 0");
	return parents;
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

template<class T>
void Node<T>::addChild(std::string edgeName,
		std::shared_ptr<Node<T> > freshChild)
{
	if (nodeDebug)
	{
		std::string tempMsg = "Adding (" + freshChild.get()->getName()
				+ ") as child\n";
		std::string edgesS = this->edgesAsString();
		lazyInfo(__LINE__, __func__, tempMsg + edgesS);
	}
	if (this == freshChild.get())
	{
		std::string badMsg = "Warning, we are trying to add self as a child.";
		badBehavior(__LINE__, __func__, badMsg);
		return;
	}
	this->outEdges.push_back(
			std::make_unique<Edge<T>>(edgeName, this->shared_from_this(),
					freshChild));
	//probably should worry about popping last out, most likely should use a temp then move it
	freshChild.get()->inEdges.push_back(this->outEdges.back().get());
	if (nodeDebug)
	{
		std::string tempMsg = "Added (" + freshChild.get()->getName()
				+ ") as child\n";
		std::string edgesS = this->edgesAsString();
		lazyInfo(__LINE__, __func__, tempMsg + edgesS);
	}
}

template<class T>
void Node<T>::addParent(std::string edgeName,
		std::shared_ptr<Node<T> > freshParent)
{
	if (nodeDebug)
	{
		std::string tempMsg = "Adding (" + freshParent.get()->getName()
				+ ") as parent\n";
		std::string edgesS = this->edgesAsString();
		lazyInfo(__LINE__, __func__, tempMsg + edgesS);
	}
	if (this == freshParent.get())
	{
		std::string badMsg = "Warning, we are trying to add self as a parent.";
		badBehavior(__LINE__, __func__, badMsg);
		return;
	}
	freshParent.get()->outEdges.push_back(
			std::make_unique<Edge<T>>(edgeName, freshParent,
					this->shared_from_this()));
	this->inEdges.push_back(freshParent.get()->outEdges.back().get());
	if (nodeDebug)
	{
		std::string tempMsg = "Added (" + freshParent.get()->getName()
				+ ") as parent\n";
		std::string edgesS = this->edgesAsString();
		lazyInfo(__LINE__, __func__, tempMsg + edgesS);
	}

}

template<class T>
void Node<T>::deleteEdgesToChild(std::shared_ptr<Node<T> > child)
{
	if (nodeVerbose)
	{
		std::vector<Edge<T>*> outConEdges = this->getOutConnectingEdges(child);
		std::vector<Edge<T>*> inConnodeB = child.get()->getInConnectingEdges(
				this->shared_from_this());
		bool niceOne = true;
		//one way of checking our vecs are equal
		for (Edge<T> *const outEdge : outConEdges)
		{
			if (std::find(inConnodeB.begin(), inConnodeB.end(), outEdge)
					== inConnodeB.end())
				niceOne = false;
		}
		//another way of checking vecs are equal
		bool nice1 = this->equalEdgeContents(inConnodeB, outConEdges);
		if (niceOne == nice1) //ensure 2 methods constantly put out same
		{
			if (niceOne)
			{
				for (Edge<T> *const toDelete : outConEdges)
				{
					child.get()->deleteInEdge(toDelete);
					this->deleteOutEdge(toDelete);
				}
			}
			else
			{
				std::string badMsg =
						"Our connecting out and in edges don't match!";
				badBehavior(__LINE__, __func__, badMsg);
			}
		}
		else
		{
			std::string badMsg =
					"Warning: Our 2 equality methods did not return same value!";
			badBehavior(__LINE__, __func__, badMsg);
		}
	}
	else
	{
		std::vector<Edge<T>*> outConEdges = this->getOutConnectingEdges(child);
		for (Edge<T> *const toDelete : outConEdges)
		{
			child.get()->deleteInEdge(toDelete);
			this->deleteOutEdge(toDelete);
		}
	}
}

template<class T>
void Node<T>::deleteEdgesToParent(std::shared_ptr<Node<T> > parent)
{
	if (nodeVerbose)
	{
		std::vector<Edge<T>*> inConEdges = this->getInConnectingEdges(parent);
		std::vector<Edge<T>*> outConnodeB = parent.get()->getOutConnectingEdges(
				this->shared_from_this());
		bool niceOne = true;
		for (auto const &inEdge : inConEdges)
		{
			if (std::find(outConnodeB.begin(), outConnodeB.end(), inEdge)
					== outConnodeB.end())
				niceOne = false;
		}
		//another way of checking vecs are equal
		bool nice1 = this->equalEdgeContents(outConnodeB, inConEdges);
		if (niceOne == nice1) //ensure 2 methods constantly put out same
		{
			if (niceOne)
			{
				for (Edge<T> *const toDelete : outConnodeB)
				{
					this->deleteInEdge(toDelete);
					parent.get()->deleteOutEdge(toDelete);
				}
			}
			else
			{
				std::string badMsg =
						"Our connecting out and in edges don't match!";
				badBehavior(__LINE__, __func__, badMsg);
			}
		}
		else
		{
			std::string badMsg =
					"Warning: Our 2 equality methods did not return same value!";
			badBehavior(__LINE__, __func__, badMsg);
		}

	}
	else
	{
		std::vector<Edge<T>*> outConnodeB = parent.get()->getOutConnectingEdges(
				this->shared_from_this());
		for (Edge<T> *const toDelete : outConnodeB)
		{
			this->deleteInEdge(toDelete);
			parent.get()->deleteOutEdge(toDelete);
		}
	}
}

template<class T>
void Node<T>::deleteEdges(std::shared_ptr<Node<T> > nodeB)
{
	if (nodeDebug)
	{
		std::string cEdgesS = this->edgesAsString();
		std::string bEdgesS = nodeB.get()->edgesAsString();

		std::string tempMsg = "Entering function deleting edges between node ("
				+ this->getName() + ") and (" + nodeB.get()->getName() + ")\n";
		lazyInfo(__LINE__, __func__, tempMsg + cEdgesS + "\n" + bEdgesS);
	}
	if (this->isNeighbor(nodeB))
	{
		this->deleteEdgesToChild(nodeB);
		this->deleteEdgesToParent(nodeB);
	}
	else
	{
		std::string badMsg = "Warning: our two nodes (" + this->getName()
				+ ") and (" + nodeB.get()->getName() + ") are not neighbors.";
		badBehavior(__LINE__, __func__, badMsg);
		if (nodeDebug)
		{
			std::string cEdgesS = this->edgesAsString();
			std::string bEdgesS = nodeB.get()->edgesAsString();
			std::string tempMsg = "Bad time trying to deleted edges between ("
					+ this->getName() + ") and (" + nodeB.get()->getName()
					+ ")\n";
			lazyInfo(__LINE__, __func__, tempMsg + cEdgesS + "\n" + bEdgesS);
		}
	}
	if (nodeDebug)
	{
		std::string cEdgesS = this->edgesAsString();
		std::string bEdgesS = nodeB.get()->edgesAsString();

		std::string tempMsg = "Leaving function deleting edges between node ("
				+ this->getName() + ") and (" + nodeB.get()->getName() + ")\n";
		lazyInfo(__LINE__, __func__, tempMsg + cEdgesS + "\n" + bEdgesS);
	}
}

template<class T>
void Node<T>::deleteEdges()
{
	if (nodeDebug)
	{
		std::string edgesS = this->edgesAsString();
		lazyInfo(__LINE__, __func__,
				"Removing all edges around node (" + this->getName() + ")\n"
						+ edgesS);
	}
	std::vector<std::weak_ptr<Node<T>>> allNeighbors = this->getNeighbors();
	if (allNeighbors.size() > 0)
	{
		for (std::weak_ptr<Node<T>> neigh : allNeighbors)
		{
			if (neigh.lock())
			{
				std::shared_ptr<Node<T>> tempN = neigh.lock();
				if (nodeDebug)
				{
					std::string infoMsg =
							"Note that we do not care about direction in this case\n\tDeleting edge connecting node ("
									+ this->getName() + ") and node ("
									+ tempN.get()->getName() + ")";
					lazyInfo(__LINE__, __func__, infoMsg);
				}
				this->deleteEdges(tempN);
			}
		}
	}
	else
	{
		badBehavior(__LINE__, __func__, "Neighbors vector has size < 0");
		return;
	}
	if (nodeDebug)
	{
		std::string edgesS = this->edgesAsString();
		lazyInfo(__LINE__, __func__,
				"Removed all edges around node (" + this->getName() + ")\n"
						+ edgesS);
	}
}

/************************************************
 *  STRUCTURAL/RELATIONSHIP CHECKS/CHANGES/GETS
 ***********************************************/

template<class T>
bool Node<T>::isChild(std::shared_ptr<Node<T> > possibleParent)
{
	for (Edge<T> *const inEdge : this->inEdges)
	{
		if (inEdge->getSourceNode().get() == possibleParent.get())
			return true;
	}
	return false;
}

template<class T>
bool Node<T>::isParent(std::shared_ptr<Node<T> > possibleChild)
{
	for (std::unique_ptr<Edge<T>> const &outEdge : this->outEdges)
	{
		if (outEdge.get()->getSinkNode().get() == possibleChild.get())
			return true;
	}
	return false;
}

template<class T>
bool Node<T>::isNeighbor(std::shared_ptr<Node<T> > possibleNeighbor)
{
	//Do we want to worry about granularity? Something can be both parent and child if either a cycle of a self-pointing edge
	return (this->isChild(possibleNeighbor) || this->isParent(possibleNeighbor));
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

/* NOTE: WOULD WE LIKE TO EVENTUALLY HIDE THESE AWAY AS HELPER FUNCTIONS?
 *			TODO: finish it
 */
template<class T>
std::vector<Edge<T>*> Node<T>::getConnectingEdges(
		std::shared_ptr<Node<T> > nodeB)
{
	if (nodeVerbose)
	{
		std::vector<Edge<T>*> inThis = this->getInConnectingEdges(nodeB);
		std::vector<Edge<T>*> outThis = this->getOutConnectingEdges(nodeB);
		std::vector<Edge<T>*> innodeB = nodeB.get()->getInConnectingEdges(
				this->shared_from_this());
		std::vector<Edge<T>*> outnodeB = nodeB.get()->getOutConnectingEdges(
				this->shared_from_thiss());

	}
	else
	{

	}
}

template<class T>
std::vector<Edge<T>*> Node<T>::getInConnectingEdges(
		std::shared_ptr<Node<T> > nodeB)
{
	std::vector<Edge<T>*> inConEdges;
	for (Edge<T>* const inEdge : this->inEdges)
	{
		if (inEdge->getSourceNode().get() == nodeB.get())
			inConEdges.push_back(inEdge);
	}
	return inConEdges;
}

template<class T>
std::vector<Edge<T>*> Node<T>::getOutConnectingEdges(
		std::shared_ptr<Node<T> > nodeB)
{
	std::vector<Edge<T>*> outConEdges;
	for (std::unique_ptr<Edge<T>> const &outEdge : this->outEdges)
	{
		if (outEdge.get()->getSinkNode().get() == nodeB.get())
			outConEdges.push_back(outEdge.get());
	}
	return outConEdges;
}

/************************************************
 *  HELPING FUNCTIONS
 ***********************************************/
template<class T>
std::string Node<T>::edgesAsString()
{
	std::string edgesString = "";
	edgesString = "========= EDGE CHECK: " + this->getName()
			+ " =========\n\tIn Edges Size: "
			+ std::to_string(this->inEdges.size()) + "\n\t";
	for (Edge<T> *const inEdge : this->inEdges)
		edgesString += inEdge->getName() + ", ";
	edgesString += "\n\tOut Edges Size: "
			+ std::to_string(this->outEdges.size()) + "\n\t";
	for (std::unique_ptr<Edge<T>> const &outEdge : this->outEdges)
		edgesString += outEdge.get()->getName() + ", ";
	edgesString += "\n";
	return edgesString;
}

/************************************************
 *  HELPER FUNCTIONS
 ***********************************************/
template<class T>
void Node<T>::deleteInEdge(Edge<T> *inEdgeToDelete)
{
	if (nodeDebug)
	{
		std::string edgesS = this->edgesAsString();
		lazyInfo(__LINE__, __func__, "Entering function\n" + edgesS);
	}
	if (this->hasInEdge(inEdgeToDelete))
	{
		this->inEdges.erase(
				std::remove(this->inEdges.begin(), this->inEdges.end(),
						inEdgeToDelete), this->inEdges.end());
	}
	else
	{
		std::string badMsg = "Warning: inEdge (" + inEdgeToDelete->getName()
				+ ") does not exist in node: (" + this->getName() + ")";
		badBehavior(__LINE__, __func__, badMsg);
		return;
	}
	if (nodeDebug)
	{
		std::string edgesS = this->edgesAsString();
		lazyInfo(__LINE__, __func__, "Leaving function\n" + edgesS);
	}
}

template<class T>
void Node<T>::deleteOutEdge(Edge<T> *outEdgeToDelete)
{
	if (nodeDebug)
	{
		std::string edgesS = this->edgesAsString();
		lazyInfo(__LINE__, __func__, "Entering function\n" + edgesS);
	}
	if (this->hasOutEdge(outEdgeToDelete))
	{
		for (std::unique_ptr<Edge<T>> &outEdge : this->outEdges)
		{
			if (outEdge.get() == outEdgeToDelete)
				outEdge.reset();
		}
		this->outEdges.erase(
				std::remove(this->outEdges.begin(), this->outEdges.end(),
						nullptr), this->outEdges.end());
	}
	else
	{
		std::string badMsg = "Warning: outEdge (" + outEdgeToDelete->getName()
				+ ") does not exist in node: (" + this->getName() + ")";
		badBehavior(__LINE__, __func__, badMsg);
	}
	if (nodeDebug)
	{
		std::string edgesS = this->edgesAsString();
		lazyInfo(__LINE__, __func__, "Leaving function\n" + edgesS);
	}
}

template<class T>
bool Node<T>::hasInEdge(Edge<T> *possibleInEdge)
{
	int count = 0;
	for (Edge<T> *const inEdge : this->inEdges)
	{
		if (inEdge == possibleInEdge)
			count++;
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
	int count = 0;
	for (std::unique_ptr<Edge<T>> const &outEdge : this->outEdges)
	{
		if (possibleOutEdge == outEdge.get())
			count++;
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

#endif /* INC_STRUCTURE_NODE_H_ */
