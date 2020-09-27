//
// Created by Zhen Peng on 09/20/2020.
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
#include "../core/Searching.202009201459.simple_search_large_graph.h"

void search_one_time(
        PANNS::Searching &engine,
        const unsigned L,
        const unsigned K,
        const unsigned points_num,
        const unsigned query_num,
        const unsigned data_dimension,
        const std::vector< std::vector<PANNS::idi> > &true_nn_list,
        std::vector<std::vector<PANNS::idi> > &set_K_list,
        std::unordered_map<unsigned, double> &recalls,
        double &runtime,
        uint64_t &compt)
{
    /* ----------------------
     * ------- Kernel ------- */
//    std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
    set_K_list.resize(query_num);
    for (unsigned i = 0; i < query_num; i++) {
        set_K_list[i].resize(K);
    }
    std::vector<PANNS::idi> init_ids(L);
    boost::dynamic_bitset<> is_visited(points_num);
    std::vector<PANNS::Candidate> set_L(L);
    auto s = std::chrono::high_resolution_clock::now();
    engine.prepare_init_ids(init_ids, L);
    for (unsigned q_i = 0; q_i < query_num; ++q_i) {
        engine.seq_search_simple_search_large_graph(
                q_i,
                K,
                L,
                set_L,
                init_ids,
                set_K_list[q_i],
                is_visited);
    }
    auto e = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = e - s;

    { // Recall values
        engine.get_recall_for_all_queries(
                true_nn_list,
                set_K_list,
                recalls,
                K);
    }
    {// Basic output
        printf(
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
                "GFLOPS: %f ",
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
                data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ / (1U << 30U) /diff.count());
        printf("\n");
    }
    runtime = diff.count();
    compt = engine.count_distance_computation_;
    engine.count_distance_computation_ = 0;
//    PANNS::DiskIO::save_result(argv[6], set_K_list);
    /* ------- End of Kernel -------
     * ----------------------------- */
}

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <L> <K> <result_file> <true_NN_file> <L_upper> <P@100>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 10) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

    PANNS::Searching engine;
    engine.load_data_load(argv[1]);
    engine.load_queries_load(argv[2]);
    engine.load_common_nsg_graph(argv[3]);

    unsigned L_lower = strtoull(argv[4], nullptr, 0);

    std::vector< std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[7],
            true_nn_list);

    unsigned L_upper = strtoull(argv[8], nullptr, 0);
    double P_dest = strtod(argv[9], nullptr);

    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;
    std::unordered_map<unsigned, double> recalls;
    unsigned L = L_upper;
    double runtime;
    uint64_t compt;
    std::vector<std::vector<PANNS::idi> > set_K_list;

    while (L_lower <= L_upper) {
//        L = (L_lower + L_upper) / 2;
        printf("L: %u "
               "L_lower: %u "
               "L_upper: %u\n",
               L,
               L_lower,
               L_upper);
        unsigned K = L;

        search_one_time(
                engine,
                L,
                K,
                points_num,
                query_num,
                data_dimension,
                true_nn_list,
                set_K_list,
                recalls,
                runtime,
                compt);

        if (recalls[100] < P_dest) {
            L_lower = L + 1;
        } else {
            L_upper = L - 1;
        }
        L = (L_lower + L_upper) / 2;
    }

    L_upper = strtoull(argv[8], nullptr, 0);
    while (recalls[100] < P_dest && L < L_upper) {
        ++L;
        unsigned K = L;

        search_one_time(
                engine,
                L,
                K,
                points_num,
                query_num,
                data_dimension,
                true_nn_list,
                set_K_list,
                recalls,
                runtime,
                compt);
    }
    PANNS::DiskIO::save_result(argv[6], set_K_list);
    printf("---- FINAL ------\n");
    printf("P_dest: %f "
            "L: %u "
           "runtime(s.): %f "
           "P@100: %f "
           "compt.: %lu\n",
           P_dest,
           L,
           runtime,
           recalls[100],
           compt);

    return 0;
}
