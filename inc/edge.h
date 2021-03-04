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

	Edge(std::string name, std::shared_ptr<Node<T>> sourceNode,
			std::shared_ptr<Node<T>> sinkNode)
	{
		this->setName(name);
		this->setIndex(1);
		this->setIsBridge(false);
		this->setIsVisited(false);
		this->sourceNode = std::make_shared<Node<T>>(sourceNode);
		this->sinkNode = std::make_shared<Node<T>>(sinkNode);
	}

	//From my understanding, the unique_ptr that owns our edge will be what is deleted, and that will automatically take care of our shared ptrs
	~Edge()
	{
		// TODO: REMOVE
		std::cout << "REMOVING Edge Name: " << this->getName() << std::endl;
		std::cout << "REMOVING Edge Index: " << this->getIndex() << std::endl;
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

	/* TODO: Bridge detection
	 *
	 * Just check if part of sep "mega" cycles?
	 */
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

	void switchOwnership();

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

/************************************************
 *  GETTER/SETTER PAIRS
 ***********************************************/

template<class T>
void Edge<T>::setIndex(unsigned short int index)
{
	this->index = index;
}

template<class T>
unsigned short int Edge<T>::getIndex() const
{
	return this->index;
}

template<class T>
void Edge<T>::setName(std::string name)
{
	this->name = name;
}

template<class T>
std::string Edge<T>::getName() const
{
	return this->name;
}

template<class T>
void Edge<T>::setLabels(std::vector<std::string> labels)
{
	this->labels = labels;
}

template<class T>
std::vector<std::string> Edge<T>::getLabels()
{
	return this->labels;
}

template<class T>
void Edge<T>::setIsBridge(bool bridge)
{
	this->bridge = bridge;
}

template<class T>
bool Edge<T>::getIsBridge() const
{
	return this->bridge;
}

template<class T>
void Edge<T>::setIsVisited(bool visited)
{
	this->visited = visited;
}

template<class T>
bool Edge<T>::getIsVisited() const
{
	return this->visited;
}

template<class T>
void Edge<T>::setSourceNode(std::shared_ptr<Node<T> > newSource)
{
	this->sourceNode = newSource;
}

template<class T>
std::shared_ptr<Node<T> > Edge<T>::getSourceNode()
{
	return this->sourceNode;
}

template<class T>
void Edge<T>::setSinkNode(std::shared_ptr<Node<T> > newSink)
{
	this->sinkNode = newSink;
}

template<class T>
std::shared_ptr<Node<T> > Edge<T>::getSinkNode()
{
	return this->sinkNode;
}

/************************************************
 *  MUTATORS
 ***********************************************/
template<class T>
void Edge<T>::addLabel(std::string label)
{
	this->labels.push_back(label);
}

template<class T>
void Edge<T>::addLabel(std::vector<std::string> labels)
{
	for (std::string currLabel : labels)
		this->addLabel(currLabel);
}

template<class T>
void Edge<T>::switchOwnership()
{
	this->sourceNode.swap(this->sinkNode);
}

#endif /* INC_EDGE_H_ */

