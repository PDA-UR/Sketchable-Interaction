# Process-Graphs of Regions and Process-Graph Compression

## Description
Process-Graphs (PG) occur when the effects of two or more interactive regions are put into a user-defined sequence in order to model more complex functionality within a SI context.
PGs are one of the three fundamental concepts of SI with the other being the collision of regions and linking of regions (links to these blog posts will release soon).

## Purpose
Process-Graphs allow users to model more complex workflows within their workspace.
Put into the context of common programmings language like Java or C++, the process-graph mechanism is a metaphor for a method or function as two or more effects are sequenced.
Therefore, this mechanism can also be a *queue* of functions or eventually be viewed as a *graph*  of functions - the processing-graph (PG).
This sequence of function can also be viewed as a function calling other function due to the application of decomposition.
Therefore, a region collision triggers a certain action (calls a function) and PGs trigger further actions according to the number of sequenced regions and their effects (calling a set of functions).
The PG is determined by its **reach**, ergo how many regions are affected by this action trigger. 
The PG's **impact** specifies, to what extend effects are applied, region properties are mutated, etc..

## Functionality
Generally, each region can be added to a PG sequence.
An easy example for that is sequencing two regions in order to first *tag* a file and lastly *send* it to a co-worker.

<!-- example image --> 

Colliding an interactive region *linked* to a file icon with the PG's designated *entry region* -the *tag* region- will first tag that file and immediately send that file to the specified co-worker via the *exit region -the *send* region-.
<!-- example image of above with file icon touching entry region -->

However, the file never directly collides with the *send region*, yet that region's effect is applied.

Of course, the file also can be sent via the *send region* by colliding with it directly.
Then, the tagging step is skipped.
Here, the current *viewing-state* (see the dedicated section for details) of the SI context determines with what amount of flexibility indiviual regions or sub-graphs of PGs can be used.
Above's example assumes that the *default-viewing-state* is active.
*Capabilities* determine whether effects can be triggered and applied or not.
Therefore, a colliding region has to satisfy all the capabilities required by the process-graph sequence effects.
Nothing happens if one or more capabilities are not met.
This also true for the sequences of subsets of the target PG.

## Viewing-State

## Directionality

### Unidirectional Process-Graphs
this should be the only possible and reasonable one because it is defined as sequence

### Bidirectional Process-Graphs


### Cyclic Process-Graphs


### Combinations




## Process-Graph Compression

### Idea

As mentioned above, the linking of regions is a *queue* or *graph* of functions which are called once a *entry region* collides with another region.
Additionally, this *graph* of functions can metaphorically be seen as a single function calling all member region of the process-graph.
Complex linking-graphs may occur within a SI context when complex workflows are modelled.
This is especially the case when e.g. a data processing pipeline is modelled.

<!-- demo image/uml of such a process e.g. copy_to\-\>tag\-\>send_by_email -->

In common programming languages large problems are divided into smaller ones until their implementation becomes relatively easy or trivial.
Therefore, multiple functions are the consequence of refactoring larger chunks of code and put into sequence of a wrapping function which is commonly known as decomposition.  
However, SI decomposition works exactly the other way around.
Within an SI context users sketch *atomic* regions.
These regions have exactly one purpose and only do exactly that one thing based on their capabilities.
Therefore, in SI users already model their workflow in the traditional, decomposed way.
However, with SI being a visual programming language users may easily get overwhelmed by great amounts of present regions.
This phenomenon is called *information clutter*.
And this gets particularly worse when great amounts of regions are linked into multiple linking-graphs.

Therefore, decomposition within an SI context means reducing the amounts of regions required to be rendered in order to accomplish the modelling of the target workflow, which results in a reduction of the information clutter.
However, this does not mean that SI limits the maximum amount of supported regions or requires users to sketch their workflows *with good performance*.
Instead, SI provides linking-graph compression as a decomposition method.
Users can decide to compress complex linking-graphs within a single region, a *compound* region.
Also, users can compress multiple linked *compound* regions into another *compound* region and so on.
This is analogous to the decomposition pattern used in modern programming languages mentioned earlier.  
In *silang*, a user's sketching can look something like this:

```
A <-> B => position, scale;
B -> C => position;
D <-> C => scale;
D <-> E => position, scale;
```

Due to space limitations or clutter, users may refactor their SI context or workspace and compress the linking-graph above.
In *silang* linking-graph compression is expressed as follows:

```
F := region => type(composition)
            & region_links := [
                (A <-> B, position, scale),
                (B -> C, position),
                (D -> C, scale),
                (D <-> E, position, scale)
            ];
```

Region **F** then has the same behaviour as the original linking-graph.
However, **F** is **not** an alias for that linking-graph.
All data and behaviour is deep-copied into **F** and not referenced to the original regions and their linking.
The original linking-graph is deleted on compression.
Additionally, SI forbids to selectively remove one link from a compression region as regions are designed to be immutable.
However, users can retrieve the linking-graph from the compression region, destroying it in the process.

<!-- CC1 example image with and linking-graph compression CC2 -->
Linking-Graph Compression is bound by rules which determine whether such a graph can be fully compressed, partially compressed or not compressed at all.
First and foremost, users of SI have to specify an *entry region* and an *exit region*.
The *entry region* represents the defined entry point of a linking-graph, therefore it is the absolute beginning of the linking-graph which the user desires to compress.
The *exit region* on the other hand represents the desired absolute ending of the compressed linking-graph.
Based on *entry* and *exit* nodes, the SI context then determines whether the linking-graph can be logically compressed as the user desires.
SI differentiates between Full Linking-Graph Compression and Partial Linking-Graph Compression.

### Full Process-Graph Compression

Full Linking-Graph Compression is probably the most occurring type of Linking-Graph Compression users of SI encounter.
It occurs when a linking-graph can be fully compressed dependent on the specified *entry region and *exit region*.
The example above with region **F** represents a compressed linking-graph, where region **A** is chosen as *entry region* and region **B** is chosen as *exit region*.
Therefore, the graph can be fully compressed due to the existing path from **A** to **B**, which is one of the rules hinted at earlier.
However, Full Linking-Graph Compression can be considered as a special case of Partial Linking-Graph Compression.
In this procedure, all non-user-deterministic rules come to effect.


<!-- example graph image -->

### Partial Process-Graph Compression

#### Rules
The following rules are key to Linking-Graph Compression and are listed below.
Additionally, further below are rule specifications.
* A linking-graph can be a sub-graph of another graph
* A linking-graph must have a path from the *entry region* to the *exit region*
* A linking-graph has to be self-contained

#### A Process-graph can be a sub-graph of another Process-graph

As users may model more complex workflows within a SI context, the more users may want to apply decomposition to their workspace.
Also, users may want to apply that technique to a subset (or sub-graph) of their complex linking-graph model.
This allows particular selections of linking-graph construction which can be individually compressed as users see fit.
Therefore, a complex linking-graph can be composed of other linking-graphs (or sub-graphs).
A sub-graph or linking-graph in general must contain at least two regions which are linked via valid capabilities, the *entry region* and *exit region*.

<!-- image of DD1 -->
The linking-graph shown in the image above, contains several sub-graphs because of the sheer amount of regions involved with various linking relationships.
However, that example linking-graph may not be something feasible users would actually build.
The compression of a specific sub-graph, however, is not bound to defining an *entry* and *exit region* by a user as well as logical rules determined by the SI Runtime powering the SI context of the linking-graph.
These logical rules are described in the following chapters.

#### A Process-graph must have a path from the *entry region* to the *exit region*
This rule is paramount to the linking process because it determines whether there is a graph to be compressed in the first place.
Immediately after the users defined the *entry region* and *exit region*, the SI Runtime checks whether there is a path from entry to exit.
If the SI Runtime evaluates an existing path, the next rule applies.

#### A Process-graph has to be self-contained
Being able to reach the *exit region* when starting from the *entry region* unfortunately is not enough for logical error prevention.
*Self-containment* in terms of linking-graphs means, that no region outside of the linking-graph unidirectionally links towards a region inside of the linking-graph except for the *entry region* dependent on the *exit region*.
When looking back at Figure DD1 containing a complex linking-graph with unidirectional and bidirectional linking relationships, several problems can occur based on the choice of the *entry region* and *exit region*.
<!-- Figure DD1 -->
For example, regions **D, E** and **C** are invalid *entry regions* because of **A** unidirectionally linking towards **B**, and the former three regions all link to **B** in one way or another.
**Q, A, B** and **G** can be valid *entry regions* because no other region unidirectionally links inside of the intended path except to the *entry region*.
However, for the regions **Q, A** and **B** this is only the case if **E** is defined as the *exit region* and for **G** this is only the case if **H** is the *exit region*.
With **I** linking to **F**, every path which includes **F** is not self-contained due to the *external* region **I** unidirectionally linking inside the linking-graph.
If **I** links bidirectionally to **F** and **G** is defined as *exit region* the graph would also be not self-contained with *entry regions* **Q, A** and **B**, because **I** links into the intended path or sub-graph.

The following figures show most of the self-contained linkin-graphs for this example and omits trivial ones.

<!-- imgs -->

## SI Standard Library in terms of Process-Graphs

SI's standard library contains several regions and effects with preset capabilities.
Therefore, which regions can be linked to which other regions is predefined.
However, it is possible to override SI's standard library regions.
Application programmers simply derive from them and override capabilities and effect functions.

## Potential Interaction Techniques
Up until to the writing of this blog post, only little and superficial thinking in terms of interaction techniques has been done.
Generally, the questions which are currently unanswered are:
* how do users want to link regions?
* how does the selection process look like?
* how does the deselection process look like?
* how to correct potential mistakes? 
* how to provide undo? (timeline within SI context?)
* how do users want to break links?
* how do users want to perform linking-graph compression?
* how do users want to perform linking-graph decompression?
* how to move a linking-graph as a unit?

These questions will be evaluated once a more mature version of SI is implemented.
