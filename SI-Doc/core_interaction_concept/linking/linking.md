## Linking Mechanic

### Description
Within a SI context, two or more interactive regions can be *linked* via their *attributes* insofar they have the necessary *capability* for that.
For example, interactive region A is linked to region B via their positions.
Then, changing the value of the position attribute of B immediatly causes this change in region A as well and A is also moved according to B's position delta.
Therefore, both regions kept their original distance to each other, however they are found at a different position in the SI context.  

### Purpose
The linking of regions mechanic enables further dynamic use of interactive regions within a SI context.
Returning to the example of the previous section, linking regions via the positional attribute enables user to mass move interactive regions by *referencing* all but one regions' position in another region.
Additionally, the linking mechanic directly enables the use of physical artifacts in a SI context.
For example, once a tangible is tracked by SI, its detected contour is this artifact's interactive region.
However, such contour-based interactive regions often are blank and effectless regions requiring setting of a effect.
Here, it is possible to *transfer* attributes from another region to the artifact's interactive region.
Therefore, the physical artifact can assume the role of a rubber, by having a Deletion-Effect *transferred* to its interactive region.

### Recap: Definition of Attribute
As seen in the previous blog post: LINK

```
An attribute is a property of an interactive region which can be linked to attributes of other interactive regions if capable either by *referencing* or *transferring*.

...
```
Additionally, *effects* are a special type of attribute, due to them being unique for each class of interactive region unlike position or scale attributes, which are present in every interactive region.

### Functionality

Generally, each region can be linked to every other present region based on region *attributes* and corresponding region linking *capabilities*.
An easy example for that is linking two regions in terms of their *position* attribute as shown in the previous sections.
Moving one region in a direction immediately causes the linked region to be relatively (positional delta) moved in that direction as well.
However, only regions sharing the capability to be linked via the 'position' property actually show this behaviour when linked and moved.
Generally, this is the case for all kinds of possible linking capabilities and between regions.

In *silang* (a link to the silang blogpost will follow soon) linking is expressed as follows:

```
A -> B => property_of_A, property_of_B;  # unidirectional
A <-> B => property_of_A, property_of_B; # bidirectional

# short forms if property_of_A equals property_of_B
A -> B => property;
A <-> B => property;

# example with position property
A -> B => position;
```

The SI way to view such linking relationships is called Linking-Graph.
Therefore, a linking-graph comprises two or more interactive regions which are linked in some way.
Here, interactive regions represent nodes and the linking relationship between nodes represent edges. 
In terms of graph theory, linking-graphs are *mixed graphs* as they feature either directed or undirected links.

### Directionality

#### Unidirectional Linking
The easiest way to link regions is by establishing a unidirectional link.
In terms of graph theory unidirectional links are directed edges between two nodes or interactive regions.
Using the example from the previous sections again, regions A and B are unidirectionally linked according to their *position* attribute.
In *silang*, this is expressed as:
```
A -> B => position, position;  # or A -> B => position; for short
```
Therefore, moving B in a direction immediately causes A to move in that direction as well.
But due to the link being unidirectional, moving A directly has no effect on B whatsoever.

[{{:projects:si_positional_link_ex1.png?nolink|Figure 01: Prototypical Visualization of Unidirectional Linking Relationship}}] 

#### Bidirectional Linking
When expanding on the example in the previous section so that movement of A also triggers movement of B, then a bidirectional linking relationship is necessary.
In terms of graph theory this is equivalent to an undirected edge.
Therefore, A and B are bidirectionally linked according to their *position* parameter.  
In *silang*, this is expressed as:

```
A <-> B => position, position;  # or A <-> B => position for short
```

Now, both can be moved directly while the other is also moved due to the bidirectional link.
[{{:projects:si_positional_link_ex2.png?nolink|Figure 02: Prototypical Visualization of Bidirectional Linking Relationship}}] 

#### Cyclic Linking

The example in the section above already features the easiest form of a cyclic linking relationship, due to A's position changes affect B's position and vice versa.
In order to showcase cyclic linking-graphs more intuitively, the example is expanded with a third region C and cyclically and unidirectionally linking it to A and A to B and B to C, forming a triangular shape.
Again, the *position* property of the regions is used.  
In *silang*, this is expressed as follows this:

```
A -> B => position;
B -> C => position;
C -> A => position;
```

[{{:projects:si_positional_link_ex3.png?nolink|Figure 03: Prototypical Visualization of Cyclic Linking Relationship}}] 

### Referencing and Transferring of Attributes
In the current state of the concept of SI, interactive regions can be linked by either attribute referencing or attribute transferring.

#### Attribute Referencing 
SI uses attribute referencing as default style of attribute linking for attributes which are present and always defined in every interactive region such as position, scale, color, etc., unlike e.g. effect.
Attribute Referencing is similar to *pass by value* of common programming languages.

The directionality of the link determines which region stores the reference of the linked attributes.
This is trivial for the bidirectional case, because both regions store the reference of the other.

For the unidirectional case (shown in silang):
```
A -> B => position;
```
A is linked to B via position.
This means B receives the reference of A's position attribute.
Therefore, changing the position of B results in A changing its position relatively as well.
However, moving A does not affect B.

#### Attribute Transferring

Attribute Transferring is SI's default way of linking interactive region's effect attributes which are not necessarily always set within a SI context.
Attribute Transfer is similar to *pass by value* of common programming languages'.

For the unidirectional case (shown in silang)
```
A -> B => DELETE;
```
the behaviour is equivalent to Attribute Referencing with the exception, that no reference is stored in region B.
Instead, B deep-copies A's effect attribute and stores it by value.
In this way, B's instance of the effect and A's instance of the effect are both unique.
Therefore, changing A's effect attribute does not change B's effect attribute. 

### Technical Challenge: Infinite Recursion
When reviewing Figure 03 again, the most problematic technical challenge is easily visible: **infinite recursion**.
Due to the cyclic nature of the linking relationship depicted in Figure 03, it does not matter which of the interactive regions is moved, in order to move all of them.
However, they also never stop moving, due to said infinite recursion because A triggers movement of B which triggers movement in C which triggers movement in A again and so on.
In order to solve this problem, SI uses universally unique identifiers (uuid).
Therefore, every interaction with a set of linked regions is bound to an uuid which is generated by the interactive region which is *directly* affected by the user, called the *entry region*.
The uuid is passed to the next region based on the linking relationship along other data (in the case of Figure 03: positional data).
Once the action reaches a region which already processed the specific uuid, SI checks whether that region's linked property is equal to the one which originally left that region.
When this is the case, the action is dropped by SI in order to prevent said *infinite recursion*.

### Difference to Process-Graphs
While Linking-Graphs and Process-Graphs share some similarities such as them being depicted as graphs, *entry region*, they are mostly distinct concepts.
The major difference is that linking-graphs, unlike process-graphs, define no sequence because interactions trigger immediate actions affecting all regions of the linking-graph at the same time. 
However, process-graphs need a specific, user-defined, sequence of effects which are applied to colliding artifacts in that specific order and process-graphs also work only with *effects* and no other potential region attribute.
Additionally, linking-graphs cannot be compressed, while process-graphs can be compressed for decomposition purposes of a SI context.  

(A dedicated Blog Post for Process-Graphs follows soon)

## Potential Interaction Techniques
Up until to the writing of this blog post, only little and superficial thinking in terms of interaction techniques for linking has been done.
Generally, the questions which are currently unanswered are:
* how do users want to link regions?
* how does the selection process look like?
* how does the deselection process look like?
* how to correct potential mistakes? 
* how to provide undo? (timeline within SI context?; generally greater problem within SI)
* how do users want to break links?
* how to move a linking-graph as a unit?
* etc.

Once SI reaches the maturity to be formally evaluated, these question are on of the first to be answered.
Therefore, dedicated blog post about that will follow once answers are either found or had to be defined.

### Linking Mechanic TL;DR

SI's linking mechanic is a powerful way of defining more advanced relationships between interactive regions.
The SI way of thinking about linking relationships between multiple region is the linking-graph.
Linking-graphs are a set of regions which are either unidirectionally or bidirectionally (and therefore cyclically) linked to each other according to the regions' *attributes* and *capabilities*.
In order to prevent infinite recursion, a uuid is generated at the start of an interaction and each region in the linking-graph performs the action exactly once, ignoring actions of the same uuid.

Linking of regions comes in two modes: Attribute Referencing and Attribute Transfer.
Attribute Referencing is similar to *pass by value* of common programming languages.
It allows binding a reference of an attribute of region A (e.g. position) to an attribute of region B.
Then changing this specific attribute of B triggers the change of attribute of A permanently.
For example, linking regions A and B via the position attribute, moving A results in B moving relatively according to A's positional delta).

Attribute Transfer is similar to *pass by value* of common programming languages'.
This method allows interactive regions to have attributes transferred to them, i.e. interactive regions can deep-copy other regions attributes according to their values.
For example, regions C and D are about to be linked. D's effect attribute is currently not set (i.e. neutral when colliding with other regions) and is meant to assume the effect stored in C.
Once, the link is established D stores a copy of C's effect attribute. 
Unlike Attribute Referencing, although C and D have the same effect attribute, none of the regions is referenced in the other, i.e. changing C's effect attribute does not change D's effect attribute and vice versa. 
