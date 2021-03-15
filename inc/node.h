#ifndef INC_NODE_H_
#define INC_NODE_H_

#include <vector>
#include <memory>
#include <unordered_map>

#include "lazyPrints.h"

//probably gonna need to full include
template<class T> class Edge;
template<class T, class E> class Graph;

const bool nodeDebug = true;

//TODO: Figure out how to proper and quickly do our hashing
int graphHash = 100;

template<class T>
class Node: public std::enable_shared_from_this<Node<T>>
{
public:
	/************************************************
	 *  CONSTRUCTORS/DESTRUCTORS
	 ***********************************************/
	Node();
	Node(std::string name);

	~Node();

	/************************************************
	 *  GETTER/SETTER PAIRS
	 ***********************************************/
	void setIndex(unsigned short int index);
	unsigned short int getIndex() const;

	void setName(std::string name);
	std::string getName() const;

	void setLabels(std::vector<std::string> labels);
	std::vector<std::string> getLabels() const;

	void setIsLeaf(bool leaf);
	bool getIsLeaf() const;

	void setIsBridge(bool bridge);
	bool getIsBridge() const;

	void setIsVisited(bool visited);
	bool getIsVisited() const;

	std::vector<std::weak_ptr<Node<T>>> getNeighbors();
	std::vector<std::weak_ptr<Node<T>>> getChildren();
	std::vector<std::weak_ptr<Node<T>>> getParents();

	/************************************************
	 *  MUTATORS
	 ***********************************************/
	void addLabel(std::string label);
	void addLabel(std::vector<std::string> labels);

	void addNeighbor(std::string edgeName,
			std::shared_ptr<Node<T>> freshNeighbor);

	void deleteEdgesToChild(std::shared_ptr<Node<T>> child);
	void deleteEdgesToParent(std::shared_ptr<Node<T>> parent);
	void deleteEdges(std::shared_ptr<Node<T>> nodeB);

	void deleteEdges();

	/************************************************
	 *  STRUCTURAL/RELATIONSHIP CHECKS/CHANGES/GETS
	 ***********************************************/
	bool isChild(std::shared_ptr<Node<T>> possibleParent);
	bool isParent(std::shared_ptr<Node<T>> possibleChild);
	bool isNeighbor(std::shared_ptr<Node<T>> possibleNeighbor);

	bool containsLabel(std::string labelToCheck);
	bool containsLabel(std::vector<std::string> labelToCheck); //Do we need?

	//POSSIBLY CHANGE THIS TO PRIVATE HELPER FUNCTIONS. AS OF NOW KEEP PUBLIC AND TRY NOT TO USE OUTSIDE
	std::vector<Edge<T>*> getConnectingEdges(std::shared_ptr<Node<T>> nodeB);
	std::vector<Edge<T>*> getInConnectingEdges(std::shared_ptr<Node<T>> nodeB);
	std::vector<Edge<T>*> getOutConnectingEdges(std::shared_ptr<Node<T>> nodeB);

	/************************************************
	 *  HELPING FUNCTIONS
	 ***********************************************/
	std::string edgesAsString();

	/* BELOW ARE FUNCTIONS THAT HAVE BEEN REMOVED BUT MAY BE ADDED AGAIN
	 *
	 */
	//std::weak_ptr<Node<T>> getNeighborByName(std::string name);
	//std::weak_ptr<Node<T>> getNeighborByIndex(unsigned short index);
private:
	/************************************************
	 *  IDENTIFIERS/INFORMATION
	 ***********************************************/
	unsigned short int index;
	std::string name;
	std::vector<std::string> labels;

	/************************************************
	 *  STRUCTURAL ATTRIBUTES
	 ***********************************************/
	bool leaf;
	bool bridge;

	/************************************************
	 *  ALGORITHM TRAVERSAL ATTRIBUTES
	 ***********************************************/
	bool visited;

	/************************************************
	 *  STRUCTURAL OWNERSHIP
	 ***********************************************/
	std::vector<std::unique_ptr<Edge<T>>> outEdges;
	std::vector<Edge<T>*> inEdges;

	/* TODO: How do we know about our owning graph? The issue
	 * 			is I do not want to have to deal with a template
	 * 			with 2 types instead fo 1. Seems impossible as of now.
	 *
	 * 			Below is how I am hoping to implement our multi-graph ownership.
	 * 			Will be uncommented and filled out in a different branch.
	 */
	//std::vector<std::weak_ptr<int>> owningGraphs; //do not use weak_ptr, other options?
	//std::unordered_map<std::weak_ptr<int>, std::vector<std::unique_ptr<Edge<T>>>> graphSpecifcOutEdges; //this is an unordered map using some type of value that signifies the graph, note that every graph in here must be present in owningGraphs but not vice versa.
	//std::unordered_map<std::weak_ptr<int>, std::vector<Edge<T>* >> graphSpecificInEdges;
	/************************************************
	 *  HELPER FUNCTIONS
	 ***********************************************/
	void deleteInEdge(Edge<T> *inEdgeToDelete);
	void deleteOutEdge(Edge<T> *outEdgeToDelete);

	bool hasInEdge(Edge<T> *possibleInEdge);
	bool hasOutEdge(Edge<T> *possibleOutEdge);

};

#endif /* INC_NODE_H_ */
