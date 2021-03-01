/*
 * PLEASE IGNORE THIS BRANCH DUE TO IT BEING A "ROUGH DRAFT".
 */

#include "../inc/edge.h"
#include "../inc/node.h"
#include "../inc/graph.h"

class Atom
{
public:
	Atom(std::string name) : atomName (name)
	{
		atomNodePtr = std::make_shared<Node<Atom>>(this, name);
	}
    inline void AddBond(Atom *otherAtom)
    {
        atomNodePtr->addNeighbor(otherAtom->atomNodePtr, this->getName());
		AddEdge(otherAtom->GetNode(), this->GetName() + "->" + otherAtom->GetName());

    }
private:
	std::shared_ptr<Node<Atom>> atomNodePtr;
	std::string atomName;
};


int main ()
		{
			return 0;
		}
