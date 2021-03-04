/**
 * @file main.cpp
 * @author Preston Sheppard
 * @date 01 Mar 2021
 * @brief A quick driver to test our graph class.
 *
 * We are ensuring the following work:
 * 	- Node deletion
 * 	- Node addition
 *
 * 	TODO:
 * 			- Create a "helper" class/functions that will create our adj matricies/adj lists
 * 				in order to ensure our data is being manipulated correctly.
 * 			- Take Olivers abstraction of lists etc. We will probably want to do some nonfunctional
 * 				reqs with the bio nerds to make sure we abstract all data that they will want between
 * 				classes
 * 			- Implement our nodes being owned by a graph structure.
 * 			- FIX SET INDEX!! Proper index tracking for all of our nodes and edges
 *
 */



#include <iostream>
#include "../inc/graph.h"
#include "../inc/node.h"
#include "../inc/edge.h"

class Atom
{
	public:
	Atom(std::string name) : atomName (name)
	{
		atomNodePtr = std::make_shared<Node<Atom>>(name);
	}
	void addBond(Atom *otherAtom)
	{
		this->atomNodePtr.get()->addNeighbor(otherAtom->getNode(), this->getName() + "->" + otherAtom->getName());
	}
	void removeBond(Atom *otherAtom)
	{
		atomNodePtr->deleteEdge(otherAtom->getNode());
	}
	//std::vector<std::shared_ptr<Atom>> getNeighbors()
	//	{
	//	return this->getNode()->getNeighbors();
	//	}
	const std::shared_ptr<Node<Atom>> getNode()
		{
		return atomNodePtr;
		}
	std::string getName()
	{
		return atomName;
	}
	private:
	std::shared_ptr<Node<Atom>> atomNodePtr;
	std::string atomName;
};

int main()
{
	Atom  * atom0 = new Atom("my");
	Atom * atom1 = new Atom("name");

	atom0->addBond(atom1);
	atom0->removeBond(atom1);



	std::cout << "Yes this is our main lole" << std::endl;
			return 0;
}
