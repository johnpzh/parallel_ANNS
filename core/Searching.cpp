//
// Created by Zhen Peng on 11/11/19.
//


#include "Searching.h"

namespace PANNS {

///**
// * Prepare init_ids and flags, as they are constant for all queries.
// * @param[out] init_ids
// * @param[out] is_visited
// * @param L
// */
//void Searching::prepare_init_ids(
//        std::vector<unsigned int> &init_ids,
//        boost::dynamic_bitset<> &is_visited,
//        unsigned L) const
//{
////    idi num_ngbrs = get_out_degree(ep_);
////    edgei edge_start = nsg_graph_indices_[ep_];
////    // Store ep_'s neighbors as candidates
////    idi tmp_l = 0;
////    for (; tmp_l < L && tmp_l < num_ngbrs; tmp_l++) {
////        init_ids[tmp_l] = nsg_graph_out_edges_[edge_start + tmp_l];
////    }
//    idi *out_edges = (idi *) (opt_nsg_graph_ + ep_ * vertex_bytes_ + data_bytes_);
//    unsigned out_degree = *out_edges++;
//    idi tmp_l = 0;
//    for (; tmp_l < L && tmp_l < out_degree; tmp_l++) {
//        init_ids[tmp_l] = out_edges[tmp_l];
//    }
//
//    for (idi i = 0; i < tmp_l; ++i) {
//        is_visited[init_ids[i]] = true;
//    }
//
//    // If ep_'s neighbors are not enough, add other random vertices
//    idi tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    while (tmp_l < L) {
//        tmp_id %= num_v_;
//        unsigned id = tmp_id++;
//        if (is_visited[id]) {
//            continue;
//        }
//        is_visited[id] = true;
//        init_ids[tmp_l] = id;
//        tmp_l++;
//    }
//}

//void Searching::search_in_batch(
//        size_t K,
//        size_t L,
//        unsigned batch_start,
//        unsigned batch_size,
//        std::vector<std::vector<Candidate> > &set_L_list,
//        std::vector<boost::dynamic_bitset<> > &is_visited_list,
//        const std::vector<idi> &init_ids,
//        const boost::dynamic_bitset<> &is_visited,
//        std::vector<std::vector<idi>> &set_K_list)
//{
////    auto *dist_fast = reinterpret_cast<DistanceFastL2 *>(distance_);
//    std::vector<boost::dynamic_bitset<> > is_checked_list(batch_size, boost::dynamic_bitset<>(num_v_));
//
//    for (unsigned q_i = 0; q_i < batch_size; ++q_i) {
//        is_visited_list[q_i] = is_visited;
//    }
////        for (unsigned i = 0; i < init_ids.size(); i++) {
////            unsigned id = init_ids[i];
//////            if (id >= nd_) continue;
////            _mm_prefetch(opt_graph_ + node_size * id, _MM_HINT_T0);
////        }
//    for (unsigned q_i = 0; q_i < batch_size; ++q_i) {
//        // Get the distances of all candidates, store in the set retset.
//        const idi query_start = (q_i + batch_start) * dimension_;
//        for (unsigned i = 0; i < L; i++) {
//            unsigned id = init_ids[i];
//            distf dist = compute_distance_with_norm(
//                    data_load_,
//                    queries_load_,
//                    id * dimension_,
//                    query_start,
//                    norms_[id],
//                    dimension_);
//            set_L_list[q_i][i] = Candidate(dist, id);
////            set_L_list[q_i][i] = efanna2e::Neighbor(id, dist, true);
//        }
////        std::sort(retset_list[q_i].begin(), retset_list[q_i].begin() + L);
//    }
//
//    for (auto &set : set_L_list) {
//        std::sort(set.begin(), set.begin() + L);
//    }
//
////    auto *dist_fast = (DistanceFastL2 *) distance_;
//
//    std::vector<unsigned> ks(batch_size, 0); // Index of every queue's first unchecked candidate.
//    boost::dynamic_bitset<> is_finished(batch_size);
//    while (!is_finished.all()) {
//        // Traverse all every query in the batch
//        for (unsigned q_i = 0; q_i < batch_size; ++q_i) {
//            if (is_finished[q_i]) {
//                continue;
//            }
////            const float *query = query_load + (q_i + batch_start) * dimension_;
//            const idi query_start = (q_i + batch_start) * dimension_;
//            auto &retset = set_L_list[q_i];
//            unsigned k = ks[q_i];
//            unsigned nk = L;
//            idi v_id = retset[k].second;
//            if (!is_checked_list[q_i][v_id]) {
//                is_checked_list[q_i][v_id] = true;
////                unsigned n = retset[k].id;
//
////                _mm_prefetch(opt_graph_ + node_size * n + data_len, _MM_HINT_T0);
////                auto *ngbrs = (unsigned *) (opt_graph_ + node_size * n + data_len);
////                unsigned MaxM = *ngbrs;
////                ngbrs++;
////                _mm_prefetch((char *) final_graph_[n].data(), _MM_HINT_T0);
////                unsigned *ngbrs = final_graph_[n].data();
////                unsigned MaxM = final_graph_[n].size();
//                const auto &ngbrs = nsg_graph_[v_id];
//                const idi out_degree = ngbrs.size();
//                // Prefetch is not necessarily better for performance.
////                for (unsigned m = 0; m < MaxM; ++m)
////                    _mm_prefetch(opt_graph_ + node_size * ngbrs[m], _MM_HINT_T0);
//                for (unsigned m = 0; m < out_degree; ++m) {
//                    unsigned id = ngbrs[m];
//                    if (is_visited_list[q_i][id]) {
//                        continue;
//                    }
//                    is_visited_list[q_i][id] = true;
//                    distf dist = compute_distance_with_norm(
//                            data_load_,
//                            queries_load_,
//                            id * dimension_,
//                            query_start,
//                            norms_[id],
//                            dimension_);
//                    if (dist >= retset[L - 1].first) continue;
////                    efanna2e::Neighbor nn(id, dist, true);
//                    Candidate nn(dist, id);
////                    unsigned r = InsertIntoPool(retset.data(), L, nn); // insert location
//                    idi r = insert_into_queue_panns(retset, L, nn);
////                    idi r = insert_into_queue_nsg(retset, L, nn);
//                    if (r < nk) nk = r;
//                }
//            }
//            if (nk <= k) {
//                k = nk;
//            } else {
//                ++k;
//            }
//            if (k >= L) {
//                is_finished[q_i] = true;
//            } else {
//                ks[q_i] = k;
//            }
//        }
//    }
//
//    for (unsigned q_i = 0; q_i < batch_size; ++q_i) {
//        for (size_t k_i = 0; k_i < K; ++k_i) {
//            set_K_list[q_i + batch_start][k_i] = set_L_list[q_i][k_i].second;
//        }
//    }
//}

//void Searching::search_in_sequential(
//        idi query_id,
////        const dataf *query_data,
////        idi query_start,
//        idi K,
//        idi L,
//        std::vector<Candidate> &set_L,
//        boost::dynamic_bitset<> is_visited,
////        boost::dynamic_bitset<> &is_checked,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K)
//{
////    const std::vector<dataf> &query = queries_load_[query_id];
////    std::vector<char> is_checked(L + 1, 0);
////    boost::dynamic_bitset<> is_checked(num_v_);
//    cache_miss_kernel.measure_stop();
////    cache_miss_kernel.measure_start();
//    const dataf *query_data = queries_load_ + query_id  * dimension_;
//
//    for (idi v_i = 0; v_i < L; ++v_i) {
//        idi v_id = init_ids[v_i];
////        _mm_prefetch(reinterpret_cast<char *>(data_load_ + v_id * dimension_), _MM_HINT_T0);
//        _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//    }
//    // Get the distances of all candidates, store in the set retset.
//    for (unsigned i = 0; i < L; i++) {
//        unsigned v_id = init_ids[i];
//        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//        dataf norm = *v_data++;
//        distf dist = compute_distance_with_norm(v_data, query_data, norm);
//        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//    }
//    std::sort(set_L.begin(), set_L.begin() + L);
////    cache_miss_kernel.measure_stop();
////    cache_miss_kernel.measure_start();
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
//                idi r = insert_into_queue_panns(set_L, L, cand);
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
//
//    for (size_t k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i].id_;
//    }
//}

/**
 * Input the data from the file.
 * @param filename
 */
void Searching::load_data_load(char *filename)
{
    auto old_d = dimension_;
    load_data(
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
    load_data(
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


//void Searching::build_opt_graph()
//{
//    data_bytes_ = (1 + dimension_) * sizeof(dataf);
//    neighbor_bytes_ = (1 + width_) * sizeof(idi);
//    vertex_bytes_ = data_bytes_ + neighbor_bytes_;
//    opt_nsg_graph_ = (char *) malloc(num_v_ * vertex_bytes_);
//    if (!opt_nsg_graph_) {
//        std::cerr << "Error: no enough memory for opt_nsg_graph_." << std::endl;
//        exit(EXIT_FAILURE);
//    }
//
//    for (idi v_id = ep_; v_id < num_v_; ++v_id) {
////    for (size_t v_id = 0; v_id < num_v_; ++v_id) {
//        char *base_location = opt_nsg_graph_ + v_id * vertex_bytes_;
////        idi degree = get_out_degree(v_id);
//
//        // Norm and data
//        distf norm = compute_norm(v_id);
//        memcpy(base_location, &norm, sizeof(distf)); // Norm
//        memcpy(base_location + sizeof(distf), data_load_ + v_id * dimension_, dimension_ * sizeof(dataf)); // Data
//        base_location += data_bytes_;
//
//        // Neighbors
//        idi degree = edge_list_[v_id].size();
//        memcpy(base_location, &degree, sizeof(idi)); // Number of neighbors
////        memcpy(base_location + sizeof(idi), &nsg_graph_out_edges_[nsg_graph_indices_[v_id]], degree * sizeof(unsigned)); // Neighbors
//        memcpy(base_location + sizeof(idi), edge_list_[v_id].data(), degree * sizeof(unsigned));
//    }
//
//}

/**
 * Input the NSG graph from the file.
 * Reference: https://github.com/ZJULearning/nsg/blob/master/src/index_nsg.cpp
 * @param filename
 */
void Searching::load_nsg_graph(char *filename)
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

//// TODO: re-code in AVX-512
//inline dataf Searching::compute_norm(
//        const dataf *data)
////        idi vertex_id)
////        const std::vector<PANNS::dataf> &data)
////        size_t loc_start,
////        idi dimension)
//{
////    const dataf *a = data.data() + loc_start;
////    const dataf *a = data_load_ + vertex_id * dimension_;
////    idi size = dimension_;
//    dataf result = 0;
////#define AVX_L2NORM(addr, dest, tmp) \
////    tmp = _mm256_load_ps(addr); \
////    tmp = _mm256_mul_ps(tmp, tmp); \
////    dest = _mm256_add_ps(dest, tmp);
//#define AVX_L2NORM(addr, dest, tmp) \
//    tmp = _mm256_loadu_ps(addr); \
//    tmp = _mm256_mul_ps(tmp, tmp); \
//    dest = _mm256_add_ps(dest, tmp);
//
//    __m256 sum;
//    __m256 l0, l1;
//    unsigned D = (dimension_ + 7) & ~7U;
//    unsigned DR = D % 16;
//    unsigned DD = D - DR;
//    const float *l = data;
//    const float *e_l = l + DD;
//    float unpack[8] __attribute__ ((aligned (32))) = {0, 0, 0, 0, 0, 0, 0, 0};
//
//    sum = _mm256_load_ps(unpack);
////    sum = _mm256_loadu_ps(unpack);
//    if (DR) { AVX_L2NORM(e_l, sum, l0); }
//    for (unsigned i = 0; i < DD; i += 16, l += 16) {
//        AVX_L2NORM(l, sum, l0);
//        AVX_L2NORM(l + 8, sum, l1);
//    }
//    _mm256_store_ps(unpack, sum);
////    _mm256_storeu_ps(unpack, sum);
//    result = unpack[0] + unpack[1] + unpack[2] + unpack[3] + unpack[4] + unpack[5] + unpack[6] + unpack[7];
//
//    return result;
//}
//
//inline dataf Searching::compute_distance_with_norm(
//        const dataf *v_data,
//        const dataf *q_data,
////        idi vertex_id,
////        idi query_id,
////        const std::vector<PANNS::dataf> &d_data,
////        const std::vector<PANNS::dataf> &q_data,
////        PANNS::idi d_start,
////        PANNS::idi q_start,
//        dataf v_norm)
////        idi dimension)
//{
////    idi size = dimension_;
//    float result = 0;
////#define AVX_DOT(addr1, addr2, dest, tmp1, tmp2) \
////          tmp1 = _mm256_load_ps(addr1);\
////          tmp2 = _mm256_load_ps(addr2);\
////          tmp1 = _mm256_mul_ps(tmp1, tmp2); \
////          dest = _mm256_add_ps(dest, tmp1);
//#define AVX_DOT(addr1, addr2, dest, tmp1, tmp2) \
//          tmp1 = _mm256_loadu_ps(addr1);\
//          tmp2 = _mm256_loadu_ps(addr2);\
//          tmp1 = _mm256_mul_ps(tmp1, tmp2); \
//          dest = _mm256_add_ps(dest, tmp1);
//
//    __m256 sum;
//    __m256 l0, l1;
//    __m256 r0, r1;
//    unsigned D = (dimension_ + 7) & ~7U;
//    unsigned DR = D % 16;
//    unsigned DD = D - DR;
//    const float *l = v_data;
//    const float *r = q_data;
////    const float *l = (float *) (opt_nsg_graph_ + vertex_id * vertex_bytes_ + sizeof(distf));
////    const float *r = queries_load_ + query_id * dimension_;
//    const float *e_l = l + DD;
//    const float *e_r = r + DD;
//    float unpack[8] __attribute__ ((aligned (32))) = {0, 0, 0, 0, 0, 0, 0, 0};
//
//    sum = _mm256_load_ps(unpack);
////    sum = _mm256_loadu_ps(unpack);
//    if (DR) { AVX_DOT(e_l, e_r, sum, l0, r0); }
//
//    for (unsigned i = 0; i < DD; i += 16, l += 16, r += 16) {
//        AVX_DOT(l, r, sum, l0, r0);
//        AVX_DOT(l + 8, r + 8, sum, l1, r1);
//    }
//    _mm256_store_ps(unpack, sum);
////    _mm256_storeu_ps(unpack, sum);
//    result = unpack[0] + unpack[1] + unpack[2] + unpack[3] + unpack[4] + unpack[5] + unpack[6] + unpack[7];
//
//    result = -2 * result + v_norm;
//
//    return result;
//}
//
///**
// * PANNS version of InsertIntoPool(): binary-search to find the insert place and then move.
// * @param[out] c_queue
// * @param c_queue_top
// * @param cand
// * @return
// */
//inline idi Searching::insert_into_queue_panns(
//        std::vector<PANNS::Candidate> &c_queue,
//        PANNS::idi c_queue_top,
//        PANNS::Candidate cand)
//{
//    // If the first
////    if (c_queue[0].first > cand.first) {
////    if (std::get<0>(c_queue[0]) > std::get<0>(cand)) {
//    if (c_queue[0].distance_ > cand.distance_) {
//        memmove(reinterpret_cast<char *>(c_queue.data() + 1),
//                reinterpret_cast<char *>(c_queue.data()),
//                c_queue_top * sizeof(Candidate));
//        c_queue[0] = cand;
//        return 0;
//    }
////    // If beyond the last
////    if (c_queue[c_queue_top - 1].first < cand.first) {
////        c_queue[c_queue_top] = cand;
////        return c_queue_top;
////    }
//
//    idi left = 0;
//    idi right = c_queue_top;
//    while (left < right) {
//        idi mid = (right - left) / 2 + left;
////        if (c_queue[mid].first > cand.first) {
////        if (std::get<0>(c_queue[mid]) > std::get<0>(cand)) {
//        if (c_queue[mid].distance_ > cand.distance_) {
//            right = mid;
//        } else {
//            left = mid + 1;
//        }
//    }
//
//    // Insert to left
//    memmove(reinterpret_cast<char *>(c_queue.data() + left + 1),
//            reinterpret_cast<char *>(c_queue.data() + left),
//            (c_queue_top - left) * sizeof(Candidate));
//    c_queue[left] = cand;
//    return left;
//}

///**
// * NSG version of InsertIntoPool()
// * @param[out] c_queue
// * @param c_queue_top
// * @param cand
// * @return
// */
//idi Searching::insert_into_queue_nsg(
//        std::vector<PANNS::Candidate> &c_queue,
//        PANNS::idi c_queue_top,
//        PANNS::Candidate cand)
//{
//    // find the location to insert
//    unsigned left=0,right=c_queue_top-1;
//    if(c_queue[left].first>cand.first){
//        memmove((char *)&c_queue[left+1], &c_queue[left],c_queue_top * sizeof(Candidate));
//        c_queue[left] = cand;
//        return left;
//    }
//    if(c_queue[right].first<cand.first){
//        c_queue[c_queue_top] = cand;
//        return c_queue_top;
//    }
//    while(left<right-1){
//        unsigned mid=(left+right)/2;
//        if(c_queue[mid].first>cand.first)right=mid;
//        else left=mid;
//    }
//    //check equal ID
////
////    while (left > 0){
////        if (c_queue[left].first < cand.first) break;
////        if (c_queue[left].second == cand.second) return c_queue_top + 1;
////        left--;
////    }
////    if(c_queue[left].second == cand.second||c_queue[right].second==cand.second)return c_queue_top+1;
//    memmove((char *)&c_queue[right+1], &c_queue[right],(c_queue_top-right) * sizeof(Candidate));
//    c_queue[right]=cand;
//    return right;
//}

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
        std::unordered_map<unsigned, double> &recalls)
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

void Searching::search_with_top_m(
        PANNS::idi M,
        PANNS::idi query_id,
        PANNS::idi K,
        PANNS::idi L,
        std::vector<Candidate> &set_L,
        std::vector<idi> &init_ids,
        std::vector<idi> &set_K,
        std::vector< std::vector<idi> > &top_m_list) const
{
    boost::dynamic_bitset<> is_visited(num_v_);

    {
        idi *out_edges = (idi *) (opt_nsg_graph_ + ep_ * vertex_bytes_ + data_bytes_);
        unsigned out_degree = *out_edges++;
        idi tmp_l = 0;
        for (; tmp_l < L && tmp_l < out_degree; tmp_l++) {
            init_ids[tmp_l] = out_edges[tmp_l];
        }

        for (idi i = 0; i < tmp_l; ++i) {
            is_visited[init_ids[i]] = true;
        }

        // If ep_'s neighbors are not enough, add other random vertices
        idi tmp_id = ep_ + 1; // use tmp_id to replace rand().
        while (tmp_l < L) {
            tmp_id %= num_v_;
            unsigned id = tmp_id++;
            if (is_visited[id]) {
                continue;
            }
            is_visited[id] = true;
            init_ids[tmp_l] = id;
            tmp_l++;
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
        distf dist = compute_distance_with_norm(v_data, query_data, norm);
        set_L[i] = Candidate(v_id, dist, false); // False means not checked.
    }
    std::sort(set_L.begin(), set_L.begin() + L);

    std::vector<idi> top_m_candidates(M);
    idi top_m_candidates_end = 0;
    idi k = 0; // Index of every queue's first unchecked candidate.
    while (k < L) {

        unsigned nk = L;

        // Select M candidates
        idi last_k = L;
        for (idi c_i = k; c_i < L; ++c_i) {
            if (set_L[c_i].is_checked_) {
                continue;
            }
            last_k = c_i; // Record the location of the last candidate selected.
            set_L[c_i].is_checked_ = true;
            top_m_candidates[top_m_candidates_end++] = set_L[c_i].id_;
            if (top_m_candidates_end == M) {
                break;
            }
        }

        if (top_m_candidates_end) {
            std::vector<idi> tmp_top_m(top_m_candidates_end);
            tmp_top_m.assign(top_m_candidates.begin(), top_m_candidates.begin() + top_m_candidates_end);
            top_m_list.push_back(tmp_top_m);
        } else {
            break;
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
                distf dist = compute_distance_with_norm(nb_data, query_data, norm);
                if (dist >= set_L[L-1].distance_) {
                    continue;
                }
                Candidate cand(nb_id, dist, false);
                idi r = insert_into_queue_panns(set_L, L, cand);
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
//                idi r = insert_into_queue_panns(set_L, L, cand);
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

    for (size_t k_i = 0; k_i < K; ++k_i) {
        set_K[k_i] = set_L[k_i].id_;
    }
}

} // namespace PANNS
