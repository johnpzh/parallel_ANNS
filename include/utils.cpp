//
// Created by Zhen Peng on 11/5/2021.
//
#include "utils.h"

namespace PANNS {

double get_dist_variation(const std::vector<distf> &distances)
{
    double mean = std::accumulate(distances.begin(), distances.end(), 0.0) /  distances.size();

    double variation = 0;
    for (distf dist : distances) {
        variation += (dist - mean) * (dist - mean);
    }
    variation /= distances.size();

    return variation;
}

double get_dist_standard_deviation(const std::vector<distf> &distances)
{
    return sqrt(get_dist_variation(distances));
}

double get_normalized_std_dev(const std::vector<distf> &distances)
{
    double mean = std::accumulate(distances.begin(), distances.end(), 0.0) /  distances.size();

    double variation = 0;
    for (distf dist : distances) {
        variation += (dist - mean) * (dist - mean);
    }
    variation /= distances.size();
    variation = sqrt(variation);
    variation /= mean;

    return variation;
}

idi get_index_median(const std::vector<idi> &ids)
{
    std::vector<idi> tmp_ids(ids);
    std::sort(tmp_ids.begin(), tmp_ids.end());

    return tmp_ids[tmp_ids.size() / 2];
}

} // namespace PANNS