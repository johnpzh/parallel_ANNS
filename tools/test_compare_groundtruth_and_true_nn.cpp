//
// Created by Zhen Peng on 11/15/2020.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include <clocale>
#include <omp.h>
#include <unordered_map>
#include <fstream>
//#include "../include/efanna2e/index_nsg.h"

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

void load_data(char* filename, unsigned*& data, unsigned& num,
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
    data = new unsigned[static_cast<uint64_t>(num) * static_cast<uint64_t>(dim)];
//    data = new float[static_cast<uint64_t>(num) * static_cast<uint64_t>(dim) * sizeof(float)];

    in.seekg(0, std::ios::beg);
    for (size_t i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur);
        in.read((char*)(data + i * dim), dim * 4);
    }
    in.close();
}

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <groundtruth_file> <true_NN_file>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

    // Ground truth file
    unsigned* groundtruth_load = NULL;
    unsigned points_num, dim;
    printf("Loading %s...\n", argv[1]);
    load_data(argv[1], groundtruth_load, points_num, dim);
    unsigned query_num = points_num;
    {
        printf("points_num: %u\t"
               "dim: %u\n",
               points_num,
               dim);
    }

    // True-NN file
    std::vector< std::vector<unsigned> > true_nn_list;
    printf("Loading %s...\n", argv[2]);
    load_true_NN(
            argv[2],
            query_num,
            true_nn_list);

    // Print out
//    printf("query_id:\t"
//           "k:\t"
//           "gt_v:\t"
//           "nn_v:\n");
    unsigned count_unequal = 0;
    for (unsigned q_i = 0; q_i < points_num; ++q_i) {
        bool is_first = true;
        unsigned gt_last;
        unsigned nn_last;
        for (unsigned k_i = 0; k_i < 100; ++k_i) {
            unsigned gt_v = groundtruth_load[q_i * 100 + k_i];
            unsigned nn_v = true_nn_list[q_i][k_i];
            if (gt_v != nn_v) {
                if (is_first) {
                    is_first = false;
                    gt_last = gt_v;
                    nn_last = nn_v;
                } else if (gt_last == nn_v && nn_last == gt_v) {
                    is_first = true;
                } else {
                    is_first = true;
                    count_unequal += 2;
                    printf("%u\t"
                           "%u\t"
                           "%u\t"
                           "%u\n",
                           q_i,
                           k_i - 1,
                           gt_last,
                           nn_last);
                    printf("%u\t"
                           "%u\t"
                           "%u\t"
                           "%u\n",
                           q_i,
                           k_i,
                           gt_v,
                           nn_v);
                }
            }
//            if (gt_v != nn_v) {
//                ++count_unequal;
//                printf("%u\t"
//                       "%u\t"
//                       "%u\t"
//                       "%u\n",
//                       q_i,
//                       k_i,
//                       gt_v,
//                       nn_v);
//            }
//            if (q_i == 9803) {
//                printf("%u\t"
//                       "%u\t"
//                       "%u\t"
//                       "%u\n",
//                       q_i,
//                       k_i,
//                       gt_v,
//                       nn_v);
//            }
        }
    }
    printf("count_unequal: %u\n", count_unequal);
    return 0;
}
