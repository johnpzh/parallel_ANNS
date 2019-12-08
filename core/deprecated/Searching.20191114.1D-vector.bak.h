//
// Created by Zhen Peng on 11/11/19.
//

#ifndef BATCH_SEARCHING_SEARCHING_H
#define BATCH_SEARCHING_SEARCHING_H

#include <vector>
#include <boost/dynamic_bitset.hpp>
#include "../../include/definitions.h"
//#include "../include/efanna2e/neighbor.h"

namespace PANNS {

class Searching {
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
            const std::vector<dataf> &data,
            size_t loc_start,
            idi dimension);
    dataf compute_distance_with_norm(
            const std::vector<dataf> &d_data,
            const std::vector<dataf> &q_data,
            idi d_start,
            idi q_start,
            dataf d_norm,
            idi dimension);
    idi insert_into_queue_panns(
            std::vector< Candidate > &c_queue,
            idi c_queue_top,
            Candidate cand);
//    idi insert_into_queue_nsg(
//            std::vector< Candidate > &c_queue,
//            idi c_queue_top,
//            Candidate cand);

public:
    void load_data_load(char *filename);
    void load_queries_load(char *filename);
    void load_nsg_graph(char *filename);
    void prepare_init_ids(
            std::vector<unsigned> &init_ids,
            boost::dynamic_bitset<> &is_visited,
            unsigned L);
//    void prepare_candidate_queue_list(
//            const float *query_load,
//            std::vector<std::vector<efanna2e::Neighbor> > &retset_list,
//            std::vector<boost::dynamic_bitset<> > &is_visited_list,
//            const std::vector<unsigned> &init_ids,
//            const boost::dynamic_bitset<> &flags,
//            unsigned batch_start,
//            unsigned batch_size,
//            unsigned L);
    void search_in_batch(
//            const float *query_load,
            size_t K,
            size_t L,
            unsigned batch_start,
            unsigned batch_size,
            std::vector< std::vector<Candidate> > &set_L_list,
            std::vector< boost::dynamic_bitset<> > &is_visited_list,
            const std::vector<idi> &init_ids,
            const boost::dynamic_bitset<> &is_visited,
            std::vector<std::vector<idi> > &set_K_list);
    void search_in_sequential(
            idi query_start,
            size_t K,
            size_t L,
            std::vector<Candidate> &set_L,
            boost::dynamic_bitset<> is_visited,
            boost::dynamic_bitset<> &is_checked,
            const std::vector<idi> &init_ids,
            std::vector<idi> &set_K);
};

} // namespace PANNS


#endif //BATCH_SEARCHING_SEARCHING_H
