/**
 * @file node.h
 * @author Preston Sheppard
 * @date 22 Feb 2021
 * @brief This node class is to be used as a part of our total graph abstraction "toolkit".
 *
 * We are defining a node object that utilizes smart pointers to not only increase efficiency but
 * to also help denote "ownership". Our node has a unique_ptr to each connected edge, thus our node
 * owns the edge. This is needed due to smart pointers needing some form of ownership hiearchy in
 * order to allow for proper use. Please note our node pointer structure. ADD MORE INFO/CLARIFICATION AS NEEDED
 *
 * TODO:
 * 		- Increase speed using unordered_set
 * 		- Figure out how to properly create and track our node index, honestly just decriment all by one
 * 			then allow our (possible) adjacency list/adjacency matrix rep to be created from nodes again.
 * 		- Worry about operator overloading
 * 		- Possibly switch rhs & lhs (parent -> child to child -> parent)
 */

/* TODO: FIX NODE PRESENCE IN MULTIPLE GRAPH STRUCTURES. FIGURE OUT HOW TO PROPERLY ALLOW NODES BE SHARED BETWEEN DIFFERENT GRAPH STRUCTURES
 * 			THIS CURRENT IMPLEMENTATION WILL NOT WORK DUE TO DIFFERENT NODES IN DIFFERENT STRUCTURES
 * 			NOT HAVING A DIFFERENCE IN EDGE PTRS. WE WOULD HAVE TO CREATE A 2D STRUCTURE WHERE HAVE SPECIFIC
 * 			EDGES FOR SPECIFIC GRAPHS, POSSIBLY USE HASH MAP FOR OUR DIFFERENT EDGE CONNECTIONS(?). NOTE WE DO NOT WANTED CLONED
 * 			MEMBERS SO UNOREDED SET OR SOMETHING OF THAT NATURE WOULD HELP. FUNCTIONS LISTED BELOW
 *
 * 	std::vector<std::vector<std::shared_ptr<Node<T>>>> getNeighbors(); //will most likeley want to return a 2d vector so we can denote which neighbors are which
 * std::vector<std::shared_ptr<Node<T>>> getNeighbors(std::weak_ptr<Graph<T>> containingGraph); //get all neighbors within a specific graph structure
 *
 * void deleteEdge(std::shared_ptr<Node<T>> secondNode, std::weak_ptr<Graph<T>> containingGraph); //delete edge between 2 nodes in specific graph structure
 * void deleteAllEdges(std::weak_ptr<Graph<T>> containingGraph); //delete all edges connected to calling node in specific graph structure
 *
 */

#ifndef INC_NODE_H_
#define INC_NODE_H_

#include <vector>
#include <string>
#include <memory>

//since we do not need to actually call any functions from graph etc. we just forward declare. Not needed yet
template<class T> class Graph;

template<class T>
class Node: public std::enable_shared_from_this<Node<T>>
{
public:

	/************************************************
	 *  CONSTRUCTORS
	 ***********************************************/
	Node() //should never be called
	{
		this->setName("DEFAULT_NODE_NAME");
		this->setIndex(1);
		this->setIsLeaf(false);
		this->setIsBridge(false);
		this->setIsVisited(false);
	}

	/* TODO: Figure out if we need the internal ptr to the object
	 * https://www.nextptr.com/tutorial/ta1414193955/enable_shared_from_this-overview-examples-and-internals
	 * https://codereview.stackexchange.com/questions/166395/custom-stdshared-ptrt-implementation
	 */
	//do we actually need an internal pointer to the object? Why not just use p.get() or something of the same?
	Node(std::string name)
	{
		this->setName(name);
		this->setIndex(1);
		this->setIsLeaf(false);
		this->setIsBridge(false);
		this->setIsVisited(false);
	}

	~Node() //possibly just delete all edges connected then we are done when this goes out of scope. Keep in mind Node will still have a reference of shared_ptr from our Graph class. HMMMMMMM. Will mostly be worrying about our in edges and preventing dangling ptrs
	{

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

	void setIsLeaf(bool isLeaf);
	bool getIsLeaf();

	void setIsBridge(bool isBridge);
	bool getIsBridge();

	void setIsVisited(bool isVisited);
	bool getIsVisited();

	std::vector<std::shared_ptr<Node<T>>> getNeighbors();

	/************************************************
	 *  MUTATORS
	 ***********************************************/
	void addLabel(std::string label);
	void addLabel(std::vector<std::string> labels); //note that we will just overload our singular add, rename to addLables?

	void addNeighbor(std::shared_ptr<Node<T>> neighborToAdd); //the calling node is the "parent", may want to sep this out into addChild/addParent
	void deleteNeighbor(std::shared_ptr<Node<T>> neighborToRemove);

	//delete an edge between 2 nodes, not sure if there will be a need to delete an edge from a specific edge id. Could make private
	void deleteEdge(std::shared_ptr<Node<T>> secondNode); //delete edge between 2 nodes
	void deleteAllEdges(); //delete all edges connected to the calling node


	//	void addGraphOwnership(std::weak_ptr<Graph <T>> graphToAdd);

	/************************************************
	 *  STRUCTURAL/RELATIONSHIP CHECKS
	 ***********************************************/
	// TODO: Possibly would like some type of relationship check, just a bool to see if Node A == Node B for hunting and raw ptr checks
	bool isNeighbor(Node<T> possibleNeighbor);
	bool isParent(Node<T> possibleParent); //a parent is a node that owns the incomming edge
	bool isChild(Node<T> possibleChild); //child is node that uses a raw ptr to reference the incomming edge

	bool containsLabel(std::string labelToCheck);

private:

	/************************************************
	 *  IDENTIFIERS
	 ***********************************************/
	unsigned short int index; //Saves space, max is ~65,000. If we need more then easy to change
	std::string name;
	std::vector<std::string> labels;

	/************************************************
	 *  STRUCTURAL ATTRIBUTES
	 ***********************************************/
	bool isLeaf;
	bool isBridge;

	/************************************************
	 *  ALGORITHM TRAVERSAL ATTRIBUTES
	 ***********************************************/
	bool isVisited;

	/************************************************
	 *  STRUCTURAL OWNERSHIP
	 ***********************************************/
	//std::vector<std::weak_ptr<Graph<T>>> parentGraphs; //graphs that this node is present in
	std::vector<std::unique_ptr<Edge<T>>> outEdges;	//edges that are from this node
	std::vector<Edge<T>*> inEdges;				//edges that are to this node
	//NOTE: We must set our inEdge ptr to null/remove it before we destruct and get rid of the unique ptr to the edge itself!

	/************************************************
	 *  HELPER FUNCTIONS
	 ***********************************************/
	//TODO: possibly just remove and write them below
	void deleteOutEdges(); 	//delete all outgoing edges from calling node
	void deleteInEdge();		//delete all incoming edges from calling node

};

#endif /* INC_NODE_H_ */

