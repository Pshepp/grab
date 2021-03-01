/**
 * @file graph.h
 * @brief THIS IS A NECRO'D BRANCH. PLEASE DO NOT USE.
 *
 * More info can go here.
 *
 * @author Preston
 * @date 22 Feb 2021
 */

#ifndef INC_GRAPH_H_
#define INC_GRAPH_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <memory>

#include "node.h"

template <class T>
class Graph
{
public:
	Graph();
	Graph(std::string inputName)
	{
		this->setName(inputName);
	}

	std::string getName();

	void addNode(std::shared_ptr<Node<T>> nodeToAdd);
	void addNodes(std::vector<std::shared_ptr<Node<T>>> nodes);

	void setName(std::string inputName);

	void printAdjList();

	void printNodeNeighbors(std::shared_ptr <Node <T> > nodeToObserve);


	//soon
	//void printNodeChildren(std::shared_ptr <Node <T> > nodeToObserve);
	//void printNodeParents(std::shared_ptr <Node <T> > nodeToObserve);

private:

	std::string name;

	//cant have dupes
	std::vector<std::shared_ptr< Node <T> > > nodesInGraph;

};




template <typename T>
void Graph<T>::addNode(std::shared_ptr<Node <T>> nodeToAdd)
{
	this->nodesInGraph.push_back(nodeToAdd);
}

template <typename T>
void Graph<T>::addNodes(std::vector<std::shared_ptr<Node<T>>> nodes)
{
	this->nodesInGraph.insert(this->nodesInGraph.end(), nodes.begin(), nodes.end());
}

//put in my data showing file (where we will handle all prints etc)
template <typename T>
void Graph<T>::printNodeNeighbors(std::shared_ptr<Node <T>> nodeToObserve)
{
	std::cout << nodeToObserve.getName() << " ";
	for (std::string name : nodeToObserve.getNeighbors().getName())
	{
		std::cout << "-> " << name <<" ";
	}
	std::cout << endl;
}

//again, vector can cause dupes
template <typename T>
void Graph<T>::printAdjList()
{
	for (auto& node : this->nodesInGraph)
	{
		this->printNodeNeighbors(node);
	}
}


template <typename T>
void Graph<T>::setName(std::string inputName)
{
	name = inputName;
}

#endif /* INC_GRAPH_H_ */
