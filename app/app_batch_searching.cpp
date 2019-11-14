//
// Created by Zhen Peng on 11/11/19.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include "../core/Searching.h"
#include "../include/utils.h"
//#include "../include/efanna2e/index_nsg.h"

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: ./%s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <query_num_max>\n", argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 8) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.

    PANNS::Searching engine;
    engine.load_data_load(argv[1]);
    engine.load_queries_load(argv[2]);
    unsigned query_num_max = strtoull(argv[7], nullptr, 0); // Limit of number of queries.
    if (engine.num_queries_ > query_num_max) {
        engine.num_queries_ = query_num_max;
    }
    engine.load_nsg_graph(argv[3]);

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
//        unsigned batch_size_max = 16;
        unsigned batch_size_max = 1;
        for (unsigned batch_size = 1; batch_size <= batch_size_max; batch_size *= 2) {
            int warmup_max = 4;
            for (int warmup = 0; warmup < warmup_max; ++warmup) {
                std::vector< std::vector<PANNS::idi> > set_K_list(query_num);
                for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

                std::vector<PANNS::idi> init_ids(L);
                boost::dynamic_bitset<> flags(engine.num_v_);
                std::vector<std::vector< PANNS::Candidate > > set_L_list(batch_size,
                        std::vector< PANNS::Candidate >(L + 1)); // Return set
                std::vector<boost::dynamic_bitset<> > is_visited_list(batch_size,
                        boost::dynamic_bitset<>(engine.num_v_));// Check flags


                unsigned remain = query_num % batch_size;
                unsigned q_i_bound = query_num - remain;
                auto s = std::chrono::high_resolution_clock::now();
                // Prepare init_ids at first, as they are constant.
                engine.prepare_init_ids(
                        init_ids,
                        flags,
                        L);

//#pragma omp parallel for
                for (unsigned q_i = 0; q_i < q_i_bound; q_i += batch_size) {
                    engine.search_in_batch(
                            K,
                            L,
                            q_i,
                            batch_size,
                            set_L_list,
                            is_visited_list,
                            init_ids,
                            flags,
                            set_K_list);
                }
                if (remain) {
                    engine.search_in_batch(
                            K,
                            L,
                            q_i_bound,
                            remain,
                            set_L_list,
                            is_visited_list,
                            init_ids,
                            flags,
                            set_K_list);
                }
                auto e = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = e - s;
                {// Basic output
                    printf("batch_size: %u "
                           "L: %u "
                           "search_time(s.): %f "
                           //                       "time_distance_computation: %f "
//                           "count_distance_computation: %lu "
                           "K: %u "
                           "Volume: %u "
                           "Dimension: %u "
                           "query_num: %u "
                           "query_per_sec: %f "
                           "average_latency(ms.): %f\n",
                           batch_size,
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

                PANNS::save_result(argv[6], set_K_list);
            }
        }
    }

    return 0;
}
