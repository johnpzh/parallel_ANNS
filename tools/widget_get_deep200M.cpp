//
// Created by Zhen Peng on 2/14/2020.
//

#include <iostream>
#include <cstdio>
//#include <vector>
#include <chrono>
#include <clocale>
#include <fstream>
#include <memory>
#include <vector>
//#include <omp.h>
//#include "../core/Searching.202002141745.critical_omp_top_m.h"
//#include "../core/Searching.202002101535.reorganization.h"
//#include "../core/Searching.201912161559.set_for_queue.h"
//#include "../core/Searching.201912091448.map_for_queries_ids.h"
//#include "../core/Searching.h"
//#include "../include/utils.h"
//#include "../include/efanna2e/index_nsg.h"
//#include "../core/Searching.202002181409.local_queue_and_merge.h"


void generate(
        char *filename_input,
        const uint32_t sample_count,
        char *filename_output)
{
    std::ifstream fin(filename_input);
    if (!fin.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename_output);
        exit(EXIT_FAILURE);
    }
    std::ofstream fout(filename_output);
    if (!fout.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename_output);
        exit(EXIT_FAILURE);
    }

//    const uint32_t total_count = 200000000; // 200M
    const uint32_t total_count = sample_count;
    int dimension = 0;
    fin.read(reinterpret_cast<char *>(&dimension), 4); // Read the dimension
    fin.seekg(0, std::ios_base::end);
    const uint32_t data_bytes = 4 * dimension; // DEEP1B is .fvecs, so every data point is float32
    const uint32_t element_bytes = 4 + data_bytes;
    const uint64_t file_bytes = fin.tellg();
    const uint64_t num_vectors = file_bytes / element_bytes; // Totle elements in the file.
    const uint64_t stride = num_vectors / total_count - 1; // Skip bytes after read one element

    // Sample the file
//    std::unique_ptr<float[]> data = std::make_unique<float[]>(dimension);
    std::vector<float> data(dimension);
    fin.seekg(0, std::ios_base::beg);
    for (uint32_t v_i = 0; v_i < total_count; ++v_i) {
        if (0 == v_i % 20000000) {
            printf("Reading %u / %u ...\n", v_i, total_count);
        }

        // Read the file
        fin.seekg(4, std::ios_base::cur);
        fin.read(reinterpret_cast<char *>(data.data()), data_bytes);
//        fin.read(reinterpret_cast<char *>(data.get()), data_bytes);

        // Write the file
        fout.write(reinterpret_cast<char *>(&dimension), 4);
        fout.write(reinterpret_cast<char *>(data.data()), data_bytes);
//        fout.write(reinterpret_cast<char *>(data.get()), data_bytes);

        // Skip
        fin.seekg(stride * element_bytes, std::ios_base::cur);
    }
    printf("Done.\n");
}
//void generate(
//        char *directory_input,
//        char *filename_output)
//{
//    std::ofstream fout(filename_output);
//    if (!fout.is_open()) {
//        fprintf(stderr, "Error: cannot open file %s\n", filename_output);
//        exit(EXIT_FAILURE);
//    }
//
//    const uint32_t avail_count = 1000000000; // 1B
//    const uint32_t total_count = 200000000; // 200M
//    const int num_files = 37;
//    const double ratio = 1.0 * total_count / avail_count;
////    const uint32_t count_every_file = (total_count - 1) / num_files + 1;
//    char basename[8];
////    for (int f_i = num_files - 1; f_i >= 0; --f_i) {
//    uint64_t read_count = 0;
//    for (int f_i = 0; f_i < num_files; ++f_i) {
//        sprintf(basename, "base_%02d", f_i);
//        std::string filename_input = std::string(directory_input) + '/' + basename;
//        std::ifstream fin(filename_input);
//        if (!fin.is_open()) {
//            fprintf(stderr, "Error: cannot open file %s\n", filename_input.c_str());
//            exit(EXIT_FAILURE);
//        }
////        uint32_t count_vectors = count_every_file;
////        if (f_i == num_files - 1) {
////            count_vectors = total_count - f_i * count_every_file;
////        }
//        // Get dimension and count of vectors
//        int dimension;
////        uint64_t dimension;
////        uint32_t dimension;
//        fin.read(reinterpret_cast<char *>(&dimension), 4); // Read the dimension
//        fin.seekg(0, std::ios_base::end);
//        uint64_t file_bytes = fin.tellg();
//        const uint32_t num_vectors = file_bytes / (4 + 4 * dimension);
//        {//test
//            printf("num_vectors: %u file_bytes: %lu dimension: %u\n",
//                    num_vectors,
//                    file_bytes,
//                    dimension);
//        }
//        uint32_t count_vectors = num_vectors * ratio + 1;
//        if (f_i == num_files - 1) {
//            count_vectors = total_count - read_count;
//        }
//
//        // Read the input file
//        printf("Reading %s (%u of total %u, done %lu) ...\n",
//               filename_input.c_str(),
//               count_vectors,
//               total_count,
//               read_count);
//        fin.seekg(0, std::ios_base::beg);
//        std::unique_ptr<float[]> data = std::make_unique<float[]>(dimension);
//        for (uint32_t v_i = 0; v_i < count_vectors; ++v_i) {
//            fin.seekg(4, std::ios_base::cur);
//            fin.read(reinterpret_cast<char *>(data.get()), 4 * dimension);
//
//            // Write to the output file.
//            fout.write(reinterpret_cast<char *>(&dimension), 4);
//            fout.write(reinterpret_cast<char *>(data.get()), 4 * dimension);
//        }
//        read_count += count_vectors;
//    }
//
//    if (read_count != total_count) {
//        fprintf(stderr, "Error: read_count %u is not equal to total_count %u\n", read_count, total_count);
//        exit(EXIT_FAILURE);
//    }
//    printf("Done.\n");
//}

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <input_file> <sample_count> <output_file>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 4) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

    uint32_t sample_count = strtoull(argv[2], nullptr, 0);

    auto start = std::chrono::high_resolution_clock::now();
    generate(argv[1], sample_count, argv[3]);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> runtime = end - start;
    printf("runtime(s.): %f\n", runtime.count());

    return 0;
}
