//
// Created by Zhen Peng on 9/26/19.
//

Queue Selecting_Lth_idea(
            Graph G,
            Vertex P, // The start/entry point
            Query Q, // The query
            Int M, // Some value M
            Int L, // Candidate pool/queue size
            Int K, // Return pool/queue size
            Int T // Number of threads
)
{
    Local Queues Ss = T priority queues whose capacity are L;
    // Select initial vertices to fill Queue Ss.
    Initialize Ss by only L vertices that are neighbors of Vertex P and other random vertices if necessary;
    Sort every queue Ss[t] according to distances to Query Q;
    Lth = the L-th minimum elements in all Ss;

    while (Any Queue Ss[t] contains unchecked elements) {
        for (every thread t) {
            A Set P = First M/T unchecked candidates in Queue Ss[t];
            for (every Candidate C in Set P) {
                Mark Candidate C as checked;
                for (every unvisited Neighbor N of Candidate C) {
                    Mark Neighbor N as visited;
                    Compute the distance between Neighbor N and Query Q;
                    if (distance(N, Q) > Lth) // If N to Q is even further than S's longest distance
                        continue;
                    Insert Neighbor N into Queue Ss[t];
                }
            }
        }
        Lth = the L-th minimum elements in all Ss;
        for (every thread t) {
            Ss[t] drops any elements larger than Lth;
        }
    }
    Copy top-K elements from Ss into set S;

    return S;
}

Queue Collector-Idea(
            Graph G,
            Vertex P, // The start/entry point
            Query Q, // The query
            Int M, // Some value M
            Int L, // Candidate pool/queue size
            Int K // Return pool/queue size
)
{
    Queue S = a priority queue whose capacity is L;
    // Select initial vertices to fill Queue S.
    Queue S initializes L candidates consisting of neighbors of Vertex P and other random vertices if necessary;
    Sort Queue S according to distances to Query Q;
    Every Collector thread has a local queue;

    while (Queue S contains unchecked elements) {
        A Set T = First M unchecked candidates in Queue S;
        for (every Candidate C in Set T) {
            Mark Candidate C as checked;
            for (every unvisited Neighbor N of Candidate C) {
                Mark Neighbor N as visited;
                Compute the distance between Neighbor N and Query Q;
                if (distance(N, Q) > S.last()) // If N to Q is even further than S's longest distance
                    continue;
                if (C belongs to Master thread) {
                    Insert Neighbor N into Queue S;
                } else { // C belongs to Collector threads
                    Insert Neighbor N into a local queue;
                }
            }
        }
    }
    Merge all local queues to Queue S;

    return First K candidates of S;
}

/*
 * 5/21/2020, Relative Distance Threshold Idea
 */
How_to_Select_Top-M_Candidates(
    Threshold H, // A percentage like 10% or 20%
    Queue,
    Query)
{
    C0 = The 1st unchecked candidate in the queue;
    D0 = C0's distance to the query;
    for (every unchecked candidate C in queue) {
        if (dist(C, query) < H * D0) {
            Select C as one of Top-M;
        } else {
            break;
        }
    }

    return Selected Candidates;
}

Queue Relative_Distance_Threshold_Search(
            Graph G,
            Vertex P, // The start/entry point
            Query Q, // The query
            // Int M_MAX, // Maximum value of M
            Int L, // Candidate pool/queue size
            Int K // Return pool/queue size
)
{
    Candidate Queue S whose capacity is L;
    // Select initial vertices to fill Queue S.
    A Queue S = Neighbors of Vertex P and other random vertices;
    Sort Queue S according to distances to Query Q;

    // Value M = 1;
    while (Queue S contains unchecked elements) {
        A Set T = selected Top-M candidates by the threshold;
        for (every Candidate C in Set T) {
            Mark Candidate C as checked;
            for (every unvisited Neighbor N of Candidate C in Graph G) {
                Mark Neighbor N as visited;
                Compute the distance between Neighbor N and Query Q;
                if (distance(N, Q) > S.last()) // If N to Q is even further than S's longest distance
                    continue;
                Insert Neighbor N into Queue S according to its distance to Query Q; // Queue S is sorted all the time.
            }
        }
        // if (M < M_MAX) {
        //     M = 2 * M;
        // }
    }

    S = First K candidates of S;
    return S;
}

Queue Scale-Top-M-Searching(
            Graph G,
            Vertex P, // The start/entry point
            Query Q, // The query
            Int M_MAX, // Maximum value of M
            Int L, // Candidate pool/queue size
            Int K // Return pool/queue size
)
{
    Candidate Queue S whose capacity is L;
    // Select initial vertices to fill Queue S.
    A Queue S = Neighbors of Vertex P and other random vertices;
    Sort Queue S according to distances to Query Q;

    Value M = 1;
    while (Queue S contains unchecked elements) {
        A Set T = First M unchecked candidates in Queue S;
        for (every Candidate C in Set T) {
            Mark Candidate C as checked;
            for (every unvisited Neighbor N of Candidate C in Graph G) {
                Mark Neighbor N as visited;
                Compute the distance between Neighbor N and Query Q;
                if (distance(N, Q) > S.last()) // If N to Q is even further than S's longest distance
                    continue;
                Insert Neighbor N into Queue S according to its distance to Query Q; // Queue S is sorted all the time.
            }
        }
        if (M < M_MAX) {
            M = 2 * M;
        }
    }

    S = First K candidates of S;
    return S;
}

Queue Scale-Top-M-Searching-with-Local-Queues(
            Graph G,
            Vertex P, // The start/entry point
            Query Q, // The query
            Int M_MAX, // Maximum value of M
            Int L, // Candidate pool/queue size
            Int K, // Return pool/queue size
            Int H // Capacity of local queues
)
{
    Global Candidate Queue S whose capacity is L;
    Local Queues Ps whose capacity is H;
    // Select initial vertices to fill Queue S.
    A Queue S = Neighbors of Vertex P and other random vertices;
    Sort Queue S according to distances to Query Q;

    Value M = 1;
    while (Queue S contains unchecked elements) {
        A Set T = First M unchecked candidates in Queue S;
        for-in-parallel (every Candidate C in Set T) {
            Mark Candidate C as checked;
            for (every unvisited Neighbor N of Candidate C in Graph G) {
                Mark Neighbor N as visited;
                Compute the distance between Neighbor N and Query Q;
                if (distance(N, Q) > S.last()) // If N to Q is even further than S's longest distance
                    continue;
                Insert Neighbor N into Local Queue Ps[T], where T is thread ID; // Ps[T] is sorted all the time.
            }
        }
        Merge Local Queues Ps into S;
        Clear all elements in Ps;
        if (M < M_MAX) {
            M = 2 * M;
        }
    }

    S = First K candidates of S;
    return S;
}

Queue Scale-Top-M-Searching-Seq-Then-Par(
            Graph G,
            Vertex P, // The start/entry point
            Query Q, // The query
            Int M_MAX, // Maximum value of M
            Int L, // Candidate pool/queue size
            Int K // Return pool/queue size
)
{
    Candidate Queue S whose capacity is L;
    // Select initial vertices to fill Queue S.
    A Queue S = Neighbors of Vertex P and other random vertices;
    Sort Queue S according to distances to Query Q;

    Value M = 1;
    while (Queue S contains unchecked elements && M < M_MAX) {
        A Set T = First M unchecked candidates in Queue S;
        for (every Candidate C in Set T) {
            Mark Candidate C as checked;
            for (every unvisited Neighbor N of Candidate C in Graph G) {
                Mark Neighbor N as visited;
                Compute the distance between Neighbor N and Query Q;
                if (distance(N, Q) > S.last()) // If N to Q is even further than S's longest distance
                    continue;
                Insert Neighbor N into Queue S according to its distance to Query Q; // Queue S is sorted all the time.
            }
        }
        M = 2 * M;
    }

    Local Queues Ps whose capacity is H;
    while (Queue S contains unchecked elements) {
        A Set T = First M unchecked candidates in Queue S;
        for-in-parallel (every Candidate C in Set T) {
            Mark Candidate C as checked;
            for (every unvisited Neighbor N of Candidate C in Graph G) {
                Mark Neighbor N as visited;
                Compute the distance between Neighbor N and Query Q;
                if (distance(N, Q) > S.last()) // If N to Q is even further than S's longest distance
                    continue;
                Insert Neighbor N into Local Queue Ps[T], where T is thread ID; // Ps[T] is sorted all the time.
            }
        }
        Merge Local Queues Ps into S;
        Clear all elements in Ps;
    }

    S = First K candidates of S;
    return S;
}

// Searching algorithm: expand top-M candidates in every iteration
// Output: K nearest neighbors for Query Q.
Queue Top-M-Searching(
            Graph G,
            Vertex P, // The start/entry point
            Query Q, // The query
            Int M, // Value of M
            Int L, // Candidate pool/queue size
            Int K // Return pool/queue size
)
{
    Candidate Queue S whose capacity is L;
    // Select initial vertices to fill Queue S.
    A Queue S = Neighbors of Vertex P and other random vertices;
    Sort Queue S according to distances to Query Q;

    while (Queue S contains unchecked elements) {
        A Set T = First M unchecked candidates in Queue S;
        for-in-parallel (every Candidate C in Set T) {
            Mark Candidate C as checked;
            for (every unvisited Neighbor N of Candidate C in Graph G) {
                Mark Neighbor N as visited;
                Compute the distance between Neighbor N and Query Q;
                if (distance(N, Q) > S.last()) // If N to Q is even further than S's longest distance
                    continue;
                Insert Neighbor N into Queue S according to its distance to Query Q; // Queue S is sorted all the time.
            }
        }
    }

    S = First K candidates of S;
    return S;
}

// Searching algorithm: expand top-M candidates in every iteration
// Output: K nearest neighbors for Query Q.
Queue Top-M-Searching-with-Local-Queues(
            Graph G,
            Vertex P, // The start/entry point
            Query Q, // The query
            Int M, // Value of M
            Int L, // Candidate pool/queue size
            Int K, // Return pool/queue size
            Int H // Capacity of local queues
)
{
    Global Candidate Queue S whose capacity is L;
    Local Queues Ps whose capacity is H;
    // Select initial vertices to fill Queue S.
    A Queue S = Neighbors of Vertex P and other random vertices;
    Sort Queue S according to distances to Query Q;

    while (Queue S contains unchecked elements) {
        A Set T = First M unchecked candidates in Queue S;
        for-in-parallel (every Candidate C in Set T) {
            Mark Candidate C as checked;
            for (every unvisited Neighbor N of Candidate C in Graph G) {
                Mark Neighbor N as visited;
                Compute the distance between Neighbor N and Query Q;
                if (distance(N, Q) > S.last()) // If N to Q is even further than S's longest distance
                    continue;
                Insert Neighbor N into Local Queue Ps[T], where T is thread ID; // Ps[T] is sorted all the time.
            }
        }
        Merge Local Queues Ps into S;
        Clear all elements in Ps;
    }

    S = First K candidates of S;
    return S;
}

// Searching algorithm: expand top-M candidates in every iteration
// Output: K nearest neighbors for Query Q.
Queue Simple-Searching(
            Graph G,
            Vertex P, // The start/entry point
            Query Q, // The query
            Int L, // Candidate pool/queue size
            Int K // Return pool/queue size
)
{
    Candidate Queue S whose capacity is L;
    // Select initial vertices to fill Queue S.
    A Queue S = Neighbors of Vertex P and other random vertices;
    Sort Queue S according to distances to Query Q;

    while (Queue S contains unchecked elements) {
        A Candidate C = The 1st unchecked element in Queue S;
        Mark Candidate C as checked;
        for (every unvisited Neighbor N of Candidate C in Graph G) {
            Mark Neighbor N as visited;
            Compute the distance between Neighbor N and Query Q;
            if (distance(N, Q) > S.last()) // If N to Q is even further than S's longest distance
                continue;
            Insert Neighbor N into Queue S according to its distance to Query Q; // Queue S is sorted all the time.
        }
    }

    S = First K candidates of S;
    return S;
}

//// Searching algorithm: expand top-M candidates in every iteration
//// Output: K nearest neighbors for Query Q.
//Queue Top-M-Searching(
//        Graph G,
//        Vertex P, // The start/entry point
//        Query Q, // The query
//        Int M, // Value of M
//        Int L, // Candidate pool/queue size
//        Int K // Return pool/queue size
//        )
//{
//    Candidate Queue S whose size is L;
//    // Select initial vertices to fill Queue S.
//    Queue S = Neighbors of Query P and other random vertices;
//    Sort Queue S according to distances to Query Q;
//
//    Index i is the index of the first unchecked candidate in Queue S;
//    Index i = 0;
//    Index last_i is the index of M-th unchecked candidate in Queue S;
//    Index last_i = L;
//    Index new_i is the index of the top inserted candidate in Queue S;
//    Index new_i = L;
//
//    while (i < L) {
//        Set T = First M unchecked candidates in Queue S starting from Index i;
//        last_i = the index of M-th unchecked candidate in Queue S;
//        new_i = L;
//        for-in-parallel (every Candidate C in Set T){
//            Mark Candidate C as checked;
//            for (every unvisited Neighbor N of Candidate C in Graph G) {
//                Mark Neighbor N as visited;
//                Compute the distance between Neighbor N and Query Q;
//                Insert Neighbor N into Queue S according to its distance to Query Q; // Queue S is sorted all the time.
//                tmp_i = the index of Candidate N newly inserted in Queue S;
//                if (tmp_i < new_i) {
//                    new_i = tmp_i;
//                }
//            }
//        }
//        if (new_i <= last_i) {
//            i = new_i;
//        } else {
//            i = last_i + 1;
//        }
//    }
//
//    S = First K candidates of S;
//    return S;
//}

// Searching algorithm: based on Top-M-Searching, and processes a batch of queries
// Output: K nearest neighbors for every query in the batch
QueueSet Top-M-Searching-in-A-Batch-of-Queries(
        Graph G,
        Vertex P, // The start/entry point
        QuerySet QS, // The batch of Queries
        Int BS, // The batch size
        Int M, // Value of M
        Int L, // Candidate pool/queue size
        Int K // Return pool/queue size
        )
{
    Candidate QueueSet SS whose size is BS;
    for-in-parallel (every Candidate Queue S in QueueSet SS) {
        Queue S is of size L;
        Queue S = Neighbors of Query P and other random vertices;
        Sort Queue S according to distances to Query Q in QuerySet QS;
    }

    Flag not_finished = true;
    while (not_finished) {
        not_finished = false;
        // Build the joint queue
        Queue JQ is to store all top-M candidates from QueueSet SS;
        for-in-parallel (every Queue S in QueueSet SS) {
            Record those M candidates are selected and checked by Query Q;
            Put into Queue JQ the first M unchecked candidates in Queue S;
        }

        // Expand candidates in the joint queue
        for-in-parallel (every Candidate C in Queue JQ) {
            for (every unvisited Neighbor N of Candidate C in Graph G) {
                for (every Query Q that selected Candidate C) {
                    Mark Neighbor N as visited by Query Q;
                    Compute the distance between Neighbor N and Query Q;
                    Insert Neighbor N into Queue S according to its distance to Query Q; // Queue S is sorted all the time.
                    if (Index of Neighbor N newly inserted in Queue S < L) {
                        not_finished = true;
                    }
                }
            }
        }
    }

    for-in-parallel (every Queue S in QueueSet SS) {
        S = First K candidates of S;
    }

    return SS;
}

// Input is a group of queries, not the whole query set.
// Output is that every query (q) in the group get a queue (QS[q]) as its final nearest candidates.
void parallel_searching_2(
        Graph G,
        Vertex P, // The start/entry point
        SearchSize L_search, // Candidate pool size
        Group GQ, // The group of quries
        Queues QS // Every query in the group has a queue of final nearest neighbors
        )
{
    // The start point
    for (every Query q : Group GQ) {
        Queue QS[q].add(P);
    }
    for (every Query q : Group GQ) {
        for (every Neighbor n : Vertex P) {
            Queue QS[q].add(n); // Use binary-search-and-insert to make QS[q] in the order to the distance to q.
            BitMap is_visited[q][n] = true; // Mark Vertex n is visited by Query q
        }
    }

    // Put all top ones in a joint queue
    for (every Query q : Group GQ) {
        Vertex v = QS[q][0]; // Vertex v is Query q's top-1 selection.
        Mark Vertex v as checked by Query q;
        List t_one[v].add(q); // List t_one records which queries have selected Vertex v as their top-1.
        if (Vertex v is not in JointQueue jq) {
            JointQueue jq.add(v); // Add the top-1 into the joint queue.
        }
    }

    while (JointQueue jq is not empty) {
        for (every Vertex v : JointQueue jq) {
            // Push v's neighbors to its selector queries' queues.
            for (every Query q : List t_one[v]) {
                for (every Neighbor n : Vertex v) {
                    if (!is_visited[q][n]) {
                        BitMap is_visited[q][n] = true; // Mark Vertex n is visited by Query q
                        Queues QS[q].add(n); // Use binary-search-and-insert to make QS[q] in the order to the distance to q.
                    }
                }
            }
            List t_one[v].clear();
        }
        JointQueue jq.clear();

        // Form the new JointQueue from all query's queues
        for (every Query q : Group GQ) {
            Vertex v = the first unchecked vertex in Queue QS[q]; // Vertex v is Query q's top-1 selection.
            Index i = the index of Vertex v in Queue QS[q];
            if (i >= L_search)
                continue; // Stop condition.
            Mark Vertex v as checked by Query q;
            List t_one[v].add(q); // List t_one records which queries have selected Vertex v as their top-1.
            if (Vertex v is not in JointQueue jq) {
                JointQueue jq.add(v);
            }
        }
    }
}

// Function: answer a bunch of queries in parallel.
// Assume queries are well-grouped into GQs. Queries in a group get the same final candidates.
void parallel_searching_1(
        Graph G,
        Start Vertex p, // searching start point
        Groups_of_queries GQs, // Well-grouped queries
        Candidate_pool_size L,
        Result_set_size K,
        fused_K_nearest_neighbors_of_queries Ss // size: GQs.size
        )
{
    Neighbor n;
    n.id = p;
    n.distance = 0;
    for (every query q : Qs)
        n.distance += GetDistance(n.id, q); // Get a fused distance.
    for (every group of queries Q : GQs) { // in parallel
        Index n_i = 0;
        while (n_i < L) {
            n_i = the index of the first unchecked vertex in Ss[Q];
            p_i = Ss[Q][n_i]; // the first unchecked vertex
            Mark p_i as checked;
            for (every neighbor n_id : p_i) {
                n.id = n_id;
                n.distance = 0;
                for (every query q : Q)
                    n.distance += GetDistance(n.id, q);
                Ss[Q].add(n); // Use binary-search-and-insert to make Ss[q_i] in order of the distance to Qs[q_i];
            }
            if (Ss[Q].size() > L)
                Ss[Q].resize(L);
        }
        Ss[Q].resize(K);
    }
}

// Function: answer a bunch of queries in parallel.
// Naive idea. Every query q_i maintains its own candidate queue Ss[q_i].
void parallel_searching_0(
        Graph G,
        Start Vertex p, // searching start point
        Queries Qs, // A bunch of queries
        Candidate_pool_size L,
        Result_set_size K,
        K_nearest_neighbors_of_queries Ss // size: Qs.size
        )
{
    Indices Is; // size: Qs.size
    Initial all index in Is as 0;
    Initial all sets in Ss as empty;
    for (every set S : Ss)
        S.add(p);
    for (int q_i = 0; q_i < Qs.size; ++q_i) { // In parallel
        while (Is[q_i] < L) {
            Is[q_i] = the index of the first unchecked vertex in Ss[q_i];
            p_i = Ss[q_i][Is[q_i]]; // the first unchecked vertex in Ss[q_i]
            Mark p_i as checked;
            for (every neighbor n : p_i)
                Ss[q_i].add(n); // Use binary-search-and-insert to make Ss[q_i] in order of the distance to Qs[q_i];
            if (Ss[q_i].size > L)
                Ss.resize(L);
        }
    }
    for (every set S : Ss)
        S.resize(K);
}