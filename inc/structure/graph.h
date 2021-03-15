/* TODO: Eventually change all of our altering functions so node-based functions
 * 			take in the value from the graph that we will want to hash and use
 * 			as a key to denote which edge is part of what graph.
 *
 *		Note that we will eventually remove refreshContaining() from everywhere
 *		once we incorperate something that allows us to delete nodes from their
 *		containing graphs also. Unsure of how to reference "up".
 *
 */

#ifndef INC_GRAPH_H_
#define INC_GRAPH_H_

#include <vector>
#include <memory>
#include <unordered_set>

#include "../lazyPrints.h"

const bool graphDebug = true;

template<class E> class Node;

template<class T, class E>
class Graph
{
public:
	/************************************************
	 *  CONSTRUCTORS/DESTRUCTORS
	 ***********************************************/
	Graph();
	Graph(std::string name);

	~Graph();

	/************************************************
	 *  GETTER/SETTER PAIRS
	 ***********************************************/
	void setIndex(unsigned short int index);
	unsigned short int getIndex() const;

	void setName(std::string name);
	std::string getName() const;

	void setLabels(std::vector<std::string> labels);
	std::vector<std::string> getLabels() const;

	std::vector<std::weak_ptr<Node<E>>> getNodes();

	/************************************************
	 *  MUTATORS
	 ***********************************************/
	void addLabel(std::string label);
	void addLabel(std::vector<std::string> labels);

	void addNode(std::shared_ptr<Node<E>> nodeToAdd);

	void deleteEdges(std::shared_ptr<Node<E>> node);
	void removeNode(std::shared_ptr<Node<E>> node);

	/************************************************
	 *  STRUCTURAL/RELATIONSHIP CHECKS/CHANGES/GETS
	 ***********************************************/
	bool containsNode(std::shared_ptr<Node<E>> possiblenode);

	/* BELOW ARE FUNCTIONS THAT HAVE BEEN REMOVED BUT MAY BE ADDED AGAIN
	 *
	 */
	//std::vector<std::vector<std::weak_ptr<Node<E>>>> makeAdjacencyList();
private:
	/************************************************
	 *  IDENTIFIERS
	 ***********************************************/
	unsigned short int index;
	std::string name;
	std::vector<std::string> labels;

	/************************************************
	 *  STRUCTURAL OWNERSHIP
	 ***********************************************/
	std::unordered_set<std::shared_ptr<Node<E>>> containingNodes;

	/************************************************
	 *  HELPER FUNCTIONS
	 ***********************************************/
	/* Until I figure out a way that I like doing the hashing this is used
	 * to ensure we dont have a ptr to a node that has been deleted within
	 * our containing list.
	 *
	 * Eventually, our node destructor will take care of deleting self from this list.
	 */
	void refreshContaining();
};

/************************************************
 *  CONSTRUCTORS/DESTRUCTORS
 ***********************************************/

template<class T, class E>
Graph<T, E>::Graph()
{
	this->setName("DEFAULT_GRAPH_NAME");
	this->setIndex(1);
	if (graphDebug)
	{
		std::string conMsg = "Warning, created a graph with the default name";
		badBehavior(__LINE__, __func__, conMsg);
	}
}

template<class T, class E>
Graph<T, E>::Graph(std::string name)
{
	this->setName(name);
	this->setIndex(1);
	if (graphDebug)
	{
		std::string conMsg = "Constructed graph with name: " + this->getName();
		lazyInfo(__LINE__, __func__, conMsg);
	}
}

template<class T, class E>
Graph<T, E>::~Graph()
{
	//eventually call our delete edges function where we pass in our grpah hash
	if (graphDebug)
	{
		std::string desMsg = "Deleting graph with name of: " + this->getName();
		lazyInfo(__LINE__, __func__, desMsg);
	}
}

/************************************************
 *  GETTER/SETTER PAIRS
 ***********************************************/

template<class T, class E>
void Graph<T, E>::setIndex(unsigned short int index)
{
	this->index = index;
}

template<class T, class E>
unsigned short int Graph<T, E>::getIndex() const
{
	return this->index;
}

template<class T, class E>
void Graph<T, E>::setName(std::string name)
{
	this->name = name;
}

template<class T, class E>
std::string Graph<T, E>::getName() const
{
	return this->name;
}

template<class T, class E>
void Graph<T, E>::setLabels(std::vector<std::string> labels)
{
	this->labels = labels;
}

template<class T, class E>
std::vector<std::string> Graph<T, E>::getLabels() const
{
	return this->labels;
}

template<class T, class E>
std::vector<std::weak_ptr<Node<E> > > Graph<T, E>::getNodes()
{
	this->refreshContaining();
	std::vector<std::weak_ptr<Node<E>>> nodes;
	nodes.insert(nodes.end(), this->containingNodes.begin(),
			this->containingNodes.end());
	return nodes;
}

/************************************************
 *  MUTATORS
 ***********************************************/

template<class T, class E>
void Graph<T, E>::addLabel(std::string label)
{
	this->labels.push_back(label);
}

template<class T, class E>
void Graph<T, E>::addLabel(std::vector<std::string> labels)
{
	for (std::string currLabel : labels)
		this->addLabel(currLabel);
}

template<class T, class E>
void Graph<T, E>::addNode(std::shared_ptr<Node<E> > nodeToAdd)
{
	this->refreshContaining();
	if (this->containsNode(nodeToAdd))
	{
		badBehavior(__LINE__, __func__, "Warning: node already present");
	}
	else
	{
		//How does our node know of our graph
		this->containingNodes.insert(nodeToAdd);
	}
}

// Deletes all edges to and from the node, yet keeps the node in the graph
template<class T, class E>
void Graph<T, E>::deleteEdges(std::shared_ptr<Node<E> > node)
{
//eventually call in our value to pass in the hash of the graph, as of now just delete all our edges of the node and also make sure it be a part of the graph
	this->refreshContaining();
	if (this->containsNode(node))
	{
		node.get()->deleteEdges();
	}
	else
	{
		std::string badMsg = "Warning: node not present in graph ("
				+ this->getName() + ")";
		badBehavior(__LINE__, __func__, badMsg);
	}
}

template<class T, class E>
void Graph<T, E>::removeNode(std::shared_ptr<Node<E> > node)
{
	this->refreshContaining();
	if (this->containsNode(node))
	{
		if (graphDebug)
		{
			std::string remMsg = "Removing node (" + node.get()->getName()
					+ ") from graph (" + this->getName() + ")";
			lazyInfo(__LINE__, __func__, remMsg);
		}
		this->containingNodes.erase(node);
	}
	else
	{
		std::string badMsg = "Warning: node not present in graph ("
				+ this->getName() + ")";
		badBehavior(__LINE__, __func__, badMsg);
	}
}

/************************************************
 *  STRUCTURAL/RELATIONSHIP CHECKS/CHANGES/GETS
 ***********************************************/
template<class T, class E>
bool Graph<T, E>::containsNode(std::shared_ptr<Node<E> > possiblenode)
{
	return this->containingNodes.count(possiblenode);
}

/************************************************
 *  HELPER FUNCTIONS
 ***********************************************/
//called before we ever mess with our containing nodes
template<class T, class E>
inline void Graph<T, E>::refreshContaining()
{
//tl:dr this keeps us happy since our nodes cant do any deleting of self from graph structures
	if (graphDebug)
		lazyInfo(__LINE__, __func__);
	std::vector<std::shared_ptr<Node<E>>> dirt;
	for (std::shared_ptr<Node<E>> const &node : this->containingNodes)
	{
		if (node.unique())
		{
			//means that this is the last place keeping the node alive, aka our kickstand is holding it up. Possible a dangle now but unsure
			if (graphDebug)
			{
				std::string refMsg = "Found to delete node: ("
						+ node.get()->getName() + ") from graph: "
						+ this->getName();
				lazyInfo(__LINE__, __func__, refMsg);
			}
			dirt.push_back(node);
		}
	}
	for (auto &node : dirt)
		this->containingNodes.erase(node);
}

#endif /* INC_GRAPH_H_ */

