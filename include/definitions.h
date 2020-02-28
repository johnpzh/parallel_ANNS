//
// Created by Zhen Peng on 11/11/19.
//

#ifndef BATCH_SEARCHING_DEFINITIONS_H
#define BATCH_SEARCHING_DEFINITIONS_H

#include <utility>
#include <stdint.h>
#include <limits>

namespace PANNS {

using dataf = float;
using distf = float;
const distf DISTF_MAX = std::numeric_limits<distf>::max();
const distf DISTF_MIN = std::numeric_limits<distf>::min();
using idi = uint32_t;
using edgei = uint64_t;
//using edgei = uint32_t;

//using Candidate = std::pair<distf, idi>; // .first: distance; .second: vertex ID
//using Candidate = std::tuple<dataf, idi, bool>; // 0: distance; 1: vertex ID; 2: flag is checked

} // namespace PANNS

#endif //BATCH_SEARCHING_DEFINITIONS_H
