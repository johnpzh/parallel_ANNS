//
// Created by Zhen Peng on 3/17/21.
//
#include <iostream>
#include <fstream>

void save_fake_reorder_map(
        const uint32_t num_v,
        const char *filename)
{
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        fprintf(stderr,
                "Error: cannot create file %s .\n",
                filename);
        exit(EXIT_FAILURE);
    }

    for (uint32_t v_i = 0; v_i < num_v; ++v_i) {
        fout.write((char *) (&v_i), sizeof(uint32_t));
        fout.write((char *) (&v_i), sizeof(uint32_t));
    }
}

void usage(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr,
                "Usage: %s <num_v> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    usage(argc, argv);
    const uint32_t num_v = strtoull(argv[1], nullptr, 0);
    const char *output_file = argv[2];

    save_fake_reorder_map(
            num_v,
            output_file);

    return EXIT_SUCCESS;
}

