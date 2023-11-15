# PCEC
FAPESP Undergraduate Research Project

PCEC - Minimum Path-Collection Exact Cover:

Given a Graph $G$ and a set $S$ of paths in $G$, find a minimum subset of paths in $S$ such that every edge is contained in exactly one path. This problem is NP-hard and we solved it with a Branch and Bound algorithm.

Working on right now:

* Preprocessing
    * Gomory-Hu
* Bounds
    * Efficiency
    * Packing
    * Sum over Packing
    * Costly discards/inclusions
    * Reduction rules
* Order of processing
    * Upper and Lower Bound propagation
        1. Calculate for every component its Lower Bound
        2. Sort the components based on some heuristic
        3. Solve the components in order and substitute its Lower Bound for its actual value
        4. Lower Bound of the parent is always 1 + sum of LB of children
        5. We can start the solve of the children with the UB of the maximum value its solution can be for it to be possible to get a better solution
    * Test different things
