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
#include <unordered_set>

//may be able to switch this to a forward declare
#include "node.h"

//include our algos
#include "leafFinder.h"

const bool graphDebug = true;

template<class T, class E>
class Graph
{
public:
	Graph()
	{
		this->setName("DEFAULT_GRAPH_NAME");
		this->setIndex(1);
		if (graphDebug)
		{
			lazyInfo(__LINE__, __func__, "Created default graph");
		}
	}

	Graph(std::string name)
	{
		this->setName(name);
		this->setIndex(1);
		if (graphDebug)
		{
			std::string conMsg = "Constructed graph with name: "
					+ this->getName();
			lazyInfo(__LINE__, __func__, conMsg);
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
	std::vector<std::string> getLabels();

	std::vector<std::vector<std::weak_ptr<Node<E>>>> makeAdjacencyList(); //adj list built when called

	std::vector<std::weak_ptr<Node<E>>> getNodes();

	bool containsNode(std::shared_ptr<Node<E>> possibleNode);

	/************************************************
	 *  MUTATORS
	 ***********************************************/
	void addLabel(std::string label);
	void addLabel(std::vector<std::string> labels);

	void addNode(std::shared_ptr<Node<E>> nodeToAdd);

	//remove node just means removing from the current graph structure. Not full delete
	void removeNode(std::shared_ptr<Node<E>> nodeToRemove);
	//completely delete out node within this graph, keep in mind this node may exist within other graphs. We must eventually worry about removing the edges that will be related to this node. Same for above
	void deleteNode(std::shared_ptr<Node<E>> nodeToDelete);

private:
	unsigned short int index;
	std::string name;
	std::vector<std::string> labels;

	//https://en.cppreference.com/w/cpp/memory/owner_less to use as a key
	//std::vector<std::shared_ptr<Node<T>>> nodesWithin;
	//need to ensure this is correct
	std::unordered_set<std::shared_ptr<Node<E>>,
			std::hash<std::shared_ptr<Node<E>>>> nodesWithin;

	void nodesWithinRefresh();

};

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
std::vector<std::string> Graph<T, E>::getLabels()
{
	return this->labels;
}

template<class T, class E>
void Graph<T, E>::addLabel(std::string label)
{
	this->labels.push_back(label);
}

template<class T, class E>
std::vector<std::vector<std::weak_ptr<Node<E> > > > Graph<T, E>::makeAdjacencyList()
{
}

template<class T, class E>
std::vector<std::weak_ptr<Node<E> > > Graph<T, E>::getNodes()
{

	this->nodesWithinRefresh();
	std::vector<std::weak_ptr<Node<E>>> nodes;
	nodes.insert(nodes.end(), this->nodesWithin.begin(),
			this->nodesWithin.end());

	/*for (std::weak_ptr<Node<E>> n : nodes)
	{
		if (n.lock())
		{
			std::shared_ptr<Node<E>> sn = n.lock();
			std::string m1 = "Node name: " + sn.get()->getName() + " \n";
			m1 = m1 + "Use count: " + std::to_string(sn.use_count());
			std::cout << m1 << std::endl;
			if (sn.get()->getName() == "Delux")
			{
				int oES = sn.get()->getChildren().size();
				int iES = sn.get()->getParents().size();
				std::string l = "Number Children: " + std::to_string(oES)
						+ "\nNumber Parents: " + std::to_string(iES);
				sn.get()->printEdges();
				l = l + "Number neighbors "
						+ std::to_string(sn.get()->getNeighbors().size());
				std::cout << l << "\n";

			}
		}
	}*/
	return nodes;
}

template<class T, class E>
bool Graph<T, E>::containsNode(std::shared_ptr<Node<E> > possibleNode)
{
	this->nodesWithinRefresh();
	return this->nodesWithin.count(possibleNode);
}

template<class T, class E>
void Graph<T, E>::addLabel(std::vector<std::string> labels)
{
	for (std::string currLabel : labels)
		this->addLabel(currLabel);
}

template<class T, class E>
void Graph<T, E>::addNode(std::shared_ptr<Node<E>> nodeToAdd)
{
	this->nodesWithinRefresh();
	if (this->nodesWithin.count(nodeToAdd))
	{
		badBehavior(__LINE__, __func__, "Node already present");
	}
	else
		this->nodesWithin.insert(nodeToAdd);

}

template<class T, class E>
void Graph<T, E>::removeNode(std::shared_ptr<Node<E>> nodeToRemove)
{
	this->nodesWithinRefresh();
	int i = this->nodesWithin.count(nodeToRemove);
	if (i)
	{
		this->nodesWithin.erase(nodeToRemove);
	}
	else
	{
		/*for (std::shared_ptr<Node<E>> i : this->nodesWithin)
		 {
		 std::cout << "\n\n\n" << nodeToRemove.get()->getName() << "\n\n\n";
		 std::cout << "\n\n\n" << i.get()->getName() << "\n\n\n";
		 std::string j =
		 (nodeToRemove.get() == i.get()) ? "SAME" : "DIFFERENT";
		 std::cout << "\n\n\n" << j << "\n\n\n";
		 }*/
		badBehavior(__LINE__, __func__,
				"Node not present in graph" + std::to_string(i));
	}

}

template<class T, class E>
void Graph<T, E>::deleteNode(std::shared_ptr<Node<E> > nodeToDelete)
{
//TODO: update our node class to handle this type of hashing because we will want to delete edges associated with this

	this->removeNode(nodeToDelete);	//remove our node from this structure first

//will eventuall use deleteAllEdges with a weak_ptr to the graph, this way we can hash it to the edges
	nodeToDelete.get()->~Node();

}

template<class T, class E>
void Graph<T, E>::nodesWithinRefresh()
{
	std::vector<std::shared_ptr<Node<E>>> dirt;
	for (std::shared_ptr<Node<E>> const &node : this->nodesWithin)
	{
		if (node.unique())
		{
			/*lazyInfo(__LINE__, __func__,
					"Deleting node: " + node.get()->getName()
							+ " node use count: "
							+ std::to_string(node.use_count()));*/
			dirt.push_back(node);

		}

	}

	for (auto &node : dirt)
	{
		this->nodesWithin.erase(node);
	}
}

#endif /* INC_GRAPH_H_ */

