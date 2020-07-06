//
// Created by Zhen Peng on 4/13/2020.
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
#include "../core/Searching.202005271122.choosing_m.h"
//#include "../core/Searching.202006191549.nested_parallel.h"

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <value_M_max> <true_NN_file> <num_threads> <value_M_middle>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 11) {
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

    unsigned M_middle = strtoull(argv[10], nullptr, 0);
//        int warmup_max = 1;

//    const PANNS::idi local_queue_length = L;
//        for (unsigned value_M = 2; value_M <= M_max; value_M *= 2) {
//        for (unsigned local_queue_length = 0; local_queue_length <= 3 * L; local_queue_length += L/10) {
//            unsigned local_queue_length = 1;
            unsigned local_queue_length = L;
//            unsigned local_queue_length = L / num_threads;
//            unsigned local_queue_length = M_max * engine.width_;
            unsigned base_set_L = (num_threads - 1) * local_queue_length;
            if (!local_queue_length) {
                local_queue_length = 1;
            }
            unsigned value_M = M_max;
            unsigned warmup_max = 4;
            for (unsigned warmup = 0; warmup < warmup_max; ++warmup) {
                std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
                for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

                std::vector<PANNS::idi> init_ids(L);
                std::vector<PANNS::Candidate> set_L(L + (num_threads - 1) * local_queue_length); // Return set
//                std::vector<PANNS::Candidate> set_L(L + 1); // Return set
//                std::vector<std::vector<std::vector<PANNS::idi> > > queries_top_m_list(query_num);
//                const PANNS::idi local_queue_length = L;
//                std::vector< std::vector<PANNS::Candidate> > local_queues_list(num_threads, std::vector<PANNS::Candidate>(local_queue_length));
//                std::vector<PANNS::Candidate> local_queues_array(num_threads * local_queue_length);
                std::vector<PANNS::idi> local_queues_ends(num_threads, 0);
//                std::vector<uint8_t> is_visited(points_num, 0);
                boost::dynamic_bitset<> is_visited(points_num);
//                PANNS::BitVector is_visited(points_num);
//                std::vector<PANNS::Candidate> top_m_candidates(value_M);
//                std::vector<PANNS::idi> top_m_candidates(L);
                std::vector<PANNS::idi> top_m_candidates(value_M);
//                std::vector<PANNS::distf> local_thresholds(num_threads - 1, -FLT_MAX);
//                std::vector<PANNS::idi> offsets_load_set_L(num_threads); // Offsets for loading from set_L.
//                for (int i_t = 0; i_t < num_threads; ++i_t) {
//                    if (0 == i_t) {
//                        offsets_load_set_L[i_t] = 0;
//                    } else {
//                        offsets_load_set_L[i_t] = L + (i_t - 1) * local_queue_length;
//                    }
//                }
//                std::vector<PANNS::idi> dest_offsets(num_threads, 0);
//                std::vector<uint8_t> is_visited(points_num, 0);
//                boost::dynamic_bitset<> is_visited(points_num);
//                PANNS::L3CacheMissRate cache_miss_rate;
//                cache_miss_rate.measure_start();
                auto s = std::chrono::high_resolution_clock::now();
//                engine.para_prepare_init_ids(init_ids, L);
                engine.prepare_init_ids(init_ids, L);
//#pragma omp parallel for
                for (unsigned q_i = 0; q_i < query_num; ++q_i) {
//                for (unsigned q_i = 499; q_i < query_num; q_i += 1000) {
//                    {//test
//                        printf("q_i: %u\n", q_i);
//                    }
//                    engine.para_search_with_top_m_merge_queues_global_threshold(
//                            M_middle,
//                            value_M,
//                            q_i,
//                            K,
//                            L,
//                            set_L,
//                            init_ids,
//                            set_K_list[q_i],
//                            local_queue_length, // Maximum size of local queue
//                            base_set_L,
//                            local_queues_ends, // Sizes of local queue
//                            top_m_candidates,
//                            is_visited);
                    engine.para_search_with_top_m_merge_queues_middle_m(
                            M_middle,
                            value_M,
                            q_i,
                            K,
                            L,
                            set_L,
                            init_ids,
                            set_K_list[q_i],
                            local_queue_length, // Maximum size of local queue
                            base_set_L,
                            local_queues_ends, // Sizes of local queue
                            top_m_candidates,
                            is_visited);
                }
//                {
//                    exit(1);
//                }
                auto e = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = e - s;
//                cache_miss_rate.measure_stop();
                {// Basic output
//                printf("M: %u "
//                        "L: %u "
//                       "search_time(s.): %f "
//                       "K: %u "
//                       "Volume: %u "
//                       "Dimension: %u "
//                       "query_num: %u "
//                       "query_per_sec: %f "
//                       "average_latency(ms.): %f\n",
//                       value_M,
//                       L,
//                       diff.count(),
//                       K,
//                       points_num,
//                       data_dimension,
//                       query_num,
//                       query_num / diff.count(),
//                       diff.count() * 1000 / query_num);
                }
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
                           "local_queue_length: %u "
                           "M_middle: %u "
                           "merge_time(s.): %f\n",
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
                           local_queue_length,
                           M_middle,
                           engine.time_merge_);
//                           engine.number_local_elements_);
                    engine.count_distance_computation_ = 0;
                    engine.time_merge_ = 0;
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
            if (local_queue_length == 1) {
                local_queue_length = 0;
            }
//        }
//        }
//    }

    return 0;
}
