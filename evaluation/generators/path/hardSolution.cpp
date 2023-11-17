// Idea is we generate a solution using probability p1
//  Choose a random edge and expand its path, every time we expand, there is a p1% chance the path ends
//  Then we erase the edges in the path and solve recursively
// And then we generate a bunch of other random paths using p2