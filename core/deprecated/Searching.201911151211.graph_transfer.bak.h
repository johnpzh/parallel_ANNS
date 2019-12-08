//
// Created by Zhen Peng on 11/11/19.
//

#ifndef BATCH_SEARCHING_SEARCHING_H
#define BATCH_SEARCHING_SEARCHING_H

#include <vector>
#include <boost/dynamic_bitset.hpp>
#include <iostream>
#include <fstream>
#include <immintrin.h>
#include <cstring>
#include "../../include/definitions.h"
//#include "../include/efanna2e/neighbor.h"
#include "../../include/utils.h"

namespace PANNS {

class Searching {
//private:
public:
    idi num_v_ = 0;
    edgei num_e_ = 0;
    idi num_queries_ = 0;
    uint64_t dimension_ = 0;

    idi width_ = 0; // NSG largest degree
    idi ep_ = 0; // Start point

//    std::vector<dataf> data_load_;
//    std::vector<dataf> queries_load_;
//    std::vector< std::vector<dataf> > data_load_;
//    std::vector< std::vector<dataf> > queries_load_;
//    std::vector<distf> norms_;
    dataf *data_load_;
    dataf *queries_load_;
//    dataf *norms_;
//    std::vector< std::vector<idi> > nsg_graph_;

//    idi *nsg_graph_indices_;
//    idi *nsg_graph_out_edges_;

    std::vector< std::vector<idi> > edge_list_;

    char *opt_nsg_graph_;
    uint64_t data_bytes_;
    uint64_t neighbor_bytes_;
    uint64_t vertex_bytes_;


    dataf compute_norm(
          idi vertex_id);
//            const std::vector<PANNS::dataf> &data);
//        size_t loc_start,
//        idi dimension)
    dataf compute_distance_with_norm(
            idi vertex_id,
            idi query_id,
//            const std::vector<dataf> &d_data,
//            const std::vector<dataf> &q_data,
//        PANNS::idi d_start,
//        PANNS::idi q_start,
            dataf vertex_norm);
//        idi dimension)
    static idi insert_into_queue_panns(
            std::vector< Candidate > &c_queue,
            idi c_queue_top,
            Candidate cand);
//    idi insert_into_queue_nsg(
//            std::vector< Candidate > &c_queue,
//            idi c_queue_top,
//            Candidate cand);

public:
    ~Searching()
    {
//        free(data_load_);
//        free(queries_load_);
//        _mm_free(data_load_);
        free(queries_load_);
//        free(norms_);
//        free(nsg_graph_indices_);
//        free(nsg_graph_out_edges_);

        free(opt_nsg_graph_);
    }
    void load_data_load(char *filename);
    void load_queries_load(char *filename);
    void load_nsg_graph(char *filename);
    void build_opt_graph();
    void prepare_init_ids(
            std::vector<unsigned> &init_ids,
            boost::dynamic_bitset<> &is_visited,
            unsigned L) const;
//    void prepare_candidate_queue_list(
//            const float *query_load,
//            std::vector<std::vector<efanna2e::Neighbor> > &retset_list,
//            std::vector<boost::dynamic_bitset<> > &is_visited_list,
//            const std::vector<unsigned> &init_ids,
//            const boost::dynamic_bitset<> &flags,
//            unsigned batch_start,
//            unsigned batch_size,
//            unsigned L);
//    void search_in_batch(
////            const float *query_load,
//            size_t K,
//            size_t L,
//            unsigned batch_start,
//            unsigned batch_size,
//            std::vector< std::vector<Candidate> > &set_L_list,
//            std::vector< boost::dynamic_bitset<> > &is_visited_list,
//            const std::vector<idi> &init_ids,
//            const boost::dynamic_bitset<> &is_visited,
//            std::vector<std::vector<idi> > &set_K_list);
    void search_in_sequential(
            idi query_start,
            idi K,
            idi L,
            std::vector<Candidate> &set_L,
            boost::dynamic_bitset<> is_visited,
//            boost::dynamic_bitset<> &is_checked,
            const std::vector<idi> &init_ids,
            std::vector<idi> &set_K);

//    idi get_out_degree(idi v_id) const
//    {
//        if (v_id < num_v_ - 1) {
//            return nsg_graph_indices_[v_id + 1] - nsg_graph_indices_[v_id];
//        } else {
//            return num_e_ - nsg_graph_indices_[v_id];
//        }
//    }
};

} // namespace PANNS


#endif //BATCH_SEARCHING_SEARCHING_H
