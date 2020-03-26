//
// Created by Zhen Peng on 03/19/2020.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
//#include "../core/Searching.202002101535.reorganization.h"
//#include "../core/Searching.201912161559.set_for_queue.h"
//#include "../core/Searching.201912091448.map_for_queries_ids.h"
//#include "../core/Searching.202002181409.local_queue_and_merge.h"
#include "../core/Searching.202003021000.profile_para_top_m_search.h"
//#include "../core/Searching.h"
//#include "../include/utils.h"
//#include "../include/efanna2e/index_nsg.h"

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <value_M_max> <batch_size_max> <true_NN_file> <num_threads>\n",
//            "Usage: %s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <value_M_max> <batch_size_max> <true_NN_file> <recall_expected>\n",
//            "Usage: %s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <query_num_max> <true_NN_file> <value_M_max> <num_measure_queries>\n",
            argv[0]);
}

//void do_searching(
//        const unsigned L,
//        const unsigned K,
//        PANNS::Searching &engine,
//        const unsigned value_M,
//        const unsigned query_num,
//        const unsigned query_batch_size,
//        const std::vector<std::vector<PANNS::idi> > &true_nn_list,
//        const char *output_filename,
//        double &recall,
//        uint64_t &distance_computation,
//        double &runtime)
void do_searching(char *argv[])
{
    PANNS::Searching engine;
    engine.load_data_load(argv[1]);
    engine.load_queries_load(argv[2]);
//    unsigned query_num_max = strtoull(argv[7], nullptr, 0); // Limit of number of queries.
//    if (engine.num_queries_ > query_num_max) {
//        engine.num_queries_ = query_num_max;
//    }
    engine.load_nsg_graph(argv[3]);
    std::vector<std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[9],
            true_nn_list);

//    engine.build_opt_graph();

    unsigned L = strtoull(argv[4], nullptr, 0);
    unsigned K = strtoull(argv[5], nullptr, 0);
    unsigned M = strtoull(argv[7], nullptr, 0);
    unsigned query_batch_size = strtoull(argv[8], nullptr, 0);
    int num_threads = static_cast<int>(strtoull(argv[10], nullptr, 0));
    unsigned query_num = engine.num_queries_;
    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    engine.num_threads_ = num_threads;

    unsigned warmup_max = 4;
    for (unsigned warmup = 0; warmup < warmup_max; ++warmup) {
        std::vector<PANNS::idi> init_ids(L);

        std::vector<std::vector<PANNS::idi> > set_K_list(query_num, std::vector<PANNS::idi>(K));
        std::vector<std::vector<PANNS::Candidate> > set_L_list(query_batch_size,
                                                               std::vector<PANNS::Candidate>(L + 1));
        std::vector< boost::dynamic_bitset<> > is_visited_list(query_batch_size, boost::dynamic_bitset<> (points_num));
        unsigned remain = query_num % query_batch_size;
        unsigned q_i_bound = query_num - remain;
//        unsigned num_batch =
//        std::vector< std::vector<std::vector<PANNS::idi> > > set_K_table()

        auto s = std::chrono::high_resolution_clock::now();
        engine.prepare_init_ids(init_ids, L);
//#pragma omp parallel for
//    omp_set_num_threads(omp_get_max_threads());
        for (unsigned q_i = 0; q_i < q_i_bound; q_i += query_batch_size) {
//                        printf("q_i: %u\n", q_i);//test
            engine.para_search_with_top_m_in_batch_embarassing_para(
                    M,
                    q_i,
                    query_batch_size,
                    K,
                    L,
                    set_L_list,
                    init_ids,
                    set_K_list,
                    is_visited_list);
        }
        if (remain) {
//                        printf("q_i_bound: %u\n", q_i_bound);//test
            engine.para_search_with_top_m_in_batch_embarassing_para(
                    M,
                    q_i_bound,
                    remain,
                    K,
                    L,
                    set_L_list,
                    init_ids,
                    set_K_list,
                    is_visited_list);
        }
        auto e = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = e - s;

        std::unordered_map<unsigned, double> recalls;
        { // Recall values
//            std::vector<std::vector<PANNS::idi> > true_nn_list;
//            engine.load_true_NN(
//                    argv[9],
//                    true_nn_list);

            engine.get_recall_for_all_queries(
                    true_nn_list,
                    set_K_list,
                    recalls);
//        recall = recalls[100];
//        distance_computation = engine.count_distance_computation;
//        runtime = diff.count();
//        printf("M: %u "
//               "batch_size: %u "
//               "L: %u "
//               "searching_time(s.): %f "
//               "count_distance_computation: %'lu "
//               "P@100: %f\n",
//               M,
//               query_batch_size,
//               L,
//               diff.count(),
//               engine.count_distance_computation,
//               recalls[100]);

        }
        {// Basic output
            printf("batch_size: %u "
                   "M: %u "
                   "L: %u "
                   "search_time(s.): %f "
                   "K: %u "
                   "Volume: %u "
                   "Dimension: %u "
                   "query_num: %u "
                   "query_per_sec: %f "
                   "average_latency(ms.): %f "
                   "P@100: %f\n",
                   query_batch_size,
                   M,
                   L,
                   diff.count(),
                   K,
                   points_num,
                   data_dimension,
                   query_num,
                   query_num / diff.count(),
                   diff.count() * 1000 / query_num,
                   recalls[100]);
//        engine.count_distance_computation = 0;
        }

        PANNS::DiskIO::save_result(argv[6], set_K_list);
    }
}

int main(int argc, char **argv)
{
    if (argc != 11) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

//    PANNS::Searching engine;
//    engine.load_data_load(argv[1]);
//    engine.load_queries_load(argv[2]);
////    unsigned query_num_max = strtoull(argv[7], nullptr, 0); // Limit of number of queries.
////    if (engine.num_queries_ > query_num_max) {
////        engine.num_queries_ = query_num_max;
////    }
//    engine.load_nsg_graph(argv[3]);

//    engine.build_opt_graph();

//    unsigned L_max = strtoull(argv[4], nullptr, 0);
//    unsigned K = strtoull(argv[5], nullptr, 0);
//    unsigned M_max = strtoull(argv[7], nullptr, 0);
//    unsigned batch_size_max = strtoull(argv[8], nullptr, 0);
//    if (L_max < K) {
//        fprintf(stderr, "Warning: search_L %u is smaller than search_K %u\n.", L_max, K);
////        fprintf(stderr, "Error: search_L %u is smaller than search_K %u\n.", L_max, K);
////        exit(EXIT_FAILURE);
//    }
//    if (K < M_max) {
//        fprintf(stderr, "Warning: search_K %u is smaller than value_M_max %u.\n", K, M_max);
////        fprintf(stderr, "Error: search_K %u is smaller than value_M %u.\n", K, M_max);
////        exit(EXIT_FAILURE);
//    }

//    std::vector<std::vector<PANNS::idi> > true_nn_list;
//    engine.load_true_NN(
//            argv[9],
//            true_nn_list);

//    unsigned data_dimension = engine.dimension_;
//    unsigned points_num = engine.num_v_;
//    unsigned query_num = engine.num_queries_;

    {
//        double recall_output = 0.0;
//        uint64_t distance_computation_output = 0;
//        double runtime = 0.0;

        do_searching(argv);
//        do_searching(
//                L_max,
//                K,
//                engine,
//                M_max,
//                query_num,
//                batch_size_max,
//                true_nn_list,
//                argv[6],
//                recall_output,
//                distance_computation_output,
//                runtime);
    }

//    {// For different value M, find the value L to achieve the same accuracy.
////    unsigned value_M = M_max;
//        for (unsigned value_M = 1; value_M <= M_max; value_M *= 2) {
//            unsigned query_batch_size = batch_size_max;
//
//            unsigned left = 1;
//            unsigned right = L_max;
//            unsigned mid = 0;
//            double recall_expected = strtod(argv[10], nullptr);
//            double recall_output = 0;
//            uint64_t distance_computation_output = 0;
//            double runtime = 0.0;
//            double min_above_recall = 1;
//            unsigned min_above_L = L_max;
//            uint64_t min_above_distance_computation = 0;
//            double min_above_runtime = 0.0;
//            bool is_found = false;
//            while (left <= right) {
//                mid = (right - left) / 2 + left;
//                do_searching(
//                        mid,
//                        K,
//                        engine,
//                        value_M,
//                        query_num,
//                        query_batch_size,
//                        true_nn_list,
//                        recall_output,
//                        distance_computation_output,
//                        runtime);
////            {//test
////                break;
////            }
//                if (recall_output - recall_expected < -0.000001) {
//                    left = mid + 1;
//                } else if (recall_output - recall_expected > 0.000001) {
//                    right = mid - 1;
//                    if (recall_output < min_above_recall) {
//                        min_above_recall = recall_output;
//                        min_above_L = mid;
//                        min_above_distance_computation = distance_computation_output;
//                        min_above_runtime = runtime;
//                    }
//                } else {
//                    is_found = true;
//                    break;
//                }
//            }
//            if (is_found) {
//                printf("Found: M: %u "
//                       "L_output: %u "
//                       "distance_computation: %'lu "
//                       "searching_time(s.): %f "
//                       "recall_output: %f "
//                       "recall_expected: %f\n",
//                       value_M,
//                       mid,
//                       distance_computation_output,
//                       runtime,
//                       recall_output,
//                       recall_expected);
//            } else {
//                printf("Not found: M: %u "
//                       "L_output: %u "
//                       "distance_computation: %'lu "
//                       "searching_time(s.): %f "
//                       "recall_output: %f "
//                       "recall_expected: %f\n",
//                       value_M,
//                       min_above_L,
//                       min_above_distance_computation,
//                       min_above_runtime,
//                       min_above_recall,
//                       recall_expected);
////            printf("Not found: M: %u L_output: %u recall_output: %f recall_expected: %f\n",
////                   value_M, mid, recall_output, recall_expected);
//            }
//        }
//    }

    return 0;
}
