//
// Created by Zhen Peng on 9/30/2020.
//
/*
 * Get the true-K nearest neighbors (NN), then save to a file to be a input for test_precision.
 */
//#include "../include/efanna2e/index_nsg.h"
//#include "../include/efanna2e/util.h"
#include <chrono>
#include <string>
#include <omp.h>
#include  "../include/utils.h"
#include <cmath>
#include "../core/Searching.202009171601.simple_v3.large_graph.h"

void get_true_NN(
        const unsigned query_id,
        const unsigned K,
        const PANNS::Searching &engine,
//        const float *query_data,
        std::vector< PANNS::Candidate > &ngbrs)
{
    const float *query_data = engine.queries_load_ + query_id * engine.dimension_;
    unsigned ngbrs_end = 0;
    for (unsigned v_id = 0; v_id < engine.num_v_; ++v_id) {
        float dist = engine.compute_distance(engine.data_load_ + v_id * engine.dimension_, query_data);
        PANNS::Candidate cand(v_id, dist, false);
        PANNS::Searching::add_into_queue(
                ngbrs,
                0,
                ngbrs_end,
                K,
                cand);
    }
}

void save_result(const char *filename,
                 unsigned query_num,
                 unsigned K,
                 const std::vector <std::vector< PANNS::Candidate > > &true_ress)
{
    assert(query_num == true_ress.size());
    std::ofstream out(filename, std::ios::binary | std::ios::out);
    if (!out.is_open()) {
        fprintf(stderr, "Error: cannot create file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    out.write(reinterpret_cast<char *>(&query_num), sizeof(query_num)); // Write query_num
    out.write(reinterpret_cast<char *>(&K), sizeof(K)); // Write K

    for (const auto &qr : true_ress) {
        assert(K == qr.size());
        for (const auto &r : qr) {
            unsigned id = r.id_;
            float dist = r.distance_;
            out.write(reinterpret_cast<char *>(&id), sizeof(id));
            out.write(reinterpret_cast<char *>(&dist), sizeof(dist));
//            {//test
//                printf("[%u %f]\n", id, dist);
//            }
        }
    }
    out.close();
}

int main(int argc, char **argv)
{
    if (argc != 6) {
        std::cout << argv[0]
                  << " <data_file> <query_file> <search_K> <result_path> <num_t>"
//                  << " <data_file> <query_file> <nsg_path> <search_K> <result_path>"
                  << std::endl;
        exit(-1);
    }
    setbuf(stdout, nullptr); // Added by Johnpzh

    PANNS::Searching engine;
    printf("Loading %s ...\n", argv[1]);
    engine.load_data_load(argv[1]);
    printf("Loading %s ...\n", argv[2]);
    engine.load_queries_load(argv[2]);
    unsigned K = strtoull(argv[3], nullptr, 0);
    int num_threads = strtoull(argv[5], nullptr, 0);
    omp_set_num_threads(num_threads);

    unsigned query_num = engine.num_queries_;
    std::vector< std::vector< PANNS::Candidate > > true_ress(query_num);
    for (unsigned q_i = 0; q_i < query_num; ++q_i) {
        true_ress[q_i].resize(K);
    }

    // Queries
    printf("Computing...\n");
    auto s = std::chrono::high_resolution_clock::now();
#pragma omp parallel for
    for (unsigned query_id = 0; query_id < query_num; query_id++) {
        get_true_NN(
                query_id,
                K,
                engine,
                true_ress[query_id]);
    }
    // Save true_res to the file
    printf("Saving...\n");
    save_result(argv[4], query_num, K, true_ress);

    auto e = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = e - s;
    std::cout << "run_time: " << diff.count() << "\n";

    return 0;
}

