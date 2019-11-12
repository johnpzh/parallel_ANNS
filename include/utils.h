//
// Created by Zhen Peng on 11/11/19.
//

#ifndef BATCH_SEARCHING_UTILS_H
#define BATCH_SEARCHING_UTILS_H

#include <vector>
#include "definitions.h"

namespace PANNS {

void load_data(
        char *filename,
        std::vector<dataf> &data,
        idi &number,
        dimi &dimension);

void save_result(const char *filename,
                 const std::vector<std::vector<unsigned>> &results);

} // PANNS

#endif //BATCH_SEARCHING_UTILS_H
