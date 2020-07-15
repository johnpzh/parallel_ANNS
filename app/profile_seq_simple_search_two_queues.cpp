//
// Created by Zhen Peng on 7/12/2020.
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
#include "../core/Searching.202007121434.two_queues.h"

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <true_NN_file>\n",
//            "Usage: %s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <value_M_max> <true_NN_file> <num_threads>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 8) {
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
//    unsigned M_max = strtoull(argv[7], nullptr, 0);
    if (L < K) {
        fprintf(stderr, "Error: search_L %u is smaller than search_K %u\n.", L, K);
        exit(EXIT_FAILURE);
    }

    std::vector< std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[7],
            true_nn_list);

    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

//    int num_threads_max = strtoull(argv[9], nullptr, 0);
//    int num_threads_max = 20;
//    for (int num_threads = 1; num_threads < num_threads_max + 1; num_threads *= 2) {
//    int num_threads = strtoull(argv[9], nullptr, 0);
//    engine.num_threads_ = num_threads;
//    omp_set_num_threads(num_threads);

//    unsigned M_middle = strtoull(argv[10], nullptr, 0);
//            unsigned local_queue_length = L;
//            unsigned base_set_L = (num_threads - 1) * local_queue_length;
//            if (!local_queue_length) {
//                local_queue_length = 1;
//            }
//            unsigned value_M = M_max;
            unsigned warmup_max = 4;
            for (unsigned warmup = 0; warmup < warmup_max; ++warmup) {
//                {//test
//                    engine.time_memmove_list_.resize(num_threads, 0);
//                }
                std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
                for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

                std::vector<PANNS::idi> init_ids(L);
                std::vector<PANNS::Neighbor> set_L(L);
//                std::vector<PANNS::Candidate> set_L(L); // Return set
//                std::vector<PANNS::Candidate> set_L(L + (num_threads - 1) * local_queue_length); // Return set
//                std::vector<PANNS::idi> local_queues_ends(num_threads, 0);
//                std::vector<uint8_t> is_visited(points_num, 0);
                boost::dynamic_bitset<> is_visited(points_num);
//                std::vector<PANNS::idi> top_m_candidates(value_M);
//                PANNS::idi local_queue_length = (L - 1) / num_threads + 1;
//                std::vector< std::vector<PANNS::idi> > top_m_candidates_list(num_threads,
//                                                                            std::vector<PANNS::idi>(local_queue_length));
                std::vector<PANNS::Neighbor> worklist(points_num);
//                std::vector<PANNS::Neighbor> worklist(L);
                auto s = std::chrono::high_resolution_clock::now();
                engine.prepare_init_ids(
                        init_ids,
                        L,
                        is_visited);
                for (unsigned q_i = 0; q_i < query_num; ++q_i) {
                    engine.simple_search_with_two_queues_seq(
                            q_i,
                            L,
                            set_L,
                            worklist,
                            init_ids,
                            is_visited,
                            K,
                            set_K_list[q_i]);
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
                }
                {// Basic output
                    printf(
//                           "num_threads: %d "
//                           "M: %u "
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
                           "addtime(s.): %f "
                           "addcount: %lu \n",
//                           "local_queue_length: %u "
//                           "M_middle: %u "
//                           "merge_time(s.): %f \n",
//                           "memmove_time(s.): %f\n",
//                           "num_local_elements: %lu\n",
//                           num_threads,
//                           value_M,
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
                           engine.time_add_to_queue_,
                           engine.count_add_to_queue_);
//                           local_queue_length,
//                           M_middle,
//                           engine.time_merge_);
//                           time_memmove);
//                           engine.number_local_elements_);
                    engine.count_distance_computation_ = 0;
                    engine.time_add_to_queue_ = 0;
                    engine.count_add_to_queue_ = 0;
//                    engine.time_merge_ = 0;
//                    engine.number_local_elements_ = 0;
//                    cache_miss_rate.print();
                }
                PANNS::DiskIO::save_result(argv[6], set_K_list);
            }
//    }

    return 0;
}
