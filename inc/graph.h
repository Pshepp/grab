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
		this->setName(name);
		this->setIndex(1);
	}

	/************************************************
	 *  GETTER/SETTER PAIRS
	 ***********************************************/
	void setIndex(unsigned short int index);
	unsigned short int getIndex();

	void setName(std::string name);
	std::string getName();

	void setLabels(std::vector<std::string> labels);
	std::vector<std::string> getLabels();

	std::vector<std::vector<Node<T>>> getAdjacencyList(); //adj list built when called

	std::vector<std::shared_ptr<Node<T>>> getNodes();

	/************************************************
	 *  MUTATORS
	 ***********************************************/
	void addLabel(std::string label);
	void addLabel(std::vector<std::string> labels);

	void addNode(std::shared_ptr<Node<T>> nodeToAdd);

	void deleteNode(std::shared_ptr<Node<T>> nodeToDelete);

private:
	unsigned short int index;
	std::string name;
	std::vector<std::string> labels;

	//TODO: Keep us from having dupes
	std::vector<std::shared_ptr<Node<T>>> nodesWithin;

};

#endif /* INC_GRAPH_H_ */

