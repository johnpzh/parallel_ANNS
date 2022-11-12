//
// Created by Zhen Peng on 11/21/2021.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include <clocale>
#include <omp.h>
//#include "../include/papi_panns.h"
#include "../core/Searching.202111210046.PSS_v5.LG.update_positions.h"

uint32_t get_average_update_position(
        PANNS::Searching &engine,
        const int num_threads)
{
    std::vector<uint64_t> avg_every_worker(num_threads);

    for (int w_i = 0; w_i < num_threads; ++w_i) {
        auto &positions = engine.update_positions[w_i];
        uint64_t sum = std::accumulate(positions.begin(), positions.end(), 0LLU);
        avg_every_worker[w_i] = sum / positions.size();
//        std::sort(positions.begin(), positions.end());
//        avg_every_worker[w_i] = positions[positions.size() / 2];
//        {//test
//            for (auto pos : positions) {
//                printf("w_i: %u pos: %u\n",
//                       w_i, pos);
//            }
//        }
    }

    uint64_t sum = std::accumulate(avg_every_worker.begin(), avg_every_worker.end(), 0LLU);
    return sum / num_threads;
//    std::sort(avg_every_worker.begin(), avg_every_worker.end());
//    return avg_every_worker[num_threads / 2];
}

uint32_t get_approx_avg_update_position(
        PANNS::Searching &engine)
{
    uint64_t sum = std::accumulate(engine.update_pos_means.begin(), engine.update_pos_means.end(), 0LLU);

    return sum / engine.update_pos_means.size();
}

double get_ratio_first_half(
        PANNS::Searching &engine,
        const uint32_t L_local)
{
    const uint32_t half_pos = L_local / 2;
    uint64_t cnt_1st_half = 0;
    for (const auto pos : engine.update_pos_means) {
        if (pos <= half_pos) {
            ++cnt_1st_half;
        }
    }

    return 1.0 * cnt_1st_half / engine.update_pos_means.size();
}

void get_recall_for_some_queries(
        const std::vector< std::vector<PANNS::idi> > &true_nn_list,
        const std::vector<std::vector<unsigned>> &set_K_list,
//        const PANNS::idi num_queries,
        const std::vector<uint32_t> &selected_queries,
        const PANNS::idi set_K_size,
        std::unordered_map<unsigned, double> &recalls)
{
    if (true_nn_list[0].size() < 100) {
        fprintf(stderr, "Error: Number of true nearest neighbors of a query is smaller than 100.\n");
        exit(EXIT_FAILURE);
    }
    recalls[1] = 0.0;
    recalls[5] = 0.0;
    recalls[10] = 0.0;
    recalls[20] = 0.0;
    recalls[50] = 0.0;
    recalls[100] = 0.0;

//    const uint32_t set_K_size = L < 100 ? L : 100;
//    for (unsigned q_i = 0; q_i < num_queries; ++q_i) {
    for (const auto q_i : selected_queries) {
        for (unsigned top_i = 0; top_i < 100; ++top_i) {
            unsigned true_id = true_nn_list[q_i][top_i];
            for (unsigned n_i = 0; n_i < set_K_size; ++n_i) {
                if (set_K_list[q_i][n_i] == true_id) {
                    if (n_i < 1) recalls[1] += 1;
                    if (n_i < 5) recalls[5] += 1;
                    if (n_i < 10) recalls[10] += 1;
                    if (n_i < 20) recalls[20] += 1;
                    if (n_i < 50) recalls[50] += 1;
                    if (n_i < 100) recalls[100] += 1;
                }
            }
        }
    }
//    }
    uint32_t num_queries = selected_queries.size();
    recalls[1] /= 1.0 * num_queries;
    recalls[5] /= 5.0 * num_queries;
    recalls[10] /= 10.0 * num_queries;
    recalls[20] /= 20.0 * num_queries;
    recalls[50] /= 50.0 * num_queries;
    recalls[100] /= 100.0 * num_queries;
}

void search_one_time(
        PANNS::Searching &engine,
        const unsigned L_master,
        const unsigned K,
        const unsigned points_num,
        const unsigned query_num,
        const unsigned data_dimension,
        const int num_threads,
        const unsigned L_local,
        const unsigned subsearch_iterations,
        const std::vector< std::vector<PANNS::idi> > &true_nn_list,
//        std::vector<std::vector<unsigned> > &set_K_list_return,
//        std::unordered_map<unsigned, double> &recalls,
        double &runtime,
        uint64_t &compt,
        double &p100,
        double &avg_merge,
        double &t_expand,
        double &t_merge,
        double &t_seq,
        double &ratio_1st_half)
//        uint32_t &avg_pos)
//        std::vector<double> &time_break)
{
    std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
    for (unsigned i = 0; i < query_num; i++) {
        set_K_list[i].resize(K);
    }
    std::vector<PANNS::idi> init_ids(L_master);
//                std::vector<uint8_t> is_visited(points_num, 0);
    boost::dynamic_bitset<> is_visited(points_num);
    std::vector<PANNS::Candidate> set_L(num_threads * L_local + L_master);
    std::vector<PANNS::idi> local_queues_sizes(num_threads + 1, 0);
    std::vector<PANNS::idi> local_queues_starts(num_threads + 1);
    for (int q_i = 0; q_i < num_threads + 1; ++q_i) {
        local_queues_starts[q_i] = q_i * L_local;
    }
    {
        engine.update_positions.resize(num_threads);
        for (int q_i = 0; q_i < num_threads; ++q_i) {
            engine.update_positions[q_i].clear();
        }
    }
//    std::vector<uint32_t> selected_queries = {2279};
    auto s = std::chrono::high_resolution_clock::now();
    engine.prepare_init_ids(init_ids, L_master);
//#pragma omp parallel for
    for (unsigned q_i = 0; q_i < query_num; ++q_i) {
//        if (std::find(selected_queries.begin(), selected_queries.end(), q_i) == selected_queries.end()) {
//            continue;
//        }
        engine.para_search_PSS_v5_large_graph_update_positions(
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
                subsearch_iterations);
    }
    auto e = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = e - s;
    std::unordered_map<unsigned, double> recalls;
    { // Recall values
        engine.get_recall_for_all_queries(
                true_nn_list,
                set_K_list,
                recalls,
                K);
//                L_master);
//        get_recall_for_some_queries(
//                true_nn_list,
//                set_K_list,
//                selected_queries,
//                K,
//                recalls);
    }
    {// update positions
//        avg_pos = get_average_update_position(
//                engine,
//                num_threads);
//        avg_pos = get_approx_avg_update_position(
//                engine);
        ratio_1st_half = get_ratio_first_half(
                engine,
                L_local);
    }
    {// Basic output
        printf(
//                            "local_queue_length: %u "
                "num_threads: %d "
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
                "GFLOPS: %f "
                "local_L: %u "
                "sub_iters: %u "
                "avg_merge: %f "
//                "avg_pos: %u "
                "ratio_1st_half: %f "
                "t_expand(s.): %f "
                "t_merge(s.): %f "
                "t_seq(s.): %f ",
//                "t_pick(s.): %f ",
                num_threads,
                L_master,
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
                data_dimension * (1.0 + 1.0 + 1.0) * engine.count_distance_computation_ / (1U << 30U) /
                diff.count(),
                L_local,
                subsearch_iterations,
                engine.count_merge_ * 1.0 / query_num,
//                avg_pos,
                ratio_1st_half,
                engine.time_expand_,
                engine.time_merge_,
                engine.time_seq_);
//                engine.time_pick_);
        printf("\n");
    }
    runtime = diff.count();
    compt = engine.count_distance_computation_;
    engine.count_distance_computation_ = 0;
    p100 = recalls[100];
    avg_merge = engine.count_merge_ * 1.0 / query_num;
    engine.count_merge_ = 0;
    t_expand = engine.time_expand_;
    t_merge = engine.time_merge_;
    t_seq = engine.time_seq_;
//    time_break[3] = engine.time_pick_;
    engine.time_expand_ = 0.0;
    engine.time_merge_ = 0.0;
    engine.time_seq_ = 0.0;
//    engine.time_pick_ = 0.0;
//    set_K_list_return.swap(set_K_list);
}

void usage(int argc, char *argv[])
{
    if (argc < 13) {
        fprintf(stderr,
//            "Usage: %s <data_file> <query_file> <nsg_path> <L_lower> <K> <result_file> <true_NN_file> <num_threads> <sub_iters> <L_upper> <P@100> [<P@100> ...]\n",
                "Usage: "
                "%s <data_file> <query_file> <nsg_path> "
                "<K> <result_file> <true_NN_file> <num_threads> "
                "<L_master_lower> <L_master_upper> <L_local> <X> "
                "<P@100> [<P@100> ...]\n",
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
    engine.load_data_load(argv[1]);
    engine.load_queries_load(argv[2]);
//    engine.load_nsg_graph(argv[3]);
    engine.load_common_nsg_graph(argv[3]);

//    unsigned L_lower_origin = strtoull(argv[4], nullptr, 0);
    const unsigned K = strtoull(argv[4], nullptr, 0);
    const char *path_results = argv[5];
    std::vector< std::vector<PANNS::idi> > true_nn_list;
    engine.load_true_NN(
            argv[6],
            true_nn_list);

    unsigned data_dimension = engine.dimension_;
    unsigned points_num = engine.num_v_;
    unsigned query_num = engine.num_queries_;

    int num_threads = strtoull(argv[7], nullptr, 0);
//    if (1 != num_threads) {
//        fprintf(stderr, "Error: num_threads is %d, which should be 1.\n", num_threads);
//        exit(EXIT_FAILURE);
//    }
    engine.num_threads_ = num_threads;
    omp_set_num_threads(num_threads);
//    omp_set_nested(1);
//    omp_set_max_active_levels(2);

//    const unsigned L_master = strtoull(argv[8], nullptr, 0);
    const unsigned L_master_lower_origin = strtoull(argv[8], nullptr, 0);
    const unsigned L_master_upper_origin = strtoull(argv[9], nullptr, 0);
    const unsigned L_local_fixed = strtoll(argv[10], nullptr, 0);
    const unsigned X = strtoull(argv[11], nullptr, 0);
    const unsigned base_loc_P_dest = 12;
    unsigned num_P_target = argc - base_loc_P_dest;
    std::vector<double> P_targets(num_P_target);
    for (int a_i = base_loc_P_dest; a_i < argc; ++a_i) {
        P_targets[a_i - base_loc_P_dest] = strtod(argv[a_i], nullptr);
    }

    for (const double P_dest : P_targets) {
//        std::vector<std::vector<unsigned> > set_K_list;
//        std::unordered_map<unsigned, double> recalls;
        unsigned L_master_upper = L_master_upper_origin;
        unsigned L_master_lower = L_master_lower_origin;
        unsigned L_master = L_master_upper;
//        unsigned L_local = L_master;
        unsigned subsearch_iterations = X;
        double runtime;
        uint64_t compt;
        double recall;
        double avg_merge;
        double t_expand;
        double t_merge;
        double t_seq;
        double ratio_1st_half;
//        uint32_t avg_pos;
//        std::vector<double> time_break(4);

        double last_runtime;
        uint64_t last_compt;
        double last_recall;
        unsigned last_L;
        double last_avg_merge;
        double last_t_expand;
        double last_t_merge;
        double last_t_seq;
        double last_ratio_1st_half;
//        uint32_t last_avg_pos;
//        std::vector<double> last_time_break(4);

        while (L_master_lower <= L_master_upper) {
            printf("L: %u "
                   "L_lower: %u "
                   "L_upper: %u\n",
                   L_master,
                   L_master_lower,
                   L_master_upper);

            search_one_time(
                    engine,
                    L_master,
                    K,
                    points_num,
                    query_num,
                    data_dimension,
                    num_threads,
                    L_local_fixed,
                    subsearch_iterations,
                    true_nn_list,
                    runtime,
                    compt,
                    recall,
                    avg_merge,
                    t_expand,
                    t_merge,
                    t_seq,
                    ratio_1st_half);
//                    avg_pos);

            if (recall < P_dest) {
                L_master_lower = L_master + 1;
            } else if (recall > P_dest) {
                L_master_upper = L_master - 1;
                last_runtime = runtime;
                last_recall = recall;
                last_compt = compt;
                last_L = L_master;
                last_avg_merge = avg_merge;
                last_t_expand = t_expand;
                last_t_merge = t_merge;
                last_t_seq = t_seq;
                last_ratio_1st_half = ratio_1st_half;
//                last_avg_pos = avg_pos;
            } else {
                break;
            }
            if (L_master_lower <= L_master_upper) {
                L_master = (L_master_lower + L_master_upper) / 2;
//                L_local = L_master;
            }
        }

        L_master_upper = L_master_upper_origin;
        if (recall < P_dest && L_master < L_master_upper) {
            runtime = last_runtime;
            recall = last_recall;
            compt = last_compt;
            L_master = last_L;
            avg_merge = last_avg_merge;
            t_expand = last_t_expand;
            t_merge = last_t_merge;
            t_seq = last_t_seq;
            ratio_1st_half = last_ratio_1st_half;
//            avg_pos = last_avg_pos;
        }

//        PANNS::DiskIO::save_result(path_results, set_K_list);
        printf("---- FINAL ----\n");
        printf("P_dest: %f "
               "runtime(s.): %f "
               "compt.: %lu "
               "P@100: %f "
               "latency(ms.): %f "
               "L_master: %u "
               "L_local: %u "
               "X: %u "
               "avg_merge: %f "
               "ratio_1st_half: %f "
//               "avg_pos: %u "
               "t_expand(s.): %f "
               "t_merge(s.): %f "
               "t_seq(s.): %f "
               "t_p_expand(%%): %f "
               "t_p_merge(%%): %f "
               "t_p_seq(%%): %f ",
               P_dest,
               runtime,
               compt,
               recall,
               runtime / query_num * 1000.0,
               L_master,
               L_local_fixed,
               subsearch_iterations,
               avg_merge,
                ratio_1st_half,
//               avg_pos,
               t_expand,
               t_merge,
               t_seq,
               t_expand / runtime * 100.0,
               t_merge / runtime * 100.0,
               t_seq / runtime * 100.0);
        printf("\n");

    }
    return 0;
}
