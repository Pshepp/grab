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
	~Atom()
	{
		lazyInfo(__LINE__, __func__);
		this->deleteAllBonds();
		this->atomNodePtr.reset();
		lazyInfo(__LINE__, __func__, "Atom destructed?");
	}
	void deleteAllBonds()
	{
		this->getNode().get()->deleteAllEdges();
	}
	void addBond(Atom *otherAtom)
	{
		this->atomNodePtr.get()->addNeighbor(otherAtom->getNode(),
				this->getName() + "->" + otherAtom->getName());
	}
	void printBondedAtoms()
	{
		std::vector<std::weak_ptr<Node<Atom>>> neighbors =
				this->getNode().get()->getNeighbors();
		lazyInfo(__LINE__, __func__,
				"Printing atoms bonded to: " + this->getName());
		std::cout << "Current bonds of calling node: " << this->getName()
				<< std::endl;
		this->getNode().get()->printEdges();
		std::cout << std::endl;
		for (auto &atom : neighbors)
		{
			if (atom.lock())
			{
				std::shared_ptr<Node<Atom>> temp = atom.lock();
				std::cout << "Current bonds of: " + temp->getName()
						<< std::endl;
				temp->printEdges();
			}
		}
		std::cout << std::endl << "DONE PRINTING BONDED ATOMS" << std::endl;
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
	void deleteAtom(Atom *atomToDelete)
	{
		//please note that this deletes the node itself
		this->getMolPtr().get()->deleteNode(atomToDelete->getNode());
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

int main()
{

	Molecule *mol1 = new Molecule("Poob");
	Atom *atom0 = new Atom("Bobie");
	Atom *atom1 = new Atom("Steve");
	Atom *atom2 = new Atom("Ronne");
	Atom *atom3 = new Atom("Bingo");
	Atom *atom4 = new Atom("Marsh");
	Atom *atom5 = new Atom("Delux");
	Atom *atom6 = new Atom("Frank");
	Atom *atom7 = new Atom("Bingo1");
	Atom *atom8 = new Atom("Marsh1");
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

	//mol1->addAtom(atom0);
	mol1->addAtom(atom1);
	mol1->addAtom(atom2);
	mol1->addAtom(atom5);

	mol1->printAtoms();
	//mol1->removeAtom(atom0);

	//mol1->deleteAtom(atom0);
	//std::cout<<"\n"<<atom0->getName()<<"\n";
	std::cout << "\ndasdasdasd" << atom0->getName() << atom0->getNode().get()->getName()
			<< "\n";
	//Note that this does not delete the shared ptr within the mol1 structure thus it does not truely delete
	delete atom5;
	//atom5->printBondedAtoms();
	atom2->printBondedAtoms();
	atom4->printBondedAtoms();

	mol1->printAtoms();
	/*atom1->printBondedAtoms();
	 std::cout << "Deleting " << atom6->getName() << "\n";
	 delete atom6;
	 atom1->printBondedAtoms();
	 std::cout << "Deleting " << atom4->getName() << "\n";
	 delete atom4;
	 std::cout << "\nName jeff time to test\n";
	 atom1->printBondedAtoms();
	 atom1->removeBond(atom2);
	 atom1->printBondedAtoms();*/
	return 0;
}
