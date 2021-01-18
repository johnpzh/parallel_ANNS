//
// Created by Zhen Peng on 01/08/2020.
//

#ifndef BATCH_SEARCHING_SEARCHING_H
#define BATCH_SEARCHING_SEARCHING_H

#include <vector>
#include <boost/dynamic_bitset.hpp>
//#include <boost/sort/sort.hpp>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <immintrin.h>
#include <cstring>
#include <unordered_set>
#include <set>
#include <cfloat>
//#include <omp.h>
#include "../include/definitions.h"
//#include "../include/efanna2e/neighbor.h"
#include "../include/utils.h"
#include "../include/Candidate.h"
#include "../include/parallelization.h"
#include "../include/bitvector.h"

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
    dataf *data_load_ = nullptr;
    dataf *queries_load_ = nullptr;
//    dataf *norms_;
//    std::vector< std::vector<idi> > nsg_graph_;

//    idi *nsg_graph_indices_;
//    idi *nsg_graph_out_edges_;

//    std::vector< std::vector<idi> > edge_list_;

//    char *opt_nsg_graph_ = nullptr;
    idi *common_nsg_deg_ngbrs_ = nullptr;
    edgei *common_nsg_vertex_base_ = nullptr;

    uint64_t data_bytes_;
    uint64_t neighbor_bytes_;
    uint64_t vertex_bytes_;

    // For multithreads
    int num_threads_ = 1;
    int num_real_threads_ = 1;

    dataf compute_norm(
            const dataf *data) const;
//          idi vertex_id);
//            const std::vector<PANNS::dataf> &data);
//        size_t loc_start,
//        idi dimension)
    dataf compute_distance_with_norm(
            const dataf *v_data,
            const dataf *q_data,
            const dataf vertex_norm) const;
    dataf compute_distance(
            const dataf *v_data,
            const dataf *q_data) const;
    static idi insert_into_queue(
            std::vector<Candidate> &c_queue,
            idi c_queue_top,
            Candidate cand);
    static idi add_into_queue(
            std::vector<PANNS::Candidate> &queue,
            idi &queue_top,
            const idi queue_size,
            const PANNS::Candidate &cand);
    static idi add_into_queue(
            std::vector<PANNS::Candidate> &queue,
            const idi queue_start,
            idi &queue_top,
            const idi queue_size,
            const PANNS::Candidate &cand);
    static void add_into_queue_at(
            const Candidate &cand,
            std::vector<Candidate> &queue,
            const idi insert_index, // The insertion location, independent with queue_start
            const idi queue_start,
            idi &queue_top, // The number of elements in queue, independent with queue_start
            const idi queue_size); // The maximum capacity of queue, independent with queue_start.
    static void insert_one_element_at(
//        const T &cand,
//        T *queue_base,
            const Candidate &cand,
            std::vector<Candidate> &queue_base,
            const idi insert_index,
            const idi queue_start,
            const idi queue_size);
//    idi insert_into_queue_nsg(
//            std::vector< Candidate > &c_queue,
//            idi c_queue_top,
//            Candidate cand);
    static idi merge_two_queues_into_1st_queue_seq_fixed(
            std::vector<Candidate> &queue1,
            const idi queue1_start,
            const idi queue1_size,
            std::vector<Candidate> &queue2,
            const idi queue2_start,
            const idi queue2_size);
    static void merge_two_queues_into_1st_queue_seq_incr(
            std::vector<Candidate> &queue1,
            const idi queue1_start,
            idi &queue1_size, // The number of element in queue1, independent with queue1_start.
            const idi queue1_length, // The maximum capacity of queue1, independent with queue1_start.
            std::vector<Candidate> &queue2,
            const idi queue2_start,
            const idi queue2_size);
    idi merge_all_queues_para_list(
            std::vector< std::vector<Candidate> > &local_queues_list,
            std::vector<idi> &local_queues_ends,
            std::vector<Candidate> &set_L,
            const idi L);
//    idi merge_all_queues_para_array(
////        std::vector< std::vector<Candidate> > &local_queues_list,
//            std::vector<Candidate> &local_queues_array,
//            std::vector<idi> &local_queues_ends,
//            const idi local_queue_length,
//            std::vector<Candidate> &set_L,
//            const idi L);
    idi merge_all_queues_para_array(
            std::vector<Candidate> &set_L,
//        std::vector<Candidate> &local_queues_array,
            std::vector<idi> &local_queues_ends,
            const idi local_queue_length,
//        std::vector<Candidate> &set_L,
            const idi L);
    idi merge_all_queues_seq(
            std::vector<Candidate> &set_L,
            std::vector<idi> &local_queues_ends,
            const idi local_queue_length,
            const idi L);
    idi merge_all_queues_queue_base(
//        std::vector< std::vector<Candidate> > &local_queues_list,
            std::vector<Candidate> &set_L,
//        std::vector<Candidate> &local_queues_array,
            std::vector<idi> &local_queues_ends,
            const idi queue_base,
            const int real_threads,
            const idi local_queue_length,
//        std::vector<Candidate> &set_L,
            const idi L);
//    idi merge_all_queues_all_together_in_sequential(
//            std::vector<Candidate> &set_L,
//            std::vector<idi> &local_queues_ends,
//            const idi local_queue_length,
//            const idi L);
    idi min_all_queues_at_heads(
            const std::vector<Candidate> &set_L,
            std::vector<idi> &queue_heads,
            const std::vector<idi> &local_queues_ends,
            const idi local_queue_length,
            const idi L);


public:
    // For Profiling
//    L3CacheMissRate cache_miss_kernel;
    uint64_t count_distance_computation_ = 0;
    uint64_t count_merge_ = 0;
    double time_expand_ = 0.0;
    double time_merge_ = 0.0;
//    uint64_t count_single_query_computation_ = 0;
//    distf dist_min_ = 0;
//    distf dist_max_ = 0;
//    double time_merge_ = 0;
//    double time_initialization_ = 0;
//    double time_sequential_phase_ = 0;
//    double time_parallel_phase_ = 0;
//    double time_insert_ = 0;
//    double time_compare_minimum_ = 0;
//    L3CacheMissRate profile_miss_rate;

    ~Searching()
    {
        free(data_load_);
        free(queries_load_);
//        free(opt_nsg_graph_);
        free(common_nsg_vertex_base_);
        free(common_nsg_deg_ngbrs_);
    }
    void load_data_load(char *filename);
    void load_queries_load(char *filename);
    void load_common_nsg_graph(char *filename);
//    void load_nsg_graph(char *filename);
//    void build_opt_graph();
    void prepare_init_ids(
            std::vector<unsigned> &init_ids,
            const unsigned L) const;
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
//    void search_in_sequential(
//            idi query_id,
//            idi K,
//            idi L,
//            std::vector<Candidate> &set_L,
////            boost::dynamic_bitset<> &is_visited,
////            boost::dynamic_bitset<> is_visited,
////            std::vector<idi> &init_ids,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K);
//    void search_in_sequential_BitVector(
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K);

//    idi get_out_degree(idi v_id) const
//    {
//        if (v_id < num_v_ - 1) {
//            return nsg_graph_indices_[v_id + 1] - nsg_graph_indices_[v_id];
//        } else {
//            return num_e_ - nsg_graph_indices_[v_id];
//        }
//    }

//    void search_with_top_m(
//            idi M,
//            idi query_id,
//            idi K,
//            idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K);
////            std::vector< std::vector<idi> > &top_m_list);
//    void search_with_top_m_scale_m(
//            const PANNS::idi value_M_max,
//            const PANNS::idi query_id,
//            const PANNS::idi K,
//            const PANNS::idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            std::vector<idi> &top_m_candidates,
//            boost::dynamic_bitset<> &is_visited);
//    void search_with_top_m_in_batch(
//            PANNS::idi M,
//            PANNS::idi batch_start,
//            PANNS::idi batch_size,
//            PANNS::idi K,
//            PANNS::idi L,
//            std::vector< std::vector<Candidate> > &set_L_list,
//            const std::vector<idi> &init_ids,
//            std::vector< std::vector<idi> > &set_K_list);
//    void test_neighbors_distance_to_father(
//            const idi num_selected) const;
//    void test_neighbors_normalized_distance_to_father(
//            const idi num_selected) const;


    void load_true_NN(
            const char *filename,
            std::vector< std::vector<idi> > &true_nn_list);
    void get_recall_for_all_queries(
            const std::vector< std::vector<idi> > &true_nn_list,
            const std::vector<std::vector<unsigned>> &set_K_list,
            std::unordered_map<unsigned, double> &recalls,
            const idi L) const;

//    void para_search_with_top_m_merge_queues_middle_m(
//            const idi value_M_middle,
    void para_search_with_scale_m_large_graph_profiling(
            const idi value_M_max,
            const idi query_id,
            const idi K,
            const idi L,
            std::vector<Candidate> &set_L,
            const std::vector<idi> &init_ids,
            std::vector<idi> &set_K,
            const idi local_queue_length, // Maximum size of local queue
            const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
            std::vector<idi> &local_queues_ends, // Sizes of local queue
//        std::vector<Candidate> &top_m_candidates,
            std::vector<idi> &top_m_candidates,
//        std::vector<uint8_t> &is_visited)
            boost::dynamic_bitset<> &is_visited);
//            std::vector<distf> &local_thresholds);
//        BitVector &is_visited)
}; // Class Searching

} // namespace PANNS


#endif //BATCH_SEARCHING_SEARCHING_H
