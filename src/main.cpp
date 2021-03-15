/* TODO: Various todos (will be completed in different branches)
 * 			- use unordered set to replace vector in places we do not need to know order
 * 			- incorporate multi-graph ownership
 *
 */

#include <iostream>
#include <memory>

#include "../inc/structure/graph.h"
#include "../inc/structure/node.h"
#include "../inc/structure/edge.h"
//how do we want to read in our data? This will allow for program <filename> and will take it in. Need to implement

const bool xtraInfo = true;

class Atom
{
public:
	Atom(std::string name) :
			atomName(name)
	{
		atomNodePtr = std::make_shared<Node<Atom>>(name);
	}
	~Atom()
	{
		lazyInfo(__LINE__, __func__);
		this->deleteAllBonds();
		this->atomNodePtr.reset();
		lazyInfo(__LINE__, __func__, "Atom destructed?");
	}
	void deleteAllBonds()
	{
		this->getNode().get()->deleteEdges();
	}
	void addBond(Atom *otherAtom)
	{
		this->atomNodePtr.get()->addChild(this->getName() + "->" + otherAtom->getName(), otherAtom->getNode());
	}
	void removeBond(Atom *otherAtom)
	{
		this->getNode().get()->deleteEdges(otherAtom->getNode());
	}

	std::shared_ptr<Node<Atom>> getNode()
	{
		return atomNodePtr;
	}
	std::string getName()
	{
		return atomName;
	}
	void printBondedAtoms()
	{
		std::vector<std::weak_ptr<Node<Atom>>> neighbors = this->getNode().get()->getNeighbors();
		std::string blurbMsg = "Printing atoms bonded to: ("  + this->getName()+")";
		lazyInfo(__LINE__, __func__, blurbMsg);
		std::cout << "Current bonds of calling node" << std::endl << this->getNode().get()->edgesAsString() << std::endl;
		//note that this is lazy, auto is depreciated
		for (auto &atom : neighbors)
		{
			if (atom.lock())
			{
				//Why am I able to resolve after capitalizing all classes smh
				std::shared_ptr<Node<Atom>> tempAtom = atom.lock();
				if (xtraInfo)
				{
					std::cout << tempAtom.get()->edgesAsString();
				}
				else
				{
					std::cout << tempAtom.get()->getName() + ", ";
				}
			}
		}
		std::cout << std::endl << "DONE PRINTING BONDED ATOMS" << std::endl;
	}
private:

	std::shared_ptr<Node<Atom>> atomNodePtr;
	std::string atomName;
};


int main(int argc, char *argv[])
{
	Atom * atom0 = new Atom("My");
	Atom * atom1 = new Atom("Name");
	Atom * atom2 = new Atom("Jeff");

	atom0->addBond(atom1);
	atom0->addBond(atom1);
	atom0->addBond(atom1);
	atom0->addBond(atom1);
	atom0->addBond(atom1);
	atom0->printBondedAtoms();
	atom1->removeBond(atom0);





	std::cout<<"End main func" << std::endl;

	atom1->printBondedAtoms();
	return 0;
}
