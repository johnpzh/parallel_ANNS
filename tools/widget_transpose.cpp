//
// Created by Zhen Peng on 01/28/2021.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
//#include <sstream>
#include <string>
#include <vector>

void transpose(
        const char *filename_input,
        const char *filename_output,
        const int row_max,
        const int col_max)
{
    std::ifstream fin(filename_input);
    std::ofstream fout(filename_output);

    if (!fin.is_open()) {
        fprintf(stderr, "Error: cannot open file %s .\n", filename_input);
        exit(EXIT_FAILURE);
    }
    if (!fout.is_open()) {
        fprintf(stderr, "Error: cannot open file %s .\n", filename_output);
        exit(EXIT_FAILURE);
    }

    std::vector<double> numbers(row_max * col_max);

    // input
    int count = 0;
    for (double e; fin >> e; ) {
        numbers[count++] = e;
//        std::cout << e << std::endl;
    }

    // output
    for (int c_i = 0; c_i < col_max; ++c_i) {
        for (int r_i = 0; r_i < row_max; ++r_i) {
            int f_i = r_i * col_max + c_i;
            double e = numbers[f_i];
            fout << e;
            if (r_i != row_max - 1) {
                fout << '\t';
            }
        }
        fout << std::endl;
    }

}

void usage(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr,
                "Usage: %s <row_max> <col_max>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    usage(argc, argv);
    const int row_max = strtoull(argv[1], nullptr, 0);
    const int col_max = strtoull(argv[2], nullptr, 0);
//    std::cout << "Hello, World!" << std::endl;
//    const int row_max = 5;
//    const int col_max = 3;
    printf("row: %d col: %d\n", row_max, col_max);
    transpose(
            "input.txt",
            "output.txt",
            row_max,
            col_max);
    printf("Done.\n");
    return 0;
}
