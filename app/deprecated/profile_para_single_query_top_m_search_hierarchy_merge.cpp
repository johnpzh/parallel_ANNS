//
// Created by Zhen Peng on 7/29/2020.
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
//#include "../core/Searching.202006222053.subsearch.h"
//#include "../core/Searching.202007271719.gather_top_m.subsearch.profile.h"
//#include "../core/Searching.202007281116.only_gather_top_m.profile.h"
//#include "../core/Searching.202007281234.only_gather_top_m.h"
//#include "../core/Searching.202007291720.hierarchy.h"
#include "../core/Searching.202008061153.less_sync.h"


void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <L> <K> <result_file> <M_max> <true_NN_file> <num_threads> <M_middle> <local_L> <local_master_L> <full_merge_freq>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 14) {
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

    unsigned L = strtoull(argv[4], nullptr, 0);
    unsigned K = strtoull(argv[5], nullptr, 0);
    unsigned M_max = strtoull(argv[7], nullptr, 0);
    if (L < K) {
        fprintf(stderr, "Error: search_L %u is smaller than search_K %u\n.", L, K);
        exit(EXIT_FAILURE);
    }
//    if (K < M_max) {
////        fprintf(stderr, "Error: search_K %u is smaller than value_M %u.\n", K, M_max);
////        exit(EXIT_FAILURE);
//        fprintf(stderr, "Warning: search_K %u is smaller than value_M %u.\n", K, M_max);
//    }

    std::vector< std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[8],
            true_nn_list);

    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

//    int num_threads_max = strtoull(argv[9], nullptr, 0);
//    int num_threads_max = 20;
//    for (int num_threads = 1; num_threads < num_threads_max + 1; num_threads *= 2) {
    int num_threads = strtoull(argv[9], nullptr, 0);
    engine.num_threads_ = num_threads;
//    engine.num_threads_intra_query_ = num_threads;
    omp_set_num_threads(num_threads);
//    omp_set_nested(1);
    omp_set_max_active_levels(2);

    unsigned M_middle = strtoull(argv[10], nullptr, 0);
    unsigned local_queue_capacity = strtoull(argv[11], nullptr, 0);
    unsigned local_master_queue_capacity = strtoull(argv[12], nullptr, 0);
    unsigned full_merge_freq = strtoull(argv[13], nullptr, 0);
//        int warmup_max = 1;

//    const PANNS::idi local_queue_length = L;
//        for (unsigned value_M = 2; value_M <= M_max; value_M *= 2) {
//        for (unsigned local_queue_length = 0; local_queue_length <= 3 * L; local_queue_length += L/10) {
//            unsigned local_queue_length = 1;
//    unsigned local_queue_length = L;
//            unsigned local_queue_length = L / num_threads;
//            unsigned local_queue_length = M_max * engine.width_;
//    unsigned base_set_L = (num_threads - 1) * local_queue_length;
//    if (!local_queue_length) {
//        local_queue_length = 1;
//    }
    const unsigned group_size = 4;
    const unsigned num_groups = (num_threads - 1) / group_size + 1;
    unsigned value_M = M_max;
    unsigned warmup_max = 1;
    for (unsigned warmup = 0; warmup < warmup_max; ++warmup) {
        std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
        for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

        std::vector<PANNS::idi> init_ids(L);
//                std::vector<uint8_t> is_visited(points_num, 0);
        boost::dynamic_bitset<> is_visited(points_num);
        std::vector<PANNS::Candidate> set_L(
                (num_groups - 1) * ((group_size - 1) * local_queue_capacity + local_master_queue_capacity)
                + (group_size - 1) * local_queue_capacity + L);
        std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
        std::vector<PANNS::idi> local_queues_starts(num_threads);
        for (int q_i = 0; q_i < num_threads; ++q_i) {
            unsigned group_id = q_i / group_size;
            unsigned g_sub = q_i % group_size;
            unsigned group_base = group_id * ((group_size - 1) * local_queue_capacity + local_master_queue_capacity);
            local_queues_starts[q_i] = group_base + g_sub * local_queue_capacity;
        }
//        std::vector<PANNS::idi> top_m_candidates(value_M);
//        std::vector< std::vector<PANNS::idi> > top_m_candidates_list(num_groups,
//                                                            std::vector<PANNS::idi>(value_M));
        std::vector<PANNS::idi> top_m_candidates(num_groups * value_M);
        std::vector<PANNS::idi> top_m_candidates_starts(num_groups);
        for (unsigned g_i = 0; g_i < num_groups; ++g_i) {
            top_m_candidates_starts[g_i] = g_i * value_M;
        }
        std::vector<PANNS::idi> top_m_candidates_sizes(num_groups, 0);
        auto s = std::chrono::high_resolution_clock::now();
        engine.prepare_init_ids(init_ids, L);
//#pragma omp parallel for
        for (unsigned q_i = 0; q_i < query_num; ++q_i) {
            engine.para_search_with_top_m_hierarchy_merge_v0(
                    M_middle,
                    value_M,
                    q_i,
                    K,
                    L,
                    set_L,
                    init_ids,
                    set_K_list[q_i],
                    local_queue_capacity,
                    local_master_queue_capacity,
                    local_queues_starts,
                    local_queues_sizes,
                    top_m_candidates,
                    top_m_candidates_starts,
                    top_m_candidates_sizes,
                    is_visited,
                    group_size,
                    full_merge_freq);
        }
        auto e = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = e - s;
        std::unordered_map<unsigned, double> recalls;
        { // Recall values
            engine.get_recall_for_all_queries(
                    true_nn_list,
                    set_K_list,
                    recalls);
//                printf("P@1: %f "
//                       "P@5: %f "
//                       "P@10: %f "
//                       "P@20: %f "
//                       "P@50: %f "
//                       "P@100: %f\n",
//                       recalls[1],
//                       recalls[5],
//                       recalls[10],
//                       recalls[20],
//                       recalls[50],
//                       recalls[100]);

//                    printf("num_threads: %u "
//                           "M: %u "
//                           "L: %u "
//                           "searching_time(s.): %f "
//                           "P@100: %f\n",
////                           "count_distance_computation: %'lu\n",
//                           num_threads_max,
//                           value_M,
//                           L,
//                           diff.count(),
//                           recalls[100]);
////                           engine.count_distance_computation);
////                    engine.count_distance_computation = 0;
//                    for (PANNS::idi q_i = 0; q_i < query_num; ++q_i) {
//                        std::sort(set_K_list[q_i].begin(), set_K_list[q_i].end());
//                        std::sort(true_nn_list[q_i].begin(), true_nn_list[q_i].end());
//                        for (unsigned t_i = 0; t_i < 100; ++t_i) {
//                            if (set_K_list[q_i][t_i] == true_nn_list[q_i][t_i]) {
//                                continue;
//                            }
//                            printf("q_i: %u "
//                                   "set_K[%u]: %u "
//                                   "true[%u]: %u\n",
//                                   q_i,
//                                   t_i, set_K_list[q_i][t_i],
//                                   t_i, true_nn_list[q_i][t_i]);
//                        }
//                    }
        }
        {// Basic output
            printf(
//                            "local_queue_length: %u "
                    "num_threads: %d "
                    "M: %u "
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
                    "local_master_L: %u "
                    "M_middle: %u "
                    "Freq: %u \n",
//                    "merge_time(s.): %f\n",
//                           "num_local_elements: %lu\n",
//                           local_queue_length,
                    num_threads,
                    value_M,
                    L,
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
                    data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ / (1U << 30U) / diff.count(),
                    local_queue_capacity,
                    local_master_queue_capacity,
                    M_middle,
                    full_merge_freq);
//                    engine.time_merge_);
//                           engine.number_local_elements_);
            engine.count_distance_computation_ = 0;
//            engine.time_merge_ = 0;
//                    engine.number_local_elements_ = 0;
//                    cache_miss_rate.print();
        }
//            { // Percentage of Sharing
//                unsigned num_measure_quries = strtoull(argv[10], nullptr, 0);
//                for (unsigned q_i = 0; q_i < num_measure_quries; q_i += 2) {
//                    double pcnt_has_shared_iterations;
//                    double avg_pcnt_shared_top_m;
//                    get_percentage_of_sharing_in_top_m(
//                            queries_top_m_list[q_i],
//                            queries_top_m_list[q_i + 1],
//                            pcnt_has_shared_iterations,
//                            avg_pcnt_shared_top_m);
//                    printf("%u-%u pcnt_has_shared_iterations: %f avg_pcnt_shared_top_m: %f\n",
//                            q_i, q_i + 1, pcnt_has_shared_iterations, avg_pcnt_shared_top_m);
//                }
//            }
        PANNS::DiskIO::save_result(argv[6], set_K_list);
    }
//        }
//        }
//    }

    return 0;
}
