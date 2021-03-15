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
		this->atomNodePtr.get()->addChild(
				this->getName() + "->" + otherAtom->getName(),
				otherAtom->getNode());
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
		std::vector<std::weak_ptr<Node<Atom>>> neighbors =
				this->getNode().get()->getNeighbors();
		std::string blurbMsg = "Printing atoms bonded to: (" + this->getName()
				+ ")";
		lazyInfo(__LINE__, __func__, blurbMsg);
		std::cout << "Current bonds of calling node" << std::endl
				<< this->getNode().get()->edgesAsString() << std::endl;
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

class Molecule
{
public:
	Molecule(std::string name) :
			moleculeName(name)
	{
		molPtr = std::make_unique<Graph<Molecule, Atom>>(name);
	}
	~Molecule();
	void addAtom(Atom *atomToAdd)
	{
		this->getMolPtr().get()->addNode(atomToAdd->getNode());
	}
	void removeAtom(Atom *atomToRemove)
	{
		this->getMolPtr().get()->removeNode(atomToRemove->getNode());
	}
	void deleteAtomBonds(Atom *atomToIslandify)
	{
		//please note that this deletes the node itself
		this->getMolPtr().get()->deleteEdges(atomToIslandify->getNode());
	}
	void printAtomBonds()
	{

	}
	void printAtoms()
	{
		std::vector<std::weak_ptr<Node<Atom>>> atoms =
				this->molPtr.get()->getNodes();
		lazyInfo(__LINE__, __func__,
				"Graph structure (molecule named " + this->getName()
						+ ") containing nodes (atoms)");
		for (auto &i : atoms)
		{
			if (i.lock())
			{
				std::shared_ptr<Node<Atom>> temp = i.lock();
				std::cout << "Atom Named: " + temp->getName() << std::endl;
				std::cout << "Bonds: " + temp.get()->edgesAsString();
 			}
			else
			{
				badBehavior(__LINE__, __func__, "Couldnt lock");
			}
		}
	}
	const std::unique_ptr<Graph<Molecule, Atom> >& getMolPtr() const
	{
		return molPtr;
	}

	const std::string& getName() const
	{
		return moleculeName;
	}

	void setName(const std::string &moleculeName)
	{
		this->moleculeName = moleculeName;
	}

private:
	std::string moleculeName;
	std::unique_ptr<Graph<Molecule, Atom>> molPtr;

};

int main(int argc, char *argv[])
{
	Molecule *mol1 = new Molecule("Water");
	Atom *atom0 = new Atom("H_1");
	Atom *atom1 = new Atom("H_2");
	Atom *atom2 = new Atom("O");
	mol1->addAtom(atom0);
	mol1->addAtom(atom1);
	mol1->addAtom(atom2);
	//add covalent bond TO oxygen
	atom0->addBond(atom2);
	atom1->addBond(atom2);

	mol1->printAtoms();


	std::cout << "~~~~~~~~~~~~Time for benzRing~~~~~~~~~~" << std::endl;

	Molecule *mol2 = new Molecule("Garbage_Benzene");
	Atom *carbon0 = new Atom("C_0");
	Atom *carbon1 = new Atom("C_1");
	Atom *carbon2 = new Atom("C_2");
	Atom *carbon3 = new Atom("C_3");
	Atom *carbon4 = new Atom("C_4");
	Atom *carbon5 = new Atom("C_5");

	mol2->addAtom(carbon0);
	mol2->addAtom(carbon1);
	mol2->addAtom(carbon2);
	mol2->addAtom(carbon3);
	mol2->addAtom(carbon4);
	mol2->addAtom(carbon5);

	carbon0->addBond(carbon1);
	carbon1->addBond(carbon2);
	carbon2->addBond(carbon3);
	carbon3->addBond(carbon4);
	carbon4->addBond(carbon5);
	carbon5->addBond(carbon0);

	mol2->printAtoms();

	std::cout << "End main func" << std::endl;

	//atom1->printBondedAtoms();
	return 0;
}
