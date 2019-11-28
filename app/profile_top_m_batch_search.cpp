//
// Created by Zhen Peng on 11/23/19.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include "../core/Searching.h"
//#include "../include/utils.h"
//#include "../include/efanna2e/index_nsg.h"

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <value_M_max> <batch_size_max>\n",
//            "Usage: %s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <query_num_max> <true_NN_file> <value_M_max> <num_measure_queries>\n",
            argv[0]);
}

//void get_percentage_of_sharing_in_top_m(
//        const std::vector< std::vector<PANNS::idi> > &top_m_list_a,
//        const std::vector< std::vector<PANNS::idi> > &top_m_list_b,
//        double &pcnt_has_shared_iterations,
//        double &avg_pcnt_shared_top_m)
//{
//    avg_pcnt_shared_top_m = 0.0;
//    size_t iter_a = top_m_list_a.size();
//    size_t iter_b = top_m_list_b.size();
//    size_t num_iters = iter_a < iter_b ? iter_a : iter_b;
//    size_t count_of_has_shared_iterations = 0;
//    for (size_t i_i = 0; i_i < num_iters; ++i_i) {
//        size_t num_tops_a = top_m_list_a[i_i].size();
//        size_t num_tops_b = top_m_list_b[i_i].size();
//        bool has_shared = false;
//        size_t count_of_shared = 0;
//        for (size_t a_t_i = 0; a_t_i < num_tops_a; ++a_t_i) {
//            PANNS::idi a_top = top_m_list_a[i_i][a_t_i];
//            for (size_t b_t_i = 0; b_t_i < num_tops_b; ++b_t_i) {
//                PANNS::idi b_top = top_m_list_b[i_i][b_t_i];
//                if (a_top == b_top) {
//                    // Shared
//                    has_shared = true;
//                    ++count_of_shared;
//                }
//            }
//        }
//        double pcnt_shared_tops = 2.0 * count_of_shared / (double) (num_tops_a + num_tops_b);
////        printf("%lu %f\n", i_i, pcnt_shared_tops);
//        avg_pcnt_shared_top_m += pcnt_shared_tops;
//        if (has_shared) {
//            ++count_of_has_shared_iterations;
//        }
//    }
//    pcnt_has_shared_iterations = 2.0 * count_of_has_shared_iterations / (double) (iter_a + iter_b);
//    avg_pcnt_shared_top_m /= num_iters;
//}

int main(int argc, char **argv)
{
    if (argc != 9) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.

    PANNS::Searching engine;
    engine.load_data_load(argv[1]);
    engine.load_queries_load(argv[2]);
//    unsigned query_num_max = strtoull(argv[7], nullptr, 0); // Limit of number of queries.
//    if (engine.num_queries_ > query_num_max) {
//        engine.num_queries_ = query_num_max;
//    }
    engine.load_nsg_graph(argv[3]);

//    engine.build_opt_graph();

    unsigned L = strtoull(argv[4], nullptr, 0);
    unsigned K = strtoull(argv[5], nullptr, 0);
    unsigned M_max = strtoull(argv[7], nullptr, 0);
    unsigned batch_size_max = strtoull(argv[8], nullptr, 0);
    if (L < K) {
        fprintf(stderr, "Error: search_L %u is smaller than search_K %u\n.", L, K);
        exit(EXIT_FAILURE);
    }
    if (K < M_max) {
        fprintf(stderr, "Error: search_K %u is smaller than value_M %u.\n", K, M_max);
        exit(EXIT_FAILURE);
    }

//    std::vector< std::vector<PANNS::idi> > true_nn_list;
//    engine.load_true_NN(
//            argv[8],
//            true_nn_list);

    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

    int num_threads_max = 1;
    for (int num_threads = 1; num_threads < num_threads_max + 1; num_threads *= 2) {
//        omp_set_num_threads(num_threads);
//        int warmup_max = 1;

//        for (unsigned value_M = 128; value_M <= M_max; value_M *= 2) {
//            for (unsigned query_batch_size = 128; query_batch_size <= batch_size_max; query_batch_size *= 2) {
//                unsigned query_batch_size = 16;
                unsigned value_M = M_max;
                unsigned query_batch_size = batch_size_max;
                unsigned warmup_max = 1;
                for (unsigned warmup = 0; warmup < warmup_max; ++warmup) {
                    std::vector< std::vector<PANNS::idi> > set_K_list(query_num, std::vector<PANNS::idi>(K));
//                    for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

                    std::vector<PANNS::idi> init_ids(L);
//                std::vector<PANNS::Candidate> set_L(L + 1); // Return set
                    std::vector<std::vector<PANNS::Candidate> > set_L_list(query_batch_size,
                                                                           std::vector<PANNS::Candidate>(L + 1));
//                std::vector<std::vector<std::vector<PANNS::idi> > > queries_top_m_list(query_num);

                    unsigned remain = query_num % query_batch_size;
                    unsigned q_i_bound = query_num - remain;
                    auto s = std::chrono::high_resolution_clock::now();
                    engine.prepare_init_ids(init_ids, L);
//#pragma omp parallel for
                    for (unsigned q_i = 0; q_i < q_i_bound; q_i += query_batch_size) {
//                        printf("q_i: %u\n", q_i);//test
                        engine.search_with_top_m_in_batch(
                                value_M,
                                q_i,
                                query_batch_size,
                                K,
                                L,
                                set_L_list,
                                init_ids,
                                set_K_list);
                    }
                    if (remain) {
//                        printf("q_i_bound: %u\n", q_i_bound);//test
                        engine.search_with_top_m_in_batch(
                                value_M,
                                q_i_bound,
                                remain,
                                K,
                                L,
                                set_L_list,
                                init_ids,
                                set_K_list);
                    }
                    auto e = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> diff = e - s;
                    {// Basic output
                        printf("M: %u "
                               "batch_size: %u "
                               "L: %u "
                               "search_time(s.): %f "
                               "K: %u "
                               "Volume: %u "
                               "Dimension: %u "
                               "query_num: %u "
                               "query_per_sec: %f "
                               "average_latency(ms.): %f\n",
                               value_M,
                               query_batch_size,
                               L,
                               diff.count(),
                               K,
                               points_num,
                               data_dimension,
                               query_num,
                               query_num / diff.count(),
                               diff.count() * 1000 / query_num);
                    }
//                { // Recall values
//                    std::unordered_map<unsigned, double> recalls;
//
//                    engine.get_recall_for_all_queries(
//                            true_nn_list,
//                            set_K_list,
//                            recalls);
////                printf("P@5: %f "
////                       "P@10: %f "
////                       "P@20: %f "
////                       "P@50: %f "
////                       "P@100: %f\n",
////                       recalls[5],
////                       recalls[10],
////                       recalls[20],
////                       recalls[50],
////                       recalls[100]);
//
//                    printf("M: %u "
//                           "searching_time(s.): %f "
//                           "P@100: %f\n",
//                           value_M,
//                           diff.count(),
//                           recalls[100]);
//                }
//                { // Percentage of Sharing
//                    unsigned num_measure_quries = strtoull(argv[10], nullptr, 0);
//                    for (unsigned q_i = 0; q_i < num_measure_quries; q_i += 2) {
//                        double pcnt_has_shared_iterations;
//                        double avg_pcnt_shared_top_m;
//                        get_percentage_of_sharing_in_top_m(
//                                queries_top_m_list[q_i],
//                                queries_top_m_list[q_i + 1],
//                                pcnt_has_shared_iterations,
//                                avg_pcnt_shared_top_m);
//                        printf("%u-%u pcnt_has_shared_iterations: %f avg_pcnt_shared_top_m: %f\n",
//                               q_i, q_i + 1, pcnt_has_shared_iterations, avg_pcnt_shared_top_m);
//                    }
//                }
                    PANNS::save_result(argv[6], set_K_list);
                }
            }
//        }
//    }

    return 0;
}
