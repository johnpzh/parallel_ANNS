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
    R = The result queue of capacity L; // A priority queue
    W = The worklist queue of capacity L; // A priority queue
    R = empty; W = empty;

    // Initilzation
    Compute dist(P, Q);
    Add P into W;
    Mark P as visited;

    while (W is not empty) {
        Vertex V = W[0]; // V is the closest unvisited vertex
        Remove W[0] from W;
        if (R.size == L && dist(V, Q) > dist(R[L - 1], Q)) {
            // Stop condition
            break;
        }
        Add V into R;
        // Access all neighbors of V
        for (every neighbor N of V) {
            if (N is unvisited) {
                Mark N as visited;
                Compute dist(N, Q);
                Add N into W;
            }
        }
    }

    return the first K vertices in R;
}