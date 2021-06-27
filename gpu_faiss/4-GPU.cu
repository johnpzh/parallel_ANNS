/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <random>

#include <faiss/IndexFlat.h>
#include <faiss/gpu/GpuIndexFlat.h>
#include <faiss/gpu/GpuIndexIVFFlat.h>
#include <faiss/gpu/StandardGpuResources.h>

#include <fstream>
#include <vector>
#include <sys/time.h>

using idi = unsigned;

/**
 * Class WallTimer to get the wall time.
 */
class WallTimer {
public:
    /**
     *
     * @return time stamp in seconds.
     */
    static double get_time_mark()
    {
        timeval t;
        gettimeofday(&t, nullptr);
        return t.tv_sec + t.tv_usec * 0.000001;
    }
};

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
//        const std::vector<std::vector<unsigned>> &set_K_list,
        const long *set_K_list, // the I from faiss, size num_queries * K.
        std::unordered_map<unsigned, double> &recalls,
        const idi K)
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

//    const idi set_K_size = L < 100 ? L : 100;
    const idi set_K_size = K;
    for (unsigned q_i = 0; q_i < num_queries; ++q_i) {
        for (unsigned top_i = 0; top_i < 100; ++top_i) {
            unsigned true_id = true_nn_list[q_i][top_i];
            for (unsigned n_i = 0; n_i < set_K_size; ++n_i) {
                unsigned rt_id = set_K_list[q_i * set_K_size + n_i];
                if (rt_id == true_id) {
//                if (set_K_list[q_i][n_i] == true_id) {
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

void load_data(const char* filename, float*& data, unsigned& num,
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

void save_result(const char* filename, std::vector<std::vector<unsigned> >& results) {
    std::ofstream out(filename, std::ios::binary | std::ios::out);

    for (unsigned i = 0; i < results.size(); i++) {
        unsigned GK = (unsigned)results[i].size();
        out.write((char*)&GK, sizeof(unsigned));
        out.write((char*)results[i].data(), GK * sizeof(unsigned));
    }
    out.close();
}

void usage(int argc, char *argv[])
{
    if (argc != 5) {
        fprintf(stderr,
                "Usage: %s <data_file> <query_file> <true_NN_file> <K>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    usage(argc, argv);
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

    const char *data_file = argv[1];
    const char *query_file = argv[2];
    const char *true_nn_file = argv[3];
    unsigned K = strtoull(argv[4], nullptr, 0);

    float* data_load = NULL;
    unsigned points_num, dim;
//    printf("Loading data %s...\n", data_file);
    load_data(data_file, data_load, points_num, dim);
    float* query_load = NULL;
    unsigned query_num, query_dim;
//    printf("Loading queries %s...\n", query_file);
    load_data(query_file, query_load, query_num, query_dim);
    assert(dim == query_dim);
    std::vector< std::vector<unsigned> > true_nn_list;
//    printf("Loading true_NN %s...\n", true_nn_file);
    load_true_NN(true_nn_file, query_num, true_nn_list);

    unsigned d = dim;      // dimension
    unsigned nb = points_num; // database size
    unsigned nq = query_num;  // nb of queries
    float* xb = data_load; // data
    float* xq = query_load; // queries

//    std::mt19937 rng;
//    std::uniform_real_distribution<> distrib;

//    for (int i = 0; i < nb; i++) {
//        for (int j = 0; j < d; j++)
//            xb[d * i + j] = distrib(rng);
//        xb[d * i] += i / 1000.;
//    }
//
//    for (int i = 0; i < nq; i++) {
//        for (int j = 0; j < d; j++)
//            xq[d * i + j] = distrib(rng);
//        xq[d * i] += i / 1000.;
//    }

    faiss::gpu::StandardGpuResources res;

    // Using a flat index

    faiss::gpu::GpuIndexFlatL2 index_flat(&res, d);

//    printf("is_trained = %s\n", index_flat.is_trained ? "true" : "false");
    index_flat.add(nb, xb); // add vectors to the index
//    printf("ntotal = %ld\n", index_flat.ntotal);

    int k = K;

    { // search xq
        long* I = new long[k * nq];
        float* D = new float[k * nq];

        double runtime = -WallTimer::get_time_mark();
//        index_flat.search(nq, xq, k, D, I);
        for (unsigned query_i = 0; query_i < nq; ++query_i) {
            float *xq_tmp = xq + static_cast<uint64_t>(query_i) * static_cast<uint64_t>(d);
            float *D_tmp = D + static_cast<uint64_t>(query_i) * static_cast<uint64_t>(k);
            long *I_tmp = I + static_cast<uint64_t>(query_i) * static_cast<uint64_t>(k);
            index_flat.search(1, xq_tmp, k, D_tmp, I_tmp);
        }
        runtime += WallTimer::get_time_mark();

//        // print results
//        printf("I (5 first results)=\n");
//        for (int i = 0; i < 5; i++) {
//            for (int j = 0; j < k; j++)
//                printf("%5ld ", I[i * k + j]);
//            printf("\n");
//        }
//
//        printf("I (5 last results)=\n");
//        for (int i = nq - 5; i < nq; i++) {
//            for (int j = 0; j < k; j++)
//                printf("%5ld ", I[i * k + j]);
//            printf("\n");
//        }

        // Get recalls.
        std::unordered_map<unsigned, double> recalls;
        get_recall_for_all_queries(
                nq,
                true_nn_list,
                I,
                recalls,
                k);

        // Print results.
        printf("Index: FlatL2 "
               "runtime(s.): %f "
               "A@100: %f "
               "latency(ms.): %f ",
               runtime,
               recalls[100],
               runtime / nq * 1000.0);
        printf("\n");
        delete[] I;
        delete[] D;
    }

    // Using an IVF index

    int nlist = 100;
    faiss::gpu::GpuIndexIVFFlat index_ivf(&res, d, nlist, faiss::METRIC_L2);

    assert(!index_ivf.is_trained);
    index_ivf.train(nb, xb);
    assert(index_ivf.is_trained);
    index_ivf.add(nb, xb); // add vectors to the index

//    printf("is_trained = %s\n", index_ivf.is_trained ? "true" : "false");
//    printf("ntotal = %ld\n", index_ivf.ntotal);

    { // search xq
        long* I = new long[k * nq];
        float* D = new float[k * nq];

        double runtime = -WallTimer::get_time_mark();
//        index_ivf.search(nq, xq, k, D, I);
        for (unsigned query_i = 0; query_i < nq; ++query_i) {
            float *xq_tmp = xq + static_cast<uint64_t>(query_i) * static_cast<uint64_t>(d);
            float *D_tmp = D + static_cast<uint64_t>(query_i) * static_cast<uint64_t>(k);
            long *I_tmp = I + static_cast<uint64_t>(query_i) * static_cast<uint64_t>(k);
            index_ivf.search(1, xq_tmp, k, D_tmp, I_tmp);
        }
        runtime += WallTimer::get_time_mark();

//        // print results
//        printf("I (5 first results)=\n");
//        for (int i = 0; i < 5; i++) {
//            for (int j = 0; j < k; j++)
//                printf("%5ld ", I[i * k + j]);
//            printf("\n");
//        }
//
//        printf("I (5 last results)=\n");
//        for (int i = nq - 5; i < nq; i++) {
//            for (int j = 0; j < k; j++)
//                printf("%5ld ", I[i * k + j]);
//            printf("\n");
//        }

        // Get recalls.
        std::unordered_map<unsigned, double> recalls;
        get_recall_for_all_queries(
                nq,
                true_nn_list,
                I,
                recalls,
                k);

        // Print results.
        printf("Index: IVFFlat "
               "runtime(s.): %f "
               "A@100: %f "
               "latency(ms.): %f ",
               runtime,
               recalls[100],
               runtime / nq * 1000.0);
        printf("\n");
        delete[] I;
        delete[] D;
    }

//    delete[] xb;
//    delete[] xq;
    free(xb);
    free(xq);

    return 0;
}