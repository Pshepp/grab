/* IMPROPER UNIQUE_PTR RELEASE WHEN DELETING
 *
 */

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
 *
 * Please note that not only are our nodes "owned" by the edges, but also by the over-arching graph
 * structure itself. This way we are able to allow nodes to be within the graph structure yet have no
 * edges.
 *
 * ADD MORE INFO/CLARIFICATION AS NEEDED
 */

/*	TODO: The following are in no particular order:
 * 			- Increase our speed using unordered_set, etc. Please keep in mind that we will have to ensure our iterator is correct.
 * 			- Properly track our node index and figure out the pipeline the adjacency matrix/list should use.
 * 					- Fix: Nest our set of edge pointers within a hashmap where key = graph ptr or something
 * 						robustly hashable and value is to our vector of ptrs for our nodes :o)
 * 			- Fix node presence in multiple graph structures, this will take adding a "layer" to our edges that correlates to each structure
 * 				- The following will be within the new "layer" due to their implications in the graph structure itself:
 * 						- leaf, bridge, visited, inEdges, outEdges
 * 			- Possibly implement a relationship check (i.e Node A == Node B) for hunting and raw ptr checks. Will make finding a specific
 * 				node to delete much easier.
 * 			- If memory footprint is too large, begin passing by reference for some of our functions i.e.
 * 					- label addition, etc.
 * 					- returning of our neighbors
 * 			- Algos (mostly worried about how we want to call them, should become clear once all are clean):
 * 				- Leaf detection
 * 				- Bridge detection
 * 			- Good error throwing instead of cout laziness
 */

#ifndef INC_NODE_H_
#define INC_NODE_H_

const bool nodeDebug = false;

// TODO: REMOVE AFTER BUG TESTING
#include <iostream>

#include <vector>
#include <string>
#include <memory>
#include <algorithm>

//cannot use forward declare due to creating our edges within our node class
#include "edge.h"
#include "lazyPrints.h"

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
		if (nodeDebug)
			lazyInfo(__LINE__, __func__, "Default Node Constructor");
	}

	//do we actually need an internal pointer to the object? Why not just use [OBJ].get() or something of the same?
	Node(std::string name)
	{
		this->setName(name);
		this->setIndex(1);
		this->setIsLeaf(false);
		this->setIsBridge(false);
		this->setIsVisited(false);
		if (nodeDebug)
			lazyInfo(__LINE__, __func__, "Node Constructor taking in a Name");
	}

	// To properly remove a node we are mostly worried about dangling pointers, will be taken care in our "deleteAllEdges()" member. Must think regarding edge cases.
	~Node()
	{
		arraysBorkCheck();
		this->deleteAllEdges();
		if (true)
		{
			std::string nodeInfo = "\t\tNode Name: " + this->getName()
					+ " removed" + "\n\t\tNode Index: "
					+ std::to_string(this->getIndex()) + " removed\n";
			lazyInfo(__LINE__, __func__, "Node Destructor: \n" + nodeInfo);
		}
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

	/* TODO: See if this is what we should protect with const. Makes much more sense
	 * 			compared to our vec of neighbors, possibly return by ref. Possibly include
	 * 			an overload that signifies "walking" to another node by changing our visited flag.
	 * 			This would also be changing the connecting edge's visited flag.
	 *
	 */
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

	void addNeighbor(std::shared_ptr<Node<T>> neighborToAdd,
			std::string edgeName); //calling node is parent
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
	bool isNeighbor(std::shared_ptr<Node<T>> possibleNeighbor);
	bool isChild(std::shared_ptr<Node<T>> possibleParent);
	bool isParent(std::shared_ptr<Node<T>> possibleChild);

	bool containsLabel(std::string labelToCheck);
	bool containsLabel(std::vector<std::string> labelToCheck);

private:
	//TODO: check out size of ish because getting bad alloc
	void arraysBorkCheck()
	{
		std::cout << "In Edges size: " << this->inEdges.size() << std::endl;
		std::cout << "Out Edges size:" << this->outEdges.size() << std::endl;
	}

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

	void deleteInEdge(Edge<T> *inEdgeToDelete);
	void deleteOutEdge(Edge<T> *outEdgeToDelete); //should be fine not having to get our unique ptr, we are working only within scope of class, possibly incorrect

	bool hasInEdge(Edge<T> *possibleInEdge);
	bool hasOutEdge(Edge<T> *possibleOutEdge);

	Edge<T>* getConnectingEdge(std::shared_ptr<Node<T>> nodeB);
};

/************************************************
 *  GETTER/SETTER PAIRS
 ***********************************************/

/**
 * @brief Set our UNIQUE index of the current node
 *
 * @pre We know our unique index
 * @post the index given to the node is actually unique
 * @tparam T
 * @param index our unique index value
 */
template<class T>
void Node<T>::setIndex(unsigned short int index)
{
	this->index = index;
}

/**
 * @brief Get our unique index of the current node
 *
 * @pre We have an index and it is unique
 * @return Our unique index value
 */
template<class T>
unsigned short int Node<T>::getIndex() const
{
	return this->index;
}

/**
 * @brief Set the calling node's name. Does not have to be unique.
 *
 * @post Node's name is name
 * @tparam T
 * @param name What we want to set the node's name to.
 */
template<class T>
void Node<T>::setName(std::string name)
{
	this->name = name;
}

/**
 * @brief Get the calling node's name.
 *
 * @pre Current node actually has a name
 * @return name What our current node's name is
 */
template<class T>
std::string Node<T>::getName() const
{
	return this->name;
}

/**
 * @brief Replace the current labels within our node with a new group of labels.
 *
 * This should be fine, I think I am using my C brain when worrying too much about the copying of vectors.
 *
 * @post This nodes labels now equal the input
 * @param labels Our list of labels to use
 */
template<class T>
void Node<T>::setLabels(std::vector<std::string> labels)
{
	this->labels = labels;
}

/**
 * @brief Get a vector of our labels within the node.
 *
 * Get a vector of our labels within the node, not THE vector itself. We want to follow good encapsulation practices.
 * Passing a reference would make our vectors privacy nothing.
 *
 *
 * @return labels Our current node's labels
 */
template<class T>
std::vector<std::string> Node<T>::getLabels() const
{
	return this->labels;
}

/**
 * @brief This allows us to set if the node is a leaf.
 *
 * A node being a leaf means that we are only connected to a single other node. Typically, it means that
 * there is only one connecting edge from this node to a single other, but this is acting in a more generalized sense.
 * This will be used to help our searching/structure observing algos run faster since we know that a leaf cannot be a part
 * of a cycle.
 *
 * @pre We know if this node is a leaf or not
 * @post The signifier is correctly set.
 * @param leaf
 */
template<class T>
void Node<T>::setIsLeaf(bool leaf)
{
	this->leaf = leaf;
}

/**
 * @brief Get if our node is a leaf or not.
 *
 * @pre We have run our "leaf detection" algo in order to tell if our node is a leaf.
 * @return leaf Whether or not our node is a leaf.
 */
template<class T>
bool Node<T>::getIsLeaf() const
{
	return this->leaf;
}

/**
 * @brief Sets if the current node is a bridge.
 *
 * A current node is a bridge if it is not present within any cycles and is not a leaf. Please note that this means
 * we CANNOT have a node that is both a leaf and a bridge within the same graph structure. This will be used by our
 * algos to run faster, we can use this to help decompose cycles since "mega" cycles will be connected by bridges which
 * will make our case for ensuring we are within the same "mega" cycle much easier.
 *
 * If we are traversing our "mega" cycle/building it etc. we know that when we hit a bridge that we are no longer within
 * the mega cycle. This can be shortened more by checking the connecting edge properties since there is a member that
 * denotes if the edge is acting as a bridge.
 *
 * @pre We know if current node is a bridge
 * @post The signifier is correctly set.
 * @param bridge
 */
template<class T>
void Node<T>::setIsBridge(bool bridge)
{
	this->bridge = bridge;
}

/**
 * @brief Get if our node is a bridge or not.
 *
 * @pre We have run our "bridge detection" algo in order to tell if our node is a bridge.
 * @return bridge Whether or not our node is a bridge
 */
template<class T>
bool Node<T>::getIsBridge() const
{
	return this->bridge;
}

/**
 * @brief To assist in algo traversal we may want to keep track of if we are visited or not.
 *
 * The use case of this is the same as our *isLeaf() *isBridge() but once all algos are completed we must reset
 * all visited to false.
 *
 * @pre We are running in an algo that needs to know when a specific node is visited. Or we are setting all
 * 			our nodes to not being visited once running through an algo.
 * @post We have set the visited proper depending on our use case
 * @param visited
 */
template<class T>
void Node<T>::setIsVisited(bool visited)
{
	this->visited = visited;
}

/**
 * @brief Want to get if our node has been visited yet.
 *
 * Please refer to the setIsVisited regarding use cases, etc.
 *
 * @pre our visited member is correctly set.
 * @return visited If current node has been visited
 */
template<class T>
bool Node<T>::getIsVisited() const
{
	return this->visited;
}

/**
 * @brief Checks all neighbors and returns a single neighbor that has the same name
 *
 * This will most likely break when usign the unique_ptr case
 *
 * @pre Calling node actually exists, all names are unique
 * @post
 * @param name The name of the node we are looking for
 * @return the Shared pointer to the appropriate node or null if it does not exist
 */
template<class T>
std::shared_ptr<Node<T> > Node<T>::getNeighborByName(std::string name)
{
	std::vector<std::shared_ptr<Node<T>>> neighbors = this->getNeighbors();
	for (auto &possibleHit : neighbors)
	{
		if (possibleHit.get()->getName() == name)
		{
			if (nodeDebug)
				lazyInfo(__LINE__, __func__,
						"Found Neighbor of " + this->getName() + " with name :"
								+ name);
			return possibleHit;
		}
	}
	badBehavior(__LINE__, __func__);
	return NULL;
}

/**
 * @brief Checks all neighbors and gets one that matches the searched for index
 *
 * @pre Calling node actually exists
 * @post
 * @param index The value we want to use to find the node
 * @return the shared pointer to the appropriate node or null if it does not exist
 */
template<class T>
std::shared_ptr<Node<T> > Node<T>::getNeighborByIndex(unsigned short index)
{
	std::vector<std::shared_ptr<Node<T>>> neighbors = this->getNeighbors();
	for (auto &possibleHit : neighbors)
	{
		if (possibleHit.get()->getIndex() == index)
		{
			if (nodeDebug)
				lazyInfo(__LINE__, __func__,
						"Found Neighbor of " + this->getName() + " with index :"
								+ index);
			return possibleHit;
		}
	}
	badBehavior(__LINE__, __func__);
	return NULL;
}

/**
 * @brief Gets all neighbors to a specific node
 *
 * 	Note that we may need to worry about if outEdge/inEdge has nothing in them but this should not be the case.
 *
 * @pre calling node actually exists
 * @return neighbors A vector of shared pointers to all the calling node's neighbors
 */
template<class T>
std::vector<std::shared_ptr<Node<T> > > Node<T>::getNeighbors()
{
	std::vector<std::shared_ptr<Node<T>>> neighbors;
	//get children
	for (auto &&outEdge : this->outEdges)
	{
		if (nodeDebug)
			lazyInfo(__LINE__, __func__,
					"Child neighbor added: "
							+ outEdge->getSinkNode().get()->getName());
		neighbors.push_back(outEdge->getSinkNode());
	}
	for (auto &inEdge : this->inEdges)
	{
		if (nodeDebug)
			lazyInfo(__LINE__, __func__,
					"Parent neighbor added: "
							+ inEdge->getSourceNode().get()->getName());
		neighbors.push_back(inEdge->getSourceNode());
	}
	return neighbors;
}

/************************************************
 *  STRUCTURAL/RELATIONSHIP CHECKS
 ***********************************************/

/**
 * @brief Check if two nodes are neighbors
 *
 * Two nodes are neighbors if they share an edge, ownership and hierarchy does not matter in this case.
 *
 * TODO: Possibly implement a check to make sure the node is not calling upon itself
 *
 * @pre Know what node we want to check and that node is present.
 * @param possibleNeighbor The node we want to see if it is connected to the calling node
 * @return True if it is a neighbor, false if otherwise.
 */
template<class T>
bool Node<T>::isNeighbor(std::shared_ptr<Node<T>> possibleNeighbor)
{
	return (this->isChild(possibleNeighbor) || this->isParent(possibleNeighbor));
}

/**
 * @brief A check to see if a different node is a parent to the calling node.
 *
 * A different node is a parent to this node if the connecting edge is owned
 * by the other node. This allows for "direction".
 *
 * @pre Know what node we want to check and that node is present
 * @param possibleParent The node we want to check if it is connect by any edges sunk by the calling node.
 * @return True if it is a parent, false if otherwise
 */
template<class T>
bool Node<T>::isChild(std::shared_ptr<Node<T>> possibleParent)
{

	for (auto const &inEdgePtr : this->inEdges)
	{
		//ensures that both of our shared ptrs go to same obj. What if null?
		if (inEdgePtr->getSourceNode().get() == possibleParent.get())
		{
			if (nodeDebug)
				lazyInfo(__LINE__, __func__,
						this->getName() + " is child of "
								+ possibleParent->getName());
			return true;
		}
	}
	return false;
}

/**
 * @brief A check to see if a different node is a child to the calling node.
 *
 * A different node is a child to this node if the connecting edge is owned by
 * this node. As with isParent, this allows for "direction". Direction is from parent
 * to child.
 *
 * @pre Know what node we want to check and that the node is present
 * @param possibleChild The node we want to check if it is connected by any edges sourced by the calling node.
 * @return
 */
template<class T>
bool Node<T>::isParent(std::shared_ptr<Node<T>> possibleChild)
{
	//lazy switch, just check if our child is actually a child of the calling node.
	if (nodeDebug)
		lazyInfo(__LINE__, __func__,
				"Checking if " + this->getName() + " is parent of "
						+ possibleChild->getName());
	return possibleChild.get()->isChild(this->shared_from_this());

//cannot convert ‘Node<Atom>*’ to ‘std::shared_ptr<Node<Atom> >’
}

/**
 * @brief Check if our current node contains the given label
 *
 * @pre We know what label we are looking for
 * @post We know if said label is present
 * @param labelToCheck This is the label in question
 * @return True if the label is present, false if not.
 */
template<class T>
bool Node<T>::containsLabel(std::string labelToCheck)
{
	return (std::find(this->labels.begin(), this->labels.end(), labelToCheck)
			!= this->labels.end()) ? true : false;
}

/**
 * @brief Checks a vector of labels to the current nodes labels. Final implementation is up for discussion.
 *
 * NOTE: Maybe go ahead and return a vec of all common members and if returned vec.size() == 0 then we know they don't share any
 * TODO: Figure out what the goons want from this, let's prepare now to make life easy. Right now just lazy check all
 * Right now we just return true if there are any commons. Do we want to check proper vs improper subset etc.
 *
 * @pre Know what labels we are looking for
 * @post Given if a single element is shared. Potential to change
 * @param labelsToCheck
 * @return true if contains a single shared label, false if not.
 */
template<class T>
bool Node<T>::containsLabel(std::vector<std::string> labelsToCheck)
{
	for (std::string currLabel : labelsToCheck)
		return this->containsLabel(currLabel);
	return false;
}

/************************************************
 *  MUTATORS
 ***********************************************/

/**
 * @brief Add a label to the end of our node's member vector of labels.
 *
 *	TODO: Do we want to prevent duplicate labels?
 *
 * @pre We know what we want to add the label to be
 * @post The label is now added to the member vector
 * @param label Is the label we want to add to the
 */
template<class T>
void Node<T>::addLabel(std::string label)
{
	this->labels.push_back(label);
}

/**
 * @brief Add a bunch of labels to the end of our node's member vector of labels
 *
 * Though this may be useful. Just a little helper. Got lazy for addition
 *
 * @pre We know what our added labels should be.
 * @post All of our labels are added to the end of the membervector
 * @param labels All the labels we want to throw onto the end
 */
template<class T>
void Node<T>::addLabel(std::vector<std::string> labels)
{
	for (std::string currLabel : labels)
		this->addLabel(currLabel);
}

/************************************************
 *  GETTER/SETTER PAIRS
 ***********************************************/

/**
 * @brief Add a neighbor node as a child to the calling node
 *
 * Please note that there may be an issue passing our this.node to the function
 *
 * @pre Both nodes exist
 * @post There is now an edge linking the two nodes from paretn to child
 * @param neighborToAdd The neighbor we want to create a link to
 * @param edgeName A name we would like to set the edge to
 */
template<class T>
void Node<T>::addNeighbor(std::shared_ptr<Node<T> > neighborToAdd,
		std::string edgeName)
{
	Edge<T> *freshEdge = new Edge<T>(edgeName, this->shared_from_this(),
			neighborToAdd);
	std::unique_ptr<Edge<T>> newUniqueEdge(freshEdge);
	neighborToAdd.get()->inEdges.push_back(freshEdge);
	this->outEdges.push_back(std::move(newUniqueEdge));
}

//TODO: Is there really a need for this? Honestly IMO no, we would want to delete a node/neighbor from our graph structure class
template<class T>
void Node<T>::deleteNeighbor(std::shared_ptr<Node<T>> neighborToRemove)
{
	std::cout << "NOT IMPLEMENTED PROBABLY DONT NEED" << std::endl;
}

/**
 * @brief Deletes an edge that is between two connecting nodes.
 *
 * How would we like to worry regarding error handling, as of now this assumes at most 1
 * edge between two nodes.
 *
 * @pre The two nodes exist
 * @post If the nodes were connected then the connection (edge) is removed
 * @param secondNode
 */
template<class T>
void Node<T>::deleteEdge(std::shared_ptr<Node<T> > secondNode)
{
	if (!this->isNeighbor(secondNode))
	{
		badBehavior(__LINE__, __func__);
		return;
	}
	arraysBorkCheck();
	Edge<T> *tempLazy = this->getConnectingEdge(secondNode);
	lazyInfo(__LINE__, __func__,
			"Edge connecting (" + this->getName() + ") to ("
					+ secondNode->getName() + ") to be deleted");
	tempLazy->getSourceNode().get()->deleteOutEdge(tempLazy);
	if (this->isNeighbor(secondNode))
		badBehavior(__LINE__, __func__, "EDGES ARE STILL NEIGHBORS");
}

/**
 * @brief Deletes all edges connected to the calling node
 *
 * @pre
 * @post The calling node has no edges connected, neither in nor out
 */
template<class T>
void Node<T>::deleteAllEdges()
{
	/* TODO: THE LOOPING THROUGH CAUSES DEATH, RECALL IT CHANGES OUR INDICIES. NEED TO ACTUALLY USE ITERATOR
	 *
	 */
	lazyInfo(__LINE__, __func__,
			"Delting all edges of node: " + this->getName());
	//Warning O(N^2), could be worse if we worry about the internal loops we begin calling
	arraysBorkCheck();
	for (auto &&outEdge : this->outEdges)
		this->deleteOutEdge(outEdge.get());
	//we just travers to the source of our edge then delete the outer edge
	for (auto &inEdge : this->inEdges)
		inEdge->getSourceNode().get()->deleteOutEdge(inEdge);
	if ((this->inEdges.size() != 0) && (this->outEdges.size() != 0))
		badBehavior(__LINE__, __func__);
}

/**
 * @brief Used to switch the ownership of an edge
 *
 * This is used to switch our edge "direction" which will do so regardless if a parent calls or child calls this.
 * It is easier if we call the raw ptr first idk why my brain (i.e. we are starting at our child node then switching)
 *
 * TODO: Clean
 *
 * @pre the two nodes share an edge
 * @post the nodes edges are switched, thus if the calling node was the parent it is now the child and vice-versa
 * @param nodeB the node we want to share an edge with.
 */
template<class T>
void Node<T>::switchEdgeDirection(std::shared_ptr<Node<T> > nodeB)
{
	/*
	 * NOTE: Let us move our unique_ptr that was in our original parent and move it to our new parent
	 */

	if (!(this->isNeighbor(nodeB)))
	{
		badBehavior(__LINE__, __func__);
		return;
	}
	//Gross

	Edge<T> *connectingEdge = this->getConnectingEdge(nodeB);

	/* We are ensuring both nodes actually have the edge we are trying to swap. This is mostly
	 * due to me being paranoid about if this sand castle will actually hold together. Helps ensure our data
	 * stays clean and will die if borked data
	 */
	if ((connectingEdge->getSourceNode().get()->hasOutEdge(connectingEdge))
			&& !(connectingEdge->getSourceNode().get()->hasInEdge(
					connectingEdge))
			&& !(connectingEdge->getSinkNode().get()->hasOutEdge(connectingEdge))
			&& (connectingEdge->getSinkNode().get()->hasInEdge(connectingEdge)))
	{
		//get current structure to check later
		bool isCallingSource = this->hasOutEdge(connectingEdge);
		bool isCallingSink = this->hasInEdge(connectingEdge);

		bool checkedNodeSource = nodeB.get()->hasOutEdge(connectingEdge);
		bool checkedNodeSink = nodeB.get()->hasInEdge(connectingEdge);

		//should not hit here due to neighbor check but stay noided, will hit if edge sourced and sunk by same node.
		if ((isCallingSource == isCallingSink)
				|| (checkedNodeSource == checkedNodeSink))
		{
			badBehavior(__LINE__, __func__);
			return;
		}
		//release the unique ptr, delete the one that now pts to null, add our raw to previous unique node, create new unique and put in our previous nodes raw vector

		//sketti, first we find, release, then delete the previous unique_ptr
		for (int currIndex = 0;
				currIndex
						< connectingEdge->getSourceNode().get()->outEdges.size();
				currIndex++)
		{
			if (connectingEdge->getSourceNode().get()->outEdges[currIndex].get()
					== connectingEdge) //found what we want
			{
				//found correct edge, so let us move our unique ptr into our sink node that is now our source (i.e. out)
				connectingEdge->getSinkNode().get()->outEdges(
						std::move(
								connectingEdge->getSourceNode().get()->outEdges[currIndex]));
				//remove the now to null unique_ptr from out source node relation, delete by index cause easier for now
				connectingEdge->getSourceNode().get()->outEdges.erase(
						connectingEdge->getSourceNode().get()->outEdges.begin()
								+ currIndex);
				lazyInfo(__LINE__, __func__,
						"Our unique ptr to edge  " + connectingEdge->getName()
								+ "\n\t\tnow in: "
								+ connectingEdge->getSinkNode().get()->getName()
								+ "\n\t\twas in: "
								+ connectingEdge->getSourceNode().get()->getName());

			}
		} //end our for loop that finds proper location for unique ptr

		//now remove from our original sink node and put it in our new sink node
		connectingEdge->getSinkNode().get()->deleteInEdge(connectingEdge);
		connectingEdge->getSourceNode().get()->inEdges.push_back(
				connectingEdge);
		lazyInfo(__LINE__, __func__,
				"Before Switch: \n\tEdge Source:"
						+ connectingEdge->getSourceNode().get()->getName()
						+ "\n\tEdge Sink: "
						+ connectingEdge->getSinkNode().get()->getName());
		connectingEdge->switchOwnership();
		lazyInfo(__LINE__, __func__,
				"After Switch: \n\tEdge Source:"
						+ connectingEdge->getSourceNode().get()->getName()
						+ "\n\tEdge Sink: "
						+ connectingEdge->getSinkNode().get()->getName());

		//now we check to make sure this worked proper
		//these should all flip at the end of this, if one of them doesnt we know we have an issue
		if ((isCallingSource == this->hasOutEdge(connectingEdge))
				|| (isCallingSink == this->hasInEdge(connectingEdge))
				|| (checkedNodeSource == nodeB.get()->hasOutEdge(connectingEdge))
				|| (checkedNodeSink == nodeB.get()->hasInEdge(connectingEdge)))
			badBehavior(__LINE__, __func__);
		else
			lazyInfo(__LINE__, __func__, "Switch successful");
	}
	else
	{
		badBehavior(__LINE__, __func__);
		return;
	}
}

/************************************************
 *  HELPER FUNCTIONS
 ***********************************************/

/**
 * @brief Helper function to delete an edge sunk by the calling node
 *
 * Note that this only removes the value from our inEdge vector. Should probably worry about
 * updating our edge sink member but that is a TODO
 *
 * @pre
 * @post
 * @tparam T
 * @param inEdgeToDelete
 */
template<class T>
void Node<T>::deleteInEdge(Edge<T> *inEdgeToDelete)
{
	arraysBorkCheck();
	if (hasInEdge(inEdgeToDelete))
	{
		this->inEdges.erase(
				std::remove(this->inEdges.begin(), this->inEdges.end(),
						inEdgeToDelete), this->inEdges.end());

		lazyInfo(__LINE__, __func__, "Edge TO " + this->getName() + " deleted");
		arraysBorkCheck();
	}
	else if (this->hasOutEdge(inEdgeToDelete))
	{
		lazyInfo(__LINE__, __func__,
				"*** In edge was actually an out edge of node "
						+ this->getName());
	}
	else if (this->inEdges.size() == 0)
	{
		lazyInfo(__LINE__, __func__, "Yo no edges here bro");
	}
	else
		badBehavior(__LINE__, __func__);
}

/**
 * @brief Helper to delete an edge sourced by the calling node
 *
 * Keep in mind that the only thing not wrapped by a smart pointer is the rear raw ptr reference within a sink node.
 * Thus if we ever delete an outgoing edge from a node we must ensure that we have no dangling raws.
 *
 * @pre
 * @post
 * @param outEdgeToDelete
 */
template<class T>
void Node<T>::deleteOutEdge(Edge<T> *outEdgeToDelete)
{
	lazyInfo(__LINE__, __func__, "Entering into delete out edge");
	arraysBorkCheck();
	if (this->hasOutEdge(outEdgeToDelete))
	{
		//as of now let us just delete the value using index, worst case O(N) but thing is we at most have ~5 bonds in our use case
		for (unsigned int currIndex = 0; currIndex < this->outEdges.size();
				currIndex++)
		{
			if (this->outEdges[currIndex].get() == outEdgeToDelete)
			{
				//we must delete the raw ptr reference in our child node, so we grab it then delete this edge that is going into the node
				outEdgeToDelete->getSinkNode().get()->deleteInEdge(
						outEdgeToDelete);
				this->outEdges.erase(this->outEdges.begin() + currIndex);
				lazyInfo(__LINE__, __func__,
						"Edge FROM " + this->getName() + " deleted");
			}
		}
	}
	else if (this->hasInEdge(outEdgeToDelete))
	{
		lazyInfo(__LINE__, __func__,
				"*** Out edge was actually an in edge of node "
						+ this->getName());
	}
	else if (this->outEdges.size() == 0 || this->inEdges.size() == 0)
	{
		lazyInfo(__LINE__, __func__, "Yo no edges here bro");
	}
	else
	{
		std::cout << "=================== EDGE CHECK =============="
				<< std::endl;
		for (Edge<T> *ed : this->inEdges)
		{
			std::cout << ed->getName() << ", ";
		}
		std::cout << "\n";

		for (int i = 0; i < this->inEdges.size(); i++)
		{
			std::cout << this->inEdges[i]->getName() << ", ";
		}
		std::cout << "\n";

		//todo: what are the values within our vector
		arraysBorkCheck();
		badBehavior(__LINE__, __func__);
	}
}

/**
 * @brief Helper function to vainlinelidate if we do contain (poitned to by) a specific edge
 *
 * @pre We have an edge and node to check against
 * @param possibleInEdge
 * @return True if we sink that edge, false otherwise
 */
template<class T>
bool Node<T>::hasInEdge(Edge<T> *possibleInEdge)
{
	for (auto &inEdgePtr : this->inEdges)
	{
		if (inEdgePtr == possibleInEdge)
			return true;
	}
	return false;
}

/**
 * @brief Helper function to validate we do contain (own) a specific edge.
 *
 * Please note that this using a pointer to the object itself which bypasses
 * our smart pointer use. This is done for convenience, please note this.
 *
 * @pre We have a node and edge to check with and you are not abusing the ownership hierarchy
 * @param possibleOutEdge Edge to see if we have
 * @return True if we own the edge, false otherwise
 */
template<class T>
bool Node<T>::hasOutEdge(Edge<T> *possibleOutEdge)
{
	for (auto &&outEdgePtr : this->outEdges)
	{
		if (outEdgePtr.get() == possibleOutEdge)
			return true;
	}
	return false;
}

/**
 * @brief A helper function that returns the connecting edge between two nodes.
 *
 * Even though this is a helper function, it is a very dangerous one which passes the RAW
 * pointer to our edge. The main reasoning for this is to make it much easier to perform
 * operations with out node, but again be careful using this.
 *
 * Steals the logic from our checks, but we just want to hide this functionality due to it
 * being dangerous
 *
 * TODO: - Return a vector instead for when we have multiple edges between nodes. Is this needed?
 * 		 - Actually worry about this, feels very dirty and unsure if any cleaner ways to do this.
 * 		 - From my understanding this SHOULD return a reference to the pointer within the vector but am unsure
 *
 * @pre Two nodes share an edge and you promise you are not abusing the ownership hierarchy
 * @post We get the raw pointer to the edge that connects the two nodes
 * @param nodeB Node to check
 * @return a raw pointer to our edge
 */
template<class T>
Edge<T>* Node<T>::getConnectingEdge(std::shared_ptr<Node<T> > nodeB)
{
	if (this->isNeighbor(nodeB))
	{
		for (auto &inEdgePtr : this->inEdges)
		{
			if (inEdgePtr->getSourceNode().get() == nodeB.get())
			{
				std::string blurb = "Found Edge connecting " + nodeB->getName()
						+ " TO " + this->getName() + "\n";
				std::string vis = "\tVisualize: " + nodeB->getName() + " --("
						+ inEdgePtr->getName() + ")-> " + this->getName();
				lazyInfo(__LINE__, __func__,
						"Found Edge\n\t"
								+ inEdgePtr->getSourceNode().get()->getName());
				return inEdgePtr;
			}

		}
		for (auto &inEdgePtr : nodeB.get()->inEdges)
		{
			if (inEdgePtr->getSourceNode().get() == this) //will most likely need the node to know its own pointer smh, does this properly return address
			{
				std::string blurb = "Found Edge connecting " + this->getName()
						+ " TO " + nodeB.get()->getName() + "\n";
				std::string vis = "\tVisualize: " + this->getName() + " --("
						+ inEdgePtr->getName() + ")-> " + nodeB->getName();
				lazyInfo(__LINE__, __func__, blurb + vis);
				//""
				//"\n\t" + inEdgePtr->getSourceNode().get()->getName());
				return inEdgePtr;
			}

		}
	}
	badBehavior(__LINE__, __func__);
	return NULL;
}

#endif /* INC_NODE_H_ */

