iQAN(graph G, starting point P, query Q, queue capacity L, output size K) {
    priority_queue pq; // GraphIt has
    P.dist = compute_dist(P, Q); // GraphIt misses
    pq.insert(p); // GraphIt has

    while (pq has unchecked vertices) { // GraphIt misses
        v = the 1st unchecked vertex in pq; // GraphIt misses
        v.checked = true; // GraphIt misses
        for (neighbor u of v if visited[u] == false) { // GraphIt has
            visited[u] = true;
            u.dist = compute_dist(u, Q); // GraphIt misses
            pq.insert(u); // GraphIt has
        }
        if (pq.size() > L) {
            pq.resize(L); // GraphIt misses
        }
    }

    return first K vertices in pq;
}


iQAN(graph G, starting point P, query Q, queue capacity L, output size K) {
    priority_queue pq; // GraphIt has
    P.dist = compute_dist(P, Q); // GraphIt misses
    pq.insert(p); // GraphIt has
    priority_queue ret; // GraphIt misses

    while (ret.size() < L || ret.updated) { // GraphIt misses
        vs = the 1st bucket in pq; // GraphIt has
        clear the 1st bucket in pq; // GraphIt has
        for_para (v in vs) { // GraphIt has
            for (neighbor u of v if visited[u] == false) { // GraphIt has
                visited[u] = true;
                u.dist = compute_dist(u, Q); // GraphIt misses
                pq.insert(u); // GraphIt has
                if (unchecked[u] == true) {
                    ret.updated = ret.insert(u);
                }
            }
        }
        if (ret.size() > L) { // GraphIt misses (not sure)
            ret.resize(L); // GraphIt misses
        }
    }

    return first K vertices in ret;
}