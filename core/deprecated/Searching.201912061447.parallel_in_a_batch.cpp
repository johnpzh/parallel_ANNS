//
// Created by Zhen Peng on 11/11/19.
//


#include "Searching.201912061447.parallel_in_a_batch.h"

namespace PANNS {

///**
// * Prepare init_ids and flags, as they are constant for all queries.
// * @param[out] init_ids
// * @param L
// */
//void Searching::prepare_init_ids(
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
//    std::unordered_set<idi> visited_ids;
//    idi *out_edges = (idi *) (opt_nsg_graph_ + ep_ * vertex_bytes_ + data_bytes_);
//    unsigned out_degree = *out_edges++;
//    idi tmp_l = 0;
//    for (; tmp_l < L && tmp_l < out_degree; tmp_l++) {
//        init_ids[tmp_l] = out_edges[tmp_l];
//        visited_ids.insert(init_ids[tmp_l]);
//    }
//
////    for (idi i = 0; i < tmp_l; ++i) {
////        is_visited[init_ids[i]] = true;
////    }
//
//    // If ep_'s neighbors are not enough, add other random vertices
//    idi tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    while (tmp_l < L) {
//        tmp_id %= num_v_;
//        unsigned id = tmp_id++;
////        if (is_visited[id]) {
////            continue;
////        }
//        if (visited_ids.find(id) != visited_ids.end()) {
//            continue;
//        }
////        is_visited[id] = true;
//        visited_ids.insert(id);
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
//                    idi r = insert_into_queue(retset, L, nn);
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

void Searching::search_with_top_m(
        const PANNS::idi M,
        const PANNS::idi query_id,
        const PANNS::idi K,
        const PANNS::idi L,
        std::vector<Candidate> &set_L,
        std::vector<idi> &init_ids,
        std::vector<idi> &set_K,
        std::vector< std::vector<idi> > &top_m_list)
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

//        while (tmp_l < L) {
//            unsigned id = rand() % num_v_;
//            if (is_visited[id]) {
//                continue;
//            }
//            is_visited[id] = true;
//            init_ids[tmp_l] = id;
//            tmp_l++;
//        }
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

void Searching::search_with_top_m_in_batch(
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
#pragma omp parallel for
        for (idi q_i = 0; q_i < batch_size; ++q_i) {
            auto &is_visited = is_visited_list[q_i];
            for (idi c_i = 0; c_i < L; ++c_i) {
                is_visited[init_ids[c_i]] = true;
            }
        }
    }

    // Initialize set_L_list
    {
#pragma omp parallel for
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
        std::vector< std::vector<idi> > cands_query_ids(num_v_, std::vector<idi>(batch_size)); // If candidate cand_id is selected by query q_i, q_i should be in cands_query_ids[cand_id].
        std::vector<idi> cands_query_ids_ends(num_v_, 0);

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
                    cands_query_ids[cand_id][cands_query_ids_ends[cand_id]++] = q_local_id;
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
                idi &q_i_bound = cands_query_ids_ends[cand_id];
                // Push neighbors to every queue of the queries that selected cand_id.
                // TODO: the order of for-loop needs change.
                // Traverse cand_id's neighbors
//                for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                    idi nb_id = out_edges[e_i];
//                    auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//                    dataf norm = *nb_data++;
//                idi &q_i_bound = cands_query_ids_ends[cand_id];
                for (idi q_i = 0; q_i < q_i_bound; ++q_i) {
                    idi q_local_id = query_local_ids[q_i];
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
//                        auto &set_L = set_L_list[q_local_id];
//                        dataf *query_data = queries_load_ + (q_local_id + batch_start) * dimension_;
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
                q_i_bound = 0; // Clear cands_query_ids[cand_id]
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
