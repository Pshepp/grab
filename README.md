# grab
An (hopefully) efficient implementation of a graph structure that allows for nodes to be shared through different structures.

## Issue
Our primary issue is the lack of a single "toolkit" to represent a molecule which may contains sugar rings. That being said, if we decide to represent our molecule as a graph we will have to worry about cycles (and to help chem-nerds we also want to worry about covalent bonds)

## Goals 
We have a handful of abstract marks we want to hit 
* Be nice to the people who will eventually use our "toolkit"
	* consistent naming
	* overloading so people can use just part of our toolkit
	* allow possibility for directional bonds
	* Figure out how much they would like to interact with our code
	* efficient
* Algos
	* Determine if a graph is a subgraph of another
		* Tell if GraphB is a subgraph of GraphA
		* Get what subgraphs of GraphA match GraphB
	* Detect ALL cycles (**ultimate test is a buckeyball**)
		* fused cycles
		* normal cycles

Handful of less abstract marks
* Edges 
	* have some signifier and some relation to our structure
	* handle memory
	* show some form of direction
* Nodes
	* have some signifier and some relation to our structure
	* Allow to be shared between different graphs (i.e. nodeA, Graph1, Graph2∃ & graph1 != graph2 but nodeA∈Graph1 & nodeA∈Graph2 thus if we delta nodeA the change is observed in both Graph1 & Graph2)
	* Can delete a node then run our algos and results are correct
	* handle memory  
* Graphs
	* have some signifier and some relation to our structure
	* handle memory 

## Approaches
Due to use wanting to be able to have the possibility of caring about a covalent/directional bond, instead of worry about order we store things within data (i.e. edge {A, B} would be if A has a directional edge to B, edge {B, A} would be if B has a directional edge to A) why don't we represent this relation with the memory structure of nodes and edges themselves? 

This yields the most sense due to not only us being able to represent our edge directionality through memory relations but we can also wrap our relations in "owning" objects which will also ensure that memory is correctly handled. 

Our structure is comprised of, in descending granularity (and "individuality" for our case), Graphs Nodes and Edges. To force our hierarchy we will be using smart pointers. Let's make some observations:

* Graph observations
	*

* Node observations
	* We can have a node (molecule) without an edge (bond)
	* If we delete a node we MUST delete all connecting edges, both incoming and outgoing 
	* We will need source node and a sink node in order to show directionality
	* We want to share our nodes between different graphs
	
	
* Edge observations
	* We need a node to create an edge, we cannot have a floating bond
		* If we eventually do need a floating bond, it is possible to create a dummy **SINK** node that is known to "not" exist but does exist for our program to be happy. I would prefer not to have a dummy source node but it is possible.
	* If we delete an edge, be it incoming or outgoing, we do not necessarily delete the nodes that we related to said edge. We may even want to have a node just "floating" within our graph with no connected edges.
	* We do **NOT** want to share our edges between graphs due to the relationship between our nodes and edges "creates" our graph structure.
	


In order to provide memory based edge directionality, an edge requiring at least one node to exist 