//
// Created by Zhen Peng on 03/15/2020.
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
    uint64_t dimension_ = 0;

    idi width_ = 0; // NSG largest degree
    idi ep_ = 0; // Start point
//    idi ep_old_ = 0;

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

    idi *common_nsg_deg_ngbrs_ = nullptr;
    edgei *common_nsg_vertex_base_ = nullptr;

    char *opt_nsg_graph_ = nullptr;
    uint64_t data_bytes_;
    uint64_t child_bytes_;
    uint64_t neighbor_bytes_;
    uint64_t vertex_bytes_;

    // For multithreads
    int num_threads_ = 1;
//    int num_real_threads_ = 1;
//    int num_threads_intra_query_ = 1;
//    int num_threads_inter_query_ = 1;
//    uint64_t thread_compuation_quota_ = 0;
//    std::vector<uint64_t> threads_computations_;
//    idi index_thresh_ = 99;

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
    static idi add_into_queue(
            std::vector<PANNS::Candidate> &queue,
            const idi queue_start,
            idi &queue_size,
            const idi queue_capacity,
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
    static idi merge_two_queues_into_1st_queue_seq_fixed(
            std::vector<Candidate> &queue1,
            const idi queue1_start,
            const idi queue1_size,
            std::vector<Candidate> &queue2,
            const idi queue2_start,
            const idi queue2_size);
    static idi merge_two_queues_into_1st_queue_seq_incr(
            std::vector<Candidate> &queue1,
            const idi queue1_start,
            idi &queue1_size, // The number of element in queue1, independent with queue1_start.
            const idi queue1_length, // The maximum capacity of queue1, independent with queue1_start.
            std::vector<Candidate> &queue2,
            const idi queue2_start,
            const idi queue2_size);
    idi merge_all_queues_to_master(
            std::vector<Candidate> &set_L,
            const std::vector<idi> &local_queues_starts,
            std::vector<idi> &local_queues_sizes,
            const idi local_queue_capacity,
            const idi L) const;
//    void get_out_degree_and_edges(
//            const idi cand_id_global,
//            idi &out_degree,
//            idi *&out_edges) const;
//    distf get_distance_to_query(
//            const idi v_id_global,
//            const dataf *query_data,
//            const dataf query_norm) const;
    idi expand_one_candidate(
            const int worker_id,
            const idi cand_id_global,
            const dataf *query_data,
            const dataf query_norm,
            const distf &dist_bound,
//            distf &dist_thresh,
            std::vector<Candidate> &set_L,
            const idi local_queue_start,
            idi &local_queue_size,
            const idi &local_queue_capacity,
            boost::dynamic_bitset<> &is_visited,
            uint64_t &local_count_computation);
    idi pick_top_m_to_workers(
//            const idi M,
            std::vector<Candidate> &set_L,
            const std::vector<idi> &local_queues_starts,
            std::vector<idi> &local_queues_sizes,
            const idi local_queue_capacity,
            const idi k_uc) const;
    void pick_top_m_unchecked(
            const idi M,
            const idi k_uc,
            std::vector<Candidate> &set_L,
            const idi local_queue_start,
            const idi local_queue_size,
            std::vector<idi> &top_m_candidates,
//        const idi top_m_candidates_start,
            idi &top_m_candidates_size,
            idi &last_k) const;
    void initialize_set_L_para(
            const dataf *query_data,
            const dataf query_norm,
            const idi L,
            std::vector<Candidate> &set_L,
            const idi set_L_start,
            idi &set_L_size,
            const std::vector<idi> &init_ids,
            boost::dynamic_bitset<> &is_visited);

public:
    // For Profiling
//    L3CacheMissRate cache_miss_kernel;
    uint64_t count_distance_computation_ = 0;
//    uint64_t count_iterations_ = 0;
    uint64_t count_merge_ = 0;
//    uint64_t count_hops_ = 0;
    double time_expand_ = 0.0;
    double time_merge_ = 0.0;
    double time_seq_ = 0.0;
    std::vector<idi> map_old_to_new_;
//    std::vector<idi> map_new_to_old_;
    idi threshold_opt_id_ = 0; // Default: only use common data structure
//    double time_pick_ = 0.0;
//    uint64_t count_full_merge_ = 0;
//    uint64_t count_iterations_ = 0;
//    idi min_iterations_ = UINT_MAX;
//    idi max_iterations_ = 0;
//    uint64_t count_threads_computation_ = 0;
//    uint64_t count_add_to_queue_ = 0;
//    uint64_t count_single_query_computation_ = 0;
//    distf dist_min_ = 0;
//    distf dist_max_ = 0;
//    double time_merge_ = 0;
//    double time_gather_ = 0;
//    double time_move_top_m_ = 0;
//    double time_full_merge_ = 0;
//    double time_select_ = 0;
//    double time_select_L_ = 0.0;
//    double time_select_M_ = 0.0;
//    double time_initialization_ = 0;
//    double time_sequential_phase_ = 0;
//    double time_parallel_phase_ = 0;
//    double time_ending_ = 0.0;
//    double time_assign_s_ = 0.0;
//    double time_expand_ = 0.0;
//    double time_pick_top_m_ = 0.0;
//    double time_distance_computation_ = 0.0;
//    double time_add_to_queue_ = 0.0;
//    double time_insert_ = 0;
//    double time_compare_minimum_ = 0;
//    double time_memmove_ = 0;
//    std::vector<double> time_memmove_list_;
//    L3CacheMissRate profile_miss_rate;
//    uint64_t number_local_elements_ = 0;
//    std::vector<idi> L_ids_;
//    std::vector<idi> M_ids_;

    ~Searching()
    {
        free(data_load_);
        free(queries_load_);
        free(opt_nsg_graph_);
        free(common_nsg_vertex_base_);
        free(common_nsg_deg_ngbrs_);
    }
    void load_data_load(const char *filename);
    void load_queries_load(const char *filename);
//    void load_nsg_graph(char *filename);
    void load_common_nsg_graph(const char *filename);
//    void build_opt_graph();
    void reorder_load_data();
    void load_nsg_and_reorder(const char *filename);
    void load_reorder_map(const char *filename);
    void load_data_and_reorder(
            const char *filename);
    void prepare_init_ids(
            std::vector<unsigned> &init_ids,
            const unsigned L) const;
    void load_true_NN(
            const char *filename,
            std::vector< std::vector< std::pair<idi, distf> > > &true_nn_list);
//            std::vector< std::vector<idi> > &true_nn_list);
    void load_true_NN_and_reorder(
            const char *filename,
            std::vector< std::vector< std::pair<idi, distf> > > &true_nn_list);
//    void reorder_true_NN(
//            const std::vector< std::vector< std::pair<idi, distf> > > &old_true_nn_list,
//            std::vector< std::vector< std::pair<idi, distf> > > &new_true_nn_list);
////            const std::vector< std::vector<idi> > &old_true_nn_list,
////            std::vector< std::vector<idi> > &new_true_nn_list);
    void get_recall_for_all_queries(
            const std::vector< std::vector< std::pair<idi, distf> > > &true_nn_list,
            const std::vector< std::vector< std::pair<idi, distf> > > &set_K_list,
//            const std::vector< std::vector<idi> > &true_nn_list,
//            const std::vector<std::vector<unsigned>> &set_K_list,
            std::unordered_map<unsigned, double> &recalls,
            const idi L) const;

//    void para_search_with_simple_v3_large_graph_scale_M(
////        const idi M,
////        const idi worker_M,
//            const idi query_id,
//            const idi K,
//            const idi L,
//            std::vector<Candidate> &set_L,
//            const std::vector<idi> &init_ids,
//            std::vector<idi> &set_K,
//            const idi local_queue_capacity, // Maximum size of local queue
//            const std::vector<idi> &local_queues_starts,
//            std::vector<idi> &local_queues_sizes, // Sizes of local queue
//            boost::dynamic_bitset<> &is_visited,
//            const idi subsearch_iterations,
//            std::vector<idi> &top_m_candidates);
    void para_search_PSS_v5_large_graph_degree_reorder(
//        const idi M,
//        const idi worker_M,
            const idi query_id,
            const idi K,
            const idi L,
            std::vector<Candidate> &set_L,
            const std::vector<idi> &init_ids,
            std::vector< std::pair<idi, distf> > &set_K,
//            std::vector<idi> &set_K,
            const idi local_queue_capacity, // Maximum size of local queue
            const std::vector<idi> &local_queues_starts,
            std::vector<idi> &local_queues_sizes, // Sizes of local queue
            boost::dynamic_bitset<> &is_visited,
            const idi subsearch_iterations);
//            std::vector<idi> &top_m_candidates);

}; // Class Searching



} // namespace PANNS


#endif //BATCH_SEARCHING_SEARCHING_H
