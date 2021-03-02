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
 */

#ifndef INC_EDGE_H_
#define INC_EDGE_H_

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
	Edge()
	{
		this->setName("DEFAULT_EDGE_NAME");
		this->setIndex(1);
		this->setIsBridge(false);
		this->setIsVisited(false);
	}

	Edge(std::string name)
	{
		this->setName(name);
		this->setIndex(1);
		this->setIsBridge(false);
		this->setIsVisited(false);
	}

	~Edge();

	/************************************************
	 *  GETTER/SETTER PAIRS
	 ***********************************************/
	void setIndex(unsigned short int index);
	unsigned short int getIndex();

	void setName(std::string name);
	std::string getName();

	void setLabels(std::vector<std::string> labels);
	std::vector<std::string> getLabels();

	void setIsBridge(bool isBridge);
	bool getIsBridge();

	void setIsVisited(bool isVisited);
	bool getIsVisited();

	void setSourceNode(std::shared_ptr<Node<T>> newSource);
	std::shared_ptr<Node<T>> getSourceNode();

	void setSinkNode(std::shared_ptr<Node<T>> newSink);
	std::shared_ptr<Node<T>> getSinkNode();

	/************************************************
	 *  MUTATORS
	 ***********************************************/
	void addLabel(std::string label);
	void addLabel(std::vector<std::string> labels); //note that we will just overload our singular add, rename to addLables?

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
	//we do not include isLeaf due to a node being a leaf but not an edge.
	bool isBridge;

	/************************************************
	 *  ALGORITHM TRAVERSAL ATTRIBUTES
	 ***********************************************/
	bool isVisited;

	/************************************************
	 *  STRUCTURAL OWNERSHIP
	 ***********************************************/
	std::shared_ptr<Node<T>> sourceNode; 	//node we are coming from
	std::shared_ptr<Node<T>> sinkNode;		//node we are going to

};

#endif /* INC_EDGE_H_ */

