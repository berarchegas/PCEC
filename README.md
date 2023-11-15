# PCEC
FAPESP Undergraduate Research Project

PCEC - Minimum Path-Collection Exact Cover:

Given a Graph $G$ and a set $S$ of paths in $G$, find a minimum subset of paths in $S$ such that every edge is contained in exactly one path. This problem is NP-hard and we solved it with a Branch and Bound algorithm.

Working on right now:

We implemented the UB propagation with hashing, but it turns out it doesnt work in the trivial way

So the working code is in mainNoHash, I still have to make it more clean, think about the vis vector, implement the other bounds and integrate with the nerfed hashing 

* Preprocessing
    * Gomory-Hu
* Bounds
    * Efficiency (Implemented)
    * Packing
    * Sum over Packing
    * Costly discards/inclusions
    * Reduction rules
* Order of processing
    * Test different things
