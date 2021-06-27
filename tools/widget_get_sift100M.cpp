//
// Created by Zhen Peng on 9/25/2020.
//

/*
 * SIFT1b.bvecs to SIFT100M.fvecs
 */
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
    uint64_t dimension = 0;
    fin.read(reinterpret_cast<char *>(&dimension), 4); // Read the dimension
    fin.seekg(0, std::ios_base::end);
    const uint64_t data_bytes = dimension; // SIFT1B is .bvces, so every data point is int8.
    const uint64_t element_bytes = 4 + data_bytes;
    const uint64_t file_bytes = fin.tellg();
    const uint64_t num_vectors = file_bytes / element_bytes; // Totle elements in the file.
    const uint64_t stride = num_vectors / total_count - 1; // Skip bytes after read one element

    // Sample the file
//    std::unique_ptr<float[]> data = std::make_unique<float[]>(dimension);
    std::vector<uint8_t> read_buffer(dimension);
    std::vector<float> write_buffer(dimension);
    fin.seekg(0, std::ios_base::beg);
    for (uint32_t v_i = 0; v_i < total_count; ++v_i) {
        if (0 == v_i % 20000000) {
            printf("Reading %u / %u ...\n", v_i, total_count);
        }

        // Read the file
        fin.seekg(4, std::ios_base::cur);
        fin.read(reinterpret_cast<char *>(read_buffer.data()), data_bytes);
//        fin.read(reinterpret_cast<char *>(data.get()), data_bytes);
        for (uint64_t d = 0; d < dimension; ++d) {
            write_buffer[d] = read_buffer[d];
        }

        // Write the file
        fout.write(reinterpret_cast<char *>(&dimension), 4);
        fout.write(reinterpret_cast<char *>(write_buffer.data()), sizeof(float) * dimension);
//        fout.write(reinterpret_cast<char *>(data.get()), data_bytes);

        // Skip
        fin.seekg(stride * element_bytes, std::ios_base::cur);
    }
    printf("Done.\n");
}

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
