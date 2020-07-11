//
// Created by Zhen Peng on 6/22/2020.
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
#include <algorithm>
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
    int dimension_ = 0;

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

    char *opt_nsg_graph_ = nullptr;
    uint64_t data_bytes_;
    uint64_t neighbor_bytes_;
    uint64_t vertex_bytes_;

    // For multithreads
    int num_threads_ = 1;
//    int num_real_threads_ = 1;
//    int num_threads_intra_query_ = 1;
//    int num_threads_inter_query_ = 1;

    dataf compute_norm(
            const dataf *data) const;
//          idi vertex_id);
//            const std::vector<PANNS::dataf> &data);
//        size_t loc_start,
//        idi dimension)
    dataf compute_distance_with_norm(
            const dataf *v_data,
            const dataf *q_data,
//            idi vertex_id,
//            idi query_id,
//            const std::vector<dataf> &d_data,
//            const std::vector<dataf> &q_data,
//        PANNS::idi d_start,
//        PANNS::idi q_start,
            const dataf vertex_norm) const;
//        idi dimension)
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
    void merge_two_consecutive_queues_in_place(
            std::vector<Candidate> &two_queues,
            const idi base_1,
//            const idi &end_1,
            const idi base_2,
            const idi &length_2);
    void merge_in_set_L(
            std::vector<Candidate> &set_L,
            const idi set_L_length,
            const idi num_queues,
            const idi local_queue_length);
    distf selecting_top_L_seq(
            std::vector<Candidate> &set_L,
            const idi global_L,
//            const idi local_L,
            const idi num_queues,
            const std::vector<idi> &local_queues_bases,
            std::vector<idi> &local_queues_ends);
//    idi merge_all_queues_all_together_in_sequential(
//            std::vector<Candidate> &set_L,
//            std::vector<idi> &local_queues_ends,
//            const idi local_queue_length,
//            const idi L);
//    idi min_all_queues_at_heads(
//            const std::vector<Candidate> &set_L,
//            std::vector<idi> &queue_heads,
//            const std::vector<idi> &local_queues_ends,
//            const idi local_queue_length,
//            const idi L);


public:
    // For Profiling
//    L3CacheMissRate cache_miss_kernel;
    uint64_t count_distance_computation_ = 0;
//    uint64_t count_single_query_computation_ = 0;
//    distf dist_min_ = 0;
//    distf dist_max_ = 0;
    double time_merge_ = 0;
//    double time_initialization_ = 0;
//    double time_sequential_phase_ = 0;
//    double time_parallel_phase_ = 0;
//    double time_insert_ = 0;
//    double time_compare_minimum_ = 0;
//    double time_memmove_ = 0;
//    std::vector<double> time_memmove_list_;
//    L3CacheMissRate profile_miss_rate;
//    uint64_t number_local_elements_ = 0;

    ~Searching()
    {
        free(data_load_);
        data_load_ = nullptr;
//        free(queries_load_);
//        _mm_free(data_load_);
        free(queries_load_);
        queries_load_ = nullptr;
//        free(norms_);
//        free(nsg_graph_indices_);
//        free(nsg_graph_out_edges_);
        free(opt_nsg_graph_);
        opt_nsg_graph_ = nullptr;
    }
    void load_data_load(char *filename);
    void load_queries_load(char *filename);
    void load_nsg_graph(char *filename);
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
    void search_in_sequential(
            idi query_id,
            idi K,
            idi L,
            std::vector<Candidate> &set_L,
//            boost::dynamic_bitset<> &is_visited,
//            boost::dynamic_bitset<> is_visited,
//            std::vector<idi> &init_ids,
            const std::vector<idi> &init_ids,
            std::vector<idi> &set_K);
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

    void search_with_top_m(
            idi M,
            idi query_id,
            idi K,
            idi L,
            std::vector<Candidate> &set_L,
            const std::vector<idi> &init_ids,
            std::vector<idi> &set_K);
//            std::vector< std::vector<idi> > &top_m_list);
    void search_with_top_m_scale_m(
            const PANNS::idi value_M_max,
            const PANNS::idi query_id,
            const PANNS::idi K,
            const PANNS::idi L,
            std::vector<Candidate> &set_L,
            const std::vector<idi> &init_ids,
            std::vector<idi> &set_K,
            std::vector<idi> &top_m_candidates,
            boost::dynamic_bitset<> &is_visited);
//    void search_with_top_m_myths_M(
//            const PANNS::idi M,
//            const PANNS::idi query_id,
//            const PANNS::idi K,
//            const PANNS::idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K);
//    void search_with_top_m_to_get_distance_range(
//            const PANNS::idi M,
//            const PANNS::idi query_id,
////        const PANNS::idi K,
//            const PANNS::idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids);
//    void search_with_top_m_profile_bit_CAS(
//            const PANNS::idi M,
//            const PANNS::idi query_id,
//            const PANNS::idi K,
//            const PANNS::idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K);

//    void search_with_top_m_no_local_arrays(
//            const PANNS::idi M,
//            const PANNS::idi query_id,
//            const PANNS::idi K,
//            const PANNS::idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            boost::dynamic_bitset<> &is_visited);
    void search_with_top_m_in_batch(
            PANNS::idi M,
            PANNS::idi batch_start,
            PANNS::idi batch_size,
            PANNS::idi K,
            PANNS::idi L,
            std::vector< std::vector<Candidate> > &set_L_list,
            const std::vector<idi> &init_ids,
            std::vector< std::vector<idi> > &set_K_list);
//    void para_search_with_top_m_critical_area(
//            idi M,
//            idi query_id,
//            idi K,
//            idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K);
//    void para_search_with_top_m_critical_area_no_omp(
//            idi M,
//            idi query_id,
//            idi K,
//            idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K);
//    void para_search_with_top_m_critical_area_yes_omp(
//            idi M,
//            idi query_id,
//            idi K,
//            idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K);
//    void para_search_with_top_m_visited_array(
//            const PANNS::idi M,
//            const PANNS::idi query_id,
//            const PANNS::idi K,
//            const PANNS::idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            std::vector<uint8_t> &is_visited);
//    void para_search_with_top_m_merge_queues(
//            const idi M,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K);
//    void para_search_with_top_m_queues_seq_merge(
//            const PANNS::idi M,
//            const PANNS::idi query_id,
//            const PANNS::idi K,
//            const PANNS::idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K);
//    void para_search_with_top_m_merge_queues_no_CAS(
//            const idi M,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length,
//            std::vector< std::vector<Candidate> > &local_queues_list,
//            std::vector<idi> &local_queues_ends,
////            std::vector<uint8_t> &is_visited);
//            boost::dynamic_bitset<> &is_visited);
//    void para_search_with_top_m_merge_queues_in_array(
//    void para_search_with_top_m_merge_queues_new_threshold(
//            const idi M,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
////        std::vector< std::vector<Candidate> > &local_queues_list,
//            std::vector<Candidate> &local_queues_array,
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
//            BitVector &is_visited);
//    void para_search_with_top_m_merge_queues_by_sort(
//            const idi M,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
////        std::vector<Candidate> &local_queues_array,
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
//            std::vector<idi> &dest_offsets,
//            const std::vector<idi> &offsets_load_set_L, // Offsets for store into set_L.
//            BitVector &is_visited);
//    void para_search_with_top_m_merge_queues_better_merge_v0(
//            const idi M,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
////            std::vector<Candidate> &top_m_candidates,
//            std::vector<idi> &top_m_candidates,
////            std::vector<uint8_t> &is_visited);
//            boost::dynamic_bitset<> &is_visited);
////            BitVector &is_visited);
//    void para_search_with_top_m_merge_queues_better_merge_v2(
//            const idi M,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
////        std::vector<Candidate> &top_m_candidates,
//            std::vector<idi> &top_m_candidates,
////        std::vector<uint8_t> &is_visited)
//            boost::dynamic_bitset<> &is_visited,
//            std::vector<distf> &local_thresholds);
////        BitVector &is_visited)
//    void para_search_with_top_m_merge_queues_better_merge_v1(
//            const idi M,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
//            std::vector<Candidate> &top_m_candidates,
////            std::vector<idi> &top_m_candidates,
////            std::vector<uint8_t> &is_visited);
//            boost::dynamic_bitset<> &is_visited);
////            BitVector &is_visited);
//    void para_search_with_top_m_merge_queues_better_merge_v0_0(
//            const idi M,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
////        std::vector<Candidate> &top_m_candidates,
//            std::vector<idi> &top_m_candidates,
////        std::vector<uint8_t> &is_visited)
//            boost::dynamic_bitset<> &is_visited);
////        BitVector &is_visited)
//    void para_search_with_top_m_merge_queues_less_merge(
//            const idi M,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
////        std::vector<Candidate> &top_m_candidates,
//            std::vector<idi> &top_m_candidates,
////        std::vector<uint8_t> &is_visited)
//            boost::dynamic_bitset<> &is_visited,
//            std::vector<distf> &local_thresholds);
////        BitVector &is_visited)
//    void para_search_with_top_m_merge_queues_no_merge(
//            const idi M,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
////        std::vector<Candidate> &top_m_candidates,
//            std::vector<idi> &top_m_candidates,
////        std::vector<uint8_t> &is_visited)
//            boost::dynamic_bitset<> &is_visited,
//            std::vector<distf> &local_thresholds,
//            const uint64_t computation_threshold);
//    void para_search_with_top_m_merge_queues_scale_m_v0(
//            const idi value_M_max,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
////        std::vector<Candidate> &top_m_candidates,
//            std::vector<idi> &top_m_candidates,
////            std::vector<uint8_t> &is_visited);
//            boost::dynamic_bitset<> &is_visited);
    void para_search_with_top_m_merge_queues_middle_m(
            const idi value_M_middle,
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
//    void para_search_with_top_m_merge_queues_scale_m_v2(
//            const idi value_M_min,
//            const idi value_M_max,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
////        std::vector<Candidate> &top_m_candidates,
//            std::vector<idi> &top_m_candidates,
////        std::vector<uint8_t> &is_visited)
//            boost::dynamic_bitset<> &is_visited);
//    void para_search_with_top_m_merge_queues_scale_m_v3(
//            const idi value_M_middle,
//            const idi value_M_max,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
////        std::vector<Candidate> &top_m_candidates,
//            std::vector<idi> &top_m_candidates,
////        std::vector<uint8_t> &is_visited)
//            boost::dynamic_bitset<> &is_visited);
    void para_search_with_top_m_merge_queues_middle_m_no_merge(
            const uint64_t computation_threshold,
            const idi value_M_middle,
            const idi value_M_max,
            const idi query_id,
            const idi K,
            const idi L,
            const idi init_size,
            std::vector<Candidate> &set_L,
            const std::vector<idi> &init_ids,
            std::vector<idi> &set_K,
            const idi local_queue_length, // Maximum size of local queue
            const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
            std::vector<idi> &local_queues_ends, // Sizes of local queue
            std::vector<idi> &top_m_candidates,
            boost::dynamic_bitset<> &is_visited);
    void para_search_with_top_m_merge_queues_sequential_merge(
            const idi value_M_middle,
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
            std::vector<idi> &top_m_candidates,
            boost::dynamic_bitset<> &is_visited);
    void para_search_with_top_m_nested_para(
            const idi batch_start,
            const idi batch_size,
            const idi value_M_middle,
            const idi value_M_max,
            const idi K,
            const idi L,
            std::vector< std::vector<Candidate> > &set_L_list,
            const std::vector<idi> &init_ids,
            std::vector< std::vector<idi> > &set_K_list,
            const idi local_queue_length, // Maximum size of local queue
            const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
            std::vector< std::vector<idi> > &local_queues_ends_list, // Sizes of local queue
            std::vector< std::vector<idi> > &top_m_candidates_list,
            std::vector< boost::dynamic_bitset<> > &is_visited_list);
    void subsearch_with_top_m(
            const idi value_M_max,
            const idi query_id,
            const idi local_L,
            std::vector<Candidate> &set_L,
            const idi base_set_L,
            idi &set_L_end,
            std::vector<idi> &local_top_m_candidates,
            boost::dynamic_bitset<> &is_visited,
            uint64_t &local_count_distance_computation);
    void subsearch_top_m_for_one_iteration(
            const idi iter,
            idi &k_uc,
            const idi value_M,
            const idi query_id,
            const dataf *query_data,
            const idi L,
            std::vector<Candidate> &set_L,
            const idi set_L_base,
            idi &set_L_end,
            std::vector<idi> &top_m_candidates,
            boost::dynamic_bitset<> &is_visited,
            uint64_t &count_distance_computation);
    void subsearch_top_m_for_one_iteration(
            const distf bound_lth,
            const idi iter,
            idi &k_uc,
            const idi value_M,
            const idi query_id,
            const dataf *query_data,
            const idi L,
            std::vector<Candidate> &set_L,
            const idi set_L_base,
            idi &set_L_end,
            std::vector<idi> &top_m_candidates,
            boost::dynamic_bitset<> &is_visited,
            uint64_t &count_distance_computation);
    void para_search_with_top_m_subsearch_v0(
//            const idi value_M_middle,
            const idi value_M_max,
            const idi query_id,
            const idi K,
            const idi L,
            std::vector<Candidate> &set_L,
            const std::vector<idi> &init_ids,
            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
            std::vector< std::vector<idi> > &top_m_candidates_list,
            boost::dynamic_bitset<> &is_visited);
    void para_search_with_top_m_subsearch_v1(
            const idi local_M_max,
            const idi query_id,
            const idi K,
            const idi global_L,
            const idi local_L,
            const idi total_L,
            const idi init_queue_end,
            std::vector<Candidate> &set_L,
            const std::vector<idi> &init_ids,
            std::vector<idi> &set_K,
            const std::vector<idi> &local_queues_bases,
            std::vector<idi> &local_queues_ends,
            std::vector< std::vector<idi> > &top_m_candidates_list,
            boost::dynamic_bitset<> &is_visited);
    void subsearch_for_simple_search(
            const idi query_id,
            const idi local_L,
            std::vector<Candidate> &set_L,
            const idi base_set_L,
            idi &set_L_end,
//            std::vector<uint8_t> &is_visited,
            boost::dynamic_bitset<> &is_visited,
            uint64_t &local_count_distance_computation);
    void para_simple_search_subsearch(
            const idi query_id,
            const idi K,
            const idi L,
            std::vector<Candidate> &set_L,
            const std::vector<idi> &init_ids,
            std::vector<idi> &set_K,
//            std::vector<uint8_t> &is_visited);
            boost::dynamic_bitset<> &is_visited);

//    void para_search_with_top_m_merge_queues_global_threshold(
//            const idi value_M_middle,
//            const idi value_M_max,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
//            std::vector<idi> &top_m_candidates,
//            boost::dynamic_bitset<> &is_visited);
//    void para_search_with_top_m_merge_queues_distance_threshold_m(
////        const idi value_M_middle,
////        const idi value_M_max,
//            const distf relative_dist_threshold,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
////        std::vector<Candidate> &top_m_candidates,
//            std::vector<idi> &top_m_candidates,
////        std::vector<uint8_t> &is_visited)
//            boost::dynamic_bitset<> &is_visited);
////        std::vector<distf> &local_thresholds)
////        BitVector &is_visited)
//    void para_search_with_top_m_merge_queues_distance_threshold_m_middle_iteration(
////        const idi value_M_middle,
////        const idi value_M_max,
//            const distf relative_dist_threshold,
//            const idi middle_iteration,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
////        std::vector<Candidate> &top_m_candidates,
//            std::vector<idi> &top_m_candidates,
////        std::vector<uint8_t> &is_visited)
//            boost::dynamic_bitset<> &is_visited);

//    void para_search_with_top_m_merge_queues_collectors(
//            const idi value_M_middle,
//            const idi value_M_max,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
////        std::vector<Candidate> &top_m_candidates,
//            std::vector<idi> &top_m_candidates,
////        std::vector<uint8_t> &is_visited)
//            boost::dynamic_bitset<> &is_visited);

//    void para_search_with_top_m_merge_queues_selecting(
//            const idi value_M_middle,
//            const idi value_M_max,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
//            const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
////        std::vector<Candidate> &top_m_candidates,
//            std::vector<idi> &top_m_candidates,
////        std::vector<uint8_t> &is_visited)
//            boost::dynamic_bitset<> &is_visited);
//    void para_search_with_top_m_merge_queues_myths(
//            const idi M,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_length, // Maximum size of local queue
////        std::vector< std::vector<Candidate> > &local_queues_list,
//            std::vector<Candidate> &local_queues_array,
//            std::vector<idi> &local_queues_ends, // Sizes of local queue
//            BitVector &is_visited);
////        std::vector<uint8_t> &is_visited);
////            boost::dynamic_bitset<> &is_visited);
////    void para_prepare_init_ids(
////            std::vector<unsigned> &init_ids,
////            unsigned L) const;
//    void para_search_with_top_m_in_batch_embarassing_para(
//            const PANNS::idi M,
//            const PANNS::idi batch_start,
//            const PANNS::idi batch_size,
//            const PANNS::idi K,
//            const PANNS::idi L,
//            std::vector< std::vector<Candidate> > &set_L_list,
//            const std::vector<idi> &init_ids,
//            std::vector< std::vector<idi> > &set_K_list,
//            std::vector< boost::dynamic_bitset<> > &is_visited_list);

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
            std::unordered_map<unsigned, double> &recalls) const;
}; // Class Searching

/**
 * Input the data from the file.
 * @param filename
 */
inline void Searching::load_data_load(char *filename)
{
    auto old_d = dimension_;
    DiskIO::load_data(
            filename,
            data_load_,
            num_v_,
            dimension_);
    if (old_d) {
        if (old_d != dimension_) {
            std::cerr << "Error: data dimension " << dimension_
                      << " is not equal to query dimension " << old_d << "." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * Input queries from the file.
 * @param filename
 */
inline void Searching::load_queries_load(char *filename)
{
    auto old_d = dimension_;
    DiskIO::load_data(
            filename,
            queries_load_,
            num_queries_,
            dimension_);
    if (old_d) {
        if (old_d != dimension_) {
            std::cerr << "Error: query dimension " << dimension_
                      << " is not equal to data dimension " << old_d << "." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * Input the NSG graph from the file.
 * Reference: https://github.com/ZJULearning/nsg/blob/master/src/index_nsg.cpp
 * @param filename
 */
inline void Searching::load_nsg_graph(char *filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "Error: cannot read file " << filename << " ." << std::endl;
        exit(EXIT_FAILURE);
    }
    fin.read(reinterpret_cast<char *>(&width_), sizeof(unsigned));
    fin.read(reinterpret_cast<char *>(&ep_), sizeof(unsigned));

    data_bytes_ = (1 + dimension_) * sizeof(dataf);
    neighbor_bytes_ = (1 + width_) * sizeof(idi);
    vertex_bytes_ = data_bytes_ + neighbor_bytes_;
    opt_nsg_graph_ = (char *) malloc(num_v_ * vertex_bytes_);
    if (!opt_nsg_graph_) {
        std::cerr << "Error: no enough memory for opt_nsg_graph_." << std::endl;
        exit(EXIT_FAILURE);
    }

    idi v_id = 0;
    num_e_ = 0;
    char *base_location = opt_nsg_graph_;
    while (true) {
        idi degree;
        fin.read(reinterpret_cast<char *>(&degree), sizeof(unsigned));
        if (fin.eof()) {
            break;
        }
        num_e_ += degree;
//        std::vector<idi> tmp_ngbrs(degree);
//        fin.read(reinterpret_cast<char *>(tmp_ngbrs.data()), degree * sizeof(unsigned));

        // Norm and data
        distf norm = compute_norm(data_load_ + v_id * dimension_);
//        distf norm = compute_norm(v_id);
        std::memcpy(base_location, &norm, sizeof(distf)); // Norm
        memcpy(base_location + sizeof(distf), data_load_ + v_id * dimension_, dimension_ * sizeof(dataf)); // Data
        base_location += data_bytes_;

        // Neighbors
        memcpy(base_location, &degree, sizeof(idi)); // Number of neighbors
        fin.read(base_location + sizeof(idi), degree * sizeof(unsigned)); // Neighbors
//        memcpy(location + sizeof(idi), tmp_ngbrs.data(), degree * sizeof(unsigned));
        base_location += neighbor_bytes_;
        ++v_id;
    }
    if (v_id != num_v_) {
        std::cerr << "Error: NSG data has " << v_id
                  << " vertices, but origin data has " << num_v_ << " vertices." << std::endl;
        exit(EXIT_FAILURE);
    }
    free(data_load_);
    data_load_ = nullptr;
//    ////////////////////////
//    idi v_id = 0;
//    num_e_ = 0;
//    while (true) {
//        idi degree;
//        fin.read(reinterpret_cast<char *>(&degree), sizeof(unsigned));
//        if (fin.eof()) {
//            break;
//        }
//        num_e_ += degree;
//
//        std::vector<idi> ngbrs(degree);
//        fin.read(reinterpret_cast<char *>(ngbrs.data()), degree * sizeof(unsigned));
////        nsg_graph_.push_back(ngbrs);
////        tmp_edge_list.push_back(ngbrs);
//        edge_list_.push_back(ngbrs);
//        ++v_id;
//    }
//    if (v_id != num_v_) {
//        std::cerr << "Error: NSG data has " << v_id
//                  << " vertices, but origin data has " << num_v_ << " vertices." << std::endl;
//        exit(EXIT_FAILURE);
//    }
}


/**
 * Load those true top-K neighbors (ground truth) of queries
 * @param filename
 * @param[out] true_nn_list
 */
inline void Searching::load_true_NN(
        const char *filename,
        std::vector< std::vector<idi> > &true_nn_list)
//        unsigned &t_K)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    idi t_query_num;
    idi t_K;
//    unsigned t_K;
    fin.read(reinterpret_cast<char *>(&t_query_num), sizeof(t_query_num));
    fin.read(reinterpret_cast<char *>(&t_K), sizeof(t_K));
//    if (t_query_num != query_num) {
//        fprintf(stderr, "Error: query_num %u is not equal to the record %u in true-NN file %s\n",
//                query_num, t_query_num, filename);
//        exit(EXIT_FAILURE);
//    }
    if (t_query_num < num_queries_) {
        fprintf(stderr, "Error: t_query_num %u is smaller than num_queries_ %u\n", t_query_num, num_queries_);
        exit(EXIT_FAILURE);
    }
    if (t_K < 100) {
        fprintf(stderr, "Error: t_K %u is smaller than 100.\n", t_K);
        exit(EXIT_FAILURE);
    }

//    data = new unsigned[(size_t) t_query_num * (size_t) t_K];
    true_nn_list.resize(t_query_num);
    for (idi q_i = 0; q_i < t_query_num; ++q_i) {
        true_nn_list[q_i].resize(t_K);
    }

    for (unsigned q_i = 0; q_i < t_query_num; ++q_i) {
//        size_t offset = q_i * t_K;
        for (unsigned n_i = 0; n_i < t_K; ++n_i) {
            unsigned id;
            float dist;
            fin.read(reinterpret_cast<char *>(&id), sizeof(id));
            fin.read(reinterpret_cast<char *>(&dist), sizeof(dist));
//            data[offset + n_i] = id;
            true_nn_list[q_i][n_i] = id;
        }
    }

    fin.close();
}

inline void Searching::get_recall_for_all_queries(
        const std::vector< std::vector<idi> > &true_nn_list,
        const std::vector<std::vector<unsigned>> &set_K_list,
        std::unordered_map<unsigned, double> &recalls) const
{
//    if (t_K < 100) {
//        fprintf(stderr, "Error: t_K %u is smaller than 100.\n", t_K);
//        exit(EXIT_FAILURE);
//    }
    if (true_nn_list[0].size() < 100) {
        fprintf(stderr, "Error: Number of true nearest neighbors of a query is smaller than 100.\n");
        exit(EXIT_FAILURE);
    }
    recalls[1] = 0.0;
    recalls[5] = 0.0;
    recalls[10] = 0.0;
    recalls[20] = 0.0;
    recalls[50] = 0.0;
    recalls[100] = 0.0;
    for (unsigned q_i = 0; q_i < num_queries_; ++q_i) {
//        size_t offset = q_i * t_K;
        for (unsigned top_i = 0; top_i < 100; ++top_i) {
            unsigned true_id = true_nn_list[q_i][top_i];
            for (unsigned n_i = 0; n_i < 100; ++n_i) {
                if (set_K_list[q_i][n_i] == true_id) {
                    if (n_i < 1) recalls[1] += 1;
                    if (n_i < 5) recalls[5] += 1;
                    if (n_i < 10) recalls[10] += 1;
                    if (n_i < 20) recalls[20] += 1;
                    if (n_i < 50) recalls[50] += 1;
                    if (n_i < 100) recalls[100] += 1;
                }
            }
        }
    }
    recalls[1] /= 1.0 * num_queries_;
    recalls[5] /= 5.0 * num_queries_;
    recalls[10] /= 10.0 * num_queries_;
    recalls[20] /= 20.0 * num_queries_;
    recalls[50] /= 50.0 * num_queries_;
    recalls[100] /= 100.0 * num_queries_;
}

inline void Searching::search_in_sequential(
        const idi query_id,
        const idi K,
        const idi L,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K)
{
//    {//test
//        printf("Iteration: Relative_Distance:\n");
////        printf("Iteration: Relative_Distance:\n");
////        printf("----query: %u----\n", query_id);
//    }
    boost::dynamic_bitset<> is_visited(num_v_);

    for (idi v_i = 0; v_i < L; ++v_i) {
        is_visited[init_ids[v_i]] = true;
    }

    const dataf *query_data = queries_load_ + query_id  * dimension_;

    for (idi v_i = 0; v_i < L; ++v_i) {
        idi v_id = init_ids[v_i];
        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
    }
    // Get the distances of all candidates, store in the set set_L.
    for (unsigned i = 0; i < L; i++) {
        unsigned v_id = init_ids[i];
        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
        dataf norm = *v_data++;
        ++count_distance_computation_;
        distf dist = compute_distance_with_norm(v_data, query_data, norm);
        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
    }
    std::sort(set_L.begin(), set_L.begin() + L);
    idi k = 0; // Index of every queue's first unchecked candidate.
    idi tmp_count = 0; // for debug

//    {// Print relative distance
////        distf top_dist = set_L[0].distance_;
//        for (idi i_l = 0; i_l < L; ++i_l) {
//            printf("%u %f\n",
//                   tmp_count, set_L[i_l].distance_);
////                   tmp_count, set_L[i_l].distance_ - top_dist);
//        }
//    }

    while (k < L) {
        Candidate &top_cand = set_L[k];
        unsigned nk = L;
        if (!top_cand.is_checked_) {
            ++tmp_count;

            top_cand.is_checked_ = true;
            idi v_id = top_cand.id_; // Vertex ID.
            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
            idi *out_edges = (idi *) (opt_nsg_graph_ + v_id * vertex_bytes_ + data_bytes_);
            idi out_degree = *out_edges++;
            for (idi n_i = 0; n_i < out_degree; ++n_i) {
                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
            }
            // Traverse v_id's all neighbors, pushing them into the queue
            for (idi e_i = 0; e_i < out_degree; ++e_i) {
                idi nb_id = out_edges[e_i];
                if (is_visited[nb_id]) {
                    continue;
                }
                is_visited[nb_id] = true;
                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
                dataf norm = *nb_data++;
                // Compute the distance
                ++count_distance_computation_;
                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
                if (dist > set_L[L-1].distance_) {
                    continue;
                }
//                if (dist >= set_L[L-1].distance_) {
//                    continue;
//                }
                Candidate cand(nb_id, dist, false);
                // Insert into the queue
                idi r = insert_into_queue(set_L, L, cand);
                if (r < nk) {
                    nk = r;
                }
            }

//            {// Print relative distance
////                distf top_dist = set_L[0].distance_;
//                for (idi i_l = 0; i_l < L; ++i_l) {
//                    printf("%u %f\n",
//                           tmp_count, set_L[i_l].distance_);
////                           tmp_count, set_L[i_l].distance_ - top_dist);
//                }
//            }
        }
        if (nk <= k) {
            k = nk;
        } else {
            ++k;
        }


    }
//    cache_miss_kernel.measure_stop();

    for (size_t k_i = 0; k_i < K; ++k_i) {
        set_K[k_i] = set_L[k_i].id_;
    }

//    {//test
//        if (0 == query_id) {
//            exit(1);
//        }
//    }
}

//inline void Searching::search_in_sequential_BitVector(
//        const idi query_id,
//        const idi K,
//        const idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K)
//{
////    boost::dynamic_bitset<> is_visited(num_v_);
//    BitVector is_visited(num_v_);
//
//#pragma omp parallel for
//    for (idi v_i = 0; v_i < L; ++v_i) {
////        is_visited[init_ids[v_i]] = true;
//        is_visited.atomic_set_bit(init_ids[v_i]);
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//
//#pragma omp parallel for
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
//#pragma omp parallel for
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//    idi k = 0; // Index of every queue's first unchecked candidate.
//    while (k < L) {
//        Candidate &top_cand = set_L[k];
//        unsigned nk = L;
//        if (!top_cand.is_checked_) {
//            top_cand.is_checked_ = true;
//            idi v_id = top_cand.id_; // Vertex ID.
//            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + v_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            // Traverse v_id's all neighbors, pushing them into the queue
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
////                if (is_visited[nb_id]) {
////                    continue;
////                }
////                is_visited[nb_id] = true;
//
//                {// Self-defined BitVector
//                    if (is_visited.atomic_is_bit_set(nb_id)) {
//                        continue;
//                    }
//                    is_visited.atomic_set_bit(nb_id);
//                }
//
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
//                // Compute the distance
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
////                if (dist >= set_L[L-1].distance_) {
////                    continue;
////                }
//                Candidate cand(nb_id, dist, false);
//                // Insert into the queue
//                idi r = insert_into_queue(set_L, L, cand);
//                if (r < nk) {
//                    nk = r;
//                }
//            }
//        }
//        if (nk <= k) {
//            k = nk;
//        } else {
//            ++k;
//        }
//    }
////    cache_miss_kernel.measure_stop();
//#pragma omp parallel for
//    for (size_t k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
//}

/**
 * Prepare init_ids and flags, as they are constant for all queries.
 * @param[out] init_ids
 * @param L
 */
inline void Searching::prepare_init_ids(
        std::vector<unsigned int> &init_ids,
        const unsigned L) const
{
//    idi num_ngbrs = get_out_degree(ep_);
//    edgei edge_start = nsg_graph_indices_[ep_];
//    // Store ep_'s neighbors as candidates
//    idi tmp_l = 0;
//    for (; tmp_l < L && tmp_l < num_ngbrs; tmp_l++) {
//        init_ids[tmp_l] = nsg_graph_out_edges_[edge_start + tmp_l];
//    }
//    std::unordered_set<idi> visited_ids;
    boost::dynamic_bitset<> is_selected(num_v_);
    idi *out_edges = (idi *) (opt_nsg_graph_ + ep_ * vertex_bytes_ + data_bytes_);
    idi out_degree = *out_edges++;
    idi init_ids_end = 0;
//    for (; tmp_l < L && tmp_l < out_degree; tmp_l++) {
    for (idi e_i = 0; e_i < out_degree && init_ids_end < L; ++e_i) {
//        idi v_id = out_edges[tmp_l];
        idi v_id = out_edges[e_i];
        if(is_selected[v_id]) {
            continue;
        }
        is_selected[v_id] = true;
//        init_ids[tmp_l] = v_id;
        init_ids[init_ids_end++] = v_id;
//        init_ids[tmp_l] = out_edges[tmp_l];
//        visited_ids.insert(init_ids[tmp_l]);
    }

//    for (idi i = 0; i < tmp_l; ++i) {
//        is_visited[init_ids[i]] = true;
//    }

    // If ep_'s neighbors are not enough, add other random vertices
    idi tmp_id = ep_ + 1; // use tmp_id to replace rand().
    while (init_ids_end < L) {
        tmp_id %= num_v_;
        idi v_id = tmp_id++;
        if (is_selected[v_id]) {
            continue;
        }
//        if (visited_ids.find(id) != visited_ids.end()) {
//            continue;
//        }
        is_selected[v_id] = true;
//        visited_ids.insert(id);
        init_ids[init_ids_end++] = v_id;
//        tmp_l++;
    }
}

// TODO: re-code in AVX-512
inline dataf Searching::compute_norm(
        const dataf *data) const
//        idi vertex_id)
//        const std::vector<PANNS::dataf> &data)
//        size_t loc_start,
//        idi dimension)
{
//    const dataf *a = data.data() + loc_start;
//    const dataf *a = data_load_ + vertex_id * dimension_;
//    idi size = dimension_;
    dataf result = 0;
//#define AVX_L2NORM(addr, dest, tmp) \
//    tmp = _mm256_load_ps(addr); \
//    tmp = _mm256_mul_ps(tmp, tmp); \
//    dest = _mm256_add_ps(dest, tmp);
#define AVX_L2NORM(addr, dest, tmp) \
    tmp = _mm256_loadu_ps(addr); \
    tmp = _mm256_mul_ps(tmp, tmp); \
    dest = _mm256_add_ps(dest, tmp);

    __m256 sum;
    __m256 l0, l1;
    unsigned D = (dimension_ + 7) & ~7U;
    unsigned DR = D % 16;
    unsigned DD = D - DR;
    const float *l = data;
    const float *e_l = l + DD;
    float unpack[8] __attribute__ ((aligned (32))) = {0, 0, 0, 0, 0, 0, 0, 0};

    sum = _mm256_load_ps(unpack);
//    sum = _mm256_loadu_ps(unpack);
    if (DR) { AVX_L2NORM(e_l, sum, l0); }
    for (unsigned i = 0; i < DD; i += 16, l += 16) {
        AVX_L2NORM(l, sum, l0);
        AVX_L2NORM(l + 8, sum, l1);
    }
    _mm256_store_ps(unpack, sum);
//    _mm256_storeu_ps(unpack, sum);
    result = unpack[0] + unpack[1] + unpack[2] + unpack[3] + unpack[4] + unpack[5] + unpack[6] + unpack[7];

    return result;
}

inline dataf Searching::compute_distance_with_norm(
        const dataf *v_data,
        const dataf *q_data,
//        idi vertex_id,
//        idi query_id,
//        const std::vector<PANNS::dataf> &d_data,
//        const std::vector<PANNS::dataf> &q_data,
//        PANNS::idi d_start,
//        PANNS::idi q_start,
        const dataf vertex_norm) const
//        idi dimension)
{
//    idi size = dimension_;
    float result = 0;
//#define AVX_DOT(addr1, addr2, dest, tmp1, tmp2) \
//          tmp1 = _mm256_load_ps(addr1);\
//          tmp2 = _mm256_load_ps(addr2);\
//          tmp1 = _mm256_mul_ps(tmp1, tmp2); \
//          dest = _mm256_add_ps(dest, tmp1);
#define AVX_DOT(addr1, addr2, dest, tmp1, tmp2) \
          tmp1 = _mm256_loadu_ps(addr1);\
          tmp2 = _mm256_loadu_ps(addr2);\
          tmp1 = _mm256_mul_ps(tmp1, tmp2); \
          dest = _mm256_add_ps(dest, tmp1);

    __m256 sum;
    __m256 l0, l1;
    __m256 r0, r1;
    unsigned D = (dimension_ + 7) & ~7U;
    unsigned DR = D % 16;
    unsigned DD = D - DR;
    const float *l = v_data;
    const float *r = q_data;
//    const float *l = (float *) (opt_nsg_graph_ + vertex_id * vertex_bytes_ + sizeof(distf));
//    const float *r = queries_load_ + query_id * dimension_;
    const float *e_l = l + DD;
    const float *e_r = r + DD;
    float unpack[8] __attribute__ ((aligned (32))) = {0, 0, 0, 0, 0, 0, 0, 0};

    sum = _mm256_load_ps(unpack);
//    sum = _mm256_loadu_ps(unpack);
    if (DR) { AVX_DOT(e_l, e_r, sum, l0, r0); }

    for (unsigned i = 0; i < DD; i += 16, l += 16, r += 16) {
        AVX_DOT(l, r, sum, l0, r0);
        AVX_DOT(l + 8, r + 8, sum, l1, r1);
    }
    _mm256_store_ps(unpack, sum);
//    _mm256_storeu_ps(unpack, sum);
    result = unpack[0] + unpack[1] + unpack[2] + unpack[3] + unpack[4] + unpack[5] + unpack[6] + unpack[7];

    result = -2 * result + vertex_norm;

    return result;
}

//// DEPRECATED.
// The difference from insert_into_queue is that add_into_queue will increase the queue size by 1.
//inline idi Searching::add_into_queue(
//        std::vector<PANNS::Candidate> &queue,
//        idi &queue_top,
//        const idi queue_size,
//        const PANNS::Candidate &cand)
//{
//    assert(queue_size > 1);
//    if (0 == queue_top) {
//        queue[queue_top++] = cand;
//        return 0;
//    } else if (1 == queue_top) {
//        if (queue[0] < cand) {
//            queue[queue_top++] = cand;
//            return 1;
//        } else {
//            queue[++queue_top] = queue[0];
//            queue[0] = cand;
//            return 0;
//        }
//    }
//
//    if (queue[queue_top - 1] < cand) {
//        if (queue_top < queue_size) {
//            queue[queue_top++] = cand;
//        }
//        return queue_top;
//    }
//
//    idi r = insert_into_queue(
//            queue,
//            queue_top - 1,
//            cand);
////    {//test
////        printf("r: %u"
////               "queue_top: %u "
////               "queue_size: %u\n",
////               r,
////               queue_top,
////               queue_size);
////    }
//    return r;
//
////    /////////////////////////////////////////////////////////////
////    // Find the insert location
////    auto it_loc = std::lower_bound(queue.begin(), queue.begin() + queue_top, cand);
////    idi insert_loc = it_loc - queue.begin();
////    if (insert_loc == queue_size) {
////        return queue_size;
////    }
////
////    // Insert
//////    if (queue_top == queue_size) {
//////        // If full already
//////        --queue_top;
//////    }
////    memmove(reinterpret_cast<char *>(queue.data() + insert_loc + 1),
////            reinterpret_cast<char *>(queue.data() + insert_loc),
////            (queue_top - insert_loc) * sizeof(Candidate));
//////    for (idi q_i = queue_top; q_i > insert_loc; --q_i) {
//////        queue.at(q_i) = queue.at(q_i - 1);
//////    }
////    queue[insert_loc] = cand;
////    ++queue_top;
////    return insert_loc;
//}
// The difference from insert_into_queue is that add_into_queue will increase the queue size by 1.
inline idi Searching::add_into_queue(
        std::vector<PANNS::Candidate> &queue,
        idi &queue_top,
        const idi queue_size,
        const PANNS::Candidate &cand)
{
    if (0 == queue_top) {
        queue[queue_top++] = cand;
        return 0;
    }

    // Find the insert location
    auto it_loc = std::lower_bound(queue.begin(), queue.begin() + queue_top, cand);
    idi insert_loc = it_loc - queue.begin();
    if (insert_loc == queue_size) {
        return queue_size;
    }

    // Insert
    if (queue_top == queue_size) {
        // If full already
        --queue_top;
    }
    memmove(reinterpret_cast<char *>(queue.data() + insert_loc + 1),
            reinterpret_cast<char *>(queue.data() + insert_loc),
            (queue_top - insert_loc) * sizeof(Candidate));
//    for (idi q_i = queue_top; q_i > insert_loc; --q_i) {
//        queue.at(q_i) = queue.at(q_i - 1);
//    }
    queue[insert_loc] = cand;
    ++queue_top;
    return insert_loc;
}

// The difference from insert_into_queue is that add_into_queue will increase the queue size by 1.
// add_into_queue with a queue_start
inline idi Searching::add_into_queue(
        std::vector<PANNS::Candidate> &queue,
        const idi queue_start,
        idi &queue_top, // The insertion location starting from queue_start
        const idi queue_size, // The maximum capacity of queue, independent with queue_start.
        const PANNS::Candidate &cand)
{
    if (0 == queue_top) {
        queue[queue_start + queue_top++] = cand;
        return 0;
    }
    idi queue_end = queue_start + queue_top;
    // Find the insert location
    const auto it_loc = std::lower_bound(queue.begin() + queue_start, queue.begin() + queue_end, cand);
//    auto it_loc = std::lower_bound(queue.begin(), queue.begin() + queue_top, cand);
    idi insert_loc = it_loc - queue.begin();

    if (insert_loc != queue_end) {
        if (cand.id_ == it_loc->id_) {
            // Duplicate
            return queue_size;
        }
        if (queue_top >= queue_size) { // Queue is full
            --queue_top;
            --queue_end;
        }
    } else { // insert_loc == queue_end, insert at the end?
        if (queue_top < queue_size) { // Queue is not full
            // Insert at the end
            queue[insert_loc] = cand;
            ++queue_top;
            return queue_top - 1;
        } else { // Queue is full
            return queue_size;
        }
    }

//    if (queue_top < queue_size) {
//        // Queue is not full
//        if (insert_loc == queue_end) {
//            // Insert at the end
//            queue[insert_loc] = cand;
//            ++queue_top;
//            return queue_top - 1;
//        }
//    } else {
//        // Queue is full
//        if (insert_loc == queue_end) {
//            return queue_size;
//        }
//        --queue_top;
//        --queue_end;
//    }
//
//    if (cand.id_ == it_loc->id_) {
//        // Duplicate
//        return queue_size;
//    }
    // Add into queue
    memmove(reinterpret_cast<char *>(queue.data() + insert_loc + 1),
            reinterpret_cast<char *>(queue.data() + insert_loc),
            (queue_end - insert_loc) * sizeof(Candidate));
    queue[insert_loc] = cand;
    ++queue_top;
    return insert_loc - queue_start;
}


inline void Searching::add_into_queue_at(
        const Candidate &cand,
        std::vector<Candidate> &queue,
        const idi insert_index, // The insertion location, independent with queue_start
        const idi queue_start,
        idi &queue_size, // The number of elements in queue, independent with queue_start
        const idi queue_length) // The maximum capacity of queue, independent with queue_start.
{
    const idi dest_index = queue_start + insert_index;
    if (queue_size == queue_length) {
        --queue_size;
    }
    memmove(reinterpret_cast<char *>(queue.data() + dest_index + 1),
            reinterpret_cast<char *>(queue.data() + dest_index),
            (queue_size - insert_index) * sizeof(Candidate));
    queue[dest_index] = cand;
    ++queue_size;
}

inline void Searching::insert_one_element_at(
//        const T &cand,
//        T *queue_base,
        const Candidate &cand,
        std::vector<Candidate> &queue,
        const idi insert_index,
        const idi queue_start,
        const idi queue_size)
{
    const idi dest_index = queue_start + insert_index;
    memmove(reinterpret_cast<char *>(queue.data() + dest_index + 1),
            reinterpret_cast<char *>(queue.data() + dest_index),
            (queue_size - insert_index - 1) * sizeof(Candidate));
    queue[dest_index] = cand;
//    memmove(reinterpret_cast<char *>(queue_base + dest_index + 1),
//            reinterpret_cast<char *>(queue_base + dest_index),
//            (queue_size - insert_index - 1) * sizeof(T));

//    for (idi q_i = queue_size - 1; q_i > insert_index; --q_i) {
//        queue_base.at(q_i + queue_start) = queue_base.at(q_i - 1 + queue_start);
//    }
//    queue_base[dest_index] = cand;
}

/**
 * PANNS version of InsertIntoPool(): binary-search to find the insert place and then move.
 * @param[out] c_queue
 * @param c_queue_top
 * @param cand
 * @return
 */
inline idi Searching::insert_into_queue(
        std::vector<PANNS::Candidate> &c_queue,
        PANNS::idi c_queue_top,
        PANNS::Candidate cand)
{

    if (c_queue[0].distance_ > cand.distance_) {
        // If the first
        memmove(reinterpret_cast<char *>(c_queue.data() + 1),
                reinterpret_cast<char *>(c_queue.data()),
                c_queue_top * sizeof(Candidate));
        c_queue[0] = cand;
        return 0;
    } else if (c_queue[c_queue_top - 1].distance_ == cand.distance_) {
        // If the last
        if (c_queue[c_queue_top - 1].id_ > cand.id_) {
            // Use ID as the second metrics for ordering
            c_queue[c_queue_top - 1] = cand;
            return c_queue_top - 1;
        } else {
            return c_queue_top;
        }
    }

    idi left = 0;
    idi right = c_queue_top;
    while (left < right) {
        idi mid = (right - left) / 2 + left;
        if (c_queue[mid].distance_ > cand.distance_) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }

    // If the distance is the same
    if (0 != left && c_queue[left - 1].distance_ != cand.distance_) {
            ;
    } else {
        while (0 != left
               && c_queue[left - 1].distance_ == cand.distance_
               && c_queue[left - 1].id_ > cand.id_) {
            // Use ID as the second metrics for ordering
            --left;
        }
    }

    // Insert to left
    memmove(reinterpret_cast<char *>(c_queue.data() + left + 1),
            reinterpret_cast<char *>(c_queue.data() + left),
            (c_queue_top - left) * sizeof(Candidate));
    c_queue[left] = cand;
    return left;
}

//inline void Searching::cand_pushes_ngbrs_into_queue(
//        idi cand_id,
//        const dataf *query_data,
//        idi L,
//        idi &new_k,
//        boost::dynamic_bitset<> &is_visited,
//        std::vector<Candidate> &set_L)
//{
//    _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//    idi *out_edges = (idi *) (opt_nsg_graph_ + v_id * vertex_bytes_ + data_bytes_);
//    idi out_degree = *out_edges++;
//    for (idi n_i = 0; n_i < out_degree; ++n_i) {
//        _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//    }
//    for (idi e_i = 0; e_i < out_degree; ++e_i) {
//        idi nb_id = out_edges[e_i];
//        if (is_visited[nb_id]) {
//            continue;
//        }
//        is_visited[nb_id] = true;
//        auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//        dataf norm = *nb_data++;
//        distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//        if (dist >= set_L[L-1].distance_) {
//            continue;
//        }
//        Candidate cand(nb_id, dist, false);
//        idi r = insert_into_queue(set_L, L, cand);
//        if (r < nk) {
//            nk = r;
//        }
//    }
//}

//inline void Searching::search_in_sequential(
//        const idi query_id,
//        const idi K,
//        const idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K) const
//{
//    boost::dynamic_bitset<> is_visited(num_v_);
//
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        is_visited[init_ids[v_i]] = true;
//    }
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//    idi k = 0; // Index of every queue's first unchecked candidate.
//    while (k < L) {
//        Candidate &top_cand = set_L[k];
//        unsigned nk = L;
//        if (!top_cand.is_checked_) {
//            top_cand.is_checked_ = true;
//            idi v_id = top_cand.id_; // Vertex ID.
//            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + v_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            // Traverse v_id's all neighbors, pushing them into the queue
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
//                if (is_visited[nb_id]) {
//                    continue;
//                }
//                is_visited[nb_id] = true;
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
//                // Compute the distance
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
//                Candidate cand(nb_id, dist, false);
//                // Insert into the queue
//                idi r = insert_into_queue(set_L, L, cand);
//                if (r < nk) {
//                    nk = r;
//                }
//            }
//        }
//        if (nk <= k) {
//            k = nk;
//        } else {
//            ++k;
//        }
//    }
//
//    for (size_t k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
//}

// Deprecated: cannot use std::set, because its element is constant.
//inline void Searching::search_in_sequential(
//        const idi query_id,
//        const idi K,
//        const idi L,
////        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K) const
//{
//    std::set<Candidate> set_L;
//    boost::dynamic_bitset<> is_visited(num_v_);
//
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        is_visited[init_ids[v_i]] = true;
//    }
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
////        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//        set_L.emplace(v_id, dist, false);
//    }
////    std::sort(set_L.begin(), set_L.begin() + L);
//    idi k = 0; // Index of every queue's first unchecked candidate.
//    while (k < L) {
////        Candidate &top_cand = set_L[k];
//        std::set<Candidate>::iterator top_cand = std::next(set_L.begin(), k);
//        unsigned nk = L;
//        if (!top_cand->is_checked_) {
//            top_cand->is_checked_ = true;
//            idi v_id = top_cand.id_; // Vertex ID.
//            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + v_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            // Traverse v_id's all neighbors, pushing them into the queue
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
//                if (is_visited[nb_id]) {
//                    continue;
//                }
//                is_visited[nb_id] = true;
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
//                // Compute the distance
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
//                Candidate cand(nb_id, dist, false);
//                // Insert into the queue
//                idi r = insert_into_queue(set_L, L, cand);
//                if (r < nk) {
//                    nk = r;
//                }
//            }
//        }
//        if (nk <= k) {
//            k = nk;
//        } else {
//            ++k;
//        }
//    }
//
//    for (size_t k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
//}

/* Function:
 * queue1_size is fixed.
 */
inline idi Searching::merge_two_queues_into_1st_queue_seq_fixed(
        std::vector<Candidate> &queue1,
        const idi queue1_start,
        const idi queue1_size,
        std::vector<Candidate> &queue2,
        const idi queue2_start,
        const idi queue2_size)
//        const idi limit_size)
{
    assert(queue1_size && queue2_size);
    // Record the lowest insert location.
    auto it_loc = std::lower_bound(
            queue1.begin() + queue1_start,
            queue1.begin() + queue1_start + queue1_size,
            queue2[queue2_start]);
    idi insert_index = it_loc - (queue1.begin() + queue1_start);
    if (insert_index == queue1_size) {
        return insert_index;
    } else if (insert_index == queue1_size - 1) {
        queue1[queue1_start + insert_index] = queue2[queue2_start];
        return insert_index;
    }

    // Insert the 1st of queue2
    if (queue2[queue2_start].id_ != it_loc->id_) {
        // Not Duplicate
        insert_one_element_at(
                queue2[queue2_start],
                queue1,
                insert_index,
                queue1_start,
                queue1_size);
    }
    if (queue2_size == 1) {
        return insert_index;
    }

    // Insert
    idi q_i_1 = insert_index + 1 + queue1_start;
    idi q_i_2 = queue2_start + 1;
    const idi q_i_1_bound = queue1_start + queue1_size;
    const idi q_i_2_bound = queue2_start + queue2_size;
//    const idi insert_i_bound = queue1_start + limit_size;
    for (idi insert_i = insert_index + 1; insert_i < queue1_size; ++insert_i) {
        if (q_i_1 >= q_i_1_bound || q_i_2 >= q_i_2_bound) {
            // queue1 or queue2 finished traverse. Rest o
            break;
        } else if (queue1[q_i_1] < queue2[q_i_2]) {
            ++q_i_1;
        } else if (queue2[q_i_2] < queue1[q_i_1]) {
            // Insert queue2[q_i_2] into queue1
            insert_one_element_at(
                    queue2[q_i_2++],
                    queue1,
                    insert_i,
                    queue1_start,
                    queue1_size);
            ++q_i_1;
        } else {
            // Duplicate
            ++q_i_2;
            ++q_i_1;
        }
    }

    return insert_index;
}

/* Function:
 * queue1_size should be updated.
 * queue1_length should be provided.
 */
inline void Searching::merge_two_queues_into_1st_queue_seq_incr(
        std::vector<Candidate> &queue1,
        const idi queue1_start,
        idi &queue1_size, // The number of element in queue1, independent with queue1_start.
        const idi queue1_length, // The maximum capacity of queue1, independent with queue1_start.
        std::vector<Candidate> &queue2,
        const idi queue2_start,
        const idi queue2_size)
//        const idi limit_size)
{
    assert(queue1_size && queue2_size);
    // Record the lowest insert location.
    auto it_loc = std::lower_bound(
            queue1.begin() + queue1_start,
            queue1.begin() + queue1_start + queue1_size,
            queue2[queue2_start]);
    idi insert_index = it_loc - (queue1.begin() + queue1_start);
    if (insert_index == queue1_size) {
        idi copy_count = (queue1_size + queue2_size > queue1_length) ?
                            queue1_length - queue1_size :
                            queue2_size;
        memmove(queue1.data() + queue1_start + queue1_size,
                queue2.data() + queue2_start,
                copy_count * sizeof(Candidate));
        queue1_size += copy_count;
        return;
    }
    if (queue2[queue2_start].id_ != it_loc->id_) {
        // Not Duplicate
        add_into_queue_at(
                queue2[queue2_start],
                queue1,
                insert_index,
                queue1_start,
                queue1_size,
                queue1_length);
    }
    if (queue2_size == 1) {
        return;
    }

    // Insert
    idi q_i_1 = insert_index + 1 + queue1_start;
    idi q_i_2 = queue2_start + 1;
    idi q_i_1_bound = queue1_start + queue1_size; // When queue1_size is updated, so should be q_i_1_bound.
    const idi q_i_2_bound = queue2_start + queue2_size;
//    idi insert_i;
    for (idi insert_i = insert_index + 1; insert_i < queue1_length; ++insert_i) {
        if (q_i_1 >= q_i_1_bound) {
            queue1_size += std::min(queue1_length - insert_i, q_i_2_bound - q_i_2);
            for ( ; insert_i < queue1_size; ++insert_i) {
                queue1[queue1_start + insert_i] = queue2[q_i_2++];
            }
            break;
        } else if (q_i_2 >= q_i_2_bound) {
            break;
        } else if (queue1[q_i_1] < queue2[q_i_2]) {
            ++q_i_1;
        } else if (queue2[q_i_2] < queue1[q_i_1]) {
            add_into_queue_at(
                    queue2[q_i_2++],
                    queue1,
                    insert_i,
                    queue1_start,
                    queue1_size,
                    queue1_length);
            ++q_i_1;
            q_i_1_bound = queue1_start + queue1_size;
        } else {
            // Duplicate
            ++q_i_2;
            ++q_i_1;
        }
    }
}

inline idi Searching::merge_all_queues_para_list(
        std::vector< std::vector<Candidate> > &local_queues_list,
        std::vector<idi> &local_queues_ends,
        std::vector<Candidate> &set_L,
        const idi L)
{
    int size = 1 << (static_cast<idi>(log2(num_threads_)));
    idi log2size = static_cast<idi>(log2(size));
    for (idi d = 0; d < log2size; ++d) {
        uint32_t by = 1 << (d + 1);
#pragma omp parallel for
        for (int i = 0; i < size; i += by) {
            idi ai = i + (1 << (d + 1)) - 1; // i + 2^(d+1) - 1
            idi bi = i + (1 << d) - 1; // i + 2^d - 1
            if (0 == local_queues_ends[bi]) {
                continue;
            }
            if (local_queues_ends[ai] == 0) {
                local_queues_list[ai].swap(local_queues_list[bi]);
                std::swap(local_queues_ends[ai], local_queues_ends[bi]);
                continue;
            }
//            else if (local_queues_ends[ai] < L && local_queues_ends[bi] >= L) {
//                local_queues_list[ai].swap(local_queues_list[bi]);
//                std::swap(local_queues_ends[ai], local_queues_ends[bi]);
//            }
//            merge_two_queues_into_1st_queue_seq(
//                    local_queues_list[ai],
//                    0,
//                    local_queues_ends[ai],
//                    local_queues_list[bi],
//                    0,
//                    local_queues_ends[bi]);
            idi tmp_length = local_queues_ends[ai] + local_queues_ends[bi];
            std::vector<Candidate> tmp_queue(tmp_length);
            std::merge(
                    local_queues_list[ai].begin(),
                    local_queues_list[ai].begin() + local_queues_ends[ai],
                    local_queues_list[bi].begin(),
                    local_queues_list[bi].begin() + local_queues_ends[bi],
                    tmp_queue.begin());
            if (tmp_length > L) {
                tmp_queue.resize(L);
                tmp_length = L;
            } else if (tmp_length < L) {
                tmp_queue.resize(L);
            }
            local_queues_list[ai].swap(tmp_queue);
            local_queues_ends[ai] = tmp_length;
//            {// Print queue a
//                printf("d: %u "
//                       "i: %u "
//                       "ai: %u "
//                       "local_queues_ends[%d]: %d\n",
//                       d,
//                       i,
//                       ai,
//                       ai,
//                       local_queues_ends[ai]);
//                for (idi i_q = 0; i_q < local_queues_ends[ai]; ++i_q) {
//                    printf("[%u]: "
//                           "id: %u "
//                           "dist: %f\n",
//                           i_q,
//                           local_queues_list[ai][i_q].id_,
//                           local_queues_list[ai][i_q].distance_);
//                }
//            }
        }
    }
    // Remain, prefix-sum-like merge
    if (size != num_threads_) {
        for (int i = size; i < num_threads_; ++i) {
            idi ai = i;
            idi bi = i - 1;
            if (0 == local_queues_ends[bi]) {
                continue;
            }
            if (local_queues_ends[ai] == 0) {
                local_queues_list[ai].swap(local_queues_list[bi]);
                std::swap(local_queues_ends[ai], local_queues_ends[bi]);
                continue;
            }
//            else if (local_queues_ends[ai] < L && local_queues_ends[bi] >= L) {
//                local_queues_list[ai].swap(local_queues_list[bi]);
//                std::swap(local_queues_ends[ai], local_queues_ends[bi]);
//            }
//            merge_two_queues_into_1st_queue_seq(
//                    local_queues_list[ai],
//                    0,
//                    local_queues_ends[ai],
//                    local_queues_list[bi],
//                    0,
//                    local_queues_ends[bi]);
            idi tmp_length = local_queues_ends[ai] + local_queues_ends[bi];
            std::vector<Candidate> tmp_queue(tmp_length);
            std::merge(
                    local_queues_list[ai].begin(),
                    local_queues_list[ai].begin() + local_queues_ends[ai],
                    local_queues_list[bi].begin(),
                    local_queues_list[bi].begin() + local_queues_ends[bi],
                    tmp_queue.begin());
            if (tmp_length > L) {
                tmp_queue.resize(L);
                tmp_length = L;
            } else if (tmp_length < L) {
                tmp_queue.resize(L);
            }
            local_queues_list[ai].swap(tmp_queue);
            local_queues_ends[ai] = tmp_length;
        }
    }
    // Merge into set_L
    idi r = L;
    if (local_queues_ends[num_threads_ - 1]) {
        r = merge_two_queues_into_1st_queue_seq_fixed(
                set_L,
                0,
                L,
                local_queues_list[num_threads_ - 1],
                0,
                local_queues_ends[num_threads_ - 1]);
    }
    // Reset local_queues_ends
    std::fill(local_queues_ends.begin(), local_queues_ends.end(), 0);

    return r;
}

/* Function:
 * Use large local_queues_array as a concatenation of all queues
 */
inline idi Searching::merge_all_queues_para_array(
        std::vector<Candidate> &set_L,
        std::vector<idi> &local_queues_ends,
        const idi local_queue_length,
        const idi L)
{
    const idi num_queues = num_threads_;
    idi nk = L;
    int size = 1 << (static_cast<idi>(log2(num_queues)));
    idi log2size = static_cast<idi>(log2(size));
    for (idi d = 0; d < log2size; ++d) {
        uint32_t by = 1 << (d + 1);
#pragma omp parallel for
        for (int i = 0; i < size; i += by) {
            idi ai = i + (1 << (d + 1)) - 1; // i + 2^(d+1) - 1
            idi a_start = ai * local_queue_length;
            idi bi = i + (1 << d) - 1; // i + 2^d - 1
            idi b_start = bi * local_queue_length;
            if (0 == local_queues_ends[bi]) {
                continue;
            }
            if (local_queues_ends[ai] == 0) {
                std::copy(set_L.begin() + b_start,
                        set_L.begin() + b_start + local_queues_ends[bi],
                        set_L.begin() + a_start); // Copy bi to ai
                local_queues_ends[ai] = local_queues_ends[bi];
                local_queues_ends[bi] = 0;
                continue;
            }
            if (ai != static_cast<idi>(num_queues - 1)) {
                merge_two_queues_into_1st_queue_seq_incr(
                        set_L,
                        a_start,
                        local_queues_ends[ai],
                        local_queue_length,
                        set_L,
                        b_start,
                        local_queues_ends[bi]);
            } else {
                idi r = merge_two_queues_into_1st_queue_seq_fixed(
                        set_L,
                        a_start,
                        L,
                        set_L,
                        b_start,
                        local_queues_ends[bi]);
                if (r < nk) {
                    nk = r;
                }
            }
        }
    }
    // Remain, prefix-sum-like merge
    if (size != num_queues) {
        for (int i = size; i < num_queues; ++i) {
            idi ai = i;
            idi a_start = ai * local_queue_length;
            idi bi = i - 1;
            idi b_start = bi * local_queue_length;
            if (0 == local_queues_ends[bi]) {
                continue;
            }
            if (local_queues_ends[ai] == 0) {
                std::copy(set_L.begin() + b_start,
                          set_L.begin() + b_start + local_queues_ends[bi],
                          set_L.begin() + a_start); // Copy bi to ai
                local_queues_ends[ai] = local_queues_ends[bi];
                local_queues_ends[bi] = 0;
                continue;
            }
            if (ai != static_cast<idi>(num_queues - 1)) {
                merge_two_queues_into_1st_queue_seq_incr(
                        set_L,
                        a_start,
                        local_queues_ends[ai],
                        local_queue_length,
                        set_L,
                        b_start,
                        local_queues_ends[bi]);
            } else {
                idi r = merge_two_queues_into_1st_queue_seq_fixed(
                        set_L,
                        a_start,
                        L,
                        set_L,
                        b_start,
                        local_queues_ends[bi]);
                if (r < nk) {
                    nk = r;
                }
            }
        }
    }
    // Reset local_queues_ends
    // Not do this for Collector Idea or Selecting Idea
    std::fill(local_queues_ends.begin(), local_queues_ends.end() - 1, 0);
//    std::fill(local_queues_ends.begin(), local_queues_ends.end(), 0);

    return nk;
//    return r;
}

/* Function:
 * When merge all queues (in an array, and [num_threads_ - 1] is the global queue),
 * the starting local is at [queue_base]
 */
inline idi Searching::merge_all_queues_queue_base(
//        std::vector< std::vector<Candidate> > &local_queues_list,
        std::vector<Candidate> &set_L,
//        std::vector<Candidate> &local_queues_array,
        std::vector<idi> &local_queues_ends,
        const idi queue_base,
        const int real_threads,
        const idi local_queue_length,
//        std::vector<Candidate> &set_L,
        const idi L)
{
    idi nk = L;
    int size = 1 << (static_cast<idi>(log2(real_threads)));
//    int size = 1 << (static_cast<idi>(log2(num_threads_)));
    idi log2size = static_cast<idi>(log2(size));
    for (idi d = 0; d < log2size; ++d) {
        idi by = 1 << (d + 1);
        idi i_bound = size + queue_base;
#pragma omp parallel for num_threads(real_threads)
        for (idi i = queue_base; i < i_bound; i += by) {
//        for (int i = 0; i < size; i += by) {
//            idi ai = i + (1 << (d + 1)) - 1 + queue_base; // i + 2^(d+1) - 1
            idi ai = i + (1 << (d + 1)) - 1; // i + 2^(d+1) - 1
            idi a_start = ai * local_queue_length;
//            idi bi = i + (1 << d) - 1 + queue_base; // i + 2^d - 1
            idi bi = i + (1 << d) - 1; // i + 2^d - 1
            idi b_start = bi * local_queue_length;
            if (0 == local_queues_ends[bi]) {
                continue;
            }
            if (local_queues_ends[ai] == 0) {
//                local_queues_list[ai].swap(local_queues_list[bi]);
                std::copy(set_L.begin() + b_start,
                          set_L.begin() + b_start + local_queues_ends[bi],
                          set_L.begin() + a_start); // Copy bi to ai
                local_queues_ends[ai] = local_queues_ends[bi];
                local_queues_ends[bi] = 0;
                continue;
            }
            if (ai != static_cast<idi>(num_threads_ - 1)) {
                merge_two_queues_into_1st_queue_seq_incr(
                        set_L,
                        a_start,
                        local_queues_ends[ai],
                        local_queue_length,
                        set_L,
                        b_start,
                        local_queues_ends[bi]);
            } else {
                idi r = merge_two_queues_into_1st_queue_seq_fixed(
                        set_L,
                        a_start,
                        L,
                        set_L,
                        b_start,
                        local_queues_ends[bi]);
                if (r < nk) {
                    nk = r;
                }
            }
        }
    }
    // Remain, prefix-sum-like merge
    if (size != real_threads) {
//    if (size != num_threads_) {
        for (int i = size + queue_base; i < num_threads_; ++i) {
//        for (int i = size; i < num_threads_; ++i) {
            idi ai = i;
            idi a_start = ai * local_queue_length;
            idi bi = i - 1;
            idi b_start = bi * local_queue_length;
            if (0 == local_queues_ends[bi]) {
                continue;
            }
            if (local_queues_ends[ai] == 0) {
                std::copy(set_L.begin() + b_start,
                          set_L.begin() + b_start + local_queues_ends[bi],
                          set_L.begin() + a_start); // Copy bi to ai
                local_queues_ends[ai] = local_queues_ends[bi];
                local_queues_ends[bi] = 0;
                continue;
            }
            if (ai != static_cast<idi>(num_threads_ - 1)) {
                merge_two_queues_into_1st_queue_seq_incr(
                        set_L,
                        a_start,
                        local_queues_ends[ai],
                        local_queue_length,
                        set_L,
                        b_start,
                        local_queues_ends[bi]);
            } else {
                idi r = merge_two_queues_into_1st_queue_seq_fixed(
                        set_L,
                        a_start,
                        L,
                        set_L,
                        b_start,
                        local_queues_ends[bi]);
                if (r < nk) {
                    nk = r;
                }
            }
        }
    }
    // Reset local_queues_ends
    std::fill(local_queues_ends.begin(), local_queues_ends.end() - 1, 0);
//    std::fill(local_queues_ends.begin(), local_queues_ends.end(), 0);

    return nk;
//    return r;
}

inline void Searching::merge_two_consecutive_queues_in_place(
        std::vector<Candidate> &two_queues,
        const idi base_1,
//        const idi &end_1,
        const idi base_2,
        const idi &length_2)
{
    idi tid = omp_get_thread_num();

    idi index_1 = base_1;
    idi index_2 = base_2;
    const idi bound_2 = base_2 + length_2;

    while (index_1 < index_2
           && index_2 < bound_2) {
        Candidate e_1 = two_queues[index_1];
        Candidate e_2 = two_queues[index_2];
        if (e_1 < e_2) {
            ++index_1;
        } else if (e_2 < e_1) {
//            time_memmove_list_[tid] -= WallTimer::get_time_mark();
            std::memmove(two_queues.data() + index_1 + 1,
                         two_queues.data() + index_1,
                         (index_2 - index_1) * sizeof(Candidate));
//            time_memmove_list_[tid] += WallTimer::get_time_mark();
            two_queues[index_1] = e_2;
            ++index_1;
            ++index_2;
        } else { // Duplicate, but have no idea what to do right now
//            time_memmove_list_[tid] -= WallTimer::get_time_mark();
            std::memmove(two_queues.data() + index_1 + 1,
                         two_queues.data() + index_1,
                         (index_2 - index_1) * sizeof(Candidate));
//            time_memmove_list_[tid] += WallTimer::get_time_mark();
            index_1 += 2;
            ++index_2;
        }
    }
}

///* Function:
// * Merge all queues to the global queue, in a two-queue-merge way
// */
//inline idi Searching::merge_all_queues_all_together_in_sequential(
//        std::vector<Candidate> &set_L,
//        std::vector<idi> &local_queues_ends,
//        const idi local_queue_length,
//        const idi L)
//{
//    const idi num_queues = num_threads_;
//    const idi global_queue_base = (num_queues - 1) * local_queue_length;
//    std::vector<idi> queue_heads(num_queues, 0);
//    idi queue_id_min;
//
////    bool is_finished = false;
//    bool is_1st_selected = true;
//    idi nk = L; // The highest location of insertion.
//    {
//        for (idi q_i = 0; q_i < num_queues; ++q_i) {
//            if (0 == local_queues_ends[q_i]) {
//                continue;
//            }
//            _mm_prefetch(set_L.data() + q_i * local_queue_length, _MM_HINT_T0);
//        }
//    }
//    while (queue_heads[num_queues - 1] < L) {
////        time_compare_minimum_ -= WallTimer::get_time_mark();
//        queue_id_min = min_all_queues_at_heads(
//                set_L,
//                queue_heads,
//                local_queues_ends,
//                local_queue_length,
//                L);
////        time_compare_minimum_ += WallTimer::get_time_mark();
//        if (queue_id_min != num_queues - 1) { // Not in the global queue
////            time_insert_ -= WallTimer::get_time_mark();
//            insert_one_element_at(
//                    set_L[queue_heads[queue_id_min] + queue_id_min * local_queue_length],
//                    set_L,
//                    queue_heads[num_queues - 1],
//                    global_queue_base,
//                    L);
////            time_insert_ += WallTimer::get_time_mark();
//            if (is_1st_selected) { // Get the highest inserting location
//                is_1st_selected = false;
//                nk = queue_heads[num_queues - 1];
//            }
//            ++queue_heads[queue_id_min];
//        }
//        ++queue_heads[num_queues - 1];
//    }
//
//    // Reset local_queues_ends
//    std::fill(local_queues_ends.begin(), local_queues_ends.end() - 1, 0);
//    return nk;
//}

///* Function:
// * Find the minimum among queues at their head locations
// */
//inline idi Searching::min_all_queues_at_heads(
//        const std::vector<Candidate> &set_L,
//        std::vector<idi> &queue_heads,
//        const std::vector<idi> &local_queues_ends,
//        const idi local_queue_length,
//        const idi L)
//{
//    const idi num_queues = num_threads_;
//    idi min_queue_id = num_queues - 1;
//    Candidate min_candidate = set_L[queue_heads[min_queue_id] + min_queue_id * local_queue_length];
//
//    for (idi q_i = 0; q_i < num_queues - 1; ++q_i) {
//        if (queue_heads[q_i] >= local_queues_ends[q_i]) { // q_i finished
//            continue;
//        }
//        const Candidate &ele = set_L[queue_heads[q_i] + q_i * local_queue_length];
//        if (ele < min_candidate) {
//            min_candidate = ele;
//            min_queue_id = q_i;
//        } else if (ele.id_ == min_candidate.id_) { // Redundant element
//            ++queue_heads[q_i];
//        }
//    }
//
//    return min_queue_id;
//}

inline void Searching::merge_in_set_L(
        std::vector<Candidate> &set_L,
        const idi set_L_length,
        const idi num_queues,
        const idi local_queue_length)
{
    idi size = 1 << (static_cast<idi>(log2(num_queues)));
    idi log2size = static_cast<idi>(log2(size));

    for (idi d = 0; d < log2size; ++d) {
        const idi merge_length = (local_queue_length << d);
        idi by = 1 << (d + 1);
        // Parallel for
#pragma omp parallel for
        for (idi i = 0; i < size; i += by) {
//			idi a = i + (1 << d) - 1;
//			idi b = i + (1 << (d + 1)) - 1;
            idi a = i;
            idi b = i + (1 << d);

            idi base_a = a * local_queue_length;
            idi base_b = b * local_queue_length;
            if (base_a >= set_L_length || base_b >= set_L_length) {
                continue;
            }
            idi length_b;
            if (a + by < size) {
                length_b = merge_length;
            } else { // The last one
                if (size == num_queues) {
                    length_b = set_L_length - base_b;
                } else {
                    length_b = merge_length;
                }
            }
//                printf("a: %u b: %u "
//                       "base_a: %u base_b: %u length_b: %u\n",
//                       a, b,
//                       base_a, base_b, length_b);
            merge_two_consecutive_queues_in_place(
                    set_L,
                    base_a,
                    base_b,
                    length_b);
        }
    }

    if (size != num_queues) {
        for (idi i = size; i < num_queues; ++i) {
            idi a = 0;
            idi b = i;

            idi base_a = a;
            idi base_b = b * local_queue_length;
            if (base_b >= set_L_length) {
                continue;
            }
            idi length_b;
            if (b != num_queues - 1) {
                length_b = local_queue_length;
            } else {
                length_b = set_L_length - base_b;
            }
//                printf("a: %u b: %u "
//                       "base_a: %u base_b: %u length_b: %u\n",
//                       a, b,
//                       base_a, base_b, length_b);
            merge_two_consecutive_queues_in_place(
                    set_L,
                    base_a,
                    base_b,
                    length_b);
        }
    }
}

/*
 * 7/5/2020-20:27
 * Every queue keeps only elements which can be ordered in the top-L globally.
 * local_queues_lengths records the end location for all queues
 */
inline distf Searching::selecting_top_L_seq(
        std::vector<Candidate> &set_L,
        const idi global_L,
//        const idi local_L,
        const idi num_queues,
        const std::vector<idi> &local_queues_bases,
        std::vector<idi> &local_queues_ends)
{
    std::vector<idi> pointers(num_queues, 0);

    distf bound_lth;
    idi rank = 0;
    bool is_finished = false;
    distf min_dist = FLT_MAX;
    distf min_q_i;
    while (rank < global_L) {
        is_finished = true;
        bound_lth = min_dist;
        min_dist = FLT_MAX;

        for (idi q_i = 0; q_i < num_queues; ++q_i) {
            if (pointers[q_i] >= local_queues_ends[q_i]) {
                // q_i is finished
                continue;
            }
            is_finished = false;
            idi sub = pointers[q_i] + local_queues_bases[q_i];
//            idi sub = pointers[q_i] + q_i * local_L;
            distf tmp_dist = set_L[sub].distance_;
            if (tmp_dist < min_dist) {
                min_dist = tmp_dist;
                min_q_i = q_i;
            }
        }
        if (is_finished) {
            {//test
                printf("Error: selecting_top_L_seq: only found %u elements but global_L is %u.\n",
                        rank,
                        global_L);
            }
            break;
        }
        ++pointers[min_q_i];
        ++rank;
    }
    std::copy(pointers.begin(), pointers.end(), local_queues_ends.begin());

    return bound_lth;
}

inline void Searching::search_with_top_m(
        const PANNS::idi M,
        const PANNS::idi query_id,
        const PANNS::idi K,
        const PANNS::idi L,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K)
{
    boost::dynamic_bitset<> is_visited(num_v_);

    {
        for (idi c_i = 0; c_i < L; ++c_i) {
            is_visited[init_ids[c_i]] = true;
        }
    }

    const dataf *query_data = queries_load_ + query_id  * dimension_;
    for (idi v_i = 0; v_i < L; ++v_i) {
        idi v_id = init_ids[v_i];
        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
    }
    // Get the distances of all candidates, store in the set set_L.
    for (unsigned i = 0; i < L; i++) {
        unsigned v_id = init_ids[i];
        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
        dataf norm = *v_data++;
        ++count_distance_computation_;
        distf dist = compute_distance_with_norm(v_data, query_data, norm);
        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
    }
    std::sort(set_L.begin(), set_L.begin() + L);

    std::vector<idi> top_m_candidates(M);
    idi top_m_candidates_end = 0;
    idi k = 0; // Index of first unchecked candidate.
    idi tmp_count = 0; // for debug
    while (k < L) {
        ++tmp_count;

        unsigned nk = L;

        // Select M candidates
        idi last_k = L;
        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
            if (set_L[c_i].is_checked_) {
                continue;
            }
            last_k = c_i; // Record the location of the last candidate selected.
            set_L[c_i].is_checked_ = true;
            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
        }

        // Push M candidates' neighbors into the queue.
        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
            idi cand_id = top_m_candidates[c_i];
            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
            idi out_degree = *out_edges++;
            for (idi n_i = 0; n_i < out_degree; ++n_i) {
                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
            }
            for (idi e_i = 0; e_i < out_degree; ++e_i) {
                idi nb_id = out_edges[e_i];
                if (is_visited[nb_id]) {
                    continue;
                }
                is_visited[nb_id] = true;
                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
                dataf norm = *nb_data++;
                ++count_distance_computation_;
                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
                if (dist > set_L[L-1].distance_) {
                    continue;
                }
                Candidate cand(nb_id, dist, false);
                idi r = insert_into_queue(set_L, L, cand);
                if (r < nk) {
                    nk = r;
                }
            }
        }
        top_m_candidates_end = 0; // Clear top_m_candidates

        if (nk <= last_k) {
            k = nk;
        } else {
            k = last_k + 1;
        }
    }

    for (idi k_i = 0; k_i < K; ++k_i) {
        set_K[k_i] = set_L[k_i].id_;
    }
}

inline void Searching::search_with_top_m_scale_m(
        const PANNS::idi value_M_max,
        const PANNS::idi query_id,
        const PANNS::idi K,
        const PANNS::idi L,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K,
        std::vector<idi> &top_m_candidates,
        boost::dynamic_bitset<> &is_visited)
{
//    boost::dynamic_bitset<> is_visited(num_v_);

    {
        for (idi c_i = 0; c_i < L; ++c_i) {
            is_visited[init_ids[c_i]] = true;
        }
    }

    const dataf *query_data = queries_load_ + query_id  * dimension_;
    for (idi v_i = 0; v_i < L; ++v_i) {
        idi v_id = init_ids[v_i];
        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
    }
    // Get the distances of all candidates, store in the set set_L.
    for (unsigned i = 0; i < L; i++) {
        unsigned v_id = init_ids[i];
        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
        dataf norm = *v_data++;
        ++count_distance_computation_;
        distf dist = compute_distance_with_norm(v_data, query_data, norm);
        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
    }
    std::sort(set_L.begin(), set_L.begin() + L);

//    std::vector<idi> top_m_candidates(M);
    idi top_m_candidates_end = 0;
    idi k = 0; // Index of first unchecked candidate.
    idi tmp_count = 0; // for debug
    idi M = 1;
    while (k < L) {
        ++tmp_count;

        unsigned nk = L;

        // Select M candidates
        idi last_k = L;
        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
            if (set_L[c_i].is_checked_) {
                continue;
            }
            last_k = c_i; // Record the location of the last candidate selected.
            set_L[c_i].is_checked_ = true;
            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
        }

        // Push M candidates' neighbors into the queue.
        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
            idi cand_id = top_m_candidates[c_i];
            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
            idi out_degree = *out_edges++;
            for (idi n_i = 0; n_i < out_degree; ++n_i) {
                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
            }
            for (idi e_i = 0; e_i < out_degree; ++e_i) {
                idi nb_id = out_edges[e_i];
                if (is_visited[nb_id]) {
                    continue;
                }
                is_visited[nb_id] = true;
                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
                dataf norm = *nb_data++;
                ++count_distance_computation_;
                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
                if (dist > set_L[L-1].distance_) {
                    continue;
                }
                Candidate cand(nb_id, dist, false);
                idi r = insert_into_queue(set_L, L, cand);
                if (r < nk) {
                    nk = r;
                }
            }
        }
        top_m_candidates_end = 0; // Clear top_m_candidates

        if (nk <= last_k) {
            k = nk;
        } else {
            k = last_k + 1;
        }

        if (M < value_M_max) {
            M <<= 1;
        }
    }

    for (idi k_i = 0; k_i < K; ++k_i) {
        set_K[k_i] = set_L[k_i].id_;
    }
    {// Reset
        is_visited.reset();
    }
}

////void Searching::search_with_top_m(
//inline void Searching::search_with_top_m_to_get_distance_range(
//        const PANNS::idi M,
//        const PANNS::idi query_id,
////        const PANNS::idi K,
//        const PANNS::idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids)
////        std::vector<idi> &set_K)
//{
//    dist_max_ = -FLT_MAX;
//    dist_min_ = FLT_MAX;
//    boost::dynamic_bitset<> is_visited(num_v_);
//
//    {
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = true;
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
////        ++count_distance_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
////        {// For distance range
////            if (dist > dist_max_) {
////                dist_max_ = dist;
////            }
////            if (dist < dist_min_) {
////                dist_min_ = dist;
////            }
////        }
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//
//    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    while (k < L) {
//        ++tmp_count;
//
//        unsigned nk = L;
//
//        // Select M candidates
//        idi last_k = L;
//        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//            if (set_L[c_i].is_checked_) {
//                continue;
//            }
//            last_k = c_i; // Record the location of the last candidate selected.
//            set_L[c_i].is_checked_ = true;
//            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
//        }
//
//        // Push M candidates' neighbors into the queue.
//        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//            idi cand_id = top_m_candidates[c_i];
//            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
//                if (is_visited[nb_id]) {
//                    continue;
//                }
//                is_visited[nb_id] = true;
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
////                ++count_distance_computation;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
////                if (dist >= set_L[L-1].distance_) {
////                    continue;
////                }
//                Candidate cand(nb_id, dist, false);
//                idi r = insert_into_queue(set_L, L, cand);
//                if (r < nk) {
//                    nk = r;
//                }
////                {// For distance range
////                    if (dist > dist_max_) {
////                        dist_max_ = dist;
////                    }
////                    if (dist < dist_min_) {
////                        dist_min_ = dist;
////                    }
////                }
//            }
//        }
//        top_m_candidates_end = 0; // Clear top_m_candidates
//
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
//        {// For histogram
//            for (idi i_l = 0; i_l < L; ++i_l) {
//                distf dist = set_L[i_l].distance_;
//                {// For distance range
//                    if (dist > dist_max_) {
//                        dist_max_ = dist;
//                    }
//                    if (dist < dist_min_) {
//                        dist_min_ = dist;
//                    }
//                }
//            }
//        }
//    }
//
////    for (idi k_i = 0; k_i < K; ++k_i) {
////        set_K[k_i] = set_L[k_i].id_;
////    }
//}
//
////void Searching::search_with_top_m(
//inline void Searching::search_with_top_m_myths_M(
//        const PANNS::idi M,
//        const PANNS::idi query_id,
//        const PANNS::idi K,
//        const PANNS::idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K)
//{
////    {//test
////        printf("query_id: %u\n", query_id);
////    }
//    const idi loc_range = L / 3;
//
//
//    boost::dynamic_bitset<> is_visited(num_v_);
//
//    {
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = true;
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
////        ++count_distance_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//
////    {// For histogram
////        const distf dist_range = dist_max_ - dist_min_;
////        printf("iter:%u\n", 0);
////        for (idi i_l = 0; i_l < L; ++i_l) {
////            printf("%f\n", (set_L[i_l].distance_ - dist_min_) / dist_range * 100.0);
////        }
////    }
//
//    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    while (k < L) {
//        std::vector<idi> range_count(3, 0);
//        idi zero_inserted_count = 0;
////        {//test
////            printf("tmp_count: %u\n", tmp_count);
////        }
//        ++tmp_count;
//
//        unsigned nk = L;
//
//        // Select M candidates
//        idi last_k = L;
//        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//            if (set_L[c_i].is_checked_) {
//                continue;
//            }
//            last_k = c_i; // Record the location of the last candidate selected.
//            set_L[c_i].is_checked_ = true;
//            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
//        }
////        {//test
////            printf("top_m_candidates_ends: %u\n", top_m_candidates_end);
////        }
//        {
//            if (0 == top_m_candidates_end) {
//                break;
//            }
//        }
//
//
//        uint64_t count_neighbors = 0;
//        uint64_t count_inserted = 0;
//        std::vector<idi> locs_to_count(M);
//        // Push M candidates' neighbors into the queue.
//        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//            idi cand_id = top_m_candidates[c_i];
//            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//
//            count_neighbors += out_degree;
//            idi num_inserted = 0;
//
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
//                if (is_visited[nb_id]) {
//                    continue;
//                }
//                is_visited[nb_id] = true;
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
////                ++count_distance_computation;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
////                if (dist >= set_L[L-1].distance_) {
////                    continue;
////                }
//                ++num_inserted;
//                Candidate cand(nb_id, dist, false);
//                idi r = insert_into_queue(set_L, L, cand);
////                {
////                    printf("c_i: %u "
////                           "count: %u "
////                           "loc_inserted: %u\n",
////                           c_i,
////                           num_inserted,
////                           r);
////                }
//                if (r < nk) {
//                    nk = r;
//                }
//                {
//                    ++range_count[r / loc_range];
//                }
//            }
//            {
//                if (0 == num_inserted) {
//                    ++zero_inserted_count;
//                }
//                locs_to_count[c_i] = num_inserted;
//                count_inserted += num_inserted;
//            }
////            {
////                printf("c_i: %u "
////                       "num_inserted: %u\n",
////                       c_i,
////                       num_inserted);
////            }
//        }
////        {
////            for (idi c_i = top_m_candidates_end; c_i < M; ++c_i) {
////                locs_to_count[c_i] = 0;
////            }
////            printf("iter:%u\n", tmp_count);
////            for (idi c_i = 0; c_i < M; ++c_i) {
////                printf("%u %u\n", c_i, locs_to_count[c_i]);
////            }
////        }
////        {//test
////            idi sum = 0;
////            for (const idi ct : range_count) sum += ct;
////            printf("tmp_count: %u "
////                   "k: %u "
////                   "actual_M: %u %.1f%% "
////                   "zero_ins: %u %.1f%% "
////                   "1/3: %u %.1f%% "
////                   "2/3: %u %.1f%% "
////                   "3/3: %u %.1f%%\n",
////                   tmp_count,
////                   k,
////                   top_m_candidates_end, 100.0 * top_m_candidates_end / M,
////                   zero_inserted_count, 100.0 * zero_inserted_count / top_m_candidates_end,
////                   range_count[0], 100.0 * range_count[0] / sum,
////                   range_count[1], 100.0 * range_count[1] / sum,
////                   range_count[2], 100.0 * range_count[2] / sum);
////        }
//        top_m_candidates_end = 0; // Clear top_m_candidates
//
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
//        {
//            printf("query:%uiter: %u "
//                   "#neighbors: %lu "
//                   "#inserted: %lu "
//                   "ratio: %.2f%%\n",
//                   query_id, tmp_count,
//                   count_neighbors,
//                   count_inserted,
//                   100.0 * count_inserted / count_neighbors);
//        }
////        {// For histogram
//////            const auto it_min = std::min_element(set_L.begin(), set_L.end());
//////            const auto it_max = std::max_element(set_L.begin(), set_L.end());
//////            const distf dist_min = it_min->distance_;
//////            const distf dist_max = it_max->distance_;
//////            const distf dist_min = it_min->distance_ - 1.0;
//////            const distf dist_max = it_max->distance_ + 1.0;
////            const distf dist_range = dist_max_ - dist_min_;
//////            const distf dist_range = dist_max - dist_min;
//////            {
//////                printf("it_min->distance_: %f dist_min: %f\n",
//////                        it_min->distance_, dist_min);
//////            }
//////            const distf dist_range = it_max->distance_ - it_min->distance_;
////            printf("iter:%u\n", tmp_count);
////            for (idi i_l = 0; i_l < L; ++i_l) {
//////                printf("%f\n", set_L[i_l].distance_);
//////                printf("%f\n", (set_L[i_l].distance_ - dist_min) / dist_range * 100.0);
////                printf("%f\n", (set_L[i_l].distance_ - dist_min_) / dist_range * 100.0);
//////                printf("%.2f\n", (set_L[i_l].distance_ - it_min->distance_) / dist_range * 100.0);
////            }
////        }
//    }
//
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
//    if (query_id == 3) {
//        exit(1);
//    }
//}
//
//// Sequential Top-M algorithm for profiling purpose: byte array, CAS, and OpenMP
////void Searching::search_with_top_m(
//inline void Searching::search_with_top_m_profile_bit_CAS(
//        const PANNS::idi M,
//        const PANNS::idi query_id,
//        const PANNS::idi K,
//        const PANNS::idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K)
//{
////    std::vector<uint8_t> is_visited(num_v_, 0); // Byte array
////    boost::dynamic_bitset<> is_visited(num_v_); // Bit array
//    BitVector is_visited(num_v_);
//
//    {
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < L; ++c_i) {
////            is_visited[init_ids[c_i]] = true;
//            is_visited.atomic_set_bit(init_ids[c_i]);
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//#pragma omp parallel for
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
//#pragma omp parallel for
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
////        ++count_distance_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//
//    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    while (k < L) {
//        ++tmp_count;
//
//        unsigned nk = L;
//
//        // Select M candidates
//        idi last_k = L;
//        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//            if (set_L[c_i].is_checked_) {
//                continue;
//            }
//            last_k = c_i; // Record the location of the last candidate selected.
//            set_L[c_i].is_checked_ = true;
//            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
//        }
//
//        // Push M candidates' neighbors into the queue.
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//            idi cand_id = top_m_candidates[c_i];
//            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
////                if (is_visited[nb_id]) {
////                    continue;
////                }
////                is_visited[nb_id] = true;
//
////                if (!AtomicOps::CAS(is_visited.data() + nb_id,
////                                    static_cast<uint8_t>(0),
////                                    static_cast<uint8_t>(1))) {
////                    continue;
////                }
//                {// Self-defined BitVector
//                    if (is_visited.atomic_is_bit_set(nb_id)) {
//                        continue;
//                    }
//                    is_visited.atomic_set_bit(nb_id);
//                }
//
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
////                ++count_distance_computation;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
////                if (dist >= set_L[L-1].distance_) {
////                    continue;
////                }
//                Candidate cand(nb_id, dist, false);
//                idi r = insert_into_queue(set_L, L, cand);
//                if (r < nk) {
//                    nk = r;
//                }
//            }
//        }
//        top_m_candidates_end = 0; // Clear top_m_candidates
//
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
//    }
//
//#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
////
////    {//test
////        for (idi k_i = 0; k_i < K; ++k_i) {
////            printf("%u: %u: %u %f\n",
////                   query_id,
////                    k_i, set_L[k_i].id_, set_L[k_i].distance_);
////        }
////        exit(1);
////    }
//}
///// Backup
//inline void Searching::search_with_top_m(
//        const PANNS::idi M,
//        const PANNS::idi query_id,
//        const PANNS::idi K,
//        const PANNS::idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K)
////        std::vector< std::vector<idi> > &top_m_list)
//{
//    boost::dynamic_bitset<> is_visited(num_v_);
//
//    {
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = true;
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
////        ++count_distance_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//
//    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    while (k < L) {
//        ++tmp_count;
//
//        unsigned nk = L;
//
//        // Select M candidates
//        idi last_k = L;
//        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//            if (set_L[c_i].is_checked_) {
//                continue;
//            }
//            last_k = c_i; // Record the location of the last candidate selected.
//            set_L[c_i].is_checked_ = true;
//            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
//        }
//
//        // Push M candidates' neighbors into the queue.
//        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//            idi cand_id = top_m_candidates[c_i];
//            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
//                if (is_visited[nb_id]) {
//                    continue;
//                }
//                is_visited[nb_id] = true;
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
////                ++count_distance_computation;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
////                if (dist >= set_L[L-1].distance_) {
////                    continue;
////                }
//                Candidate cand(nb_id, dist, false);
//                idi r = insert_into_queue(set_L, L, cand);
//                if (r < nk) {
//                    nk = r;
//                }
//            }
//        }
//        top_m_candidates_end = 0; // Clear top_m_candidates
//
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
//    }
//
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
////
////    {//test
////        for (idi k_i = 0; k_i < K; ++k_i) {
////            printf("%u: %u: %u %f\n",
////                   query_id,
////                    k_i, set_L[k_i].id_, set_L[k_i].distance_);
////        }
////        exit(1);
////    }
//}

//
////// DEPRECATED: the is_visited array cannot be shared among threads.
//inline void Searching::search_with_top_m_no_local_arrays(
//        const PANNS::idi M,
//        const PANNS::idi query_id,
//        const PANNS::idi K,
//        const PANNS::idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K,
//        boost::dynamic_bitset<> &is_visited)
////        std::vector< std::vector<idi> > &top_m_list)
//{
////    boost::dynamic_bitset<> is_visited(num_v_);
//
//    {
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = true;
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
////        ++count_distance_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//
//    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    while (k < L) {
//        ++tmp_count;
//
//        unsigned nk = L;
//
//        // Select M candidates
//        idi last_k = L;
//        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//            if (set_L[c_i].is_checked_) {
//                continue;
//            }
//            last_k = c_i; // Record the location of the last candidate selected.
//            set_L[c_i].is_checked_ = true;
//            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
//        }
//
//        // Push M candidates' neighbors into the queue.
//        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//            idi cand_id = top_m_candidates[c_i];
//            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
//                if (is_visited[nb_id]) {
//                    continue;
//                }
//                is_visited[nb_id] = true;
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
////                ++count_distance_computation;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
////                if (dist >= set_L[L-1].distance_) {
////                    continue;
////                }
//                Candidate cand(nb_id, dist, false);
//                idi r = insert_into_queue(set_L, L, cand);
//                if (r < nk) {
//                    nk = r;
//                }
//            }
//        }
//        top_m_candidates_end = 0; // Clear top_m_candidates
//
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
//    }
//
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
////
////    {//test
////        for (idi k_i = 0; k_i < K; ++k_i) {
////            printf("%u: %u: %u %f\n",
////                   query_id,
////                    k_i, set_L[k_i].id_, set_L[k_i].distance_);
////        }
////        exit(1);
////    }
//}


inline void Searching::search_with_top_m_in_batch(
        const PANNS::idi M,
        const PANNS::idi batch_start,
        const PANNS::idi batch_size,
        const PANNS::idi K,
        const PANNS::idi L,
        std::vector< std::vector<Candidate> > &set_L_list,
        const std::vector<idi> &init_ids,
        std::vector< std::vector<idi> > &set_K_list)
{
    std::vector< boost::dynamic_bitset<> > is_visited_list(batch_size, boost::dynamic_bitset<> (num_v_));

    // Prepare the init_ids
    {
//#pragma omp parallel for
        for (idi q_i = 0; q_i < batch_size; ++q_i) {
            auto &is_visited = is_visited_list[q_i];
            for (idi c_i = 0; c_i < L; ++c_i) {
                is_visited[init_ids[c_i]] = true;
            }
        }
    }

    // Initialize set_L_list
    {
//#pragma omp parallel for
        for (idi q_i = 0; q_i < batch_size; ++q_i) {
            const dataf *query_data = queries_load_ + (q_i + batch_start) * dimension_;
            for (idi i = 0; i < L; i++) {
                idi v_id = init_ids[i];
                auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
                dataf norm = *v_data++;
//                ++count_distance_computation_;
                distf dist = compute_distance_with_norm(v_data, query_data, norm);
                set_L_list[q_i][i] = Candidate(v_id, dist, false); // False means not checked.
            }
            std::sort(set_L_list[q_i].begin(), set_L_list[q_i].begin() + L);
        }
    }

    {
        std::vector<idi> joint_queue(M * batch_size); // Joint queue for all shared top-M candidates
        idi joint_queue_end = 0;
        boost::dynamic_bitset<> is_in_joint_queue(num_v_);
//        std::vector< std::vector<idi> > cands_query_ids(num_v_, std::vector<idi>(batch_size)); // If candidate cand_id is selected by query q_i, q_i should be in cands_query_ids[cand_id].
//        std::vector<idi> cands_query_ids_ends(num_v_, 0);
        std::unordered_map< idi, std::vector<idi> > cands_query_ids(batch_size * M);
        std::vector<idi> ks(batch_size, 0); // Indices of every queue's first unchecked candidate.
        std::vector<idi> nks(batch_size, L); // Indices of highest candidate inserted
        std::vector<idi> last_ks(batch_size, L); // Indices of lowest candidate unchecked
        std::vector<idi> queries_not_finished(batch_size);
        idi queries_not_finished_end = batch_size;
        for (idi q_i = 0; q_i < batch_size; ++q_i) {
            queries_not_finished[q_i] = q_i;
        }
        bool is_finished = false;

        idi counter_for_debug = 0;

        while (!is_finished) {
            ++counter_for_debug;
            // Build the new joint queue
            // Traverse every query's queue
            for(idi q_i = 0; q_i < queries_not_finished_end; ++q_i) {
                idi q_local_id = queries_not_finished[q_i];
//                last_ks[q_local_id] = L;
                auto &set_L = set_L_list[q_local_id];
                idi top_m_count = 0;
                for (idi c_i = ks[q_local_id]; c_i < L && top_m_count < M; ++c_i) {
                    if (set_L[c_i].is_checked_) {
                        continue;
                    }
                    set_L[c_i].is_checked_ = true;
                    last_ks[q_local_id] = c_i;
                    ++top_m_count;
                    idi cand_id = set_L[c_i].id_;
                    // Record which query selected cand_id
                    auto tmp_c = cands_query_ids.find(cand_id);
                    if (tmp_c != cands_query_ids.end()) {
                        tmp_c->second.push_back(q_local_id);
                    } else {
                        cands_query_ids.emplace(cand_id, std::vector<idi>());
                        cands_query_ids[cand_id].reserve(batch_size);
                        cands_query_ids[cand_id].push_back(q_local_id);
                    }
//                    cands_query_ids[cand_id][cands_query_ids_ends[cand_id]++] = q_local_id;
                    // Add candidate cand_id into the joint queue
                    if (is_in_joint_queue[cand_id]) {
                        continue;
                    }
                    is_in_joint_queue[cand_id] = true;
                    joint_queue[joint_queue_end++] = cand_id;
                }
            }
            queries_not_finished_end = 0; // Clear queries_not_finished

            // Traverse every shared candidate
            for (idi c_i = 0; c_i < joint_queue_end; ++c_i) {
                idi cand_id = joint_queue[c_i];
                is_in_joint_queue[cand_id] = false; // Reset is_in_joint_queue
                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
                idi out_degree = *out_edges++;
                const auto &query_local_ids = cands_query_ids[cand_id];
                // Push neighbors to every queue of the queries that selected cand_id.
                // Traverse cand_id's neighbors
//                idi &q_i_bound = cands_query_ids_ends[cand_id];

//                for (idi q_i = 0; q_i < q_i_bound; ++q_i) {
//                    idi q_local_id = query_local_ids[q_i];
                for (idi q_local_id : query_local_ids) {
                    dataf *query_data = queries_load_ + (q_local_id + batch_start) * dimension_;
                    auto &is_visited = is_visited_list[q_local_id];
                    auto &set_L = set_L_list[q_local_id];
//                    // Traverse cand_id's neighbors
                    for (idi e_i = 0; e_i < out_degree; ++e_i) {
                        idi nb_id = out_edges[e_i];
                        if (is_visited[nb_id]) {
                            continue;
                        }
                        is_visited[nb_id] = true;
                        auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
                        dataf norm = *nb_data++;
//                        ++count_distance_computation_;
                        distf dist = compute_distance_with_norm(nb_data, query_data, norm);
                        if (dist > set_L[L-1].distance_) {
                            continue;
                        }
//                        if (dist >= set_L[L-1].distance_) {
//                            continue;
//                        }
                        Candidate new_cand(nb_id, dist, false);
                        idi insert_loc = insert_into_queue(set_L, L, new_cand);
                        if (insert_loc < nks[q_local_id]) {
                            nks[q_local_id] = insert_loc;
                        }
                    }
                }
                cands_query_ids.erase(cand_id);
//                q_i_bound = 0; // Clear cands_query_ids[cand_id]
            }
            joint_queue_end = 0; //  Clear joint_queue
            for (idi q_local_id = 0; q_local_id < batch_size; ++q_local_id) {
                if (nks[q_local_id] <= last_ks[q_local_id]) {
                    ks[q_local_id] = nks[q_local_id];
                } else {
                    ks[q_local_id] = last_ks[q_local_id] + 1;
                }
                nks[q_local_id] = L;
                last_ks[q_local_id] = L;
                if (ks[q_local_id] < L) {
                    queries_not_finished[queries_not_finished_end++] = q_local_id;
                }
            }
            if (!queries_not_finished_end) {
                is_finished = true;
            }
        }
    }

    {
        for (idi q_i = 0; q_i < batch_size; ++q_i) {
            for (idi c_i = 0; c_i < K && c_i < L; ++c_i) {
                set_K_list[q_i + batch_start][c_i] = set_L_list[q_i][c_i].id_;
            }
        }
    }
////
//    {//test
//        for (idi q_i = 0; q_i < batch_size; ++q_i) {
//            printf("query: %u\n", q_i + batch_start);
//            for (idi c_i = 0; c_i < K; ++c_i) {
//                printf("%u: %u %f\n", c_i, set_L_list[q_i][c_i].id_, set_L_list[q_i][c_i].distance_);
//            }
//        }
//    }
}



//inline void Searching::para_search_with_top_m_critical_area(
//        const PANNS::idi M,
//        const PANNS::idi query_id,
//        const PANNS::idi K,
//        const PANNS::idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K)
////        std::vector< std::vector<idi> > &top_m_list)
//{
//    std::vector<uint8_t> is_visited(num_v_, 0);
////    boost::dynamic_bitset<> is_visited(num_v_);
//
//    {
////#pragma omp parallel for
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = 1;
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
////#pragma omp parallel for
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
//        ++count_distance_computation_;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//
//    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    while (k < L) {
//        ++tmp_count;
//
//        unsigned nk = L;
////        int nk = L;
//
//        // Select M candidates
//        idi last_k = L;
//        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//            if (set_L[c_i].is_checked_) {
//                continue;
//            }
//            last_k = c_i; // Record the location of the last candidate selected.
//            set_L[c_i].is_checked_ = true;
//            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
//        }
//
//        // Push M candidates' neighbors into the queue.
//        // OpenMP reduction(min : nk) has a problem if nk is unsigned. nk might end up with being MAX_UINT.
////#pragma omp parallel for
////#pragma omp parallel for reduction(min : nk)
//        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//            idi cand_id = top_m_candidates[c_i];
//            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
////                if (is_visited[nb_id]) {
////                    continue;
////                }
////                is_visited[nb_id] = 1;
//
//                if (!AtomicOps::CAS(is_visited.data() + nb_id,
//                        static_cast<uint8_t>(0),
//                        static_cast<uint8_t>(1))) {
//                    continue;
//                }
//
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
//                ++count_distance_computation_;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
////                if (dist >= set_L[L-1].distance_) {
////                    continue;
////                }
//                Candidate cand(nb_id, dist, false);
//                idi r;
////#pragma omp critical
//                {
//                    r = insert_into_queue(set_L, L, cand);
//                    if (r < nk) {
//                        nk = r;
//                    }
//                }
//            }
//        }
//        top_m_candidates_end = 0; // Clear top_m_candidates
//
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
//    }
//
////#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
//}
//
//inline void Searching::para_search_with_top_m_critical_area_no_omp(
//        const PANNS::idi M,
//        const PANNS::idi query_id,
//        const PANNS::idi K,
//        const PANNS::idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K)
////        std::vector< std::vector<idi> > &top_m_list)
//{
//    std::vector<uint8_t> is_visited(num_v_, 0);
////    boost::dynamic_bitset<> is_visited(num_v_);
//
//    {
////#pragma omp parallel for
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = 1;
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
////#pragma omp parallel for
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
//        ++count_distance_computation_;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//
//    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    while (k < L) {
//        ++tmp_count;
//
//        unsigned nk = L;
////        int nk = L;
//
//        // Select M candidates
//        idi last_k = L;
//        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//            if (set_L[c_i].is_checked_) {
//                continue;
//            }
//            last_k = c_i; // Record the location of the last candidate selected.
//            set_L[c_i].is_checked_ = true;
//            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
//        }
//
//        // Push M candidates' neighbors into the queue.
//        // OpenMP reduction(min : nk) has a problem if nk is unsigned. nk might end up with being MAX_UINT.
////#pragma omp parallel for
////#pragma omp parallel for reduction(min : nk)
//        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//            idi cand_id = top_m_candidates[c_i];
//            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
////                if (is_visited[nb_id]) {
////                    continue;
////                }
////                is_visited[nb_id] = 1;
//
//                if (!AtomicOps::CAS(is_visited.data() + nb_id,
//                                    static_cast<uint8_t>(0),
//                                    static_cast<uint8_t>(1))) {
//                    continue;
//                }
//
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
//                ++count_distance_computation_;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
////                if (dist >= set_L[L-1].distance_) {
////                    continue;
////                }
//                Candidate cand(nb_id, dist, false);
//                idi r;
////#pragma omp critical
//                {
//                    r = insert_into_queue(set_L, L, cand);
//                    if (r < nk) {
//                        nk = r;
//                    }
//                }
//            }
//        }
//        top_m_candidates_end = 0; // Clear top_m_candidates
//
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
//    }
//
////#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
//}
//
//inline void Searching::para_search_with_top_m_critical_area_yes_omp(
//        const PANNS::idi M,
//        const PANNS::idi query_id,
//        const PANNS::idi K,
//        const PANNS::idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K)
////        std::vector< std::vector<idi> > &top_m_list)
//{
//    std::vector<uint8_t> is_visited(num_v_, 0);
////    boost::dynamic_bitset<> is_visited(num_v_);
//
//    {
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = 1;
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
////#pragma omp parallel for
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
//        ++count_distance_computation_;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//
//    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    while (k < L) {
//        ++tmp_count;
//
//        unsigned nk = L;
////        int nk = L;
//
//        // Select M candidates
//        idi last_k = L;
//        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//            if (set_L[c_i].is_checked_) {
//                continue;
//            }
//            last_k = c_i; // Record the location of the last candidate selected.
//            set_L[c_i].is_checked_ = true;
//            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
//        }
//
//        // Push M candidates' neighbors into the queue.
//        // OpenMP reduction(min : nk) has a problem if nk is unsigned. nk might end up with being MAX_UINT.
////#pragma omp parallel for
////#pragma omp parallel for reduction(min : nk)
//        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//            idi cand_id = top_m_candidates[c_i];
//            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
////                if (is_visited[nb_id]) {
////                    continue;
////                }
////                is_visited[nb_id] = 1;
//
//                if (!AtomicOps::CAS(is_visited.data() + nb_id,
//                                    static_cast<uint8_t>(0),
//                                    static_cast<uint8_t>(1))) {
//                    continue;
//                }
//
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
//                ++count_distance_computation_;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
////                if (dist >= set_L[L-1].distance_) {
////                    continue;
////                }
//                Candidate cand(nb_id, dist, false);
//                idi r;
////#pragma omp critical
//                {
//                    r = insert_into_queue(set_L, L, cand);
//                    if (r < nk) {
//                        nk = r;
//                    }
//                }
//            }
//        }
//        top_m_candidates_end = 0; // Clear top_m_candidates
//
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
//    }
//
////#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
//}
//
//inline void Searching::para_search_with_top_m_visited_array(
//        const PANNS::idi M,
//        const PANNS::idi query_id,
//        const PANNS::idi K,
//        const PANNS::idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K,
//        std::vector<uint8_t> &is_visited)
////        std::vector< std::vector<idi> > &top_m_list)
//{
////    uint64_t count_visited = 0;
//
////    std::vector<uint8_t> is_visited(num_v_, 0);
////    boost::dynamic_bitset<> is_visited(num_v_);
//
//    {
////#pragma omp parallel for
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = 1;
////            ++count_visited;
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
////#pragma omp parallel for
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
//        ++count_distance_computation_;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//
//    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    while (k < L) {
//        ++tmp_count;
//
//        unsigned nk = L;
////        int nk = L;
//
//        // Select M candidates
//        idi last_k = L;
//        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//            if (set_L[c_i].is_checked_) {
//                continue;
//            }
//            last_k = c_i; // Record the location of the last candidate selected.
//            set_L[c_i].is_checked_ = true;
//            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
//        }
//
//        // Push M candidates' neighbors into the queue.
//        // OpenMP reduction(min : nk) has a problem if nk is unsigned. nk might end up with being MAX_UINT.
////#pragma omp parallel for
////#pragma omp parallel for reduction(min : nk)
//        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//            idi cand_id = top_m_candidates[c_i];
//            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
////                if (is_visited[nb_id]) {
////                    continue;
////                }
////                is_visited[nb_id] = 1;
//
//                if (!AtomicOps::CAS(is_visited.data() + nb_id,
//                                    static_cast<uint8_t>(0),
//                                    static_cast<uint8_t>(1))) {
//                    continue;
//                }
////                ++count_visited;
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
//                ++count_distance_computation_;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
////                if (dist >= set_L[L-1].distance_) {
////                    continue;
////                }
//                Candidate cand(nb_id, dist, false);
//                idi r;
////#pragma omp critical
//                {
//                    r = insert_into_queue(set_L, L, cand);
//                    if (r < nk) {
//                        nk = r;
//                    }
//                }
//            }
//        }
//        top_m_candidates_end = 0; // Clear top_m_candidates
//
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
//    }
//
////#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
//
////    {
////        printf("query_id: %u "
////               "count_visited: %lu %f%%\n",
////               query_id,
////               count_visited,
////               100.0 * count_visited / num_v_);
////    }
//}
//
//inline void Searching::para_search_with_top_m_merge_queues(
//        const idi M,
//        const idi query_id,
//        const idi K,
//        const idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K)
//{
////    {//test
////        printf("query_id: %u\n", query_id);
////    }
////    const idi local_queue_length = ((M - 1) / num_threads_ + 1) * width_;
//    const idi local_queue_length = L;
//    std::vector< std::vector<Candidate> > local_queues_list(num_threads_, std::vector<Candidate>(local_queue_length));
//    std::vector<idi> local_queues_ends(num_threads_, 0);
//    std::vector<uint8_t> is_visited(num_v_, 0);
////    boost::dynamic_bitset<> is_visited(num_v_);
//
//    {
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = 1;
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//#pragma omp parallel for
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
//#pragma omp parallel for
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
////        ++count_distance_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//
//    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    while (k < L) {
//        ++tmp_count;
////        {//test
////            printf("tmp_count: %d\n", tmp_count);
////        }
//
//        // Select M candidates
//        idi last_k = L;
//// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
//        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//            if (set_L[c_i].is_checked_) {
//                continue;
//            }
//            last_k = c_i; // Record the location of the last candidate selected.
//            set_L[c_i].is_checked_ = true;
//            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
//        }
//
//        // Push M candidates' neighbors into the queue.
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//            int tid = omp_get_thread_num();
//            idi cand_id = top_m_candidates[c_i];
//            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
////                if (is_visited[nb_id]) {
////                    continue;
////                }
////                is_visited[nb_id] = 1;
//
//                if (!AtomicOps::CAS(is_visited.data() + nb_id,
//                                    static_cast<uint8_t>(0),
//                                    static_cast<uint8_t>(1))) {
//                    continue;
//                }
//
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
////                ++count_distance_computation;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
////                if (dist >= set_L[L-1].distance_) {
////                    continue;
////                }
//                Candidate cand(nb_id, dist, false);
//                // Add to the local queue.
//                add_into_queue(local_queues_list[tid], local_queues_ends[tid], local_queue_length, cand);
//            }
//        }
//        top_m_candidates_end = 0; // Clear top_m_candidates
//
//        idi nk = L;
////        // Merge. Parallel merging in every two queues.
////        {
////            for (int tid = 0; tid < num_threads_; ++tid) {
////                if (0 == local_queues_ends[tid]) continue;
////                idi r = merge_two_queues_into_1st_queue_para(
////                        set_L,
////                        0,
////                        L,
////                        local_queues_list[tid],
////                        0,
////                        local_queues_ends[tid]);
//////                idi r = merge_two_queues_into_1st_queue_seq(
//////                        set_L,
//////                        0,
//////                        L,
//////                        local_queues_list[tid],
//////                        0,
//////                        local_queues_ends[tid]);
////                local_queues_ends[tid] = 0; // Reset the local queue
////                if (r < nk) {
////                    nk = r;
////                }
////            }
////        }
////        {// text
////            if (query_id == 4 &&
////                    tmp_count == 5) {
////                // Print local queues
////                for (int t_i = 0; t_i < num_threads_; ++t_i) {
//////                    idi start_i = t_i * local_queue_length;
////                    for (idi q_i = 0; q_i < local_queues_ends[t_i]; ++q_i) {
////                        printf("t[%u][%u]: "
////                               "id: %u "
////                               "dist: %f\n",
////                               t_i, q_i,
////                               local_queues_list[t_i][q_i].id_,
////                               local_queues_list[t_i][q_i].distance_);
////                    }
////                }
////                printf("----------\n");
////                for (idi i = 0; i < L; ++i) {
////                    printf("set_L[%u]: "
////                           "id: %u "
////                           "dist: %f\n",
////                           i,
////                           set_L[i].id_,
////                           set_L[i].distance_);
////                }
////                printf("----------\n");
////            }
////        }
//        // Merge. Merge all queues in parallel.
//        {
//            if (num_threads_ > 1) {
//                idi r = merge_all_queues_para_list(
//                        local_queues_list,
//                        local_queues_ends,
//                        set_L,
//                        L);
//                if (r < nk) {
//                    nk = r;
//                }
//            } else {
//                if (local_queues_ends[0]) {
//                    idi r = merge_two_queues_into_1st_queue_seq_fixed(
//                            set_L,
//                            0,
//                            L,
//                            local_queues_list[0],
//                            0,
//                            local_queues_ends[0]);
//                    local_queues_ends[0] = 0;
//                    if (r < nk) {
//                        nk = r;
//                    }
//                }
//            }
//        }
////        {//test
////            if (query_id == 4) {
////                for (idi i = 0; i < L; ++i) {
////                    printf("tmp_count: %u "
////                           "set_L[%u]: "
////                           "id: %u "
////                           "dist: %f\n",
////                           tmp_count,
////                           i,
////                           set_L[i].id_,
////                           set_L[i].distance_);
////                }
////            }
////
////        }
//
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
//    }
//
//#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
////    {
////        exit(1);
////    }
////    {//test
////
//////        if (query_id == 4) {
////            for (idi i = 0; i < L; ++i) {
////                printf("set_L[%u]: "
////                       "id: %u "
////                       "dist: %f\n",
////                       i,
////                       set_L[i].id_,
////                       set_L[i].distance_);
////            }
//////            exit(1);
//////        }
////    }
//}
//
////// Using local queue and then sequential merge.
//inline void Searching::para_search_with_top_m_queues_seq_merge(
//        const PANNS::idi M,
//        const PANNS::idi query_id,
//        const PANNS::idi K,
//        const PANNS::idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K)
////        std::vector< std::vector<idi> > &top_m_list)
//{
////    const idi local_queue_length = ((L - 1) / num_threads_ + 1) * width_;
//    const idi local_queue_length = L;
//    std::vector< std::vector<Candidate> > local_queues_list(num_threads_, std::vector<Candidate>(local_queue_length));
//    std::vector<idi> local_queues_ends(num_threads_, 0);
//    std::vector<uint8_t> is_visited(num_v_, 0);
////    boost::dynamic_bitset<> is_visited(num_v_);
//
//    {
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = 1;
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
////    for (idi v_i = 0; v_i < L; ++v_i) {
////        idi v_id = init_ids[v_i];
////        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
////    }
//    // Get the distances of all candidates, store in the set set_L.
//#pragma omp parallel for
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
////        ++count_distance_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//
//    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    while (k < L) {
//        ++tmp_count;
////        {
////            printf("tmp_count: %u "
////                   "k: %u\n",
////                   tmp_count,
////                   k);
////        }
//
////        unsigned nk = L;
////        int nk = L;
//
//        // Select M candidates
//        idi last_k = L;
//        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//            if (set_L[c_i].is_checked_) {
//                continue;
//            }
//            last_k = c_i; // Record the location of the last candidate selected.
//            set_L[c_i].is_checked_ = true;
//            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
//        }
//
//        // Push M candidates' neighbors into the queue.
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//            int tid = omp_get_thread_num();
//            idi cand_id = top_m_candidates[c_i];
//            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
////                if (is_visited[nb_id]) {
////                    continue;
////                }
////                is_visited[nb_id] = 1;
//
//                if (!AtomicOps::CAS(is_visited.data() + nb_id,
//                                    static_cast<uint8_t>(0),
//                                    static_cast<uint8_t>(1))) {
//                    continue;
//                }
//
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
////                ++count_distance_computation;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
////                if (dist >= set_L[L-1].distance_) {
////                    continue;
////                }
//                Candidate cand(nb_id, dist, false);
////                idi r;
////#pragma omp critical
////                {
////                    r = insert_into_queue(set_L, L, cand);
////                    if (r < nk) {
////                        nk = r;
////                    }
////                }
//                // Add to the local queue.
//                add_into_queue(local_queues_list[tid], local_queues_ends[tid], local_queue_length, cand);
//            }
//        }
//        top_m_candidates_end = 0; // Clear top_m_candidates
//
//        idi nk = L;
//        // Merge
//        {
//            for (int tid = 0; tid < num_threads_; ++tid) {
//                if (0 == local_queues_ends[tid]) continue;
//                idi r = merge_two_queues_into_1st_queue_seq_fixed(
//                        set_L,
//                        0,
//                        L,
//                        local_queues_list[tid],
//                        0,
//                        local_queues_ends[tid]);
////                        L + 1);
//                local_queues_ends[tid] = 0; // Reset the local queue
//                if (r < nk) {
//                    nk = r;
//                }
//            }
//        }
//
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
//    }
//
//#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
////
////    {//test
////        for (idi k_i = 0; k_i < K; ++k_i) {
////            printf("%u: %u: %u %f\n",
////                    query_id,
////                    k_i, set_L[k_i].id_, set_L[k_i].distance_);
////        }
////        exit(1);
////    }
//}
//
//inline void Searching::para_search_with_top_m_merge_queues_no_CAS(
//        const idi M,
//        const idi query_id,
//        const idi K,
//        const idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K,
//        const idi local_queue_length,
//        std::vector< std::vector<Candidate> > &local_queues_list,
//        std::vector<idi> &local_queues_ends,
////        std::vector<uint8_t> &is_visited)
//        boost::dynamic_bitset<> &is_visited)
//{
//////    const idi local_queue_length = ((M - 1) / num_threads_ + 1) * width_;
////    const idi local_queue_length = L;
////    std::vector< std::vector<Candidate> > local_queues_list(num_threads_, std::vector<Candidate>(local_queue_length));
////    std::vector<idi> local_queues_ends(num_threads_, 0);
//////    std::vector<uint8_t> is_visited(num_v_, 0);
////    boost::dynamic_bitset<> is_visited(num_v_);
//
//    {
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = 1;
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//#pragma omp parallel for
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
//#pragma omp parallel for
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
////        ++count_distance_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//
//    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    while (k < L) {
//        ++tmp_count;
//
//        // Select M candidates
//        idi last_k = L;
//// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
//        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//            if (set_L[c_i].is_checked_) {
//                continue;
//            }
//            last_k = c_i; // Record the location of the last candidate selected.
//            set_L[c_i].is_checked_ = true;
//            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
//        }
//
//        // Push M candidates' neighbors into the queue.
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//            int tid = omp_get_thread_num();
//            idi cand_id = top_m_candidates[c_i];
//            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
//                if (is_visited[nb_id]) {
//                    continue;
//                }
//                is_visited[nb_id] = 1;
//
////                if (!AtomicOps::CAS(is_visited.data() + nb_id,
////                                    static_cast<uint8_t>(0),
////                                    static_cast<uint8_t>(1))) {
////                    continue;
////                }
//
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
////                ++count_distance_computation;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
////                if (dist >= set_L[L-1].distance_) {
////                    continue;
////                }
//                Candidate cand(nb_id, dist, false);
//                // Add to the local queue.
//                add_into_queue(local_queues_list[tid], local_queues_ends[tid], local_queue_length, cand);
//            }
//        }
//        top_m_candidates_end = 0; // Clear top_m_candidates
//
//        idi nk = L;
////        // Merge. Parallel merging in every two queues.
////        {
////            for (int tid = 0; tid < num_threads_; ++tid) {
////                if (0 == local_queues_ends[tid]) continue;
////                idi r = merge_two_queues_into_1st_queue_para(
////                        set_L,
////                        0,
////                        L,
////                        local_queues_list[tid],
////                        0,
////                        local_queues_ends[tid]);
//////                idi r = merge_two_queues_into_1st_queue_seq(
//////                        set_L,
//////                        0,
//////                        L,
//////                        local_queues_list[tid],
//////                        0,
//////                        local_queues_ends[tid]);
////                local_queues_ends[tid] = 0; // Reset the local queue
////                if (r < nk) {
////                    nk = r;
////                }
////            }
////        }
////        // Merge. Merge all queues in parallel.
////        {
////            if (num_threads_ > 1) {
////                idi r = merge_all_queues_para(
////                        local_queues_list,
////                        local_queues_ends,
////                        set_L,
////                        L);
////                if (r < nk) {
////                    nk = r;
////                }
////            } else {
////                if (local_queues_ends[0]) {
////                    idi r = merge_two_queues_into_1st_queue_seq(
////                            set_L,
////                            0,
////                            L,
////                            local_queues_list[0],
////                            0,
////                            local_queues_ends[0]);
////                    local_queues_ends[0] = 0;
////                    if (r < nk) {
////                        nk = r;
////                    }
////                }
////            }
////        }
//        // Merge
//        {
//            for (int tid = 0; tid < num_threads_; ++tid) {
//                if (0 == local_queues_ends[tid]) continue;
//                idi r = merge_two_queues_into_1st_queue_seq_fixed(
//                        set_L,
//                        0,
//                        L,
//                        local_queues_list[tid],
//                        0,
//                        local_queues_ends[tid]);
////                        L + 1);
//                local_queues_ends[tid] = 0; // Reset the local queue
//                if (r < nk) {
//                    nk = r;
//                }
//            }
//        }
//
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
//    }
//
//#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
//
//    {// Reset
//        is_visited.reset();
////        std::fill(is_visited.begin(), is_visited.end(), 0);
//        std::fill(local_queues_ends.begin(), local_queues_ends.end(), 0);
//    }
//}

//inline void Searching::para_search_with_top_m_merge_queues_in_array(
//inline void Searching::para_search_with_top_m_merge_queues_new_threshold(
//        const idi M,
//        const idi query_id,
//        const idi K,
//        const idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K,
//        const idi local_queue_length, // Maximum size of local queue
////        std::vector< std::vector<Candidate> > &local_queues_list,
//        std::vector<Candidate> &local_queues_array,
//        std::vector<idi> &local_queues_ends, // Sizes of local queue
//        BitVector &is_visited)
////        std::vector<uint8_t> &is_visited)
////        boost::dynamic_bitset<> &is_visited)
//{
//    {
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < L; ++c_i) {
////            is_visited[init_ids[c_i]] = 1;
//            is_visited.atomic_set_bit(init_ids[c_i]);
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//#pragma omp parallel for
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set set_L.
//#pragma omp parallel for
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
////        ++count_distance_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//
//    idi min_index = L - 1;
//    distf min_1st = set_L[min_index].distance_;
//
//    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    while (k < L) {
//        ++tmp_count;
////        {//test
////            printf("tmp_count: %d\n", tmp_count);
////        }
//
//        // Select M candidates
//        idi last_k = L;
//// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
//        for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//            if (set_L[c_i].is_checked_) {
//                continue;
//            }
//            last_k = c_i; // Record the location of the last candidate selected.
//            set_L[c_i].is_checked_ = true;
//            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
//        }
//
//        // Push M candidates' neighbors into the queue.
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//            int tid = omp_get_thread_num();
//            const idi local_queue_start = tid * local_queue_length;
//            idi cand_id = top_m_candidates[c_i];
//            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
////                { // Sequential edition
////                    if (is_visited[nb_id]) {
////                        continue;
////                    }
////                    is_visited[nb_id] = 1;
////                }
////                { // __ATOMIC_SEQ_CST edition
////                    if (!AtomicOps::CAS(is_visited.data() + nb_id,
////                                        static_cast<uint8_t>(0),
////                                        static_cast<uint8_t>(1))) {
////                        continue;
////                    }
////                }
////                {// Acquire and Release edition
////                    if (__atomic_load_n(is_visited.data() + nb_id, __ATOMIC_ACQUIRE)) {
////                        continue;
////                    }
////                    __atomic_store_n(is_visited.data() + nb_id, 1, __ATOMIC_RELEASE);
////                }
//                {// Self-defined BitVector
//                    if (is_visited.atomic_is_bit_set(nb_id)) {
//                        continue;
//                    }
//                    is_visited.atomic_set_bit(nb_id);
//                }
//
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
////                ++count_distance_computation;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//
//                if (dist > min_1st) {
//                    continue;
//                } else if (min_index > 0) {
//                    // Inserted, so min_1st needs update
//                    if (dist > set_L[min_index - 1].distance_) {
//                        min_1st = dist;
//                        if (min_index < L - 1) {
//                            ++min_index;
//                        }
//                    } else {
//                        min_1st = set_L[--min_index].distance_;
//                    }
////                    min_1st = set_L[--min_index].distance_;
//                }
//
////                if (dist > set_L[L-1].distance_) {
////                    continue;
////                }
//
//                Candidate cand(nb_id, dist, false);
//                // Add to the local queue.
//                add_into_queue(local_queues_array, local_queue_start, local_queues_ends[tid], local_queue_length, cand);
//            }
//        }
//        top_m_candidates_end = 0; // Clear top_m_candidates
//
//        idi nk = L;
////        // Merge. Parallel merging in every two queues.
////        {
////            for (int tid = 0; tid < num_threads_; ++tid) {
////                if (0 == local_queues_ends[tid]) continue;
////                idi r = merge_two_queues_into_1st_queue_para(
////                        set_L,
////                        0,
////                        L,
////                        local_queues_list[tid],
////                        0,
////                        local_queues_ends[tid]);
//////                idi r = merge_two_queues_into_1st_queue_seq(
//////                        set_L,
//////                        0,
//////                        L,
//////                        local_queues_list[tid],
//////                        0,
//////                        local_queues_ends[tid]);
////                local_queues_ends[tid] = 0; // Reset the local queue
////                if (r < nk) {
////                    nk = r;
////                }
////            }
////        }
//        // Merge. Merge all queues in parallel.
//        {
//            if (num_threads_ > 1) {
//                idi r = merge_all_queues_para_array(
////                        local_queues_list,
//                        local_queues_array,
//                        local_queues_ends,
//                        local_queue_length,
//                        set_L,
//                        L);
//                if (r < nk) {
//                    nk = r;
//                }
//            } else {
//                if (local_queues_ends[0]) {
//                    idi r = merge_two_queues_into_1st_queue_seq_fixed(
//                            set_L,
//                            0,
//                            L,
////                            local_queues_list[0],
//                            local_queues_array,
//                            0,
//                            local_queues_ends[0]);
//                    local_queues_ends[0] = 0;
//                    if (r < nk) {
//                        nk = r;
//                    }
//                }
//            }
//        }
////        // Merge Sequentially
////        {
////            for (int tid = 0; tid < num_threads_; ++tid) {
////                if (0 == local_queues_ends[tid]) continue;
////                idi r = merge_two_queues_into_1st_queue_seq_fixed(
////                        set_L,
////                        0,
////                        L,
//////                        local_queues_list[tid],
//////                        0,
////                        local_queues_array,
////                        tid * local_queue_length,
////                        local_queues_ends[tid]);
//////                        L + 1);
////                local_queues_ends[tid] = 0; // Reset the local queue
////                if (r < nk) {
////                    nk = r;
////                }
////            }
////        }
//
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
//    }
//
//#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
//
//    {// Reset
////        is_visited.reset();
////        std::fill(is_visited.begin(), is_visited.end(), 0);
//        is_visited.clear_all();
//        std::fill(local_queues_ends.begin(), local_queues_ends.end(), 0);
//    }
//}


/*
 * 5/7/2020-15:14
 * Use 1 threads to scale M until the value_M_middle.
 * Then use multiple threads.
 */
inline void Searching::para_search_with_top_m_merge_queues_middle_m(
        const idi value_M_middle,
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
        std::vector<idi> &top_m_candidates,
        boost::dynamic_bitset<> &is_visited)
{
//    const idi base_set_L = (num_threads_ - 1) * local_queue_length;
    {
#pragma omp parallel for
        for (idi c_i = 0; c_i < L; ++c_i) {
            is_visited[init_ids[c_i]] = 1;
        }
    }

    const dataf *query_data = queries_load_ + query_id  * dimension_;
#pragma omp parallel for
    for (idi v_i = 0; v_i < L; ++v_i) {
        idi v_id = init_ids[v_i];
        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
    }
    uint64_t tmp_count_computation = 0;
    // Get the distances of all candidates, store in the set set_L.
//#pragma omp parallel for
#pragma omp parallel for reduction(+ : tmp_count_computation)
    for (unsigned i = 0; i < L; i++) {
        unsigned v_id = init_ids[i];
        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
        dataf norm = *v_data++;
        ++tmp_count_computation;
        distf dist = compute_distance_with_norm(v_data, query_data, norm);
        set_L[i + base_set_L] = Candidate(v_id, dist, false); // False means not checked.
    }
    count_distance_computation_ += tmp_count_computation;
    tmp_count_computation = 0;
//    std::sort(set_L.begin(), set_L.begin() + L);
    std::sort(
            set_L.begin() + base_set_L,
            set_L.begin() + base_set_L + L);
    local_queues_ends[num_threads_ - 1] = L;

    idi top_m_candidates_end = 0;
    idi k = 0; // Index of first unchecked candidate.
    idi tmp_count = 0; // for debug
    idi M = 1;

    { // Single thread
        while (k < L && M < value_M_middle) {
            ++tmp_count;
//        {//test
//            printf("tmp_count: %d\n", tmp_count);
//        }

            // Select M candidates
            idi last_k = L;
// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
            for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
                idi index_set_L = c_i + base_set_L;
                if (set_L[index_set_L].is_checked_) {
                    continue;
                }
                last_k = c_i; // Record the location of the last candidate selected.
                set_L[index_set_L].is_checked_ = true;
                top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
            }

            idi nk = L;
            // Push M candidates' neighbors into the queue.
            for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
                idi cand_id = top_m_candidates[c_i];
                _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
                idi out_degree = *out_edges++;
                for (idi n_i = 0; n_i < out_degree; ++n_i) {
                    _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
                }
                for (idi e_i = 0; e_i < out_degree; ++e_i) {
                    idi nb_id = out_edges[e_i];
                    { // Sequential edition
                        if (is_visited[nb_id]) {
                            continue;
                        }
                        is_visited[nb_id] = 1;
                    }

                    auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
                    dataf norm = *nb_data++;
                    ++tmp_count_computation;
                    distf dist = compute_distance_with_norm(nb_data, query_data, norm);
                    if (dist > set_L[L - 1 + base_set_L].distance_) {
                        continue;
                    }

                    Candidate cand(nb_id, dist, false);
                    // Thread 0 maintains the "global" queue
                    idi r = add_into_queue(
                            set_L,
                            base_set_L,
                            local_queues_ends[num_threads_ - 1],
                            L,
                            cand);
                    if (r < nk) {
                        nk = r;
                    }
                }
            }
            top_m_candidates_end = 0; // Clear top_m_candidates
            count_distance_computation_ += tmp_count_computation;
            tmp_count_computation = 0;

            if (nk <= last_k) {
                k = nk;
            } else {
                k = last_k + 1;
            }

            {// Scale M
                if (M < value_M_max) {
                    M <<= 1;
                } else {
                    M = value_M_max;
                }
            }

        }
    }

    { // Multiple Threads
        while (k < L) {
            ++tmp_count;
//        {//test
//            printf("tmp_count: %d\n", tmp_count);
//        }
            // Select M candidates
            idi last_k = L;
// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
            for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
                idi index_set_L = c_i + base_set_L;
                if (set_L[index_set_L].is_checked_) {
                    continue;
                }
                last_k = c_i; // Record the location of the last candidate selected.
                set_L[index_set_L].is_checked_ = true;
                top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
            }


            idi nk = L;
            // Push M candidates' neighbors into the queue.
//#pragma omp parallel for reduction(+ : tmp_count_computation) num_threads(real_threads)
#pragma omp parallel for reduction(+ : tmp_count_computation)
            for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
                int tid = omp_get_thread_num();
                idi cand_id = top_m_candidates[c_i];
                _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
                idi out_degree = *out_edges++;
                for (idi n_i = 0; n_i < out_degree; ++n_i) {
                    _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
                }
                for (idi e_i = 0; e_i < out_degree; ++e_i) {
                    idi nb_id = out_edges[e_i];
                    { // Sequential edition
                        if (is_visited[nb_id]) {
                            continue;
                        }
                        is_visited[nb_id] = 1;
                    }

                    auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
                    dataf norm = *nb_data++;
                    ++tmp_count_computation;
                    distf dist = compute_distance_with_norm(nb_data, query_data, norm);
                    if (dist > set_L[L - 1 + base_set_L].distance_) {
                        continue;
                    }

                    Candidate cand(nb_id, dist, false);
                    // Add to the local queue.
                    if (0 != tid) {
                        // Non-Master threads using local queues
                        add_into_queue(
                                set_L,
                                (tid - 1) * local_queue_length,
                                local_queues_ends[tid - 1],
                                local_queue_length,
                                cand);
                    } else {
                        // Thread 0 maintains the "global" queue
                        idi r = add_into_queue(
                                set_L,
                                base_set_L,
                                local_queues_ends[num_threads_ - 1],
                                L,
                                cand);
                        if (r < nk) {
                            nk = r;
                        }
                    }
                }
            }
            top_m_candidates_end = 0; // Clear top_m_candidates
            count_distance_computation_ += tmp_count_computation;
            tmp_count_computation = 0;

//        {// Local queues' ends
//            printf("query%u:iter: %u", query_id, tmp_count);
//            for (int i_t = 0; i_t < num_threads_; ++i_t) {
//                printf(" [%u]: %u", i_t, local_queues_ends[i_t]);
//            }
//            printf("\n");
//        }

//        // Merge. Merge all queues in parallel.
            {
                time_merge_ -= WallTimer::get_time_mark();
                if (num_threads_ > 1) {
                    idi r = merge_all_queues_para_array(
                            set_L,
                            local_queues_ends,
                            local_queue_length,
                            L);
                    if (r < nk) {
                        nk = r;
                    }
                }
                time_merge_ += WallTimer::get_time_mark();
            }
            if (nk <= last_k) {
                k = nk;
            } else {
                k = last_k + 1;
            }
            {// Scale M
                if (M < value_M_max) {
                    M <<= 1;
                } else {
                    M = value_M_max;
                }
            }

        }
    }


#pragma omp parallel for
    for (idi k_i = 0; k_i < K; ++k_i) {
        set_K[k_i] = set_L[k_i + base_set_L].id_;
//        set_K[k_i] = set_L[k_i].id_;
    }

    {// Reset
//        std::fill(is_visited.begin(), is_visited.end(), 0);
        is_visited.reset();
//        is_visited.clear_all();
        std::fill(local_queues_ends.begin(), local_queues_ends.end(), 0);
    }

//    {//test
//        if (3 == query_id) {
//            exit(1);
//        }
//    }
}

inline void Searching::para_search_with_top_m_merge_queues_middle_m_no_merge(
        const uint64_t computation_threshold,
        const idi value_M_middle,
        const idi value_M_max,
        const idi query_id,
        const idi K,
        const idi L,
        const idi init_size,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K,
        const idi local_queue_length, // Maximum size of local queue
        const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
        std::vector<idi> &local_queues_ends, // Sizes of local queue
        std::vector<idi> &top_m_candidates,
        boost::dynamic_bitset<> &is_visited)
{
    uint64_t count_single_query_computation = 0;
    uint64_t count_init_computation = 0;
    uint64_t count_seq_computation = 0;
    uint64_t count_par_computation = 0;
//    {//test
//        printf("query_id: %u\n", query_id);
//    }
//    time_initialization_ -= WallTimer::get_time_mark();
//    const idi base_set_L = (num_threads_ - 1) * local_queue_length;
    {
#pragma omp parallel for
        for (idi c_i = 0; c_i < init_size; ++c_i) {
//        for (idi c_i = 0; c_i < L; ++c_i) {
            is_visited[init_ids[c_i]] = 1;
//            is_visited.atomic_set_bit(init_ids[c_i]);
        }
    }

    const dataf *query_data = queries_load_ + query_id  * dimension_;
#pragma omp parallel for
    for (idi v_i = 0; v_i < init_size; ++v_i) {
//    for (idi v_i = 0; v_i < L; ++v_i) {
        idi v_id = init_ids[v_i];
        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
    }
    uint64_t tmp_count_computation = 0;
    // Get the distances of all candidates, store in the set set_L.
//#pragma omp parallel for
#pragma omp parallel for reduction(+ : tmp_count_computation)
    for (unsigned i = 0; i < init_size; i++) {
//    for (unsigned i = 0; i < L; i++) {
        unsigned v_id = init_ids[i];
        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
        dataf norm = *v_data++;
//        ++count_distance_computation_;
        ++tmp_count_computation;
        distf dist = compute_distance_with_norm(v_data, query_data, norm);
        set_L[i + base_set_L] = Candidate(v_id, dist, false); // False means not checked.
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
    }
    count_distance_computation_ += tmp_count_computation;
    count_init_computation += tmp_count_computation;
    count_single_query_computation += tmp_count_computation;
    tmp_count_computation = 0;
//    std::sort(set_L.begin(), set_L.begin() + L);
    std::sort(
            set_L.begin() + base_set_L,
            set_L.begin() + base_set_L + init_size);
//            set_L.begin() + base_set_L + L);
    local_queues_ends[num_threads_ - 1] = init_size;
//    local_queues_ends[num_threads_ - 1] = L;

//    time_initialization_ += WallTimer::get_time_mark();
//    time_sequential_phase_ -= WallTimer::get_time_mark();

//    std::vector<idi> top_m_candidates(M);
    idi &global_queue_size = local_queues_ends[num_threads_ - 1];
    idi top_m_candidates_end = 0;
    idi k = 0; // Index of first unchecked candidate.
    idi tmp_count = 0; // for debug
    idi M = 1;
    { // Single thread
        while (k < L && M < value_M_middle && count_single_query_computation <= computation_threshold) {
            ++tmp_count;
//            {//test
//                printf("tmp_count: %d\n", tmp_count);
//            }

//            int real_threads = std::min(static_cast<int>(M), num_threads_);
//            idi queue_base = num_threads_ - real_threads;
            // Select M candidates

            idi last_k = L;
// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
            for (idi c_i = k; c_i < global_queue_size && top_m_candidates_end < M; ++c_i) {
//            for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
                idi index_set_L = c_i + base_set_L;
                if (set_L[index_set_L].is_checked_) {
                    continue;
                }
                last_k = c_i; // Record the location of the last candidate selected.
                set_L[index_set_L].is_checked_ = true;
                top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
            }

            idi nk = L;
            // Push M candidates' neighbors into the queue.
            for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
                idi cand_id = top_m_candidates[c_i];
                _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
                idi out_degree = *out_edges++;
                for (idi n_i = 0; n_i < out_degree; ++n_i) {
                    _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
                }
                for (idi e_i = 0; e_i < out_degree; ++e_i) {
                    idi nb_id = out_edges[e_i];
                    { // Sequential edition
                        if (is_visited[nb_id]) {
                            continue;
                        }
                        is_visited[nb_id] = 1;
                    }

                    auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
                    dataf norm = *nb_data++;
//                ++count_distance_computation_;
                    ++tmp_count_computation;
                    distf dist = compute_distance_with_norm(nb_data, query_data, norm);
                    if (dist > set_L[global_queue_size - 1 + base_set_L].distance_) {
//                    if (dist > set_L[L - 1 + base_set_L].distance_) {
                        continue;
                    }

                    Candidate cand(nb_id, dist, false);
                    // Thread 0 maintains the "global" queue
                    idi r = add_into_queue(
                            set_L,
                            base_set_L,
                            global_queue_size,
//                            local_queues_ends[num_threads_ - 1],
                            L,
                            cand);
                    if (r < nk) {
                        nk = r;
                    }
                }
            }
            top_m_candidates_end = 0; // Clear top_m_candidates
            count_distance_computation_ += tmp_count_computation;
            count_seq_computation += tmp_count_computation;
            count_single_query_computation += tmp_count_computation;
            tmp_count_computation = 0;

//        {// Local queues' ends
//            printf("query%u:iter: %u", query_id, tmp_count);
//            for (int i_t = 0; i_t < num_threads_; ++i_t) {
//                printf(" [%u]: %u", i_t, local_queues_ends[i_t]);
//            }
//            printf("\n");
//        }

            if (nk <= last_k) {
                k = nk;
            } else {
                k = last_k + 1;
            }

            {// Scale M
                if (M < value_M_max) {
                    M <<= 1;
                } else {
                    M = value_M_max;
                }
            }
        }
    }
//    time_sequential_phase_ += WallTimer::get_time_mark();

//    time_parallel_phase_ -= WallTimer::get_time_mark();
    { // Multiple Threads
        while (k < L and count_single_query_computation <= computation_threshold) {
//        while (k < L) {
            ++tmp_count;
//            {//test
//                printf("tmp_count: %d "
//                       "k: %u "
//                       "global_queue_size: %u\n",
//                       tmp_count,
//                       k,
//                       global_queue_size);
//            }
//            int real_threads = std::min(static_cast<int>(M), num_threads_);
//            idi queue_base = num_threads_ - real_threads;
            // Select M candidates
            idi last_k = L;
// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
            for (idi c_i = k; c_i < global_queue_size && top_m_candidates_end < M; ++c_i) {
//            for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
                idi index_set_L = c_i + base_set_L;
                if (set_L[index_set_L].is_checked_) {
                    continue;
                }
                last_k = c_i; // Record the location of the last candidate selected.
                set_L[index_set_L].is_checked_ = true;
                top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
            }

            idi nk = L;
            // Push M candidates' neighbors into the queue.
//#pragma omp parallel for reduction(+ : tmp_count_computation) num_threads(real_threads)
#pragma omp parallel for reduction(+ : tmp_count_computation)
            for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
                int tid = omp_get_thread_num();
                idi cand_id = top_m_candidates[c_i];
                _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
                idi out_degree = *out_edges++;
                for (idi n_i = 0; n_i < out_degree; ++n_i) {
                    _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
                }
                for (idi e_i = 0; e_i < out_degree; ++e_i) {
                    idi nb_id = out_edges[e_i];
                    { // Sequential edition
                        if (is_visited[nb_id]) {
                            continue;
                        }
                        is_visited[nb_id] = 1;
                    }

                    auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
                    dataf norm = *nb_data++;
//                ++count_distance_computation_;
                    ++tmp_count_computation;
                    distf dist = compute_distance_with_norm(nb_data, query_data, norm);
                    if (dist > set_L[global_queue_size - 1 + base_set_L].distance_) {
//                    if (dist > set_L[L - 1 + base_set_L].distance_) {
                        continue;
                    }

                    Candidate cand(nb_id, dist, false);
                    // Add to the local queue.
                    if (0 != tid) {
                        // Non-Master threads using local queues
                        add_into_queue(
                                set_L,
                                (tid - 1) * local_queue_length,
                                local_queues_ends[tid - 1],
                                local_queue_length,
                                cand);
                    } else {
                        // Thread 0 maintains the "global" queue
                        idi r = add_into_queue(
                                set_L,
                                base_set_L,
                                global_queue_size,
//                                local_queues_ends[num_threads_ - 1],
                                L,
                                cand);
                        if (r < nk) {
                            nk = r;
                        }
                    }
                }
            }
            top_m_candidates_end = 0; // Clear top_m_candidates
            count_distance_computation_ += tmp_count_computation;
            count_par_computation += tmp_count_computation;
            count_single_query_computation += tmp_count_computation;
            tmp_count_computation = 0;

//        {// Local queues' ends
//            printf("query%u:iter: %u", query_id, tmp_count);
//            for (int i_t = 0; i_t < num_threads_; ++i_t) {
//                printf(" [%u]: %u", i_t, local_queues_ends[i_t]);
//            }
//            printf("\n");
//        }

            // Merge. Merge all queues in parallel.
            {
                if (num_threads_ > 1) {
//                    idi r = merge_all_queues_queue_base(
//                            set_L,
//                            local_queues_ends,
//                            queue_base,
//                            real_threads,
//                            local_queue_length,
//                            L);
                    idi r = merge_all_queues_para_array(
                            set_L,
                            local_queues_ends,
                            local_queue_length,
                            L);
                    if (r < nk) {
                        nk = r;
                    }
                }
            }
            if (nk <= last_k) {
                k = nk;
            } else {
                k = last_k + 1;
            }
            {// Scale M
                if (M < value_M_max) {
                    M <<= 1;
                } else {
                    M = value_M_max;
                }
            }

//            {// Print relative distance
////                distf top_dist = set_L[base_set_L].distance_;
//                for (idi i_l = 0; i_l < L; ++i_l) {
//                    printf("%u %f\n",
//                           tmp_count, set_L[i_l + base_set_L].distance_);
////                           tmp_count, set_L[i_l + base_set_L].distance_ - top_dist);
//                }
//            }
        }
    }
//    time_parallel_phase_ += WallTimer::get_time_mark();

#pragma omp parallel for
    for (idi k_i = 0; k_i < K; ++k_i) {
        set_K[k_i] = set_L[k_i + base_set_L].id_;
//        set_K[k_i] = set_L[k_i].id_;
    }

    {// Reset
//        std::fill(is_visited.begin(), is_visited.end(), 0);
        is_visited.reset();
//        is_visited.clear_all();
        std::fill(local_queues_ends.begin(), local_queues_ends.end(), 0);
    }

//    {//test
//        if (3 == query_id) {
//            exit(1);
//        }
//    }
//    {//test
//        printf("count_single: %lu "
//               "ct_init: %lu "
//               "ct_seq: %lu "
//               "ct_par: %lu\n",
//               count_single_query_computation,
//               count_init_computation,
//               count_seq_computation,
//               count_par_computation);
//    }
}

///*
// * 6/15/2020-14:40
// * Queues merging together to the global queue
// */
//inline void Searching::para_search_with_top_m_merge_queues_sequential_merge(
//        const idi value_M_middle,
//        const idi value_M_max,
//        const idi query_id,
//        const idi K,
//        const idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K,
//        const idi local_queue_length, // Maximum size of local queue
//        const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
//        std::vector<idi> &local_queues_ends, // Sizes of local queue
//        std::vector<idi> &top_m_candidates,
//        boost::dynamic_bitset<> &is_visited)
//{
////    const idi base_set_L = (num_threads_ - 1) * local_queue_length;
//    {
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = 1;
////            is_visited.atomic_set_bit(init_ids[c_i]);
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//#pragma omp parallel for
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    uint64_t tmp_count_computation = 0;
//    // Get the distances of all candidates, store in the set set_L.
////#pragma omp parallel for
//#pragma omp parallel for reduction(+ : tmp_count_computation)
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
////        ++count_distance_computation_;
//        ++tmp_count_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i + base_set_L] = Candidate(v_id, dist, false); // False means not checked.
////        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    count_distance_computation_ += tmp_count_computation;
//    tmp_count_computation = 0;
////    std::sort(set_L.begin(), set_L.begin() + L);
//    std::sort(
//            set_L.begin() + base_set_L,
//            set_L.begin() + base_set_L + L);
//    local_queues_ends[num_threads_ - 1] = L;
//
////    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    idi M = 1;
//
//    { // Single thread
//        while (k < L && M < value_M_middle) {
//            ++tmp_count;
////        {//test
////            printf("tmp_count: %d\n", tmp_count);
////        }
//
//            // Select M candidates
//            idi last_k = L;
//// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
//            for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//                idi index_set_L = c_i + base_set_L;
//                if (set_L[index_set_L].is_checked_) {
//                    continue;
//                }
//                last_k = c_i; // Record the location of the last candidate selected.
//                set_L[index_set_L].is_checked_ = true;
//                top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
//            }
//
//
//            idi nk = L;
//            // Push M candidates' neighbors into the queue.
//            for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//                idi cand_id = top_m_candidates[c_i];
//                _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//                idi out_degree = *out_edges++;
//                for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                    _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//                }
//                for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                    idi nb_id = out_edges[e_i];
//                    { // Sequential edition
//                        if (is_visited[nb_id]) {
//                            continue;
//                        }
//                        is_visited[nb_id] = 1;
//                    }
//
//                    auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                    dataf norm = *nb_data++;
////                ++count_distance_computation_;
//                    ++tmp_count_computation;
//                    distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                    if (dist > set_L[L - 1 + base_set_L].distance_) {
//                        continue;
//                    }
//
//                    Candidate cand(nb_id, dist, false);
//                    // Thread 0 maintains the "global" queue
//                    idi r = add_into_queue(
//                            set_L,
//                            base_set_L,
//                            local_queues_ends[num_threads_ - 1],
//                            L,
//                            cand);
//                    if (r < nk) {
//                        nk = r;
//                    }
//                }
//            }
//            top_m_candidates_end = 0; // Clear top_m_candidates
//            count_distance_computation_ += tmp_count_computation;
//            tmp_count_computation = 0;
//
//            if (nk <= last_k) {
//                k = nk;
//            } else {
//                k = last_k + 1;
//            }
//
//            {// Scale M
//                if (M < value_M_max) {
//                    M <<= 1;
//                } else {
//                    M = value_M_max;
//                }
//            }
//        }
//    }
//
//    { // Multiple Threads
//        while (k < L) {
//            ++tmp_count;
////        {//test
////            if (num_threads_ == 2) {
////                printf("tmp_count: %d "
////                       "k: %u\n",
////                        tmp_count,
////                        k);
////            }
////        }
//            // Select M candidates
//            idi last_k = L;
//// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
//            for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//                idi index_set_L = c_i + base_set_L;
//                if (set_L[index_set_L].is_checked_) {
//                    continue;
//                }
//                last_k = c_i; // Record the location of the last candidate selected.
//                set_L[index_set_L].is_checked_ = true;
//                top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
//            }
//
//
//            idi nk = L;
//            // Push M candidates' neighbors into the queue.
////#pragma omp parallel for reduction(+ : tmp_count_computation) num_threads(real_threads)
//#pragma omp parallel for reduction(+ : tmp_count_computation)
//            for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//                int tid = omp_get_thread_num();
//                idi cand_id = top_m_candidates[c_i];
//                _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//                idi out_degree = *out_edges++;
//                for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                    _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//                }
//                for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                    idi nb_id = out_edges[e_i];
//                    { // Sequential edition
//                        if (is_visited[nb_id]) {
//                            continue;
//                        }
//                        is_visited[nb_id] = 1;
//                    }
//
//                    auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                    dataf norm = *nb_data++;
////                ++count_distance_computation_;
//                    ++tmp_count_computation;
//                    distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                    if (dist > set_L[L - 1 + base_set_L].distance_) {
//                        continue;
//                    }
//
//                    Candidate cand(nb_id, dist, false);
//                    // Add to the local queue.
//                    if (0 != tid) {
//                        // Non-Master threads using local queues
//                        add_into_queue(
//                                set_L,
//                                (tid - 1) * local_queue_length,
//                                local_queues_ends[tid - 1],
//                                local_queue_length,
//                                cand);
//                    } else {
//                        // Thread 0 maintains the "global" queue
//                        idi r = add_into_queue(
//                                set_L,
//                                base_set_L,
//                                local_queues_ends[num_threads_ - 1],
//                                L,
//                                cand);
//                        if (r < nk) {
//                            nk = r;
//                        }
//                    }
//                }
//            }
//            top_m_candidates_end = 0; // Clear top_m_candidates
//            count_distance_computation_ += tmp_count_computation;
//            tmp_count_computation = 0;
//
////        // Merge. Merge all queues in parallel.
//            {
////                {//test
////                    for (idi q_i = 0; q_i < num_threads_; ++q_i) {
////                        if (0 == local_queues_ends[q_i]) {
////                            continue;
////                        }
////                        for (idi e_i = 0; e_i < local_queues_ends[q_i]; ++e_i) {
////                            printf("tmp_count: %u "
////                                   "q_i: %u "
////                                   "[%u]: (%u, %f)\n",
////                                   tmp_count,
////                                   q_i,
////                                   e_i, set_L[q_i * local_queue_length + e_i].id_, set_L[q_i * local_queue_length + e_i].distance_);
////                        }
////                    }
////                }
////                time_merge_ -= WallTimer::get_time_mark();
//                if (num_threads_ > 1) {
//                    idi r = merge_all_queues_all_together_in_sequential(
//                            set_L,
//                            local_queues_ends,
//                            local_queue_length,
//                            L);
////                    idi r = merge_all_queues_para_array(
////                            set_L,
////                            local_queues_ends,
////                            local_queue_length,
////                            L);
//                    if (r < nk) {
//                        nk = r;
//                    }
////                    {//test
////                        printf("tmp_count: %u "
////                               "r: %u "
////                               "last_k: %u\n",
////                               tmp_count,
////                               r,
////                               last_k);
////                        for (idi l_i = 0; l_i < L; ++l_i) {
////                            printf("tmp_count: %u "
////                                   "[%u]: (%u, %f)\n",
////                                   tmp_count,
////                                   l_i, set_L[l_i + base_set_L].id_, set_L[l_i + base_set_L].distance_);
////                        }
////                    }
//                }
//
////                time_merge_ += WallTimer::get_time_mark();
//            }
//            if (nk <= last_k) {
//                k = nk;
//            } else {
//                k = last_k + 1;
//            }
//            {// Scale M
//                if (M < value_M_max) {
//                    M <<= 1;
//                } else {
//                    M = value_M_max;
//                }
//            }
//        }
//    }
//
//#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i + base_set_L].id_;
////        set_K[k_i] = set_L[k_i].id_;
//    }
//
//    {// Reset
////        std::fill(is_visited.begin(), is_visited.end(), 0);
//        is_visited.reset();
////        is_visited.clear_all();
//        std::fill(local_queues_ends.begin(), local_queues_ends.end(), 0);
//    }
//
////    {//test
////        if (0 == query_id) {
////            exit(1);
////        }
////    }
//}

///*
// * 6/19/2020:
// * Intra-query + Inter-query
// */
//inline void Searching::para_search_with_top_m_nested_para(
//        const idi batch_start,
//        const idi batch_size,
//        const idi value_M_middle,
//        const idi value_M_max,
//        const idi K,
//        const idi L,
//        std::vector< std::vector<Candidate> > &set_L_list,
//        const std::vector<idi> &init_ids,
//        std::vector< std::vector<idi> > &set_K_list,
//        const idi local_queue_length, // Maximum size of local queue
//        const idi base_set_L, // base_set_L = (num_threads_intra_query_ - 1) * local_queue_length;
//        std::vector< std::vector<idi> > &local_queues_ends_list, // Sizes of local queue
//        std::vector< std::vector<idi> > &top_m_candidates_list,
//        std::vector< boost::dynamic_bitset<> > &is_visited_list)
//{
//    {// Initialize is_visited flag array
//#pragma omp parallel for num_threads(num_threads_inter_query_)
//        for (idi q_i = 0; q_i < batch_size; ++q_i) {
//            auto &is_visited = is_visited_list[q_i];
//#pragma omp parallel for num_threads(num_threads_intra_query_)
//            for (idi c_i = 0; c_i < L; ++c_i) {
//                is_visited[init_ids[c_i]] = 1;
//            }
//        }
//    }
//
//#pragma omp parallel for
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//
//    uint64_t tmp_count_total_computation = 0;
//#pragma omp parallel for num_threads(num_threads_inter_query_) reduction(+ : tmp_count_total_computation)
//    for (idi q_i = 0; q_i < batch_size; ++q_i) {
//        idi query_id = batch_start + q_i;
//        auto &set_L = set_L_list[q_i];
//        auto &local_queues_ends = local_queues_ends_list[q_i];
//        auto &is_visited = is_visited_list[q_i];
//
//        const dataf *query_data = queries_load_ + query_id * dimension_;
////#pragma omp parallel for
////        for (idi v_i = 0; v_i < L; ++v_i) {
////            idi v_id = init_ids[v_i];
////            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
////        }
//        uint64_t tmp_count_computation = 0;
//        // Get the distances of all candidates, store in the set set_L.
////#pragma omp parallel for
//#pragma omp parallel for reduction(+ : tmp_count_computation) num_threads(num_threads_intra_query_)
//        for (unsigned i = 0; i < L; i++) {
//            unsigned v_id = init_ids[i];
//            auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//            dataf norm = *v_data++;
////        ++count_distance_computation_;
//            ++tmp_count_computation;
//            distf dist = compute_distance_with_norm(v_data, query_data, norm);
//            set_L[i + base_set_L] = Candidate(v_id, dist, false); // False means not checked.
////        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//        }
////        count_distance_computation_ += tmp_count_computation;
//        tmp_count_total_computation += tmp_count_computation;
//        tmp_count_computation = 0;
////    std::sort(set_L.begin(), set_L.begin() + L);
//        std::sort(
//                set_L.begin() + base_set_L,
//                set_L.begin() + base_set_L + L);
//        local_queues_ends[num_threads_intra_query_ - 1] = L;
//
////    std::vector<idi> top_m_candidates(M);
//        idi top_m_candidates_end = 0;
//        idi k = 0; // Index of first unchecked candidate.
//        idi tmp_count = 0; // for debug
//        idi M = 1;
//
//        auto &top_m_candidates = top_m_candidates_list[q_i];
//        { // Single thread
//            while (k < L && M < value_M_middle) {
//                ++tmp_count;
////        {//test
////            printf("tmp_count: %d\n", tmp_count);
////        }
//
//                // Select M candidates
//                idi last_k = L;
//// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
//                for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//                    idi index_set_L = c_i + base_set_L;
//                    if (set_L[index_set_L].is_checked_) {
//                        continue;
//                    }
//                    last_k = c_i; // Record the location of the last candidate selected.
//                    set_L[index_set_L].is_checked_ = true;
//                    top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
//                }
//
//                idi nk = L;
//                // Push M candidates' neighbors into the queue.
//                for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//                    idi cand_id = top_m_candidates[c_i];
//                    _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//                    idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//                    idi out_degree = *out_edges++;
//                    for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                        _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//                    }
//                    for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                        idi nb_id = out_edges[e_i];
//                        { // Sequential edition
//                            if (is_visited[nb_id]) {
//                                continue;
//                            }
//                            is_visited[nb_id] = 1;
//                        }
//
//                        auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                        dataf norm = *nb_data++;
////                ++count_distance_computation_;
//                        ++tmp_count_computation;
//                        distf dist = compute_distance_with_norm(nb_data, query_data, norm);
////                        {//test
////                            if (391655 == nb_id) {
////                                printf("tmp_count: %u "
////                                       "nb_id: %u "
////                                       "distf: %f\n",
////                                       tmp_count,
////                                       nb_id,
////                                       dist);
////                            }
////                        }
//                        if (dist > set_L[L - 1 + base_set_L].distance_) {
//                            continue;
//                        }
//
//                        Candidate cand(nb_id, dist, false);
//                        // Thread 0 maintains the "global" queue
//                        idi r = add_into_queue(
//                                set_L,
//                                base_set_L,
//                                local_queues_ends[num_threads_intra_query_ - 1],
//                                L,
//                                cand);
//                        if (r < nk) {
//                            nk = r;
//                        }
//                    }
//                }
//                top_m_candidates_end = 0; // Clear top_m_candidates
////                count_distance_computation_ += tmp_count_computation;
//                tmp_count_total_computation += tmp_count_computation;
//                tmp_count_computation = 0;
//
//                if (nk <= last_k) {
//                    k = nk;
//                } else {
//                    k = last_k + 1;
//                }
//
//                {// Scale M
//                    if (M < value_M_max) {
//                        M <<= 1;
//                    } else {
//                        M = value_M_max;
//                    }
//                }
//            }
//        }
//
//        { // Multiple Threads
//            while (k < L) {
//                ++tmp_count;
////        {//test
////            printf("tmp_count: %d\n", tmp_count);
////        }
//                // Select M candidates
//                idi last_k = L;
//// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
//                for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//                    idi index_set_L = c_i + base_set_L;
//                    if (set_L[index_set_L].is_checked_) {
//                        continue;
//                    }
//                    last_k = c_i; // Record the location of the last candidate selected.
//                    set_L[index_set_L].is_checked_ = true;
//                    top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
//                }
//
//                idi nk = L;
//                // Push M candidates' neighbors into the queue.
//#pragma omp parallel for reduction(+ : tmp_count_computation) num_threads(num_threads_intra_query_)
//                for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//                    int tid = omp_get_thread_num();
//                    idi cand_id = top_m_candidates[c_i];
//                    _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//                    idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//                    idi out_degree = *out_edges++;
//                    for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                        _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//                    }
//                    for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                        idi nb_id = out_edges[e_i];
//                        { // Sequential edition
//                            if (is_visited[nb_id]) {
//                                continue;
//                            }
//                            is_visited[nb_id] = 1;
//                        }
//
//                        auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                        dataf norm = *nb_data++;
////                ++count_distance_computation_;
//                        ++tmp_count_computation;
//                        distf dist = compute_distance_with_norm(nb_data, query_data, norm);
////                        {//test
////                            if (391655 == nb_id) {
////                                printf("tmp_count: %u "
////                                       "nb_id: %u "
////                                       "distf: %f\n",
////                                       tmp_count,
////                                       nb_id,
////                                       dist);
////                            }
////                        }
//                        if (dist > set_L[L - 1 + base_set_L].distance_) {
//                            continue;
//                        }
//
//                        Candidate cand(nb_id, dist, false);
//                        // Add to the local queue.
//                        if (0 != tid) {
//                            // Non-Master threads using local queues
//                            add_into_queue(
//                                    set_L,
//                                    (tid - 1) * local_queue_length,
//                                    local_queues_ends[tid - 1],
//                                    local_queue_length,
//                                    cand);
//                        } else {
//                            // Thread 0 maintains the "global" queue
//                            idi r = add_into_queue(
//                                    set_L,
//                                    base_set_L,
//                                    local_queues_ends[num_threads_intra_query_ - 1],
//                                    L,
//                                    cand);
//                            if (r < nk) {
//                                nk = r;
//                            }
//                        }
//                    }
//                }
//                top_m_candidates_end = 0; // Clear top_m_candidates
////                count_distance_computation_ += tmp_count_computation;
//                tmp_count_total_computation += tmp_count_computation;
//                tmp_count_computation = 0;
//
////        // Merge. Merge all queues in parallel.
//                {
////                    time_merge_ -= WallTimer::get_time_mark();
//                    if (num_threads_intra_query_ > 1) {
//                        idi r = merge_all_queues_para_array(
//                                set_L,
//                                local_queues_ends,
//                                local_queue_length,
//                                L);
//                        if (r < nk) {
//                            nk = r;
//                        }
//                    }
////                    time_merge_ += WallTimer::get_time_mark();
//                }
//                if (nk <= last_k) {
//                    k = nk;
//                } else {
//                    k = last_k + 1;
//                }
//                {// Scale M
//                    if (M < value_M_max) {
//                        M <<= 1;
//                    } else {
//                        M = value_M_max;
//                    }
//                }
//            }
//        }
//        count_distance_computation_ += tmp_count_total_computation;
//        tmp_count_total_computation = 0;
//
//        auto &set_K = set_K_list[query_id];
//
//#pragma omp parallel for num_threads(num_threads_intra_query_)
//        for (idi k_i = 0; k_i < K; ++k_i) {
//            set_K[k_i] = set_L[k_i + base_set_L].id_;
////        set_K[k_i] = set_L[k_i].id_;
//        }
//
//        {// Reset
////        std::fill(is_visited.begin(), is_visited.end(), 0);
//            is_visited.reset();
////        is_visited.clear_all();
//            std::fill(local_queues_ends.begin(), local_queues_ends.end(), 0);
//        }
//    }
//
////    {//test
////        if (3 == query_id) {
////            exit(1);
////        }
////    }
////    {
////        for (idi k_i = 0; k_i < K; ++k_i) {
////            printf("%u: (%u %f)\n",
////                    k_i, set_L_list[0][k_i].id_, set_L_list[0][k_i].distance_);
////        }
////        if (0 == batch_start) {
////            exit(1);
////        }
////    }
//}

/*
 * 6/22/2020-21:30
 * Do searching on the local_set_L
 * local_set_L is already sorted
 * is_visited is already set up.
 */
inline void Searching::subsearch_with_top_m(
        const idi value_M_max,
        const idi query_id,
        const idi local_L,
        std::vector<Candidate> &set_L,
        const idi base_set_L,
        idi &set_L_end,
        std::vector<idi> &local_top_m_candidates,
        boost::dynamic_bitset<> &is_visited,
        uint64_t &local_count_distance_computation)
{
    const dataf *query_data = queries_load_ + query_id  * dimension_;
//    idi local_top_m_candidates_end = 0;
    idi k = 0; // Index of first unchecked candidate.
    idi iter = 0;
    idi M = 1; // value of M

    while (k < local_L) {
        ++iter;
        subsearch_top_m_for_one_iteration(
                iter,
                k,
                M,
                query_id,
                query_data,
                local_L,
                set_L,
                base_set_L,
                set_L_end,
                local_top_m_candidates,
                is_visited,
                local_count_distance_computation);

        {// Scale M
            if (M < value_M_max) {
                M <<= 1;
            } else {
                M = value_M_max;
            }
        }
    }
//    {//test
//        printf("base_set_L: %u "
//               "local_count_distance_computation: %lu\n",
//                base_set_L,
//                local_count_distance_computation);
//    }
}
//// Backup
//inline void Searching::subsearch_with_top_m(
//        const idi value_M_max,
//        const idi query_id,
//        const idi local_L,
//        std::vector<Candidate> &set_L,
//        const idi base_set_L,
//        idi &set_L_end,
//        std::vector<idi> &local_top_m_candidates,
//        boost::dynamic_bitset<> &is_visited,
//        uint64_t &local_count_distance_computation)
//{
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//    idi local_top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi iter = 0;
//    idi M = 1; // value of M
//
//    while (k < local_L) {
//        ++iter;
//        // Select M candidates
//        idi last_k = local_L;
//// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
//        for (idi c_i = k; c_i < set_L_end && local_top_m_candidates_end < M; ++c_i) {
//            idi index_set_L = c_i + base_set_L;
//            if (set_L[index_set_L].is_checked_) {
//                continue;
//            }
//            last_k = c_i; // Record the location of the last candidate selected.
//            set_L[index_set_L].is_checked_ = true;
//            local_top_m_candidates[local_top_m_candidates_end++] = set_L[index_set_L].id_;
//        }
//
//        idi nk = local_L;
//        // Push M candidates' neighbors into the queue.
//        for (idi c_i = 0; c_i < local_top_m_candidates_end; ++c_i) {
//            idi cand_id = local_top_m_candidates[c_i];
//            _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//            idi out_degree = *out_edges++;
//            for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//            }
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi nb_id = out_edges[e_i];
//                { // Sequential edition
//                    if (is_visited[nb_id]) {
//                        continue;
//                    }
//                    is_visited[nb_id] = 1;
//                }
//
//                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                dataf norm = *nb_data++;
//                ++local_count_distance_computation;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[set_L_end - 1 + base_set_L].distance_) {
//                    continue;
//                }
//
//                Candidate cand(nb_id, dist, false);
//                // Thread 0 maintains the "global" queue
//                idi r = add_into_queue(
//                        set_L,
//                        base_set_L,
//                        set_L_end,
//                        local_L,
//                        cand);
//                if (r < nk) {
//                    nk = r;
//                }
//            }
//        }
//        local_top_m_candidates_end = 0; // Clear top_m_candidates
//
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
//
//        {// Scale M
//            if (M < value_M_max) {
//                M <<= 1;
//            } else {
//                M = value_M_max;
//            }
//        }
//    }
//}

/*
 * 7/6/2020-23:17
 * Subsearch only 1 iteration using top-m
 */
inline void Searching::subsearch_top_m_for_one_iteration(
        const idi iter,
        idi &k_uc,
        const idi value_M,
        const idi query_id,
        const dataf *query_data,
        const idi L,
        std::vector<Candidate> &set_L,
        const idi set_L_base,
        idi &set_L_end,
        std::vector<idi> &top_m_candidates,
        boost::dynamic_bitset<> &is_visited,
        uint64_t &count_distance_computation)
{
//    uint64_t count_iter_computation = 0;

    // Select M candidates
    idi top_m_candidates_end = 0;
    idi last_k = L;
// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
    for (idi c_i = k_uc; c_i < set_L_end && top_m_candidates_end < value_M; ++c_i) {
        idi index_set_L = c_i + set_L_base;
        if (set_L[index_set_L].is_checked_) {
            continue;
        }
        last_k = c_i; // Record the location of the last candidate selected.
        set_L[index_set_L].is_checked_ = true;
        top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
    }

    idi nk = L;
    // Push M candidates' neighbors into the queue.
    for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
        idi cand_id = top_m_candidates[c_i];
        _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
        idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
        idi out_degree = *out_edges++;
        for (idi n_i = 0; n_i < out_degree; ++n_i) {
            _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
        }
        for (idi e_i = 0; e_i < out_degree; ++e_i) {
            idi nb_id = out_edges[e_i];
            { // Sequential edition
                if (is_visited[nb_id]) {
                    continue;
                }
                is_visited[nb_id] = 1;
            }

            auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
            dataf norm = *nb_data++;
            ++count_distance_computation;
//            {//test
//                ++count_iter_computation;
//            }
            distf dist = compute_distance_with_norm(nb_data, query_data, norm);
            if (dist > set_L[set_L_end - 1 + set_L_base].distance_) {
                continue;
            }

            Candidate cand(nb_id, dist, false);
//            {//test
//                if (set_L_end != L) {
//                    printf("before_add: "
//                           "query_id: %u "
//                           "iter: %u "
//                           "set_L_end: %u "
//                           "L: %u\n",
//                           query_id,
//                           iter,
//                           set_L_end,
//                           L);
//                }
//            }
            idi r = add_into_queue(
                    set_L,
                    set_L_base,
                    set_L_end,
                    L,
                    cand);
//            {//test
//                if (set_L_end != L) {
//                    printf("after_add: "
//                           "query_id: %u "
//                           "iter: %u "
//                           "set_L_end: %u "
//                           "L: %u "
//                           "r: %u\n",
//                           query_id,
//                           iter,
//                           set_L_end,
//                           L,
//                           r);
//                }
//            }
            if (r < nk) {
                nk = r;
            }
        }
    }
//    top_m_candidates_end = 0; // Clear top_m_candidates

    if (nk <= last_k) {
        k_uc = nk;
    } else {
        k_uc = last_k + 1;
    }
//    {//test
//        printf("iter: %u "
//               "set_L_base: %u "
//               "count_iter_computation: %lu\n",
//               iter,
//               set_L_base,
//               count_iter_computation);
//    }
}

/*
 * One more parameter for distance bound
 */
inline void Searching::subsearch_top_m_for_one_iteration(
        const distf bound_lth,
        const idi iter,
        idi &k_uc,
        const idi value_M,
        const idi query_id,
        const dataf *query_data,
        const idi L,
        std::vector<Candidate> &set_L,
        const idi set_L_base,
        idi &set_L_end,
        std::vector<idi> &top_m_candidates,
        boost::dynamic_bitset<> &is_visited,
        uint64_t &count_distance_computation)
{
    // Select M candidates
    idi top_m_candidates_end = 0;
    idi last_k = L;
// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
    for (idi c_i = k_uc; c_i < set_L_end && top_m_candidates_end < value_M; ++c_i) {
        idi index_set_L = c_i + set_L_base;
        if (set_L[index_set_L].is_checked_) {
            continue;
        }
        last_k = c_i; // Record the location of the last candidate selected.
        set_L[index_set_L].is_checked_ = true;
        top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
    }

    idi nk = L;
    // Push M candidates' neighbors into the queue.
    for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
        idi cand_id = top_m_candidates[c_i];
        _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
        idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
        idi out_degree = *out_edges++;
        for (idi n_i = 0; n_i < out_degree; ++n_i) {
            _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
        }
        for (idi e_i = 0; e_i < out_degree; ++e_i) {
            idi nb_id = out_edges[e_i];
            { // Sequential edition
                if (is_visited[nb_id]) {
                    continue;
                }
                is_visited[nb_id] = 1;
            }

            auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
            dataf norm = *nb_data++;
            ++count_distance_computation;
            distf dist = compute_distance_with_norm(nb_data, query_data, norm);
            if (dist > bound_lth) {
                continue;
            }

            Candidate cand(nb_id, dist, false);
            idi r = add_into_queue(
                    set_L,
                    set_L_base,
                    set_L_end,
                    L,
                    cand);
            if (r < nk) {
                nk = r;
            }
        }
    }

    if (nk <= last_k) {
        k_uc = nk;
    } else {
        k_uc = last_k + 1;
    }
}

/*
 * 6/23/2020-13:37
 * Is is good to use subsearch by every thread it self?
 */
inline void Searching::para_search_with_top_m_subsearch_v0(
//        const idi value_M_middle,
        const idi value_M_max,
        const idi query_id,
        const idi K,
        const idi L,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K,
//        const idi local_queue_length, // Maximum size of local queue
//        const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
//        std::vector<idi> &local_queues_ends, // Sizes of local queue
        std::vector< std::vector<idi> > &top_m_candidates_list,
        boost::dynamic_bitset<> &is_visited)
{
    uint64_t tmp_count_computation = 0;
    {// Initialization
        // is_visited flag array
//#pragma omp parallel for
// Cannot use OMP for bit array is_visited!
        for (idi c_i = 0; c_i < L; ++c_i) {
            is_visited[init_ids[c_i]] = 1;
        }

        const dataf *query_data = queries_load_ + query_id * dimension_;
#pragma omp parallel for
        for (idi v_i = 0; v_i < L; ++v_i) {
            idi v_id = init_ids[v_i];
            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
        }

        // Get the distances of all candidates, store in the set set_L.
//#pragma omp parallel for
#pragma omp parallel for reduction(+ : tmp_count_computation)
        for (unsigned i = 0; i < L; i++) {
            unsigned v_id = init_ids[i];
            auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
            dataf norm = *v_data++;
            ++tmp_count_computation;
            distf dist = compute_distance_with_norm(v_data, query_data, norm);
            set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//            {//test
//                printf("q_i: %u "
//                       "init_ids[%u]: "
//                       "count: %u "
//                       "id: %u "
//                       "dist: %f \n",
//                       query_id,
//                       i,
//                       tmp_count_computation,
//                       v_id,
//                       dist);
//            }
        }
//        {//test
//            printf("Initialization tmp_count_computation: %lu\n",
//                    tmp_count_computation);
//        }
        count_distance_computation_ += tmp_count_computation;
        tmp_count_computation = 0;
//        std::sort(
//                set_L.begin(),
//                set_L.begin() + L);
    }
    idi queue_end = L;

    // Searching
    if (num_threads_ == 1) { // Single threads
        std::sort(
                set_L.begin(),
                set_L.end());
        subsearch_with_top_m(
                value_M_max,
                query_id,
                L,
                set_L,
                0,
                queue_end,
                top_m_candidates_list[0],
                is_visited,
                tmp_count_computation);
        count_distance_computation_ += tmp_count_computation;
        tmp_count_computation = 0;

//        {
//            idi half_length = queue_end / 2;
//            std::sort(
//                    set_L.begin(),
//                    set_L.begin() + half_length);
//
//            subsearch_with_top_m(
//                    value_M_max,
//                    query_id,
//                    half_length,
//                    set_L,
//                    0,
//                    half_length,
//                    top_m_candidates_list[0],
//                    is_visited,
//                    tmp_count_computation);
//
//            std::sort(
//                    set_L.begin() + half_length,
//                    set_L.end());
//            subsearch_with_top_m(
//                    value_M_max,
//                    query_id,
//                    half_length,
//                    set_L,
//                    half_length,
//                    half_length,
//                    top_m_candidates_list[0],
//                    is_visited,
//                    tmp_count_computation);
//            count_distance_computation_ += tmp_count_computation;
//
//            std::vector <Candidate> tmp_set_L(L);
//            std::merge(set_L.begin(), set_L.begin() + half_length,
//                       set_L.begin() + half_length, set_L.end(),
//                       tmp_set_L.begin());
//            std::copy(tmp_set_L.begin(), tmp_set_L.end(), set_L.begin());
//        }
//        {//test
//            printf("q_i: %u "
//                   "count_distance_computation_: %lu\n",
//                   query_id,
//                   count_distance_computation_);
//        }
    } else { // Multiple threads
        const idi num_queues = num_threads_;
        const idi local_queue_length = (L - 1) / num_queues + 1;
        // Parallel for
#pragma omp parallel for reduction(+ : tmp_count_computation)
        for (idi q_i = 0; q_i < num_queues; ++q_i) {
            idi local_queue_base = q_i * local_queue_length;
            if (local_queue_base >= L) {
                continue;
            }
            idi local_queue_end = local_queue_length;
            if (local_queue_base + local_queue_end > L) {
                local_queue_end = L - local_queue_base;
            }
            std::sort(
                    set_L.begin() + local_queue_base,
                    set_L.begin() + local_queue_base + local_queue_end);

            subsearch_with_top_m(
//                    1,
                    value_M_max / num_queues, // value_M_max
//                    local_queue_end, // value_M_max
                    query_id,
                    local_queue_end, // local_L
                    set_L,
                    local_queue_base, // base_set_L
                    local_queue_end, // set_L_end
                    top_m_candidates_list[q_i],
                    is_visited,
                    tmp_count_computation);
//            {//test
//                printf("q_i: %u "
//                       "tmp_count_computation: %lu\n",
//                       q_i,
//                       tmp_count_computation);
//                count_distance_computation_ += tmp_count_computation;
//                tmp_count_computation = 0;
//            }
        }
        count_distance_computation_ += tmp_count_computation;
        tmp_count_computation = 0;
//        {//test
//            printf("query_id: %u "
//                   "count_distance_computation_: %lu\n",
//                   query_id,
//                   count_distance_computation_);
//        }

        // Merge
        time_merge_ -= WallTimer::get_time_mark();
        merge_in_set_L(
                set_L,
                L,
                num_queues,
                local_queue_length);
        time_merge_ += WallTimer::get_time_mark();
//        {//test
//            printf("q_i: %u "
//                   "count_distance_computation_: %lu\n",
//                   query_id,
//                   count_distance_computation_);
//        }
    }

    {// Return the results to set_K
        // How to deal with duplicate?
        idi last_id = set_L[0].id_;
        set_K[0] = last_id;
        idi k_i = 1;
        idi l_i = 1;
        while (k_i < K && l_i < L) {
            if (last_id == set_L[l_i].id_) {
                ++l_i;
                continue;
            }
            last_id = set_L[l_i++].id_;
            set_K[k_i++] = last_id;
        }

//#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
////        set_K[k_i] = set_L[k_i].id_;
//    }
    }
//    {
//        for (idi k_i = 0; k_i < L; ++k_i) {
//            printf("q_i: %u "
//                   "k_i: %u "
//                   "id: %u "
//                   "dist: %f\n",
//                   query_id,
//                   k_i,
//                   set_L[k_i].id_,
//                   set_L[k_i].distance_);
//        }
//    }

    {// Reset
//        std::fill(is_visited.begin(), is_visited.end(), 0);
        is_visited.reset();
//        is_visited.clear_all();
    }

//    {//test
//        if (0 == query_id) {
//            exit(1);
//        }
//    }
}

/*
 * 7/5/2020-18:38
 * L-th Selection, and every thread does its own searching
 */
inline void Searching::para_search_with_top_m_subsearch_v1(
        const idi local_M_max,
        const idi query_id,
        const idi K,
        const idi global_L,
        const idi local_L,
        const idi total_L,
        const idi init_queue_end,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K,
        const std::vector<idi> &local_queues_bases,
        std::vector<idi> &local_queues_ends,
        std::vector< std::vector<idi> > &top_m_candidates_list,
        boost::dynamic_bitset<> &is_visited)
{
    uint64_t tmp_count_computation = 0;
    {// Initialization
        // is_visited flag array
//#pragma omp parallel for
// Cannot use OMP for bit array is_visited!
        for (idi c_i = 0; c_i < total_L; ++c_i) {
            is_visited[init_ids[c_i]] = 1;
        }

        const dataf *query_data = queries_load_ + query_id * dimension_;
#pragma omp parallel for
        for (idi v_i = 0; v_i < total_L; ++v_i) {
            idi v_id = init_ids[v_i];
            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
        }

        // Get the distances of all candidates, store in the set set_L.
#pragma omp parallel for reduction(+ : tmp_count_computation)
        for (int q_i = 0; q_i < num_threads_; ++q_i) {
            idi local_queue_base = local_queues_bases[q_i];
            idi init_ids_base = q_i * init_queue_end;
            idi init_ids_bound = init_ids_base + init_queue_end;
            for (idi id_i = init_ids_base; id_i < init_ids_bound; ++id_i) {
                idi v_id = init_ids[id_i];
                auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
                dataf norm = *v_data++;
                ++tmp_count_computation;
                distf dist = compute_distance_with_norm(v_data, query_data, norm);
                set_L[local_queue_base++] = Candidate(v_id, dist, false); // False means not checked.
            }
            local_queues_ends[q_i] = init_queue_end;
        }
        count_distance_computation_ += tmp_count_computation;
        tmp_count_computation = 0;

//#pragma omp parallel for reduction(+ : tmp_count_computation)
//        for (unsigned i = 0; i < total_L; i++) {
//            unsigned v_id = init_ids[i];
//            auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//            dataf norm = *v_data++;
//            ++tmp_count_computation;
//            distf dist = compute_distance_with_norm(v_data, query_data, norm);
//            set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//        }
//        count_distance_computation_ += tmp_count_computation;
//        tmp_count_computation = 0;
    }

    // Searching
    if (num_threads_ == 1) { // Single threads
//        local_queues_lengths[0] = local_L;
        std::sort(
                set_L.begin(),
                set_L.end());
        subsearch_with_top_m(
                local_M_max,
                query_id,
                local_L,
                set_L,
                0,
                local_queues_ends[0],
                top_m_candidates_list[0],
                is_visited,
                tmp_count_computation);
        count_distance_computation_ += tmp_count_computation;
        tmp_count_computation = 0;
    } else { // Multiple threads
//        std::fill(local_queues_lengths.begin(), local_queues_lengths.end(), local_L);
        const dataf *query_data = queries_load_ + query_id  * dimension_;
        const idi num_queues = num_threads_;
#pragma omp parallel for
        for (idi q_i = 0; q_i < num_queues; ++q_i) {
            idi local_queue_base = local_queues_bases[q_i];
            std::sort(
                    set_L.begin() + local_queue_base,
                    set_L.begin() + local_queue_base + init_queue_end);
        }
        distf bound_lth = FLT_MAX;
        for (idi q_i = 0; q_i < num_queues; ++q_i) {
            bound_lth = std::min(bound_lth, set_L[local_queues_bases[q_i] + init_queue_end - 1].distance_);
        }
//        const idi local_queue_length = (L - 1) / num_queues + 1;
        std::vector<idi> ks(num_queues, 0);
        idi iter = 0;
        idi local_M = 1;
        uint8_t not_finished = 1;
        while (not_finished) {
            not_finished = 0;
            ++iter;
#pragma omp parallel for reduction(+ : tmp_count_computation)
            for (idi q_i = 0; q_i < num_queues; ++q_i) {
                idi &k = ks[q_i];
                idi &local_queue_end = local_queues_ends[q_i];
                auto &local_top_m_candidates = top_m_candidates_list[q_i];
                if (k >= local_queue_end) {
                    continue;
                }
                not_finished = 1;
                const idi local_queue_base = local_queues_bases[q_i];

                subsearch_top_m_for_one_iteration(
                        bound_lth,
                        iter,
                        k,
                        local_M,
                        query_id,
                        query_data,
                        local_L,
                        set_L,
                        local_queue_base,
                        local_queue_end,
                        local_top_m_candidates,
                        is_visited,
                        tmp_count_computation);
            }
            count_distance_computation_ += tmp_count_computation;
            tmp_count_computation = 0;
            {// Setecting and update local_queues_lengths
                time_merge_ -= WallTimer::get_time_mark();
                bound_lth = selecting_top_L_seq(
                        set_L,
                        global_L,
//                        local_L,
                        num_queues,
                        local_queues_bases,
                        local_queues_ends);
                time_merge_ += WallTimer::get_time_mark();
//                {// local_queues_ends
//                    printf("query_id: %u "
//                           "iter: %u",
////                           "local_queues_ends:",
//                           query_id,
//                           iter);
//                    for (idi q_i = 0; q_i < num_queues; ++q_i) {
//                        printf(" [%u]: %u",
//                                q_i,
//                                local_queues_ends[q_i]);
//                    }
//                    printf("\n");
//                }
            }
            {// Scale M
                if (local_M < local_M_max) {
                    local_M <<= 1;
                }
//                else {
//                    local_M = value_M_max;
//                }
            }
        }
    }

    time_merge_ -= WallTimer::get_time_mark();
    {// Return the results to set_K
        std::vector<idi> pointer(num_threads_, 0);
        // get the first
        distf min_dist = FLT_MAX;
        idi min_q_i;
        idi min_sub;
        idi last_id;
        for (int q_i = 0; q_i < num_threads_; ++q_i) {
            if (pointer[q_i] >= local_queues_ends[q_i]) {
                continue;
            }
            idi sub = pointer[q_i] + local_queues_bases[q_i];
            distf tmp_dist = set_L[sub].distance_;
            if (tmp_dist < min_dist) {
                min_dist = tmp_dist;
                min_q_i = q_i;
                min_sub = sub;
            }
        }
        set_K[0] = set_L[min_sub].id_;
        ++pointer[min_q_i];
        last_id = set_K[0];

        bool is_finished = false;
        idi k_i = 1;
        while (k_i < K && !is_finished) {
            is_finished = true;
            min_dist = FLT_MAX;
            for (int q_i = 0; q_i < num_threads_; ++q_i) {
                const idi local_queue_end = local_queues_ends[q_i];
                if (pointer[q_i] >= local_queue_end) {
                    continue;
                }
                is_finished = false;
                idi sub = pointer[q_i] + local_queues_bases[q_i];
                while (set_L[sub].id_ == last_id
                        && pointer[q_i] < local_queue_end) {
                    ++pointer[q_i];
                    sub = pointer[q_i] + local_queues_bases[q_i];
                }
                if (pointer[q_i] >= local_queue_end) {
                    continue;
                }
//                if (set_L[sub].id_ == last_id) {
//                    // Duplicate
//                    ++pointer[q_i];
//                    continue;
//                }
                distf tmp_dist = set_L[sub].distance_;
                if (tmp_dist < min_dist) {
                    min_dist = tmp_dist;
                    min_q_i = q_i;
                    min_sub = sub;
                }
            }
            set_K[k_i] = set_L[min_sub].id_;
            ++pointer[min_q_i];
            ++k_i;
        }
    }
    time_merge_ += WallTimer::get_time_mark();
//    {// Return the results to set_K
//        // How to deal with duplicate?
//        idi last_id = set_L[0].id_;
//        set_K[0] = last_id;
//        idi k_i = 1;
//        idi l_i = 1;
//        while (k_i < K && l_i < L) {
//            if (last_id == set_L[l_i].id_) {
//                ++l_i;
//                continue;
//            }
//            last_id = set_L[l_i++].id_;
//            set_K[k_i++] = last_id;
//        }
//    }

    {// Reset
//        std::fill(is_visited.begin(), is_visited.end(), 0);
        is_visited.reset();
//        is_visited.clear_all();
//        std::fill(local_queues_ends.begin(), local_queues_ends.end(), init_queue_end);
    }

//    {//test
//        if (3 == query_id) {
//            exit(1);
//        }
//    }
}
//// Backup
//inline void Searching::para_search_with_top_m_subsearch_v1(
//        const idi local_M_max,
//        const idi query_id,
//        const idi K,
//        const idi total_L,
//        const idi local_L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K,
//        const std::vector<idi> &local_queues_bases,
//        std::vector<idi> &local_queues_lengths,
//        std::vector< std::vector<idi> > &top_m_candidates_list,
//        boost::dynamic_bitset<> &is_visited)
//{
//    uint64_t tmp_count_computation = 0;
//    {// Initialization
//        // is_visited flag array
////#pragma omp parallel for
//// Cannot use OMP for bit array is_visited!
//        for (idi c_i = 0; c_i < total_L; ++c_i) {
//            is_visited[init_ids[c_i]] = 1;
//        }
//
//        const dataf *query_data = queries_load_ + query_id * dimension_;
//#pragma omp parallel for
//        for (idi v_i = 0; v_i < total_L; ++v_i) {
//            idi v_id = init_ids[v_i];
//            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//        }
//
//        // Get the distances of all candidates, store in the set set_L.
////#pragma omp parallel for
//#pragma omp parallel for reduction(+ : tmp_count_computation)
//        for (unsigned i = 0; i < total_L; i++) {
//            unsigned v_id = init_ids[i];
//            auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//            dataf norm = *v_data++;
//            ++tmp_count_computation;
//            distf dist = compute_distance_with_norm(v_data, query_data, norm);
//            set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//        }
//        count_distance_computation_ += tmp_count_computation;
//        tmp_count_computation = 0;
////        std::sort(
////                set_L.begin(),
////                set_L.begin() + L);
//    }
////    idi queue_end = L;
//
//    // Searching
//    if (num_threads_ == 1) { // Single threads
////        local_queues_lengths[0] = local_L;
//        std::sort(
//                set_L.begin(),
//                set_L.end());
//        subsearch_with_top_m(
//                local_M_max,
//                query_id,
//                local_L,
//                set_L,
//                0,
//                local_queues_lengths[0],
//                top_m_candidates_list[0],
//                is_visited,
//                tmp_count_computation);
//        count_distance_computation_ += tmp_count_computation;
//    } else { // Multiple threads
////        std::fill(local_queues_lengths.begin(), local_queues_lengths.end(), local_L);
//        const dataf *query_data = queries_load_ + query_id  * dimension_;
//        const idi num_queues = num_threads_;
//#pragma omp parallel for
//        for (idi q_i = 0; q_i < num_queues; ++q_i) {
//            idi local_queue_base = local_queues_bases[q_i];
//            std::sort(
//                    set_L.begin() + local_queue_base,
//                    set_L.begin() + local_queue_base + local_L);
//        }
////        const idi local_queue_length = (L - 1) / num_queues + 1;
//        std::vector<idi> ks(num_queues, 0);
//        idi iter = 0;
//        idi local_M = 1;
//        uint8_t not_finished = 1;
//        while (not_finished) {
//            not_finished = 0;
//            ++iter;
//
////#pragma omp parallel for reduction(+ : tmp_count_computation)
//            for (idi q_i = 0; q_i < num_queues; ++q_i) {
//                idi &k = ks[q_i];
//                idi &local_queue_end = local_queues_lengths[q_i];
//                auto &local_top_m_candidates = top_m_candidates_list[q_i];
//                if (k >= local_queue_end) {
//                    continue;
//                }
//                not_finished = 1;
////                ++not_finished;
//                const idi local_queue_base = local_queues_bases[q_i];
//
//                // Select top-M unchecked vertices.
//                idi last_k = local_L;
//                idi local_top_m_candidates_end = 0;
//// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
//                for (idi c_i = k; c_i < local_queue_end && local_top_m_candidates_end < local_M; ++c_i) {
//                    idi index_set_L = c_i + local_queue_base;
//                    if (set_L[index_set_L].is_checked_) {
//                        continue;
//                    }
//                    last_k = c_i; // Record the location of the last candidate selected.
//                    set_L[index_set_L].is_checked_ = true;
//                    local_top_m_candidates[local_top_m_candidates_end++] = set_L[index_set_L].id_;
//                }
//
//                idi nk = local_L;
//                // Push M candidates' neighbors into the queue.
//                for (idi c_i = 0; c_i < local_top_m_candidates_end; ++c_i) {
//                    idi cand_id = local_top_m_candidates[c_i];
//                    _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//                    idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//                    idi out_degree = *out_edges++;
//                    for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                        _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//                    }
//                    for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                        idi nb_id = out_edges[e_i];
//                        { // Sequential edition
//                            if (is_visited[nb_id]) {
//                                continue;
//                            }
//                            is_visited[nb_id] = 1;
//                        }
//
//                        auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                        dataf norm = *nb_data++;
//                        ++tmp_count_computation;
//                        distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                        if (dist > set_L[local_queue_end - 1 + local_queue_end].distance_) {
//                            continue;
//                        }
//
//                        Candidate cand(nb_id, dist, false);
//                        // Thread 0 maintains the "global" queue
//                        idi r = add_into_queue(
//                                set_L,
//                                local_queue_base,
//                                local_queue_end,
//                                local_L,
//                                cand);
//                        if (r < nk) {
//                            nk = r;
//                        }
//                    }
//                }
//                local_top_m_candidates_end = 0; // Clear top_m_candidates
//
//                if (nk <= last_k) {
//                    k = nk;
//                } else {
//                    k = last_k + 1;
//                }
//            }
//            count_distance_computation_ += tmp_count_computation;
//            tmp_count_computation = 0;
//            {// Setecting and update local_queues_lengths
//                time_merge_ -= WallTimer::get_time_mark();
//                selecting_top_L_seq(
//                        set_L,
//                        total_L,
////                        local_L,
//                        num_queues,
//                        local_queues_bases,
//                        local_queues_lengths);
////        merge_in_set_L(
////                set_L,
////                L,
////                num_queues,
////                local_queue_length);
//                time_merge_ += WallTimer::get_time_mark();
//            }
//            {// Scale M
//                if (local_M < local_M_max) {
//                    local_M <<= 1;
//                }
////                else {
////                    local_M = value_M_max;
////                }
//            }
//        }
//    }
//
//    {// Return the results to set_K
//        std::vector<idi> pointer(num_threads_, 0);
//        // get the first
//        distf min_dist = FLT_MAX;
//        idi min_q_i;
//        idi min_sub;
//        idi last_id;
//        for (int q_i = 0; q_i < num_threads_; ++q_i) {
//            {//test
//                if (local_queues_lengths[q_i] != local_L) {
//                    printf("What? local_queues_lengths[%u]: %u != local_L: %u\n",
//                            q_i, local_queues_lengths[q_i], local_L);
//                }
//            }
//            if (pointer[q_i] >= local_queues_lengths[q_i]) {
//                continue;
//            }
//            idi sub = pointer[q_i] + local_queues_bases[q_i];
//            distf tmp_dist = set_L[sub].distance_;
//            if (tmp_dist < min_dist) {
//                min_dist = tmp_dist;
//                min_q_i = q_i;
//                min_sub = sub;
//            }
//        }
//        set_K[0] = set_L[min_sub].id_;
//        ++pointer[min_q_i];
//        last_id = set_K[0];
//
//        bool is_finished = false;
//        idi k_i = 1;
//        while (k_i < K && !is_finished) {
//            is_finished = true;
//            min_dist = FLT_MAX;
////            distf min_dist = FLT_MAX;
////            idi min_sub;
//            for (int q_i = 0; q_i < num_threads_; ++q_i) {
//                if (pointer[q_i] >= local_queues_lengths[q_i]) {
//                    continue;
//                }
//                is_finished = false;
//                idi sub = pointer[q_i] + local_queues_bases[q_i];
//                if (set_L[sub].id_ == last_id) {
//                    // Duplicate
//                    ++pointer[q_i];
//                    continue;
//                }
//                distf tmp_dist = set_L[sub].distance_;
//                if (tmp_dist < min_dist) {
//                    min_dist = tmp_dist;
//                    min_q_i = q_i;
//                    min_sub = sub;
//                }
//            }
//            set_K[k_i] = set_L[min_sub].id_;
//            ++pointer[min_q_i];
//            ++k_i;
//        }
//    }
////    {// Return the results to set_K
////        // How to deal with duplicate?
////        idi last_id = set_L[0].id_;
////        set_K[0] = last_id;
////        idi k_i = 1;
////        idi l_i = 1;
////        while (k_i < K && l_i < L) {
////            if (last_id == set_L[l_i].id_) {
////                ++l_i;
////                continue;
////            }
////            last_id = set_L[l_i++].id_;
////            set_K[k_i++] = last_id;
////        }
////    }
//
//    {// Reset
////        std::fill(is_visited.begin(), is_visited.end(), 0);
//        is_visited.reset();
////        is_visited.clear_all();
//        std::fill(local_queues_lengths.begin(), local_queues_lengths.end(), local_L);
//    }
//
////    {//test
////        if (1000 == query_id) {
////            exit(1);
////        }
////    }
//}

/*
 * 6/27/2020-12:33
 * Do searching on the local_set_L
 * local_set_L is already sorted
 * is_visited is already set up.
 */
inline void Searching::subsearch_for_simple_search(
        const idi query_id,
        const idi local_L,
        std::vector<Candidate> &set_L,
        const idi base_set_L,
        idi &set_L_end,
//        std::vector<uint8_t> &is_visited,
        boost::dynamic_bitset<> &is_visited,
        uint64_t &local_count_distance_computation)
{
    const dataf *query_data = queries_load_ + query_id  * dimension_;
//    idi local_top_m_candidates_end = 0;
    idi k = 0; // Index of first unchecked candidate.
    idi iter = 0;
//    idi M = 1; // value of M

    while (k < local_L) {
        ++iter;
//        {//test
//            printf("query_id: %u "
//                   "iter: %u\n",
//                   query_id,
//                   iter);
//        }

        // Select the top-1 unchecked candidate
        idi top_1;
        idi last_k = local_L;
// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
        for (idi c_i = k; c_i < set_L_end; ++c_i) {
            idi index_set_L = c_i + base_set_L;
            if (set_L[index_set_L].is_checked_) {
                continue;
            }
            top_1 = set_L[index_set_L].id_;
            last_k = c_i; // Record the location of the last candidate selected.
            set_L[index_set_L].is_checked_ = true;
//            local_top_m_candidates[local_top_m_candidates_end++] = set_L[index_set_L].id_;
            break;
        }
        if (last_k == local_L) {
            break;
        }
        idi nk = local_L;
        // Push top-1' neighbors into the queue.
        idi cand_id = top_1;
        _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
        idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
        idi out_degree = *out_edges++;
        for (idi n_i = 0; n_i < out_degree; ++n_i) {
            _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
        }
        for (idi e_i = 0; e_i < out_degree; ++e_i) {
            idi nb_id = out_edges[e_i];
            { // Sequential edition
                if (is_visited[nb_id]) {
                    continue;
                }
                is_visited[nb_id] = 1;
            }

//            {// Critical edition
//                if (!AtomicOps::CAS(is_visited.data() + nb_id,
//                                    static_cast<uint8_t>(0),
//                                    static_cast<uint8_t>(1))) {
//                    continue;
//                }
//            }

            auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
            dataf norm = *nb_data++;
            ++local_count_distance_computation;
            distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//            {
//                if (0 == query_id
//                    && (785802 == nb_id
//                        || 180955 == nb_id
//                        || 240996 == nb_id
//                        || 813701 == nb_id
//                        || 708177 == nb_id
//                        || 87578 == nb_id
//                        || 561813 == nb_id
//                        || 701258 == nb_id
//                        || 872728 == nb_id)) {
////                    && 180955 == nb_id) {
//                    printf("parent: %u "
//                           "nb_id: %u "
//                           "dist: %f "
//                           "base_set_L: %u "
//                           "set_L_end: %u\n",
//                           cand_id,
//                           nb_id,
//                           dist,
//                           base_set_L,
//                           set_L_end);
//                }
//            }
            if (dist > set_L[set_L_end - 1 + base_set_L].distance_) {
                continue;
            }

            Candidate cand(nb_id, dist, false);
            // Thread 0 maintains the "global" queue
            idi r = add_into_queue(
                    set_L,
                    base_set_L,
                    set_L_end,
                    local_L,
                    cand);
            if (r < nk) {
                nk = r;
            }
        }

        if (nk <= last_k) {
            k = nk;
        } else {
            k = last_k + 1;
        }
    }
}

/*
 * 6/27/2020-12:26
 * Is is good to use subsearch by every thread it self?
 */
inline void Searching::para_simple_search_subsearch(
        const idi query_id,
        const idi K,
        const idi L,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K,
//        std::vector<uint8_t> &is_visited)
        boost::dynamic_bitset<> &is_visited)
{
    uint64_t tmp_count_computation = 0;
    {// Initialization
        // is_visited flag array
//#pragma omp parallel for
// Cannot use OMP for bit array is_visited!
        for (idi c_i = 0; c_i < L; ++c_i) {
            is_visited[init_ids[c_i]] = 1;
        }

        const dataf *query_data = queries_load_ + query_id * dimension_;
#pragma omp parallel for
        for (idi v_i = 0; v_i < L; ++v_i) {
            idi v_id = init_ids[v_i];
            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
        }

        // Get the distances of all candidates, store in the set set_L.
//#pragma omp parallel for
#pragma omp parallel for reduction(+ : tmp_count_computation)
        for (unsigned i = 0; i < L; i++) {
            unsigned v_id = init_ids[i];
            auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
            dataf norm = *v_data++;
            ++tmp_count_computation;
            distf dist = compute_distance_with_norm(v_data, query_data, norm);
            set_L[i] = Candidate(v_id, dist, false); // False means not checked.
        }
        count_distance_computation_ += tmp_count_computation;
        tmp_count_computation = 0;
//        std::sort(
//                set_L.begin(),
//                set_L.begin() + L);
    }
    idi queue_end = L;

    // Searching
    if (num_threads_ == 1) { // Single threads
        std::sort(
                set_L.begin(),
                set_L.end());
        subsearch_for_simple_search(
                query_id,
                L,
                set_L,
                0,
                queue_end,
                is_visited,
                tmp_count_computation);
        count_distance_computation_ += tmp_count_computation;

//        {
////            {//test
////                for (idi i = 0; i < queue_end; ++i) {
////                    printf("start: "
////                           "query_id: %u "
////                           "set_L[%u]: "
////                           "(%u %f)\n",
////                           query_id,
////                           i,
////                           set_L[i].id_, set_L[i].distance_);
////                }
////            }
//
//            idi half_length = queue_end / 2;
//            std::sort(
//                    set_L.begin(),
//                    set_L.begin() + half_length);
////            {//test
////                for (idi i = 0; i < half_length; ++i) {
////                    printf("sorted: "
////                           "query_id: %u "
////                           "set_L[%u]: "
////                           "(%u %f)\n",
////                           query_id,
////                           i,
////                           set_L[i].id_, set_L[i].distance_);
////                }
////            }
//
//            subsearch_for_simple_search(
//                    query_id,
//                    half_length, // local_L
//                    set_L,
//                    0, // base_set_L
//                    half_length, // set_L_end
//                    is_visited,
//                    tmp_count_computation);
//
////            {//test
////                for (idi i = 0; i < half_length; ++i) {
////                    printf("subsearched: "
////                           "query_id: %u "
////                           "set_L[%u]: "
////                           "(%u %f)\n",
////                           query_id,
////                           i,
////                           set_L[i].id_, set_L[i].distance_);
////                }
////            }
//
//            std::sort(
//                    set_L.begin() + half_length,
//                    set_L.end());
//
////            {//test
////                for (idi i = half_length; i < queue_end; ++i) {
////                    printf("sorted: "
////                           "query_id: %u "
////                           "set_L[%u]: "
////                           "(%u %f)\n",
////                           query_id,
////                           i,
////                           set_L[i].id_, set_L[i].distance_);
////                }
////            }
//
//            subsearch_for_simple_search(
//                    query_id,
//                    half_length, // local_L
//                    set_L,
//                    half_length, // base_set_L
//                    half_length, // set_L_end
//                    is_visited,
//                    tmp_count_computation);
////            {//test
////                for (idi i = half_length; i < queue_end; ++i) {
////                    printf("subsearched: "
////                           "query_id: %u "
////                           "set_L[%u]: "
////                           "(%u %f)\n",
////                           query_id,
////                           i,
////                           set_L[i].id_, set_L[i].distance_);
////                }
////            }
////            {//test
////                for (idi i = 0; i < queue_end; ++i) {
////                    printf("explored: "
////                           "query_id: %u "
////                           "set_L[%u]: "
////                           "(%u %f)\n",
////                           query_id,
////                           i,
////                           set_L[i].id_, set_L[i].distance_);
////                }
////            }
//            count_distance_computation_ += tmp_count_computation;
//
//            std::vector <Candidate> tmp_set_L(L);
//            std::merge(set_L.begin(), set_L.begin() + half_length,
//                       set_L.begin() + half_length, set_L.end(),
//                       tmp_set_L.begin());
//            std::copy(tmp_set_L.begin(), tmp_set_L.end(), set_L.begin());
////            {//test
////                for (idi i = 0; i < queue_end; ++i) {
////                    printf("merged: "
////                           "query_id: %u "
////                           "set_L[%u]: "
////                           "(%u %f)\n",
////                           query_id,
////                           i,
////                           set_L[i].id_, set_L[i].distance_);
////                }
////            }
//        }

    } else { // Multiple threads
        const idi num_queues = num_threads_;
        const idi local_queue_length = (L - 1) / num_queues + 1;
        // Parallel for
#pragma omp parallel for reduction(+ : tmp_count_computation)
        for (idi q_i = 0; q_i < num_queues; ++q_i) {
            idi local_queue_base = q_i * local_queue_length;
            if (local_queue_base >= L) {
                continue;
            }
            idi local_queue_end = local_queue_length;
            if (local_queue_base + local_queue_end > L) {
                local_queue_end = L - local_queue_base;
            }
            std::sort(
                    set_L.begin() + local_queue_base,
                    set_L.begin() + local_queue_base + local_queue_end);
            subsearch_for_simple_search(
                    query_id,
                    local_queue_end, // local_L
                    set_L,
                    local_queue_base, // base_set_L
                    local_queue_end, // set_L_end
                    is_visited,
                    tmp_count_computation);
        }
        count_distance_computation_ += tmp_count_computation;

        // Merge
        time_merge_ -= WallTimer::get_time_mark();
        merge_in_set_L(
                set_L,
                L,
                num_queues,
                local_queue_length);
        time_merge_ += WallTimer::get_time_mark();
    }

    {// Return the results to set_K
        // How to deal with duplicate?
        idi last_id = set_L[0].id_;
        set_K[0] = last_id;
        idi k_i = 1;
        idi l_i = 1;
        while (k_i < K && l_i < L) {
            if (last_id == set_L[l_i].id_) {
                ++l_i;
                continue;
            }
            last_id = set_L[l_i++].id_;
            set_K[k_i++] = last_id;
        }

//#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
////        set_K[k_i] = set_L[k_i].id_;
//    }
    }

    {// Reset
//        std::fill(is_visited.begin(), is_visited.end(), 0);
        is_visited.reset();
//        is_visited.clear_all();
    }

//    {//test
//        if (0 == query_id) {
//            exit(1);
//        }
//    }
}

///*
// * 6/22/2020-09:38
// * A synchronized last element as the sentinel
// */
//inline void Searching::para_search_with_top_m_merge_queues_global_threshold(
//        const idi value_M_middle,
//        const idi value_M_max,
//        const idi query_id,
//        const idi K,
//        const idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K,
//        const idi local_queue_length, // Maximum size of local queue
//        const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
//        std::vector<idi> &local_queues_ends, // Sizes of local queue
//        std::vector<idi> &top_m_candidates,
//        boost::dynamic_bitset<> &is_visited)
//{
////    const idi base_set_L = (num_threads_ - 1) * local_queue_length;
//    {
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = 1;
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//#pragma omp parallel for
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    uint64_t tmp_count_computation = 0;
//    // Get the distances of all candidates, store in the set set_L.
////#pragma omp parallel for
//#pragma omp parallel for reduction(+ : tmp_count_computation)
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
//        ++tmp_count_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i + base_set_L] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    count_distance_computation_ += tmp_count_computation;
//    tmp_count_computation = 0;
////    std::sort(set_L.begin(), set_L.begin() + L);
//    std::sort(
//            set_L.begin() + base_set_L,
//            set_L.begin() + base_set_L + L);
//    local_queues_ends[num_threads_ - 1] = L;
//
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    idi M = 1;
//
//    { // Single thread
//        while (k < L && M < value_M_middle) {
//            ++tmp_count;
////        {//test
////            printf("tmp_count: %d\n", tmp_count);
////        }
//
//            // Select M candidates
//            idi last_k = L;
//// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
//            for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//                idi index_set_L = c_i + base_set_L;
//                if (set_L[index_set_L].is_checked_) {
//                    continue;
//                }
//                last_k = c_i; // Record the location of the last candidate selected.
//                set_L[index_set_L].is_checked_ = true;
//                top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
//            }
//
//            idi nk = L;
//            // Push M candidates' neighbors into the queue.
//            for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//                idi cand_id = top_m_candidates[c_i];
//                _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//                idi out_degree = *out_edges++;
//                for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                    _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//                }
//                for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                    idi nb_id = out_edges[e_i];
//                    { // Sequential edition
//                        if (is_visited[nb_id]) {
//                            continue;
//                        }
//                        is_visited[nb_id] = 1;
//                    }
//
//                    auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                    dataf norm = *nb_data++;
//                    ++tmp_count_computation;
//                    distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                    if (dist > set_L[L - 1 + base_set_L].distance_) {
//                        continue;
//                    }
//
//                    Candidate cand(nb_id, dist, false);
//                    // Thread 0 maintains the "global" queue
//                    idi r = add_into_queue(
//                            set_L,
//                            base_set_L,
//                            local_queues_ends[num_threads_ - 1],
//                            L,
//                            cand);
//                    if (r < nk) {
//                        nk = r;
//                    }
//                }
//            }
//            top_m_candidates_end = 0; // Clear top_m_candidates
//            count_distance_computation_ += tmp_count_computation;
//            tmp_count_computation = 0;
//
//            if (nk <= last_k) {
//                k = nk;
//            } else {
//                k = last_k + 1;
//            }
//
//            {// Scale M
//                if (M < value_M_max) {
//                    M <<= 1;
//                } else {
//                    M = value_M_max;
//                }
//            }
//
//        }
//    }
//
//    { // Multiple Threads
//        while (k < L) {
//            ++tmp_count;
////        {//test
////            printf("tmp_count: %d\n", tmp_count);
////        }
//            // Select M candidates
//            idi last_k = L;
//// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
//            for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//                idi index_set_L = c_i + base_set_L;
//                if (set_L[index_set_L].is_checked_) {
//                    continue;
//                }
//                last_k = c_i; // Record the location of the last candidate selected.
//                set_L[index_set_L].is_checked_ = true;
//                top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
//            }
//
//
//            idi nk = L;
//            // Push M candidates' neighbors into the queue.
////#pragma omp parallel for reduction(+ : tmp_count_computation) num_threads(real_threads)
//#pragma omp parallel for reduction(+ : tmp_count_computation)
//            for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//                int tid = omp_get_thread_num();
//                idi cand_id = top_m_candidates[c_i];
//                _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//                idi out_degree = *out_edges++;
//                for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                    _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//                }
//                for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                    idi nb_id = out_edges[e_i];
//                    { // Sequential edition
//                        if (is_visited[nb_id]) {
//                            continue;
//                        }
//                        is_visited[nb_id] = 1;
//                    }
//
//                    auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                    dataf norm = *nb_data++;
//                    ++tmp_count_computation;
//                    distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                    if (dist > set_L[L - 1 + base_set_L].distance_) {
//                        continue;
//                    }
//
//                    Candidate cand(nb_id, dist, false);
//                    // Add to the local queue.
//                    if (0 != tid) {
//                        // Non-Master threads using local queues
//                        add_into_queue(
//                                set_L,
//                                (tid - 1) * local_queue_length,
//                                local_queues_ends[tid - 1],
//                                local_queue_length,
//                                cand);
//                    } else {
//                        // Thread 0 maintains the "global" queue
//                        idi r = add_into_queue(
//                                set_L,
//                                base_set_L,
//                                local_queues_ends[num_threads_ - 1],
//                                L,
//                                cand);
//                        if (r < nk) {
//                            nk = r;
//                        }
//                    }
//                }
//            }
//            top_m_candidates_end = 0; // Clear top_m_candidates
//            count_distance_computation_ += tmp_count_computation;
//            tmp_count_computation = 0;
//
//        {// Local queues' ends
////            printf("query%u:iter: %u", query_id, tmp_count);
//            idi total_elements = 0;
//            for (int i_t = 0; i_t < num_threads_ - 1; ++i_t) {
//                total_elements += local_queues_ends[i_t];
//            }
//            number_local_elements_ += total_elements;
////            printf(" total_elements: %u+%u\n", total_elements - local_queues_ends[num_threads_ - 1], local_queues_ends[num_threads_ - 1]);
////            for (int i_t = 0; i_t < num_threads_; ++i_t) {
////                printf(" [%u]: %u", i_t, local_queues_ends[i_t]);
////            }
////            printf("\n");
//        }
//
////        // Merge. Merge all queues in parallel.
//            {
//                time_merge_ -= WallTimer::get_time_mark();
//                if (num_threads_ > 1) {
//                    idi r = merge_all_queues_para_array(
//                            set_L,
//                            local_queues_ends,
//                            local_queue_length,
//                            L);
//                    if (r < nk) {
//                        nk = r;
//                    }
//                }
//                time_merge_ += WallTimer::get_time_mark();
//            }
//            if (nk <= last_k) {
//                k = nk;
//            } else {
//                k = last_k + 1;
//            }
//            {// Scale M
//                if (M < value_M_max) {
//                    M <<= 1;
//                } else {
//                    M = value_M_max;
//                }
//            }
//
//        }
//    }
//
//
//#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i + base_set_L].id_;
////        set_K[k_i] = set_L[k_i].id_;
//    }
//
//    {// Reset
////        std::fill(is_visited.begin(), is_visited.end(), 0);
//        is_visited.reset();
////        is_visited.clear_all();
//        std::fill(local_queues_ends.begin(), local_queues_ends.end(), 0);
//    }
//
////    {//test
////        if (0 == query_id) {
////            exit(1);
////        }
////    }
//}

///*
// * 6/7/2020-16:55
// * Use 1 threads to scale M until the value_M_middle.
// * Then use multiple threads.
// * Except for Thread 0, other threads are collectors. They collect, but do not merge.
// * Only merge once after Thread 0 stops.
// */
//inline void Searching::para_search_with_top_m_merge_queues_collectors(
//        const idi value_M_middle,
//        const idi value_M_max,
//        const idi query_id,
//        const idi K,
//        const idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K,
//        const idi local_queue_length, // Maximum size of local queue
//        const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
//        std::vector<idi> &local_queues_ends, // Sizes of local queue
////        std::vector<Candidate> &top_m_candidates,
//        std::vector<idi> &top_m_candidates,
////        std::vector<uint8_t> &is_visited)
//        boost::dynamic_bitset<> &is_visited)
////        std::vector<distf> &local_thresholds)
////        BitVector &is_visited)
//{
//    {
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = 1;
////            is_visited.atomic_set_bit(init_ids[c_i]);
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//#pragma omp parallel for
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    uint64_t tmp_count_computation = 0;
//    // Get the distances of all candidates, store in the set set_L.
////#pragma omp parallel for
//#pragma omp parallel for reduction(+ : tmp_count_computation)
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
////        ++count_distance_computation_;
//        ++tmp_count_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i + base_set_L] = Candidate(v_id, dist, false); // False means not checked.
////        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    count_distance_computation_ += tmp_count_computation;
//    tmp_count_computation = 0;
////    std::sort(set_L.begin(), set_L.begin() + L);
//    std::sort(
//            set_L.begin() + base_set_L,
//            set_L.begin() + base_set_L + L);
////    boost::sort::block_indirect_sort(
////            set_L.begin() + base_set_L,
////            set_L.begin() + base_set_L + L,
////            num_threads_);
//    local_queues_ends[num_threads_ - 1] = L;
//
////    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    idi M = 1;
//
//    // Single thread
//    {
//        while (k < L && M < value_M_middle) {
//            ++tmp_count;
////        {//test
////            printf("tmp_count: %d\n", tmp_count);
////        }
//
////            int real_threads = std::min(static_cast<int>(M), num_threads_);
////            idi queue_base = num_threads_ - real_threads;
//            // Select M candidates
//            idi last_k = L;
//// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
//            for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//                idi index_set_L = c_i + base_set_L;
//                if (set_L[index_set_L].is_checked_) {
//                    continue;
//                }
//                last_k = c_i; // Record the location of the last candidate selected.
//                set_L[index_set_L].is_checked_ = true;
//                top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
//            }
//
//            idi nk = L;
//            // Push M candidates' neighbors into the queue.
//            for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//                idi cand_id = top_m_candidates[c_i];
//                _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//                idi out_degree = *out_edges++;
//                for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                    _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//                }
//                for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                    idi nb_id = out_edges[e_i];
//                    { // Sequential edition
//                        if (is_visited[nb_id]) {
//                            continue;
//                        }
//                        is_visited[nb_id] = 1;
//                    }
//
//                    auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                    dataf norm = *nb_data++;
////                ++count_distance_computation_;
//                    ++tmp_count_computation;
//                    distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                    if (dist > set_L[L - 1 + base_set_L].distance_) {
//                        continue;
//                    }
//
//                    Candidate cand(nb_id, dist, false);
//                    // Thread 0 maintains the "global" queue
//                    idi r = add_into_queue(
//                            set_L,
//                            base_set_L,
//                            local_queues_ends[num_threads_ - 1],
//                            L,
//                            cand);
//                    if (r < nk) {
//                        nk = r;
//                    }
//                }
//            }
//            top_m_candidates_end = 0; // Clear top_m_candidates
//            count_distance_computation_ += tmp_count_computation;
//            tmp_count_computation = 0;
//
//            if (nk <= last_k) {
//                k = nk;
//            } else {
//                k = last_k + 1;
//            }
//
//            {// Scale M
//                if (M < value_M_max) {
//                    M <<= 1;
//                } else {
//                    M = value_M_max;
//                }
//            }
//        }
//    }
//
//    // Multiple Threads
//    {
////        while (k < L/num_threads_/2) {
//        while (k < L) {
//            ++tmp_count;
////        {//test
////            printf("tmp_count: %d\n", tmp_count);
////        }
////            int real_threads = std::min(static_cast<int>(M), num_threads_);
////            idi queue_base = num_threads_ - real_threads;
//            // Select M candidates
//            idi last_k = L;
//// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
//            for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//                idi index_set_L = c_i + base_set_L;
//                if (set_L[index_set_L].is_checked_) {
//                    continue;
//                }
//                last_k = c_i; // Record the location of the last candidate selected.
//                set_L[index_set_L].is_checked_ = true;
//                top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
//            }
//
//            idi chunk_size;
//            if (num_threads_ <= top_m_candidates_end) {
//                chunk_size = (top_m_candidates_end - 1) / num_threads_ + 1;
//            } else {
//                chunk_size = 1;
//            }
//            idi nk = L;
//            // Push M candidates' neighbors into the queue.
////#pragma omp parallel for reduction(+ : tmp_count_computation) num_threads(real_threads)
////#pragma omp parallel for reduction(+ : tmp_count_computation)
//#pragma omp parallel for reduction(+ : tmp_count_computation) schedule(static, chunk_size)
//            for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//                int tid = omp_get_thread_num();
////                {
////                    if (c_i < chunk_size && tid != 0) {
////                        printf("query_id: %u "
////                               "tmp_count: %u "
////                               "chunk_size: %u "
////                               "c_i: %u "
////                               "tid: %u\n",
////                               query_id,
////                               tmp_count,
////                               chunk_size,
////                               c_i,
////                               tid);
////                    }
////                }
//                idi cand_id = top_m_candidates[c_i];
//                _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//                idi out_degree = *out_edges++;
//                for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                    _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//                }
//                for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                    idi nb_id = out_edges[e_i];
//                    { // Sequential edition
//                        if (is_visited[nb_id]) {
//                            continue;
//                        }
//                        is_visited[nb_id] = 1;
//                    }
//
//                    auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                    dataf norm = *nb_data++;
////                ++count_distance_computation_;
//                    ++tmp_count_computation;
//                    distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                    if (dist > set_L[L - 1 + base_set_L].distance_) {
//                        continue;
//                    }
//
//                    Candidate cand(nb_id, dist, false);
//                    // Add to the local queue.
//                    if (0 != tid) {
//                        // Non-Master threads using local queues
//                        add_into_queue(
//                                set_L,
//                                (tid - 1) * local_queue_length,
//                                local_queues_ends[tid - 1],
//                                local_queue_length,
//                                cand);
//                    } else {
//                        // Thread 0 maintains the "global" queue
//                        idi r = add_into_queue(
//                                set_L,
//                                base_set_L,
//                                local_queues_ends[num_threads_ - 1],
//                                L,
//                                cand);
//                        if (r < nk) {
//                            nk = r;
//                        }
//                    }
//                }
//            }
//            top_m_candidates_end = 0; // Clear top_m_candidates
//            count_distance_computation_ += tmp_count_computation;
//            tmp_count_computation = 0;
//
//////        // Merge. Merge all queues in parallel.
////            {
////                time_merge_ -= WallTimer::get_time_mark();
////                if (num_threads_ > 1) {
//////                    idi r = merge_all_queues_queue_base(
//////                            set_L,
//////                            local_queues_ends,
//////                            queue_base,
//////                            real_threads,
//////                            local_queue_length,
//////                            L);
////                    idi r = merge_all_queues_para_array(
////                            set_L,
////                            local_queues_ends,
////                            local_queue_length,
////                            L);
////                    if (r < nk) {
////                        nk = r;
////                    }
////                }
////                time_merge_ += WallTimer::get_time_mark();
////            }
//            if (nk <= last_k) {
//                k = nk;
//            } else {
//                k = last_k + 1;
//            }
//            {// Scale M
//                if (M < value_M_max) {
//                    M <<= 1;
//                } else {
//                    M = value_M_max;
//                }
//            }
//        }
//
////        // Merge only once after Master Thread stops.
////        {
////            time_merge_ -= WallTimer::get_time_mark();
////            if (num_threads_ > 1) {
//////                    idi r = merge_all_queues_queue_base(
//////                            set_L,
//////                            local_queues_ends,
//////                            queue_base,
//////                            real_threads,
//////                            local_queue_length,
//////                            L);
////                merge_all_queues_para_array(
////                        set_L,
////                        local_queues_ends,
////                        local_queue_length,
////                        L);
////            }
////            time_merge_ += WallTimer::get_time_mark();
////        }
//    }
//
//
//#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i + base_set_L].id_;
////        set_K[k_i] = set_L[k_i].id_;
//    }
//
//    {// Reset
////        std::fill(is_visited.begin(), is_visited.end(), 0);
//        is_visited.reset();
////        is_visited.clear_all();
//        std::fill(local_queues_ends.begin(), local_queues_ends.end(), 0);
//    }
//
////    {//test
////        printf("tmp_count: %u\n", tmp_count);
////        if (3 == query_id) {
////            exit(1);
////        }
////    }
//}

///*
// * 6/8/2020-16:39
// * Selecting rather than merging
// */
//inline void Searching::para_search_with_top_m_merge_queues_selecting(
//        const idi value_M_middle,
//        const idi value_M_max,
//        const idi query_id,
//        const idi K,
//        const idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K,
//        const idi local_queue_length, // Maximum size of local queue
//        const idi base_set_L, // base_set_L = (num_threads_ - 1) * local_queue_length;
//        std::vector<idi> &local_queues_ends, // Sizes of local queue
////        std::vector<Candidate> &top_m_candidates,
//        std::vector<idi> &top_m_candidates,
////        std::vector<uint8_t> &is_visited)
//        boost::dynamic_bitset<> &is_visited)
//{
//    {
//#pragma omp parallel for
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = 1;
////            is_visited.atomic_set_bit(init_ids[c_i]);
//        }
//    }
//
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//#pragma omp parallel for
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    uint64_t tmp_count_computation = 0;
//    // Get the distances of all candidates, store in the set set_L.
////#pragma omp parallel for
//#pragma omp parallel for reduction(+ : tmp_count_computation)
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
////        ++count_distance_computation_;
//        ++tmp_count_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i + base_set_L] = Candidate(v_id, dist, false); // False means not checked.
////        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    count_distance_computation_ += tmp_count_computation;
//    tmp_count_computation = 0;
////    std::sort(set_L.begin(), set_L.begin() + L);
//    std::sort(
//            set_L.begin() + base_set_L,
//            set_L.begin() + base_set_L + L);
////    boost::sort::block_indirect_sort(
////            set_L.begin() + base_set_L,
////            set_L.begin() + base_set_L + L,
////            num_threads_);
//    local_queues_ends[num_threads_ - 1] = L;
//
////    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    idi M = 1;
//
//    // Single thread
//    {
//        while (k < L && M < value_M_middle) {
//            ++tmp_count;
////        {//test
////            printf("tmp_count: %d\n", tmp_count);
////        }
//
////            int real_threads = std::min(static_cast<int>(M), num_threads_);
////            idi queue_base = num_threads_ - real_threads;
//            // Select M candidates
//            idi last_k = L;
//// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
//            for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
//                idi index_set_L = c_i + base_set_L;
//                if (set_L[index_set_L].is_checked_) {
//                    continue;
//                }
//                last_k = c_i; // Record the location of the last candidate selected.
//                set_L[index_set_L].is_checked_ = true;
//                top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
//            }
//
//            idi nk = L;
//            // Push M candidates' neighbors into the queue.
//            for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//                idi cand_id = top_m_candidates[c_i];
//                _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//                idi out_degree = *out_edges++;
//                for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                    _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//                }
//                for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                    idi nb_id = out_edges[e_i];
//                    { // Sequential edition
//                        if (is_visited[nb_id]) {
//                            continue;
//                        }
//                        is_visited[nb_id] = 1;
//                    }
//
//                    auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                    dataf norm = *nb_data++;
////                ++count_distance_computation_;
//                    ++tmp_count_computation;
//                    distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                    if (dist > set_L[L - 1 + base_set_L].distance_) {
//                        continue;
//                    }
//
//                    Candidate cand(nb_id, dist, false);
//                    // Thread 0 maintains the "global" queue
//                    idi r = add_into_queue(
//                            set_L,
//                            base_set_L,
//                            local_queues_ends[num_threads_ - 1],
//                            L,
//                            cand);
//                    if (r < nk) {
//                        nk = r;
//                    }
//                }
//            }
//            top_m_candidates_end = 0; // Clear top_m_candidates
//            count_distance_computation_ += tmp_count_computation;
//            tmp_count_computation = 0;
//
//            if (nk <= last_k) {
//                k = nk;
//            } else {
//                k = last_k + 1;
//            }
//
//            {// Scale M
//                if (M < value_M_max) {
//                    M <<= 1;
//                } else {
//                    M = value_M_max;
//                }
//            }
//        }
//    }
//
//    // Multiple Threads
//    {
////        while (k < L/num_threads_/2) {
////        while (k < L) {
//        while (true) {
//            ++tmp_count;
////        {//test
////            printf("tmp_count: %d\n", tmp_count);
////        }
////            // Select M candidates
////            idi last_k = L;
////// Cannot use OpenMP here because this for-loop needs early break by the 2nd condition.
////            for (idi c_i = k; c_i < L && top_m_candidates_end < M; ++c_i) {
////                idi index_set_L = c_i + base_set_L;
////                if (set_L[index_set_L].is_checked_) {
////                    continue;
////                }
////                last_k = c_i; // Record the location of the last candidate selected.
////                set_L[index_set_L].is_checked_ = true;
////                top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
////            }
//
//            // Select M candidates
//            {
//                idi traverse_count = 0;
//                idi bound_sub = L; // This is not always true!
//                for (idi sub = 0; sub < bound_sub && top_m_candidates_end < M && traverse_count < L; ++sub) {
//                    for (int tid = 0; tid < num_threads_ && top_m_candidates_end < M && traverse_count < L; ++tid) {
//                        if (sub >= local_queues_ends[tid]) {
//                            continue;
//                        }
//                        idi index_set_L = tid * local_queue_length + sub;
//                        if (set_L[index_set_L].is_checked_) {
//                            continue;
//                        }
//                        set_L[index_set_L].is_checked_ = true;
//                        top_m_candidates[top_m_candidates_end++] = set_L[index_set_L].id_;
//                    }
//                }
//
//                if (0 == top_m_candidates_end) {
//                    break;
//                }
//            }
//
////            idi nk = L;
//            // Push M candidates' neighbors into the queue.
////#pragma omp parallel for reduction(+ : tmp_count_computation) num_threads(real_threads)
//#pragma omp parallel for reduction(+ : tmp_count_computation)
//            for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//                int tid = omp_get_thread_num();
//                idi cand_id = top_m_candidates[c_i];
//                _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//                idi out_degree = *out_edges++;
//                for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                    _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//                }
//                for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                    idi nb_id = out_edges[e_i];
//                    { // Sequential edition
//                        if (is_visited[nb_id]) {
//                            continue;
//                        }
//                        is_visited[nb_id] = 1;
//                    }
//
//                    auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                    dataf norm = *nb_data++;
////                ++count_distance_computation_;
//                    ++tmp_count_computation;
//                    distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                    if (dist > set_L[L - 1 + base_set_L].distance_) {
//                        continue;
//                    }
//
//                    Candidate cand(nb_id, dist, false);
//                    // Add to the local queue.
//                    if (0 != tid) {
//                        // Non-Master threads using local queues
//                        add_into_queue(
//                                set_L,
//                                (tid - 1) * local_queue_length,
//                                local_queues_ends[tid - 1],
//                                local_queue_length,
//                                cand);
//                    } else {
//                        // Thread 0 maintains the "global" queue
////                        idi r =
//                        add_into_queue(
//                                set_L,
//                                base_set_L,
//                                local_queues_ends[num_threads_ - 1],
//                                L,
//                                cand);
////                        if (r < nk) {
////                            nk = r;
////                        }
//                    }
//                }
//            }
//            top_m_candidates_end = 0; // Clear top_m_candidates
//            count_distance_computation_ += tmp_count_computation;
//            tmp_count_computation = 0;
//
////        // Merge. Merge all queues in parallel.
//            {
//                time_merge_ -= WallTimer::get_time_mark();
//                if (num_threads_ > 1) {
////                    idi r = merge_all_queues_queue_base(
////                            set_L,
////                            local_queues_ends,
////                            queue_base,
////                            real_threads,
////                            local_queue_length,
////                            L);
////                    idi r =
//                    merge_all_queues_para_array(
//                            set_L,
//                            local_queues_ends,
//                            local_queue_length,
//                            L);
////                    if (r < nk) {
////                        nk = r;
////                    }
//                }
//                time_merge_ += WallTimer::get_time_mark();
//            }
////            if (nk <= last_k) {
////                k = nk;
////            } else {
////                k = last_k + 1;
////            }
//            {// Scale M
//                if (M < value_M_max) {
//                    M <<= 1;
//                } else {
//                    M = value_M_max;
//                }
//            }
//        }
//    }
//
//
////#pragma omp parallel for
////    for (idi k_i = 0; k_i < K; ++k_i) {
////        set_K[k_i] = set_L[k_i + base_set_L].id_;
//////        set_K[k_i] = set_L[k_i].id_;
////    }
//
//    {
//        idi k_i = 0;
//        idi bound_sub = K / num_threads_;
//        for (idi sub = 0; sub < bound_sub; ++sub) {
//            for (int tid = 0; tid < num_threads_; ++tid) {
//                idi index_set_L = tid * local_queue_length + sub;
//                set_K[k_i++] = set_L[index_set_L].id_;
//            }
//        }
//        idi remain = K - k_i;
//        if (remain) {
//            for (int tid = 0; tid < remain; ++tid) {
//                idi index_set_L = tid * local_queue_length + bound_sub;
//                set_K[k_i++] = set_L[index_set_L].id_;
//            }
//        }
//    }
//
//    {// Reset
////        std::fill(is_visited.begin(), is_visited.end(), 0);
//        is_visited.reset();
////        is_visited.clear_all();
//        std::fill(local_queues_ends.begin(), local_queues_ends.end(), 0);
//    }
//
////    {//test
////        printf("tmp_count: %u\n", tmp_count);
////        if (3 == query_id) {
////            exit(1);
////        }
////    }
//}

} // namespace PANNS


#endif //BATCH_SEARCHING_SEARCHING_H
