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
//    const auto the_pair = std::minmax_element(distances.begin(), distances.end());
//    const distf min_dist = *the_pair.first;
//    const distf max_dist = *the_pair.second;
//    if (min_dist == max_dist) {
//        return 0;
//    }
//    const distf dist_range = max_dist - min_dist;
//    // normalize
//    std::vector<distf> tmp_dists(distances);
//    for (distf &dist : tmp_dists) {
//        dist = (dist - min_dist) / dist_range;
//    }
//
//    double mean = std::accumulate(tmp_dists.begin(), tmp_dists.end(), 0.0) /  tmp_dists.size();
//
//    double variation = 0;
//    for (distf dist : tmp_dists) {
//        variation += (dist - mean) * (dist - mean);
//    }
//    variation /= distances.size();
//    variation = sqrt(variation);

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

idi get_index_mean(const std::vector<idi> &ids)
{
    idi sum = std::accumulate(ids.begin(), ids.end(), static_cast<idi>(0));
    return sum / ids.size();
}

} // namespace PANNS