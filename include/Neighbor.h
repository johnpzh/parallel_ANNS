//
// Created by Zhen Peng on 7/12/20.
//

#ifndef PANNS_NEIGHBOR_H
#define PANNS_NEIGHBOR_H

namespace PANNS {

struct Neighbor {
    idi id_;
    distf distance_;

    Neighbor() = default;

    Neighbor(
            idi id,
            distf dist) : id_(id), distance_(dist)
    {
        ;
    }

    bool operator<(const Neighbor &b) const
    {
        if (this->distance_ != b.distance_) {
            return this->distance_ < b.distance_;
        } else {
            return this->id_ < b.id_;
        }
    }
};

} // namespace PANNS
#endif //PANNS_NEIGHBOR_H
