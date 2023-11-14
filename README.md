# PCEC
FAPESP Undergraduate Research Project

PCEC - Minimum Path-Collection Exact Cover:

Given a Graph $G$ and a set $S$ of paths in $G$, find a minimum subset of paths in $S$ such that every edge is contained in exactly one path. This problem is NP-hard and we solved it with a Branch and Bound algorithm.

Working on right now:

* Hashing to avoid overcounting
    * The initial dfs returns the xor of the values of the edges in the component and then we check if that component is already solved
* Bounds
    * Efficiency
    * Packing
    * Sum over Packing
    * Costly discards/inclusions
    * Reduction rules
* Order of processing
    * Upper and Lower Bound propagation
    * Test different things
