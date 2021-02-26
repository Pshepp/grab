/**
 * @file graph.h
 * @brief Something about this graph class and its structure.
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

private:

	std::unordered_set<std::shared_ptr< Node <T> > > nodesInGraph;

};

#endif /* INC_GRAPH_H_ */
