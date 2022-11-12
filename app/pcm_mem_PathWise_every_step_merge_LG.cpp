//
// Created by Zhen Peng on 04/21/2022.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include <clocale>
#include <omp.h>
//#include "../include/papi_panns.h"
#include "../core/Searching.202204210403.PSS_v5.large_graph.path_wise_every_step_merge.h"
#include "../include/pcm_mem.h"

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
        double &t_seq)
//        std::vector<double> &time_break)
{
    std::vector<std::vector<PANNS::idi> > set_K_list(query_num);
    for (unsigned i = 0; i < query_num; i++) {
        set_K_list[i].resize(K);
    }
    std::vector<PANNS::idi> init_ids(L_master);
//                std::vector<uint8_t> is_visited(points_num, 0);
    boost::dynamic_bitset<> is_visited(points_num);
    std::vector<PANNS::Candidate> set_L((num_threads - 1) * L_local + L_master);
    std::vector<PANNS::idi> local_queues_sizes(num_threads, 0);
    std::vector<PANNS::idi> local_queues_starts(num_threads);
    for (int q_i = 0; q_i < num_threads; ++q_i) {
        local_queues_starts[q_i] = q_i * L_local;
    }
    auto s = std::chrono::high_resolution_clock::now();
    engine.prepare_init_ids(init_ids, L_master);

    // PCM Mem
    bool csv = false, csvheader = false, show_channel_output = true, print_update = false;
    pcm::uint32 no_columns = 2; // Default number of columns is 2
    pcm::PCM *m = pcm::PCM::getInstance();
    pcm::ServerUncoreMemoryMetrics metrics;
    metrics = m->PMMTrafficMetricsAvailable() ? pcm::Pmem : pcm::PartialWrites;
    m->disableJKTWorkaround();
    pcm::print_cpu_details();

    pcm::ServerUncoreCounterState *BeforeState = new pcm::ServerUncoreCounterState[m->getNumSockets()];
    pcm::ServerUncoreCounterState *AfterState = new pcm::ServerUncoreCounterState[m->getNumSockets()];
    pcm::uint64 BeforeTime = 0, AfterTime = 0;

    // Not sure which one should be used.
    m->setBlocked(true);
//    m->setBlocked(false);

    for (pcm::uint32 i = 0; i < m->getNumSockets(); ++i)
        BeforeState[i] = m->getServerUncoreCounterState(i);

    BeforeTime = m->getTickCount();
    // End PCM Mem

//#pragma omp parallel for
    for (unsigned q_i = 0; q_i < query_num; ++q_i) {

        engine.para_search_PSS_v5_large_graph_path_wise_every_step_merge(
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

    // PCM Mem
    AfterTime = m->getTickCount();
    for (pcm::uint32 i = 0; i < m->getNumSockets(); ++i)
        AfterState[i] = m->getServerUncoreCounterState(i);

    calculate_bandwidth(m, BeforeState, AfterState, AfterTime - BeforeTime, csv, csvheader, no_columns, metrics,
                        show_channel_output, print_update);

    delete[] BeforeState;
    delete[] AfterState;
    // End PCM Mem

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
    if (argc < 12) {
        fprintf(stderr,
//            "Usage: %s <data_file> <query_file> <nsg_path> <L_lower> <K> <result_file> <true_NN_file> <num_threads> <sub_iters> <L_upper> <P@100> [<P@100> ...]\n",
                "Usage: "
                "%s <data_file> <query_file> <nsg_path> "
                "<K> <result_file> <true_NN_file> <num_threads> "
                "<L_lower> <L_upper> <X> <P@100> [<P@100> ...]\n",
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

//    for (num_threads = 32; num_threads <= 64; num_threads *= 2) {

        engine.num_threads_ = num_threads;
        omp_set_num_threads(num_threads);
//    omp_set_nested(1);
//    omp_set_max_active_levels(2);
//    const unsigned L_master = strtoull(argv[8], nullptr, 0);
        const unsigned L_lower_origin = strtoull(argv[8], nullptr, 0);
        const unsigned L_upper_origin = strtoull(argv[9], nullptr, 0);
        const unsigned X = strtoull(argv[10], nullptr, 0);
        const unsigned base_loc_P_dest = 11;
        unsigned num_P_target = argc - base_loc_P_dest;
        std::vector<double> P_targets(num_P_target);
        for (int a_i = base_loc_P_dest; a_i < argc; ++a_i) {
            P_targets[a_i - base_loc_P_dest] = strtod(argv[a_i], nullptr);
        }

//    for (int run_i = 0; run_i < 6; ++run_i) {
    // PCM Mem
    bool csv = false, csvheader = false, show_channel_output = true, print_update = false;
    pcm::uint32 no_columns = 2; // Default number of columns is 2
    pcm::PCM *m = pcm::PCM::getInstance();
    pcm::ServerUncoreMemoryMetrics metrics;
    metrics = m->PMMTrafficMetricsAvailable() ? pcm::Pmem : pcm::PartialWrites;
    m->disableJKTWorkaround();
    pcm::print_cpu_details();

//    pcm::ServerUncoreCounterState *BeforeState = new pcm::ServerUncoreCounterState[m->getNumSockets()];
//    pcm::ServerUncoreCounterState *AfterState = new pcm::ServerUncoreCounterState[m->getNumSockets()];
//    pcm::uint64 BeforeTime = 0, AfterTime = 0;
//
//    // Not sure which one should be used.
//    m->setBlocked(true);
////    m->setBlocked(false);
//
//    for (pcm::uint32 i = 0; i < m->getNumSockets(); ++i)
//        BeforeState[i] = m->getServerUncoreCounterState(i);
//
//    BeforeTime = m->getTickCount();
    // End PCM Mem

    for (const double P_dest: P_targets) {
//        std::vector<std::vector<unsigned> > set_K_list;
//        std::unordered_map<unsigned, double> recalls;
        unsigned L_upper = L_upper_origin;
        unsigned L_lower = L_lower_origin;
        unsigned L_master = L_upper;
        unsigned L_local = L_master;
        unsigned subsearch_iterations = X;
        double runtime;
        uint64_t compt;
        double recall;
        double avg_merge;
        double t_expand;
        double t_merge;
        double t_seq;
//        std::vector<double> time_break(4);

        double last_runtime;
        uint64_t last_compt;
        double last_recall;
        unsigned last_L;
        double last_avg_merge;
        double last_t_expand;
        double last_t_merge;
        double last_t_seq;
//        std::vector<double> last_time_break(4);

        while (L_lower <= L_upper) {
            printf("L: %u "
                   "L_lower: %u "
                   "L_upper: %u\n",
                   L_local,
                   L_lower,
                   L_upper);

            // PCM Begins
            pcm::ServerUncoreCounterState *BeforeState = new pcm::ServerUncoreCounterState[m->getNumSockets()];
            pcm::ServerUncoreCounterState *AfterState = new pcm::ServerUncoreCounterState[m->getNumSockets()];
            pcm::uint64 BeforeTime = 0, AfterTime = 0;

            // Not sure which one should be used.
            m->setBlocked(true);
//    m->setBlocked(false);

            for (pcm::uint32 i = 0; i < m->getNumSockets(); ++i)
                BeforeState[i] = m->getServerUncoreCounterState(i);

            BeforeTime = m->getTickCount();
            // End PCM

            search_one_time(
                    engine,
                    L_master,
                    K,
                    points_num,
                    query_num,
                    data_dimension,
                    num_threads,
                    L_local,
                    subsearch_iterations,
                    true_nn_list,
                    runtime,
                    compt,
                    recall,
                    avg_merge,
                    t_expand,
                    t_merge,
                    t_seq);

            // PCM Mem
            AfterTime = m->getTickCount();
            for (pcm::uint32 i = 0; i < m->getNumSockets(); ++i)
                AfterState[i] = m->getServerUncoreCounterState(i);

            calculate_bandwidth(m, BeforeState, AfterState, AfterTime - BeforeTime, csv, csvheader, no_columns, metrics,
                                show_channel_output, print_update);

            delete[] BeforeState;
            delete[] AfterState;
            // End PCM Mem

            if (recall < P_dest) {
                L_lower = L_local + 1;
            } else if (recall > P_dest) {
                L_upper = L_local - 1;
                last_runtime = runtime;
                last_recall = recall;
                last_compt = compt;
                last_L = L_local;
                last_avg_merge = avg_merge;
                last_t_expand = t_expand;
                last_t_merge = t_merge;
                last_t_seq = t_seq;
            } else {
                break;
            }
            if (L_lower <= L_upper) {
                L_master = (L_lower + L_upper) / 2;
                L_local = L_master;
            }
        }

        L_upper = L_upper_origin;
        if (recall < P_dest && L_master < L_upper) {
            runtime = last_runtime;
            recall = last_recall;
            compt = last_compt;
            L_local = last_L;
            avg_merge = last_avg_merge;
            t_expand = last_t_expand;
            t_merge = last_t_merge;
            t_seq = last_t_seq;
        }

//        PANNS::DiskIO::save_result(path_results, set_K_list);
        printf("---- FINAL ----\n");
        printf("P_dest: %f "
               "runtime(s.): %f "
               "compt.: %lu "
               "P@100: %f "
               "latency(ms.): %f "
               "L: %u "
               "X: %u "
               "avg_merge: %f "
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
               L_local,
               subsearch_iterations,
               avg_merge,
               t_expand,
               t_merge,
               t_seq,
               t_expand / runtime * 100.0,
               t_merge / runtime * 100.0,
               t_seq / runtime * 100.0);
        printf("\n");

    }

//    // PCM Mem
//    AfterTime = m->getTickCount();
//    for (pcm::uint32 i = 0; i < m->getNumSockets(); ++i)
//        AfterState[i] = m->getServerUncoreCounterState(i);
//
//    calculate_bandwidth(m, BeforeState, AfterState, AfterTime - BeforeTime, csv, csvheader, no_columns, metrics,
//                        show_channel_output, print_update);
//
//    delete[] BeforeState;
//    delete[] AfterState;
//    // End PCM Mem
//    }
//    }
    return 0;
}
