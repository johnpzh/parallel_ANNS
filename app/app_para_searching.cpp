//
// Created by Zhen Peng on 11/13/19.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include <omp.h>
//#include "../core/Searching.h"
//#include "../core/Searching.202002101535.reorganization.h"
//#include "../core/Searching.202002181409.local_queue_and_merge.h"
//#include "../core/Searching.202002250815.buckets_equal_width.h"
#include "../core/Searching.202003021000.profile_para_top_m_search.h"
//#include "../include/utils.h"
//#include "../include/efanna2e/index_nsg.h"

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <true_NN_file> <num_threads>\n", argv[0]);
//            "Usage: %s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <query_num_max>\n", argv[0]);
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

    if (L < K) {
        std::cout << "search_L cannot be smaller than search_K!" << std::endl;
        exit(-1);
    }
    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

//    int num_threads_max = 1;
//    for (int num_threads = 1; num_threads < num_threads_max + 1; num_threads *= 2) {
        int num_threads = strtoull(argv[8], nullptr, 0);
        omp_set_num_threads(num_threads);
        int warmup_max = 3;
        for (int warmup = 0; warmup < warmup_max; ++warmup) {
            std::vector< std::vector<PANNS::idi> > set_K_list(query_num);
            for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

            std::vector<PANNS::idi> init_ids(L);
//            std::vector<PANNS::Candidate> set_L(L + 1); // Return set
            std::vector< std::vector<PANNS::Candidate> > set_L_list(query_num, std::vector<PANNS::Candidate>(L + 1));

            auto s = std::chrono::high_resolution_clock::now();

            engine.prepare_init_ids(init_ids, L);
#pragma omp parallel for
            for (unsigned q_i = 0; q_i < query_num; ++q_i) {
                engine.search_in_sequential(
                        q_i,
                        K,
                        L,
                        set_L_list[q_i],
//                        set_L,
                        init_ids,
                        set_K_list[q_i]);
            }
            auto e = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = e - s;

            std::unordered_map<unsigned, double> recalls;
            {// Recall values
                std::vector< std::vector<PANNS::idi> > true_nn_list;
                engine.load_true_NN(
                        argv[7],
                        true_nn_list);
                engine.get_recall_for_all_queries(
                        true_nn_list,
                        set_K_list,
                        recalls);
            }
            {// Basic output
                printf("num_threads: %d "
                       "L: %u "
                       "search_time(s.): %f "
                       //                       "time_distance_computation: %f "
                       "count_distance_computation: %'lu "
                       "K: %u "
                       "Volume: %u "
                       "Dimension: %u "
                       "query_num: %u "
                       "query_per_sec: %f "
                       "average_latency(ms.): %f "
                       "P@100: %f\n",
                       num_threads,
                       L,
                       diff.count(),
//                       index.time_distance_computation,
//                           index.count_distance_computation,
                       engine.count_distance_computation_,
                       K,
                       points_num,
                       data_dimension,
                       query_num,
                       query_num / diff.count(),
                       diff.count() * 1000 / query_num,
                       recalls[100]);
//                index.time_distance_computation = 0.0;
//                    index.count_distance_computation = 0;
                engine.count_distance_computation_ = 0;
            }
            PANNS::DiskIO::save_result(argv[6], set_K_list);
        }
//    }

    return 0;
}
