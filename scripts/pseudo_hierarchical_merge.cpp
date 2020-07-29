//
// Created by Zhen Peng on 7/23/20.
//

void search_one_iteration(
        Query X,
        Queue Q,
        Set S, // Unchecked vertices to be expanded
        Graph G)
{
    for (every vertex U in S) {
        Mark U as checked;
        for (every unvisited neighbor V of U) {
            Mark V as checked;
            Compute dist(V, X);
            Add V into Q;
        }
    }
}

Queue Search-with-Hierarchical-Merge(
        Graph G,
        Vertex P, // The starting point
        Query X,
        int M, // Maximum value M for top-M
        int L, // value L for queue size
        int K, // value K for results size
        int T) // Number of workers
{
    Qs = T queues of capacity L, every worker has 1 queue;
    if (T is 1, 2, 4) {
        // Use the last queue Qs[T - 1] as the global queue
        Add P, neighbors of P, and random vertices totally L into Qs[T - 1];
        while (Qs[T - 1] contains unchecked vertices) {
            S = first M unchecked vertices in Qs[T - 1];
            Divide S into T parts;
            for-in-parallel (every worker W) { // W is 0, 1, 3, ..., T - 1
                search_one_iteration(
                                X, // Query X
                                Qs[W], // The queue
                                S[W], // Unchecked vertices to be expanded
                                G); // Graph G
            }
            for-in-paralle (every worker W) {
                Merge Qs[W] into Q[T - 1];
                Clear Qs[W];
            }
        }
    } else if (T is 8, 16, 32, ...) {
        // The hierarchy level is only 2 right now.
        // Every 4 workers is a group;
        // the last queue of every 4 queue is the semi-global queue;
        // and Qs[T - 1] is the global queue;
        // In every F iterations, merge semi-global queues into the global queue;
        // in other iterations, merge every queue only to its semi-global queue.
        Add P, neighbors of P, and random vertices into Qs[3], Qs[7], ..., Qs[T - 1];
        iter = 0;
        Y = false; // Y means finished;
        while (!Y) {
            Y = true;
            ++iter;
            for-in-parallel (every group B in T/4 groups) { // B is 0, 1, 2, ..., T/4 - 1
                S = first M unchecked vertices in Qs[4 * B + 3];
                if (S is empty) continue;
                Y = false; // S is not empty, which means not finished yet.
                Divide S into 4 parts;
                for-in-parallel (every worker W in B) { // W is 0, 1, 2, 3
                    search_one_iteration(
                            X, // Query X
                            Qs[W + 4 * B], // The queue
                            S[W + 4 * B], // Unchecked vertices to be expanded
                            G); // Graph G
                }
                // Merge only to the semi-global queue in this group.
                for-in-parallel (every worker W in B) { // W is 0, 1, 2, 3
                    Merge Qs[W + 4 * B] into Qs[4 * B + 3];
                    Clear Qs[W + 4 * B];
                }
            }
            if (!Y && iter % F == 0) {
                // Merge semi-global queues into the global queue.
                for-in-parallel (every group B in T/4 groups) { // B is 0, 1, 2, ..., T/4 - 1
                    Merge Qs[4 * B + 3] into Qs[T - 1];
                    Clear Qs[4 * B + 3];
                }
                // How to avoid the following copy?
                Copy every vertex in Qs[T - 1] into Qs[3], Qs[7], Qs[11], ... by Round-robin way; // How to avoid copy?
            }
        }
    }
}
