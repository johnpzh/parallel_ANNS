//
// Created by Zhen Peng on 11/12/19.
//

#include "utils.h"


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
//        std::vector<dataf> &data,
//        std::vector< std::vector<dataf> > &data,
        dataf *&data,
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
    uint64_t file_size = fin.tellg();
    number = file_size / (4 + 4 * dimension);
//    data.resize(static_cast<size_t>(number) * static_cast<size_t>(dimension));
//    data.resize(number);
//    data = (dataf *) malloc(static_cast<size_t>(number) * static_cast<size_t>(dimension) * sizeof(dataf));
    data = (dataf *) _mm_malloc(static_cast<size_t>(number) * static_cast<size_t>(dimension) * sizeof(dataf), 64);
    if (!data) {
        fprintf(stderr, "Error: cannot malloc %lu bytes.\n", static_cast<size_t>(number) * static_cast<size_t>(dimension) * sizeof(dataf));
        exit(EXIT_FAILURE);
    }

    fin.seekg(0, std::ios_base::beg);
    for (size_t i = 0; i < number; i++) {
//        data[i].resize(dimension);

        fin.seekg(4, std::ios_base::cur);
//        fin.read(reinterpret_cast<char *>(&data[i * dimension]), 4 * dimension);
//        fin.read(reinterpret_cast<char *>(data[i].data()), 4 * dimension);
        fin.read(reinterpret_cast<char *>(data + i * dimension), 4 * dimension);
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

double get_time_mark()
{
    timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec * 0.000001;
}

} // namespace PANNS
