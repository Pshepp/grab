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

template<class T>
class Edge
{
public:
	Edge()
	{
		defaultInit();
	}

	std::vector<std::weak_ptr<Graph<T> > > parentGraphs;

	Edge(std::string inputName,	std::shared_ptr<Node<T> > sourceNode,std::shared_ptr<Node<T>> destNode)
	{
		this->setIndex(1);
		this->setName(inputName);
		this->setIsVisited(false);
		this->setIsBridge(false);
		this->sourceNode = make_shared<Node <T>>(sourceNode);
		this->destNode = make_shared<Node <T>>(destNode);
	}

	void setName(std::string inputName);
	std::string getName();

	void setLabels(std::vector<std::string> inputLabels);
	std::vector<std::string> getLabels();
	bool constainsLabel(std::string possibleLabel);

	void setIndex(unsigned short int index);
	short int getIndex();

	void setIsVisited(bool isVisited);
	bool getIsVisited();

	void setIsBridge(bool isBridge);
	bool getIsBridge();

	void setSourceNode(std::shared_ptr<Node<T>> inputSourceNode);
	std::shared_ptr<Node<T>> getSourceNode();

	void setDestNode(std::shared_ptr<Node<T>> inputDestNode);
	std::shared_ptr<Node<T>> getDestNode();

private:

	unsigned short int index;
	bool isBridge;
	bool isVisited;
	std::string name;
	std::vector<std::string> labels;

	std::shared_ptr<Node<T> > sourceNode;
	std::shared_ptr<Node<T> > destNode;

	void defaultInit();
	void defaultInit(std::string inputName,
			std::shared_ptr<Node<T>> sourceNode,
			std::shared_ptr<Node<T>> destNode);
};


/************************************************
 *  GETTERS
 ***********************************************/

template<typename T>
short int Edge<T>::getIndex()
{
	return this->index;
}

template<typename T>
std::vector<std::string> Edge<T>::getLabels()
{
	return this->labels;
}

template<typename T>
bool Edge<T>::getIsBridge()
{
	return isBridge;
}

template<typename T>
bool Edge<T>::getIsVisited()
{
	return isVisited;
}

template<typename T>
std::string Edge<T>::getName()
{
	return name;
}

/************************************************
 *  SETTERS
 ***********************************************/

template<typename T>
void Edge<T>::setIndex(unsigned short int index)
{
	this->index = index;
}

// TODO: Check that this properly replaces the member vector
template<typename T>
void Edge<T>::setLabels(std::vector<std::string> inputLabels)
{
	this->labels = inputLabels;
}

template<typename T>
void Edge<T>::setIsVisited(bool status)
{
	this->isVisited = status;
}

template<typename T>
void Edge<T>::setIsBridge(bool status)
{
	this->isBridge = status;
}

//std::vector<std::unique_ptr<Edge<T> > > outEdges;


template<typename T>
void Edge <T>::setName(std::string inputName)
{
	this->name = inputName;
}

/************************************************
 *  PRIVATE FUNCTIONS
 ***********************************************/

// CONSTRUCTION HELPERS
template<typename T>
void Edge<T>::defaultInit() //this should never be used
{
	this->setIndex(1);
	this->setName("DEFAULT_EDGE_NAME");
	this->setIsVisited(false);
	this->setIsBridge(false);
}

template<typename T>
void Edge<T>::defaultInit(std::string inputName,
		std::shared_ptr<Node<T> > sourceNode,
		std::shared_ptr<Node<T>> destNode)
{
	this->setIndex(1);
	this->setName(inputName);
	this->setIsVisited(false);
	this->setIsBridge(false);
	this->sourceNode = make_shared<Node <T>>(sourceNode);
	this->destNode = make_shared<Node <T>>(destNode);
}

#endif /* INC_EDGE_H_ */
