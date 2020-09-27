//
// Created by Zhen Peng on 09/22/2020.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include <clocale>
#include <omp.h>
#include "../core/Searching.202009221831.bfs_test_nsg.h"

void read_num_v(
        char *filename,
        PANNS::Searching &engine)
{
    unsigned dimension;
    unsigned number;

    std::ifstream fin(filename);
    if (!fin.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    fin.read(reinterpret_cast<char *>(&dimension), 4); // Read the dimension
    fin.seekg(0, std::ios_base::end);
    uint64_t file_size = fin.tellg();
    number = file_size / (4 + 4 * dimension);

    engine.dimension_ = dimension;
    engine.num_v_ = number;
}

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <nsg_path>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

    PANNS::Searching engine;
    printf("Loading %s ...\n", argv[1]);
    read_num_v(argv[1], engine);
    printf("Loading %s ...\n", argv[2]);
    engine.load_common_nsg_graph(argv[2]);

    printf("BFS...\n");
//#pragma omp parallel for num_threads(64)
//    for (unsigned v_id = 0; v_id < engine.num_v_; v_id += engine.num_v_ / 64) {
        unsigned v_id = engine.ep_;
        unsigned visited1 = engine.bfs1(v_id);
        unsigned visited2 = engine.bfs2(v_id);
//        if (visited != engine.num_v_) {
//            printf("v_id: %u "
//                   "visited: %u disconnected.\n",
//                   v_id,
//                   visited);
        if (visited1 != visited2) {
            printf("visited1: %u "
                   "visited2: %u "
                   "disconnected\n",
                   visited1,
                   visited2);
        } else if (visited1 != engine.num_v_) {
            printf("visited: %u disconnected\n", visited1);
        } else {
            printf("v_id: %u connected.\n", v_id);
        }
//    }

    return 0;
}
