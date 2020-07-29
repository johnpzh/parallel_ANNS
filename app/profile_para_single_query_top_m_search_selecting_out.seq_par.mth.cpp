//
// Created by Zhen Peng on 7/26/2020.
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
#include "../core/Searching.202006222053.subsearch.h"
//#include "../core/Searching.202007270051.subsearch.profile.h"

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <global_L> <K> <result_path> <value_M_max> <true_NN_file> <num_threads> <local_L> <value_M_middle>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 12) {
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

    const unsigned global_L = strtoull(argv[4], nullptr, 0);
//    unsigned L = strtoull(argv[4], nullptr, 0);
    const unsigned K = strtoull(argv[5], nullptr, 0);
    if (global_L < K) {
        fprintf(stderr, "Error: search_L %u is smaller than search_K %u.\n", global_L, K);
        exit(EXIT_FAILURE);
    }
    const unsigned M_max = strtoull(argv[7], nullptr, 0);
    const int num_threads = strtoull(argv[9], nullptr, 0);
    engine.num_threads_ = num_threads;
    omp_set_num_threads(num_threads);
    const unsigned local_L = strtoull(argv[10], nullptr, 0);
    const unsigned value_M_middle = strtoull(argv[11], nullptr, 0);
//    const unsigned initial_queue_size = (global_L - 1) / num_threads + 1;
//    const unsigned total_L = initial_queue_size * num_threads;
//    if (local_L < initial_queue_size) {
//        fprintf(stderr, "Error: local_L %u is too small to hold %u initial_queue_size in every thread. "
//                        "(global_L: %u num_threads: %u total_L: %u)\n",
//                local_L,
//                initial_queue_size,
//                global_L, num_threads, total_L);
//        exit(EXIT_FAILURE);
//    }
//    unsigned total_L = num_threads * local_L;

//    if (K < M_max) {
////        fprintf(stderr, "Error: search_K %u is smaller than value_M %u.\n", K, M_max);
////        exit(EXIT_FAILURE);
//        fprintf(stderr, "Warning: search_K %u is smaller than value_M %u.\n", K, M_max);
//    }

    std::vector< std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[8],
            true_nn_list);

    const unsigned data_dimension = engine.dimension_;
    const unsigned points_num = engine.num_v_;
    const unsigned query_num = engine.num_queries_;

//    int num_threads_max = strtoull(argv[9], nullptr, 0);
//    int num_threads_max = 20;
//    for (int num_threads = 1; num_threads < num_threads_max + 1; num_threads *= 2) {

//            unsigned local_queue_length = L;
//            unsigned base_set_L = (num_threads - 1) * local_queue_length;
//            if (!local_queue_length) {
//                local_queue_length = 1;
//            }
            const unsigned value_M = M_max;
            const unsigned warmup_max = 4;
            for (unsigned warmup = 0; warmup < warmup_max; ++warmup) {
                std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
                for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

                std::vector<PANNS::idi> init_ids(global_L);
                std::vector<PANNS::Candidate> set_L(global_L + (num_threads - 1) * local_L); // Return set
//                std::vector<PANNS::idi> init_ids(L);
//                std::vector<PANNS::Candidate> set_L(L); // Return set
                std::vector<PANNS::idi> local_queues_starts(num_threads);
                for (int t_i = 0; t_i < num_threads; ++t_i) {
                    if (0 == t_i) {
                        local_queues_starts[t_i] = 0;
                    } else {
                        local_queues_starts[t_i] = global_L + (t_i - 1) * local_L;
                    }
                }
                std::vector<PANNS::idi> local_queues_sizes(num_threads);
                std::vector<PANNS::idi> local_m_counts(num_threads);
                boost::dynamic_bitset<> is_visited(points_num);
//                std::vector< std::vector<PANNS::idi> > top_m_candidates_list(num_threads);
//                for (int t_i = 0; t_i < num_threads; ++t_i) {
//                    if (!t_i) {
//                        top_m_candidates_list[t_i].resize(value_M);
//                    } else {
//                        top_m_candidates_list[t_i].resize(value_M / num_threads + 40);
////                        top_m_candidates_list[t_i].resize(value_M - 10);
////                        top_m_candidates_list[t_i].resize(value_M / num_threads + 20);
//                    }
//                }
                std::vector< std::vector<PANNS::idi> > top_m_candidates_list(num_threads,
                                                                            std::vector<PANNS::idi>(value_M));
                auto s = std::chrono::high_resolution_clock::now();
                engine.prepare_init_ids(init_ids, global_L);
                for (unsigned q_i = 0; q_i < query_num; ++q_i) {
                    engine.para_search_with_top_m_subsearch_v3(
                            value_M,
                            value_M_middle,
                            q_i,
                            K,
                            global_L,
                            local_L,
                            set_L,
                            init_ids,
                            set_K_list[q_i],
                            local_queues_starts,
                            local_queues_sizes,
                            local_m_counts,
                            top_m_candidates_list,
                            is_visited);
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
//                    double time_memmove = 0;
//                    for (int i = 0; i < num_threads; ++i) {
//                        time_memmove += engine.time_memmove_list_[i];
//                        engine.time_memmove_list_[i] = 0;
//                    }
                    printf(
                           "num_threads: %d "
                           "M: %u "
                           "global_L: %u "
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
//                           "local_queue_length: %u "
//                           "M_middle: %u "
                           "local_L: %u "
                           "select(s.): %f\n",
//                           "init(s.): %f "
//                           "seq(s.): %f "
//                           "par(s.): %f "
//                           "end(s.): %f "
//                           "assign(s.): %f "
//                           "expand(s.): %f "
//                           "pick(s.): %f "
//                           "add_to_queue: %lu "
//                           "add_to_queue(s.): %f "
//                           "dist_compt(s.): %f "
//                           "select(s.): %f\n",
//                           "merge(s.): %f(%.2f%%) "
//                           "select_L(s.): %f(%.2f%%) "
//                           "select_M(s.): %f(%.2f%%)\n",
//                           "memmove_time(s.): %f\n",
//                           "num_local_elements: %lu\n",
                           num_threads,
                           value_M,
                           global_L,
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
//                           local_queue_length,
//                           M_middle,
                           local_L,
//                           engine.time_initialization_,
//                           engine.time_sequential_phase_,
//                           engine.time_parallel_phase_,
//                           engine.time_ending_,
//                           engine.time_assign_s_,
//                           engine.time_expand_,
//                           engine.time_pick_top_m_,
//                           engine.count_add_to_queue_,
//                           engine.time_add_to_queue_,
//                           engine.time_distance_computation_,
                           engine.time_select_);
//                           engine.time_merge_, engine.time_merge_ / diff.count() * 100,
//                           engine.time_select_L_, engine.time_select_L_ / diff.count() * 100,
//                           engine.time_select_M_, engine.time_select_M_ / diff.count() * 100);
//                           time_memmove);
//                           engine.number_local_elements_);
                    engine.count_distance_computation_ = 0;
//                    engine.time_initialization_ = 0;
//                    engine.time_sequential_phase_ = 0;
//                    engine.time_parallel_phase_ = 0;
//                    engine.time_ending_ = 0;
//                    engine.time_assign_s_ = 0;
//                    engine.time_expand_ = 0;
//                    engine.time_pick_top_m_ = 0;
//                    engine.count_add_to_queue_ = 0;
//                    engine.time_add_to_queue_ = 0;
//                    engine.time_distance_computation_ = 0;
                    engine.time_select_ = 0;
//                    engine.time_merge_ = 0.0;
//                    engine.time_select_L_ = 0.0;
//                    engine.time_select_M_ = 0.0;
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
//            if (local_queue_length == 1) {
//                local_queue_length = 0;
//            }
//    }

    return 0;
}
