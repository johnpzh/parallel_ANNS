//
// Created by Zhen Peng on 7/12/2020.
//

#ifndef BATCH_SEARCHING_SEARCHING_H
#define BATCH_SEARCHING_SEARCHING_H

#include <vector>
#include <boost/dynamic_bitset.hpp>
//#include <boost/thread/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>
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
#include <thread>
#include <atomic>
//#include <mutex>
//#include <shared_mutex>
#include <omp.h>
#include "../include/definitions.h"
//#include "../include/efanna2e/neighbor.h"
#include "../include/utils.h"
//#include "../include/Candidate.h"
//#include "../include/parallelization.h"
//#include "../include/bitvector.h"
#include "../include/Neighbor.h"
#include "../include/Spinlock.h"

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

    dataf *data_load_ = nullptr;
    dataf *queries_load_ = nullptr;
    char *opt_nsg_graph_ = nullptr;
    uint64_t data_bytes_; // Byte size of data domain: 1 norm and all data
    uint64_t neighbor_bytes_;
    uint64_t vertex_bytes_; //

    // For multithreads
    int num_threads_ = 1;

    dataf compute_norm(
            const dataf *data) const;
    dataf compute_distance_with_norm(
            const dataf *v_data,
            const dataf *q_data,
            const dataf vertex_norm) const;
    idi add_into_queue(
            std::vector<Neighbor> &queue,
            const idi queue_start,
            idi &queue_size,
            const idi queue_capacity,
            const Neighbor &cand);
    void get_start_point_and_its_neighbors(
            const idi query_id,
            std::vector<Neighbor> &worklist,
            const idi worklist_start,
            idi &worklist_size,
//            const idi L,
            const std::vector<idi> &init_ids,
            const idi init_ids_start,
            const idi init_ids_size,
            boost::dynamic_bitset<> &is_visited);

public:
    // For Profiling
//    L3CacheMissRate cache_miss_kernel;
    uint64_t count_distance_computation_{0};
    std::atomic<uint64_t> count_distance_computation_atomic_{0};
    uint64_t count_add_to_queue_ = 0;
//    uint64_t count_single_query_computation_ = 0;
//    distf dist_min_ = 0;
//    distf dist_max_ = 0;
    double time_merge_ = 0;
    double time_add_to_queue_ = 0;
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
        free(queries_load_);
        queries_load_ = nullptr;
        free(opt_nsg_graph_);
        opt_nsg_graph_ = nullptr;
    }
    void load_data_load(char *filename);
    void load_queries_load(char *filename);
    void load_nsg_graph(char *filename);
    void prepare_init_ids(
            std::vector<unsigned> &init_ids,
            const unsigned L,
            boost::dynamic_bitset<> &is_visited) const;
    void load_true_NN(
            const char *filename,
            std::vector< std::vector<idi> > &true_nn_list);
    void get_recall_for_all_queries(
            const std::vector< std::vector<idi> > &true_nn_list,
            const std::vector<std::vector<unsigned>> &set_K_list,
            std::unordered_map<unsigned, double> &recalls) const;

    void simple_search_with_two_queues_seq(
            const idi query_id,
            const idi L,
            std::vector<Neighbor> &set_L,
            std::vector<Neighbor> &worklist,
            const std::vector<idi> &init_ids,
            boost::dynamic_bitset<> &is_visited,
            const idi K,
            std::vector<idi> &set_K);
    void simple_search_worker_set_L_global_worklist_global(
            const idi worker_id,
            const idi query_id,
            const idi global_L,
            std::vector<Neighbor> &set_L,
            const idi set_L_start,
            idi &set_L_size,
//            Spinlock &set_L_spinlock,
        boost::shared_mutex &set_L_mutex,
            std::vector<Neighbor> &worklist,
            idi &worklist_start,
            idi &worklist_size,
//            Spinlock &worklist_spinlock,
        boost::shared_mutex &worklist_mutex,
            boost::dynamic_bitset<> &is_visited);
    void simple_search_two_global_queues_para(
            const idi query_id,
            const idi global_L,
            std::vector<Neighbor> &set_L,
            std::vector<Neighbor> &worklist,
            const std::vector<idi> &init_ids,
            boost::dynamic_bitset<> &is_visited,
            const idi K,
            std::vector<idi> &set_K);
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


/**
 * Prepare init_ids and flags, as they are constant for all queries.
 * @param[out] init_ids
 * @param L
 */
inline void Searching::prepare_init_ids(
        std::vector<unsigned int> &init_ids,
        const unsigned L,
        boost::dynamic_bitset<> &is_visited) const
{
//    boost::dynamic_bitset<> is_selected(num_v_);
    idi *out_edges = (idi *) (opt_nsg_graph_ + ep_ * vertex_bytes_ + data_bytes_);
    idi out_degree = *out_edges++;
    idi init_ids_end = 0;
    for (idi e_i = 0; e_i < out_degree && init_ids_end < L; ++e_i) {
        idi v_id = out_edges[e_i];
        if(is_visited[v_id]) {
            continue;
        }
        is_visited[v_id] = 1;
        init_ids[init_ids_end++] = v_id;
    }

    // If ep_'s neighbors are not enough, add other random vertices
    idi tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    std::srand(1);
    while (init_ids_end < L) {
        tmp_id %= num_v_;
        idi v_id = tmp_id++;
//        idi v_id = std::rand() % num_v_;
        if (is_visited[v_id]) {
            continue;
        }
        is_visited[v_id] = 1;
        init_ids[init_ids_end++] = v_id;
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

inline idi Searching::add_into_queue(
        std::vector<Neighbor> &queue,
        const idi queue_start,
        idi &queue_size,
        const idi queue_capacity,
        const Neighbor &cand)
{
    if (0 == queue_size) {
        queue[queue_start + queue_size++] = cand;
        return 0;
    }

    // Find the insert location
    const auto insert_it = std::lower_bound(
                            queue.begin() + queue_start,
                            queue.begin() + queue_start + queue_size,
                            cand);
    idi insert_sub = insert_it - queue.begin() - queue_start;

    if (insert_sub != queue_size) {
        // Do we need to check duplicate here?
        if (insert_it->id_ == cand.id_) {
            // Duplicate
            return queue_size;
        }

        if (queue_size == queue_capacity) { // Queue is full
            --queue_size;
        }

        // Add into queue
        memmove(reinterpret_cast<char *>(queue.data() + queue_start + insert_sub + 1),
                reinterpret_cast<char *>(queue.data() + queue_start + insert_sub),
                (queue_size - insert_sub) * sizeof(Neighbor));
        queue[insert_sub + queue_start] = cand;
        ++queue_size;
        return insert_sub;
    } else { // Insert at the end?
        if (queue_size < queue_capacity) { // Queue is not full
            // Insert at the end
            queue[insert_sub + queue_start] = cand;
            ++queue_size;
            return insert_sub;
        } else { // Queue is full
            // No insert
            return insert_sub;
        }
    }
}

inline void Searching::get_start_point_and_its_neighbors(
        const idi query_id,
        std::vector<Neighbor> &worklist,
        const idi worklist_start,
        idi &worklist_size,
//        const idi L,
        const std::vector<idi> &init_ids,
        const idi init_ids_start,
        const idi init_ids_size,
        boost::dynamic_bitset<> &is_visited)
{
    const dataf *query_data = queries_load_ + query_id * dimension_;
    uint64_t tmp_distance_computation = 0;
    idi v_i_start = init_ids_start;
    idi v_i_bound = v_i_start + init_ids_size;
    for (idi v_i = v_i_start; v_i < v_i_bound; ++v_i) {
        idi v_id = init_ids[v_i];
        is_visited[v_id] = 1;
        dataf *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
        dataf v_norm = *v_data++;
        ++tmp_distance_computation;
        distf dist = compute_distance_with_norm(v_data, query_data, v_norm);
        worklist[worklist_start + worklist_size++] = Neighbor(v_id, dist);
    }
    count_distance_computation_ += tmp_distance_computation;


//    const dataf *query_data = queries_load_ + query_id * dimension_;
//    // Add start point
//    {
//        idi v_id = ep_;
//        is_visited[v_id] = 1;
//        dataf *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf v_norm = *v_data++;
//        distf dist = compute_distance_with_norm(v_data, query_data, v_norm);
//        worklist[worklist_end++] = Neighbor(v_id, dist);
//    }
//    ++count_distance_computation_;
//
//    // Add its neighbors
//
//    idi *out_edges = reinterpret_cast<idi *>(opt_nsg_graph_ + ep_ * vertex_bytes_ + data_bytes_);
//    idi out_degree = *out_edges++;
//    uint64_t tmp_distance_computation = 0;
//
//    for (idi e_i = 0; e_i < out_degree && worklist_end < L; ++e_i) {
//        idi v_id = out_edges[e_i];
//        if(is_visited[v_id]) {
//            continue;
//        }
//        is_visited[v_id] = 1;
//        dataf *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf v_norm = *v_data++;
//        ++tmp_distance_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, v_norm);
//        worklist[worklist_end++] = Neighbor(v_id, dist);
//    }
//    count_distance_computation_ += tmp_distance_computation;
//    tmp_distance_computation = 0;
//
//    // If not smaller than L
//    idi tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    while (worklist_end < L) {
//        tmp_id %= num_v_;
//        idi v_id = tmp_id++;
//        if (is_visited[v_id]) {
//            continue;
//        }
//        is_visited[v_id] = 1;
//        dataf *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf v_norm = *v_data++;
//        ++tmp_distance_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, v_norm);
//        worklist[worklist_end++] = Neighbor(v_id, dist);
//    }
//    count_distance_computation_ += tmp_distance_computation;


//    // Reset is_visited
//    for (idi e_i = 0; e_i < worklist_end; ++e_i) {
//        is_visited[worklist[e_i].id_] = 0;
//    }
}
//// Backup
//inline void Searching::get_start_point_and_its_neighbors(
//        const idi query_id,
//        std::vector<Neighbor> &worklist,
//        idi &worklist_end,
//        boost::dynamic_bitset<> &is_visited)
//{
//    const dataf *query_data = queries_load_ + query_id * dimension_;
//    // Add start point
//    {
//        idi v_id = ep_;
//        is_visited[v_id] = 1;
//        dataf *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf v_norm = *v_data++;
//        distf dist = compute_distance_with_norm(v_data, query_data, v_norm);
//        worklist[worklist_end++] = Neighbor(v_id, dist);
//    }
//    ++count_distance_computation_;
//
//    // Add its neighbors
//
//    idi *out_edges = reinterpret_cast<idi *>(opt_nsg_graph_ + ep_ * vertex_bytes_ + data_bytes_);
//    idi out_degree = *out_edges++;
//    uint64_t tmp_distance_computation = 0;
//
//    for (idi e_i = 0; e_i < out_degree; ++e_i) {
//        idi v_id = out_edges[e_i];
//        if(is_visited[v_id]) {
//            continue;
//        }
//        is_visited[v_id] = 1;
//        dataf *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf v_norm = *v_data++;
//        ++tmp_distance_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, v_norm);
//        worklist[worklist_end++] = Neighbor(v_id, dist);
//    }
//    count_distance_computation_ += tmp_distance_computation;
////    // Reset is_visited
////    for (idi e_i = 0; e_i < worklist_end; ++e_i) {
////        is_visited[worklist[e_i].id_] = 0;
////    }
//}

inline void Searching::simple_search_with_two_queues_seq(
        const idi query_id,
        const idi L,
        std::vector<Neighbor> &set_L,
        std::vector<Neighbor> &worklist,
        const std::vector<idi> &init_ids,
        boost::dynamic_bitset<> &is_visited,
        const idi K,
        std::vector<idi> &set_K)
{
    idi set_L_end = 0; // real size
    idi worklist_start = 0; // base location
    idi worklist_size = 0; // real size

    {// Initialization
        get_start_point_and_its_neighbors(
                query_id,
                worklist,
                worklist_start,
                worklist_size,
//                L,
                init_ids,
                0,
                L,
                is_visited);
        std::sort(
                worklist.begin(),
                worklist.begin() + worklist_size);
    }

    const dataf *query_data = queries_load_ + query_id * dimension_;
    idi iter = 0;
    while (worklist_size) {
        ++iter;
//        {//
//            printf("iter: %u "
//                   "set_L_end: %u "
//                   "worklist_end: %u\n",
//                   iter,
//                   set_L_end,
//                   worklist_end);
//        }
        // Get the top-1
        Neighbor cand = worklist[worklist_start];
//        Neighbor cand = worklist[0];
        // Remove
//        memmove(reinterpret_cast<char *>(worklist.data()),
//                reinterpret_cast<char *>(worklist.data() + 1),
//                (worklist_end - 1) * sizeof(Neighbor));
        ++worklist_start;
        --worklist_size;
//        is_visited[cand.id_] = 1;
        // Add to set_L
        ++count_add_to_queue_;
        time_add_to_queue_ -= WallTimer::get_time_mark();
        idi insert_sub = add_into_queue(
                set_L,
                0,
                set_L_end,
                L,
                cand);
        time_add_to_queue_ += WallTimer::get_time_mark();
        if (insert_sub >= L) {
            // Stop condition:
            break;
        }

        // Explore neighbors
        idi tmp_distance_computation = 0;
        idi *out_edges = reinterpret_cast<idi *>(opt_nsg_graph_ + cand.id_ * vertex_bytes_ + data_bytes_);
        idi out_degree = *out_edges++;
        for (idi e_i = 0; e_i < out_degree; ++e_i) {
            idi v_id = out_edges[e_i];
            if(is_visited[v_id]) {
                continue;
            }
            is_visited[v_id] = 1;
            dataf *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
            dataf v_norm = *v_data++;
            ++tmp_distance_computation;
            distf dist = compute_distance_with_norm(v_data, query_data, v_norm);
            if (dist > worklist[worklist_start + worklist_size - 1].distance_) {
                continue;
            }
            if (set_L_end == L && dist > set_L[set_L_end - 1].distance_) {
                continue;
            }
            ++count_add_to_queue_;
            time_add_to_queue_ -= WallTimer::get_time_mark();
            add_into_queue(
                    worklist,
                    worklist_start,
                    worklist_size,
                    L,
                    Neighbor(v_id, dist));
            time_add_to_queue_ += WallTimer::get_time_mark();
        }
        count_distance_computation_ += tmp_distance_computation;
    }

    {// Return results
        for (idi e_i = 0; e_i < K; ++e_i) {
            set_K[e_i] = set_L[e_i].id_;
        }
    }

//    {//test
//        if (100 == query_id) {
//            exit(1);
//        }
//    }
    {// Reset flags
        is_visited.reset();
    }
}

inline void Searching::simple_search_worker_set_L_global_worklist_global(
        const idi worker_id,
        const idi query_id,
        const idi global_L,
        std::vector<Neighbor> &set_L,
        const idi set_L_start,
        idi &set_L_size,
//        Spinlock &set_L_spinlock,
        boost::shared_mutex &set_L_mutex,
        std::vector<Neighbor> &worklist,
        idi &worklist_start,
        idi &worklist_size,
//        Spinlock &worklist_spinlock,
        boost::shared_mutex &worklist_mutex,
        boost::dynamic_bitset<> &is_visited)
{
//    idi set_L_end = 0; // real size
//    idi worklist_start = 0; // base location
//    set_L_ends[worker_id] = 0;
//    idi worklist_size = 0; // real size

//    {// Initialization
//        get_start_point_and_its_neighbors(
//                query_id,
//                worklist,
//                worklist_start,
//                worklist_size,
//                local_L,
//                init_ids,
//                init_ids_start,
//                is_visited);
//        std::sort(
//                worklist.begin(),
//                worklist.begin() + worklist_end);
//    }

    const dataf *query_data = queries_load_ + query_id * dimension_;
    idi iter = 0;
    while (worklist_size) {
        ++iter;
//        {//
//            printf("query_id: %u "
//                   "iter: %u "
//                   "set_L_size: %u "
//                   "worklist_size: %u\n",
//                   query_id,
//                   iter,
//                   set_L_size,
//                   worklist_size);
//        }
        // Get the top-1
        Neighbor cand;
        {
            boost::lock_guard<boost::shared_mutex> lock_worklist(worklist_mutex);
//            worklist_spinlock.lock();
            cand = worklist[worklist_start++];
            // Remove
            --worklist_size;
//            worklist_spinlock.unlock();
        }
        // Add to set_L
//        time_add_to_queue_ -= WallTimer::get_time_mark();
        idi insert_sub;
        {
            boost::lock_guard<boost::shared_mutex> lock_set_L(set_L_mutex);
//            set_L_spinlock.lock();
            insert_sub = add_into_queue(
                    set_L,
                    set_L_start,
                    set_L_size,
                    global_L,
                    cand);
//            set_L_spinlock.unlock();
        }
//        time_add_to_queue_ += WallTimer::get_time_mark();
        if (insert_sub >= global_L) {
            // Stop condition:
            break;
        }

        // Explore neighbors
        idi tmp_distance_computation = 0;
        idi *out_edges = reinterpret_cast<idi *>(opt_nsg_graph_ + cand.id_ * vertex_bytes_ + data_bytes_);
        idi out_degree = *out_edges++;
        for (idi e_i = 0; e_i < out_degree; ++e_i) {
            idi v_id = out_edges[e_i];
            if(is_visited[v_id]) {
                continue;
            }
            is_visited[v_id] = 1;
            dataf *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
            dataf v_norm = *v_data++;
            ++tmp_distance_computation;
            distf dist = compute_distance_with_norm(v_data, query_data, v_norm);
            {
                boost::shared_lock<boost::shared_mutex> lock_worklist(worklist_mutex);
//                worklist_spinlock.lock();
                if (dist > worklist[worklist_start + worklist_size - 1].distance_) {
//                    worklist_spinlock.unlock();
                    continue;
                }
//                worklist_spinlock.unlock();
            }
            {
                boost::shared_lock<boost::shared_mutex> lock_set_L(set_L_mutex);
//                set_L_spinlock.lock();
                if (set_L_size == global_L && dist > set_L[set_L_size - 1].distance_) {
//                    set_L_spinlock.unlock();
                    continue;
                }
//                set_L_spinlock.unlock();
            }
//            ++count_add_to_queue_;
//            time_add_to_queue_ -= WallTimer::get_time_mark();
            {
                boost::lock_guard<boost::shared_mutex> lock_worklist(worklist_mutex);
//                worklist_spinlock.lock();
                add_into_queue(
                        worklist,
                        worklist_start,
//                    0,
                        worklist_size,
                        global_L,
                        Neighbor(v_id, dist));
//                worklist_spinlock.unlock();
            }
//            time_add_to_queue_ += WallTimer::get_time_mark();
        }
        count_distance_computation_atomic_ += tmp_distance_computation;
    }

//    {// Return results
//        for (idi e_i = 0; e_i < K; ++e_i) {
//            set_K[e_i] = set_L[e_i].id_;
//        }
//    }

//    {//test
//        if (100 == query_id) {
//            exit(1);
//        }
//    }
//    {// Reset flags
//        is_visited.reset();
//    }
}

////Backup
//inline void Searching::simple_search_worker_set_L_global_worklist_global(
//        const idi worker_id,
//        const idi query_id,
//        const idi global_L,
//        std::vector<Neighbor> &set_L,
//        const idi set_L_start,
//        idi &set_L_size,
//        boost::shared_mutex &set_L_mutex,
//        std::vector<Neighbor> &worklist,
//        idi &worklist_start,
//        idi &worklist_size,
//        boost::shared_mutex &worklist_mutex,
//        boost::dynamic_bitset<> &is_visited)
//{
////    idi set_L_end = 0; // real size
////    idi worklist_start = 0; // base location
////    set_L_ends[worker_id] = 0;
////    idi worklist_size = 0; // real size
//
////    {// Initialization
////        get_start_point_and_its_neighbors(
////                query_id,
////                worklist,
////                worklist_start,
////                worklist_size,
////                local_L,
////                init_ids,
////                init_ids_start,
////                is_visited);
////        std::sort(
////                worklist.begin(),
////                worklist.begin() + worklist_end);
////    }
//
//    const dataf *query_data = queries_load_ + query_id * dimension_;
//    idi iter = 0;
//    while (worklist_size) {
//        ++iter;
////        {//
////            printf("iter: %u "
////                   "set_L_end: %u "
////                   "worklist_end: %u\n",
////                   iter,
////                   set_L_end,
////                   worklist_end);
////        }
//        // Get the top-1
//        Neighbor cand;
//        {
//            boost::lock_guard<boost::shared_mutex> lock_worklist(worklist_mutex);
//            cand = worklist[worklist_start++];
//            // Remove
//            --worklist_size;
//        }
//        // Add to set_L
////        time_add_to_queue_ -= WallTimer::get_time_mark();
//        idi insert_sub;
//        {
//            boost::lock_guard<boost::shared_mutex> lock_set_L(set_L_mutex);
//            insert_sub = add_into_queue(
//                    set_L,
//                    set_L_start,
//                    set_L_size,
//                    global_L,
//                    cand);
//        }
////        time_add_to_queue_ += WallTimer::get_time_mark();
//        if (insert_sub >= global_L) {
//            // Stop condition:
//            break;
//        }
//
//        // Explore neighbors
//        idi tmp_distance_computation = 0;
//        idi *out_edges = reinterpret_cast<idi *>(opt_nsg_graph_ + cand.id_ * vertex_bytes_ + data_bytes_);
//        idi out_degree = *out_edges++;
//        for (idi e_i = 0; e_i < out_degree; ++e_i) {
//            idi v_id = out_edges[e_i];
//            if(is_visited[v_id]) {
//                continue;
//            }
//            is_visited[v_id] = 1;
//            dataf *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//            dataf v_norm = *v_data++;
//            ++tmp_distance_computation;
//            distf dist = compute_distance_with_norm(v_data, query_data, v_norm);
//            {
//                boost::shared_lock<boost::shared_mutex> lock_worklist(worklist_mutex);
//                if (dist > worklist[worklist_start + worklist_size - 1].distance_) {
//                    continue;
//                }
//            }
//            {
//                boost::shared_lock<boost::shared_mutex> lock_set_L(set_L_mutex);
//                if (set_L_size == global_L && dist > set_L[set_L_size - 1].distance_) {
//                    continue;
//                }
//            }
////            ++count_add_to_queue_;
////            time_add_to_queue_ -= WallTimer::get_time_mark();
//            {
//                boost::lock_guard<boost::shared_mutex> lock_worklist(worklist_mutex);
//                add_into_queue(
//                        worklist,
//                        worklist_start,
////                    0,
//                        worklist_size,
//                        global_L,
//                        Neighbor(v_id, dist));
//            }
////            time_add_to_queue_ += WallTimer::get_time_mark();
//        }
//        count_distance_computation_atomic_ += tmp_distance_computation;
//    }
//
////    {// Return results
////        for (idi e_i = 0; e_i < K; ++e_i) {
////            set_K[e_i] = set_L[e_i].id_;
////        }
////    }
//
////    {//test
////        if (100 == query_id) {
////            exit(1);
////        }
////    }
////    {// Reset flags
////        is_visited.reset();
////    }
//}

inline void Searching::simple_search_two_global_queues_para(
        const idi query_id,
        const idi global_L,
        std::vector<Neighbor> &set_L,
        std::vector<Neighbor> &worklist,
        const std::vector<idi> &init_ids,
        boost::dynamic_bitset<> &is_visited,
        const idi K,
        std::vector<idi> &set_K)
{
    idi set_L_start = 0;
    idi set_L_size = 0; // real size
    idi worklist_start = 0; // base location
    idi worklist_size = 0; // real size

    {// Initialization
        get_start_point_and_its_neighbors(
                query_id,
                worklist,
                worklist_start,
                worklist_size,
//                L,
                init_ids,
                0,
                global_L,
                is_visited);
        std::sort(
                worklist.begin(),
                worklist.begin() + worklist_size);
    }
    boost::shared_mutex set_L_mutex;
    boost::shared_mutex worklist_mutex;
//    Spinlock set_L_spinlock;
//    Spinlock worklist_spinlock;

    std::vector<std::thread> threads(num_threads_ - 1);
    for (int t_i = 1; t_i < num_threads_; ++t_i) {
        threads[t_i - 1] = std::thread(
                &Searching::simple_search_worker_set_L_global_worklist_global,
                this,
                t_i,
                query_id,
                global_L,
                std::ref(set_L),
                set_L_start,
                std::ref(set_L_size),
//                std::ref(set_L_spinlock),
                std::ref(set_L_mutex),
                std::ref(worklist),
                std::ref(worklist_start),
                std::ref(worklist_size),
//                std::ref(worklist_spinlock),
                std::ref(worklist_mutex),
                std::ref(is_visited));
    }
    simple_search_worker_set_L_global_worklist_global(
            0,
            query_id,
            global_L,
            set_L,
            set_L_start,
            set_L_size,
//            set_L_spinlock,
            set_L_mutex,
            worklist,
            worklist_start,
            worklist_size,
//            set_L_spinlock,
            worklist_mutex,
            is_visited);
    for (int t_i = 1; t_i < num_threads_; ++t_i) {
        threads[t_i - 1].join();
    }
//    ////////////////////
//    const dataf *query_data = queries_load_ + query_id * dimension_;
//    idi iter = 0;
//    while (worklist_size) {
//        ++iter;
////        {//
////            printf("iter: %u "
////                   "set_L_end: %u "
////                   "worklist_end: %u\n",
////                   iter,
////                   set_L_end,
////                   worklist_end);
////        }
//        // Get the top-1
//        Neighbor cand = worklist[worklist_start];
////        Neighbor cand = worklist[0];
//        // Remove
////        memmove(reinterpret_cast<char *>(worklist.data()),
////                reinterpret_cast<char *>(worklist.data() + 1),
////                (worklist_end - 1) * sizeof(Neighbor));
//        ++worklist_start;
//        --worklist_size;
////        is_visited[cand.id_] = 1;
//        // Add to set_L
//        ++count_add_to_queue_;
//        time_add_to_queue_ -= WallTimer::get_time_mark();
//        idi insert_sub = add_into_queue(
//                set_L,
//                0,
//                set_L_end,
//                L,
//                cand);
//        time_add_to_queue_ += WallTimer::get_time_mark();
//        if (insert_sub >= L) {
//            // Stop condition:
//            break;
//        }
//
//        // Explore neighbors
//        idi tmp_distance_computation = 0;
//        idi *out_edges = reinterpret_cast<idi *>(opt_nsg_graph_ + cand.id_ * vertex_bytes_ + data_bytes_);
//        idi out_degree = *out_edges++;
//        for (idi e_i = 0; e_i < out_degree; ++e_i) {
//            idi v_id = out_edges[e_i];
//            if(is_visited[v_id]) {
//                continue;
//            }
//            is_visited[v_id] = 1;
//            dataf *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//            dataf v_norm = *v_data++;
//            ++tmp_distance_computation;
//            distf dist = compute_distance_with_norm(v_data, query_data, v_norm);
//            if (dist > worklist[worklist_start + worklist_size - 1].distance_) {
//                continue;
//            }
//            if (set_L_end == L && dist > set_L[set_L_end - 1].distance_) {
//                continue;
//            }
//            ++count_add_to_queue_;
//            time_add_to_queue_ -= WallTimer::get_time_mark();
//            add_into_queue(
//                    worklist,
//                    worklist_start,
//                    worklist_size,
//                    L,
//                    Neighbor(v_id, dist));
//            time_add_to_queue_ += WallTimer::get_time_mark();
//        }
//        count_distance_computation_ += tmp_distance_computation;
//    }

    {// Return results
        for (idi e_i = 0; e_i < K; ++e_i) {
            set_K[e_i] = set_L[e_i].id_;
        }
    }

//    {//test
//        for (idi e_i = 0; e_i < global_L; ++e_i) {
//            printf("query_id:\t%u\t"
//                   "e_i:\t%u\t"
//                   "%u\t"
//                   "%f\n",
//                   query_id,
//                   e_i,
//                   set_L[e_i].id_,
//                   set_L[e_i].distance_);
//        }
//        if (1 == query_id) {
//            exit(1);
//        }
//    }
    {// Reset flags
        is_visited.reset();
    }
}

} // namespace PANNS


#endif //BATCH_SEARCHING_SEARCHING_H
