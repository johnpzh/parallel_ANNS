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
using dimi = uint32_t;
using Candidate = std::pair<distf, idi>; // .first: distance; .second: vertex ID

} // namespace PANNS

#endif //BATCH_SEARCHING_DEFINITIONS_H
