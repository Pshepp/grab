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
#include "edge.h"

template <class T>
class Graph
{
public:

private:

	std::unordered_set<std::shared_ptr< Node <T> > > nodesInGraph;
	std::unordered_set<std::shared_ptr< Edge <T> > > edgesInGraph;

};


/*
 * https://github.com/hsutter/gcpp
 *
 * http://www.club.cc.cmu.edu/~ajo/disseminate/Nweak_ptr_proposal.html
 *
 * https://stackoverflow.com/questions/15384443/doubly-linked-list-using-stdunique-ptr
 *
 * https://gist.github.com/mukunda-/153d802065c130e2956c
 */
#endif /* INC_GRAPH_H_ */
