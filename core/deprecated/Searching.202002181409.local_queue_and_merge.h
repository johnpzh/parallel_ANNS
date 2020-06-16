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
#include <omp.h>
#include "../include/definitions.h"
//#include "../include/efanna2e/neighbor.h"
#include "../include/utils.h"
#include "../include/Candidate.h"
#include "../include/parallelization.h"

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
            const PANNS::Candidate &cand);
//    idi insert_into_queue_nsg(
//            std::vector< Candidate > &c_queue,
//            idi c_queue_top,
//            Candidate cand);
    template<typename T>
    static void insert_one_element_at(
            const T &cand,
            T *queue_base,
            const idi dest_index,
            const idi queue_size);
    static idi merge_two_queues_into_1st_queue_seq(
            std::vector<Candidate> &queue1,
            const idi queue1_start,
            const idi queue1_size,
            std::vector<Candidate> &queue2,
            const idi queue2_start,
            const idi queue2_size);
//            const idi limit_size);

public:
    // For Profiling
//    L3CacheMissRate cache_miss_kernel;
    uint64_t count_distance_computation = 0;

    // For multithreads
    int num_threads_ = 1;
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
        for (unsigned top_i = 0; top_i < 100; ++top_i) {
            unsigned true_id = true_nn_list[q_i][top_i];
            for (unsigned n_i = 0; n_i < 100; ++n_i) {
                if (set_K_list[q_i][n_i] == true_id) {
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
        std::vector<idi> &set_K) const
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
        const PANNS::Candidate &cand)
{
    if (0 == queue_top) {
        queue[queue_top++] = cand;
        return 0;
    }

    // Find the insert location
    auto it_loc = std::lower_bound(queue.begin(), queue.begin() + queue_top, cand);
    idi insert_loc = it_loc - queue.begin();

    // Insert
    memmove(reinterpret_cast<char *>(queue.data() + insert_loc + 1),
            reinterpret_cast<char *>(queue.data() + insert_loc),
            (queue_top - insert_loc) * sizeof(Candidate));
    *it_loc = cand;
    ++queue_top;
    return insert_loc;
}

// TODO: need verification.
template<typename T>
inline void Searching::insert_one_element_at(
        const T &cand,
        T *queue_base,
        const idi dest_index,
        const idi queue_size)
{
    memmove(reinterpret_cast<char *>(queue_base + dest_index + 1),
            reinterpret_cast<char *>(queue_base + dest_index),
            (queue_size - dest_index) * sizeof(T));
    queue_base[dest_index] = cand;
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
    idi insert_loc = it_loc - (queue1.begin() + queue1_start);

    auto *queue1_base = queue1.data() + queue1_start;
    // Insert the 1st of queue2
    insert_one_element_at(
            queue2[queue2_start],
            queue1_base,
            insert_loc,
            queue1_size);
//    memmove(reinterpret_cast<char *>(queue1_base + insert_loc + 1),
//            reinterpret_cast<char *>(queue1_base + insert_loc),
//            (queue1_size - insert_loc) * sizeof(Candidate));
//    queue1[insert_loc] = queue2[queue2_start];

    // Insert
    idi q_i_1 = insert_loc + 1;
    idi q_i_2 = queue2_start + 1;
    const idi q_i_1_bound = queue1_start + queue1_size;
    const idi q_i_2_bound = queue2_start + queue2_size;
//    const idi insert_i_bound = queue1_start + limit_size;
    for (idi insert_i = insert_loc + 1; insert_i < q_i_1_bound; ++insert_i) {
        if (q_i_1 >= q_i_1_bound || q_i_2 >= q_i_2_bound) {
            // queue1 or queue2 finished traverse. Rest o
            break;
        } else if (queue1[q_i_1] < queue2[q_i_2]) {
            ++q_i_1;
        } else {
            // Insert queue2[q_i_2] into queue1
            insert_one_element_at(
                    queue2[q_i_2++],
                    queue1_base,
                    insert_i,
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

    return insert_loc;
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
//        ++count_distance_computation;
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

//        {//test
//            printf("tmp_count: %u\n", tmp_count);
//            for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//                printf("top_m_candidates[%u]: %u\n",
//                       c_i,
//                       top_m_candidates[c_i]);
//            }
//            if (3 == tmp_count) {
//                exit(1);
//            }
//            if (3 == tmp_count) {
//                printf("top_m_candidates[76]: %u\n",
//                       top_m_candidates[76]);
//            }
//        }
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
//                {//test
//                    if (793600 == cand_id) {
//                        printf("e_i: %u "
//                               "nb_id: %u\n",
//                               e_i,
//                               nb_id);
//                    }
//                }
                if (is_visited[nb_id]) {
                    continue;
                }
                is_visited[nb_id] = true;
                auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
                dataf norm = *nb_data++;
//                ++count_distance_computation;
                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                {//test
//                    if (180955 == nb_id) {
//                        printf("dist: %f "
//                               "set_L[L-1].distance_: %f "
//                               "e_i: %u\n",
//                               dist,
//                               set_L[L-1].distance_,
//                               e_i);
//                        exit(1);
//                    }
//                }
                if (dist > set_L[L-1].distance_) {
                    continue;
                }
//                if (dist >= set_L[L-1].distance_) {
//                    continue;
//                }
                Candidate cand(nb_id, dist, false);
                idi r = insert_into_queue(set_L, L, cand);
                {
//                    {//test
//                        if (nb_id == 180955) {
//                            printf("dist: %f "
//                                   "r: %u\n",
//                                   dist,
//                                   r);
//                            exit(1);
//                        }
//                    }
                }
                if (r < nk) {
                    nk = r;
                }
            }
//            {//test
//                if (793600 == cand_id) {
//                    exit(1);
//                }
//            }
        }
        top_m_candidates_end = 0; // Clear top_m_candidates

        if (nk <= last_k) {
            k = nk;
        } else {
            k = last_k + 1;
        }
//        {//test
//            printf("nk: %u "
//                   "last_k: %u "
//                   "k: %u "
//                   "tmp_count: %u\n",
//                   nk,
//                   last_k,
//                   k,
//                   tmp_count);
//        }

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

//// Using local queue and then merge.
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
    const idi local_queue_length = ((L - 1) / num_threads_ + 1) * width_;
    std::vector< std::vector<Candidate> > local_queues_list(num_threads_, std::vector<Candidate>(local_queue_length));
    std::vector<idi> local_queues_ends(num_threads_, 0);
    std::vector<uint8_t> is_visited(num_v_, 0);
//    boost::dynamic_bitset<> is_visited(num_v_);

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
#pragma omp parallel for
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
    idi tmp_count = 0; // for debug
    while (k < L) {
        ++tmp_count;
//        {
//            printf("tmp_count: %u "
//                   "k: %u\n",
//                   tmp_count,
//                   k);
//        }

//        unsigned nk = L;
//        int nk = L;

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
#pragma omp parallel for
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
                if (dist > set_L[L-1].distance_) {
                    continue;
                }
//                if (dist >= set_L[L-1].distance_) {
//                    continue;
//                }
                Candidate cand(nb_id, dist, false);
//                idi r;
//#pragma omp critical
//                {
//                    r = insert_into_queue(set_L, L, cand);
//                    if (r < nk) {
//                        nk = r;
//                    }
//                }
                // Add to the local queue.
                add_into_queue(local_queues_list[tid], local_queues_ends[tid], cand);
            }
        }
        top_m_candidates_end = 0; // Clear top_m_candidates

        idi nk = L;
        // Merge
        {
            for (int tid = 0; tid < num_threads_; ++tid) {
                if (0 == local_queues_ends[tid]) continue;
                idi r = merge_two_queues_into_1st_queue_seq(
                        set_L,
                        0,
                        L,
                        local_queues_list[tid],
                        0,
                        local_queues_ends[tid]);
//                        L + 1);
                local_queues_ends[tid] = 0; // Reset the local queue
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
    }

#pragma omp parallel for
    for (idi k_i = 0; k_i < K; ++k_i) {
        set_K[k_i] = set_L[k_i].id_;
    }
//
//    {//test
//        for (idi k_i = 0; k_i < K; ++k_i) {
//            printf("%u: %u: %u %f\n",
//                    query_id,
//                    k_i, set_L[k_i].id_, set_L[k_i].distance_);
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
