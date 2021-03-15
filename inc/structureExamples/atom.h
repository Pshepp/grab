#ifndef INC__STRUCTUREEXAMPLES_ATOM_H_
#define INC__STRUCTUREEXAMPLES_ATOM_H_

#include "../lazyPrints.h"
#include "../structure/node.h"
#include "../structure/edge.h"

class Atom
{
public:
        Atom(std::string name);
        ~Atom();

        void setName(std::string name);
        std::string getName() const;

        void addBond(Atom * otherAtom);
        void removeBond(Atom * otherAtom);
        void deleteAllBonds();

        std::shared_ptr<Node<Atom>> getNodePtr();

        void printBondedAtoms();

private:
        std::shared_ptr<Node<Atom>> nodePtr;
        std::string name;
};


Atom::Atom(std::string name)
{
	this->setName(name);
	nodePtr = std::make_shared<Node<Atom>>(name);
}

Atom::~Atom()
{
	lazyInfo(__LINE__, __func__);
}

void Atom::setName(std::string name)
{
	this->name = name;
}

std::string Atom::getName() const
{
	return this->name;
}

void Atom::addBond(Atom *otherAtom)
{
	this->getNodePtr().get()->addChild(
			this->getName() + "->" + otherAtom->getName(),
			otherAtom->getNodePtr());
}

void Atom::removeBond(Atom *otherAtom)
{
	this->getNodePtr().get()->deleteEdges(otherAtom->getNodePtr());
}

void Atom::deleteAllBonds()
{
	this->getNodePtr().get()->deleteEdges();
}

std::shared_ptr<Node<Atom> > Atom::getNodePtr()
{
        return this->nodePtr;
}

void Atom::printBondedAtoms()
{
	std::vector<std::weak_ptr<Node<Atom>>> neighbors =
			this->getNodePtr().get()->getNeighbors();
	std::string blurbMsg = "Printing atoms bonded to: (" + this->getName()
			+ ")";
	lazyInfo(__LINE__, __func__, blurbMsg);
	std::cout << "Current bonds of calling node" << std::endl
			<< this->getNodePtr().get()->edgesAsString() << std::endl;
	//note that this is lazy, auto is depreciated
	for (auto &atom : neighbors)
	{
		if (atom.lock())
		{
			//Why am I able to resolve after capitalizing all classes smh
			std::shared_ptr<Node<Atom>> tempAtom = atom.lock();

			std::cout << tempAtom.get()->getName() + ", ";
		}
	}

	std::cout << std::endl << "DONE PRINTING BONDED ATOMS" << std::endl;
}

#endif /*INC__STRUCTUREEXAMPLES_ATOM_H_*/
