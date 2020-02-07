//
// Created by Zhen Peng on 11/15/19.
//

#ifndef BATCH_SEARCHING_CANDIDATE_H
#define BATCH_SEARCHING_CANDIDATE_H


namespace PANNS {

struct Candidate {
    idi id_;
    distf distance_;
    bool is_checked_;

    Candidate() = default;
    Candidate(idi id, distf dist, bool is_checked) : id_(id), distance_(dist), is_checked_(is_checked)
    {}
    bool operator<(const Candidate &b) const
    {
//        return this->distance_ < b.distance_;
        if (this->distance_ != b.distance_) {
            return this->distance_ < b.distance_;
        } else {
            return this->id_ < b.id_;
        }
    }
};

} // namespace PANNS

#endif //BATCH_SEARCHING_CANDIDATE_H
