//
// Created by Zhen Peng on 11/12/19.
//

#include "utils.h"
#include <fstream>

namespace PANNS {

/**
 * load data with sift10K pattern: 1 dimension number (4 bytes), float data (4 * dimension bytes), [repeat].
 * Reference: https://github.com/ZJULearning/nsg/blob/master/tests/test_nsg_optimized_search.cpp
 * @param filename
 * @param[out] data
 * @param[out] number
 * @param[out] dimension
 */
void load_data(
        char *filename,
        std::vector<dataf> &data,
        idi &number,
        dimi &dimension)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    fin.read(reinterpret_cast<char *>(&dimension), 4); // Read the dimension
    fin.seekg(0, std::ios_base::end);
//    std::ios::pos_type ss = in.tellg();
//    size_t fsize = (size_t) ss;
    uint64_t file_size = fin.tellg();
//    idi num_v = (unsigned) (fsize / (dim + 1) / 4);
    number = file_size / (4 + 4 * dimension);
//    data = (float *) _mm_malloc((size_t) num * (size_t) dim * sizeof(float), 64);
//    data = new float[(size_t) num * (size_t) dim];
    data.resize(static_cast<size_t>(number) * static_cast<size_t>(dimension));

    fin.seekg(0, std::ios_base::beg);
    for (size_t i = 0; i < number; i++) {
        fin.seekg(4, std::ios_base::cur);
        fin.read(reinterpret_cast<char *>(&data[i * dimension]), 4 * dimension);
//        fin.read((char *) (data + i * dim), dim * 4);
    }
}

/**
 * Save candidates ID for all queries.
 * Reference: https://github.com/ZJULearning/nsg/blob/master/tests/test_nsg_optimized_search.cpp
 * @param filename
 * @param results
 */
void save_result(const char *filename,
                 const std::vector<std::vector<unsigned>> &results)
{
    std::ofstream fout(filename);

//    for (size_t i = 0; i < results.size(); i++) {
//        unsigned GK = results[i].size();
//        fout.write(reinterpret_cast<char *>(&GK), sizeof(unsigned));
//        fout.write(reinterpret_cast<const char *>(results[i].data()), GK * sizeof(unsigned));
    for (const auto &qs : results) {
        unsigned GK = qs.size();
        fout.write(reinterpret_cast<char *>(&GK), sizeof(unsigned));
        fout.write(reinterpret_cast<const char *>(qs.data()), GK * sizeof(unsigned));
    }
}

} // namespace PANNS
