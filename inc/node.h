/**
 * @file node.h
 * @author Preston Sheppard
 * @date 22 Feb 2021
 * @brief This node class is to be used as a part of our total graph abstraction "toolkit".
 *
 * We are defining a node object that utilizes smart pointers to not only increase efficiency but
 * to also help denote "ownership". Our node has a unique_ptr to each connected edge, thus our node
 * owns the edge. This is needed due to smart pointers needing some form of ownership hierarchy in
 * order to allow for proper use. Please note our node pointer structure. The reasoning behind a node
 * owning an edge is due to if a node is deleted all edges must be deleted but not necessarily vice-versa.
 * ADD MORE INFO/CLARIFICATION AS NEEDED
 */

/*	TODO: The following are in no particular order:
 * 			- Increase our speed using unordered_set, etc. Please keep in mind that we will have to ensure our iterator is correct.
 * 			- Properly track our node index and figure out the pipeline the adjacency matrix/list should use.
 * 					- Fix: Nest our set of edge pointers within a hashmap where key = graph ptr or something
 * 						robustly hashable and value is to our vector of ptrs for our nodes :o)
 * 			- Fix node presence in multiple graph structures, this will take adding a "layer" to our edges that correlates to each structure
 * 			- Possibly implement a relationship check (i.e Node A == Node B) for hunting and raw ptr checks.
 * 			- If memory footprint is too large, begin passing by reference for some of our functions i.e.
 * 					- label addition, etc.
 * 					- returning of our neighbors
 */

#ifndef INC_NODE_H_
#define INC_NODE_H_

#include <vector>
#include <string>
#include <memory>

template<class T> class Graph;

template<class T>
class Node: public std::enable_shared_from_this<Node<T>>
{
public:
	/************************************************
	 *  CONSTRUCTORS
	 ***********************************************/
	Node()
	{
		this->setName("DEFAULT_NODE_NAME");
		this->setIndex(1);
		this->setIsLeaf(false);
		this->setIsBridge(false);
		this->setIsVisited(false);
	}

	//do we actually need an internal pointer to the object? Why not just use [OBJ].get() or something of the same?
	Node(std::string name)
	{
		this->setName(name);
		this->setIndex(1);
		this->setIsLeaf(false);
		this->setIsBridge(false);
		this->setIsVisited(false);
	}

	// To properly remove a node we are mostly worried about dangling pointers, will be taken care in our "deleteAllEdges()" member. Must think regarding edge cases.
	~Node()
	{
		this->deleteAllEdges();
	}

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

	std::shared_ptr<Node<T>> getNeighborByName(std::string name);
	std::shared_ptr<Node<T>> getNeighborByIndex(unsigned short index);

	//Can we const protect this? I feel no due to this being created from algo and not being a data member. Maybe take in a reference to a vec and mutate?
	std::vector<std::shared_ptr<Node<T>>> getNeighbors();
	/* TODO: Change functionalities to allow nodes to be owned by multiple graph structures
	 *
	 * std::vector<std::vector<std::shared_ptr<Node<T>>>> getNeighbors();
	 * std::vector<std::shared_ptr<Node<T>>> getNeighbors(std::weak_ptr<Graph<T>> containingGraph);
	 */

	/************************************************
	 *  MUTATORS
	 ***********************************************/
	void addLabel(std::string label);
	void addLabel(std::vector<std::string> labels);

	void addNeighbor(std::shared_ptr<Node<T>> neighborToAdd); //calling node is parent
	void deleteNeighbor(std::shared_ptr<Node<T>> neighborToRemove);

	void deleteEdge(std::shared_ptr<Node<T>> secondNode);
	void deleteAllEdges();
	/*	TODO: Change functionalities to allow nodes to be owned by multiple graph structures and mutate per specific graph structures.
	 *
	 *	void deleteEdge(std::shared_ptr<Node<T>> secondNode, std::weak_ptr<Graph<T>> containingGraph);
	 *	void deleteAllEdges(std::weak_ptr<Graph<T>> containingGraph);
	 *
	 *	void addGraphOwnership(std::weak_ptr<Graph <T>> graphToAdd);
	 */

	void switchEdgeDirection(std::shared_ptr<Node<T>> nodeB);

	/************************************************
	 *  STRUCTURAL/RELATIONSHIP CHECKS
	 ***********************************************/
	bool isNeighbor(Node<T> possibleNeighbor);
	bool isParent(Node<T> possibleParent);
	bool isChild(Node<T> possibleChild);

	bool containsLabel(std::string labelToCheck);
	bool containsLabel(std::vector<std::string> labelToCheck);

private:
	/************************************************
	 *  IDENTIFIERS
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
	/*	TODO: CREATE A MONOGOMY PTR, YOU ALREADY HAVE THE NEEDED LOGIC
	 * 			JUST TEMPLATE OUT. THINK REGARDING ROBUSTNESS
	 */
	std::vector<std::unique_ptr<Edge<T>>> outEdges;
	//	NOTE: We must set our raw ptr (in our destination node) to null before destructing the unique ptr to the edge itself.
	std::vector<Edge<T>*> inEdges;
	/*	TODO: Change functionalities to allow nodes to be owned by multiple graph structures and mutate per specific graph structures.
	 *
	 * 	std::vector<std::weak_ptr<Graph<T>>> parentGraphs;
	 */

	/************************************************
	 *  HELPER FUNCTIONS
	 ***********************************************/
	void deleteInEdge(Edge<T>* inEdgeToDelete);
	void deleteOutEdge(std::unique_ptr<Edge<T>> outEdgeToDelete); //should be fine not having to get our unique ptr, we are working only within scope of class, possibly incorrect

};

#endif /* INC_NODE_H_ */

