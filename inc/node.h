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
	Node();


private:
	unsigned short int index;

	std::string name;
	std::vector<std::string> labels;

	bool isVisited;
	bool isBridge;
	bool isLeaf;

	std::unordered_set< std::unique_ptr<Edge <T> > > outEdges;
	std::unordered_set< Edge <T> * > inEdges;

	std::unordered_set< std::weak_ptr<Graph <T> > > parentGraphs;
};



#endif /* INC_NODE_H_ */
