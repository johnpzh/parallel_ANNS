//
// Created by Zhen Peng on 03/10/2020.
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
#include "../core/Searching.202103101606.PSS_v5.large_graph.v_counter.h"

void save_reorder_map(
        const std::vector<uint64_t> &visit_counter,
        const char *output_file)
{
    PANNS::idi num_v = visit_counter.size();
    std::vector< std::pair<uint64_t, PANNS::idi> > pairs_count_id(num_v);
    std::vector<PANNS::idi> old_to_new(num_v);
    for (PANNS::idi v_i = 0; v_i < num_v; ++v_i) {
        pairs_count_id[v_i].first = visit_counter[v_i];
        pairs_count_id[v_i].second = v_i;
    }

    // Sort according to count of visiting
    std::sort(pairs_count_id.rbegin(), pairs_count_id.rend());
    
    for (PANNS::idi new_i = 0; new_i < num_v; ++new_i) {
        PANNS::idi old_i = pairs_count_id[new_i].second;
        old_to_new[old_i] = new_i;

//        // test
//        if (pairs_count_id[new_i].first > 4) {
//            printf("%u %u %lu\n", old_i, new_i, pairs_count_id[new_i].first);
//        }
    }

    // Save to file
    std::ofstream fout(output_file);
    if (!fout.is_open()) {
        fprintf(stderr, "Error: cannot create file %s .\n", output_file);
        exit(EXIT_FAILURE);
    }
    for (PANNS::idi old_i = 0; old_i < num_v; ++old_i) {
        PANNS::idi new_i = old_to_new[old_i];
        fout.write(reinterpret_cast<char *>(&old_i), sizeof(old_i));
        fout.write(reinterpret_cast<char *>(&new_i), sizeof(new_i));
    }
}

void usage(int argc, char *argv[])
{
    if (argc != 18) {
        fprintf(stderr,
                "Usage: %s <data_file> <query_file> <nsg_path> "
                "<K> <result_file> <true_NN_file> <num_threads> "
                "<L_master_low> <L_master_up> <L_master_step> <L_local_low> "
                "<L_local_up> <L_local_step> <X_low> <X_up> "
                "<X_step> <output_reorder_map>\n",
//                "<X_step> <I_thresh_low> <I_thresh_up> <I_thresh_step>\n",
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
    engine.load_common_nsg_graph(argv[3]);

//    engine.build_opt_graph();

//    unsigned L_min = strtoull(argv[4], nullptr, 0);
    const unsigned K = strtoull(argv[4], nullptr, 0);
//    unsigned M_max = strtoull(argv[7], nullptr, 0);
//    if (L < K) {
//        fprintf(stderr, "Error: search_L %u is smaller than search_K %u\n.", L, K);
//        exit(EXIT_FAILURE);
//    }
//    if (K < M_max) {
////        fprintf(stderr, "Error: search_K %u is smaller than value_M %u.\n", K, M_max);
////        exit(EXIT_FAILURE);
//        fprintf(stderr, "Warning: search_K %u is smaller than value_M %u.\n", K, M_max);
//    }
    const char *path_results = argv[5];
    std::vector< std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[6],
            true_nn_list);

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
    const unsigned L_local_low = strtoull(argv[11], nullptr, 0);
    const unsigned L_local_up = strtoull(argv[12], nullptr, 0);
    const unsigned L_local_step = strtoull(argv[13], nullptr, 0);
    const unsigned X_low = strtoull(argv[14], nullptr, 0);
    const unsigned X_up = strtoull(argv[15], nullptr, 0);
    const unsigned X_step = strtoull(argv[16], nullptr, 0);
    const char *file_reorder_map = argv[17];
//    const unsigned I_thresh_low = strtoull(argv[17], nullptr, 0);
//    const unsigned I_thresh_up = strtoull(argv[18], nullptr, 0);
//    const unsigned I_thresh_step = strtoull(argv[19], nullptr, 0);

    // Counter for time of visiting.
    engine.visit_counter_.resize(engine.num_v_, 0);

    for (unsigned L_master = L_master_low; L_master <= L_master_up; L_master += L_master_step) {
        unsigned L_local = L_master;
        unsigned Index_thresh = L_local - 1;
//        for (unsigned L_local = L_local_low; L_local <= L_local_up; L_local += L_local_step) {

        for (unsigned subsearch_iterations = X_low; subsearch_iterations <= X_up; subsearch_iterations += X_step) {

//                for (unsigned Index_thresh = I_thresh_low; Index_thresh <= I_thresh_up; Index_thresh += I_thresh_step) {
            engine.index_thresh_ = Index_thresh;
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
                    engine.para_search_PSS_v5_large_graph_dist_thresh_profiling(
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
                            "index_thresh: %u "
                            "avg_merge: %f "
                            "t_expand(s.): %f "
                            "t_merge(s.): %f "
                            "t_seq(s.): %f "
                            "t_p_expand(%%): %f "
                            "t_p_merge(%%): %f "
                            "t_p_seq(%%): %f ",
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
                            engine.index_thresh_,
                            engine.count_merge_ * 1.0 / query_num,
                            engine.time_expand_,
                            engine.time_merge_,
                            engine.time_seq_,
//                            engine.time_pick_,
                            engine.time_expand_ / diff.count() * 100.0,
                            engine.time_merge_ / diff.count() * 100.0,
                            engine.time_seq_ / diff.count() * 100.0);
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
                PANNS::DiskIO::save_result(path_results, set_K_list);

                {
                    save_reorder_map(
                            engine.visit_counter_,
                            file_reorder_map);
                }
            }
//                } // Index_threshold Ranged
        } // X Ranged
//        } // L_local ranged
    } // L_master ranged

    return 0;
}
