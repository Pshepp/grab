/**
 * @file edge.h
 * @author Preston Sheppard
 * @date 22 Feb 2021
 * @brief Our edge class is to be templated and used in conjunction with out node class.
 *
 * Please note our edges are designed to be "owned" by our node class. This is due to the fact
 * that if we delete a single node then we must delete all edges connected to the node, but if
 * we delete a single edge then we may not be required to delete the single node. Please note we do not support
 * edge contraction, etc.
 *
 * To ensure that an edge is deleted properly, it must be deleted from our node class. This is due
 * to the node class containing the needed logic to prevent dangling pointers within said class.
 *
 * An edge cannot exist without at least two nodes.
 */

#ifndef INC_EDGE_H_
#define INC_EDGE_H_

// TODO: REMOVE AFTER BUG TESTING
#include <iostream>

#include <vector>
#include <string>
#include <memory>

//may need to bite the bullet and just to a full include of the header
template<class T> class Node;

template<class T>
class Edge
{
public:

	/************************************************
	 *  CONSTRUCTORS
	 ***********************************************/
	Edge() //Honestly, should not be called. Need to think more.
	{
		this->setName("DEFAULT_EDGE_NAME");
		this->setIndex(1);
		this->setIsBridge(false);
		this->setIsVisited(false);
	}

	Edge(std::string name, std::shared_ptr<Node<T>> sourceNode, std::shared_ptr<Node<T>> sinkNode)
	{
		this->setName(name);
		this->setIndex(1);
		this->setIsBridge(false);
		this->setIsVisited(false);
		this->sourceNode = make_shared<Node<T>>(sourceNode);
		this->sinkNode = make_shared<Node<T>>(sinkNode);
	}

	//From my understanding, the unique_ptr that owns our edge will be what is deleted, and that will automatically take care of our shared ptrs
	~Edge()
	{
		// TODO: REMOVE
		std::cout << "Edge Name: " << this->getName() << "\ removed" << std::endl;
		std::cout << "Edge Index: " << this->getIndex() << "\ removed" << std::endl;
	}

	/************************************************
	 *  GETTER/SETTER PAIRS
	 ***********************************************/
	void setIndex(unsigned short int index);
	unsigned short int getIndex() const;

	void setName(std::string name);
	std::string getName() const;

	void setLabels(std::vector<std::string> labels);
	std::vector<std::string> getLabels();

	void setIsBridge(bool bridge);
	bool getIsBridge() const;

	void setIsVisited(bool visited);
	bool getIsVisited() const;

	/* TODO: Do we want to protect our get node with const?
	 *
	 */
	void setSourceNode(std::shared_ptr<Node<T>> newSource);
	std::shared_ptr<Node<T>> getSourceNode();

	void setSinkNode(std::shared_ptr<Node<T>> newSink);
	std::shared_ptr<Node<T>> getSinkNode();

	/************************************************
	 *  MUTATORS
	 ***********************************************/
	void addLabel(std::string label);
	void addLabel(std::vector<std::string> labels);

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
	bool bridge;

	/************************************************
	 *  ALGORITHM TRAVERSAL ATTRIBUTES
	 ***********************************************/
	bool visited;

	/************************************************
	 *  STRUCTURAL OWNERSHIP
	 ***********************************************/
	std::shared_ptr<Node<T>> sourceNode; 	//node we are coming from
	std::shared_ptr<Node<T>> sinkNode;		//node we are going to

};

#endif /* INC_EDGE_H_ */

