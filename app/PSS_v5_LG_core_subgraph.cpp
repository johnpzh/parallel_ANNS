//
// Created by Zhen Peng on 05/30/2020.
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
#include "../core/Searching.202105302215.PSS_v5_LG_core_subgraph.h"
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

void save_set_K(
        const std::vector<PANNS::idi> &set_K,
        const PANNS::idi K,
        const char *filename,
        const std::vector<PANNS::idi> &old_to_new)
{
    FILE *fout = fopen(filename, "w");
    if (!fout) {
        fprintf(stderr, "Error: cannot create file %s .\n", filename);
        exit(EXIT_FAILURE);
    }
    for (PANNS::idi k_i = 0; k_i < K; ++k_i) {
        fprintf(fout, "%u\n", old_to_new[set_K[k_i]]);
    }
    fclose(fout);
}

void get_refined_edgelist(
        const PANNS::Searching &engine,
        const PANNS::idi selected_query_id,
        const std::vector<char> &is_in_core,
        std::vector< std::vector< std::pair<PANNS::idi, PANNS::distf> > > &f_edgelist)
{
    PANNS::idi points_num = engine.num_v_;
    PANNS::dataf *query_data = engine.queries_load_ + selected_query_id * engine.dimension_;
    for (PANNS::idi v_id = 0; v_id < points_num; ++v_id) {
        if (!is_in_core[v_id]) {
            continue;
        }
        PANNS::dataf *v_data = engine.data_load_ + v_id * engine.dimension_;
        PANNS::distf v_dist = engine.compute_distance(v_data, query_data);
        PANNS::idi *out_edges = engine.common_nsg_deg_ngbrs_ + engine.common_nsg_vertex_base_[v_id];
        PANNS::idi out_degree = *out_edges++;
        for (PANNS::idi e_i = 0; e_i < out_degree; ++e_i) {
            PANNS::idi nb_id = out_edges[e_i];
            if (!is_in_core[nb_id]) {
                continue;
            }
            PANNS::dataf *nb_data = engine.data_load_ + nb_id * engine.dimension_;
            PANNS::distf nb_dist = engine.compute_distance(nb_data, query_data);
            if (nb_dist >= v_dist) {
                continue;
            }
            PANNS::distf edge_weight = engine.compute_distance(v_data, nb_data);
            f_edgelist[v_id].emplace_back(nb_id, edge_weight);
        }
    }
}

void get_vid_old_to_new_map(
        const std::vector< std::vector< std::pair<PANNS::idi, PANNS::distf> > > &edgelist,
        const PANNS::idi points_num,
        std::vector<PANNS::idi> &old_to_new,
        PANNS::idi &sb_num_v,
        PANNS::edgei &sb_num_e)
{
    std::vector<bool> is_in_refined(points_num, false);
    for (PANNS::idi v_id = 0; v_id < points_num; ++v_id) {
        if (edgelist[v_id].empty()) {
            continue;
        }
        if (!is_in_refined[v_id]) {
            is_in_refined[v_id] = true;
            old_to_new[v_id] = sb_num_v++;
        }
        for (const auto &edge : edgelist[v_id]) {
            ++sb_num_e;
            PANNS::idi e_id = edge.first;
            if (!is_in_refined[e_id]) {
                is_in_refined[e_id] = true;
                old_to_new[e_id] = sb_num_v++;
            }
        }
    }
}

void save_subgraph(
        const char *path_results,
        const std::vector< std::vector< std::pair<PANNS::idi, PANNS::distf> > > &edgelist,
        const PANNS::idi points_num,
        const std::vector<PANNS::idi> &old_to_new,
        const PANNS::idi sb_num_v)
{
    FILE *fout = fopen(path_results, "w");
    if (!fout) {
        fprintf(stderr, "Error: cannot create file %s .\n", path_results);
        exit(EXIT_FAILURE);
    }
    PANNS::idi tmp_num_v = 0;
    for (PANNS::idi v_id = 0; v_id < points_num; ++v_id) {
        const auto &edges = edgelist[v_id];
        if (edges.empty()) {
            continue;
        }
        PANNS::idi new_v = old_to_new[v_id];
        tmp_num_v = std::max(new_v + 1, tmp_num_v);
        for (const auto &e_i : edges) {
            PANNS::idi e_id = e_i.first;
            PANNS::distf dist = e_i.second;
            PANNS::idi new_e = old_to_new[e_id];
            tmp_num_v = std::max(new_e + 1, tmp_num_v);
            fprintf(fout, "%u\t%u\t%f\n", new_v, new_e, dist);
        }
    }
    if (tmp_num_v != sb_num_v) {
        fprintf(stderr, "Error: tmp_num_v (%u) is not equal to sb_num_v (%u).\n", tmp_num_v, sb_num_v);
        exit(EXIT_FAILURE);
    }

    fclose(fout);
}

void get_reverse_edgelist(
        const std::vector< std::vector< std::pair<PANNS::idi, PANNS::distf> > > &edgelist,
        const PANNS::idi points_num,
        const std::vector<PANNS::idi> &old_to_new,
        std::vector< std::vector< std::pair<PANNS::idi, PANNS::distf> > > &r_edgelist)
{
    for (PANNS::idi v_id = 0; v_id < points_num; ++v_id) {
        const auto &edges = edgelist[v_id];
        if (edges.empty()) {
            continue;
        }
        PANNS::idi new_v = old_to_new[v_id];
        for (const auto &e_i : edges) {
            PANNS::idi e_id = e_i.first;
            PANNS::distf dist = e_i.second;
            PANNS::idi new_e = old_to_new[e_id];

            r_edgelist[new_e].emplace_back(new_v, dist);
        }
    }
}

void get_connected_vertices(
        const std::vector< std::vector< std::pair<PANNS::idi, PANNS::distf> > > &r_edgelist,
        const PANNS::idi sb_num_v,
        const std::vector<PANNS::idi> &set_K,
        const std::vector<PANNS::idi> &old_to_new,
        std::vector<bool> &is_connected_to_finals)
{
    std::vector<PANNS::idi> queue(sb_num_v);
    PANNS::idi queue_head = 0;
    PANNS::idi queue_end = 0;

    for (PANNS::idi k_i : set_K) {
        PANNS::idi new_k = old_to_new[k_i];
        queue[queue_end++] = new_k;
        is_connected_to_finals[new_k] = true;
    }

    while (queue_end > queue_head) {
        PANNS::idi v_id = queue[queue_head++];
        const auto &edges = r_edgelist[v_id];
        for (const auto &e_i : edges) {
            PANNS::idi nb_id = e_i.first;
//            PANNS::distf nb_dist = e_i.second;
            if (is_connected_to_finals[nb_id]) {
                continue;
            }
            is_connected_to_finals[nb_id] = true;
            queue[queue_end++] = nb_id;
        }
    }
}

void get_connected_edgelist(
        const std::vector< std::vector< std::pair<PANNS::idi, PANNS::distf> > > &r_edgelist,
        const PANNS::idi sb_num_v,
        const std::vector<bool> &is_connected_to_finals,
        std::vector< std::vector< std::pair<PANNS::idi, PANNS::distf> > > &c_edgelist,
        PANNS::idi &c_num_v,
        PANNS::edgei &c_num_e,
        const std::string &connected_filename,
        const std::vector<PANNS::idi> &f_old_to_new,
        const std::vector<PANNS::idi> &set_K,
        const PANNS::idi K)
{
//    std::vector<bool> is_connected_to_finals(sb_num_v, false);
    std::vector<PANNS::idi> old_to_new(sb_num_v);
    c_num_v = 0;
    c_num_e = 0;
    for (PANNS::idi old_i = 0; old_i < sb_num_v; ++old_i) {
        if (!is_connected_to_finals[old_i]) {
            continue;
        }
        old_to_new[old_i] = c_num_v++;
    }
    c_edgelist.resize(c_num_v);

    for (PANNS::idi v_i = 0; v_i < sb_num_v; ++v_i) {
        if (!is_connected_to_finals[v_i]) {
            continue;
        }
        PANNS::idi new_v = old_to_new[v_i];
        const auto &edges = r_edgelist[v_i];
        for (const auto &e_i : edges) {
            PANNS::idi nb_id = e_i.first;
            PANNS::distf nb_dist = e_i.second;
            if (!is_connected_to_finals[nb_id]) {
                continue;
            }
            ++c_num_e;
            PANNS::idi new_nb_id = old_to_new[nb_id];
            c_edgelist[new_nb_id].emplace_back(new_v, nb_dist);
//            is_connected_to_finals[nb_id] = true;
//            queue[queue_end++] = nb_id;
//            {//
//                printf("v_i: %u "
//                       "new_v: %u "
//                       "nb_id: %u "
//                       "new_nb: %u \n",
//                       v_i,
//                       new_v,
//                       nb_id,
//                       new_nb_id);
//            }
        }
    }

//    {//test
//        printf("save_set_K...\n");
//    }
    // Save K, because of new ids
    std::vector<PANNS::idi> f_set_K(K);
    for (PANNS::idi k_i = 0; k_i < K; ++k_i) {
        f_set_K[k_i] = f_old_to_new[set_K[k_i]];
    }
    std::string no_suffix_name = get_no_suffix_name(connected_filename.c_str());
    std::string result_filename = no_suffix_name + "_K" + std::to_string(K) + ".txt";
    save_set_K(
            f_set_K,
            K,
            result_filename.c_str(),
            old_to_new);
}

void save_connected_graph(
        const char *filename,
        const std::vector< std::vector< std::pair<PANNS::idi, PANNS::distf> > > &c_edgelist,
        const PANNS::idi c_num_v)
{
    FILE *fout = fopen(filename, "w");
    if (!fout) {
        fprintf(stderr, "Error: cannot create file %s .\n", filename);
        exit(EXIT_FAILURE);
    }
    for (PANNS::idi v_id = 0; v_id < c_num_v; ++v_id) {
        const auto &edges = c_edgelist[v_id];
        if (edges.empty()) {
//            printf("Error: save_connected_graph: something is wrong!\n");
            continue;
        }
//        PANNS::idi new_v = old_to_new[v_id];
//        tmp_num_v = std::max(new_v + 1, tmp_num_v);
        for (const auto &e_i : edges) {
            PANNS::idi e_id = e_i.first;
            PANNS::distf dist = e_i.second;
//            PANNS::idi new_e = old_to_new[e_id];
//            tmp_num_v = std::max(new_e + 1, tmp_num_v);
//            fprintf(fout, "%u\t%u\t%f\n", new_v, new_e, dist);
            fprintf(fout, "%u\t%u\t%f\n", v_id, e_id, dist);
        }
    }

    fclose(fout);
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
//    engine.load_true_NN(
//            argv[6],
//            true_nn_list);

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
//                std::vector<uint8_t> is_visited(points_num, 0);
                boost::dynamic_bitset<> is_visited(points_num);
                std::vector<PANNS::Candidate> set_L((num_threads - 1) * L_local + L_master);
                std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
                std::vector<PANNS::idi> local_queues_starts(num_threads);
                for (int q_i = 0; q_i < num_threads; ++q_i) {
                    local_queues_starts[q_i] = q_i * L_local;
                }
//                std::vector<PANNS::idi> top_m_candidates(num_threads);
                auto s = std::chrono::high_resolution_clock::now();
//                engine.prepare_init_ids(init_ids, L_local);
                engine.prepare_init_ids(init_ids, L_master);
//                std::vector< std::map<PANNS::idi, PANNS::distf> > edgelist(points_num);
                std::vector<char> is_in_core(points_num, false);
                std::vector<PANNS::idi> iter_inserted(points_num);
                for (unsigned q_i = 0; q_i < query_num; ++q_i) {
#ifdef DEBUG_PRINT
                    printf("query_id: %u\n", q_i);
#endif
                    if (q_i != selected_query_id) {
                        continue;
                    }
                    engine.para_search_get_core_subgraph(
                            q_i,
                            K,
                            L_master,
                            set_L,
                            init_ids,
                            set_K_list[q_i],
                            L_local,
                            local_queues_starts,
                            local_queues_sizes,
                            is_visited,
                            subsearch_iterations,
                            is_in_core,
                            iter_inserted);
//                            edgelist);
                }
                auto e = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = e - s;
                {// output subgraph
                    // Get refined edgelist
                    std::vector< std::vector< std::pair<PANNS::idi, PANNS::distf> > > f_edgelist(points_num);
                    get_refined_edgelist(
                            engine,
                            selected_query_id,
                            is_in_core,
                            f_edgelist);

                    // Get the old_to_new mapping
                    PANNS::idi sb_num_v = 0;
                    PANNS::edgei sb_num_e = 0;
                    std::vector<PANNS::idi> old_to_new(points_num);
                    get_vid_old_to_new_map(
                            f_edgelist,
                            points_num,
                            old_to_new,
                            sb_num_v,
                            sb_num_e);

                    // Save the subgraph
                    save_subgraph(
                            path_results,
                            f_edgelist,
                            points_num,
                            old_to_new,
                            sb_num_v);
                    printf("num_threads: %d num_v: %u num_e: %lu\n", num_threads, sb_num_v, sb_num_e);

                    // Save results
                    std::string no_suffix_name = get_no_suffix_name(path_results);
                    std::string result_filename = no_suffix_name + "_K" + std::to_string(K) + ".txt";
                    save_set_K(
                            set_K_list[selected_query_id],
                            K,
                            result_filename.c_str(),
                            old_to_new);

                    std::vector< std::vector< std::pair<PANNS::idi, PANNS::distf> > > r_edgelist(sb_num_v);
//                    {//test
//                        printf("get_reverse_edgelist...\n");
//                    }
                    get_reverse_edgelist(
                            f_edgelist,
                            points_num,
                            old_to_new,
                            r_edgelist);

                    std::vector<bool> is_connected_to_finals(sb_num_v, false);
//                    {//test
//                        printf("get_connected_vertices...\n");
//                    }
                    get_connected_vertices(
                            r_edgelist,
                            sb_num_v,
                            set_K_list[selected_query_id],
                            old_to_new,
                            is_connected_to_finals);

                    std::vector< std::vector< std::pair<PANNS::idi, PANNS::distf> > > c_edgelist;
                    PANNS::idi c_num_v = 0;
                    PANNS::edgei c_num_e = 0;
                    std::string connected_filename = no_suffix_name + "_con2nn.txt";
//                    {//test
//                        printf("get_connected_edgelist...\n");
//                    }
                    get_connected_edgelist(
                            r_edgelist,
                            sb_num_v,
                            is_connected_to_finals,
                            c_edgelist,
                            c_num_v,
                            c_num_e,
                            connected_filename,
                            old_to_new,
                            set_K_list[selected_query_id],
                            K);

//                    {//test
//                        printf("save_connected_graph...\n");
//                    }
                    save_connected_graph(
                            connected_filename.c_str(),
                            c_edgelist,
                            c_num_v);
                    printf("num_threads: %d c_num_v: %u c_num_e: %lu\n", num_threads, c_num_v, c_num_e);
                }
//                std::unordered_map<unsigned, double> recalls;
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

    return 0;
}
