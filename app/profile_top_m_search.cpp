//
// Created by Zhen Peng on 11/18/19.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include <clocale>
//#include "../core/Searching.202002101535.reorganization.h"
//#include "../core/Searching.202002141745.critical_omp_top_m.h"
#include "../core/Searching.202002181409.local_queue_and_merge.h"
//#include "../core/Searching.201912161559.set_for_queue.h"
//#include "../core/Searching.201912091448.map_for_queries_ids.h"
//#include "../core/Searching.h"
//#include "../include/utils.h"
//#include "../include/efanna2e/index_nsg.h"

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <value_M_max> <true_NN_file>\n",
//            "Usage: %s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <query_num_max> <true_NN_file> <value_M_max> <num_measure_queries>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 9) {
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
    if (K < M_max) {
//        fprintf(stderr, "Error: search_K %u is smaller than value_M %u.\n", K, M_max);
//        exit(EXIT_FAILURE);
        fprintf(stderr, "Warning: search_K %u is smaller than value_M %u.\n", K, M_max);
    }


    std::vector< std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[8],
            true_nn_list);

    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

    int num_threads_max = 1;
    for (int num_threads = 1; num_threads < num_threads_max + 1; num_threads *= 2) {
//        omp_set_num_threads(num_threads);
//        int warmup_max = 1;

//        for (unsigned value_M = 2; value_M <= M_max; value_M *= 2) {
            unsigned value_M = M_max;
            unsigned warmup_max = 1;
            for (unsigned warmup = 0; warmup < warmup_max; ++warmup) {
                std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
                for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

                std::vector<PANNS::idi> init_ids(L);
                std::vector<PANNS::Candidate> set_L(L + 1); // Return set
                std::vector<std::vector<std::vector<PANNS::idi> > > queries_top_m_list(query_num);

                auto s = std::chrono::high_resolution_clock::now();
                engine.prepare_init_ids(init_ids, L);
//#pragma omp parallel for
                for (unsigned q_i = 0; q_i < query_num; ++q_i) {
                    engine.search_with_top_m(
                            value_M,
                            q_i,
                            K,
                            L,
                            set_L,
                            init_ids,
                            set_K_list[q_i]);
//                            queries_top_m_list[q_i]);
                }
                auto e = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = e - s;
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
                { // Recall values
                    std::unordered_map<unsigned, double> recalls;

                    engine.get_recall_for_all_queries(
                            true_nn_list,
                            set_K_list,
                            recalls);
//                printf("P@5: %f "
//                       "P@10: %f "
//                       "P@20: %f "
//                       "P@50: %f "
//                       "P@100: %f\n",
//                       recalls[5],
//                       recalls[10],
//                       recalls[20],
//                       recalls[50],
//                       recalls[100]);

                    printf("M: %u "
                           "L: %u "
                           "searching_time(s.): %f "
                           "P@100: %f\n",
//                           "count_distance_computation: %'lu\n",
                           value_M,
                           L,
                           diff.count(),
                           recalls[100]);
//                           engine.count_distance_computation);
//                    engine.count_distance_computation = 0;
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
    }

    return 0;
}
