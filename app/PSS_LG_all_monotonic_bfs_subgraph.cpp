//
// Created by Zhen Peng on 06/08/2020.
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
#include "../core/Searching.202106051704.PSS_LG_bfs_subgraph.h"
//#define DEBUG_PRINT

std::string get_no_suffix_name(const char *filename)
{
    std::string namestr(filename);
    std::string::size_type dot_pos = namestr.rfind(".");
    if (dot_pos != std::string::npos) {
        return namestr.substr(0, dot_pos);
    } else {
        return namestr;
    }
}

PANNS::idi get_num_points_exclude_on_paths(
        const std::vector<bool> &is_reached,
        const PANNS::idi num_v,
        const PANNS::idi num_on_paths)
{
    PANNS::idi num_points = 0;
    for (PANNS::idi v_i = 0; v_i < num_v; ++v_i) {
        if (is_reached[v_i]) {
            ++num_points;
        }
    }
    return num_points - num_on_paths;
}




void usage(int argc, char *argv[])
{
    if (argc != 21) {
        fprintf(stderr,
                "Usage: %s <data_file> <query_file> <nsg_path> "
                "<K> <result_file> <true_NN_file> <num_threads> "
                "<L_master_low> <L_master_up> <L_master_step> <L_local_low> "
                "<L_local_up> <L_local_step> <X_low> <X_up> "
                "<X_step> <I_thresh_low> <I_thresh_up> <I_thresh_step> "
                "<query_id>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    usage(argc, argv);
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

    PANNS::Searching engine;
#ifdef DEBUG_PRINT
    printf("Loading data: %s\n", argv[1]);
#endif
    engine.load_data_load(argv[1]);
#ifdef DEBUG_PRINT
    printf("Loading queries: %s\n", argv[2]);
#endif
    engine.load_queries_load(argv[2]);
#ifdef DEBUG_PRINT
    printf("Loading nsg index: %s\n", argv[3]);
#endif
//    {//test
//        engine.num_v_ = 1000000000; // 1 Billion
//        engine.num_queries_ = 10000;
//        engine.dimension_ = 96;
//    }
    engine.load_common_nsg_graph(argv[3]);

//    engine.build_opt_graph();

//    unsigned L_min = strtoull(argv[4], nullptr, 0);
    const unsigned K = strtoull(argv[4], nullptr, 0);
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
    const char *path_results = argv[5];
    std::vector< std::vector<PANNS::idi> > true_nn_list;
#ifdef DEBUG_PRINT
    printf("Loading true_NN: %s\n", argv[6]);
#endif
    engine.load_true_NN(
            argv[6],
            true_nn_list);

    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

    int num_threads = strtoull(argv[7], nullptr, 0);
    engine.num_threads_ = num_threads;
    omp_set_num_threads(num_threads);
//    omp_set_nested(1);
//    omp_set_max_active_levels(2);
    const unsigned L_master_low = strtoull(argv[8], nullptr, 0);
    const unsigned L_master_up = strtoull(argv[9], nullptr, 0);
    const unsigned L_master_step = strtoull(argv[10], nullptr, 0);
    const unsigned L_local_low = strtoull(argv[11], nullptr, 0);
    const unsigned L_local_up = strtoull(argv[12], nullptr, 0);
    const unsigned L_local_step = strtoull(argv[13], nullptr, 0);
    const unsigned X_low = strtoull(argv[14], nullptr, 0);
    const unsigned X_up = strtoull(argv[15], nullptr, 0);
    const unsigned X_step = strtoull(argv[16], nullptr, 0);
    const unsigned I_thresh_low = strtoull(argv[17], nullptr, 0);
    const unsigned I_thresh_up = strtoull(argv[18], nullptr, 0);
    const unsigned I_thresh_step = strtoull(argv[19], nullptr, 0);
    const unsigned selected_query_id = strtoull(argv[20], nullptr, 0);
#ifdef DEBUG_PRINT
    printf("set num_threads: %d\n", num_threads);
#endif

    using PANNS::idi;
    using PANNS::distf;
    using PANNS::edgei;
    std::vector< std::vector< std::pair<idi, idi> > > shortest_path_list(K); // pair: <id, step>
    std::vector<bool> is_found_path(K, false);
    for (idi k_i = 0; k_i < K; ++k_i) {
        idi nn_id = true_nn_list[selected_query_id][k_i];
        is_found_path[k_i] = engine.get_mononic_shortest_path(
                nn_id,
                selected_query_id,
                shortest_path_list[k_i]);
//        engine.get_shortest_path(nn_id, shortest_path_list[k_i]);
//        // test
//        engine.check_shortest_path(nn_id, shortest_path_list[k_i]);
    }

    // Get all vertices and their step and distance
    std::map<idi, std::pair<idi, distf> > vertices_on_path; // pair: <step, distance>
    std::vector< std::set<idi> > path_edgelist(points_num);
    std::vector<bool> is_on_path(points_num, false);
    engine.get_vertices_on_path(
            shortest_path_list,
            K,
            selected_query_id,
            vertices_on_path,
            path_edgelist,
            is_on_path);
//    {//test nearest neighbor
//        // Get the longest distance in nearest neighbors
//        idi nn_id = true_nn_list[selected_query_id][0];
//        distf *query_data = engine.queries_load_ + selected_query_id * engine.dimension_;
//        distf *nn_data = engine.data_load_ + nn_id * engine.dimension_;
//        distf nn_max_dist = engine.compute_distance(nn_data, query_data);
//        idi nn_id_max_dist = nn_id;
//        for (idi nn_i = 1; nn_i < K; ++nn_i) {
//            nn_id = true_nn_list[selected_query_id][nn_i];
//            nn_data = engine.data_load_ + nn_id * engine.dimension_;
//            distf nn_dist = engine.compute_distance(nn_data, query_data);
//            if (nn_dist > nn_max_dist) {
//                nn_max_dist = nn_dist;
//                nn_id_max_dist = nn_id;
//            }
//        }
//
//        // Compare all vertices in paths
//        for (const auto &vert : vertices_on_path) {
//            idi v_id = vert.first;
//            const auto &true_nn = true_nn_list[selected_query_id];
//            if (std::find(true_nn.begin(), true_nn.end(), v_id) != true_nn.end()) {
//                // Is one of the nearest neighbors
//                continue;
//            }
//            distf v_dist = vert.second.second;
//            if (v_dist < nn_max_dist) {
//                printf("!!!! v_id: %u "
//                       "v_dist: %f "
//                       "nn_max_dist: %f "
//                       "nn_id: %u\n",
//                       v_id,
//                       v_dist,
//                       nn_max_dist,
//                       nn_id_max_dist);
//            }
//        }
//    }

    // Do BFS to all vertices with monotonic edges.
    std::vector<bool> is_reached(points_num, false);
    std::vector<idi> my_step(points_num, 0);
    std::vector<distf> my_distance(points_num, 0.0);
    std::vector< std::vector<idi> > all_mono_edgelist(points_num);
    engine.do_bfs_to_all_monotonic(
            selected_query_id,
            is_reached,
            my_step,
            my_distance,
            all_mono_edgelist);

    // Save to the file
    FILE *fout = fopen(path_results, "w");
    if (!fout) {
        fprintf(stderr, "Error: cannot create file %s.\n", path_results);
        exit(EXIT_FAILURE);
    }
    // Save kNN
    fprintf(fout, "%u\n", K);
    distf *query_data = engine.queries_load_ + selected_query_id * engine.dimension_;
    for (idi k_i = 0; k_i < K; ++k_i) {
        // Compute the distance
        idi nn_id = true_nn_list[selected_query_id][k_i];
        distf *nn_data = engine.data_load_ + nn_id * engine.dimension_;
        distf nn_dist = engine.compute_distance(nn_data, query_data);
        fprintf(fout, "%u\t%f\n", true_nn_list[selected_query_id][k_i], nn_dist);
    }
    // Save points on shortest paths
    idi total_points_on_paths = vertices_on_path.size();
    fprintf(fout, "%u\n", total_points_on_paths);
    for (const auto &vert : vertices_on_path) {
        idi v_id = vert.first;
        idi v_step = vert.second.first;
        distf v_dist = vert.second.second;
        fprintf(fout, "%u\t%u\t%f\n", v_id, v_step, v_dist);
    }
    // Save other points with monotonic edges
    idi total_points_all_monotonic = get_num_points_exclude_on_paths(
                                                            is_reached,
                                                            points_num,
                                                            total_points_on_paths);
    fprintf(fout, "%u\n", total_points_all_monotonic);
    for (idi v_i = 0; v_i < points_num; ++v_i) {
        if (!is_reached[v_i]) {
            continue;
        }
        // Exclude core points
        if (vertices_on_path.find(v_i) != vertices_on_path.end()) {
            continue;
        }
        idi v_id = v_i;
        idi v_step = my_step[v_i];
        distf v_dist = my_distance[v_i];
        fprintf(fout, "%u\t%u\t%f\n", v_id, v_step, v_dist);
    }

    // Save arrows
    // Get number of arrows
    edgei total_arrows = 0;
    for (const auto &edges : all_mono_edgelist) {
        total_arrows += edges.size();
    }
    fprintf(fout, "%lu\n", total_arrows);
    for (idi v_i = 0; v_i < points_num; ++v_i) {
        const auto &edges = all_mono_edgelist[v_i];
        if (edges.empty()) {
            continue;
        }
        idi v_step = my_step[v_i];
        distf v_dist = my_distance[v_i];

        for (idi e_i : edges) {
            idi e_step = my_step[e_i];
            distf e_dist = my_distance[e_i];
            fprintf(fout,
                    "%u\t"
                    "%f\t"
                    "%u\t"
                    "%f\n",
                    v_step,
                    v_dist,
                    e_step,
                    e_dist);
        }
    }
//    for (const auto &edges : path_edgelist) {
//        total_arrows += edges.size();
//    }
//    fprintf(fout, "%lu\n", total_arrows);
//    for (idi v_i = 0; v_i < points_num; ++v_i) {
//        const auto &edges = path_edgelist[v_i];
//        if (edges.empty()) {
//            continue;
//        }
//        idi v_step = vertices_on_path[v_i].first;
//        distf v_dist = vertices_on_path[v_i].second;
//
//        for (idi e_i : edges) {
//            idi e_step = vertices_on_path[e_i].first;
//            distf e_dist = vertices_on_path[e_i].second;
//            fprintf(fout,
//                    "%u\t"
//                    "%f\t"
//                    "%u\t"
//                    "%f\n",
//                    v_step,
//                    v_dist,
//                    e_step,
//                    e_dist);
//        }
//    }
    fclose(fout);

    {
        idi cnt_not_found = 0;
        for (idi k_i = 0; k_i < K; ++k_i) {
            if (is_found_path[k_i]) {
                continue;
            }
            ++cnt_not_found;
        }
        printf("num_not_found: %u\n", cnt_not_found);
    }

    return 0;
}
