//
// Created by Zhen Peng on 10/14/2020.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include <clocale>
#include <omp.h>
//#include "../include/papi_panns.h"
#include "../core/Searching.202102022027.PSS_v5.dist_thresh.profiling.h"

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
        double &hops_mean)
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

        engine.para_search_PSS_v5_dist_thresh_profiling(
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
    engine.count_merge_ = 0;
    engine.time_expand_ = 0.0;
    engine.time_merge_ = 0.0;
    engine.time_seq_ = 0.0;
    runtime = diff.count();
    set_K_list_return.swap(set_K_list);
}

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <L_lower> <K> <result_file> <true_NN_file> <num_threads> <sub_iters> <L_upper> <P@100> [<P@100> ...]\n",
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

    unsigned L_lower_origin = strtoull(argv[4], nullptr, 0);
    unsigned K = strtoull(argv[5], nullptr, 0);
    std::vector< std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[7],
            true_nn_list);

    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

    int num_threads = strtoull(argv[8], nullptr, 0);
    if (1 != num_threads) {
        fprintf(stderr, "Error: num_threads is %d, which should be 1.\n", num_threads);
        exit(EXIT_FAILURE);
    }
    engine.num_threads_ = num_threads;
    omp_set_num_threads(num_threads);
//    omp_set_nested(1);
//    omp_set_max_active_levels(2);

    unsigned subsearch_iterations = strtoull(argv[9], nullptr, 0);
    unsigned L_upper_origin = strtoull(argv[10], nullptr, 0);
//    double P_dest = strtod(argv[11], nullptr);
    const unsigned base_loc_P_dest = 11;
    unsigned num_P_target = argc - base_loc_P_dest;
    std::vector<double> P_targets(num_P_target);
    for (int a_i = base_loc_P_dest; a_i < argc; ++a_i) {
        P_targets[a_i - base_loc_P_dest] = strtod(argv[a_i], nullptr);
    }


    for (const double P_dest : P_targets) {
        std::vector<std::vector<unsigned> > set_K_list;
        std::unordered_map<unsigned, double> recalls;
        unsigned L_upper = L_upper_origin;
        unsigned L_lower = L_lower_origin;
        unsigned L = L_upper;
        unsigned local_queue_capacity = L;
        double runtime;
        uint64_t compt;
        double hops_mean;

        double last_runtime;
        uint64_t last_compt;
        double last_recall;
        unsigned last_L;
        double last_hops_mean;

        while (L_lower <= L_upper) {
            printf("L: %u "
                   "L_lower: %u "
                   "L_upper: %u\n",
                   L,
                   L_lower,
                   L_upper);

            search_one_time(
                    engine,
                    L,
                    K,
                    points_num,
                    query_num,
                    data_dimension,
                    num_threads,
                    local_queue_capacity,
                    subsearch_iterations,
                    true_nn_list,
                    set_K_list,
                    recalls,
                    runtime,
                    compt,
                    hops_mean);

            if (recalls[100] < P_dest) {
                L_lower = L + 1;
            } else if (recalls[100] > P_dest) {
                L_upper = L - 1;
                last_runtime = runtime;
                last_recall = recalls[100];
                last_compt = compt;
                last_L = L;
                last_hops_mean = hops_mean;
            } else {
                break;
            }
            if (L_lower <= L_upper) {
                L = (L_lower + L_upper) / 2;
                local_queue_capacity = L;
            }
        }

        L_upper = strtoull(argv[10], nullptr, 0);
        if (recalls[100] < P_dest && L < L_upper) {
            runtime = last_runtime;
            recalls[100] = last_recall;
            compt = last_compt;
            L = last_L;
            hops_mean = last_hops_mean;
        }

        PANNS::DiskIO::save_result(argv[6], set_K_list);
        printf("---- FINAL ----\n");
        printf("P_dest: %f "
               "runtime(s.): %f "
               "compt.: %lu "
               "P@100: %f "
               "latency(ms.): %f "
               "L: %u "
               "X: %u "
               "avg_hops: %f ",
               P_dest,
               runtime,
               compt,
               recalls[100],
               runtime / query_num * 1000.0,
               L,
               subsearch_iterations,
               hops_mean);
        printf("\n");

    }
    return 0;
}
