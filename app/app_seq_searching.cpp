//
// Created by Zhen Peng on 11/13/19.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
//#include "../core/Searching.h"
//#include "../core/Searching.202002101535.reorganization.h"
#include "../core/Searching.202002181409.local_queue_and_merge.h"
//#include "../include/utils.h"
//#include "../include/efanna2e/index_nsg.h"

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: ./%s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path>\n", argv[0]);
//            "Usage: ./%s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <query_num_max>\n", argv[0]);
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
//    unsigned query_num_max = strtoull(argv[7], nullptr, 0); // Limit of number of queries.
//    if (engine.num_queries_ > query_num_max) {
//        engine.num_queries_ = query_num_max;
//    }
    engine.load_nsg_graph(argv[3]);

//    engine.build_opt_graph();

    unsigned L = strtoull(argv[4], nullptr, 0);
    unsigned K = strtoull(argv[5], nullptr, 0);

    if (L < K) {
        std::cout << "search_L cannot be smaller than search_K!" << std::endl;
        exit(-1);
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
//            boost::dynamic_bitset<> is_visited_master(engine.num_v_); // Default 0
//            boost::dynamic_bitset<> is_visited(engine.num_v_);
//            boost::dynamic_bitset<> is_checked(engine.num_v_);
            std::vector<PANNS::Candidate> set_L(L + 1); // Return set

//            PANNS::L3CacheMissRate cache_miss;
            auto s = std::chrono::high_resolution_clock::now();
//            cache_miss.measure_start();
//            engine.cache_miss_kernel.measure_start();
            // Prepare init_ids at first, as they are constant.
//            engine.cache_miss_kernel.measure_start();
//            engine.prepare_init_ids(
//                    init_ids,
//                    is_visited_master,
//                    L);
//            engine.cache_miss_kernel.measure_stop();

            engine.prepare_init_ids(init_ids, L);
//#pragma omp parallel for
            for (unsigned q_i = 0; q_i < query_num; ++q_i) {
//                is_visited = is_visited_master;
//                engine.cache_miss_kernel.measure_start();
                engine.search_in_sequential(
//                        q_i * data_dimension,
                        q_i,
//                        engine.queries_load_ + q_i * engine.dimension_,
                        K,
                        L,
                        set_L,
//                        is_visited,
//                        is_visited_master,
//                        is_checked,
                        init_ids,
                        set_K_list[q_i]);
//                is_visited_master.reset();
//                is_checked.reset();
            }
//            cache_miss.measure_stop();
//            engine.cache_miss_kernel.measure_stop();
            auto e = std::chrono::high_resolution_clock::now();
//            cache_miss.print();
//            engine.cache_miss_kernel.print();
            std::chrono::duration<double> diff = e - s;
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
                       "average_latency(ms.): %f\n",
                       L,
                       diff.count(),
//                       index.time_distance_computation,
//                           index.count_distance_computation,
                       K,
                       points_num,
                       data_dimension,
                       query_num,
                       query_num / diff.count(),
                       diff.count() * 1000 / query_num);
//                index.time_distance_computation = 0.0;
//                    index.count_distance_computation = 0;
            }

            PANNS::DiskIO::save_result(argv[6], set_K_list);
        }
    }

    return 0;
}
