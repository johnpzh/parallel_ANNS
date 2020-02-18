//
// Created by Zhen Peng on 11/11/19.
//

#ifndef BATCH_SEARCHING_DEFINITIONS_H
#define BATCH_SEARCHING_DEFINITIONS_H

#include <utility>
#include <stdint.h>

namespace PANNS {

using dataf = float;
using distf = float;
using idi = uint32_t;
using edgei = uint64_t;
//using edgei = uint32_t;

//using Candidate = std::pair<distf, idi>; // .first: distance; .second: vertex ID
//using Candidate = std::tuple<dataf, idi, bool>; // 0: distance; 1: vertex ID; 2: flag is checked

} // namespace PANNS

#endif //BATCH_SEARCHING_DEFINITIONS_H
