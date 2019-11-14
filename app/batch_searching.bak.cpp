//
// Created by Zhen Peng on 11/11/19.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include "../core/Searching.h"
#include "../include/utils.h"
#include "../include/efanna2e/index_nsg.20191114.vector_norms.h"

int main(int argc, char **argv)
{
    if (argc != 8) {
        std::cout << argv[0]
                  << " data_file query_file nsg_path search_L search_K result_path query_num_max"
                  << std::endl;
        exit(-1);
    }
    setbuf(stdout, nullptr); // Added by Johnpzh

    PANNS::Searching engine;
    engine.load_data_load(argv[1]);
    engine.load_queries_load(argv[2]);
    unsigned query_num_max = strtoull(argv[7], nullptr, 0); // Limit of number of queries.
    if (engine.num_queries_ > query_num_max) {
        engine.num_queries_ = query_num_max;
    }
    engine.load_nsg_graph(argv[3]);
//    std::vector<float> data_load;
//    unsigned data_dimension, points_num;
//    PANNS::load_data(
//            argv[1],
//            data_load,
//            points_num,
//            data_dimension);
//    std::vector<float> query_load;
//    unsigned query_dimension, query_num;
//    PANNS::load_data(
//            argv[2],
//            query_load,
//            query_num,
//            query_dimension);
//    assert(data_dimension == query_dimension);
//    unsigned query_num_max = strtoull(argv[7], nullptr, 0); // Limit of number of queries.
//    if (query_num > query_num_max) {
//        query_num = query_num_max;
//    }
//
    unsigned L = strtoull(argv[4], nullptr, 0);
    unsigned K = strtoull(argv[5], nullptr, 0);

    if (L < K) {
        std::cout << "search_L cannot be smaller than search_K!" << std::endl;
        exit(-1);
    }
//
//    // data_load = efanna2e::data_align(data_load, points_num, dim);//one must
//    // align the data before build query_load = efanna2e::data_align(query_load,
//    // query_num, query_dim);
    efanna2e::IndexNSG index(data_dimension, points_num, efanna2e::FAST_L2, nullptr);
//    index.Load(argv[3]);
//    index.OptimizeGraph(data_load.data());
//
//    efanna2e::Parameters paras;
//    paras.Set<unsigned>("L_search", L);
//    paras.Set<unsigned>("P_search", L);

        int num_threads_max = 1;
    for (int num_threads = 1; num_threads < num_threads_max + 1; num_threads *= 2) {
//        omp_set_num_threads(num_threads);
//        std::vector <std::vector<unsigned>> res(query_num);
//        for (unsigned i = 0; i < query_num; i++) res[i].resize(K);
//        unsigned batch_size_max = 16;
        unsigned batch_size_max = 1;
        for (unsigned batch_size = 1; batch_size <= batch_size_max; batch_size *= 2) {
            // Added by Johnpzh
            int warmup_max = 4;
            for (int warmup = 0; warmup < warmup_max; ++warmup) {
                std::vector<std::vector<unsigned>> res(query_num);
                for (unsigned i = 0; i < query_num; i++) res[i].resize(K);

                std::vector<unsigned> init_ids(L);
                boost::dynamic_bitset<> flags(index.GetSizeOfDataset());
                std::vector<std::vector<efanna2e::Neighbor> > retset_list(batch_size, std::vector<efanna2e::Neighbor>(
                        L + 1)); // Return set
                std::vector<boost::dynamic_bitset<> > is_visited_list(batch_size, boost::dynamic_bitset<>(
                        index.GetSizeOfDataset()));// Check flags


                unsigned remain = query_num % batch_size;
                unsigned q_i_bound = query_num - remain;
                auto s = std::chrono::high_resolution_clock::now();
                index.PrepareInitIDs(
                        init_ids,
                        flags,
                        L);
                // Prepare init_ids at first, as they are constant.

//#pragma omp parallel for
                for (unsigned q_i = 0; q_i < q_i_bound; q_i += batch_size) {
                    index.PrepareRetsetList(
                            query_load.data(),
                            retset_list,
                            is_visited_list,
                            init_ids,
                            flags,
                            q_i,
                            batch_size,
                            L);
                    index.SearchWithOptGraphInBatch(
                            query_load.data(),
                            K,
                            L,
//                            paras,
                            retset_list,
                            is_visited_list,
//                            init_ids,
//                            flags,
                            q_i,
                            batch_size,
                            res);
                }
                if (remain) {
                    index.PrepareRetsetList(
                            query_load.data(),
                            retset_list,
                            is_visited_list,
                            init_ids,
                            flags,
                            q_i_bound,
                            batch_size,
                            L);
                    index.SearchWithOptGraphInBatch(
                            query_load.data(),
                            K,
                            L,
//                            paras,
                            retset_list,
                            is_visited_list,
//                            init_ids,
//                            flags,
                            q_i_bound,
                            remain,
                            res);
                }
//                for (unsigned i = 0; i < query_num; i++) {
//                    index.SearchWithOptGraph(query_load + i * dim, K, paras, res[i].data());
//                }
                // Ended by Johnpzh
                auto e = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = e - s;
                // Add by Johnpzh
                {// Basic output
                    printf("batch_size: %u "
                           "L: %u "
                           "search_time(s.): %f "
                           //                       "time_distance_computation: %f "
                           "count_distance_computation: %lu "
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
                           index.count_distance_computation,
                           K,
                           points_num,
                           data_dimension,
                           query_num,
                           query_num / diff.count(),
                           diff.count() * 1000 / query_num);
//                index.time_distance_computation = 0.0;
                    index.count_distance_computation = 0;
                }

                // Ended by Johnpzh

                PANNS::save_result(argv[6], res);
            }
        }
    }

//    _mm_free(data_load);
//    _mm_free(query_load);
//    delete data_load;
//    delete query_load;
    return 0;
}
//void usage(char *argv[])
//{
//    printf("Usage: ./%s\n", argv[0]);
//}
//
//int main(int argc, char *argv[])
//{
//    if (argc != 1) {
//        usage(argv);
//        exit(EXIT_FAILURE);
//    }
//    printf("Done.\n");
//
//    return EXIT_SUCCESS;
//}