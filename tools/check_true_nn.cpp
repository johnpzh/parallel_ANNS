//
// Created by Zhen Peng on 09/21/2020.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include <clocale>
#include <omp.h>
#include "../core/Searching.202009171601.simple_v3.large_graph.h"

using idi = PANNS::idi;

void check_true_nn_for_one_query(
        const unsigned query_id,
        const unsigned K,
        const PANNS::Searching &engine,
        const std::vector< std::pair<idi, float> > &true_nn)
//        std::vector<PANNS::Candidate> &candidates)
{
    const unsigned num_v = engine.num_v_;
    const uint64_t dim = engine.dimension_;
    const float *data = engine.data_load_;
    const float *query = engine.queries_load_ + query_id * dim;
    // Compute nearest neighbors from query and data
    std::vector<PANNS::Candidate> candidates(num_v);
//#pragma omp parallel for
    for (unsigned v_id = 0; v_id < num_v; ++v_id) {
        float dist = engine.compute_distance(data + v_id * dim, query);
        candidates[v_id] = PANNS::Candidate(v_id, dist, false);
    }

    // Sort
    std::sort(candidates.begin(), candidates.end());

    // Check
    for (unsigned e_i = 0; e_i < K; ++e_i) {
        if (true_nn[e_i].first != candidates[e_i].id_) {
            printf("query_id: %u "
                   "true_nn[%u]: (%u, %f)"
                   "cands[%u]: (%u, %f)\n",
                   query_id,
                   e_i, true_nn[e_i].first, true_nn[e_i].second,
                   e_i, candidates[e_i].id_, candidates[e_i].distance_);
        }
    }
}

void load_true_NN(
        const char *filename,
        const unsigned num_queries,
        std::vector< std::vector< std::pair<idi, float> > > &true_nn_list)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    idi t_query_num;
    idi t_K;
    fin.read(reinterpret_cast<char *>(&t_query_num), sizeof(t_query_num));
    fin.read(reinterpret_cast<char *>(&t_K), sizeof(t_K));

    if (t_query_num < num_queries) {
        fprintf(stderr, "Error: t_query_num %u is smaller than num_queries_ %u\n", t_query_num, num_queries);
        exit(EXIT_FAILURE);
    }
    if (t_K < 100) {
        fprintf(stderr, "Error: t_K %u is smaller than 100.\n", t_K);
        exit(EXIT_FAILURE);
    }

//    data = new unsigned[(size_t) t_query_num * (size_t) t_K];
    true_nn_list.resize(t_query_num);
    for (idi q_i = 0; q_i < t_query_num; ++q_i) {
        true_nn_list[q_i].resize(t_K);
    }

    for (unsigned q_i = 0; q_i < t_query_num; ++q_i) {
//        size_t offset = q_i * t_K;
        for (unsigned n_i = 0; n_i < t_K; ++n_i) {
            unsigned id;
            float dist;
            fin.read(reinterpret_cast<char *>(&id), sizeof(id));
            fin.read(reinterpret_cast<char *>(&dist), sizeof(dist));
//            data[offset + n_i] = id;
            true_nn_list[q_i][n_i].first = id;
            true_nn_list[q_i][n_i].second = dist;
        }
    }

    fin.close();
}

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <true_NN_file> <K> <num_t>\n",
//            "Usage: %s <data_file> <query_file> <nsg_path> <L> <K> <result_file> <true_NN_file> <num_threads> <local_L> <sub_iters>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 6) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

    PANNS::Searching engine;
    printf("Loading %s ...\n", argv[1]);
    engine.load_data_load(argv[1]);

    printf("Loading %s ...\n", argv[2]);
    engine.load_queries_load(argv[2]);

    //    unsigned data_dimension = engine.dimension_;
//    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

    std::vector< std::vector< std::pair<idi, float> > > true_nn_list;
    printf("Loading %s ...\n", argv[3]);
    load_true_NN(
            argv[3],
            query_num,
            true_nn_list);
//    engine.load_true_NN(
//            argv[3],
//            true_nn_list);

    unsigned K = strtoull(argv[4], nullptr, 0);
    int num_threads = strtoull(argv[5], nullptr, 0);
    engine.num_threads_ = num_threads;
    omp_set_num_threads(num_threads);
//    std::vector<PANNS::Candidate> candidates(points_num);

    printf("Checking ...\n");
#pragma omp parallel for
    for (unsigned query_id = 0; query_id < query_num; ++query_id) {
        check_true_nn_for_one_query(
                query_id,
                K,
                engine,
                true_nn_list[query_id]);
//                candidates);
    }

    return 0;
}
