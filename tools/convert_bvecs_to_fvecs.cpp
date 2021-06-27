//
// Created by Zhen Peng on 10/01/2020.
//

/*
 * SIFT100M_query.bvecs to SIFT100M_query.fvecs
 */
#include <iostream>
#include <cstdio>
#include <chrono>
#include <clocale>
#include <fstream>
#include <memory>
#include <vector>


void convert(
        char *filename_input,
        char *filename_output)
{
    std::ifstream fin(filename_input);
    if (!fin.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename_output);
        exit(EXIT_FAILURE);
    }
    std::ofstream fout(filename_output);
    if (!fout.is_open()) {
        fprintf(stderr, "Error: cannot create file %s\n", filename_output);
        exit(EXIT_FAILURE);
    }

    uint64_t dimension = 0;
    fin.read(reinterpret_cast<char *>(&dimension), 4); // Read the dimension
    fin.seekg(0, std::ios_base::end);
    const uint64_t data_bytes = dimension;
    const uint64_t element_bytes = 4 + data_bytes;
    const uint64_t file_bytes = fin.tellg();
    const uint64_t num_vectors = file_bytes / element_bytes; // Totle elements in the file.
    printf("num_vectors: %lu\n", num_vectors);

    // Sample the file
    std::vector<uint8_t> read_buffer(dimension);
    std::vector<float> write_buffer(dimension);
    fin.seekg(0, std::ios_base::beg);
    for (uint32_t v_i = 0; v_i < num_vectors; ++v_i) {
//        if (0 == v_i % 20000000) {
//            printf("Reading %u / %u ...\n", v_i, total_count);
//        }

        // Read the file
        fin.seekg(4, std::ios_base::cur);
        fin.read(reinterpret_cast<char *>(read_buffer.data()), data_bytes);
        for (uint64_t d = 0; d < dimension; ++d) {
            write_buffer[d] = read_buffer[d];
        }

        // Write the file
        fout.write(reinterpret_cast<char *>(&dimension), 4);
        fout.write(reinterpret_cast<char *>(write_buffer.data()), sizeof(float) * dimension);

    }
}

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <input_file> <output_file>\n",
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

    auto start = std::chrono::high_resolution_clock::now();
    convert(argv[1], argv[2]);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> runtime = end - start;
    printf("Done, %f sec.\n", runtime.count());

    return 0;
}
