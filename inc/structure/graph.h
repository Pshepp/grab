/* TODO: Eventually change all of our altering functions so node-based functions
 * 			take in the value from the graph that we will want to hash and use
 * 			as a key to denote which edge is part of what graph.
 *
 *
 *
 */

#ifndef INC_GRAPH_H_
#define INC_GRAPH_H_

#include <vector>
#include <memory>
#include <unordered_set>

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
	bool containsNode(std::shared_ptr<Node<E>> possibleNode);

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
};

#endif /* INC_GRAPH_H_ */

