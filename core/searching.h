//
// Created by Zhen Peng on 11/11/19.
//

#ifndef BATCH_SEARCHING_SEARCHING_H
#define BATCH_SEARCHING_SEARCHING_H

#include <vector>
#include <boost/dynamic_bitset.hpp>
#include "../include/definitions.h"
#include "../include/efanna2e/neighbor.h"

namespace PANNS {

class searching {
//private:
public:
    idi num_v_ = 0;
    idi num_queries_ = 0;
    dimi dimension_ = 0;

    idi width_ = 0;
    idi ep_ = 0;

    std::vector<dataf> data_load_;
    std::vector<dataf> queries_load_;
    std::vector<distf> norms_;
    std::vector< std::vector<idi> > nsg_graph_;

    dataf compute_norm(
            const std::vector<PANNS::dataf> &data,
            size_t loc_start,
            idi size);

public:
    void load_data_load(char *filename);
    void load_queries_load(char *filename);
    void load_nsg_graph(char *filename);
    void prepare_init_ids(
            std::vector<unsigned> &init_ids,
            boost::dynamic_bitset<> &flags,
            unsigned L);
    void prepare_candidate_queue_list(
            const float *query_load,
            std::vector<std::vector<efanna2e::Neighbor> > &retset_list,
            std::vector<boost::dynamic_bitset<> > &is_visited_list,
            const std::vector<unsigned> &init_ids,
            const boost::dynamic_bitset<> &flags,
            unsigned batch_start,
            unsigned batch_size,
            unsigned L);
    void search_in_batch(
            const float *query_load,
            size_t K,
            size_t L,
//            const Parameters &parameters,
            std::vector< std::vector<efanna2e::Neighbor> > &retset_list,
            std::vector< boost::dynamic_bitset<> > &is_visited_list,
//            const std::vector<unsigned> &init_ids,
//            const boost::dynamic_bitset<> &flags,
            unsigned batch_start,
            unsigned batch_size,
            std::vector<std::vector<unsigned> > &indices_list);
};

} // namespace PANNS


#endif //BATCH_SEARCHING_SEARCHING_H
