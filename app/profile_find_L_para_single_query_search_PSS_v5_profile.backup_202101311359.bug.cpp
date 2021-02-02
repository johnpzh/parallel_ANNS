//
// Created by Zhen Peng on 01/30/2021.
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
#include "../core/Searching.202101301609.PSS_v5.profiling.h"

void search_one_time(
        PANNS::Searching &engine,
        const unsigned L_master,
        const unsigned K,
        const unsigned points_num,
        const unsigned query_num,
        const unsigned data_dimension,
        const int num_threads,
        const unsigned L_local,
        const unsigned subsearch_iterations,
        const std::vector< std::vector<PANNS::idi> > &true_nn_list,
        std::vector<std::vector<unsigned> > &set_K_list_return,
        std::unordered_map<unsigned, double> &recalls,
        double &runtime,
        uint64_t &compt,
        double &avg_merge,
        std::vector<double> &time_break)
{
    std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
    for (unsigned i = 0; i < query_num; i++) {
        set_K_list[i].resize(K);
    }
    std::vector<PANNS::idi> init_ids(L_master);
//                std::vector<uint8_t> is_visited(points_num, 0);
    boost::dynamic_bitset<> is_visited(points_num);
    std::vector<PANNS::Candidate> set_L((num_threads - 1) * L_local + L_master);
    std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
    std::vector<PANNS::idi> local_queues_starts(num_threads);
    for (int q_i = 0; q_i < num_threads; ++q_i) {
        local_queues_starts[q_i] = q_i * L_local;
    }
    auto s = std::chrono::high_resolution_clock::now();
    engine.prepare_init_ids(init_ids, L_master);
//#pragma omp parallel for
    for (unsigned q_i = 0; q_i < query_num; ++q_i) {

        engine.para_search_PSS_v5_profiling(
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
    { // Recall values
        engine.get_recall_for_all_queries(
                true_nn_list,
                set_K_list,
                recalls,
                L_master);
    }
    {// Basic output
        printf(
//                            "local_queue_length: %u "
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
                "avg_merge: %f "
                "t_expand(s.): %f "
                "t_merge(s.): %f "
                "t_seq(s.): %f "
                "t_pick(s.): %f ",
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
                data_dimension * 4.0 * engine.count_distance_computation_ / (1U << 30U) / diff.count(),
                data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ / (1U << 30U) /
                diff.count(),
                L_local,
                subsearch_iterations,
                engine.count_merge_ * 1.0 / query_num,
                engine.time_expand_,
                engine.time_merge_,
                engine.time_seq_,
                engine.time_pick_);
        printf("\n");
    }
    runtime = diff.count();
    compt = engine.count_distance_computation_;
    engine.count_distance_computation_ = 0;
    avg_merge = engine.count_merge_ * 1.0 / query_num;
    engine.count_merge_ = 0;
    time_break[0] = engine.time_expand_;
    time_break[1] = engine.time_merge_;
    time_break[2] = engine.time_seq_;
    time_break[3] = engine.time_pick_;
    engine.time_expand_ = 0.0;
    engine.time_merge_ = 0.0;
    engine.time_seq_ = 0.0;
    engine.time_pick_ = 0.0;
    set_K_list_return.swap(set_K_list);
}

void usage(char *argv[])
{
    fprintf(stderr,
//            "Usage: %s <data_file> <query_file> <nsg_path> <L_lower> <K> <result_file> <true_NN_file> <num_threads> <sub_iters> <L_upper> <P@100> [<P@100> ...]\n",
            "Usage: %s <data_file> <query_file> <nsg_path> <K> <result_file> <true_NN_file> <num_threads> <L_master> <L_local_low> <L_local_up> <X> <P@100> [<P@100> ...]\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc < 13) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

    PANNS::Searching engine;
    engine.load_data_load(argv[1]);
    engine.load_queries_load(argv[2]);
    engine.load_nsg_graph(argv[3]);

//    unsigned L_lower_origin = strtoull(argv[4], nullptr, 0);
    const unsigned K = strtoull(argv[4], nullptr, 0);
    const char *path_results = argv[5];
    std::vector< std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[6],
            true_nn_list);

    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

    int num_threads = strtoull(argv[7], nullptr, 0);
//    if (1 != num_threads) {
//        fprintf(stderr, "Error: num_threads is %d, which should be 1.\n", num_threads);
//        exit(EXIT_FAILURE);
//    }
    engine.num_threads_ = num_threads;
    omp_set_num_threads(num_threads);
//    omp_set_nested(1);
//    omp_set_max_active_levels(2);

//    unsigned subsearch_iterations = strtoull(argv[9], nullptr, 0);
    const unsigned L_master = strtoull(argv[8], nullptr, 0);
    const unsigned L_lower_origin = strtoull(argv[9], nullptr, 0);
    const unsigned L_upper_origin = strtoull(argv[10], nullptr, 0);
    const unsigned X = strtoull(argv[11], nullptr, 0);
    const unsigned base_loc_P_dest = 12;
    unsigned num_P_target = argc - base_loc_P_dest;
    std::vector<double> P_targets(num_P_target);
    for (int a_i = base_loc_P_dest; a_i < argc; ++a_i) {
        P_targets[a_i - base_loc_P_dest] = strtod(argv[a_i], nullptr);
    }

//    {
//        unsigned L_local = L_upper_origin;
//        unsigned subsearch_iterations = X;
//        std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
//        set_K_list.resize(query_num);
//        for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);
//
//        std::vector<PANNS::idi> init_ids(L_master);
////                std::vector<uint8_t> is_visited(points_num, 0);
//        boost::dynamic_bitset<> is_visited(points_num);
//        std::vector<PANNS::Candidate> set_L((num_threads - 1) * L_local + L_master);
//        std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
//        std::vector<PANNS::idi> local_queues_starts(num_threads);
//        for (int q_i = 0; q_i < num_threads; ++q_i) {
//            local_queues_starts[q_i] = q_i * L_local;
//        }
////                std::vector<PANNS::idi> top_m_candidates(num_threads);
//        auto s = std::chrono::high_resolution_clock::now();
////                engine.prepare_init_ids(init_ids, L_local);
//        engine.prepare_init_ids(init_ids, L_master);
//        for (unsigned q_i = 0; q_i < query_num; ++q_i) {
//            engine.para_search_PSS_v5_profiling(
//                    q_i,
//                    K,
//                    L_master,
//                    set_L,
//                    init_ids,
//                    set_K_list[q_i],
//                    L_local,
//                    local_queues_starts,
//                    local_queues_sizes,
//                    is_visited,
//                    subsearch_iterations);
//        }
//        auto e = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<double> diff = e - s;
//        std::unordered_map<unsigned, double> recalls;
//        { // Recall values
//            engine.get_recall_for_all_queries(
//                    true_nn_list,
//                    set_K_list,
//                    recalls,
//                    L_master);
//        }
//        {// Basic output
//            printf(
//                    "num_threads: %d "
//                    "L: %u "
//                    "runtime(s.): %f "
//                    "computation: %lu "
//                    "K: %u "
//                    "Volume: %u "
//                    "Dimension: %u "
//                    "query_num: %u "
//                    "query_per_sec: %f "
//                    "avg_latency(ms.): %f "
//                    "P@100: %f "
//                    "P@1: %f "
//                    "G/s: %f "
//                    "GFLOPS: %f "
//                    "local_L: %u "
//                    "sub_iters: %u "
//                    "avg_merge: %f "
//                    "t_expand(s.): %f "
//                    "t_merge(s.): %f "
//                    "t_seq(s.): %f "
//                    "t_pick(s.): %f "
//                    "t_p_expand(%%): %f "
//                    "t_p_merge(%%): %f "
//                    "t_p_seq(%%): %f "
//                    "t_p_pick(%%): %f",
//                    num_threads,
//                    L_master,
//                    diff.count(),
//                    engine.count_distance_computation_,
//                    K,
//                    points_num,
//                    data_dimension,
//                    query_num,
//                    query_num / diff.count(),
//                    diff.count() * 1000 / query_num,
//                    recalls[100],
//                    recalls[1],
//                    data_dimension * 4.0 * engine.count_distance_computation_ / (1U << 30U) / diff.count(),
//                    data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ / (1U << 30U) /
//                    diff.count(),
//                    L_local,
//                    subsearch_iterations,
//                    engine.count_merge_ * 1.0 / query_num,
//                    engine.time_expand_,
//                    engine.time_merge_,
//                    engine.time_seq_,
//                    engine.time_pick_,
//                    engine.time_expand_ / diff.count() * 100.0,
//                    engine.time_merge_ / diff.count() * 100.0,
//                    engine.time_seq_ / diff.count() * 100.0,
//                    engine.time_pick_ / diff.count() * 100.0);
//            printf("\n");
//        }
//        engine.count_distance_computation_ = 0;
////                engine.count_iterations_ = 0;
//        engine.count_checked_ = 0;
//        engine.count_merge_ = 0;
//        engine.time_expand_ = 0.0;
//        engine.time_merge_ = 0.0;
//        engine.time_seq_ = 0.0;
//        engine.time_pick_ = 0.0;
//        PANNS::DiskIO::save_result(path_results, set_K_list);
//        exit(0);
//    }

    for (const double P_dest : P_targets) {
//        {
//            unsigned L_local = L_upper_origin;
//            unsigned subsearch_iterations = X;
//            std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
//            set_K_list.resize(query_num);
//            for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);
//
//            std::vector<PANNS::idi> init_ids(L_master);
////                std::vector<uint8_t> is_visited(points_num, 0);
//            boost::dynamic_bitset<> is_visited(points_num);
//            std::vector<PANNS::Candidate> set_L((num_threads - 1) * L_local + L_master);
//            std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
//            std::vector<PANNS::idi> local_queues_starts(num_threads);
//            for (int q_i = 0; q_i < num_threads; ++q_i) {
//                local_queues_starts[q_i] = q_i * L_local;
//            }
////                std::vector<PANNS::idi> top_m_candidates(num_threads);
//            auto s = std::chrono::high_resolution_clock::now();
////                engine.prepare_init_ids(init_ids, L_local);
//            engine.prepare_init_ids(init_ids, L_master);
//            for (unsigned q_i = 0; q_i < query_num; ++q_i) {
//                engine.para_search_PSS_v5_profiling(
//                        q_i,
//                        K,
//                        L_master,
//                        set_L,
//                        init_ids,
//                        set_K_list[q_i],
//                        L_local,
//                        local_queues_starts,
//                        local_queues_sizes,
//                        is_visited,
//                        subsearch_iterations);
//            }
//            auto e = std::chrono::high_resolution_clock::now();
//            std::chrono::duration<double> diff = e - s;
//            std::unordered_map<unsigned, double> recalls;
//            { // Recall values
//                engine.get_recall_for_all_queries(
//                        true_nn_list,
//                        set_K_list,
//                        recalls,
//                        L_master);
//            }
//            {// Basic output
//                printf(
//                        "num_threads: %d "
//                        "L: %u "
//                        "runtime(s.): %f "
//                        "computation: %lu "
//                        "K: %u "
//                        "Volume: %u "
//                        "Dimension: %u "
//                        "query_num: %u "
//                        "query_per_sec: %f "
//                        "avg_latency(ms.): %f "
//                        "P@100: %f "
//                        "P@1: %f "
//                        "G/s: %f "
//                        "GFLOPS: %f "
//                        "local_L: %u "
//                        "sub_iters: %u "
//                        "avg_merge: %f "
//                        "t_expand(s.): %f "
//                        "t_merge(s.): %f "
//                        "t_seq(s.): %f "
//                        "t_pick(s.): %f "
//                        "t_p_expand(%%): %f "
//                        "t_p_merge(%%): %f "
//                        "t_p_seq(%%): %f "
//                        "t_p_pick(%%): %f",
//                        num_threads,
//                        L_master,
//                        diff.count(),
//                        engine.count_distance_computation_,
//                        K,
//                        points_num,
//                        data_dimension,
//                        query_num,
//                        query_num / diff.count(),
//                        diff.count() * 1000 / query_num,
//                        recalls[100],
//                        recalls[1],
//                        data_dimension * 4.0 * engine.count_distance_computation_ / (1U << 30U) / diff.count(),
//                        data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ / (1U << 30U) /
//                        diff.count(),
//                        L_local,
//                        subsearch_iterations,
//                        engine.count_merge_ * 1.0 / query_num,
//                        engine.time_expand_,
//                        engine.time_merge_,
//                        engine.time_seq_,
//                        engine.time_pick_,
//                        engine.time_expand_ / diff.count() * 100.0,
//                        engine.time_merge_ / diff.count() * 100.0,
//                        engine.time_seq_ / diff.count() * 100.0,
//                        engine.time_pick_ / diff.count() * 100.0);
//                printf("\n");
//            }
//            engine.count_distance_computation_ = 0;
////                engine.count_iterations_ = 0;
//            engine.count_checked_ = 0;
//            engine.count_merge_ = 0;
//            engine.time_expand_ = 0.0;
//            engine.time_merge_ = 0.0;
//            engine.time_seq_ = 0.0;
//            engine.time_pick_ = 0.0;
//            PANNS::DiskIO::save_result(path_results, set_K_list);
//            exit(0);
//        }

        std::vector<std::vector<unsigned> > set_K_list;
        std::unordered_map<unsigned, double> recalls;
        unsigned L_upper = L_upper_origin;
        unsigned L_lower = L_lower_origin;
        unsigned L_local = L_upper;
        unsigned subsearch_iterations = X;
//        unsigned subsearch_iterations = L_local;
//        unsigned local_queue_capacity = L;

//        {
//            unsigned L_local = L_upper_origin;
//            unsigned subsearch_iterations = X;
//            std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
//            set_K_list.resize(query_num);
//            for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);
//
//            std::vector<PANNS::idi> init_ids(L_master);
////                std::vector<uint8_t> is_visited(points_num, 0);
//            boost::dynamic_bitset<> is_visited(points_num);
//            std::vector<PANNS::Candidate> set_L((num_threads - 1) * L_local + L_master);
//            std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
//            std::vector<PANNS::idi> local_queues_starts(num_threads);
//            for (int q_i = 0; q_i < num_threads; ++q_i) {
//                local_queues_starts[q_i] = q_i * L_local;
//            }
////                std::vector<PANNS::idi> top_m_candidates(num_threads);
//            auto s = std::chrono::high_resolution_clock::now();
////                engine.prepare_init_ids(init_ids, L_local);
//            engine.prepare_init_ids(init_ids, L_master);
//            for (unsigned q_i = 0; q_i < query_num; ++q_i) {
//                engine.para_search_PSS_v5_profiling(
//                        q_i,
//                        K,
//                        L_master,
//                        set_L,
//                        init_ids,
//                        set_K_list[q_i],
//                        L_local,
//                        local_queues_starts,
//                        local_queues_sizes,
//                        is_visited,
//                        subsearch_iterations);
//            }
//            auto e = std::chrono::high_resolution_clock::now();
//            std::chrono::duration<double> diff = e - s;
//            std::unordered_map<unsigned, double> recalls;
//            { // Recall values
//                engine.get_recall_for_all_queries(
//                        true_nn_list,
//                        set_K_list,
//                        recalls,
//                        L_master);
//            }
//            {// Basic output
//                printf(
//                        "num_threads: %d "
//                        "L: %u "
//                        "runtime(s.): %f "
//                        "computation: %lu "
//                        "K: %u "
//                        "Volume: %u "
//                        "Dimension: %u "
//                        "query_num: %u "
//                        "query_per_sec: %f "
//                        "avg_latency(ms.): %f "
//                        "P@100: %f "
//                        "P@1: %f "
//                        "G/s: %f "
//                        "GFLOPS: %f "
//                        "local_L: %u "
//                        "sub_iters: %u "
//                        "avg_merge: %f "
//                        "t_expand(s.): %f "
//                        "t_merge(s.): %f "
//                        "t_seq(s.): %f "
//                        "t_pick(s.): %f "
//                        "t_p_expand(%%): %f "
//                        "t_p_merge(%%): %f "
//                        "t_p_seq(%%): %f "
//                        "t_p_pick(%%): %f",
//                        num_threads,
//                        L_master,
//                        diff.count(),
//                        engine.count_distance_computation_,
//                        K,
//                        points_num,
//                        data_dimension,
//                        query_num,
//                        query_num / diff.count(),
//                        diff.count() * 1000 / query_num,
//                        recalls[100],
//                        recalls[1],
//                        data_dimension * 4.0 * engine.count_distance_computation_ / (1U << 30U) / diff.count(),
//                        data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ / (1U << 30U) /
//                        diff.count(),
//                        L_local,
//                        subsearch_iterations,
//                        engine.count_merge_ * 1.0 / query_num,
//                        engine.time_expand_,
//                        engine.time_merge_,
//                        engine.time_seq_,
//                        engine.time_pick_,
//                        engine.time_expand_ / diff.count() * 100.0,
//                        engine.time_merge_ / diff.count() * 100.0,
//                        engine.time_seq_ / diff.count() * 100.0,
//                        engine.time_pick_ / diff.count() * 100.0);
//                printf("\n");
//            }
//            engine.count_distance_computation_ = 0;
////                engine.count_iterations_ = 0;
//            engine.count_checked_ = 0;
//            engine.count_merge_ = 0;
//            engine.time_expand_ = 0.0;
//            engine.time_merge_ = 0.0;
//            engine.time_seq_ = 0.0;
//            engine.time_pick_ = 0.0;
//            PANNS::DiskIO::save_result(path_results, set_K_list);
//            exit(0);
//        }

        double runtime;
        uint64_t compt;
        double avg_merge;
//        {
//            unsigned L_local = L_upper_origin;
//            unsigned subsearch_iterations = X;
//            std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
//            set_K_list.resize(query_num);
//            for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);
//
//            std::vector<PANNS::idi> init_ids(L_master);
////                std::vector<uint8_t> is_visited(points_num, 0);
//            boost::dynamic_bitset<> is_visited(points_num);
//            std::vector<PANNS::Candidate> set_L((num_threads - 1) * L_local + L_master);
//            std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
//            std::vector<PANNS::idi> local_queues_starts(num_threads);
//            for (int q_i = 0; q_i < num_threads; ++q_i) {
//                local_queues_starts[q_i] = q_i * L_local;
//            }
////                std::vector<PANNS::idi> top_m_candidates(num_threads);
//            auto s = std::chrono::high_resolution_clock::now();
////                engine.prepare_init_ids(init_ids, L_local);
//            engine.prepare_init_ids(init_ids, L_master);
//            for (unsigned q_i = 0; q_i < query_num; ++q_i) {
//                engine.para_search_PSS_v5_profiling(
//                        q_i,
//                        K,
//                        L_master,
//                        set_L,
//                        init_ids,
//                        set_K_list[q_i],
//                        L_local,
//                        local_queues_starts,
//                        local_queues_sizes,
//                        is_visited,
//                        subsearch_iterations);
//            }
//            auto e = std::chrono::high_resolution_clock::now();
//            std::chrono::duration<double> diff = e - s;
//            std::unordered_map<unsigned, double> recalls;
//            { // Recall values
//                engine.get_recall_for_all_queries(
//                        true_nn_list,
//                        set_K_list,
//                        recalls,
//                        L_master);
//            }
//            {// Basic output
//                printf(
//                        "num_threads: %d "
//                        "L: %u "
//                        "runtime(s.): %f "
//                        "computation: %lu "
//                        "K: %u "
//                        "Volume: %u "
//                        "Dimension: %u "
//                        "query_num: %u "
//                        "query_per_sec: %f "
//                        "avg_latency(ms.): %f "
//                        "P@100: %f "
//                        "P@1: %f "
//                        "G/s: %f "
//                        "GFLOPS: %f "
//                        "local_L: %u "
//                        "sub_iters: %u "
//                        "avg_merge: %f "
//                        "t_expand(s.): %f "
//                        "t_merge(s.): %f "
//                        "t_seq(s.): %f "
//                        "t_pick(s.): %f "
//                        "t_p_expand(%%): %f "
//                        "t_p_merge(%%): %f "
//                        "t_p_seq(%%): %f "
//                        "t_p_pick(%%): %f",
//                        num_threads,
//                        L_master,
//                        diff.count(),
//                        engine.count_distance_computation_,
//                        K,
//                        points_num,
//                        data_dimension,
//                        query_num,
//                        query_num / diff.count(),
//                        diff.count() * 1000 / query_num,
//                        recalls[100],
//                        recalls[1],
//                        data_dimension * 4.0 * engine.count_distance_computation_ / (1U << 30U) / diff.count(),
//                        data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ / (1U << 30U) /
//                        diff.count(),
//                        L_local,
//                        subsearch_iterations,
//                        engine.count_merge_ * 1.0 / query_num,
//                        engine.time_expand_,
//                        engine.time_merge_,
//                        engine.time_seq_,
//                        engine.time_pick_,
//                        engine.time_expand_ / diff.count() * 100.0,
//                        engine.time_merge_ / diff.count() * 100.0,
//                        engine.time_seq_ / diff.count() * 100.0,
//                        engine.time_pick_ / diff.count() * 100.0);
//                printf("\n");
//            }
//            engine.count_distance_computation_ = 0;
////                engine.count_iterations_ = 0;
//            engine.count_checked_ = 0;
//            engine.count_merge_ = 0;
//            engine.time_expand_ = 0.0;
//            engine.time_merge_ = 0.0;
//            engine.time_seq_ = 0.0;
//            engine.time_pick_ = 0.0;
//            PANNS::DiskIO::save_result(path_results, set_K_list);
//            exit(0);
//        }
        std::vector<double> time_break(4);
        {
            unsigned L_local = L_upper_origin;
            unsigned subsearch_iterations = X;
            std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
            set_K_list.resize(query_num);
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
                engine.para_search_PSS_v5_profiling(
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
                        "avg_merge: %f "
                        "t_expand(s.): %f "
                        "t_merge(s.): %f "
                        "t_seq(s.): %f "
                        "t_pick(s.): %f "
                        "t_p_expand(%%): %f "
                        "t_p_merge(%%): %f "
                        "t_p_seq(%%): %f "
                        "t_p_pick(%%): %f",
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
                        data_dimension * 4.0 * engine.count_distance_computation_ / (1U << 30U) / diff.count(),
                        data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ / (1U << 30U) /
                        diff.count(),
                        L_local,
                        subsearch_iterations,
                        engine.count_merge_ * 1.0 / query_num,
                        engine.time_expand_,
                        engine.time_merge_,
                        engine.time_seq_,
                        engine.time_pick_,
                        engine.time_expand_ / diff.count() * 100.0,
                        engine.time_merge_ / diff.count() * 100.0,
                        engine.time_seq_ / diff.count() * 100.0,
                        engine.time_pick_ / diff.count() * 100.0);
                printf("\n");
            }
            engine.count_distance_computation_ = 0;
//                engine.count_iterations_ = 0;
            engine.count_checked_ = 0;
            engine.count_merge_ = 0;
            engine.time_expand_ = 0.0;
            engine.time_merge_ = 0.0;
            engine.time_seq_ = 0.0;
            engine.time_pick_ = 0.0;
            PANNS::DiskIO::save_result(path_results, set_K_list);
            exit(0);
        }

        double last_runtime;
        uint64_t last_compt;
        double last_recall;
        unsigned last_L;
        double last_avg_merge;
        std::vector<double> last_time_break(4);

//        {
//            unsigned L_local = L_upper_origin;
//            unsigned subsearch_iterations = X;
//            std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
//            set_K_list.resize(query_num);
//            for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);
//
//            std::vector<PANNS::idi> init_ids(L_master);
////                std::vector<uint8_t> is_visited(points_num, 0);
//            boost::dynamic_bitset<> is_visited(points_num);
//            std::vector<PANNS::Candidate> set_L((num_threads - 1) * L_local + L_master);
//            std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
//            std::vector<PANNS::idi> local_queues_starts(num_threads);
//            for (int q_i = 0; q_i < num_threads; ++q_i) {
//                local_queues_starts[q_i] = q_i * L_local;
//            }
////                std::vector<PANNS::idi> top_m_candidates(num_threads);
//            auto s = std::chrono::high_resolution_clock::now();
////                engine.prepare_init_ids(init_ids, L_local);
//            engine.prepare_init_ids(init_ids, L_master);
//            for (unsigned q_i = 0; q_i < query_num; ++q_i) {
//                engine.para_search_PSS_v5_profiling(
//                        q_i,
//                        K,
//                        L_master,
//                        set_L,
//                        init_ids,
//                        set_K_list[q_i],
//                        L_local,
//                        local_queues_starts,
//                        local_queues_sizes,
//                        is_visited,
//                        subsearch_iterations);
//            }
//            auto e = std::chrono::high_resolution_clock::now();
//            std::chrono::duration<double> diff = e - s;
//            std::unordered_map<unsigned, double> recalls;
//            { // Recall values
//                engine.get_recall_for_all_queries(
//                        true_nn_list,
//                        set_K_list,
//                        recalls,
//                        L_master);
//            }
//            {// Basic output
//                printf(
//                        "num_threads: %d "
//                        "L: %u "
//                        "runtime(s.): %f "
//                        "computation: %lu "
//                        "K: %u "
//                        "Volume: %u "
//                        "Dimension: %u "
//                        "query_num: %u "
//                        "query_per_sec: %f "
//                        "avg_latency(ms.): %f "
//                        "P@100: %f "
//                        "P@1: %f "
//                        "G/s: %f "
//                        "GFLOPS: %f "
//                        "local_L: %u "
//                        "sub_iters: %u "
//                        "avg_merge: %f "
//                        "t_expand(s.): %f "
//                        "t_merge(s.): %f "
//                        "t_seq(s.): %f "
//                        "t_pick(s.): %f "
//                        "t_p_expand(%%): %f "
//                        "t_p_merge(%%): %f "
//                        "t_p_seq(%%): %f "
//                        "t_p_pick(%%): %f",
//                        num_threads,
//                        L_master,
//                        diff.count(),
//                        engine.count_distance_computation_,
//                        K,
//                        points_num,
//                        data_dimension,
//                        query_num,
//                        query_num / diff.count(),
//                        diff.count() * 1000 / query_num,
//                        recalls[100],
//                        recalls[1],
//                        data_dimension * 4.0 * engine.count_distance_computation_ / (1U << 30U) / diff.count(),
//                        data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ / (1U << 30U) /
//                        diff.count(),
//                        L_local,
//                        subsearch_iterations,
//                        engine.count_merge_ * 1.0 / query_num,
//                        engine.time_expand_,
//                        engine.time_merge_,
//                        engine.time_seq_,
//                        engine.time_pick_,
//                        engine.time_expand_ / diff.count() * 100.0,
//                        engine.time_merge_ / diff.count() * 100.0,
//                        engine.time_seq_ / diff.count() * 100.0,
//                        engine.time_pick_ / diff.count() * 100.0);
//                printf("\n");
//            }
//            engine.count_distance_computation_ = 0;
////                engine.count_iterations_ = 0;
//            engine.count_checked_ = 0;
//            engine.count_merge_ = 0;
//            engine.time_expand_ = 0.0;
//            engine.time_merge_ = 0.0;
//            engine.time_seq_ = 0.0;
//            engine.time_pick_ = 0.0;
//            PANNS::DiskIO::save_result(path_results, set_K_list);
//            exit(0);
//        }

        while (L_lower <= L_upper) {
//            printf("L: %u "
//                   "L_lower: %u "
//                   "L_upper: %u\n",
//                   L_local,
//                   L_lower,
//                   L_upper);

            search_one_time(
                    engine,
                    L_master,
                    K,
                    points_num,
                    query_num,
                    data_dimension,
                    num_threads,
                    L_local,
                    subsearch_iterations,
                    true_nn_list,
                    set_K_list,
                    recalls,
                    runtime,
                    compt,
                    avg_merge,
                    time_break);

            if (recalls[100] < P_dest) {
                L_lower = L_local + 1;
            } else if (recalls[100] > P_dest) {
                L_upper = L_local - 1;
                last_runtime = runtime;
                last_recall = recalls[100];
                last_compt = compt;
                last_L = L_local;
                last_avg_merge = avg_merge;
                last_time_break = time_break;
            } else {
                break;
            }
            if (L_lower <= L_upper) {
                L_local = (L_lower + L_upper) / 2;
//                subsearch_iterations = L_local;
            }
        }

        L_upper = L_upper_origin;
        if (recalls[100] < P_dest && L_local < L_upper) {
            runtime = last_runtime;
            recalls[100] = last_recall;
            compt = last_compt;
            L_local = last_L;
            avg_merge = last_avg_merge;
            time_break = last_time_break;
        }

        PANNS::DiskIO::save_result(path_results, set_K_list);
//        printf("---- FINAL ----\n");
//        printf("P_dest: %f "
//               "runtime(s.): %f "
//               "compt.: %lu "
//               "P@100: %f "
//               "latency(ms.): %f "
//               "L: %u "
//               "X: %u "
//               "avg_merge: %f "
//               "t_expand(s.): %f "
//               "t_merge(s.): %f "
//               "t_seq(s.): %f "
//               "t_pick(s.): %f "
//               "t_p_expand(%%): %f "
//               "t_p_merge(%%): %f "
//               "t_p_seq(%%): %f "
//               "t_p_pick(%%): %f ",
//               P_dest,
//               runtime,
//               compt,
//               recalls[100],
//               runtime / query_num * 1000.0,
//               L_local,
//               subsearch_iterations,
//               avg_merge,
//               time_break[0],
//               time_break[1],
//               time_break[2],
//               time_break[3],
//               time_break[0] / runtime * 100.0,
//               time_break[1] / runtime * 100.0,
//               time_break[2] / runtime * 100.0,
//               time_break[3] / runtime * 100.0);
        printf("\n");

    }
    return 0;
}
