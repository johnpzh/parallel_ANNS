//
// Created by Zhen Peng on 11/11/19.
//

#ifndef BATCH_SEARCHING_SEARCHING_H
#define BATCH_SEARCHING_SEARCHING_H

#include <vector>
#include <boost/dynamic_bitset.hpp>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <immintrin.h>
#include <cstring>
#include <unordered_set>
#include <set>
//#include <omp.h>
#include "../../include/definitions.h"
//#include "../include/efanna2e/neighbor.h"
#include "../../include/utils.h"
#include "../../include/Candidate.h"

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

    char *opt_nsg_graph_ = nullptr;
    uint64_t data_bytes_;
    uint64_t neighbor_bytes_;
    uint64_t vertex_bytes_;


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
            dataf vertex_norm) const;
//        idi dimension)
    static idi insert_into_queue(
            std::vector<Candidate> &c_queue,
            idi c_queue_top,
            Candidate cand);
//    idi insert_into_queue_nsg(
//            std::vector< Candidate > &c_queue,
//            idi c_queue_top,
//            Candidate cand);


public:
    // For Profiling
//    L3CacheMissRate cache_miss_kernel;
    uint64_t count_distance_computation = 0;

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
            idi query_id,
            idi K,
            idi L,
            std::vector<Candidate> &set_L,
//            boost::dynamic_bitset<> &is_visited,
//            boost::dynamic_bitset<> is_visited,
//            std::vector<idi> &init_ids,
            const std::vector<idi> &init_ids,
            std::vector<idi> &set_K) const;

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
    void search_with_top_m_in_batch(
            PANNS::idi M,
            PANNS::idi batch_start,
            PANNS::idi batch_size,
            PANNS::idi K,
            PANNS::idi L,
            std::vector< std::vector<Candidate> > &set_L_list,
            const std::vector<idi> &init_ids,
            std::vector< std::vector<idi> > &set_K_list);

    void load_true_NN(
            const char *filename,
            std::vector< std::vector<idi> > &true_nn_list);
    void get_recall_for_all_queries(
            const std::vector< std::vector<idi> > &true_nn_list,
            const std::vector<std::vector<unsigned>> &set_K_list,
            std::unordered_map<unsigned, double> &recalls) const;
}; // Class Searching

/**
 * Prepare init_ids and flags, as they are constant for all queries.
 * @param[out] init_ids
 * @param L
 */
inline void Searching::prepare_init_ids(
        std::vector<unsigned int> &init_ids,
        unsigned L) const
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
        dataf vertex_norm) const
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

inline void Searching::search_with_top_m(
        const PANNS::idi M,
        const PANNS::idi query_id,
        const PANNS::idi K,
        const PANNS::idi L,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K)
//        std::vector< std::vector<idi> > &top_m_list)
{
    boost::dynamic_bitset<> is_visited(num_v_);

    {
//        idi *out_edges = (idi *) (opt_nsg_graph_ + ep_ * vertex_bytes_ + data_bytes_);
//        unsigned out_degree = *out_edges++;
//        idi tmp_l = 0;
//        for (; tmp_l < L && tmp_l < out_degree; tmp_l++) {
//            init_ids[tmp_l] = out_edges[tmp_l];
//        }
//
//        for (idi i = 0; i < tmp_l; ++i) {
//            is_visited[init_ids[i]] = true;
//        }
//
//        // If ep_'s neighbors are not enough, add other random vertices
//        idi tmp_id = ep_ + 1; // use tmp_id to replace rand().
//        while (tmp_l < L) {
//            tmp_id %= num_v_;
//            unsigned id = tmp_id++;
//            if (is_visited[id]) {
//                continue;
//            }
//            is_visited[id] = true;
//            init_ids[tmp_l] = id;
//            tmp_l++;
//        }
//
////        while (tmp_l < L) {
////            unsigned id = rand() % num_v_;
////            if (is_visited[id]) {
////                continue;
////            }
////            is_visited[id] = true;
////            init_ids[tmp_l] = id;
////            tmp_l++;
////        }
        for (idi c_i = 0; c_i < L; ++c_i) {
            is_visited[init_ids[c_i]] = true;
        }
    }

    const dataf *query_data = queries_load_ + query_id  * dimension_;
    for (idi v_i = 0; v_i < L; ++v_i) {
        idi v_id = init_ids[v_i];
        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
    }
    // Get the distances of all candidates, store in the set retset.
    for (unsigned i = 0; i < L; i++) {
        unsigned v_id = init_ids[i];
        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
        dataf norm = *v_data++;
//        ++count_distance_computation;
        distf dist = compute_distance_with_norm(v_data, query_data, norm);
        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
    }
    std::sort(set_L.begin(), set_L.begin() + L);

    std::vector<idi> top_m_candidates(M);
    idi top_m_candidates_end = 0;
    idi k = 0; // Index of first unchecked candidate.
    while (k < L) {

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
//            if (top_m_candidates_end == M) {
//                break;
//            }
        }

//        if (top_m_candidates_end) {
//            std::vector<idi> tmp_top_m(top_m_candidates_end);
//            tmp_top_m.assign(top_m_candidates.begin(), top_m_candidates.begin() + top_m_candidates_end);
//            top_m_list.push_back(tmp_top_m);
//        } else {
//            break;
//        }

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
//                ++count_distance_computation;
                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
                if (dist > set_L[L-1].distance_) {
                    continue;
                }
//                if (dist >= set_L[L-1].distance_) {
//                    continue;
//                }
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

//        /////////////////////////////////////////
//        Candidate &top_cand = set_L[k];
//        if (!top_cand.is_checked_) {
//            top_cand.is_checked_ = true;
//            idi v_id = top_cand.id_; // Vertex ID.
//            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
//            idi *out_edges = (idi *) (opt_nsg_graph_ + v_id * vertex_bytes_ + data_bytes_);
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
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist >= set_L[L-1].distance_) {
//                    continue;
//                }
//                Candidate cand(nb_id, dist, false);
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
    }

    for (idi k_i = 0; k_i < K; ++k_i) {
        set_K[k_i] = set_L[k_i].id_;
    }
//
//    {//test
//        for (idi k_i = 0; k_i < K; ++k_i) {
//            printf("%u: %u %f\n", k_i, set_L[k_i].id_, set_L[k_i].distance_);
//        }
//        exit(1);
//    }
}

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
                ++count_distance_computation;
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
        std::vector<idi> ks(batch_size, 0); // Indices of every queue's first unchekced candidate.
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
                        ++count_distance_computation;
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


} // namespace PANNS


#endif //BATCH_SEARCHING_SEARCHING_H
