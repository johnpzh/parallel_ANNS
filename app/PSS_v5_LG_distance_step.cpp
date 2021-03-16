//
// Created by Zhen Peng on 03/14/2020.
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
#include "../core/Searching.202103141957.PSS_v5.large_graph.distance_step.h"



void usage(int argc, char *argv[])
{
    if (argc != 14) {
        fprintf(stderr,
                "Usage: %s <data_file> <query_file> <nsg_path> "
                "<K> <result_file> <true_NN_file> <num_threads> "
                "<L_master_low> <L_master_up> <L_master_step> <X_low> "
                "<X_up> <X_step>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    usage(argc, argv);
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

    PANNS::Searching engine;
    engine.load_data_load(argv[1]);
    engine.load_queries_load(argv[2]);
//    const char *file_reorder_map = argv[17];
//    const unsigned opt_id_threshold = strtoull(argv[18], nullptr, 0);
//    engine.load_reorder_map(file_reorder_map);
//    engine.threshold_opt_id_ = std::min(opt_id_threshold, engine.num_v_);
//    engine.reorder_load_data();
//    engine.load_and_reorder_nsg_graph(argv[3]);
    engine.load_common_nsg_graph(argv[3]);

    const unsigned K = strtoull(argv[4], nullptr, 0);
    const char *path_results = argv[5];
    std::vector< std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[6],
            true_nn_list);
//    {
//        std::vector< std::vector<PANNS::idi> > old_true_nn_list;
//        engine.load_true_NN(
//                argv[6],
//                old_true_nn_list);
//        engine.reorder_true_NN(
//                old_true_nn_list,
//                true_nn_list);
//    }

    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

    int num_threads = strtoull(argv[7], nullptr, 0);
    engine.num_threads_ = num_threads;
    omp_set_num_threads(num_threads);
//    omp_set_nested(1);
//    omp_set_max_active_levels(2);
    const unsigned L_master_low = strtoull(argv[8], nullptr, 0);
    const unsigned L_master_up = strtoull(argv[9], nullptr, 0);
    const unsigned L_master_step = strtoull(argv[10], nullptr, 0);
//    const unsigned L_local_low = strtoull(argv[11], nullptr, 0);
//    const unsigned L_local_up = strtoull(argv[12], nullptr, 0);
//    const unsigned L_local_step = strtoull(argv[13], nullptr, 0);
    const unsigned X_low = strtoull(argv[11], nullptr, 0);
    const unsigned X_up = strtoull(argv[12], nullptr, 0);
    const unsigned X_step = strtoull(argv[13], nullptr, 0);

//    const unsigned I_thresh_low = strtoull(argv[17], nullptr, 0);
//    const unsigned I_thresh_up = strtoull(argv[18], nullptr, 0);
//    const unsigned I_thresh_step = strtoull(argv[19], nullptr, 0);

    // Read reorder map
    {
        engine.prf_step_dist_parent_list_.resize(num_threads);
        engine.cnt_step_parent_list_.resize(num_threads);
        engine.prf_step_dist_query_list_.resize(num_threads);
        engine.cnt_step_query_list_.resize(num_threads);
    }


    for (unsigned L_master = L_master_low; L_master <= L_master_up; L_master += L_master_step) {
        unsigned L_local = L_master;
//        unsigned Index_thresh = L_local - 1;
//        for (unsigned L_local = L_local_low; L_local <= L_local_up; L_local += L_local_step) {

        for (unsigned subsearch_iterations = X_low; subsearch_iterations <= X_up; subsearch_iterations += X_step) {

//                for (unsigned Index_thresh = I_thresh_low; Index_thresh <= I_thresh_up; Index_thresh += I_thresh_step) {
//            engine.index_thresh_ = Index_thresh;
            unsigned warmup_max = 1;
            for (unsigned warmup = 0; warmup < warmup_max; ++warmup) {
                std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
                for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

                std::vector<PANNS::idi> init_ids(L_master);
//                std::vector<uint8_t> is_visited(points_num, 0);
                boost::dynamic_bitset<> is_visited(points_num);
                std::vector<PANNS::Candidate> set_L((num_threads - 1) * L_local + L_master);
                std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
                std::vector<PANNS::idi> local_queues_starts(num_threads);
                for (int q_i = 0; q_i < num_threads; ++q_i) {
                    local_queues_starts[q_i] = q_i * L_local;
                }
//                std::vector<PANNS::idi> top_m_candidates(num_threads);
                auto s = std::chrono::high_resolution_clock::now();
//                engine.prepare_init_ids(init_ids, L_local);
                engine.prepare_init_ids(init_ids, L_master);
                for (unsigned q_i = 0; q_i < query_num; ++q_i) {
//                    {//test
//                        printf("q_i: %u\n", q_i);
//                    }
                    engine.para_search_PSS_v5_large_graph_distance_step(
                            q_i,
                            K,
                            L_master,
                            set_L,
                            init_ids,
                            set_K_list[q_i],
                            L_local,
                            local_queues_starts,
                            local_queues_sizes,
                            is_visited,
                            subsearch_iterations);
                }
                auto e = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = e - s;
                std::unordered_map<unsigned, double> recalls;
                { // Recall values
                    engine.get_recall_for_all_queries(
                            true_nn_list,
                            set_K_list,
                            recalls,
                            L_master);
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
                            "sub_iters: %u "
//                            "index_thresh: %u "
                            "avg_merge: %f "
                            "t_expand(s.): %f "
                            "t_merge(s.): %f "
                            "t_seq(s.): %f "
                            "t_p_expand(%%): %f "
                            "t_p_merge(%%): %f "
                            "t_p_seq(%%): %f "
                            "opt_id_thrd: %u ",
                            num_threads,
                            L_master,
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
                            data_dimension * 4.0 * engine.count_distance_computation_ / (1U << 30U) /
                            diff.count(),
                            data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ /
                            (1U << 30U) /
                            diff.count(),
                            L_local,
                            subsearch_iterations,
//                            engine.index_thresh_,
                            engine.count_merge_ * 1.0 / query_num,
                            engine.time_expand_,
                            engine.time_merge_,
                            engine.time_seq_,
//                            engine.time_pick_,
                            engine.time_expand_ / diff.count() * 100.0,
                            engine.time_merge_ / diff.count() * 100.0,
                            engine.time_seq_ / diff.count() * 100.0,
                            engine.threshold_opt_id_);
//                            engine.time_pick_ / diff.count() * 100.0);
                    printf("\n");
                }
                engine.count_distance_computation_ = 0;
//                engine.count_iterations_ = 0;
//                    engine.count_checked_ = 0;
                engine.count_merge_ = 0;
                engine.time_expand_ = 0.0;
                engine.time_merge_ = 0.0;
                engine.time_seq_ = 0.0;
//                engine.time_pick_ = 0.0;
                {
                    printf("==== Dist query\n");
                    printf("Step");
                    for (int w_i = 0; w_i < num_threads; ++w_i) {
                        printf("\tT%d", w_i);
                    }
                    printf("\n");
                    for (int w_i = 0; w_i < num_threads; ++w_i) {
                        auto &cnt_step_query = engine.cnt_step_query_list_[w_i];
                        auto &prf_dist_query = engine.prf_step_dist_query_list_[w_i];

                        for (unsigned s_i = 0; s_i < cnt_step_query.size(); ++s_i) {
                            if (0 == cnt_step_query[s_i]) {
                                continue;
                            }
                            printf("%u\t", s_i);
                            // Print tab
                            for (int t_i = 0; t_i < w_i; ++t_i) {
                                printf("\t");
                            }
                            printf("%f\n", prf_dist_query[s_i] / cnt_step_query[s_i]);
                        }
                    }

                    printf("==== Dist parent\n");
                    printf("Step");
                    for (int w_i = 0; w_i < num_threads; ++w_i) {
                        printf("\tT%d", w_i);
                    }
                    printf("\n");
                    for (int w_i = 0; w_i < num_threads; ++w_i) {
                        auto &cnt_step_parent = engine.cnt_step_parent_list_[w_i];
                        auto &prf_dist_parent = engine.prf_step_dist_parent_list_[w_i];

                        for (unsigned s_i = 0; s_i < cnt_step_parent.size(); ++s_i) {
                            if (0 == cnt_step_parent[s_i]) {
                                continue;
                            }
                            printf("%u\t", s_i);
                            // Print tab
                            for (int t_i = 0; t_i < w_i; ++t_i) {
                                printf("\t");
                            }
                            printf("%f\n", prf_dist_parent[s_i] / cnt_step_parent[s_i]);
                        }
                    }
                    engine.cnt_step_query_list_.clear();
                    engine.prf_step_dist_query_list_.clear();
                    engine.cnt_step_parent_list_.clear();
                    engine.prf_step_dist_parent_list_.clear();
                }
                PANNS::DiskIO::save_result(path_results, set_K_list);
            }
//                } // Index_threshold Ranged
        } // X Ranged
//        } // L_local ranged
    } // L_master ranged

    return 0;
}
