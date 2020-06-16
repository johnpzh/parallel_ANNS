//
// Created by Zhen Peng on 12/16/19.
//

// DEPRECATED. std::set cannot be used because its element cannot be modified.

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
//#include "../core/Searching.201912161559.set_for_queue.h"
#include "../../core/deprecated/Searching.202002101535.reorganization.h"
//#include "../include/utils.h"
//#include "../include/efanna2e/index_nsg.h"

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: ./%s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path>\n", argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 7) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.

    PANNS::Searching engine;
    engine.load_data_load(argv[1]);
    engine.load_queries_load(argv[2]);
    engine.load_nsg_graph(argv[3]);

    unsigned L = strtoull(argv[4], nullptr, 0);
    unsigned K = strtoull(argv[5], nullptr, 0);

    if (L < K) {
        std::cout << "search_L cannot be smaller than search_K!" << std::endl;
        exit(EXIT_FAILURE);
    }
    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

    int num_threads_max = 1;
    for (int num_threads = 1; num_threads < num_threads_max + 1; num_threads *= 2) {
//        omp_set_num_threads(num_threads);
        int warmup_max = 1;
        for (int warmup = 0; warmup < warmup_max; ++warmup) {
            std::vector< std::vector<PANNS::idi> > set_K_list(query_num);
            for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

            std::vector<PANNS::idi> init_ids(L);
            std::vector<PANNS::Candidate> set_L(L + 1); // Return set

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
            {// Basic output
                printf("L: %u "
                       "search_time(s.): %f "
                       "K: %u "
                       "Volume: %u "
                       "Dimension: %u "
                       "query_num: %u "
                       "query_per_sec: %f "
                       "average_latency(ms.): %f\n",
                       L,
                       diff.count(),
                       K,
                       points_num,
                       data_dimension,
                       query_num,
                       query_num / diff.count(),
                       diff.count() * 1000 / query_num);
            }

            PANNS::DiskIO::save_result(argv[6], set_K_list);
        }
    }

    return 0;
}
