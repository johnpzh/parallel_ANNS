//
// Created by Zhen Peng on 01/08/2020.
//

#include "Searching.202101081821.scale_m_pure_large_graph.profiling.h"
#define BREAKDOWN_PRINT

namespace PANNS {
/**
 * Input the data from the file.
 * @param filename
 */
void Searching::load_data_load(char *filename)
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
void Searching::load_queries_load(char *filename)
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

///**
// * Input the NSG graph from the file.
// * Reference: https://github.com/ZJULearning/nsg/blob/master/src/index_nsg.cpp
// * @param filename
// */
//void Searching::load_nsg_graph(char *filename)
//{
//    std::ifstream fin(filename);
//    if (!fin.is_open()) {
//        std::cerr << "Error: cannot read file " << filename << " ." << std::endl;
//        exit(EXIT_FAILURE);
//    }
//    fin.read(reinterpret_cast<char *>(&width_), sizeof(unsigned));
//    fin.read(reinterpret_cast<char *>(&ep_), sizeof(unsigned));
//
//    data_bytes_ = (1 + dimension_) * sizeof(dataf);
//    neighbor_bytes_ = (1 + width_) * sizeof(idi);
//    vertex_bytes_ = data_bytes_ + neighbor_bytes_;
//    opt_nsg_graph_ = (char *) malloc(num_v_ * vertex_bytes_);
//    if (!opt_nsg_graph_) {
//        std::cerr << "Error: no enough memory for opt_nsg_graph_." << std::endl;
//        exit(EXIT_FAILURE);
//    }
//
//    idi v_id = 0;
//    num_e_ = 0;
//    char *base_location = opt_nsg_graph_;
//    while (true) {
//        idi degree;
//        fin.read(reinterpret_cast<char *>(&degree), sizeof(unsigned));
//        if (fin.eof()) {
//            break;
//        }
//        num_e_ += degree;
////        std::vector<idi> tmp_ngbrs(degree);
////        fin.read(reinterpret_cast<char *>(tmp_ngbrs.data()), degree * sizeof(unsigned));
//
//        // Norm and data
//        distf norm = compute_norm(data_load_ + v_id * dimension_);
////        distf norm = compute_norm(v_id);
//        std::memcpy(base_location, &norm, sizeof(distf)); // Norm
//        memcpy(base_location + sizeof(distf), data_load_ + v_id * dimension_, dimension_ * sizeof(dataf)); // Data
//        base_location += data_bytes_;
//
//        // Neighbors
//        memcpy(base_location, &degree, sizeof(idi)); // Number of neighbors
//        fin.read(base_location + sizeof(idi), degree * sizeof(unsigned)); // Neighbors
////        memcpy(location + sizeof(idi), tmp_ngbrs.data(), degree * sizeof(unsigned));
//        base_location += neighbor_bytes_;
//        ++v_id;
//    }
//    if (v_id != num_v_) {
//        std::cerr << "Error: NSG data has " << v_id
//                  << " vertices, but origin data has " << num_v_ << " vertices." << std::endl;
//        exit(EXIT_FAILURE);
//    }
//    free(data_load_);
//    data_load_ = nullptr;
////    ////////////////////////
////    idi v_id = 0;
////    num_e_ = 0;
////    while (true) {
////        idi degree;
////        fin.read(reinterpret_cast<char *>(&degree), sizeof(unsigned));
////        if (fin.eof()) {
////            break;
////        }
////        num_e_ += degree;
////
////        std::vector<idi> ngbrs(degree);
////        fin.read(reinterpret_cast<char *>(ngbrs.data()), degree * sizeof(unsigned));
//////        nsg_graph_.push_back(ngbrs);
//////        tmp_edge_list.push_back(ngbrs);
////        edge_list_.push_back(ngbrs);
////        ++v_id;
////    }
////    if (v_id != num_v_) {
////        std::cerr << "Error: NSG data has " << v_id
////                  << " vertices, but origin data has " << num_v_ << " vertices." << std::endl;
////        exit(EXIT_FAILURE);
////    }
//}

/*
 * Read NSG graph, save as index and neighbors.
 */
void Searching::load_common_nsg_graph(char *filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "Error: cannot read file " << filename << " ." << std::endl;
        exit(EXIT_FAILURE);
    }
    fin.read(reinterpret_cast<char *>(&width_), sizeof(unsigned));
    fin.read(reinterpret_cast<char *>(&ep_), sizeof(unsigned));

    std::vector< std::vector<idi> > edge_list(num_v_);
    // Read edge_list
    {
        common_nsg_vertex_base_ = (edgei *) malloc(num_v_ * sizeof(edgei));
        if (!common_nsg_vertex_base_) {
            fprintf(stderr, "Error: load_common_nsg_graph(): common_nsg_vertex_base_ malloc failed.\n");
            exit(EXIT_FAILURE);
        }
        edgei base_offset = 0;
        idi v_id = 0;
        num_e_ = 0;
        while (true) {
            idi degree;
            fin.read(reinterpret_cast<char *>(&degree), sizeof(unsigned));
            if (fin.eof()) {
                break;
            }
            common_nsg_vertex_base_[v_id] = base_offset;

            num_e_ += degree;
            base_offset += 1 + degree;

            edge_list[v_id].resize(degree);
            fin.read(reinterpret_cast<char *>(edge_list[v_id].data()), sizeof(unsigned) * degree);
            ++v_id;
        }
        if (v_id != num_v_) {
            std::cerr << "Error: for out degrees, NSG data has " << v_id
                      << " vertices, but origin data has " << num_v_ << " vertices." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Copy neighbors
    {
        common_nsg_deg_ngbrs_ = (idi *) malloc((num_v_ + num_e_) * sizeof(idi));
        if (!common_nsg_deg_ngbrs_) {
            fprintf(stderr, "Error: load_common_nsg_graph(): common_nsg_deg_ngbrs_ malloc failed.\n");
            exit(EXIT_FAILURE);
        }
        idi *nsg_base = common_nsg_deg_ngbrs_;
        for (idi v_id = 0; v_id < num_v_; ++v_id) {
            idi degree = edge_list[v_id].size();
            *nsg_base++ = degree;
            memcpy(nsg_base, edge_list[v_id].data(), sizeof(unsigned) * degree);
            nsg_base += degree;
        }
//        fin.clear(std::ios_base::goodbit);
//        fin.seekg(2 * sizeof(unsigned), std::ios_base::beg);
//        idi v_id = 0;
//        while (fin.good()) {
//            idi degree;
//            fin.read(reinterpret_cast<char *>(&degree), sizeof(unsigned));
////            idi tmp_degree;
////            fin.read(reinterpret_cast<char *>(&tmp_degree), sizeof(unsigned));
////            fin.seekg(sizeof(unsigned), std::ios_base::cur); // skip the degree
//            if (!fin.good()) {
//                break;
//            }
////            idi degree = out_degrees[v_id++];
////            if (tmp_degree != degree) {
////                printf("tmp_degree: %u "
////                       "degree: %u\n",
////                       tmp_degree,
////                       degree);
////            }
//            ++v_id;
//            *nsg_base++ = degree;
//            fin.read(reinterpret_cast<char *>(nsg_base), degree * sizeof(unsigned));
//            nsg_base += degree;
//        }
//        if (v_id != num_v_) {
//            std::cerr << "Error: for neighbors, NSG data has " << v_id
//                      << " vertices, but origin data has " << num_v_ << " vertices." << std::endl;
//            exit(EXIT_FAILURE);
//        }
    }
}

/**
 * Load those true top-K neighbors (ground truth) of queries
 * @param filename
 * @param[out] true_nn_list
 */
void Searching::load_true_NN(
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

void Searching::get_recall_for_all_queries(
        const std::vector< std::vector<idi> > &true_nn_list,
        const std::vector<std::vector<unsigned>> &set_K_list,
        std::unordered_map<unsigned, double> &recalls,
        const idi L) const
{
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

    idi set_K_size = L < 100 ? L : 100;
    for (unsigned q_i = 0; q_i < num_queries_; ++q_i) {
//        size_t offset = q_i * t_K;
        for (unsigned top_i = 0; top_i < 100; ++top_i) {
            unsigned true_id = true_nn_list[q_i][top_i];
            for (unsigned n_i = 0; n_i < set_K_size; ++n_i) {
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

//void Searching::search_in_sequential(
//        const idi query_id,
//        const idi K,
//        const idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K)
//{
////    {//test
////        printf("Iteration: Relative_Distance:\n");
//////        printf("Iteration: Relative_Distance:\n");
//////        printf("----query: %u----\n", query_id);
////    }
//    boost::dynamic_bitset<> is_visited(num_v_);
//
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        is_visited[init_ids[v_i]] = true;
//    }
//
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
//        ++count_distance_computation_;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//    idi k = 0; // Index of every queue's first unchecked candidate.
//    idi tmp_count = 0; // for debug
//
////    {// Print relative distance
//////        distf top_dist = set_L[0].distance_;
////        for (idi i_l = 0; i_l < L; ++i_l) {
////            printf("%u %f\n",
////                   tmp_count, set_L[i_l].distance_);
//////                   tmp_count, set_L[i_l].distance_ - top_dist);
////        }
////    }
//
//    while (k < L) {
//        Candidate &top_cand = set_L[k];
//        unsigned nk = L;
//        if (!top_cand.is_checked_) {
//            ++tmp_count;
//
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
//                ++count_distance_computation_;
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
//
////            {// Print relative distance
//////                distf top_dist = set_L[0].distance_;
////                for (idi i_l = 0; i_l < L; ++i_l) {
////                    printf("%u %f\n",
////                           tmp_count, set_L[i_l].distance_);
//////                           tmp_count, set_L[i_l].distance_ - top_dist);
////                }
////            }
//        }
//        if (nk <= k) {
//            k = nk;
//        } else {
//            ++k;
//        }
//
//
//    }
////    cache_miss_kernel.measure_stop();
//
//    for (size_t k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
//
////    {//test
////        if (0 == query_id) {
////            exit(1);
////        }
////    }
//}

//void Searching::search_in_sequential_BitVector(
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
void Searching::prepare_init_ids(
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
    idi *out_edges = common_nsg_deg_ngbrs_ + common_nsg_vertex_base_[ep_];
//    idi *out_edges = (idi *) (opt_nsg_graph_ + ep_ * vertex_bytes_ + data_bytes_);
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
dataf Searching::compute_norm(
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

dataf Searching::compute_distance_with_norm(
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

/*
 * Return = (v - q)^2
 * Ref: https://github.com/ZJULearning/nsg/blob/master/include/efanna2e/distance.h
 */
dataf Searching::compute_distance(
        const dataf *v_data,
        const dataf *q_data) const
{
    float result = 0;
#define AVX_L2SQR(addr1, addr2, dest, tmp1, tmp2) \
      tmp1 = _mm256_loadu_ps(addr1);\
      tmp2 = _mm256_loadu_ps(addr2);\
      tmp1 = _mm256_sub_ps(tmp1, tmp2); \
      tmp1 = _mm256_mul_ps(tmp1, tmp1); \
      dest = _mm256_add_ps(dest, tmp1);

    __m256 sum;
    __m256 l0, l1;
    __m256 r0, r1;
    unsigned D = (dimension_ + 7) & ~7U;
    unsigned DR = D % 16;
    unsigned DD = D - DR;
    const float *l = v_data;
    const float *r = q_data;
    const float *e_l = l + DD;
    const float *e_r = r + DD;
    float unpack[8] __attribute__ ((aligned (32))) = {0, 0, 0, 0, 0, 0, 0, 0};

    sum = _mm256_load_ps(unpack);
    if(DR) { AVX_L2SQR(e_l, e_r, sum, l0, r0); }

    for (unsigned i = 0; i < DD; i += 16, l += 16, r += 16) {
        AVX_L2SQR(l, r, sum, l0, r0);
        AVX_L2SQR(l + 8, r + 8, sum, l1, r1);
    }
    _mm256_store_ps(unpack, sum);
    result = unpack[0] + unpack[1] + unpack[2] + unpack[3] + unpack[4] + unpack[5] + unpack[6] + unpack[7];

    return result;
}

//// DEPRECATED.
// The difference from insert_into_queue is that add_into_queue will increase the queue size by 1.
//idi Searching::add_into_queue(
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
idi Searching::add_into_queue(
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
idi Searching::add_into_queue(
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
    auto it_loc = std::lower_bound(queue.begin() + queue_start, queue.begin() + queue_end, cand);
//    auto it_loc = std::lower_bound(queue.begin(), queue.begin() + queue_top, cand);
    idi insert_loc = it_loc - queue.begin();

    if (queue_top < queue_size) {
        // Queue is not full
        if (insert_loc == queue_end) {
            // Insert at the end
            queue[insert_loc] = cand;
            ++queue_top;
            return queue_top - 1;
        }
    } else {
        // Queue is full
        if (insert_loc == queue_end) {
            return queue_size;
        }
        --queue_top;
        --queue_end;
    }

    if (cand.id_ == it_loc->id_) {
        // Duplicate
        return queue_size;
    }
    // Add into queue
    memmove(reinterpret_cast<char *>(queue.data() + insert_loc + 1),
            reinterpret_cast<char *>(queue.data() + insert_loc),
            (queue_end - insert_loc) * sizeof(Candidate));
    queue[insert_loc] = cand;
    ++queue_top;
    return insert_loc - queue_start;

//    ////////////////
//    if (insert_loc == queue_size + queue_start) {
//        return queue_size;
//    }
//
//    if (cand.id_ == it_loc->id_) {
//        // Duplicate
//        return queue_size;
//    }
//
//    // Insert
//    if (queue_top == queue_size) {
//        // If full already
//        --queue_top;
//        --queue_end;
//    }
//    memmove(reinterpret_cast<char *>(queue.data() + insert_loc + 1),
//            reinterpret_cast<char *>(queue.data() + insert_loc),
//            (queue_end - insert_loc) * sizeof(Candidate));
//    queue[insert_loc] = cand;
//    ++queue_top;
//    return insert_loc - queue_start;
}


void Searching::add_into_queue_at(
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

void Searching::insert_one_element_at(
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
idi Searching::insert_into_queue(
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

//void Searching::cand_pushes_ngbrs_into_queue(
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

//void Searching::search_in_sequential(
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
//void Searching::search_in_sequential(
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
idi Searching::merge_two_queues_into_1st_queue_seq_fixed(
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
//    insert_one_element_at(
//            queue2[queue2_start],
//            queue1,
//            insert_index,
//            queue1_start,
//            queue1_size);
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
//    for (idi insert_i = insert_index + 1; insert_i < q_i_1_bound; ++insert_i) {
        if (q_i_1 >= q_i_1_bound || q_i_2 >= q_i_2_bound) {
            // queue1 or queue2 finished traverse. Rest o
            break;
        } else if (queue1[q_i_1] < queue2[q_i_2]) {
            ++q_i_1;
        } else if (queue2[q_i_2] < queue1[q_i_1]) {
            // Insert queue2[q_i_2] into queue1
            insert_one_element_at(
                    queue2[q_i_2++],
//                    queue1.data(),
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
//        else {
//            // Insert queue2[q_i_2] into queue1
//            insert_one_element_at(
//                    queue2[q_i_2++],
////                    queue1.data(),
//                    queue1,
//                    insert_i,
//                    queue1_start,
//                    queue1_size);
//            ++q_i_1;
//        }
    }

    return insert_index;
}

/* Function:
 * queue1_size should be updated.
 * queue1_length should be provided.
 */
void Searching::merge_two_queues_into_1st_queue_seq_incr(
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
//    add_into_queue_at(
//            queue2[queue2_start],
//            queue1,
//            insert_index,
//            queue1_start,
//            queue1_size,
//            queue1_length);
    if (queue2_size == 1) {
        return;
    }

    // Insert
    idi q_i_1 = insert_index + 1 + queue1_start;
    idi q_i_2 = queue2_start + 1;
//    const idi q_i_1_bound = queue1_start + queue1_size;
    idi q_i_1_bound = queue1_start + queue1_size; // When queue1_size is updated, so should be q_i_1_bound.
    const idi q_i_2_bound = queue2_start + queue2_size;
    idi insert_i;
    for (insert_i = insert_index + 1; insert_i < queue1_length; ++insert_i) {
//    for (idi insert_i = insert_index + 1; insert_i < queue1_size; ++insert_i) {
        if (q_i_1 >= q_i_1_bound) {
            idi remain = std::min(queue1_length - insert_i, q_i_2_bound - q_i_2);
            for (idi i_r = 0; i_r < remain; ++i_r) {
                queue1[queue1_start + insert_i + i_r] = queue2[q_i_2 + i_r];
            }
            queue1_size += remain;
//            queue1_size += std::min(queue1_length - insert_i, q_i_2_bound - q_i_2);
//            while (insert_i < queue1_length && q_i_2 < q_i_2_bound) {
//                queue1[queue1_start + insert_i++] = queue2[q_i_2++];
//            }
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
//        else {
//            add_into_queue_at(
//                    queue2[q_i_2++],
//                    queue1,
//                    insert_i,
//                    queue1_start,
//                    queue1_size,
//                    queue1_length);
//            ++q_i_1;
//            q_i_1_bound = queue1_start + queue1_size;
//        }
    }
}

idi Searching::merge_all_queues_para_list(
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
idi Searching::merge_all_queues_para_array(
//        std::vector< std::vector<Candidate> > &local_queues_list,
        std::vector<Candidate> &set_L,
//        std::vector<Candidate> &local_queues_array,
        std::vector<idi> &local_queues_ends,
        const idi local_queue_length,
//        std::vector<Candidate> &set_L,
        const idi L)
{
    idi nk = L;
    int size = 1 << (static_cast<idi>(log2(num_threads_)));
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
    if (size != num_threads_) {
        for (int i = size; i < num_threads_; ++i) {
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
    // Not do this for Collector Idea or Selecting Idea
    std::fill(local_queues_ends.begin(), local_queues_ends.end() - 1, 0);
//    std::fill(local_queues_ends.begin(), local_queues_ends.end(), 0);

    return nk;
//    return r;
}

idi Searching::merge_all_queues_seq(
        std::vector<Candidate> &set_L,
        std::vector<idi> &local_queues_ends,
        const idi local_queue_length,
        const idi L)
{
    idi nk = L;
    for (int i = 1; i < num_threads_; ++i) {
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
        local_queues_ends[bi] = 0;
    }
    // Reset local_queues_ends
    // Not do this for Collector Idea or Selecting Idea
//    std::fill(local_queues_ends.begin(), local_queues_ends.end() - 1, 0);
//    std::fill(local_queues_ends.begin(), local_queues_ends.end(), 0);

    return nk;
//    return r;
}

/* Function:
 * When merge all queues (in an array, and [num_threads_ - 1] is the global queue),
 * the starting local is at [queue_base]
 */
idi Searching::merge_all_queues_queue_base(
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

///* Function:
// * Merge all queues to the global queue, in a two-queue-merge way
// */
//idi Searching::merge_all_queues_all_together_in_sequential(
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
//        time_compare_minimum_ -= WallTimer::get_time_mark();
//        queue_id_min = min_all_queues_at_heads(
//                set_L,
//                queue_heads,
//                local_queues_ends,
//                local_queue_length,
//                L);
//        time_compare_minimum_ += WallTimer::get_time_mark();
//        if (queue_id_min != num_queues - 1) { // Not in the global queue
//            time_insert_ -= WallTimer::get_time_mark();
//            insert_one_element_at(
//                    set_L[queue_heads[queue_id_min] + queue_id_min * local_queue_length],
//                    set_L,
//                    queue_heads[num_queues - 1],
//                    global_queue_base,
//                    L);
//            time_insert_ += WallTimer::get_time_mark();
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

/* Function:
 * Find the minimum among queues at their head locations
 */
idi Searching::min_all_queues_at_heads(
        const std::vector<Candidate> &set_L,
        std::vector<idi> &queue_heads,
        const std::vector<idi> &local_queues_ends,
        const idi local_queue_length,
        const idi L)
{
    const idi num_queues = num_threads_;
    idi min_queue_id = num_queues - 1;
    Candidate min_candidate = set_L[queue_heads[min_queue_id] + min_queue_id * local_queue_length];

    for (idi q_i = 0; q_i < num_queues - 1; ++q_i) {
        if (queue_heads[q_i] >= local_queues_ends[q_i]) { // q_i finished
            continue;
        }
        const Candidate &ele = set_L[queue_heads[q_i] + q_i * local_queue_length];
        if (ele < min_candidate) {
            min_candidate = ele;
            min_queue_id = q_i;
        } else if (ele.id_ == min_candidate.id_) { // Redundant element
            ++queue_heads[q_i];
        }
    }

    return min_queue_id;
}

//void Searching::search_with_top_m(
//        const PANNS::idi M,
//        const PANNS::idi query_id,
//        const PANNS::idi K,
//        const PANNS::idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K)
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
//                ++count_distance_computation_;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
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
//}
//
//void Searching::search_with_top_m_scale_m(
//        const PANNS::idi value_M_max,
//        const PANNS::idi query_id,
//        const PANNS::idi K,
//        const PANNS::idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K,
//        std::vector<idi> &top_m_candidates,
//        boost::dynamic_bitset<> &is_visited)
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
//        ++count_distance_computation_;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
//
////    std::vector<idi> top_m_candidates(M);
//    idi top_m_candidates_end = 0;
//    idi k = 0; // Index of first unchecked candidate.
//    idi tmp_count = 0; // for debug
//    idi M = 1;
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
//                ++count_distance_computation_;
//                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                if (dist > set_L[L-1].distance_) {
//                    continue;
//                }
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
//
//        if (M < value_M_max) {
//            M <<= 1;
//        }
//    }
//
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
//    {// Reset
//        is_visited.reset();
//    }
//}

////void Searching::search_with_top_m(
//void Searching::search_with_top_m_to_get_distance_range(
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
//void Searching::search_with_top_m_myths_M(
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
//void Searching::search_with_top_m_profile_bit_CAS(
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
//void Searching::search_with_top_m(
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
//void Searching::search_with_top_m_no_local_arrays(
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


//void Searching::search_with_top_m_in_batch(
//        const PANNS::idi M,
//        const PANNS::idi batch_start,
//        const PANNS::idi batch_size,
//        const PANNS::idi K,
//        const PANNS::idi L,
//        std::vector< std::vector<Candidate> > &set_L_list,
//        const std::vector<idi> &init_ids,
//        std::vector< std::vector<idi> > &set_K_list)
//{
//    std::vector< boost::dynamic_bitset<> > is_visited_list(batch_size, boost::dynamic_bitset<> (num_v_));
//
//    // Prepare the init_ids
//    {
////#pragma omp parallel for
//        for (idi q_i = 0; q_i < batch_size; ++q_i) {
//            auto &is_visited = is_visited_list[q_i];
//            for (idi c_i = 0; c_i < L; ++c_i) {
//                is_visited[init_ids[c_i]] = true;
//            }
//        }
//    }
//
//    // Initialize set_L_list
//    {
////#pragma omp parallel for
//        for (idi q_i = 0; q_i < batch_size; ++q_i) {
//            const dataf *query_data = queries_load_ + (q_i + batch_start) * dimension_;
//            for (idi i = 0; i < L; i++) {
//                idi v_id = init_ids[i];
//                auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//                dataf norm = *v_data++;
////                ++count_distance_computation_;
//                distf dist = compute_distance_with_norm(v_data, query_data, norm);
//                set_L_list[q_i][i] = Candidate(v_id, dist, false); // False means not checked.
//            }
//            std::sort(set_L_list[q_i].begin(), set_L_list[q_i].begin() + L);
//        }
//    }
//
//    {
//        std::vector<idi> joint_queue(M * batch_size); // Joint queue for all shared top-M candidates
//        idi joint_queue_end = 0;
//        boost::dynamic_bitset<> is_in_joint_queue(num_v_);
////        std::vector< std::vector<idi> > cands_query_ids(num_v_, std::vector<idi>(batch_size)); // If candidate cand_id is selected by query q_i, q_i should be in cands_query_ids[cand_id].
////        std::vector<idi> cands_query_ids_ends(num_v_, 0);
//        std::unordered_map< idi, std::vector<idi> > cands_query_ids(batch_size * M);
//        std::vector<idi> ks(batch_size, 0); // Indices of every queue's first unchecked candidate.
//        std::vector<idi> nks(batch_size, L); // Indices of highest candidate inserted
//        std::vector<idi> last_ks(batch_size, L); // Indices of lowest candidate unchecked
//        std::vector<idi> queries_not_finished(batch_size);
//        idi queries_not_finished_end = batch_size;
//        for (idi q_i = 0; q_i < batch_size; ++q_i) {
//            queries_not_finished[q_i] = q_i;
//        }
//        bool is_finished = false;
//
//        idi counter_for_debug = 0;
//
//        while (!is_finished) {
//            ++counter_for_debug;
//            // Build the new joint queue
//            // Traverse every query's queue
//            for(idi q_i = 0; q_i < queries_not_finished_end; ++q_i) {
//                idi q_local_id = queries_not_finished[q_i];
////                last_ks[q_local_id] = L;
//                auto &set_L = set_L_list[q_local_id];
//                idi top_m_count = 0;
//                for (idi c_i = ks[q_local_id]; c_i < L && top_m_count < M; ++c_i) {
//                    if (set_L[c_i].is_checked_) {
//                        continue;
//                    }
//                    set_L[c_i].is_checked_ = true;
//                    last_ks[q_local_id] = c_i;
//                    ++top_m_count;
//                    idi cand_id = set_L[c_i].id_;
//                    // Record which query selected cand_id
//                    auto tmp_c = cands_query_ids.find(cand_id);
//                    if (tmp_c != cands_query_ids.end()) {
//                        tmp_c->second.push_back(q_local_id);
//                    } else {
//                        cands_query_ids.emplace(cand_id, std::vector<idi>());
//                        cands_query_ids[cand_id].reserve(batch_size);
//                        cands_query_ids[cand_id].push_back(q_local_id);
//                    }
////                    cands_query_ids[cand_id][cands_query_ids_ends[cand_id]++] = q_local_id;
//                    // Add candidate cand_id into the joint queue
//                    if (is_in_joint_queue[cand_id]) {
//                        continue;
//                    }
//                    is_in_joint_queue[cand_id] = true;
//                    joint_queue[joint_queue_end++] = cand_id;
//                }
//            }
//            queries_not_finished_end = 0; // Clear queries_not_finished
//
//            // Traverse every shared candidate
//            for (idi c_i = 0; c_i < joint_queue_end; ++c_i) {
//                idi cand_id = joint_queue[c_i];
//                is_in_joint_queue[cand_id] = false; // Reset is_in_joint_queue
//                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
//                idi out_degree = *out_edges++;
//                const auto &query_local_ids = cands_query_ids[cand_id];
//                // Push neighbors to every queue of the queries that selected cand_id.
//                // Traverse cand_id's neighbors
////                idi &q_i_bound = cands_query_ids_ends[cand_id];
//
////                for (idi q_i = 0; q_i < q_i_bound; ++q_i) {
////                    idi q_local_id = query_local_ids[q_i];
//                for (idi q_local_id : query_local_ids) {
//                    dataf *query_data = queries_load_ + (q_local_id + batch_start) * dimension_;
//                    auto &is_visited = is_visited_list[q_local_id];
//                    auto &set_L = set_L_list[q_local_id];
////                    // Traverse cand_id's neighbors
//                    for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                        idi nb_id = out_edges[e_i];
//                        if (is_visited[nb_id]) {
//                            continue;
//                        }
//                        is_visited[nb_id] = true;
//                        auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                        dataf norm = *nb_data++;
////                        ++count_distance_computation_;
//                        distf dist = compute_distance_with_norm(nb_data, query_data, norm);
//                        if (dist > set_L[L-1].distance_) {
//                            continue;
//                        }
////                        if (dist >= set_L[L-1].distance_) {
////                            continue;
////                        }
//                        Candidate new_cand(nb_id, dist, false);
//                        idi insert_loc = insert_into_queue(set_L, L, new_cand);
//                        if (insert_loc < nks[q_local_id]) {
//                            nks[q_local_id] = insert_loc;
//                        }
//                    }
//                }
//                cands_query_ids.erase(cand_id);
////                q_i_bound = 0; // Clear cands_query_ids[cand_id]
//            }
//            joint_queue_end = 0; //  Clear joint_queue
//            for (idi q_local_id = 0; q_local_id < batch_size; ++q_local_id) {
//                if (nks[q_local_id] <= last_ks[q_local_id]) {
//                    ks[q_local_id] = nks[q_local_id];
//                } else {
//                    ks[q_local_id] = last_ks[q_local_id] + 1;
//                }
//                nks[q_local_id] = L;
//                last_ks[q_local_id] = L;
//                if (ks[q_local_id] < L) {
//                    queries_not_finished[queries_not_finished_end++] = q_local_id;
//                }
//            }
//            if (!queries_not_finished_end) {
//                is_finished = true;
//            }
//        }
//    }
//
//    {
//        for (idi q_i = 0; q_i < batch_size; ++q_i) {
//            for (idi c_i = 0; c_i < K && c_i < L; ++c_i) {
//                set_K_list[q_i + batch_start][c_i] = set_L_list[q_i][c_i].id_;
//            }
//        }
//    }
//////
////    {//test
////        for (idi q_i = 0; q_i < batch_size; ++q_i) {
////            printf("query: %u\n", q_i + batch_start);
////            for (idi c_i = 0; c_i < K; ++c_i) {
////                printf("%u: %u %f\n", c_i, set_L_list[q_i][c_i].id_, set_L_list[q_i][c_i].distance_);
////            }
////        }
////    }
//}



//void Searching::para_search_with_top_m_critical_area(
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
//void Searching::para_search_with_top_m_critical_area_no_omp(
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
//void Searching::para_search_with_top_m_critical_area_yes_omp(
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
//void Searching::para_search_with_top_m_visited_array(
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
//void Searching::para_search_with_top_m_merge_queues(
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
//void Searching::para_search_with_top_m_queues_seq_merge(
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
//void Searching::para_search_with_top_m_merge_queues_no_CAS(
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

//void Searching::para_search_with_top_m_merge_queues_in_array(
//void Searching::para_search_with_top_m_merge_queues_new_threshold(
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
//void Searching::para_search_with_top_m_merge_queues_middle_m(
//        const idi value_M_middle,
void Searching::para_search_with_scale_m_large_graph_profiling(
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
        boost::dynamic_bitset<> &is_visited)
//        std::vector<distf> &local_thresholds)
//        BitVector &is_visited)
{
#ifdef BREAKDOWN_PRINT
    time_seq_ -= WallTimer::get_time_mark();
#endif
//    const idi base_set_L = (num_threads_ - 1) * local_queue_length;
    {
//#pragma omp parallel for
        for (idi c_i = 0; c_i < L; ++c_i) {
            is_visited[init_ids[c_i]] = 1;
        }
    }

    const dataf *query_data = queries_load_ + query_id  * dimension_;
//#pragma omp parallel for
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
    uint64_t tmp_count_computation = 0;
    // Get the distances of all candidates, store in the set set_L.
//#pragma omp parallel for
#pragma omp parallel for reduction(+ : tmp_count_computation)
    for (unsigned i = 0; i < L; i++) {
        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
//        ++tmp_count_computation;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
        auto *v_data = data_load_ + v_id * dimension_;
        ++tmp_count_computation;
        distf dist = compute_distance(v_data, query_data);
        set_L[i + base_set_L] = Candidate(v_id, dist, false); // False means not checked.
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
    }
    count_distance_computation_ += tmp_count_computation;
    tmp_count_computation = 0;
//    std::sort(set_L.begin(), set_L.begin() + L);
    std::sort(
            set_L.begin() + base_set_L,
            set_L.begin() + base_set_L + L);
//    boost::sort::block_indirect_sort(
//            set_L.begin() + base_set_L,
//            set_L.begin() + base_set_L + L,
//            num_threads_);
    local_queues_ends[num_threads_ - 1] = L;

//    std::vector<idi> top_m_candidates(M);
    idi top_m_candidates_end = 0;
    idi k = 0; // Index of first unchecked candidate.
    idi tmp_count = 0; // for debug
    idi M = 1;

#ifdef BREAKDOWN_PRINT
    time_seq_ += WallTimer::get_time_mark();
#endif
    { // Multiple Threads
        while (k < L) {
#ifdef BREAKDOWN_PRINT
            time_seq_ -= WallTimer::get_time_mark();
#endif
            ++tmp_count;
//        {//test
//            printf("tmp_count: %d\n", tmp_count);
//        }
//            int real_threads = std::min(static_cast<int>(M), num_threads_);
//            idi queue_base = num_threads_ - real_threads;
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
#ifdef BREAKDOWN_PRINT
            time_seq_ += WallTimer::get_time_mark();
            time_expand_ -= WallTimer::get_time_mark();
#endif
            // Expand
            idi nk = L;
            // Push M candidates' neighbors into the queue.
//#pragma omp parallel for reduction(+ : tmp_count_computation) num_threads(real_threads)
#pragma omp parallel for reduction(+ : tmp_count_computation)
//#pragma omp parallel for schedule(dynamic, 1) reduction(+ : tmp_count_computation)
            for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
                int tid = omp_get_thread_num();
                idi cand_id = top_m_candidates[c_i];
//                _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
                idi *out_edges = common_nsg_deg_ngbrs_ + common_nsg_vertex_base_[cand_id];
//                idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
                idi out_degree = *out_edges++;
//                for (idi n_i = 0; n_i < out_degree; ++n_i) {
//                    _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//                }
                for (idi e_i = 0; e_i < out_degree; ++e_i) {
                    idi nb_id = out_edges[e_i];
                    { // Sequential edition
                        if (is_visited[nb_id]) {
                            continue;
                        }
                        is_visited[nb_id] = 1;
                    }

//                    auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                    dataf norm = *nb_data++;
//                    ++tmp_count_computation;
//                    distf dist = compute_distance_with_norm(nb_data, query_data, norm);
                    dataf *nb_data = data_load_ + nb_id * dimension_;
                    ++tmp_count_computation;
                    distf dist = compute_distance(nb_data, query_data);
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

#ifdef BREAKDOWN_PRINT
            time_expand_ += WallTimer::get_time_mark();
            time_merge_ -= WallTimer::get_time_mark();
#endif

//        // Merge. Merge all queues in parallel.
            {
                ++count_merge_;
                if (num_threads_ > 1) {
//                    idi r = merge_all_queues_seq(
//                            set_L,
//                            local_queues_ends,
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
                }
//                else {
//                    M = value_M_max;
//                }
            }
#ifdef BREAKDOWN_PRINT
            time_merge_ += WallTimer::get_time_mark();
#endif
        }
    }
#ifdef BREAKDOWN_PRINT
    time_seq_ -= WallTimer::get_time_mark();
#endif
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
#ifdef BREAKDOWN_PRINT
    time_seq_ += WallTimer::get_time_mark();
#endif
}

} // namespace PANNS
