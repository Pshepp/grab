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

## Observations
Due to use wanting to be able to have the possibility of caring about a covalent/directional bond, instead of worry about order we store things within data (i.e. edge {A, B} would be if A has a directional edge to B, edge {B, A} would be if B has a directional edge to A) why don't we represent this relation with the memory structure of nodes and edges themselves? 

This yields the most sense due to not only us being able to represent our edge directionality through memory relations but we can also wrap our relations in "owning" objects which will also ensure that memory is correctly handled. 

Our structure is comprised of, in descending granularity (and "individuality" for our case), Graphs Nodes and Edges. To force our hierarchy we will be using smart pointers. Let's make some observations:

* Graph observations
    * More but forgot
    * We can think of our graph being our structure container, our node being our data within, and finally the edges between nodes create the actual structure (relation between all nodes)
	* We want to share nodes between graphs but not necessarily the structure.
        * Please note our edges denote our actual "structure" (how all nodes are connected), we can think of our nodes as just data "points"

* Node observations
	* We can have a node (molecule) without an edge (bond)
	* If we delete a node we MUST delete all connecting edges, both incoming and outgoing 
	* We will need source node and a sink node in order to show directionality
	* We want to share our nodes between different graphs
	* Multiple edges can interact with same node
	
* Edge observations
	* We need a node to create an edge, we cannot have a floating bond
		* If we eventually do need a floating bond, it is possible to create a dummy **SINK** node that is known to "not" exist but does exist for our program to be happy. I would prefer not to have a dummy source node but it is possible.
	* If we delete an edge, be it incoming or outgoing, we do not necessarily delete the nodes that we related to said edge. We may even want to have a node just "floating" within our graph with no connected edges.
	* We do **NOT** want to share our edges between graphs due to the relationship between our nodes and edges "creates" our graph structure.
	* has a SINGLE source node and a SINGLE sink node

## Structure Approach
Think of a doubly-linked list but as a graph. Due to our observations we want to enforce the following relations with memory. 


**I need to put a couple diagrams here.**


### Edge
* We must add on the ability to traverse both "forward" and "backwards" across our data, thus our edges must point to connected nodes. We use a share_ptr for both the source and sink node. Used due to:
    * Our nodes can have multiple edges pointing to them, thus they must be at least a shared_ptr
    * We want our nodes to be forced to stay alive as long as they are a part of our "structure". Please keep in mind that our "structure" is created by the way our data is connected AND the structure container of our graph
    * When we delete an edge there is no possibility of a cyclic reference becoming an issue due to our unique_ptr wrapper taking care of the shared_ptrs
    * nodeA and nodeB exist and dont equal, let us have edge e1 where e1 uses a shared_ptr to its sourceNode and uses a weak_ptr to its sinkNode
        * if we have nodeA --(e1)--> nodeB then our nodeA use_count is 1 (aka wont destruct) but our nodeB use_count is 0 (causes auto destruct for shared_ptr). If we have no other shared_ptrs to our nodeB (i.e. all edges in nodeB are incomming) then it will automatically destruct. Granted the design of our graph class will have a "kick-stand" to prevent this (decision was because stay noided), but if we decide to not use our graph class we will run into issues
        * tl;dr the use of shared_ptr for both sinkNode and sourceNode acts like a lock that will prevent our nodes from being deleted before we completely get rid of our edge.

### Node
* In order to "own" our edges (point to an edge we are sourcing), our source nodes will use a unique_ptr to each edge that comes from the node. This is due to:
    * If we delete a node, we must delete all connecting edges but not necessarily vice-versa. Going through and just removing a unique_ptr to get rid of our edge is easier than having to lock, dequeu, etc.
    * A large part of using smart pointers is enforcing a heirarchy, in fact it seems their primary function (once abstracted) is to create ownership heirarchies (almost 100% on this, honestly dont know why else they would be used). That being said our sourced edge should be "owned" by the node that sources it in order to clearly demonstrate not only the fact that we need at least 1 node to even think of creating an edge, but to also show which "direction" the edge is going.
    * In order to take advantage of shared_ptrs, 
    * We want to have the ability to create directed edges, and doing so using our memory structure itself is more elegant than keeping track of the order that data is used. Instead of order we worry about hierarchy.
* Now we must also have the ability to go "backwards" to an edge that we are the sinkNode to, due to the edge being owned by a unique_ptr, and that we do not want to change/deal with any ownership we will use a raw_ptr to refer "backwards" to our edge
    * This does create some implementation complexity, specifically when calling delete we HAVE to ensure that the raw_ptr is taken care of BEFORE we go to our sourceNode and destroy our edge by deleting the associated unique_ptr. 
        * tld;r the raw_ptr can dangle so we must take care of our raw_ptr before deleting edge
    * This also creates some implementation/human interaction worry due to us having the ptr completely exposed and are able to alter it
        * Just liberally use const when we ever access/return the raw_ptr, possibly have the raw_ptr ITSELF set as a const
* **NOW THE QUESTION IS HOW DO WE KNOW WHICH EDGE BELONGS TO EACH GRAPH? CONSIDERED USING OUR GRAPH AS A KEY THAT GETS HASHED THEN REFERS TO A CONTAINER OF OUR EDGES BUT THIS FEELS GROSS. ALT IS GRAPH ALSO KEEPS TRACK OF EDGES BUT THATS INEFFICIENT. THINK. MAYBE JUST HAVE NODE CONTAIN A FUNCTION THAT TAKES IN A HASH (I.E. WE HAVE GRAPH CALL NODE. FUNCTION WHICH TAKES IN SOME HASH THAT GRAPH GENERATES AND PASSES IN. WHY NOT PUT IN EDGE CLASS SINCE WE CAN CALL OUR EDGE THEN TAKE CARE OF SOURCE & SINK INSTEAD OF HAVING TO FIGURE OUT WHERE TO GO TO GET OUR OTHER NODE TO PUT EDGE IN HASH**
    * Promise I am not crazy this pasing in a hash created "above" our function call is a good idea, it should allow for a simple function that just takes the hash in as an int or something then we have no need for edge to know of graph. Also, I think it will help show in code that edges are owned/directed. This feels very nice
* **NOW THE QUESTION IS HOW DO WE KNOW WHAT GRAPH WE ARE IN? DOES THIS EVEN MATTER? CONSIDERED WEAK BUT IDK, REFER TO RANT ABOVE** 
* Need to look into callbacks ngl

### Graph
NOTE SUBJECT TO CHANGE A TON
* As said before, our graph class is more of a "container" that our structure eventually comes together in. But that does not mean it cannot be very useful. We are going to use a list of shared_ptrs to the nodes within each graph
    * To be used as a "kickstand", this way our graph structure(s) has(ve) the final say with regards to a node being deleted. If these were weak_ptrs our nodes would not be able to "float" and could "accidently" destruct due to an accidental change instigated by a different graph structure.

## Algo Approach
Some divine blurb

* KISS and get entity info
    * We need to have the ability to quickly look at an entity and know if it is automatically invalid. Properties include if our node is a leaf, if it is a bridge, if it has been visited --- if our edge is a bridge or a leaf, if it has been visited. These properties will be members of the entity, and the algo will set values accordingly. Examples of use below
        * Let us want to seperate our all "mega-trimmed-cycles" (subgraphs that are only of nodes that have potential to be in a cycle), we would use dfs or bfs to start and try to traverse our whole graph like we usually do but when we hit a node/edge that is a leaf then we ignore said node and continue on. The same logic is taken for a node that is a bridge and an edge that is a bridge. Please note that here we would NEVER hit a node that is acting as a bridge due to our refusal to take any edges that are labeled as a bridge (you need a bridge-edge on every side of the node for it to be a node that behaves a bridge (possible edge case).
* Cycles and the issues. A blurb and two approaches
    * I feel this is where our entity properties (i.e. is leaf) will shine. As above, we would create our list of "mega-trimmed-cycles" which we would THEN run the full cycle detection algos on. 
    * Brute force each path: 
    * Break down to simple cycles then iterate and try to throw them on the graph to detect cycles
* Subgraph matching (subgraph isomorphism problem)
    * Same as above, tons of papers on subgraph matching.

## ARGH NOTES
As of now, we gotta keep in mind that this is only our memory structure itself, we can add anything to classes as long as they know of one another in the previously described manner. 

Need to do testing to see which is better for both subgraph matching and our total cycle detect. 

### Good Info
http://www.sc.ehu.es/acwbecae/ikerkuntza/these/Ch2.pdf
https://getd.libs.uga.edu/pdfs/saltz_matthew_w_201308_ms.pdf
https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/mcgregor_common_subgraphs.html
https://www.mdpi.com/2079-9292/9/1/184/htm
https://link.springer.com/article/10.1007/s41019-019-0090-z
https://academic.oup.com/bioinformatics/article/23/2/232/205026
https://bmcbioinformatics.biomedcentral.com/articles/10.1186/1471-2105-14-S7-S13
 
 
not for us but cool
https://en.wikipedia.org/wiki/Clique_problem
