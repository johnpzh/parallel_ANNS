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

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <L> <K> <result_file> <true_NN_file> <num_threads> <P@100>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 10) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

    PANNS::Searching engine;
    engine.load_data_load(argv[1]);
    engine.load_queries_load(argv[2]);
    engine.load_nsg_graph(argv[3]);

//    engine.build_opt_graph();

    unsigned L_value = strtoull(argv[4], nullptr, 0);
    unsigned K = strtoull(argv[5], nullptr, 0);
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

    std::vector< std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[7],
            true_nn_list);

    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

    int num_threads = strtoull(argv[8], nullptr, 0);
    engine.num_threads_ = num_threads;
    omp_set_num_threads(num_threads);
//    omp_set_nested(1);
//    omp_set_max_active_levels(2);

//    unsigned local_queue_capacity = strtoull(argv[9], nullptr, 0);

//    unsigned subsearch_iterations = strtoull(argv[10], nullptr, 0);
//    unsigned L_upper = strtoull(argv[11], nullptr, 0);
    double P_dest = strtod(argv[9], nullptr);
    unsigned local_queue_capacity = L_value;
    std::unordered_map<unsigned, double> recalls;
//    unsigned L;
    unsigned X_lower = 1;
    unsigned X_upper = L_value + 5;
    unsigned X_mid;
    double runtime;
    uint64_t compt;


    while (X_lower <= X_upper) {
//        L = (L_lower + L_upper) / 2;
//        local_queue_capacity = L;
        X_mid = (X_lower + X_upper) / 2;


        std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
        for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);
        std::vector<PANNS::idi> init_ids(L_value);
//                std::vector<uint8_t> is_visited(points_num, 0);
        boost::dynamic_bitset<> is_visited(points_num);
        std::vector<PANNS::Candidate> set_L((num_threads - 1) * local_queue_capacity + L_value);
        std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
        std::vector<PANNS::idi> local_queues_starts(num_threads);
        for (int q_i = 0; q_i < num_threads; ++q_i) {
            local_queues_starts[q_i] = q_i * local_queue_capacity;
        }
        auto s = std::chrono::high_resolution_clock::now();
        engine.prepare_init_ids(init_ids, L_value);
//#pragma omp parallel for
        for (unsigned q_i = 0; q_i < query_num; ++q_i) {

            engine.para_search_with_simple_v3(
                    q_i,
                    K,
                    L_value,
                    set_L,
                    init_ids,
                    set_K_list[q_i],
                    local_queue_capacity,
                    local_queues_starts,
                    local_queues_sizes,
                    is_visited,
                    X_mid);
        }
        auto e = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = e - s;
        { // Recall values
            engine.get_recall_for_all_queries(
                    true_nn_list,
                    set_K_list,
                    recalls);
        }
        {// Basic output
            printf(
//                            "local_queue_length: %u "
                    "num_threads: %d "
                    "L: %u "
                    "runtime(s.): %f "
                    "computation: %lu "
                    "K: %u "
                    "Volume: %u "
                    "Dimension: %u "
                    "query_num: %u "
                    "query_per_sec: %f "
                    "avg_latency(ms.): %f "
                    "P@100: %f "
                    "P@1: %f "
                    "G/s: %f "
                    "GFLOPS: %f "
                    "local_L: %u "
                    "X: %u",
//                        "thd_quota: %lu ",
//                        "iters: %lu "
//                        "avg_iter: %f "
//                        "min_iter: %u "
//                        "max_iter: %u\n",
                    //                    "move_top_m(s.): %f "
//                        "full_merge(s.): %f "
//                        "full_merge: %lu ",
//                        "pick(s.): %f "
//                        "expand(s.): %f \n",
//                        "thd_compt: %lu\n",
//                    "merge_time(s.): %f\n",
//                           "num_local_elements: %lu\n",
//                           local_queue_length,
                    num_threads,
                    L_value,
                    diff.count(),
                    engine.count_distance_computation_,
                    K,
                    points_num,
                    data_dimension,
                    query_num,
                    query_num / diff.count(),
                    diff.count() * 1000 / query_num,
                    recalls[100],
                    recalls[1],
                    data_dimension * 4.0 * engine.count_distance_computation_ / (1U << 30U) / diff.count(),
                    data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ / (1U << 30U) /
                    diff.count(),
                    local_queue_capacity,
//                        M_middle,
                    X_mid);
            printf("\n");
        }
        runtime = diff.count();
        compt = engine.count_distance_computation_;
        engine.count_distance_computation_ = 0;
        PANNS::DiskIO::save_result(argv[6], set_K_list);


//        if (recalls[100] < P_dest) {
//            L_lower = L + 1;
//        } else {
//            L_upper = L - 1;
//        }
        if (recalls[100] < P_dest) {
            X_lower = X_mid + 1;
        } else {
            X_upper = X_mid - 1;
        }
    }

    while (recalls[100] < P_dest) {
        ++X_mid;
//        local_queue_capacity = L;

        std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
        for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

        std::vector<PANNS::idi> init_ids(L_value);
//                std::vector<uint8_t> is_visited(points_num, 0);
        boost::dynamic_bitset<> is_visited(points_num);
        std::vector<PANNS::Candidate> set_L((num_threads - 1) * local_queue_capacity + L_value);
        std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
        std::vector<PANNS::idi> local_queues_starts(num_threads);
        for (int q_i = 0; q_i < num_threads; ++q_i) {
            local_queues_starts[q_i] = q_i * local_queue_capacity;
        }
        auto s = std::chrono::high_resolution_clock::now();
        engine.prepare_init_ids(init_ids, L_value);
//#pragma omp parallel for
        for (unsigned q_i = 0; q_i < query_num; ++q_i) {

            engine.para_search_with_simple_v3(
                    q_i,
                    K,
                    L_value,
                    set_L,
                    init_ids,
                    set_K_list[q_i],
                    local_queue_capacity,
                    local_queues_starts,
                    local_queues_sizes,
                    is_visited,
                    X_mid);
        }
        auto e = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = e - s;
        { // Recall values
            engine.get_recall_for_all_queries(
                    true_nn_list,
                    set_K_list,
                    recalls);
        }
        {// Basic output
            printf(
                    "num_threads: %d "
                    "L: %u "
                    "runtime(s.): %f "
                    "computation: %lu "
                    "K: %u "
                    "Volume: %u "
                    "Dimension: %u "
                    "query_num: %u "
                    "query_per_sec: %f "
                    "avg_latency(ms.): %f "
                    "P@100: %f "
                    "P@1: %f "
                    "G/s: %f "
                    "GFLOPS: %f "
                    "local_L: %u "
                    "X: %u",
                    num_threads,
                    L_value,
                    diff.count(),
                    engine.count_distance_computation_,
                    K,
                    points_num,
                    data_dimension,
                    query_num,
                    query_num / diff.count(),
                    diff.count() * 1000 / query_num,
                    recalls[100],
                    recalls[1],
                    data_dimension * 4.0 * engine.count_distance_computation_ / (1U << 30U) / diff.count(),
                    data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ / (1U << 30U) /
                    diff.count(),
                    local_queue_capacity,
                    X_mid);
            printf("\n");
        }
        runtime = diff.count();
        compt = engine.count_distance_computation_;
        engine.count_distance_computation_ = 0;
        PANNS::DiskIO::save_result(argv[6], set_K_list);
    }
    printf("---- FINAL ----\n");
    printf("P_dest: %f "
           "L: %u "
           "X: %u "
           "runtime(s.): %f "
           "P@100: %f "
           "compt.: %lu ",
           P_dest,
           L_value,
           X_mid,
           runtime,
           recalls[100],
           compt);
    printf("\n");
//    printf("---------------\n");

    return 0;
}
