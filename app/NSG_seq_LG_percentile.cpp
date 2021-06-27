//
// Created by Zhen Peng on 10/02/2020.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include <clocale>
#include <omp.h>
#include <unordered_map>
#include "../include/efanna2e/index_nsg.h"

using idi = unsigned;

void load_true_NN(
        const char *filename,
        const unsigned num_queries,
        std::vector< std::vector<idi> > &true_nn_list)
//        unsigned &t_K)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    idi t_query_num;
    idi t_K;
//    unsigned t_K;
    fin.read(reinterpret_cast<char *>(&t_query_num), sizeof(t_query_num));
    fin.read(reinterpret_cast<char *>(&t_K), sizeof(t_K));
//    if (t_query_num != query_num) {
//        fprintf(stderr, "Error: query_num %u is not equal to the record %u in true-NN file %s\n",
//                query_num, t_query_num, filename);
//        exit(EXIT_FAILURE);
//    }
    if (t_query_num < num_queries) {
        fprintf(stderr, "Error: t_query_num %u is smaller than num_queries_ %u\n", t_query_num, num_queries);
        exit(EXIT_FAILURE);
    }
    if (t_K < 100) {
        fprintf(stderr, "Error: t_K %u is smaller than 100.\n", t_K);
        exit(EXIT_FAILURE);
    }

//    data = new unsigned[(size_t) t_query_num * (size_t) t_K];
    true_nn_list.resize(t_query_num);
    for (idi q_i = 0; q_i < t_query_num; ++q_i) {
        true_nn_list[q_i].resize(t_K);
    }

    for (unsigned q_i = 0; q_i < t_query_num; ++q_i) {
//        size_t offset = q_i * t_K;
        for (unsigned n_i = 0; n_i < t_K; ++n_i) {
            unsigned id;
            float dist;
            fin.read(reinterpret_cast<char *>(&id), sizeof(id));
            fin.read(reinterpret_cast<char *>(&dist), sizeof(dist));
//            data[offset + n_i] = id;
            true_nn_list[q_i][n_i] = id;
        }
    }

    fin.close();
}

void get_recall_for_all_queries(
        const idi num_queries,
        const std::vector< std::vector<idi> > &true_nn_list,
        const std::vector<std::vector<unsigned>> &set_K_list,
        std::unordered_map<unsigned, double> &recalls,
        const idi L)
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

    const idi set_K_size = L < 100 ? L : 100;
    for (unsigned q_i = 0; q_i < num_queries; ++q_i) {
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
    recalls[1] /= 1.0 * num_queries;
    recalls[5] /= 5.0 * num_queries;
    recalls[10] /= 10.0 * num_queries;
    recalls[20] /= 20.0 * num_queries;
    recalls[50] /= 50.0 * num_queries;
    recalls[100] /= 100.0 * num_queries;
}

void load_data(char* filename, float*& data, unsigned& num,
               unsigned& dim) {  // load data with sift10K pattern
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    in.read((char*)&dim, 4);
//    std::cout << "data dimension: " << dim << std::endl;
    in.seekg(0, std::ios::end);
    std::ios::pos_type ss = in.tellg();
    size_t fsize = (size_t)ss;
    num = (unsigned)(fsize / (dim + 1) / 4);
    data = new float[static_cast<uint64_t>(num) * static_cast<uint64_t>(dim)];
//    data = new float[static_cast<uint64_t>(num) * static_cast<uint64_t>(dim) * sizeof(float)];

    in.seekg(0, std::ios::beg);
    for (size_t i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur);
        in.read((char*)(data + i * dim), dim * 4);
    }
    in.close();
}

void save_result(char* filename, std::vector<std::vector<unsigned> >& results) {
    std::ofstream out(filename, std::ios::binary | std::ios::out);

    for (unsigned i = 0; i < results.size(); i++) {
        unsigned GK = (unsigned)results[i].size();
        out.write((char*)&GK, sizeof(unsigned));
        out.write((char*)results[i].data(), GK * sizeof(unsigned));
    }
    out.close();
}

void search_one_time(
        efanna2e::IndexNSG &index,
        const unsigned L,
        const unsigned K,
        const float *data_load,
        const float *query_load,
        const unsigned points_num,
        const unsigned query_num,
        const unsigned dim,
//        const efanna2e::Parameters &paras,
        const std::vector< std::vector<idi> > &true_nn_list,
        std::vector<std::vector<unsigned> > &set_K_list,
        std::unordered_map<unsigned, double> &recalls,
        double &runtime,
        uint64_t &compt,
        double &avg_iters,
        std::unordered_map<unsigned, double> &percentile_latency)
{
    /* ----------------------
     * ------- Kernel ------- */
    efanna2e::Parameters paras;
    paras.Set<unsigned>("L_search", L);
    paras.Set<unsigned>("P_search", L);
    std::vector<std::vector<unsigned> > res(query_num);
    for (unsigned i = 0; i < query_num; i++) res[i].resize(K);

    std::vector<double> latency_of_queries(query_num);
    auto s = std::chrono::high_resolution_clock::now();
    for (unsigned i = 0; i < query_num; i++) {
        auto q_s = std::chrono::high_resolution_clock::now();
        index.Search(query_load + i * dim, data_load, K, paras, res[i].data());
        auto q_e = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> latency = q_e - q_s;
        latency_of_queries[i] = latency.count() * 1000.0;
    }
    auto e = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = e - s;
//    std::unordered_map<unsigned, double> recalls;
    get_recall_for_all_queries(
            query_num,
            true_nn_list,
            res,
            recalls,
            L);

    {// Percentile
        std::sort(latency_of_queries.begin(), latency_of_queries.end());
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
                "GFLOPS: %f "
                "avg_iters: %f ",
                L,
                diff.count(),
                index.count_distance_computation_,
                K,
                points_num,
                dim,
                query_num,
                query_num / diff.count(),
                diff.count() * 1000 / query_num,
                recalls[100],
                recalls[1],
                dim * 4.0 * index.count_distance_computation_ / (1U << 30U) / diff.count(),
                dim * (1.0 + 1.0 + 1.0) * index.count_distance_computation_ / (1U << 30U) /diff.count(),
                index.count_iters_ * 1.0 / query_num);
        printf("90.0%%tile(ms.): %f "
               "95.0%%tile(ms.): %f "
               "99.0%%tile(ms.): %f "
               "99.5%%tile(ms.): %f "
               "99.9%%tile(ms.): %f ",
               latency_of_queries[static_cast<uint32_t>(0.900 * query_num)],
               latency_of_queries[static_cast<uint32_t>(0.950 * query_num)],
               latency_of_queries[static_cast<uint32_t>(0.990 * query_num)],
               latency_of_queries[static_cast<uint32_t>(0.995 * query_num)],
               latency_of_queries[static_cast<uint32_t>(0.999 * query_num)]);
        printf("\n");
    }
    runtime = diff.count();
    compt = index.count_distance_computation_;
    index.count_distance_computation_ = 0;
    avg_iters = index.count_iters_ * 1.0 / query_num;
    index.count_iters_ = 0;
    set_K_list.swap(res);
    percentile_latency[900] = latency_of_queries[static_cast<uint32_t>(0.900 * query_num)];
    percentile_latency[950] = latency_of_queries[static_cast<uint32_t>(0.950 * query_num)];
    percentile_latency[990] = latency_of_queries[static_cast<uint32_t>(0.990 * query_num)];
    percentile_latency[995] = latency_of_queries[static_cast<uint32_t>(0.995 * query_num)];
    percentile_latency[999] = latency_of_queries[static_cast<uint32_t>(0.999 * query_num)];

//    PANNS::DiskIO::save_result(argv[6], set_K_list);
    /* ------- End of Kernel -------
     * ----------------------------- */
}

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <query_file> <nsg_path> <L> <K> <result_file> <true_NN_file> <L_upper> <P@100> [<P@100> ...]\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc < 10) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

    float* data_load = NULL;
    unsigned points_num, dim;
    printf("Loading %s...\n", argv[1]);
    load_data(argv[1], data_load, points_num, dim);
    float* query_load = NULL;
    unsigned query_num, query_dim;
    printf("Loading %s...\n", argv[2]);
    load_data(argv[2], query_load, query_num, query_dim);
    assert(dim == query_dim);
    printf("Loading %s...\n", argv[3]);
    efanna2e::IndexNSG index(dim, points_num, efanna2e::L2, nullptr);
    index.Load(argv[3]);
    unsigned L_lower_origin = strtoull(argv[4], nullptr, 0);
    unsigned K = strtoull(argv[5], nullptr, 0);
    std::vector< std::vector<unsigned> > true_nn_list;
    printf("Loading %s...\n", argv[7]);
    load_true_NN(
            argv[7],
            query_num,
            true_nn_list);
    unsigned L_upper_origin = strtoull(argv[8], nullptr, 0);
//    double P_dest = strtod(argv[9], nullptr);
    const unsigned base_loc_P_dest = 9;
    unsigned num_P_target = argc - base_loc_P_dest;
    std::vector<double> P_targets(num_P_target);
    for (int a_i = base_loc_P_dest; a_i < argc; ++a_i) {
        P_targets[a_i - base_loc_P_dest] = strtod(argv[a_i], nullptr);
    }

    for (const double P_dest : P_targets) {
        std::vector<std::vector<unsigned> > set_K_list;
        std::unordered_map<unsigned, double> recalls;
        unsigned L_upper = L_upper_origin;
        unsigned L_lower = L_lower_origin;
        unsigned L = L_upper;
        double runtime;
        uint64_t compt;
        double avg_iters;
        std::unordered_map<unsigned, double> percentile_latency;

        double last_runtime;
        uint64_t last_compt;
        double last_recall;
        unsigned last_L;
        double last_avg_iters;
        std::unordered_map<unsigned, double> last_percentile_latency;

        while (L_lower <= L_upper) {
//        L = (L_lower + L_upper) / 2;
            printf("L: %u "
                   "L_lower: %u "
                   "L_upper: %u\n",
                   L,
                   L_lower,
                   L_upper);

            search_one_time(
                    index,
                    L,
                    K,
                    data_load,
                    query_load,
                    points_num,
                    query_num,
                    dim,
                    true_nn_list,
                    set_K_list,
                    recalls,
                    runtime,
                    compt,
                    avg_iters,
                    percentile_latency);

            if (recalls[100] < P_dest) {
                L_lower = L + 1;
            } else if (recalls[100] > P_dest) {
                L_upper = L - 1;
                last_runtime = runtime;
                last_recall = recalls[100];
                last_compt = compt;
                last_L = L;
                last_avg_iters = avg_iters;
                last_percentile_latency = percentile_latency;
            } else {
                break;
            }
            if (L_lower <= L_upper) {
                L = (L_lower + L_upper) / 2;
            }
        }

//        L_upper = strtoull(argv[8], nullptr, 0);
        if (recalls[100] < P_dest && L < L_upper_origin) {
            runtime = last_runtime;
            recalls[100] = last_recall;
            compt = last_compt;
            L = last_L;
            avg_iters = last_avg_iters;
            percentile_latency = last_percentile_latency;
        }
//    L_upper = strtoull(argv[8], nullptr, 0);
//    while (recalls[100] < P_dest && L < L_upper) {
//        ++L;
//
//        search_one_time(
//                index,
//                L,
//                K,
//                data_load,
//                query_load,
//                points_num,
//                query_num,
//                dim,
//                true_nn_list,
//                set_K_list,
//                recalls,
//                runtime,
//                compt);
//    }
        save_result(argv[6], set_K_list);
        printf("---- FINAL ------\n");
        printf("P_dest: %f "
               "runtime(s.): %f "
               "compt.: %lu "
               "P@100: %f "
               "latency(ms.): %f "
               "L: %u "
               "avg_iters: %f ",
               P_dest,
               runtime,
               compt,
               recalls[100],
               runtime / query_num * 1000.0,
               L,
               avg_iters);
        printf("90.0%%tile(ms.): %f "
               "95.0%%tile(ms.): %f "
               "99.0%%tile(ms.): %f "
               "99.5%%tile(ms.): %f "
               "99.9%%tile(ms.): %f ",
               percentile_latency[900],
               percentile_latency[950],
               percentile_latency[990],
               percentile_latency[995],
               percentile_latency[999]);
        printf("\n");
    }
    return 0;
}
