#ifndef INC__STRUCTUREEXAMPLES_MOLECULE_H_
#define INC__STRUCTUREEXAMPLES_MOLECULE_H_

#include "../structure/graph.h"
#include "atom.h"

class Molecule
{
public:
	Molecule(std::string name);
	~Molecule();

	void setName(std::string name);
	std::string getName() const;

	void addAtom(Atom *atomToAdd);
	void removeAtom(Atom *atomToRemove);

	void printAtoms();

	const std::unique_ptr<Graph<Molecule, Atom> >& getMolPtr() const;

private:
	std::string name;
	std::unique_ptr<Graph<Molecule, Atom>> molPtr;
};

Molecule::Molecule(std::string name)
{
	this->setName(name);
	molPtr = std::make_unique<Graph<Molecule, Atom>>(name);
}

Molecule::~Molecule()
{
}

void Molecule::setName(std::string name)
{
	this->name = name;
}

std::string Molecule::getName() const
{
	return this->name;
}

void Molecule::addAtom(Atom *atomToAdd)
{
	this->getMolPtr().get()->addNode(atomToAdd->getNodePtr());
}

void Molecule::removeAtom(Atom *atomToRemove)
{
	this->getMolPtr().get()->removeNode(atomToRemove->getNodePtr());
}

void Molecule::printAtoms()
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

const std::unique_ptr<Graph<Molecule, Atom> >& Molecule::getMolPtr() const
{
	return this->molPtr;
}

#endif /*INC__STRUCTUREEXAMPLES_MOLECULE_H_*/
