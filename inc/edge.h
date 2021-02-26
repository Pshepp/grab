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

template <class T>
class Edge
{
public:
	Edge();

private:
	unsigned short int index;

	bool isBridge;
	bool isVisited;
	std::string name;
	std::vector<std::string> labels;

	std::shared_ptr<Node <T> > sourceNode;
	std::shared_ptr<Node <T> > destNode;

};

#endif /* INC_EDGE_H_ */
