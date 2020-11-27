//
// Created by Zhen Peng on 11/25/2020.
//

#include "ligra.h"

struct Search {
    float *distances_;
    bool *is_visited_;
    float *data_load_;
    float *query_load_;
    int query_id_;

    Search(float *dist, bool *is_visited, float *data_load, float *query_load, int query_id) :
            distances_(dist), is_visited_(is_visited), data_load_(data_load), query_load_(query_load), query_id_(query_id) {}
    inline bool update(int s, int d)
    {
        distances_[d] = compute_distance(&query_load_[query_id_], &data_load_[d]);
        is_visited_[d] = 1;
    }

    inline bool cond (int d)
    {
        return is_visited_[d];
    }

    float compute_distance(float *query_data, float *v_data)
    {
        // Compute distance and return
    }
}

void Compute(
        graph GA,
        int query_id,
        float *data_load,
        float *query_load,
        int starting_point)
{
    float *distances = new float[GA.n];
    bool *is_visited = new bool[GA.n];
    priorityQueue rts; // final results
    rts.add(starting_point);
    vertexSubset Frontier = find_top_m(rts); // or top-1
    while (!Frontier.isEmpty()) {
        vertexSubset output = edgeMap(GA, Frontier, Search(...)); // output only contains scalar IDs.
        for (int v_id : output) {
            float dist = distances[v_id];
            rts.add({v_id, dist}); // add new element to the queue.
        }
        Frontier = find_top_m(rts);
    }
}