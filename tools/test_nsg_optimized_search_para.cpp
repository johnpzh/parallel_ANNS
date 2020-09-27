//
// Created by Zhen Peng on 09/21/2020.
//

#include "../include/efanna2e/index_nsg.h"
#include "../include/efanna2e/util.h"
#include <chrono>
#include <string>
#include <clocale>
// Added by Johnpzh
#include <omp.h>
//#include  "extras/utils.h"
//#include "../include/utils.h"
//#include "../core/Searching.h"
// Ended by Johnpzh

void load_data(char *filename, float *&data, unsigned &num,
               unsigned &dim)
{  // load data with sift10K pattern
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    in.read((char *) &dim, 4);
    // std::cout<<"data dimension: "<<dim<<std::endl;
    in.seekg(0, std::ios::end);
    std::ios::pos_type ss = in.tellg();
    size_t fsize = (size_t) ss;
    num = (unsigned) (fsize / (dim + 1) / 4);
//    data = new float[(size_t) num * (size_t) dim];
    data = (float *) _mm_malloc((size_t) num * (size_t) dim * sizeof(float), 64);

    in.seekg(0, std::ios::beg);
    for (size_t i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur);
        in.read((char *) (data + i * dim), dim * 4);
    }
    in.close();
}

void save_result(const char *filename,
                 std::vector<std::vector<unsigned>> &results)
{
    std::ofstream out(filename, std::ios::binary | std::ios::out);

    for (unsigned i = 0; i < results.size(); i++) {
        unsigned GK = (unsigned) results[i].size();
        out.write((char *) &GK, sizeof(unsigned));
        out.write((char *) results[i].data(), GK * sizeof(unsigned));
    }
    out.close();
}

int main(int argc, char **argv)
{
    if (argc != 8) {
        std::cout << argv[0]
                  << " data_file query_file nsg_path search_L search_K result_path num_threads"
//                  << " data_file query_file nsg_path search_L search_K result_path query_num_max"
                  << std::endl;
        exit(-1);
    }
    setbuf(stdout, nullptr); // Added by Johnpzh
    setlocale(LC_NUMERIC, ""); // For comma number format

    float *data_load = NULL;
    unsigned points_num, dim;
    load_data(argv[1], data_load, points_num, dim);
    float *query_load = NULL;
    unsigned query_num, query_dim;
    load_data(argv[2], query_load, query_num, query_dim);
    assert(dim == query_dim);
    // Added by Johnpzh
//    {
//        unsigned query_num_max = strtoull(argv[7], nullptr, 0);
//        if (query_num > query_num_max) query_num = query_num_max;
////        printf("query_num: %u\n", query_num);
//    }
    // Ended by Johnpzh

    unsigned L = strtoull(argv[4], nullptr, 0);
    unsigned K = strtoull(argv[5], nullptr, 0);
//    unsigned L = (unsigned) atoi(argv[4]);
//    unsigned K = (unsigned) atoi(argv[5]);

    if (L < K) {
        std::cout << "search_L cannot be smaller than search_K!" << std::endl;
        exit(-1);
    }

    // data_load = efanna2e::data_align(data_load, points_num, dim);//one must
    // align the data before build query_load = efanna2e::data_align(query_load,
    // query_num, query_dim);
    efanna2e::IndexNSG index(dim, points_num, efanna2e::FAST_L2, nullptr);
    index.Load(argv[3]);
    index.OptimizeGraph(data_load);

    efanna2e::Parameters paras;
    paras.Set<unsigned>("L_search", L);
    paras.Set<unsigned>("P_search", L);

//    int num_threads_max = 1;
//    for (int num_threads = 1; num_threads < num_threads_max + 1; num_threads *= 2) {
        int num_threads = strtoull(argv[7], nullptr, 0);
        omp_set_num_threads(num_threads);
        // Added by Johnpzh
        int warmup_max = 1;
        for (int warmup = 0; warmup < warmup_max; ++warmup) {
            std::vector<std::vector<unsigned>> res(query_num);
            for (unsigned i = 0; i < query_num; i++) res[i].resize(K);

//            PANNS::L3CacheMissRate cache_miss;
            auto s = std::chrono::high_resolution_clock::now();
//            cache_miss.measure_start();
#pragma omp parallel for
            for (unsigned i = 0; i < query_num; i++) {
                index.SearchWithOptGraph(query_load + i * dim, K, paras, res[i].data());
            }
            // Ended by Johnpzh
//            cache_miss.measure_stop();
            auto e = std::chrono::high_resolution_clock::now();
//            cache_miss.print();
//            index.cache_miss_kernel.print();
            std::chrono::duration<double> diff = e - s;
            // Add by Johnpzh
            {// Basic output
                printf("num_threads: %d "
                       "L: %u "
                       "search_time(s.): %f "
//                       "time_distance_computation: %f "
                       "count_distance_computation: %'lu "
                       "K: %u "
                       "Volume: %u "
                       "Dimension: %u "
                       "query_num: %u "
                       "query_per_sec: %f "
                       "average_latency(ms.): %f\n",
                       num_threads,
                       L,
                       diff.count(),
//                       index.time_distance_computation,
                       index.count_distance_computation,
                       K,
                       points_num,
                       dim,
                       query_num,
                       query_num / diff.count(),
                       diff.count() * 1000 / query_num);
//                index.time_distance_computation = 0.0;
                index.count_distance_computation = 0;
            }
            // Ended by Johnpzh

            save_result(argv[6], res);
        }
//    }
    return 0;
}
