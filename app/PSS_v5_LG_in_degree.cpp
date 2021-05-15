//
// Created by Zhen Peng on 03/16/2020.
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
//#include "../core/Searching.202008211350.simple_top_m.h"
#include "../core/Searching.202103101606.PSS_v5.large_graph.v_counter.h"

void save_reorder_map(
        const PANNS::Searching &engine,
        const char *output_file)
{
    // The in degree
    PANNS::idi num_v = engine.num_v_;
    std::vector<PANNS::idi> old_to_new(num_v);
    std::vector< std::pair<PANNS::idi, PANNS::idi> > in_degrees(num_v);
    for (PANNS::idi v_i = 0; v_i < num_v; ++v_i) {
        in_degrees[v_i].first = 0;
        in_degrees[v_i].second = v_i;
    }

    for (PANNS::idi v_i = 0; v_i < num_v; ++v_i) {
        const PANNS::idi *out_edges = engine.common_nsg_deg_ngbrs_ + engine.common_nsg_vertex_base_[v_i];
        PANNS::idi out_dgree = *out_edges++;
        // Traverse neighbors
        for (PANNS::idi e_i = 0; e_i < out_dgree; ++e_i) {
            PANNS::idi nb_id = out_edges[e_i];
            ++in_degrees[nb_id].first;
        }
    }

    std::sort(in_degrees.rbegin(), in_degrees.rend());

    for (PANNS::idi new_i = 0; new_i < num_v; ++new_i) {
        PANNS::idi old_i = in_degrees[new_i].second;
        old_to_new[old_i] = new_i;

//        // test
//        if (in_degrees[new_i].first > 4) {
//            printf("%u %u %lu\n", old_i, new_i, in_degrees[new_i].first);
//        }
    }

    // Save to file
    std::ofstream fout(output_file);
    if (!fout.is_open()) {
        fprintf(stderr, "Error: cannot create file %s .\n", output_file);
        exit(EXIT_FAILURE);
    }
    for (PANNS::idi old_i = 0; old_i < num_v; ++old_i) {
        PANNS::idi new_i = old_to_new[old_i];
        fout.write(reinterpret_cast<char *>(&old_i), sizeof(old_i));
        fout.write(reinterpret_cast<char *>(&new_i), sizeof(new_i));
    }
}

void usage(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr,
                "Usage: %s <data_file> <nsg_path> <output_reorder_map>\n",
                argv[0]);
//        fprintf(stderr,
//                "Usage: %s <data_file> <query_file> <nsg_path> "
//                "<K> <result_file> <true_NN_file> <num_threads> "
//                "<L_master_low> <L_master_up> <L_master_step> <L_local_low> "
//                "<L_local_up> <L_local_step> <X_low> <X_up> "
//                "<X_step> <output_reorder_map>\n",
//                argv[0]);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    usage(argc, argv);
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

    PANNS::Searching engine;
    engine.load_data_load(argv[1]);
    engine.load_common_nsg_graph(argv[2]);

    const char *file_reorder_map = argv[3];

    save_reorder_map(
            engine,
            file_reorder_map);

    return 0;
}
