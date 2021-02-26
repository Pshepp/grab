/**
 * @file edge.h
 * @brief Something about this edge class and its structure.
 *
 * More info can go here.
 *
 * @author Preston
 * @date 22 Feb 2021
 */

#ifndef INC_EDGE_H_
#define INC_EDGE_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <memory>

#include "node.h"
#include "graph.h"

template <class T>
class Edge
{
public:

private:
	unsigned short int index;

	bool isBridge;
	bool isVisited;
	std::string name;
	std::vector<std::string> labels;

	/*NOTE: Keep in mind that our source node owns the edge itself
	 * 			so this would be perfect for a smart pointer that
	 * 			enforces ownership (possibly shared pointer between
	 * 			our graph structure and our node?).
	 *
	 * 			Using shared as of now due to us needing this in the
	 * 			graph structure itself and to force us to have a source
	 * 			node.
	 *
	 * 			Note that the edge will either be sunk or sourced by a node.
	 * 			Once this node has no references from either sinks or sources
	 * 			or our graph we should get rid of it.
	 *
	 * 			We must use shared_ptr due to the fact that multiple edges can
	 * 			be related to each node.
	 */
	std::shared_ptr<Node <T> > sourceNode;
	std::shared_ptr<Node <T> > destNode;

	std::unordered_set< std::weak_ptr<Graph <T> > > parentGraphs;

};

#endif /* INC_EDGE_H_ */
