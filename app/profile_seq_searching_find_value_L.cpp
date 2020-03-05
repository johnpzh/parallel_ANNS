//
// Created by Zhen Peng on 11/13/19.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
//#include "../core/Searching.h"
//#include "../core/Searching.202002101535.reorganization.h"
//#include "../core/Searching.202002181409.local_queue_and_merge.h"
#include "../core/Searching.202002250815.buckets_equal_width.h"
//#include "../include/utils.h"
//#include "../include/efanna2e/index_nsg.h"


double do_searching(
        PANNS::Searching &engine,
        const unsigned L,
        const unsigned K,
        const std::vector<std::vector<PANNS::idi> > &true_nn_list,
        const char *output_filename)
{
    const unsigned query_num = engine.num_queries_;
    std::vector< std::vector<PANNS::idi> > set_K_list(query_num);
    for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

    std::vector<PANNS::idi> init_ids(L);
    std::vector<PANNS::Candidate> set_L(L + 1); // Return set

//            PANNS::L3CacheMissRate cache_miss;
    auto s = std::chrono::high_resolution_clock::now();

    engine.prepare_init_ids(init_ids, L);
//#pragma omp parallel for
    for (unsigned q_i = 0; q_i < query_num; ++q_i) {
        engine.search_in_sequential(
                q_i,
                K,
                L,
                set_L,
                init_ids,
                set_K_list[q_i]);
    }
    auto e = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = e - s;

    std::unordered_map<unsigned, double> recalls;
    {// Recall values
        engine.get_recall_for_all_queries(
                true_nn_list,
                set_K_list,
                recalls);
    }
    {// Basic output
        printf("L: %u "
               "search_time(s.): %f "
               //                       "time_distance_computation: %f "
               //                           "count_distance_computation: %lu "
               "K: %u "
               "Volume: %u "
               "Dimension: %u "
               "query_num: %u "
               "query_per_sec: %f "
               "average_latency(ms.): %f "
               "P@100: %f\n",
               L,
               diff.count(),
//                       index.time_distance_computation,
//                           index.count_distance_computation,
               K,
               engine.num_v_,
               engine.dimension_,
               query_num,
               query_num / diff.count(),
               diff.count() * 1000 / query_num,
               recalls[100]);
//                index.time_distance_computation = 0.0;
//                    index.count_distance_computation = 0;
    }
    PANNS::DiskIO::save_result(output_filename, set_K_list);

    return recalls[100];
}


void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <search_L_min> <search_L_max> <search_K> <result_path> <true_NN_file> <recall_expected>\n", argv[0]);
//            "Usage: %s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <query_num_max>\n", argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 10) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.

    PANNS::Searching engine;
    engine.load_data_load(argv[1]);
    engine.load_queries_load(argv[2]);
    engine.load_nsg_graph(argv[3]);

//    unsigned L = strtoull(argv[4], nullptr, 0);
    unsigned L_min = strtoull(argv[4], nullptr, 0);
    unsigned L_max = strtoull(argv[5], nullptr, 0);
    unsigned K = strtoull(argv[6], nullptr, 0);
    double recall_expected = strtod(argv[9], nullptr);

    if (L_min < K || L_max < K) {
        std::cout << "search_L cannot be smaller than search_K!" << std::endl;
        exit(-1);
    }
//    unsigned data_dimension = engine.dimension_;
//    unsigned points_num = engine.num_v_;
//    unsigned query_num = engine.num_queries_;

    std::vector< std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[8],
            true_nn_list);

    {
        double min_above_recall = 1.0;
        unsigned min_above_L = L_max;
        bool is_found = false;
        unsigned mid = 0;
        double recall_output = 0;
        unsigned left = L_min;
        unsigned right = L_max + 1;
        while (left < right && !is_found) {
            mid = (right - left) / 2 + left;
            recall_output = do_searching(
                    engine,
                    mid,
                    K,
                    true_nn_list,
                    argv[7]);
            if (recall_output - recall_expected < -0.000001) {
                left = mid + 1;
            } else if (recall_output - recall_expected > 0.000001) {
                right = mid;
                if (recall_output < min_above_recall) {
                    min_above_recall = recall_output;
                    min_above_L = mid;
                }
            } else {
                is_found = true;
            }
        }
        if (is_found) {
            printf("Found: "
                   "L_output: %u "
                   "recall_output: %f "
                   "recall_expected: %f\n",
                   mid,
                   recall_output,
                   recall_expected);
        } else {
            printf("Not found: "
                   "L_output: %u "
                   "recall_output: %f "
                   "recall_expected: %f\n",
                   min_above_L,
                   min_above_recall,
                   recall_expected);
        }
    }

////        omp_set_num_threads(num_threads);
//        int warmup_max = 1;
//        for (int warmup = 0; warmup < warmup_max; ++warmup) {
//            std::vector< std::vector<PANNS::idi> > set_K_list(query_num);
//            for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);
//
//            std::vector<PANNS::idi> init_ids(L);
//            std::vector<PANNS::Candidate> set_L(L + 1); // Return set
//
////            PANNS::L3CacheMissRate cache_miss;
//            auto s = std::chrono::high_resolution_clock::now();
//
//            engine.prepare_init_ids(init_ids, L);
////#pragma omp parallel for
//            for (unsigned q_i = 0; q_i < query_num; ++q_i) {
//                engine.search_in_sequential(
//                        q_i,
//                        K,
//                        L,
//                        set_L,
//                        init_ids,
//                        set_K_list[q_i]);
//            }
//            auto e = std::chrono::high_resolution_clock::now();
//            std::chrono::duration<double> diff = e - s;
//
//            std::unordered_map<unsigned, double> recalls;
//            {// Recall values
//                std::vector< std::vector<PANNS::idi> > true_nn_list;
//                engine.load_true_NN(
//                        argv[7],
//                        true_nn_list);
//                engine.get_recall_for_all_queries(
//                        true_nn_list,
//                        set_K_list,
//                        recalls);
//            }
//            {// Basic output
//                printf("L: %u "
//                       "search_time(s.): %f "
//                       //                       "time_distance_computation: %f "
//                       //                           "count_distance_computation: %lu "
//                       "K: %u "
//                       "Volume: %u "
//                       "Dimension: %u "
//                       "query_num: %u "
//                       "query_per_sec: %f "
//                       "average_latency(ms.): %f "
//                       "P@100: %f\n",
//                       L,
//                       diff.count(),
////                       index.time_distance_computation,
////                           index.count_distance_computation,
//                       K,
//                       points_num,
//                       data_dimension,
//                       query_num,
//                       query_num / diff.count(),
//                       diff.count() * 1000 / query_num,
//                       recalls[100]);
////                index.time_distance_computation = 0.0;
////                    index.count_distance_computation = 0;
//            }
//            PANNS::DiskIO::save_result(argv[6], set_K_list);
//        }

    return 0;
}
