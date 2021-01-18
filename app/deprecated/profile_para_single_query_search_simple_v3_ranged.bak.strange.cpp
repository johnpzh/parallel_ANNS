//
// Created by Zhen Peng on 09/13/2020.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include <clocale>
#include <omp.h>
#include "../core/Searching.202008310636.simple_v3.h"

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <L_low> <K> <result_file> <true_NN_file> <num_threads> <L_max> <L_step> <X_low> <X_max> <X_step>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 14) {
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

    unsigned L_min = strtoull(argv[4], nullptr, 0);
    unsigned K = strtoull(argv[5], nullptr, 0);
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

    std::vector< std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[7],
            true_nn_list);

    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

    int num_threads = strtoull(argv[8], nullptr, 0);
    engine.num_threads_ = num_threads;
    omp_set_num_threads(num_threads);
//    omp_set_nested(1);
//    omp_set_max_active_levels(2);

    unsigned L_max = strtoull(argv[9], nullptr, 0);
    unsigned L_step = strtoull(argv[10], nullptr, 0);
    unsigned X_low = strtoull(argv[11], nullptr, 0);
    unsigned X_max = strtoull(argv[12], nullptr, 0);
    unsigned X_step = strtoull(argv[13], nullptr, 0);

    for (unsigned L = L_min; L <= L_max; L += L_step) {
        unsigned local_queue_capacity = L;

        for (unsigned subsearch_iterations = X_low; subsearch_iterations <= X_max; subsearch_iterations += X_step) {

            unsigned warmup_max = 1;
            for (unsigned warmup = 0; warmup < warmup_max; ++warmup) {
                std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
                for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

                std::vector<PANNS::idi> init_ids(L);
//                std::vector<uint8_t> is_visited(points_num, 0);
                boost::dynamic_bitset<> is_visited(points_num);
                std::vector<PANNS::Candidate> set_L((num_threads - 1) * local_queue_capacity + L);
                std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
                std::vector<PANNS::idi> local_queues_starts(num_threads);
                for (int q_i = 0; q_i < num_threads; ++q_i) {
                    local_queues_starts[q_i] = q_i * local_queue_capacity;
                }
                auto s = std::chrono::high_resolution_clock::now();
                engine.prepare_init_ids(init_ids, L);
                for (unsigned q_i = 0; q_i < query_num; ++q_i) {
                    engine.para_search_with_simple_v3(
                            q_i,
                            K,
                            L,
                            set_L,
                            init_ids,
                            set_K_list[q_i],
                            local_queue_capacity,
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
                            recalls);
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
                            "iter_mean: %f "
                            "merge_mean: %f "
                            "checked_mean: %f",
                            num_threads,
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
                            data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ / (1U << 30U) /
                            diff.count(),
                            local_queue_capacity,
                            subsearch_iterations,
                            engine.count_iterations_ * 1.0 / query_num,
                            engine.count_merge_ * 1.0 / query_num,
                            engine.count_checked_ * 1.0 / engine.count_merge_);
                    printf("\n");
                }
                engine.count_distance_computation_ = 0;
                engine.count_iterations_ = 0;
                engine.count_checked_ = 0;
                engine.count_merge_ = 0;
                PANNS::DiskIO::save_result(argv[6], set_K_list);
            }
        } // X Ranged
    } // L ranged

    return 0;
}
