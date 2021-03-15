#ifndef INC_EDGE_H_
#define INC_EDGE_H_

#include <memory>
#include <vector>

template<class T> class Node;
template<class T, class E> class Graph;

const bool edgeDebug = true;

template<class T>
class Edge
{
public:
	/************************************************
	 *  CONSTRUCTORS/DESTRUCTORS
	 ***********************************************/
	Edge();
	Edge(std::string name, std::shared_ptr<Node<T>> sourceNode,
			std::shared_ptr<Node<T>> sinkNode);
	//Edge(std::string name, std::weak_ptr<Graph<T, E>> somePtrToGraph, std::shared_ptr<Node<T>> sourceNode, std::shared_ptr<Node<T>> sinkNode);

	~Edge();

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

	//replace w: void setSourceNode(const std::shared_ptr<Node<T> > &sourceNode)
	void setSourceNode(std::shared_ptr<Node<T>> freshSource);
	//replace w: const std::shared_ptr<Node<T> >& getSourceNode() const
	std::shared_ptr<Node<T>> getSourceNode();

	//replace w: void setSinkNode(const std::shared_ptr<Node<T> > &sinkNode)
	void setSinkNode(std::shared_ptr<Node<T>> freshSink);
	//replace w: const std::shared_ptr<Node<T> >& getSinkNode() const
	std::shared_ptr<Node<T>> getSinkNode();

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
	std::shared_ptr<Node<T>> sourceNode;
	std::shared_ptr<Node<T>> sinkNode;

	/*TODO: If we are in a specific graph we should know which graph we are in. What
	 *			is our "default" graph when we dont actually use a graph.
	 */
	//int owningGraph;
	/************************************************
	 *  HELPER FUNCTIONS
	 ***********************************************/

};

#endif /* INC_EDGE_H_ */
