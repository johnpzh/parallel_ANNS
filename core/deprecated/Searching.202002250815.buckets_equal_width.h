//
// Created by Zhen Peng on 02/25/2020.
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
#include <algorithm>
#include <omp.h>
#include "../include/definitions.h"
//#include "../include/efanna2e/neighbor.h"
#include "../include/utils.h"
#include "../include/Candidate.h"
#include "../include/parallelization.h"
//#include "../include/Buckets.h"

namespace PANNS {

class Searching {
//private:
public:
    idi num_v_ = 0;
    edgei num_e_ = 0;
    idi num_queries_ = 0;
    int dimension_ = 0;
//    uint64_t dimension_ = 0;

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
            const Candidate &cand);
    static idi add_into_queue(
            std::vector<PANNS::Candidate> &queue,
            idi &queue_top,
            const idi queue_size,
            const PANNS::Candidate &cand);
//    idi insert_into_queue_nsg(
//            std::vector< Candidate > &c_queue,
//            idi c_queue_top,
//            Candidate cand);
//    template<typename T>
    static void insert_one_element_at(
//        const T &cand,
//        T *queue_base,
            const Candidate &cand,
            std::vector<Candidate> &queue_base,
            const idi insert_index,
            const idi queue_start,
            const idi queue_size);
    static idi merge_two_queues_into_1st_queue_seq(
            std::vector<Candidate> &queue1,
            const idi queue1_start,
            const idi queue1_size,
            std::vector<Candidate> &queue2,
            const idi queue2_start,
            const idi queue2_size);
//            const idi limit_size);
    static idi merge_two_queues_into_1st_queue_para(
            std::vector<Candidate> &queue1,
            const idi queue1_start,
            const idi queue1_size,
            std::vector<Candidate> &queue2,
            const idi queue2_start,
            const idi queue2_size);
    static void merge_two_bkt_arrays_into_1st_bkt_array_seq(
            std::vector< std::vector<Candidate> > &a_array,
//            idi a_bound,
            std::vector< std::vector<Candidate> > &b_array,
            const idi b_bound);
    static idi add_into_CandidatesBuckets(
            const Candidate &cand,
            std::vector< std::vector<Candidate> > &buckets,
            const distf bucket_lower,
            const distf overall_width,
            const distf bucket_width);
    idi merge_all_queues_para(
            std::vector< std::vector<Candidate> > &local_queues_list,
            std::vector<idi> &local_queues_ends,
            std::vector<Candidate> &set_L,
            const idi L);
    idi merge_all_buckets_para(
            std::vector< std::vector< std::vector<Candidate> > > &local_buckets_list,
            std::vector<idi> &local_insert_locations,
            std::vector< std::vector<Candidate> > &global_buckets,
            const idi num_buckets);

public:
    // For Profiling
//    L3CacheMissRate cache_miss_kernel;
    uint64_t count_distance_computation_ = 0;


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
            std::vector<idi> &set_K);

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
    void para_search_with_top_m(
            idi M,
            idi query_id,
            idi K,
            idi L,
            std::vector<Candidate> &set_L,
            const std::vector<idi> &init_ids,
            std::vector<idi> &set_K);
//    void para_prepare_init_ids(
//            std::vector<unsigned> &init_ids,
//            unsigned L) const;

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
    recalls[5] = 0.0;
    recalls[10] = 0.0;
    recalls[20] = 0.0;
    recalls[50] = 0.0;
    recalls[100] = 0.0;
    for (unsigned q_i = 0; q_i < num_queries_; ++q_i) {
//        size_t offset = q_i * t_K;
        auto &set_K_q_i = set_K_list[q_i];
        for (unsigned top_i = 0; top_i < 100; ++top_i) {
            unsigned true_id = true_nn_list[q_i][top_i];
            for (unsigned n_i = 0; n_i < 100; ++n_i) {
                if (set_K_q_i[n_i] == true_id) {
                    if (n_i < 5) recalls[5] += 1;
                    if (n_i < 10) recalls[10] += 1;
                    if (n_i < 20) recalls[20] += 1;
                    if (n_i < 50) recalls[50] += 1;
                    if (n_i < 100) recalls[100] += 1;
                }
            }
        }
    }
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
//        boost::dynamic_bitset<> &is_visited,
//        boost::dynamic_bitset<> is_visited,
//        std::vector<idi> &init_ids,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K)
{
//    std::vector<Candidate> set_L(L+1);
//    std::vector<idi> init_ids(L);
    boost::dynamic_bitset<> is_visited(num_v_);

    for (idi v_i = 0; v_i < L; ++v_i) {
        is_visited[init_ids[v_i]] = true;
    }
//    {
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
//    }

//    const std::vector<dataf> &query = queries_load_[query_id];
//    std::vector<char> is_checked(L + 1, 0);
//    boost::dynamic_bitset<> is_checked(num_v_);
//    cache_miss_kernel.measure_stop();
//    cache_miss_kernel.measure_start();
    const dataf *query_data = queries_load_ + query_id  * dimension_;

    for (idi v_i = 0; v_i < L; ++v_i) {
        idi v_id = init_ids[v_i];
//        _mm_prefetch(reinterpret_cast<char *>(data_load_ + v_id * dimension_), _MM_HINT_T0);
        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
    }
    // Get the distances of all candidates, store in the set set_L.
    for (unsigned i = 0; i < L; i++) {
        unsigned v_id = init_ids[i];
        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
        dataf norm = *v_data++;
//        ++count_distance_computation_;
        distf dist = compute_distance_with_norm(v_data, query_data, norm);
        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
    }
    std::sort(set_L.begin(), set_L.begin() + L);
//    cache_miss_kernel.measure_stop();
//    cache_miss_kernel.measure_start();
    idi k = 0; // Index of every queue's first unchecked candidate.
    while (k < L) {
        Candidate &top_cand = set_L[k];
        unsigned nk = L;
        if (!top_cand.is_checked_) {
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
//                ++count_distance_computation_;
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
}

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
        const PANNS::Candidate &cand)
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

//template<typename T>
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

inline idi Searching::merge_two_queues_into_1st_queue_seq(
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

//    auto *queue1_base = queue1.data() + queue1_start;
    // Insert the 1st of queue2
    insert_one_element_at(
            queue2[queue2_start],
//            queue1.data(),
            queue1,
            insert_index,
            queue1_start,
            queue1_size);
    if (queue2_size == 1) {
        return insert_index;
    }
//    memmove(reinterpret_cast<char *>(queue1_base + insert_index + 1),
//            reinterpret_cast<char *>(queue1_base + insert_index),
//            (queue1_size - insert_index) * sizeof(Candidate));
//    queue1[insert_index] = queue2[queue2_start];

    // Insert
    idi q_i_1 = insert_index + 1 + queue1_start;
    idi q_i_2 = queue2_start + 1;
    const idi q_i_1_bound = queue1_start + queue1_size;
    const idi q_i_2_bound = queue2_start + queue2_size;
//    const idi insert_i_bound = queue1_start + limit_size;
    for (idi insert_i = insert_index + 1; insert_i < queue1_size; ++insert_i) {
//    for (idi insert_i = insert_index + 1; insert_i < q_i_1_bound; ++insert_i) {
        if (q_i_1 >= q_i_1_bound || q_i_2 >= q_i_2_bound) {
            // queue1 or queue2 finished traverse. Rest o
            break;
        } else if (queue1[q_i_1] < queue2[q_i_2]) {
            ++q_i_1;
        } else {
            // Insert queue2[q_i_2] into queue1
            insert_one_element_at(
                    queue2[q_i_2++],
//                    queue1.data(),
                    queue1,
                    insert_i,
                    queue1_start,
                    queue1_size);
            ++q_i_1;
        }
    }
//
//    // Merge queue1 and queue2 into tmp_queue.
//    std::vector<Candidate> tmp_queue(queue1_size + queue2_size);
//    std::merge(queue1.begin() + queue1_start,
//            queue1.begin() + queue1_start + queue1_size,
//            queue2.begin() + queue2_start,
//            queue2.begin() + queue2_start + queue2_size,
//            tmp_queue.begin());
//    // Resize tmp_queue.
//    tmp_queue.resize(limit_size);
//
//    // Swap queue1 and tmp_queue
//    queue1.swap(tmp_queue);

    return insert_index;
}

//// Backup
//inline idi Searching::merge_two_queues_into_1st_queue_seq(
//        std::vector<Candidate> &queue1,
//        const idi queue1_start,
//        const idi queue1_size,
//        std::vector<Candidate> &queue2,
//        const idi queue2_start,
//        const idi queue2_size,
//        const idi limit_size)
//{
//    assert(queue1_size);
//    // Record the lowest insert location.
//    auto it_loc = std::lower_bound(
//            queue1.begin() + queue1_start,
//            queue1.begin() + queue1_start + queue1_size,
//            queue2[queue2_start]);
//    idi insert_loc = it_loc - (queue1.begin() + queue1_start);
//
//    // Merge queue1 and queue2 into tmp_queue.
//    std::vector<Candidate> tmp_queue(queue1_size + queue2_size);
//    std::merge(queue1.begin() + queue1_start,
//            queue1.begin() + queue1_start + queue1_size,
//            queue2.begin() + queue2_start,
//            queue2.begin() + queue2_start + queue2_size,
//            tmp_queue.begin());
//    // Resize tmp_queue.
//    tmp_queue.resize(limit_size);
//
//    // Swap queue1 and tmp_queue
//    queue1.swap(tmp_queue);
//
//    return insert_loc;
//}

inline idi Searching::merge_two_queues_into_1st_queue_para(
        std::vector<Candidate> &queue1,
        const idi queue1_start,
        const idi queue1_size,
        std::vector<Candidate> &queue2,
        const idi queue2_start,
        const idi queue2_size)
//        const idi limit_size)
{
    assert(queue1_size && queue2_size);

    if (queue2_size < 16) {
//    if (true) {
//    if (queue2_size < 1024) {
        return merge_two_queues_into_1st_queue_seq(
                queue1,
                queue1_start,
                queue1_size,
                queue2,
                queue2_start,
                queue2_size);
    } else {
        auto it1_begin = queue1.begin() + queue1_start;
        auto it1_end = it1_begin + queue1_size;
//    auto it2_begin = queue2.begin() + queue2_start;
//    auto it2_end = it2_begin + queue2_size;

        // Record the lowest insert location.
        idi lowest_insert_loc;
        {
            auto it_loc = std::lower_bound(
                    it1_begin,
                    it1_end,
                    queue2[queue2_start]);
            lowest_insert_loc = it_loc - it1_begin;
            if (lowest_insert_loc == queue1_size) {
                return lowest_insert_loc;
            } else if (lowest_insert_loc == queue1_size - 1) {
                queue1[queue1_start + lowest_insert_loc] = queue2[queue2_start];
                return lowest_insert_loc;
            }
        }
        // Partition queue2 and queue1
        const idi stride = log2(static_cast<double>(queue2_size));
        const idi num_partitions = (queue2_size - 1) / stride + 1;
        // Partitions in queue1
        std::vector<idi> parts_in_q1(num_partitions);
        parts_in_q1[0] = 0;
#pragma omp parallel for
        for (idi p_i = 1; p_i < num_partitions; ++p_i) {
            idi q_i = p_i * stride + queue2_start;
            auto it_loc = std::lower_bound(
                    it1_begin,
                    it1_end,
                    queue2[q_i]);
            parts_in_q1[p_i] = it_loc - it1_begin;
        }
        // Partitions in queue2
        std::vector<idi> parts_in_q2(num_partitions);
        parts_in_q2[0] = 0;
#pragma omp parallel for
        for (idi p_i = 1; p_i < num_partitions; ++p_i) {
            parts_in_q2[p_i] = p_i * stride;
        }

        // Merge
        // Find partitions needed.
        std::vector<idi> offsets_queue3; // For the tmp_queue when merging.
        offsets_queue3.push_back(0);
        idi elements_count = 0;
        idi p_i_bound = 0;
        while (elements_count < queue1_size && p_i_bound < num_partitions) {
            idi q1_size;
            idi q2_size;
            if (p_i_bound != num_partitions - 1) {
                q1_size = parts_in_q1[p_i_bound + 1] - parts_in_q1[p_i_bound];
                q2_size = parts_in_q2[p_i_bound + 1] - parts_in_q2[p_i_bound];
            } else {
                q1_size = queue1_size - parts_in_q1[p_i_bound];
                q2_size = queue2_size - parts_in_q2[p_i_bound];
            }
            ++p_i_bound;
            idi tmp_size = q1_size + q2_size;
            elements_count += tmp_size;
            offsets_queue3.push_back(*offsets_queue3.rbegin() + tmp_size);
        }
        // Merge into tmp_queue in parallel
        std::vector<Candidate> tmp_queue(elements_count);
#pragma omp parallel for
        for (idi p_i = 0; p_i < p_i_bound; ++p_i) {
            idi q1_start = parts_in_q1[p_i] + queue1_start;
            idi q2_start = parts_in_q2[p_i] + queue2_start;
            idi q1_end;
            idi q2_end;
            if (p_i != num_partitions - 1) {
                q1_end = parts_in_q1[p_i + 1] + queue1_start;
                q2_end = parts_in_q2[p_i + 1] + queue1_start;
            } else {
                q1_end = queue1_size + queue1_start;
                q2_end = queue2_size + queue2_start;
            }
            std::merge(
                    queue1.begin() + q1_start,
                    queue1.begin() + q1_end,
                    queue2.begin() + q2_start,
                    queue2.begin() + q2_end,
                    tmp_queue.begin() + offsets_queue3[p_i]);
        }
        if (elements_count > queue1_size) {
            tmp_queue.resize(queue1_size);
        }
        tmp_queue.swap(queue1);
//// Deprecated. Wrong implementation.
////#pragma omp parallel for
//        for (idi p_i = 0; p_i < num_partitions; ++p_i) {
//            idi q1_start = parts_in_q1[p_i] + queue1_start;
//            idi q2_start = parts_in_q2[p_i] + queue2_start;
//            idi q1_size;
//            idi q2_size;
//            if (p_i != num_partitions - 1) {
//                q1_size = parts_in_q1[p_i + 1] - parts_in_q1[p_i];
//                q2_size = parts_in_q2[p_i + 1] - parts_in_q2[p_i];
//            } else {
//                q1_size = queue1_size - parts_in_q1[p_i];
//                q2_size = queue2_size - parts_in_q2[p_i];
//            }
//            if (0 == q1_size || 0 == q2_size) continue;
//            merge_two_queues_into_1st_queue_seq(
//                    queue1,
//                    q1_start,
//                    q1_size,
//                    queue2,
//                    q2_start,
//                    q2_size);
//        }
        return lowest_insert_loc;
    }


//    //////////////////////////////////////////
//    //// Backup
//    // Record the lowest insert location.
//    auto it_loc = std::lower_bound(
//            queue1.begin() + queue1_start,
//            queue1.begin() + queue1_start + queue1_size,
//            queue2[queue2_start]);
//    idi insert_loc = it_loc - (queue1.begin() + queue1_start);
//
//    auto *queue1_base = queue1.data() + queue1_start;
//    // Insert the 1st of queue2
//    insert_one_element_at(
//            queue2[queue2_start],
//            queue1_base,
//            insert_loc,
//            queue1_size);
//
//    // Insert
//    idi q_i_1 = insert_loc + 1;
//    idi q_i_2 = queue2_start + 1;
//    const idi q_i_1_bound = queue1_start + queue1_size;
//    const idi q_i_2_bound = queue2_start + queue2_size;
////    const idi insert_i_bound = queue1_start + limit_size;
//    for (idi insert_i = insert_loc + 1; insert_i < q_i_1_bound; ++insert_i) {
//        if (q_i_1 >= q_i_1_bound || q_i_2 >= q_i_2_bound) {
//            // queue1 or queue2 finished traverse. Rest o
//            break;
//        } else if (queue1[q_i_1] < queue2[q_i_2]) {
//            ++q_i_1;
//        } else {
//            // Insert queue2[q_i_2] into queue1
//            insert_one_element_at(
//                    queue2[q_i_2++],
//                    queue1_base,
//                    insert_i,
//                    queue1_size);
//            ++q_i_1;
//        }
//    }
//
//    return insert_loc;
}

inline void Searching::merge_two_bkt_arrays_into_1st_bkt_array_seq(
        std::vector< std::vector<Candidate> > &a_array,
//        const idi a_bound,
        std::vector< std::vector<Candidate> > &b_array,
        const idi b_bound)
{
//    idi a_size = a_bucket.size();
//    idi b_size = b_bucket.size();
//    a_bucket.resize(a_size + b_size);
//    std::memmove(a_bucket.data() + a_size, b_bucket.data(), b_size * sizeof(b_bucket[0]));

    for (idi bk_i = 0; bk_i < b_bound; ++bk_i) {
//            for (idi bk_i = 0; bk_i < num_buckets; ++bk_i) {
        auto &b_bucket = b_array[bk_i];
        if (b_bucket.empty())
            continue;
        auto &a_bucket = a_array[bk_i];
//                a_bucket.insert(a_bucket.end(), b_bucket.begin(), b_bucket.end()); // TODO: memory optimization needed.
//                {
        idi a_size = a_bucket.size();
        idi b_size = b_bucket.size();
        a_bucket.resize(a_size + b_size);
        std::memmove(a_bucket.data() + a_size, b_bucket.data(), b_size * sizeof(b_bucket[0]));
//                }
        b_bucket.clear();
    }
//    if (a_bound < b_bound) {
//        a_bound = b_bound;
//    }
//    b_bound = 0;
}

inline idi Searching::add_into_CandidatesBuckets(
        const Candidate &cand,
        std::vector< std::vector<Candidate> > &buckets,
        const distf bucket_lower,
        const distf overall_width,
        const distf bucket_width)
{
    idi b_i;
    if (cand.distance_ < bucket_lower) {
        b_i = 0;
//        buckets[0].push_back(cand);
    } else {
        b_i = (cand.distance_ - bucket_lower) / overall_width / bucket_width;
//        buckets[b_i].push_back(cand);
    }
    buckets[b_i].push_back(cand);
    return b_i;
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
    // Get the distances of all candidates, store in the set set_L.
    for (unsigned i = 0; i < L; i++) {
        unsigned v_id = init_ids[i];
        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
        dataf norm = *v_data++;
//        ++count_distance_computation_;
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
//                ++count_distance_computation_;
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
//            printf("%u: %u: %u %f\n",
//                   query_id,
//                    k_i, set_L[k_i].id_, set_L[k_i].distance_);
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
//                ++count_distance_computation;
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
//                        ++count_distance_computation;
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



inline idi Searching::merge_all_buckets_para(
        std::vector< std::vector< std::vector<Candidate> > > &local_buckets_list,
        std::vector<idi> &local_insert_locations,
        std::vector< std::vector<Candidate> > &global_buckets,
        const idi num_buckets)
{
    idi num_arrays = num_threads_;
    idi size = 1 << (static_cast<idi>(log2(num_arrays)));
    idi log2size = static_cast<idi>(log2(size));
    for (idi d = 0; d < log2size; ++d) {
        uint32_t by = 1 << (d + 1);
#pragma omp parallel for
        for (idi i = 0; i < size; i += by) {
            idi ai = i + (1 << (d + 1)) - 1;
            idi bi = i + (1 << d) - 1;
            auto &a_array = local_buckets_list[ai];
            auto &b_array = local_buckets_list[bi];
            idi &a_bound = local_insert_locations[ai];
            idi &b_bound = local_insert_locations[bi];
////#pragma omp parallel for
//// Fine-grained is slower.
//            for (idi bk_i = 0; bk_i < b_bound; ++bk_i) {
////            for (idi bk_i = 0; bk_i < num_buckets; ++bk_i) {
//                auto &b_bucket = b_array[bk_i];
//                if (b_bucket.empty())
//                    continue;
//                auto &a_bucket = a_array[bk_i];
//                merge_two_buckets_into_1st_bucket_set(a_bucket, b_bucket);
////                a_bucket.insert(a_bucket.end(), b_bucket.begin(), b_bucket.end()); // TODO: memory optimization needed.
////                {
////                    idi a_size = a_bucket.size();
////                    idi b_size = b_bucket.size();
////                    a_bucket.resize(a_size + b_size);
////                    std::memmove(a_bucket.data() + a_size, b_bucket.data(), b_size * sizeof(b_bucket[0]));
////                }
//                b_bucket.clear();
//            }
//            if (a_bound < b_bound) {
//                a_bound = b_bound;
//            }
//            b_bound = num_threads_;
            merge_two_bkt_arrays_into_1st_bkt_array_seq(
                    a_array,
//                    a_bound,
                    b_array,
                    num_buckets);
//                    b_bound);
            if (a_bound < b_bound) {
                a_bound = b_bound;
            }
            b_bound = 0;
        }
    }
    if (size != num_arrays) {
        for (idi a_i = size; a_i < num_arrays; ++a_i) {
            idi ai = a_i;
            idi bi = ai - 1;
            auto &a_array = local_buckets_list[ai];
            auto &b_array = local_buckets_list[bi];
            idi &a_bound = local_insert_locations[ai];
            idi &b_bound = local_insert_locations[bi];
////#pragma omp parallel for
//// Fine-grained is slower.
//            for (idi bk_i = 0; bk_i < b_bound; ++bk_i) {
////            for (idi bk_i = 0; bk_i < num_buckets; ++bk_i) {
//                auto &b_bucket = b_array[bk_i];
//                if (b_bucket.empty())
//                    continue;
//                auto &a_bucket = a_array[bk_i];
//                merge_two_buckets_into_1st_bucket_set(a_bucket, b_bucket);
////                a_bucket.insert(a_bucket.end(), b_bucket.begin(), b_bucket.end()); // TODO: memory optimization needed.
////                {
////                    idi a_size = a_bucket.size();
////                    idi b_size = b_bucket.size();
////                    a_bucket.resize(a_size + b_size);
////                    std::memmove(a_bucket.data() + a_size, b_bucket.data(), b_size * sizeof(b_bucket[0]));
////                }
//                b_bucket.clear();
//            }
//            if (a_bound < b_bound) {
//                a_bound = b_bound;
//            }
//            b_bound = num_threads_;
            merge_two_bkt_arrays_into_1st_bkt_array_seq(
                    a_array,
//                    a_bound,
                    b_array,
                    num_buckets);
//                    b_bound);
            if (a_bound < b_bound) {
                a_bound = b_bound;
            }
            b_bound = 0;
        }
    }

    // Merge into the global_buckets.
    idi first_bucket = 0;
    auto &b_array = *local_buckets_list.rbegin();
    while (first_bucket < num_buckets && b_array[first_bucket].size() == 0) {
        ++first_bucket;
    }
    idi &b_bound = *local_insert_locations.rbegin();
////#pragma omp parallel for
//// Fine-grained is slower.
//    for (idi bk_i = 0; bk_i < b_bound; ++bk_i) {
////    for (idi bk_i = 0; bk_i < num_buckets; ++bk_i) {
//        auto &b_bucket = b_array[bk_i];
//        if (b_bucket.empty())
//            continue;
//        auto &a_bucket = global_buckets[bk_i];
//        merge_two_buckets_into_1st_bucket_set(a_bucket, b_bucket);
////        a_bucket.insert(a_bucket.end(), b_bucket.begin(), b_bucket.end());
////        {
////            idi a_size = a_bucket.size();
////            idi b_size = b_bucket.size();
////            a_bucket.resize(a_size + b_size);
////            std::memmove(a_bucket.data() + a_size, b_bucket.data(), b_size * sizeof(b_bucket[0]));
////        }
//        b_bucket.clear();
//    }
//    b_bound = num_threads_;

    merge_two_bkt_arrays_into_1st_bkt_array_seq(
            global_buckets,
            b_array,
            num_buckets);
//            b_bound);
    b_bound = 0;

    return first_bucket;
}

inline idi Searching::merge_all_queues_para(
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
        r = merge_two_queues_into_1st_queue_seq(
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


//// Using local buckets and then merge.
inline void Searching::para_search_with_top_m(
        const PANNS::idi M,
        const PANNS::idi query_id,
        const PANNS::idi K,
        const PANNS::idi L,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K)
//        std::vector< std::vector<idi> > &top_m_list)
{
//    const idi local_queue_length = ((M - 1) / num_threads_ + 1) * width_;
//    const idi local_queue_length = L;
//    std::vector< std::vector<Candidate> > local_queues_list(num_threads_, std::vector<Candidate>(local_queue_length));
//    std::vector<idi> local_queues_ends(num_threads_, 0);
//    boost::dynamic_bitset<> is_visited(num_v_);
    std::vector<uint8_t> is_visited(num_v_, 0);

    {
#pragma omp parallel for
        for (idi c_i = 0; c_i < L; ++c_i) {
            is_visited[init_ids[c_i]] = 1;
        }
    }
    const dataf *query_data = queries_load_ + query_id  * dimension_;
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
    // Get the distances of all candidates, store in the set set_L.
    distf dist_lower = DISTF_MAX;
    distf dist_upper = DISTF_MIN;
#pragma omp parallel for reduction(max : dist_upper) reduction(min : dist_lower)
    for (unsigned i = 0; i < L; i++) {
        unsigned v_id = init_ids[i];
        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
        dataf norm = *v_data++;
//        ++count_distance_computation;
        distf dist = compute_distance_with_norm(v_data, query_data, norm);
        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
        dist_lower = std::min(dist_lower, dist);
        dist_upper = std::max(dist_upper, dist);
//        if (dist < dist_lower) {
//            dist_lower = dist;
//        }
//        else if (dist > dist_upper) {
////        if (dist > dist_upper) {
//            dist_upper = dist;
//        }
    }
//    std::sort(set_L.begin(), set_L.begin() + L);

    // Compute the width for buckets
    const idi num_buckets = 64;
    const distf bucket_width = 1.0 / num_buckets;

//    CandidatesBuckets global_buckets(num_buckets, set_L.begin()->distance_, set_L.rbegin()->distance_);

    std::vector< std::vector<Candidate> > global_buckets(num_buckets);
    std::vector< std::vector< std::vector<Candidate> > > local_buckets_list(
            num_threads_,
            std::vector< std::vector<Candidate> >(num_buckets));
    std::vector<idi> local_insert_locations(num_threads_, 0); // Record a local array's highest bucket id that has new insertion.
    distf bucket_lower = dist_lower;
    distf bucket_upper = dist_upper >= 0 ? dist_upper + 1 : dist_upper - 1; // TODO: is it proper?
//    distf bucket_lower = set_L.begin()->distance_;
//    distf bucket_upper = set_L.rbegin()->distance_;
    distf overall_width = bucket_upper - bucket_lower;


    // Copy set_L into global_bucket.
    for (idi q_i = 0; q_i < L; ++q_i) {
//        idi b_i = (set_L[q_i].distance_ - bucket_lower) / overall_width / bucket_width;
//        global_buckets[b_i].push_back(set_L[q_i]);
        add_into_CandidatesBuckets(
                set_L[q_i],
                global_buckets,
                bucket_lower,
                overall_width,
                bucket_width);
    }


    std::vector<idi> top_m_candidates(M);
    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
    idi b_index = 0; //  Index of the first bucket that contains unchecked candidates.
    bool is_finished = false;
    idi tmp_count = 0; // for debug
//    while (k < L) {
    while (!is_finished) {
        ++tmp_count;

        // Select M candidates
        idi last_b = num_buckets; // The last index of the bucket that contains unchecked candidates. Used for updating b_index.
        for (idi b_i = b_index; b_i < num_buckets && top_m_candidates_end < M; ++b_i) {
            // Traverse bucket b_i
            auto &bucket_i = global_buckets[b_i];
            idi q_i_bound = bucket_i.size();
            for (idi q_i = 0; q_i < q_i_bound && top_m_candidates_end < M; ++q_i) {
                if (bucket_i[q_i].is_checked_) {
                    continue;
                }
                last_b = b_i;
                bucket_i[q_i].is_checked_ = true;
                top_m_candidates[top_m_candidates_end++] = bucket_i[q_i].id_;
            }
        }
//// Backup
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
////

        // Push M candidates' neighbors into the queue.
#pragma omp parallel for
        for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
            int tid = omp_get_thread_num();
            idi cand_id = top_m_candidates[c_i];
            idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
            idi out_degree = *out_edges++;
            for (idi e_i = 0; e_i < out_degree; ++e_i) {
                idi nb_id = out_edges[e_i];
//                if (is_visited[nb_id]) {
//                    continue;
//                }
//                is_visited[nb_id] = 1;
                if (!AtomicOps::CAS(is_visited.data() + nb_id,
                        static_cast<uint8_t>(0),
                        static_cast<uint8_t>(1))) {
                    continue;
                }

                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
                dataf norm = *nb_data++;
//                ++count_distance_computation;
                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
                if (dist > dist_upper) {
                    continue;
                }
                Candidate cand(nb_id, dist, false);
                // Add to the local buckets.
                idi r = add_into_CandidatesBuckets(
                        cand,
                        local_buckets_list[tid],
                        bucket_lower,
                        overall_width,
                        bucket_width);
                if (r > local_insert_locations[tid]) {
                    local_insert_locations[tid] = r;
                }
//                idi b_i = (cand.distance_ - bucket_lower) / overall_width / bucket_width;
//                local_buckets_list[tid][b_i].push_back(cand);
//                // Add to the local queue.
//                add_into_queue(local_queues_list[tid], local_queues_ends[tid], local_queue_length, cand);
            }
        }
        top_m_candidates_end = 0; // Clear top_m_candidates


        idi lowest_b_i = num_buckets; // The largest index of the bucket that has new candidates
        // Merge buckets. Merge all local buckets in parallel.
        {
//            if (num_threads_ > 1) {
                lowest_b_i = merge_all_buckets_para(
                        local_buckets_list,
                        local_insert_locations,
                        global_buckets,
                        num_buckets);
//            } else {
//                auto &b_array = local_buckets_list[0];
//                while (b_array[lowest_b_i].size() == 0) {
//                    ++lowest_b_i;
//                }
//                for (idi bk_i = lowest_b_i; bk_i < num_buckets; ++bk_i) {
//                    auto &b_bucket = b_array[bk_i];
//                    if (b_bucket.empty())
//                        continue;
//                    auto &a_bucket = global_buckets[bk_i];
//                    a_bucket.insert(a_bucket.end(), b_bucket.begin(), b_bucket.end());
//                }
//            }
        }
        {// Update the start bucket.
            if (lowest_b_i <= last_b) {
                b_index = lowest_b_i;
            } else {
                b_index = last_b + 1;
            }
            // TODO: dynamic buckets width.
        }
        {// Check the converge condition
            if (b_index >= num_buckets) {
                is_finished = true;
            } else {
                idi fixed_count = 0;
                for (idi b_i = 0; b_i < b_index && fixed_count < L; ++b_i) {
                    fixed_count += global_buckets[b_i].size();
                }
                if (fixed_count >= L) {
                    is_finished = true;
                }
//                else {
//                    // Update dist_upper
//                    idi b_i = b_index;
//                    for ( ; b_i < num_buckets && fixed_count < L; ++b_i) {
//                        fixed_count += global_buckets[b_i].size();
//                    }
//                    distf tmp_upper = b_i * bucket_width * overall_width + bucket_lower;
//                    if (tmp_upper < dist_upper) {
//                        dist_upper = tmp_upper;
//                    }
//                }
            }
        }
//        {//test
//            // Print global_buckets' sizes
//            printf("sizes:");
//            for (idi b_i = 0; b_i < num_buckets; ++b_i) {
//                printf(" %u:%lu",
//                        b_i, global_buckets[b_i].size());
//            }
//            printf("\n");
//        }
//        idi nk = L;
//        // Merge. Parallel merging in every two queues.
//        {
//            for (int tid = 0; tid < num_threads_; ++tid) {
//                if (0 == local_queues_ends[tid]) continue;
//                idi r = merge_two_queues_into_1st_queue_para(
//                        set_L,
//                        0,
//                        L,
//                        local_queues_list[tid],
//                        0,
//                        local_queues_ends[tid]);
////                idi r = merge_two_queues_into_1st_queue_seq(
////                        set_L,
////                        0,
////                        L,
////                        local_queues_list[tid],
////                        0,
////                        local_queues_ends[tid]);
//                local_queues_ends[tid] = 0; // Reset the local queue
//                if (r < nk) {
//                    nk = r;
//                }
//            }
//        }
//        // Merge. Merge all queues in parallel.
//        {
//            if (num_threads_ > 1) {
//                idi r = merge_all_queues_para(
//                        local_queues_list,
//                        local_queues_ends,
//                        set_L,
//                        L);
//                if (r < nk) {
//                    nk = r;
//                }
//            } else {
//                if (local_queues_ends[0]) {
//                    idi r = merge_two_queues_into_1st_queue_seq(
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
//        if (nk <= last_k) {
//            k = nk;
//        } else {
//            k = last_k + 1;
//        }
    }

//#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
    {// Move to set_K
        idi count = 0;
        idi last_b = 0;
        for ( ; last_b < num_buckets && count < K; ++last_b) {
            auto &bucket_i = global_buckets[last_b];
            count += bucket_i.size();
            std::sort(bucket_i.begin(), bucket_i.end());
        }
        idi k_i = 0;
        for (idi b_i = 0; b_i < last_b && k_i < K; ++b_i) {
            const auto &bucket_i = global_buckets[b_i];
            const idi size_b = bucket_i.size();
            for (idi q_i = 0; q_i < size_b && k_i < K; ++q_i) {
                set_K[k_i++] = bucket_i[q_i].id_;
            }
        }
    }
//    {//// DEPRECATED. Cannot guarantee the accuracy.
//        idi count = 0;
//        for (idi b_i = 0; b_i < num_buckets && count < K; ++b_i) {
//            auto &bucket_i = global_buckets[b_i];
//            idi size_b = bucket_i.size();
//            if (count + size_b >= K) {
//                idi remain = K - count;
////                std::copy_n(set_L.begin() + count, remain, bucket_i.begin());
//                std::memmove(set_L.data() + count, bucket_i.data(), remain * sizeof(Candidate));
//                count = K;
//            } else {
////                std::copy_n(set_L.begin() + count, size_b, bucket_i.begin());
//                std::memmove(set_L.data() + count, bucket_i.data(), size_b * sizeof(Candidate));
//                count += size_b;
//            }
//        }
//    }
//
//    {//test
//        for (idi k_i = 0; k_i < K; ++k_i) {
//            printf("query_id: %u "
//                   "set_L[%u]: "
//                   "id: %u "
//                   "dist: %f\n",
//                    query_id,
//                    k_i,
//                    set_L[k_i].id_,
//                    set_L[k_i].distance_);
//        }
//        exit(1);
//    }
}

////// Backup: using OpenMP critical clause
//inline void Searching::para_search_with_top_m(
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
//#pragma omp parallel for
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
////                ++count_distance_computation;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
////                if (dist >= set_L[L-1].distance_) {
////                    continue;
////                }
//                Candidate cand(nb_id, dist, false);
//                idi r;
//#pragma omp critical
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

// DEPRECATED. No enough workload for OpenMP, and hard to implement efficiently.
///**
// * Prepare init_ids and flags, as they are constant for all queries.
// * @param[out] init_ids
// * @param L
// */
//inline void Searching::para_prepare_init_ids(
//        std::vector<unsigned int> &init_ids,
//        unsigned L) const
//{
////    idi num_ngbrs = get_out_degree(ep_);
////    edgei edge_start = nsg_graph_indices_[ep_];
////    // Store ep_'s neighbors as candidates
////    idi tmp_l = 0;
////    for (; tmp_l < L && tmp_l < num_ngbrs; tmp_l++) {
////        init_ids[tmp_l] = nsg_graph_out_edges_[edge_start + tmp_l];
////    }
////    std::unordered_set<idi> visited_ids;
//    std::vector<uint8_t> is_selected(num_v_, 0);
////    boost::dynamic_bitset<> is_selected(num_v_);
//    idi *out_edges = (idi *) (opt_nsg_graph_ + ep_ * vertex_bytes_ + data_bytes_);
//    idi out_degree = *out_edges++;
//    idi init_ids_end = 0;
////    idi e_i_bound = out_degree <= L ? out_degree : L;
//#pragma omp parallel for
//    for (idi e_i = 0; e_i < out_degree && init_ids_end < L; ++e_i) {
////    for (idi e_i = 0; e_i < e_i_bound; ++e_i) {
//        idi v_id = out_edges[e_i];
////        if(is_selected[v_id]) {
////            continue;
////        }
////        is_selected[v_id] = 1;
//
//        if (!AtomicOps::CAS(is_selected.data() + v_id,
//                            static_cast<uint8_t>(0),
//                            static_cast<uint8_t>(1))) {
//            continue;
//        }
//
////        init_ids[init_ids_end++] = v_id;
//        volatile idi old_v = init_ids_end;
//        volatile idi new_v = old_v + 1;
//        while (!AtomicOps::CAS(&init_ids_end, old_v, new_v)) {
//            old_v = init_ids_end;
//            new_v = old_v + 1;
//        }
//        init_ids[old_v] = v_id;
//    }
//
////    for (idi i = 0; i < tmp_l; ++i) {
////        is_visited[init_ids[i]] = true;
////    }
//
//    // If ep_'s neighbors are not enough, add other random vertices
//    idi tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    while (init_ids_end < L) {
//        tmp_id %= num_v_;
//        idi v_id = tmp_id++;
//        if (is_selected[v_id]) {
//            continue;
//        }
////        if (visited_ids.find(id) != visited_ids.end()) {
////            continue;
////        }
//        is_selected[v_id] = 1;
////        visited_ids.insert(id);
//        init_ids[init_ids_end++] = v_id;
////        tmp_l++;
//    }
//}


} // namespace PANNS


#endif //BATCH_SEARCHING_SEARCHING_H
