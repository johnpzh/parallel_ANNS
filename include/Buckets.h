//
// Created by Zhen Peng on 2/25/20.
//

#ifndef PANNS_BUCKETS_H
#define PANNS_BUCKETS_H

#include <vector>
#include "Candidate.h"
#include "definitions.h"

namespace PANNS {

class CandidatesBuckets {
private:
    std::vector< std::vector<Candidate> > buckets_;
public:
    idi num_buckets_;
    distf buckets_lower_;
    distf buckets_upper_;
    distf overall_width_;
    distf bucket_width_;

    CandidatesBuckets() = default;
    CandidatesBuckets(idi num_buckets,
                      distf buckets_lower,
                      distf buckets_upper) :
                      num_buckets_(num_buckets),
                      buckets_lower_(buckets_lower),
                      buckets_upper_(buckets_upper)
    {
        overall_width_ = buckets_upper_ - buckets_lower_;
        bucket_width_ = overall_width_ / num_buckets_;
        buckets_.resize(num_buckets_);
    }

    void set_up(
            idi num_buckets,
            distf buckets_lower,
            distf buckets_upper)
    {
        num_buckets_ = num_buckets;
        buckets_lower_ = buckets_lower;
        buckets_upper_ = buckets_upper;
        overall_width_ = buckets_upper_ - buckets_lower_;
        bucket_width_ = overall_width_ / num_buckets_;
        buckets_.resize(num_buckets_);
    }

    void push_in(const Candidate &cand)
    {
        idi b_i = (cand.distance_ - buckets_lower_) / bucket_width_;
        buckets_[b_i].push_back(cand);
    }
};

}

#endif //PANNS_BUCKETS_H
