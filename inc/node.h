/**
 * @file node.h
 * @brief Something about this node class and its structure.
 *
 * More info can go here.
 *
 * @author Preston
 * @date 22 Feb 2021
 */

#ifndef INC_NODE_H_
#define INC_NODE_H_

#include <vector>
#include <string>
#include <unordered_set>

#include "graph.h"
template <class T>
class Node
{
public:

private:
	unsigned short int index;

	std::string name;
	std::vector<std::string> labels;

	bool isVisited;
	bool isBridge;
	bool isLeaf;

	//return neighbors = out of in and out edges

	/* our edges that are coming from the node. From my understanding
	 * this should be unique because only
	 *
	 * Let us make a monogomy_ptr which will be a shared poitner but destructs when <2 references instead of <1
	 */
	//can I tuple unique? Aka have a unique ptr that owns the in and out edges?
	std::unordered_set< std::shared_ptr<Edge <T> > > outEdges;
	/*our edges that end up pointing to the node. I would like to also use unique_ptr here
	 * due to the ability of if this node disapears then all connected edges get taken care
	 * of.
	 */
	std::unordered_set< std::weak_ptr<Edge <T> > > inEdges;

	std::unordered_set< std::weak_ptr<Graph <T> > > parentGraphs;
};



#endif /* INC_NODE_H_ */
