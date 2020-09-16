//
// Created by Zhen Peng on 08/31/2020.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include <clocale>
#include <omp.h>
//#include "../include/papi_panns.h"
//#include "../core/Searching.202002101535.reorganization.h"
//#include "../core/Searching.201912161559.set_for_queue.h"
//#include "../core/Searching.201912091448.map_for_queries_ids.h"
//#include "../core/Searching.h"
//#include "../include/utils.h"
//#include "../include/efanna2e/index_nsg.h"
//#include "../core/Searching.202002141745.critical_omp_top_m.h"
//#include "../core/Searching.202002181409.local_queue_and_merge.h"
//#include "../core/Searching.202002201424.parallel_merge_local_queues.h"
//#include "../core/Searching.202003021000.profile_para_top_m_search.h"
//#include "../core/Searching.202004131634.better_merge.h"
//#include "../core/Searching.202005271122.choosing_m.h"
//#include "../core/Searching.202006191549.nested_parallel.h"
//#include "../core/Searching.202008061153.less_sync.h"
//#include "../core/Searching.202008062049.computation_quota.h"
//#include "../core/Searching.202008090117.interval_merge.h"
//#include "../core/Searching.202008101718.interval_merge_v2.h"
//#include "../core/Searching.202008141252.interval_merge_v4.h"
//#include "../core/Searching.202008152055.interval_merge_v5.h"
//#include "../core/Searching.202008211350.simple_top_m.h"
#include "../core/Searching.202008310636.simple_v3.h"
using idi = PANNS::idi;

void load_true_NN(
        const char *filename,
        std::vector< std::vector<std::pair<idi, float> > > &true_nn_list)
//        unsigned &t_K)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    idi t_query_num;
    idi t_K;
//    unsigned t_K;
    fin.read(reinterpret_cast<char *>(&t_query_num), sizeof(t_query_num));
    fin.read(reinterpret_cast<char *>(&t_K), sizeof(t_K));

//    data = new unsigned[(size_t) t_query_num * (size_t) t_K];
    true_nn_list.resize(t_query_num);
    for (idi q_i = 0; q_i < t_query_num; ++q_i) {
        true_nn_list[q_i].resize(t_K);
    }

    for (unsigned q_i = 0; q_i < t_query_num; ++q_i) {
        for (unsigned n_i = 0; n_i < t_K; ++n_i) {
            unsigned id;
            float dist;
            fin.read(reinterpret_cast<char *>(&id), sizeof(id));
            fin.read(reinterpret_cast<char *>(&dist), sizeof(dist));

            true_nn_list[q_i][n_i] = std::make_pair(id, dist);
//            true_nn_list[q_i][n_i] = id;
        }
    }

    fin.close();
}

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <true1> <true2>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

//    PANNS::Searching engine;
//    engine.load_data_load(argv[1]);
//    engine.load_queries_load(argv[2]);
//    engine.load_nsg_graph(argv[3]);

//    engine.build_opt_graph();

//    unsigned L = strtoull(argv[4], nullptr, 0);
//    unsigned K = strtoull(argv[5], nullptr, 0);
//    unsigned M_max = strtoull(argv[7], nullptr, 0);
//    if (L < K) {
//        fprintf(stderr, "Error: search_L %u is smaller than search_K %u\n.", L, K);
//        exit(EXIT_FAILURE);
//    }
//    if (K < M_max) {
////        fprintf(stderr, "Error: search_K %u is smaller than value_M %u.\n", K, M_max);
////        exit(EXIT_FAILURE);
//        fprintf(stderr, "Warning: search_K %u is smaller than value_M %u.\n", K, M_max);
//    }

    std::vector< std::vector< std::pair<idi, float> > > true_nn_list1;
    std::vector< std::vector< std::pair<idi, float> > > true_nn_list2;

    load_true_NN(
            argv[1],
            true_nn_list1);
    load_true_NN(
            argv[2],
            true_nn_list2);

    for (unsigned v_id = 0; v_id < true_nn_list1.size(); ++v_id) {
        for (unsigned n_id = 0; n_id < true_nn_list1[v_id].size(); ++n_id) {
            if (true_nn_list1[v_id][n_id].first != true_nn_list2[v_id][n_id].first) {
//            if (true_nn_list1[v_id][n_id] != true_nn_list2[v_id][n_id]) {
                printf("true1[%u][%u]: (%u, %f) "
                       "true2[%u][%u]: (%u, %f)\n",
                       v_id, n_id, true_nn_list1[v_id][n_id].first, true_nn_list1[v_id][n_id].second,
                       v_id, n_id, true_nn_list2[v_id][n_id].first, true_nn_list2[v_id][n_id].second);
            }
        }
    }

    return 0;
}
