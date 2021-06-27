//
// Created by Zhen Peng on 06/26/2020.
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
#include "../core/Searching.202106261435.PSS_LG_bfs_parallel_path_2_steps_dist_prune.h"
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

void do_PSS_parallel_search(
        PANNS::Searching &engine,
        const unsigned K,
        const unsigned points_num,
        const unsigned query_num,
        const int num_threads,
        const unsigned L_master_low,
        const unsigned L_master_up,
        const unsigned L_master_step,
        const unsigned X_low,
        const unsigned X_up,
        const unsigned X_step,
        const unsigned selected_query_id,
        std::vector< std::vector<bool> > &is_search_expanded,
        std::vector<PANNS::idi> &my_step,
        std::vector<PANNS::distf> &my_distance,
        std::vector<PANNS::idi> &my_parent)
{
    for (unsigned L_master = L_master_low; L_master <= L_master_up; L_master += L_master_step) {
        unsigned L_local = L_master;
//        unsigned Index_thresh = L_local - 1;
//        for (unsigned L_local = L_local_low; L_local <= L_local_up; L_local += L_local_step) {

        for (unsigned subsearch_iterations = X_low; subsearch_iterations <= X_up; subsearch_iterations += X_step) {

//                for (unsigned Index_thresh = I_thresh_low; Index_thresh <= I_thresh_up; Index_thresh += I_thresh_step) {
//            engine.index_thresh_ = Index_thresh;
            unsigned warmup_max = 1;
            for (unsigned warmup = 0; warmup < warmup_max; ++warmup) {
                std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
                for (unsigned i = 0; i < query_num; i++) set_K_list[i].resize(K);

                std::vector<PANNS::idi> init_ids(L_master);
                std::vector<uint8_t> is_visited(points_num, 0);
//                boost::dynamic_bitset<> is_visited(points_num);
                std::vector<PANNS::Candidate> set_L((num_threads - 1) * L_local + L_master);
                std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
                std::vector<PANNS::idi> local_queues_starts(num_threads);
                for (int q_i = 0; q_i < num_threads; ++q_i) {
                    local_queues_starts[q_i] = q_i * L_local;
                }
//                std::vector<PANNS::idi> top_m_candidates(num_threads);
                auto s = std::chrono::high_resolution_clock::now();
//                engine.prepare_init_ids(init_ids, L_local);
                PANNS::idi cnt_init_ids = engine.prepare_init_ids_only_neighbors(
                                                            selected_query_id,
                                                            init_ids,
                                                            L_master);
                {//
                    printf("cnt_init_ids: %u\n", cnt_init_ids);
                }
                for (unsigned q_i = 0; q_i < query_num; ++q_i) {
#ifdef DEBUG_PRINT
                    printf("query_id: %u\n", q_i);
#endif
                    if (q_i != selected_query_id) {
                        continue;
                    }
                    engine.PSS_parallel_expanded_vertices_w_2_steps_dist_prune(
                            q_i,
                            K,
                            L_master,
                            set_L,
                            init_ids,
                            cnt_init_ids,
                            set_K_list[q_i],
                            L_local,
                            local_queues_starts,
                            local_queues_sizes,
                            is_visited,
                            subsearch_iterations,
                            is_search_expanded,
                            my_step,
                            my_distance,
                            my_parent);
                }
                auto e = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = e - s;
                std::unordered_map<unsigned, double> recalls;
//                { // Recall values
//                    engine.get_recall_for_all_queries(
//                            true_nn_list,
//                            set_K_list,
//                            recalls,
//                            L_master);
//                }
//                {// Basic output
//                    printf(
//                            "num_threads: %d "
//                            "L: %u "
//                            "runtime(s.): %f "
//                            "computation: %lu "
//                            "K: %u "
//                            "Volume: %u "
//                            "Dimension: %u "
//                            "query_num: %u "
//                            "query_per_sec: %f "
//                            "avg_latency(ms.): %f "
//                            "P@100: %f "
//                            "P@1: %f "
//                            "G/s: %f "
//                            "GFLOPS: %f "
//                            "local_L: %u "
//                            "sub_iters: %u "
//                            "index_thresh: %u "
//                            "avg_merge: %f "
//                            "t_expand(s.): %f "
//                            "t_merge(s.): %f "
//                            "t_seq(s.): %f "
//                            "t_p_expand(%%): %f "
//                            "t_p_merge(%%): %f "
//                            "t_p_seq(%%): %f ",
//                            num_threads,
//                            L_master,
//                            diff.count(),
//                            engine.count_distance_computation_,
//                            K,
//                            points_num,
//                            data_dimension,
//                            query_num,
//                            query_num / diff.count(),
//                            diff.count() * 1000 / query_num,
//                            recalls[100],
//                            recalls[1],
//                            data_dimension * 4.0 * engine.count_distance_computation_ / (1U << 30U) /
//                            diff.count(),
//                            data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ /
//                            (1U << 30U) /
//                            diff.count(),
//                            L_local,
//                            subsearch_iterations,
//                            engine.index_thresh_,
//                            engine.count_merge_ * 1.0 / query_num,
//                            engine.time_expand_,
//                            engine.time_merge_,
//                            engine.time_seq_,
////                            engine.time_pick_,
//                            engine.time_expand_ / diff.count() * 100.0,
//                            engine.time_merge_ / diff.count() * 100.0,
//                            engine.time_seq_ / diff.count() * 100.0);
////                            engine.time_pick_ / diff.count() * 100.0);
//                    printf("\n");
//                }
                engine.count_distance_computation_ = 0;
//                engine.count_iterations_ = 0;
//                    engine.count_checked_ = 0;
                engine.count_merge_ = 0;
                engine.time_expand_ = 0.0;
                engine.time_merge_ = 0.0;
                engine.time_seq_ = 0.0;
//                engine.time_pick_ = 0.0;
//                PANNS::DiskIO::save_result(path_results, set_K_list);
            }
//                } // Index_threshold Ranged
        } // X Ranged
//        } // L_local ranged
    } // L_master ranged
}

PANNS::idi filter_out_non_subgraph_vertices(
        const int num_threads,
        const PANNS::idi num_v,
        const std::vector<bool> &is_connected_to_nn,
        std::vector< std::vector<bool> > &is_search_expanded)
{
    PANNS::idi cnt_filtered_out = 0;
    for (int th_id = 0; th_id < num_threads; ++th_id) {
        auto &is_expanded = is_search_expanded[th_id];
        for (PANNS::idi v_id = 0; v_id < num_v; ++v_id) {
            if (is_expanded[v_id] && !is_connected_to_nn[v_id]) {
                is_expanded[v_id] = false;
                ++cnt_filtered_out;
            }
        }
    }

    return cnt_filtered_out;
}

PANNS::idi get_united_set_of_expanded_vertices(
        const PANNS::idi num_v,
        const int num_threads,
        const std::vector< std::vector<bool> > &is_search_expanded,
        std::vector<bool> &is_in_united_set)
{
    PANNS::idi num_in = 0;
    for (PANNS::idi v_id = 0; v_id < num_v; ++v_id) {
        bool is_in = true;
        for (int th_id = 0; th_id < num_threads; ++th_id) {
            is_in &= is_search_expanded[th_id][v_id];
        }
        if (is_in) {
            ++num_in;
            is_in_united_set[v_id] = is_in;
        }
    }

    return num_in;
}

PANNS::idi get_num_search_expanded_vertices(
        const PANNS::idi num_v,
        const std::vector<bool> &is_expanded)
{
    PANNS::idi num_in = 0;
    for (PANNS::idi v_id = 0; v_id < num_v; ++v_id) {
        if (!is_expanded[v_id]) {
            continue;
        }
        ++num_in;
    }

    return num_in;
}

uint64_t get_remained_edges_to_be_plotted(
        const PANNS::idi num_v,
        const int num_threads,
//        const std::vector<bool> &is_connected_to_nn,
        const std::vector< std::vector<bool> > &is_search_expanded,
        const std::vector<PANNS::idi> &my_parent,
        std::vector< std::set<PANNS::idi> > &plot_edgelist)
{
    // Need to combine those visited vertices at first.
    std::vector<bool> is_visited(num_v, false);
    for (int th_id = 0; th_id < num_threads; ++th_id) {
        auto &is_expanded = is_search_expanded[th_id];
        for (PANNS::idi v_id = 0; v_id < num_v; ++v_id) {
            if (is_expanded[v_id] && !is_visited[v_id]) {
                is_visited[v_id] = true;
            }
        }
    }
    for (int th_id = 0; th_id < num_threads; ++th_id) {
        for (PANNS::idi v_id = 0; v_id < num_v; ++v_id) {
            if (!is_visited[v_id]) {
                continue;
            }
            PANNS::idi parent_id = my_parent[v_id];
            if (!is_visited[parent_id]) {
                continue;
            }
            plot_edgelist[parent_id].insert(v_id);
        }
    }

//    for (int th_id = 0; th_id < num_threads; ++th_id) {
//        auto &is_expanded = is_search_expanded[th_id];
//        for (PANNS::idi v_id = 0; v_id < num_v; ++v_id) {
//            if (!is_expanded[v_id]) {
//                continue;
//            }
////            if (!is_expanded[v_id] || !is_connected_to_nn[v_id]) {
////                continue;
////            }
//            PANNS::idi parent_id = my_parent[v_id];
////            if (!is_connected_to_nn[parent_id]) {
////                continue;
////            }
////            if (!is_expanded[parent_id]) {
////                continue;
////            }
//            plot_edgelist[parent_id].insert(v_id);
//        }
//    }

    uint64_t num_remained_edges = 0;
    for (PANNS::idi v_id = 0; v_id < num_v; ++v_id) {
        num_remained_edges += plot_edgelist[v_id].size();
    }

    return num_remained_edges;
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
//    std::vector<bool> is_connected_to_nn(points_num, false);
//    {
//        printf("BFS from source to all...\n");
//        std::vector<bool> is_reached(points_num, false);
////    std::vector<idi> my_step(points_num, 0);
////    std::vector<distf> my_distance(points_num, 0.0);
//        std::vector<std::vector<idi> > all_mono_edgelist(points_num);
//        engine.do_bfs_to_all_monotonic(
//                selected_query_id,
//                is_reached,
//                my_step,
//                my_distance,
//                all_mono_edgelist);
////        {//test
////            std::cout << "is_reached[640961]: " << is_reached[640961] << std::endl;
////            std::cout << "is_reached[735238]: " << is_reached[735238] << std::endl;
////        }
//
//        // Get reverse edgelist
//        printf("Getting reversed monotonic edges...\n");
//        std::vector<std::vector<idi> > r_edgelist(points_num);
//        engine.get_reversed_edgelist(
//                selected_query_id,
//                is_reached,
////                my_distance,
//                r_edgelist);
////        get_reversed_edgelist(all_mono_edgelist, points_num, r_edgelist);
//
//        // Get the visiting map of vertices that connected to nearest neighbors
//        printf("BFS from NN using reversed monotonic edges...\n");
////        std::vector<bool> is_connected_to_nn(points_num, false);
//        do_bfs_from_nn_to_all_monotonic(
//                true_nn_list[selected_query_id],
//                r_edgelist,
//                is_reached,
//                points_num,
////                my_step,
////                my_distance, // for test
//                is_connected_to_nn);
//
////        {//test
////            std::cout << "is_connected_to_nn[640961]: " << is_connected_to_nn[640961] << std::endl;
////            std::cout << "is_connected_to_nn[735238]: " << is_connected_to_nn[735238] << std::endl;
////        }
////        // Merge the vertices connected to the source and to the nearest neighbors
////        printf("Merging vertices connected to source and nn...\n");
////        get_vertices_connected_to_source_and_nn(
////                is_reached,
////                is_connected_to_nn,
////                points_num,
////                is_two_ends_connected);
//
//
//    }
//    // Get the edgelist that connected to nearest neighbors
//    std::vector< std::vector<idi> > connected_to_nn_edgelist(points_num);
//    printf("Getting arrow list for painting...\n");
//    engine.get_connected_to_nn_edgelist(
////            is_two_ends_connected,
//            is_connected_to_nn,
//            my_distance,
//            connected_to_nn_edgelist);
//
////    printf("BFS again to get steps...\n");
////    engine.do_bfs_from_source_to_two_ends_connected(
////            connected_to_nn_edgelist,
////            my_step);

    // Do PSS parallel search
    printf("PSS searching...\n");
    std::vector< std::vector<bool> > is_search_expanded(num_threads, std::vector<bool>(points_num, false));
    std::vector<PANNS::idi> my_parent(points_num);
    do_PSS_parallel_search(
            engine,
            K,
            points_num,
            query_num,
            num_threads,
            L_master_low,
            L_master_up,
            L_master_step,
            X_low,
            X_up,
            X_step,
            selected_query_id,
            is_search_expanded,
            my_step,
            my_distance,
            my_parent);

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

    // Filter out non-subgraph vertices in the expanded map
    idi cnt_filtered_out = 0;
//    cnt_filtered_out = filter_out_non_subgraph_vertices(
//            num_threads,
//            points_num,
//            is_connected_to_nn,
//            is_search_expanded);

    // Get remained edges to be plotted
    std::vector< std::set<PANNS::idi> > remained_edgelist(points_num);
    uint64_t num_remained_edges = get_remained_edges_to_be_plotted(
            points_num,
            num_threads,
//            is_connected_to_nn,
            is_search_expanded,
            my_parent,
            remained_edgelist);

//    // Get united set of expanded vertices
//    std::vector<bool> is_in_united_set(points_num, false);
//    idi num_in_united_set = get_united_set_of_expanded_vertices(
//            points_num,
//            num_threads,
//            is_search_expanded,
//            is_in_united_set);
//    fprintf(fout, "%u\n", num_in_united_set);
//    printf("num_in_united_set: %u\n", num_in_united_set);
//    // Save united set
//    for (PANNS::idi v_id = 0; v_id < points_num; ++v_id) {
//        if (!is_in_united_set[v_id]) {
//            continue;
//        }
//        idi v_step = my_step[v_id];
//        distf v_dist = my_distance[v_id];
//        fprintf(fout, "%u\t%u\t%f\n", v_id, v_step, v_dist);
//    }

    // Save every thread's expanded vertices
    fprintf(fout, "%d\n", num_threads);
    printf("num_threads: %d\n", num_threads);
    for (int th_id = 0; th_id < num_threads; ++th_id) {
        const auto &is_expanded = is_search_expanded[th_id];
        idi num_expanded = get_num_search_expanded_vertices(
                                        points_num,
                                        is_expanded);
        fprintf(fout, "%u\n", num_expanded);
        printf("th_id: %d num_expanded: %u\n", th_id, num_expanded);
        for (PANNS::idi v_id = 0; v_id < points_num; ++v_id) {
            if (!is_expanded[v_id]) {
                continue;
            }
            idi v_step = my_step[v_id];
            distf v_dist = my_distance[v_id];
            fprintf(fout, "%u\t%u\t%f\n", v_id, v_step, v_dist);
        }
    }

    // Save arrows
    fprintf(fout, "%lu\n", num_remained_edges);
    printf("num_remained_edges: %lu\n", num_remained_edges);
    for (idi v_i = 0; v_i < points_num; ++v_i) {
        const auto &edges = remained_edgelist[v_i];
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

    fprintf(fout, "%u\n", cnt_filtered_out);
    printf("cnt_filtered_out: %u\n", cnt_filtered_out);



//    // Save kNN
//    fprintf(fout, "%u\n", K);
//    printf("K: %u\n", K);
//    distf *query_data = engine.queries_load_ + selected_query_id * engine.dimension_;
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        // Compute the distance
//        idi nn_id = true_nn_list[selected_query_id][k_i];
//        distf *nn_data = engine.data_load_ + nn_id * engine.dimension_;
//        distf nn_dist = engine.compute_distance(nn_data, query_data);
//        fprintf(fout, "%u\t%f\n", true_nn_list[selected_query_id][k_i], nn_dist);
//    }
//    // Save points on shortest paths
//    idi total_points_on_paths = vertices_on_path.size();
//    printf("num_core: %u\n", total_points_on_paths);
//    fprintf(fout, "%u\n", total_points_on_paths);
//    for (const auto &vert : vertices_on_path) {
//        idi v_id = vert.first;
//        idi v_step = vert.second.first;
//        distf v_dist = vert.second.second;
//        fprintf(fout, "%u\t%u\t%f\n", v_id, v_step, v_dist);
//    }
//    // Save other points with monotonic edges
//    idi total_points_all_monotonic = get_num_points_exclude_on_paths(
////                                                            is_reached,
//                                                            is_connected_to_nn,
////                                                            is_two_ends_connected,
//                                                            points_num,
//                                                            total_points_on_paths);
//    fprintf(fout, "%u\n", total_points_all_monotonic);
//    printf("num_not-core: %u\n", total_points_all_monotonic);
//    for (idi v_i = 0; v_i < points_num; ++v_i) {
////        if (!is_two_ends_connected[v_i]) {
////            continue;
////        }
//        if (!is_connected_to_nn[v_i]) {
//            continue;
//        }
////        if (!is_reached[v_i]) {
////            continue;
////        }
//        // Exclude core points
//        if (vertices_on_path.find(v_i) != vertices_on_path.end()) {
//            continue;
//        }
//        idi v_id = v_i;
//        idi v_step = my_step[v_i];
//        distf v_dist = my_distance[v_i];
//        fprintf(fout, "%u\t%u\t%f\n", v_id, v_step, v_dist);
//    }
//
//    // Save arrows
//    // Get number of arrows
//    edgei total_arrows = 0;
//    for (const auto &edges : connected_to_nn_edgelist) {
//        total_arrows += edges.size();
//    }
//    fprintf(fout, "%lu\n", total_arrows);
//    printf("num_edges: %lu\n", total_arrows);
//    for (idi v_i = 0; v_i < points_num; ++v_i) {
//        const auto &edges = connected_to_nn_edgelist[v_i];
//        if (edges.empty()) {
//            continue;
//        }
//        idi v_step = my_step[v_i];
//        distf v_dist = my_distance[v_i];
//
//        for (idi e_i : edges) {
//            idi e_step = my_step[e_i];
//            distf e_dist = my_distance[e_i];
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
