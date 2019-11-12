//
// Created by Zhen Peng on 11/11/19.
//

#include <iostream>
#include <fstream>
#include <immintrin.h>
#include "searching.h"
#include "../include/utils.h"

namespace PANNS {

/**
 * Input the data from the file.
 * @param filename
 */
void searching::load_data_load(char *filename)
{
    auto old_d = dimension_;
    load_data(
            filename,
            data_load_,
            num_v_,
            dimension_);
    if (old_d) {
        if (old_d != dimension_) {
            std::cerr << "Error: data dimension " << dimension_
                    << " is not equal to query dimension " << old_d << "." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * Input queries from the file.
 * @param filename
 */
void searching::load_queries_load(char *filename)
{
    auto old_d = dimension_;
    load_data(
            filename,
            queries_load_,
            num_queries_,
            dimension_);
    if (old_d) {
        if (old_d != dimension_) {
            std::cerr << "Error: query dimension " << dimension_
                    << " is not equal to data dimension " << old_d << "." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * Input the NSG graph from the file.
 * Reference: https://github.com/ZJULearning/nsg/blob/master/src/index_nsg.cpp
 * @param filename
 */
void searching::load_nsg_graph(char *filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "Error: cannot read file " << filename << " ." << std::endl;
        exit(EXIT_FAILURE);
    }
    fin.read(reinterpret_cast<char *>(&width_), sizeof(unsigned));
    fin.read(reinterpret_cast<char *>(&ep_), sizeof(unsigned));

    while (true) {
        idi degree;
        fin.read(reinterpret_cast<char *>(&degree), sizeof(unsigned));
        if (fin.eof()) {
            break;
        }
        std::vector<idi> ngbrs(degree);
        fin.read(reinterpret_cast<char *>(ngbrs.data()), degree * sizeof(unsigned));
        nsg_graph_.push_back(ngbrs);
    }

    // Compute norms
    norms_.resize(num_v_);
    for (idi v_i = 0; v_i < num_v_; ++v_i) {
        norms_[v_i] = compute_norm(data_load_, v_i * dimension_, dimension_);
    }
}

// TODO: re-code in AVX-512
dataf searching::compute_norm(
        const std::vector<PANNS::dataf> &data,
        size_t loc_start,
        idi size)
{
    const dataf *a = data.data() + loc_start;
//    idi size = dimension_;
    dataf result = 0;
#define AVX_L2NORM(addr, dest, tmp) \
    tmp = _mm256_loadu_ps(addr); \
    tmp = _mm256_mul_ps(tmp, tmp); \
    dest = _mm256_add_ps(dest, tmp);

    __m256 sum;
    __m256 l0, l1;
    unsigned D = (size + 7) & ~7U;
    unsigned DR = D % 16;
    unsigned DD = D - DR;
    const float *l = a;
    const float *e_l = l + DD;
    float unpack[8] __attribute__ ((aligned (32))) = {0, 0, 0, 0, 0, 0, 0, 0};

    sum = _mm256_loadu_ps(unpack);
    if(DR){AVX_L2NORM(e_l, sum, l0);}
    for (unsigned i = 0; i < DD; i += 16, l += 16) {
        AVX_L2NORM(l, sum, l0);
        AVX_L2NORM(l + 8, sum, l1);
    }
    _mm256_storeu_ps(unpack, sum);
    result = unpack[0] + unpack[1] + unpack[2] + unpack[3] + unpack[4] + unpack[5] + unpack[6] + unpack[7];

    return result;
}

} // namespace PANNS
