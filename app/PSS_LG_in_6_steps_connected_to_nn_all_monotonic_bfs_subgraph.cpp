//
// Created by Zhen Peng on 06/23/2020.
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

void get_reversed_edgelist(
        const std::vector< std::vector<PANNS::idi> > &f_edgelist,
        const PANNS::idi num_v,
        std::vector< std::vector<PANNS::idi> > &r_edgelist)
{
    for (PANNS::idi v_i = 0; v_i < num_v; ++v_i) {
        const auto &edges = f_edgelist[v_i];
        if (edges.empty()) {
            continue;
        }
        {//test
            if (v_i == 58507 || v_i == 840857) {
                printf("!! v_i: %u\n", v_i);
            }
        }
        for (const auto e_i : edges) {
            r_edgelist[e_i].push_back(v_i);
            {//test
                if (e_i == 58507 || e_i == 840857) {
                    printf("!! e_i: %u\n", v_i);
                }
            }
        }
    }
}

void do_bfs_from_nn_to_all_monotonic(
        const std::vector<PANNS::idi> &true_nn,
        const std::vector< std::vector<PANNS::idi> > &r_edgelist,
        const std::vector<bool> &is_reached,
        const PANNS::idi num_v,
//        std::vector<PANNS::idi> &my_step,
//        const std::vector<PANNS::distf> &my_distance, // for test
        std::vector<bool> &is_connected_to_nn)
{
    using PANNS::idi;
    std::vector<idi> queue(num_v);
    idi queue_head = 0;
    idi queue_end = 0;
//    std::vector<bool> is_visited(num_v, false);
//    std::vector<idi> r_steps(num_v, 0);
    // test
    std::vector<idi> my_parent(num_v);
    // end test
    for (idi nn : true_nn) {
        if (!is_reached[nn]) {
            continue;
        }
        queue[queue_end++] = nn;
//        is_visited[nn] = true;
        is_connected_to_nn[nn] = true;

        my_parent[nn] = nn;
    }

    while (queue_end > queue_head) {
        idi v_id = queue[queue_head++];
//        idi v_step = my_step[v_id];
        const auto &edges = r_edgelist[v_id];
        for (const auto nb_id : edges) {
//            if (is_visited[nb_id]) {
//                continue;
//            }
//            is_visited[nb_id] = true;
            if (is_connected_to_nn[nb_id]) {
                continue;
            }
            is_connected_to_nn[nb_id] = true;
            queue[queue_end++] = nb_id;
//            my_step[nb_id] = v_step - 1;
            my_parent[nb_id] = v_id;
        }
    }

//    {//test
//        idi cur_id = 640961;
//        idi par_id = my_parent[640961];
//        while (cur_id != par_id) {
//            printf("%u,%f -> %u,%f\n",
//                   par_id, my_distance[par_id],
//                   cur_id, my_distance[cur_id]);
//            cur_id = par_id;
//            par_id = my_parent[cur_id];
//        }
//    }
}

void get_vertices_connected_to_source_and_nn(
        const std::vector<bool> &is_reached, // vertices that connected to source
        const std::vector<bool> &is_connected_to_nn, // vertices that connected to nn
        const PANNS::idi num_v,
        std::vector<bool> &is_two_ends_connected)
{
    for (PANNS::idi v_id = 0; v_id < num_v; ++v_id) {
        if (is_reached[v_id] && is_connected_to_nn[v_id]) {
            is_two_ends_connected[v_id] = true;
        }
    }
}

void usage(int argc, char *argv[])
{
    if (argc != 22) {
        fprintf(stderr,
                "Usage: %s <data_file> <query_file> <nsg_path> "
                "<K> <result_file> <true_NN_file> <num_threads> "
                "<L_master_low> <L_master_up> <L_master_step> <L_local_low> "
                "<L_local_up> <L_local_step> <X_low> <X_up> "
                "<X_step> <I_thresh_low> <I_thresh_up> <I_thresh_step> "
                "<query_id> <step_bound>\n",
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
    const unsigned step_bound = strtoull(argv[21], nullptr, 0);
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
    {
        printf("Getting core vertices...\n");
        std::vector<std::set<idi> > path_edgelist(points_num);
        std::vector<bool> is_on_path(points_num, false);
        engine.get_vertices_on_path(
                shortest_path_list,
                K,
                selected_query_id,
                vertices_on_path,
                path_edgelist,
                is_on_path);
    }


    // Do BFS to all vertices with monotonic edges.
//    std::vector<bool> is_two_ends_connected(points_num, false);
    std::vector<idi> my_step(points_num, 0);
    std::vector<distf> my_distance(points_num, 0.0);
    std::vector<bool> is_connected_to_nn(points_num, false);
    {
        printf("BFS from source to all...\n");
        std::vector<bool> is_reached(points_num, false);
//    std::vector<idi> my_step(points_num, 0);
//    std::vector<distf> my_distance(points_num, 0.0);
        std::vector<std::vector<idi> > all_mono_edgelist(points_num);
        engine.do_bfs_to_all_monotonic_in_some_steps(
                selected_query_id,
                step_bound,
                is_reached,
                my_step,
                my_distance,
                all_mono_edgelist);
//        {//test
//            std::cout << "is_reached[640961]: " << is_reached[640961] << std::endl;
//            std::cout << "is_reached[735238]: " << is_reached[735238] << std::endl;
//        }

        // Get reverse edgelist
        printf("Getting reversed monotonic edges...\n");
        std::vector<std::vector<idi> > r_edgelist(points_num);
        engine.get_reversed_edgelist(
                selected_query_id,
                is_reached,
//                my_distance,
                r_edgelist);
//        get_reversed_edgelist(all_mono_edgelist, points_num, r_edgelist);

        // Get the visiting map of vertices that connected to nearest neighbors
        printf("BFS from NN using reversed monotonic edges...\n");
//        std::vector<bool> is_connected_to_nn(points_num, false);
        do_bfs_from_nn_to_all_monotonic(
                true_nn_list[selected_query_id],
                r_edgelist,
                is_reached,
                points_num,
//                my_step,
//                my_distance, // for test
                is_connected_to_nn);

//        {//test
//            std::cout << "is_connected_to_nn[640961]: " << is_connected_to_nn[640961] << std::endl;
//            std::cout << "is_connected_to_nn[735238]: " << is_connected_to_nn[735238] << std::endl;
//        }
//        // Merge the vertices connected to the source and to the nearest neighbors
//        printf("Merging vertices connected to source and nn...\n");
//        get_vertices_connected_to_source_and_nn(
//                is_reached,
//                is_connected_to_nn,
//                points_num,
//                is_two_ends_connected);


    }
    // Get the edgelist that connected to nearest neighbors
    std::vector< std::vector<idi> > connected_to_nn_edgelist(points_num);
    printf("Getting arrow list for painting...\n");
    engine.get_connected_to_nn_edgelist(
//            is_two_ends_connected,
            is_connected_to_nn,
            my_distance,
            connected_to_nn_edgelist);

//    printf("BFS again to get steps...\n");
//    engine.do_bfs_from_source_to_two_ends_connected(
//            connected_to_nn_edgelist,
//            my_step);

    // Save to the file
    printf("Saving...\n");
    FILE *fout = fopen(path_results, "w");
    if (!fout) {
        fprintf(stderr, "Error: cannot create file %s.\n", path_results);
        exit(EXIT_FAILURE);
    }
    // Save kNN
    fprintf(fout, "%u\n", K);
    printf("K: %u\n", K);
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
    printf("num_core: %u\n", total_points_on_paths);
    fprintf(fout, "%u\n", total_points_on_paths);
    for (const auto &vert : vertices_on_path) {
        idi v_id = vert.first;
        idi v_step = vert.second.first;
        distf v_dist = vert.second.second;
        fprintf(fout, "%u\t%u\t%f\n", v_id, v_step, v_dist);
    }
    // Save other points with monotonic edges
    idi total_points_all_monotonic = get_num_points_exclude_on_paths(
//                                                            is_reached,
                                                            is_connected_to_nn,
//                                                            is_two_ends_connected,
                                                            points_num,
                                                            total_points_on_paths);
    fprintf(fout, "%u\n", total_points_all_monotonic);
    printf("num_not-core: %u\n", total_points_all_monotonic);
    for (idi v_i = 0; v_i < points_num; ++v_i) {
//        if (!is_two_ends_connected[v_i]) {
//            continue;
//        }
        if (!is_connected_to_nn[v_i]) {
            continue;
        }
//        if (!is_reached[v_i]) {
//            continue;
//        }
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
    for (const auto &edges : connected_to_nn_edgelist) {
        total_arrows += edges.size();
    }
    fprintf(fout, "%lu\n", total_arrows);
    printf("num_edges: %lu\n", total_arrows);
    for (idi v_i = 0; v_i < points_num; ++v_i) {
        const auto &edges = connected_to_nn_edgelist[v_i];
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
