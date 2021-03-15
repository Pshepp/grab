#ifndef INC_EDGE_H_
#define INC_EDGE_H_

#include <memory>
#include <vector>

#include "../lazyPrints.h"

template<class T> class node;
template<class T, class E> class graph;

const bool edgeDebug = true;

template<class T>
class edge
{
public:
	/************************************************
	 *  CONSTRUCTORS/DESTRUCTORS
	 ***********************************************/
	edge();
	edge(std::string name, std::shared_ptr<node<T>> sourceNode,
			std::shared_ptr<node<T>> sinkNode);
	//edge(std::string name, std::weak_ptr<Graph<T, E>> somePtrToGraph, std::shared_ptr<node<T>> sourceNode, std::shared_ptr<node<T>> sinkNode);

	~edge();

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

	//replace w: void setSourceNode(const std::shared_ptr<node<T> > &sourceNode)
	void setSourceNode(std::shared_ptr<node<T>> freshSource);
	//replace w: const std::shared_ptr<node<T> >& getSourceNode() const
	std::shared_ptr<node<T>> getSourceNode();

	//replace w: void setSinkNode(const std::shared_ptr<node<T> > &sinkNode)
	void setSinkNode(std::shared_ptr<node<T>> freshSink);
	//replace w: const std::shared_ptr<node<T> >& getSinkNode() const
	std::shared_ptr<node<T>> getSinkNode();

	/************************************************
	 *  MUTATORS
	 ***********************************************/
	void addLabel(std::string label);
	void addLabel(std::vector<std::string> labels);

	/* BELOW ARE FUNCTIONS THAT HAVE BEEN REMOVED BUT MAY BE ADDED AGAIN
	 *
	 */
	//void switchOwnership();
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
	bool leaf; //probably not needed but can remove later

	/************************************************
	 *  ALGORITHM TRAVERSAL ATTRIBUTES
	 ***********************************************/
	bool visited;

	/************************************************
	 *  STRUCTURAL OWNERSHIP
	 ***********************************************/
	std::shared_ptr<node<T>> sourceNode;
	std::shared_ptr<node<T>> sinkNode;

	/*TODO: If we are in a specific graph we should know which graph we are in. What
	 *			is our "default" graph when we dont actually use a graph.
	 */
	//int owningGraph;
	/************************************************
	 *  HELPER FUNCTIONS
	 ***********************************************/

};

/************************************************
 *  CONSTRUCTORS/DESTRUCTORS
 ***********************************************/

template<class T>
edge<T>::edge()
{
	this->setName("DEFAULT_EDGE_NAME");
	this->setIndex(1);
	this->setIsBridge(false);
	this->setIsLeaf(false);
	this->setIsVisited(false);
	if (edgeDebug)
	{
		std::string conMsg =
				"Warning, created an edge with no name or connecting node. This should not have been called.";
		badBehavior(__LINE__, __func__, conMsg);
	}
}

template<class T>
edge<T>::edge(std::string name, std::shared_ptr<node<T> > sourceNode,
		std::shared_ptr<node<T> > sinkNode)
{
	this->setName(name);
	this->setIndex(1);
	this->setIsBridge(false);
	this->setIsVisited(false);
	this->setSourceNode(sourceNode);
	this->setSinkNode(sinkNode);
	if (edgeDebug)
	{
		std::string conMsg = "Created an edge with name: " + this->getName() + "\n\tSource Node Name: " + sourceNode.get()->getName() + "\n\tSink Node Name: " + sinkNode.get()->getName();
		lazyInfo(__LINE__, __func__, conMsg);
	}
}

template<class T>
edge<T>::~edge()
{
	if (edgeDebug)
	{
		/*NOTE: We could delete ourself by calling a node. Bad feeling about this.
		 * 			Honestly, if we we call delete this edge to our sink node, we can just delete our unique_ptr to this.
		 *
		 * 			tl;dr: this handles ensuring we delete in proper order (handle raw then unique)
		 */
		std::string desMsg = "Deleting edge with name of: " + this->getName();
		lazyInfo(__LINE__, __func__, desMsg);
	}
}

/************************************************
 *  GETTER/SETTER PAIRS
 ***********************************************/

template<class T>
void edge<T>::setIndex(unsigned short int index)
{
	this->index = index;
}

template<class T>
unsigned short int edge<T>::getIndex() const
{
	return index;
}

template<class T>
void edge<T>::setName(std::string name)
{
	this->name = name;
}

template<class T>
std::string edge<T>::getName() const
{
	return name;
}

template<class T>
void edge<T>::setLabels(std::vector<std::string> labels)
{
	this->labels = labels;
}

template<class T>
std::vector<std::string> edge<T>::getLabels() const
{
	return labels;
}

template<class T>
void edge<T>::setIsLeaf(bool leaf)
{
	this->leaf = leaf;
}

template<class T>
bool edge<T>::getIsLeaf() const
{
	return leaf;
}

template<class T>
void edge<T>::setIsBridge(bool bridge)
{
	this->bridge = bridge;
}

template<class T>
bool edge<T>::getIsBridge() const
{
	return bridge;
}

template<class T>
void edge<T>::setIsVisited(bool visited)
{
	this->visited = visited;
}

template<class T>
bool edge<T>::getIsVisited() const
{
	return visited;
}

template<class T>
void edge<T>::setSourceNode(std::shared_ptr<node<T> > freshSource)
{
	this->sourceNode = freshSource;
}

template<class T>
std::shared_ptr<node<T> > edge<T>::getSourceNode()
{
	return this->sourceNode;
}

template<class T>
void edge<T>::setSinkNode(std::shared_ptr<node<T> > freshSink)
{
	this->sinkNode = freshSink;
}

template<class T>
std::shared_ptr<node<T> > edge<T>::getSinkNode()
{
	return this->sinkNode;
}

/************************************************
 *  MUTATORS
 ***********************************************/

template<class T>
void edge<T>::addLabel(std::string label)
{
	this->labels.push_back(label);
}

template<class T>
void edge<T>::addLabel(std::vector<std::string> labels)
{
	for (std::string currLabel : labels)
		this->addLabel(currLabel);
}

#endif /* INC_EDGE_H_ */
