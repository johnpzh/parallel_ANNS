//
// Created by 付聪 on 2017/6/21.
//
/*
 * Modified by Zhen Peng 09/14/2020.
 */

#include "../include/efanna2e/index_nsg.h"
#include "../include/efanna2e/util.h"

void load_data(char* filename, float*& data, unsigned& num,
               unsigned& dim) {  // load data with sift10K pattern
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    in.read((char*)&dim, 4);
    in.seekg(0, std::ios::end);
    std::ios::pos_type ss = in.tellg();
    size_t fsize = (size_t)ss;
    num = (unsigned)(fsize / (dim + 1) / 4);
    data = new float[(size_t)num * (size_t)dim];

    in.seekg(0, std::ios::beg);
    for (size_t i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur);
        in.read((char*)(data + i * dim), dim * 4);
    }
    in.close();
}
int main(int argc, char** argv) {
    if (argc != 7) {
        std::cout << argv[0] << " data_file nn_graph_path L R C save_graph_file"
                  << std::endl;
        exit(-1);
    }
    setbuf(stdout, nullptr);
    float* data_load = NULL;
    unsigned points_num, dim;
    printf("Loading %s ...\n", argv[1]);
    load_data(argv[1], data_load, points_num, dim);

    printf("Loading %s ...\n", argv[2]);
    std::string nn_graph_path(argv[2]);
    unsigned L = (unsigned)atoi(argv[3]);
    unsigned R = (unsigned)atoi(argv[4]);
    unsigned C = (unsigned)atoi(argv[5]);

    // data_load = efanna2e::data_align(data_load, points_num, dim);//one must
    // align the data before build
    efanna2e::IndexNSG index(dim, points_num, efanna2e::L2, nullptr);

    auto s = std::chrono::high_resolution_clock::now();
    efanna2e::Parameters paras;
    paras.Set<unsigned>("L", L);
    paras.Set<unsigned>("R", R);
    paras.Set<unsigned>("C", C);
    paras.Set<std::string>("nn_graph_path", nn_graph_path);

    printf("Building...\n");
    index.Build(points_num, data_load, paras);
    auto e = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = e - s;

    std::cout << "indexing_time: " << diff.count() << "\n";
    index.Save(argv[6]);

    return 0;
}