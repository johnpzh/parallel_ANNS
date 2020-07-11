//
// Created by Zhen Peng on 7/10/20.
//

/*
 * The base line algorithm SimpleSearch.
 * Simple Search selects the closest (top 1) vertex and accesses its neighbors in every iteration.
 * This version's difference is using 2 queues for results and unvisited vertices, respectively.
 * The original version only uses 1 queue.
 */
Queue SimpleSearch_using_two_queues(
        Graph G, // Graph data
        Vertex P, // The starting vertex
        Query Q, // Query point
        Int L, // Queue size
        Int K) // Results size
{
    R = The result queue of capacity L; // A priority queue, storing closest L vertices that have been visited.
    W = The worklist queue; // A priority queue, storing unvisited vertices
    R = empty; W = empty;

    // Initilzation
    Compute dist(P, Q);
    Add P into W;
    for (every neighbor N of P) {
        Compute dist(N, Q);
        Add N into W;
    }
    is_finished = false;

    while (!is_finished) {
        Vertex V = W[0]; // V is the closest unvisited vertex
        Remove W[0] from W;
        Mark V as visited;
        Add V into R; // Try to add V to R
        if (Successfully added V into R) {
            is_finished = false;
        } else { // Unsuccessful means V is not the L closest vertices to Q
            is_finished = true;
        }
        // Access all neighbors of V
        for (every neighbor N of V) {
            if (N is unvisited) {
                Compute dist(N, Q);
                Add N into W;
            }
        }
    }

    return the first K vertices in R;
}