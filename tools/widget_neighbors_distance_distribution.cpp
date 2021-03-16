//
// Created by Zhen Peng on 03/12/2020.
//
/*
 * get the distance distribution of all neighbors. The distance is between a neighbor and its parent.
 */

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
#include "../core/Searching.202103101852.PSS_v5.large_graph.count_reorder.h"

void get_ngbrs_distance(
        std::vector<PANNS::distf> &ngbrs_distance,
        const PANNS::Searching &engine)
{
    PANNS::idi num_v = engine.num_v_;

    PANNS::edgei edge_idx = 0;
    for (PANNS::idi v_i = 0; v_i < num_v; ++v_i) {
        PANNS::dataf *v_data = engine.data_load_ + v_i * engine.dimension_;
        PANNS::dataf v_norm = engine.compute_norm(v_data);
        PANNS::idi out_degree = 0;
        PANNS::idi *out_edges = nullptr;
        engine.get_out_degree_and_edges(
                v_i,
                out_degree,
                out_edges);
        for (PANNS::idi e_i = 0; e_i < out_degree; ++e_i) {
            PANNS::idi nb_id = out_edges[e_i];
            PANNS::distf dist = engine.get_distance_to_query(
                    nb_id,
                    v_data,
                    v_norm);
            ngbrs_distance[edge_idx++] = dist;
        }
    }

    // Check
    if (edge_idx != engine.num_e_) {
        fprintf(stderr,
                "Error: get_ngbrs_distance(): edge_idx %lu != num_e_ %lu .\n", edge_idx, engine.num_e_);
        exit(EXIT_FAILURE);
    }
}

void get_histogram(
        std::vector<PANNS::edgei> &hist_data,
        const std::vector<PANNS::distf> &ngbrs_distance,
        const uint32_t num_bins)
{
    // Get max and min
    PANNS::distf min_dist = FLT_MAX;
    PANNS::distf max_dist = -FLT_MAX;
    for (auto dist : ngbrs_distance) {
        min_dist = std::min(min_dist, dist);
        max_dist = std::max(max_dist, dist);
    }

    PANNS::distf bin_width = (max_dist - min_dist) / num_bins;

    // Get histogram
    for (auto dist : ngbrs_distance) {
        PANNS::idi idx = (dist - min_dist) / bin_width;
        if (idx == num_bins) {
            --idx;
        }
        ++hist_data[idx];
    }
}

void save_histogram(
        const char *filename,
        const std::vector<PANNS::edgei> &hist_data,
        const uint32_t num_bins)
{
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        fprintf(stderr, "Error: cannot open file %s .\n", filename);
        exit(EXIT_FAILURE);
    }

    for (uint32_t b_i = 0; b_i < num_bins; ++b_i) {
        fout << b_i << '\t' << hist_data[b_i] << std::endl;
    }
}

void usage(int argc, char *argv[])
{
    if (argc != 5) {
//        fprintf(stderr,
//                "Usage: %s <data_file> <query_file> <nsg_path> "
//                "<K> <result_file> <true_NN_file> <num_threads> "
//                "<L_master_low> <L_master_up> <L_master_step> <L_local_low> "
//                "<L_local_up> <L_local_step> <X_low> <X_up> "
//                "<X_step> <reorder_map> <opt_id_threshold>\n",
//                argv[0]);
        fprintf(stderr,
                "Usage: %s <data_file> <nsg_path> <num_bins> "
                "<output_histogram>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    usage(argc, argv);
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

    const char *file_data = argv[1];
    const char *file_nsg = argv[2];
    const uint32_t num_bins = strtoull(argv[3], nullptr, 0);
    const char *file_hist = argv[4];
    PANNS::Searching engine;
    engine.load_data_load(file_data);
//    engine.load_and_reorder_nsg_graph(file_nsg);
    engine.load_common_nsg_graph(file_nsg);

    std::vector<PANNS::distf> ngbrs_distance(engine.num_e_);

    // Get all neighbors distance to their own parents.
    get_ngbrs_distance(
            ngbrs_distance,
            engine);

    // Get histogram data
    std::vector<PANNS::edgei> hist_data(num_bins, 0);
    get_histogram(
            hist_data,
            ngbrs_distance,
            num_bins);

    // Save histogram data to file
    save_histogram(
            file_hist,
            hist_data,
            num_bins);

    return EXIT_SUCCESS;
}
