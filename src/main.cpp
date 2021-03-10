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
 * 			- Switch our vectors to maps
 * 			- Valgrind & integration into CI/CD -> ask godfrey regarding this
 *
 */

#include <iostream>
#include <memory>
#include "../inc/graph.h"
#include "../inc/node.h"
#include "../inc/edge.h"

class Atom
{
public:
	Atom(std::string name) :
			atomName(name)
	{
		atomNodePtr = std::make_shared<Node<Atom>>(name);
	}
	void addBond(Atom *otherAtom)
	{
		this->atomNodePtr.get()->addNeighbor(otherAtom->getNode(),
				this->getName() + "->" + otherAtom->getName());
	}
	void removeBond(Atom *otherAtom)
	{
		atomNodePtr->deleteEdge(otherAtom->getNode());
	}
	void printBondedAtoms()
	{
		std::vector<std::weak_ptr<Node<Atom>>> neighbors =
				this->getNode().get()->getNeighbors();
		std::vector<std::string> atomNames;
		std::vector<std::string> bondNames;
		for (auto &atom : neighbors)
		{
			if (atom.lock())
			{
				std::shared_ptr<Node<Atom>> temp = atom.lock();
				atomNames.push_back(temp->getName());
				if (temp.get()->getConnectingEdge(this->getNode()) != NULL)
				{
					bondNames.push_back(temp.get()->getConnectingEdge(this->getNode())->getName());
				}
				else
				{
					badBehavior(__LINE__, __func__, "We couldnt find our connecting edge!!");
				}
			}
		}
		lazyInfo(__LINE__, __func__, "Printing bonded atoms to atom: " + this->getName());
		std::cout <<"~~~Bonded Atom/Neighbor Names:" << std::endl;
		for (auto& i : atomNames)
		{
			std::cout << i << std::endl;
		}
		std::cout<<"\n~~~The bonds between our atom and its neighbors:" <<std::endl;
		for (auto& i : bondNames)
		{
			std::cout<<i<<std::endl;
		}

		std::cout << std::endl
				<< "Finished printing bonded atoms for atom: " + this->getName()
				<< std::endl;
	}

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

	Atom *atom0 = new Atom("my");
	Atom *atom1 = new Atom("name");
	atom0->addBond(atom1);
	atom0->printBondedAtoms();
	std::cout << "\n\n NOW DELETEEEEEEEEEEEEEEEEEEEEEE\n";
	atom0->removeBond(atom1);

	//atom0->printBonds();
	/*Atom *atom2 = new Atom("jeff");
	 atom0->addBond(atom1);
	 atom1->addBond(atom2);
	 atom2->addBond(atom0);

	 Atom *atom3 = new Atom("Bobie");
	 Atom *atom4 = new Atom("Steve");
	 Atom *atom5 = new Atom("Ronne");
	 Atom *atom6 = new Atom("Bingo");
	 Atom *atom7 = new Atom("Marsh");
	 Atom *atom8 = new Atom("Delux");


	 atom0->addBond(atom1);
	 atom1->addBond(atom2);
	 atom2->addBond(atom3);
	 atom3->addBond(atom4);
	 atom4->addBond(atom5);
	 atom1->addBond(atom6);
	 atom5->addBond(atom6);
	 atom2->addBond(atom5);
	 atom2->addBond(atom6);
	 atom5->addBond(atom3);
	 atom2->addBond(atom7);
	 atom7->addBond(atom8);

	 //atom0->printBonds();
	 //atom1->printBonds();
	 //atom2->printBonds();

	 atom6->printBonds();
	 delete atom6->getNode().get();
	 //atom0->removeBond(atom1);

	 //atom0->printBonds();
	 //atom1->printBonds();
	 //atom2->printBonds();
	 */
	std::cout << "Yes this is our main lole" << std::endl;
	atom0->printBondedAtoms();
	return 0;
}
