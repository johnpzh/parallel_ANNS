//
// Created by Zhen Peng on 9/14/2020.
//
/*
 * Get the true-K nearest neighbors (NN), then save to a file to be a input for test_precision.
 */
#include "../include/efanna2e/index_nsg.h"
#include "../include/efanna2e/util.h"
#include <chrono>
#include <string>
#include <omp.h>
#include  "../include/utils.h"
#include <math.h>

void load_data(char *filename, float *&data, unsigned &num,
               unsigned &dim)
{  // load data with sift10K pattern
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cout << "open file error" << std::endl;
        exit(-1);
    }
    in.read((char *) &dim, 4);
    // std::cout<<"data dimension: "<<dim<<std::endl;
    in.seekg(0, std::ios::end);
    std::ios::pos_type ss = in.tellg();
    size_t fsize = (size_t) ss;
    num = (unsigned) (fsize / (dim + 1) / 4);
    data = new float[(size_t) num * (size_t) dim];

    in.seekg(0, std::ios::beg);
    for (size_t i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur);
        in.read((char *) (data + i * dim), dim * 4);
    }
    in.close();
}

void save_result(const char *filename,
                 unsigned query_num,
                 unsigned K,
                 const std::vector <std::vector< std::pair<unsigned, float> > > &true_ress)
{
    assert(query_num == true_ress.size());
    std::ofstream out(filename, std::ios::binary | std::ios::out);
    if (!out.is_open()) {
        fprintf(stderr, "Error: cannot create file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    out.write(reinterpret_cast<char *>(&query_num), sizeof(query_num)); // Write query_num
    out.write(reinterpret_cast<char *>(&K), sizeof(K)); // Write K

    for (const auto &qr : true_ress) {
        assert(K == qr.size());
        for (const auto &r : qr) {
            unsigned id = r.first;
            float dist = r.second;
            out.write(reinterpret_cast<char *>(&id), sizeof(id));
            out.write(reinterpret_cast<char *>(&dist), sizeof(dist));
//            {//test
//                printf("[%u %f]\n", id, dist);
//            }
        }
    }
    out.close();
}

int main(int argc, char **argv)
{
    if (argc != 6) {
        std::cout << argv[0]
                  << " <data_file> <query_file> <nsg_path> <search_K> <result_path>"
                  << std::endl;
        exit(-1);
    }
    setbuf(stdout, nullptr); // Added by Johnpzh
    float *data_load = nullptr;
    unsigned points_num, dim;
    load_data(argv[1], data_load, points_num, dim);

    float *query_load = nullptr;
    unsigned query_num, query_dim;
    load_data(argv[2], query_load, query_num, query_dim);
    assert(dim == query_dim);

    unsigned K = strtoull(argv[4], nullptr, 0);
    {
        if (K < 100) {
            fprintf(stderr, "Error: K %u is smaller than 100.\n", K);
            exit(EXIT_FAILURE);
        }
//        unsigned query_num_max = strtoull(argv[6], nullptr, 0);
//        if (query_num > query_num_max) {
//            query_num = query_num_max;
//        }
        printf("query_num: %u\n", query_num);
        printf("K: %u\n", K);
    }

    // data_load = efanna2e::data_align(data_load, points_num, dim);//one must
    // align the data before build query_load = efanna2e::data_align(query_load,
    // query_num, query_dim);
    efanna2e::IndexNSG index(dim, points_num, efanna2e::FAST_L2, nullptr);
    index.Load(argv[3]);
    index.OptimizeGraph(data_load);

    efanna2e::Parameters paras;
    std::vector< std::vector< std::pair<unsigned, float> > > true_ress(query_num); // [].first: id, [].second: distance.
    auto s = std::chrono::high_resolution_clock::now();

    // Queries
    for (unsigned i = 0; i < query_num; i++) {
        index.get_true_NN(query_load + i * dim, K, true_ress[i]);
    }
    // Save true_res to the file
    save_result(argv[5], query_num, K, true_ress);

    auto e = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = e - s;
    std::cout << "run_time: " << diff.count() << "\n";

    delete [] data_load;
    delete [] query_load;
    return 0;
}

