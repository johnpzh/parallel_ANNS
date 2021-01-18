//
// Created by Zhen Peng on 01/08/2020.
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
#include "../core/Searching.202101081737.top_m_pure.profiling.h"

void search_one_time(
        PANNS::Searching &engine,
        const unsigned M,
        const unsigned L,
        const unsigned K,
        const unsigned points_num,
        const unsigned query_num,
        const unsigned data_dimension,
        const int num_threads,
        const unsigned local_queue_capacity,
//        const unsigned subsearch_iterations,
        const std::vector< std::vector<PANNS::idi> > &true_nn_list,
        std::vector< std::vector<unsigned> > &set_K_list_return,
        std::unordered_map<unsigned, double> &recalls,
        double &runtime,
        uint64_t &compt,
        double &avg_merge,
        double &t_expand,
        double &t_merge)
{
    std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
    for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

    std::vector<PANNS::idi> init_ids(L);
//                std::vector<uint8_t> is_visited(points_num, 0);
    boost::dynamic_bitset<> is_visited(points_num);
    std::vector<PANNS::Candidate> set_L((num_threads - 1) * local_queue_capacity + L);
    std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
    std::vector<PANNS::idi> local_queues_starts(num_threads);
    for (int queue_i = 0; queue_i < num_threads; ++queue_i) {
        local_queues_starts[queue_i] = queue_i * local_queue_capacity;
    }
    std::vector<PANNS::idi> top_m_candidates(M);
    auto s = std::chrono::high_resolution_clock::now();
    engine.prepare_init_ids(init_ids, L);
    for (unsigned q_i = 0; q_i < query_num; ++q_i) {
        engine.para_search_with_top_m_pure_profiling(
                M,
                q_i,
                K,
                L,
                set_L,
                init_ids,
                set_K_list[q_i],
                local_queue_capacity,
                local_queues_starts,
                local_queues_sizes,
                top_m_candidates,
                is_visited);
    }
    auto e = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = e - s;
//    std::unordered_map<unsigned, double> recalls;
    { // Recall values
        engine.get_recall_for_all_queries(
                true_nn_list,
                set_K_list,
                recalls,
                L);
    }
    {// Basic output
        printf(
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
                "avg_merge: %f "
                "t_expand(s.): %f "
                "t_merge(s.): %f ",
                num_threads,
                M,
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
                data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ / (1U << 30U) /diff.count(),
                engine.count_merge_ * 1.0 / query_num,
                engine.time_expand_,
                engine.time_merge_);
        printf("\n");
    }
    runtime = diff.count();
    compt = engine.count_distance_computation_;
    engine.count_distance_computation_ = 0;
    avg_merge = engine.count_merge_ * 1.0 / query_num;
    engine.count_merge_ = 0;
    t_expand = engine.time_expand_;
    t_merge = engine.time_merge_;
    engine.time_expand_ = 0.0;
    engine.time_merge_ = 0.0;
    set_K_list_return.swap(set_K_list);
}

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <L_lower> <K> <result_file> <M> <true_NN_file> <num_threads> <L_upper> <P@100> [<P@100> ...]\n",
//            "Usage: %s <data_file> <query_file> <nsg_path> <L_lower> <K> <result_file> <M> <true_NN_file> <num_threads> <local_L>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc < 12) {
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

    unsigned L_lower_origin = strtoull(argv[4], nullptr, 0);
    unsigned K = strtoull(argv[5], nullptr, 0);
    unsigned M = strtoull(argv[7], nullptr, 0);
//    if (L < K) {
//        fprintf(stderr, "Error: search_L %u is smaller than search_K %u\n.", L, K);
//        exit(EXIT_FAILURE);
//    }

    std::vector< std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[8],
            true_nn_list);

    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

    int num_threads = strtoull(argv[9], nullptr, 0);
    engine.num_threads_ = num_threads;
    omp_set_num_threads(num_threads);
//    unsigned local_queue_capacity = strtoull(argv[10], nullptr, 0);

    unsigned L_upper_origin = strtoull(argv[10], nullptr, 0);
    const unsigned base_loc_P_dest = 11;
    unsigned num_P_target = argc - base_loc_P_dest;
    std::vector<double> P_targets(num_P_target);
    for (int a_i = base_loc_P_dest; a_i < argc; ++a_i) {
        P_targets[a_i - base_loc_P_dest] = strtod(argv[a_i], nullptr);
    }

//    unsigned warmup_max = 1;
//    for (unsigned warmup = 0; warmup < warmup_max; ++warmup) {
    for (const double P_dest : P_targets) {
        std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
//        for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);
        std::unordered_map<unsigned, double> recalls;
        unsigned L_upper = L_upper_origin;
        unsigned L_lower = L_lower_origin;
        unsigned L = L_upper;
        unsigned local_queue_capacity = L;
        double runtime;
        uint64_t compt;
        double avg_merge;
        double t_expand;
        double t_merge;

        double last_runtime;
        uint64_t last_compt;
        double last_recall;
        unsigned last_L;
        double last_avg_merge;
        double last_t_expand;
        double last_t_merge;

        while (L_lower <= L_upper) {
            printf("L: %u "
                   "L_lower: %u "
                   "L_upper: %u\n",
                   L,
                   L_lower,
                   L_upper);

            search_one_time(
                    engine,
                    M,
                    L,
                    K,
                    points_num,
                    query_num,
                    data_dimension,
                    num_threads,
                    local_queue_capacity,
                    true_nn_list,
                    set_K_list,
                    recalls,
                    runtime,
                    compt,
                    avg_merge,
                    t_expand,
                    t_merge);

            if (recalls[100] < P_dest) {
                L_lower = L + 1;
            } else if (recalls[100] > P_dest) {
                L_upper = L - 1;
                last_runtime = runtime;
                last_recall = recalls[100];
                last_compt = compt;
                last_L = L;
                last_avg_merge = avg_merge;
                last_t_expand = t_expand;
                last_t_merge = t_merge;
            } else {
                break;
            }
            if (L_lower <= L_upper) {
                L = (L_lower + L_upper) / 2;
                local_queue_capacity = L;
            }
        }

//        L_upper = strtoull(argv[10], nullptr, 0);
        if (recalls[100] < P_dest && L < L_upper_origin) {
            runtime = last_runtime;
            recalls[100] = last_recall;
            compt = last_compt;
            L = last_L;
            avg_merge = last_avg_merge;
            t_expand = last_t_expand;
            t_merge = last_t_merge;
        }
        PANNS::DiskIO::save_result(argv[6], set_K_list);
        printf("---- FINAL ----\n");
        printf("P_dest: %f "
               "runtime(s.): %f "
               "compt.: %lu "
               "P@100: %f "
               "latency(ms.): %f "
               "L: %u "
               "M: %u "
               "avg_merge: %f "
               "t_expand(s.): %f "
               "t_merge(s.): %f "
               "t_p_expand(%%): %f "
               "t_p_merge(%%): %f",
               P_dest,
               runtime,
               compt,
               recalls[100],
               runtime / query_num * 1000.0,
               L,
               M,
               avg_merge,
               t_expand,
               t_merge,
               t_expand / runtime * 100.0,
               t_merge / runtime * 100.0);
        printf("\n");
    }
//    }

    return 0;
}
