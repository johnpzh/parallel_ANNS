//
// Created by Zhen Peng on 01/08/2020.
//

#include "Searching.202101081737.top_m_pure.profiling.h"

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

    const idi set_K_size = L < 100 ? L : 100;
    for (unsigned q_i = 0; q_i < num_queries_; ++q_i) {
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

//
// The difference from insert_into_queue is that add_into_queue will increase the queue size by 1.
// add_into_queue with a queue_start
idi Searching::add_into_queue(
        std::vector<PANNS::Candidate> &queue,
        const idi queue_start,
        idi &queue_size, // The insertion location starting from queue_start
        const idi queue_capacity, // The maximum capacity of queue, independent with queue_start.
        const PANNS::Candidate &cand)
{
    if (0 == queue_size) {
        queue[queue_start + queue_size++] = cand;
        return 0;
    }
    idi queue_end = queue_start + queue_size;
    // Find the insert location
    const auto it_loc = std::lower_bound(queue.begin() + queue_start, queue.begin() + queue_end, cand);
//    auto it_loc = std::lower_bound(queue.begin(), queue.begin() + queue_size, cand);
    idi insert_loc = it_loc - queue.begin();

    if (insert_loc != queue_end) {
        if (cand.id_ == it_loc->id_) {
            // Duplicate
            return queue_capacity;
        }
        if (queue_size >= queue_capacity) { // Queue is full
            --queue_size;
            --queue_end;
        }
    } else { // insert_loc == queue_end, insert at the end?
        if (queue_size < queue_capacity) { // Queue is not full
            // Insert at the end
            queue[insert_loc] = cand;
            ++queue_size;
            return queue_size - 1;
        } else { // Queue is full
            return queue_capacity;
        }
    }
    // Add into queue
    memmove(reinterpret_cast<char *>(queue.data() + insert_loc + 1),
            reinterpret_cast<char *>(queue.data() + insert_loc),
            (queue_end - insert_loc) * sizeof(Candidate));
    queue[insert_loc] = cand;
    ++queue_size;
    return insert_loc - queue_start;
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
idi Searching::merge_two_queues_into_1st_queue_seq_incr(
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
        return insert_index;
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
        return insert_index;
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
    return insert_index;
}


/* Function:
 * Use large local_queues_array as a concatenation of all queues
 */
idi Searching::merge_all_queues_to_master(
        std::vector<Candidate> &set_L,
        const std::vector<idi> &local_queues_starts,
        std::vector<idi> &local_queues_sizes,
        const idi local_queue_capacity,
        const idi L)
{
//    const int num_queues = num_threads_;
    idi nk = L;
    int size = 1 << (static_cast<idi>(log2(num_threads_)));
    idi log2size = static_cast<idi>(log2(size));
    for (idi d = 0; d < log2size; ++d) {
        uint32_t by = 1 << (d + 1);
#pragma omp parallel for
        for (int i = 0; i < size; i += by) {
            const idi ai = i + (1 << (d + 1)) - 1; // i + 2^(d+1) - 1
            const idi a_start = local_queues_starts[ai];
            idi &a_size = local_queues_sizes[ai];
//            idi a_start = ai * local_queue_length;
            const idi bi = i + (1 << d) - 1; // i + 2^d - 1
            const idi b_start = local_queues_starts[bi];
            idi &b_size = local_queues_sizes[bi];
//            idi b_start = bi * local_queue_length;
            if (0 == b_size) {
                continue;
            }
            if (a_size == 0) {
                std::copy(set_L.begin() + b_start,
                          set_L.begin() + b_start + b_size,
                          set_L.begin() + a_start); // Copy bi to ai
                a_size = b_size;
                b_size = 0;
                continue;
            }
            if (ai != static_cast<idi>(num_threads_ - 1)) {
                merge_two_queues_into_1st_queue_seq_incr(
                        set_L,
                        a_start,
                        a_size,
                        local_queue_capacity,
                        set_L,
                        b_start,
                        b_size);
            } else {
                idi r = merge_two_queues_into_1st_queue_seq_fixed(
                        set_L,
                        a_start,
                        L,
                        set_L,
                        b_start,
                        b_size);
                if (r < nk) {
                    nk = r;
                }
            }
            b_size = 0;
        }
    }
    // Reset local_queues_sizes
    // Not do this for Collector Idea or Selecting Idea
//    std::fill(local_queues_sizes.begin(), local_queues_sizes.end() - 1, 0);
//    std::fill(local_queues_sizes.begin(), local_queues_sizes.end(), 0);

    return nk;
//    return r;
}

/*
 * Function: merge 4 queues into the last queue
 */
idi Searching::merge_queues_of_four(
        std::vector<Candidate> &set_L,
        const std::vector<idi> &local_queues_starts,
        std::vector<idi> &local_queues_sizes,
        const idi group_id,
        const idi local_queue_capacity,
        const idi master_queue_capacity)
{
//    const int num_queues = 4;
    const idi group_start = group_id * 4;
    idi nk = master_queue_capacity;

#pragma omp parallel for num_threads(2)
    for (int i = 0; i < 2; ++i) {
        const idi bi = 2 * i + group_start;
        const idi ai = bi + 1;
        if (!local_queues_sizes[bi]) {
            continue;
        }
        if (!local_queues_sizes[ai]) {
            std::copy(
                    set_L.begin() + local_queues_starts[bi],
                    set_L.begin() + local_queues_starts[bi] + local_queues_sizes[bi],
                    set_L.begin() + local_queues_starts[ai]);
            local_queues_sizes[ai] = local_queues_sizes[bi];
            local_queues_sizes[bi] = 0;
            continue;
        }
        if (ai != 3 + group_start) {
            merge_two_queues_into_1st_queue_seq_incr(
                    set_L,
                    local_queues_starts[ai],
                    local_queues_sizes[ai],
                    local_queue_capacity,
                    set_L,
                    local_queues_starts[bi],
                    local_queues_sizes[bi]);
        } else {
            idi r = merge_two_queues_into_1st_queue_seq_incr(
                    set_L,
                    local_queues_starts[ai],
                    local_queues_sizes[ai],
                    master_queue_capacity,
                    set_L,
                    local_queues_starts[bi],
                    local_queues_sizes[bi]);
            if (r < nk) {
                nk = r;
            }
        }
        local_queues_sizes[bi] = 0;
    }
    {
        const idi bi = 1 + group_start;
        const idi ai = 3 + group_start;
        if (!local_queues_sizes[bi]) {
            return nk;
        }
        if (!local_queues_sizes[ai]) {
            std::copy(
                    set_L.begin() + local_queues_starts[bi],
                    set_L.begin() + local_queues_starts[bi] + local_queues_sizes[bi],
                    set_L.begin() + local_queues_starts[ai]);
            local_queues_sizes[ai] = local_queues_sizes[bi];
            local_queues_sizes[bi] = 0;
            return 0;
        }
        idi r = merge_two_queues_into_1st_queue_seq_incr(
                set_L,
                local_queues_starts[ai],
                local_queues_sizes[ai],
                master_queue_capacity,
                set_L,
                local_queues_starts[bi],
                local_queues_sizes[bi]);
        if (r < nk) {
            nk = r;
        }
        local_queues_sizes[bi] = 0;
    }

    return nk;
}

/*
 * Function: used by hierarchical merging idea.
 * Merge all queues into the last queue.
 * Difference with merge_all_queues_para_array: here the last queue might not have L elements in the beginning,
 * so use merge_two_queues_into_1st_queue_seq_incr(), not merge_two_queues_into_1st_queue_seq_fixed().
 */
idi Searching::merge_all_queues_to_master(
        std::vector<Candidate> &set_L,
        const std::vector<idi> &local_queues_starts,
        std::vector<idi> &local_queues_sizes,
        const idi local_queue_capacity,
        const idi local_master_queue_capacity,
        const idi master_queue_capacity,
        const idi group_size)
{
    const idi num_queues = num_threads_;
    idi nk = master_queue_capacity;
    int size = num_queues;
//    int size = 1 << (static_cast<idi>(log2(num_queues)));
    idi log2size = static_cast<idi>(log2(size));
    idi num_t = num_queues >> 1;
    for (idi d = 0; d < log2size; ++d) {
        uint32_t by = 1 << (d + 1);
//#pragma omp parallel for num_threads(2)
//#pragma omp parallel for
        for (int i = 0; i < size; i += by) {
            idi ai = i + (1 << (d + 1)) - 1; // i + 2^(d+1) - 1
//            idi a_start = ai * local_queue_capacity;
            idi a_start = local_queues_starts[ai];
            idi bi = i + (1 << d) - 1; // i + 2^d - 1
//            idi b_start = bi * local_queue_capacity;
            idi b_start = local_queues_starts[bi];
            if (0 == local_queues_sizes[bi]) {
                continue;
            }
            {//test
                printf("local_queues_sizes[%u]: %u\n",
                       bi, local_queues_sizes[bi]);
            }
            if (local_queues_sizes[ai] == 0) {
                std::copy(set_L.begin() + b_start,
                          set_L.begin() + b_start + local_queues_sizes[bi],
                          set_L.begin() + a_start); // Copy bi to ai
                local_queues_sizes[ai] = local_queues_sizes[bi];
                local_queues_sizes[bi] = 0;
                continue;
            }
            if ((group_size - 1) != ai % 4) {
                merge_two_queues_into_1st_queue_seq_incr(
                        set_L,
                        a_start,
                        local_queues_sizes[ai],
                        local_queue_capacity,
                        set_L,
                        b_start,
                        local_queues_sizes[bi]);
            } else if (num_queues - 1 != ai) {
                merge_two_queues_into_1st_queue_seq_incr(
                        set_L,
                        a_start,
                        local_queues_sizes[ai],
                        local_master_queue_capacity,
                        set_L,
                        b_start,
                        local_queues_sizes[bi]);
            } else {
                idi r = merge_two_queues_into_1st_queue_seq_incr(
                        set_L,
                        a_start,
                        local_queues_sizes[ai],
                        master_queue_capacity,
                        set_L,
                        b_start,
                        local_queues_sizes[bi]);
                if (ai == num_queues - 1 && r < nk) {
                    nk = r;
                }
            }
            local_queues_sizes[bi] = 0;
        }
        num_t >>= 1;
    }
    // Reset local_queues_sizes
    // Not do this for Collector Idea or Selecting Idea
//    std::fill(local_queues_sizes.begin(), local_queues_sizes.end() - 1, 0);
//    std::fill(local_queues_sizes.begin(), local_queues_sizes.end(), 0);

    return nk;
}


/*
 * 6/22/2020-21:30
 * Do searching on the local_set_L
 * local_set_L is already sorted
 * is_visited is already set up.
 */
void Searching::subsearch_with_top_m(
        const idi value_M_max,
        const idi query_id,
        const idi local_L,
        std::vector<Candidate> &set_L,
        const idi set_L_start,
        idi &set_L_size,
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
                set_L_start,
                set_L_size,
                local_top_m_candidates,
                is_visited,
                local_count_distance_computation);

        {// Scale M
            if (M < value_M_max) {
                M <<= 1;
            }
//            else {
//                M = value_M_max;
//            }
        }
    }
//    {//test
//        printf("set_L_start: %u "
//               "local_count_distance_computation: %lu\n",
//                set_L_start,
//                local_count_distance_computation);
//    }
}

/*
 * 7/6/2020-23:17
 * Subsearch only 1 iteration using top-m
 */
void Searching::subsearch_top_m_for_one_iteration(
        const idi iter,
        idi &k_uc,
        const idi value_M,
        const idi query_id,
        const dataf *query_data,
        const idi L,
        std::vector<Candidate> &set_L,
        const idi set_L_start,
        idi &set_L_size,
        std::vector<idi> &top_m_candidates,
        boost::dynamic_bitset<> &is_visited,
        uint64_t &count_distance_computation)
{
    // Select M candidates
    idi top_m_candidates_end = 0;
    idi last_k = L;
    pick_top_m_unchecked(
            value_M,
            k_uc,
            set_L,
            set_L_start,
            set_L_size,
            top_m_candidates,
            top_m_candidates_end,
            last_k);

//    bool is_quota_done = false;
    idi nk = L;
    // Push M candidates' neighbors into the queue.
    for (idi c_i = 0; c_i < top_m_candidates_end; ++c_i) {
//    for (idi c_i = 0; c_i < top_m_candidates_end && !is_quota_done; ++c_i) {
        idi cand_id = top_m_candidates[c_i];
        idi r = expand_one_candidate(
                0,
                cand_id,
                query_data,
                set_L[set_L_size - 1 + set_L_start].distance_,
                set_L,
                set_L_start,
                set_L_size,
                L,
                is_visited,
                count_distance_computation);
//                is_quota_done);
        if (r < nk) {
            nk = r;
        }
    }
    if (nk <= last_k) {
        k_uc = nk;
    } else {
        k_uc = last_k + 1;
    }
}



/*
 * Function: pick master queue's top-M unchecked elements to top_m_candidates.
 * Used by interval merge.
 */
idi Searching::pick_top_m_to_workers(
        std::vector<Candidate> &set_L,
        const std::vector<idi> &local_queues_starts,
        std::vector<idi> &local_queues_sizes,
        std::vector<idi> &top_m_candidates,
        const std::vector<idi> &top_m_candidates_starts,
        std::vector<idi> &top_m_candidates_sizes,
        const idi k_uc,
        idi &last_k,
        const idi M)
//        const idi num_groups)
//        const idi group_size)
{
    const idi last_queue_start = local_queues_starts[num_threads_ - 1];
    idi c_i_start = k_uc + last_queue_start;
    idi c_i_bound = last_queue_start + local_queues_sizes[num_threads_ - 1];
    idi top_m_count = 0;
    for (idi c_i = c_i_start; c_i < c_i_bound && top_m_count < M; ++c_i) {
        if (set_L[c_i].is_checked_) {
            continue;
        }
        last_k = c_i - last_queue_start;
        set_L[c_i].is_checked_ = true;
        idi q_i = top_m_count & (num_threads_ - 1); // top_m_count % num_groups;
        ++top_m_count;
        top_m_candidates[top_m_candidates_starts[q_i] + top_m_candidates_sizes[q_i]++] = set_L[c_i].id_;
    }
    return top_m_count;
}

/*
 * Function: pick top-M unchecked candidates from queue
 */
void Searching::pick_top_m_unchecked(
        const idi M,
        const idi k_uc,
        std::vector<Candidate> &set_L,
        const idi local_queue_start,
        const idi local_queue_size,
        std::vector<idi> &top_m_candidates,
//        const idi top_m_candidates_start,
        idi &top_m_candidates_size,
        idi &last_k)
{
    idi tmp_last_k = local_queue_size;
    idi tmc_size = 0;
    idi c_i_start = local_queue_start + k_uc;
    idi c_i_bound = local_queue_start + local_queue_size;
    // Pick top-M
    for (idi c_i = c_i_start; c_i < c_i_bound && tmc_size < M; ++c_i) {
        if (set_L[c_i].is_checked_) {
            continue;
        }
        tmp_last_k = c_i - local_queue_start; // Record the location of the last candidate selected.
        set_L[c_i].is_checked_ = true;
        top_m_candidates[tmc_size++] = set_L[c_i].id_;
    }
    last_k = tmp_last_k;
    top_m_candidates_size = tmc_size;
}

/*
 * Pick top-M unchecked candidates from queues.
 * Not returen until picked top-M candidates or all queueus are checked.
 */
void Searching::pick_top_m_unchecked_from_queues(
        const idi M,
        const int queue_i_start,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &local_queues_starts,
        const std::vector<idi> &local_queues_sizes,
        std::vector<idi> &top_m_candidates,
        idi &top_m_candidates_size,
        int &queue_i_end,
        const std::vector<idi> &ks,
        std::vector<idi> &last_ks) const
{
    int queue_i = queue_i_start;
    while (queue_i < num_threads_) {
        const idi local_queue_size = local_queues_sizes[queue_i];
        if (!local_queue_size) {
            ++queue_i;
            continue;
        }
        idi &last_k = last_ks[queue_i];
        const idi local_queue_start = local_queues_starts[queue_i];
        idi c_i_start = local_queue_start + ks[queue_i];
        idi c_i_bound = local_queue_start + local_queues_sizes[queue_i];

        for (idi c_i = c_i_start; c_i < c_i_bound && top_m_candidates_size < M; ++c_i) {
            if (set_L[c_i].is_checked_) {
                continue;
            }
            last_k = c_i - local_queue_start; // Record the location of the last candidate selected.
            set_L[c_i].is_checked_ = true;
            top_m_candidates[top_m_candidates_size++] = set_L[c_i].id_;
        }
        if (top_m_candidates_size < M) {
            ++queue_i;
        } else {
            break;
        }
    }
    queue_i_end = queue_i;
//    last_k = local_queue_size;
//    idi c_i_start = local_queue_start + k_uc;
//    idi c_i_bound = local_queue_start + local_queue_size;
//    // Pick top-M
//    for (idi c_i = c_i_start; c_i < c_i_bound && top_m_candidates_size < M; ++c_i) {
//        if (set_L[c_i].is_checked_) {
//            continue;
//        }
//        last_k = c_i - local_queue_start; // Record the location of the last candidate selected.
//        set_L[c_i].is_checked_ = true;
//        top_m_candidates[top_m_candidates_size++] = set_L[c_i].id_;
//    }
}


/*
 * Function: expand a candidate, visiting its neighbors.
 * Return the lowest adding location.
 */
idi Searching::expand_one_candidate(
        const idi q_i,
        const idi cand_id,
        const dataf *query_data,
        const distf &dist_bound,
        std::vector<Candidate> &set_L,
        const idi local_queue_start,
        idi &local_queue_size,
        const idi &local_queue_capacity,
        boost::dynamic_bitset<> &is_visited,
//        const idi nk_init,
        uint64_t &local_count_computation)
//        bool &is_quota_done)
{
    uint64_t tmp_count_computation = 0;
//    _mm_prefetch(opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
    idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id * vertex_bytes_ + data_bytes_);
    idi out_degree = *out_edges++;
//    if (threads_computations_[q_i] + out_degree >= thread_compuation_quota_) {
//        is_quota_done = true;
//        return local_queue_capacity;
//    }
//    for (idi n_i = 0; n_i < out_degree; ++n_i) {
//        _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//    }
//    tmp_time_pick_top_m += WallTimer::get_time_mark();
//    uint64_t tmp_last_count_computation = tmp_count_computation;
    idi nk = local_queue_capacity;
    for (idi e_i = 0; e_i < out_degree; ++e_i) {
        idi nb_id = out_edges[e_i];
        { // Sequential edition
            if (is_visited[nb_id]) {
                continue;
            }
            is_visited[nb_id] = true;
        }

        auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
        dataf norm = *nb_data++;
        ++tmp_count_computation;
        distf dist = compute_distance_with_norm(nb_data, query_data, norm);
        if (dist > dist_bound) {
//        if (dist > set_L[L - 1 + master_queue_start].distance_) {
            continue;
        }
        Candidate cand(nb_id, dist, false);
        // Add to the local queue.
        idi r = add_into_queue(
                set_L,
                local_queue_start,
                local_queue_size,
                local_queue_capacity,
                cand);
        if (r < nk) {
            nk = r;
        }
    }
//    threads_computations_[q_i] += tmp_count_computation - tmp_last_count_computation;
//    if (threads_computations_[q_i] >= thread_compuation_quota_) {
//        is_quota_done = true;
//    }
    local_count_computation += tmp_count_computation;

    return nk;
}

void Searching::initialize_set_L_para(
        const dataf *query_data,
        const idi L,
        std::vector<Candidate> &set_L,
        const idi set_L_start,
        idi &set_L_size,
        const std::vector<idi> &init_ids,
        boost::dynamic_bitset<> &is_visited)
{
    //#pragma omp parallel for
    for (idi c_i = 0; c_i < L; ++c_i) {
        is_visited[init_ids[c_i]] = true;
    }

//#pragma omp parallel for
//        for (idi v_i = 0; v_i < L; ++v_i) {
//            idi v_id = init_ids[v_i];
//            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//        }

    // Get the distances of all candidates, store in the set set_L.
    uint64_t tmp_count_computation = 0;
#pragma omp parallel for reduction(+ : tmp_count_computation)
    for (unsigned i = 0; i < L; i++) {
        unsigned v_id = init_ids[i];
        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
        dataf norm = *v_data++;
        ++tmp_count_computation;
        distf dist = compute_distance_with_norm(v_data, query_data, norm);
        set_L[set_L_start + i] = Candidate(v_id, dist, false); // False means not checked.
    }
    count_distance_computation_ += tmp_count_computation;
//        threads_computations_[0] += tmp_count_computation;
//        tmp_count_computation = 0;
    std::sort(
            set_L.begin() + set_L_start,
            set_L.begin() + set_L_start + L);
    set_L_size = L;
}

void Searching::initialize_set_L_seq(
        const dataf *query_data,
        const idi L,
        std::vector<Candidate> &set_L,
        const idi set_L_start,
        idi &set_L_size,
        const std::vector<idi> &init_ids,
        boost::dynamic_bitset<> &is_visited)
{
    //#pragma omp parallel for
    for (idi c_i = 0; c_i < L; ++c_i) {
        is_visited[init_ids[c_i]] = true;
    }

//#pragma omp parallel for
//        for (idi v_i = 0; v_i < L; ++v_i) {
//            idi v_id = init_ids[v_i];
//            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//        }

    // Get the distances of all candidates, store in the set set_L.
    uint64_t tmp_count_computation = 0;
    for (unsigned i = 0; i < L; i++) {
        unsigned v_id = init_ids[i];
        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
        dataf norm = *v_data++;
        ++tmp_count_computation;
        distf dist = compute_distance_with_norm(v_data, query_data, norm);
        set_L[set_L_start + i] = Candidate(v_id, dist, false); // False means not checked.
    }
    count_distance_computation_ += tmp_count_computation;
//        threads_computations_[0] += tmp_count_computation;
//        tmp_count_computation = 0;
    std::sort(
            set_L.begin() + set_L_start,
            set_L.begin() + set_L_start + L);
    set_L_size = L;
}

void Searching::seq_search_with_top_m_scale_m(
        const idi M_max,
        const idi query_id,
        const idi K,
        const idi L,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K,
        std::vector<idi> &top_m_candidates,
        boost::dynamic_bitset<> &is_visited)
{
//    time_initialization_ -= WallTimer::get_time_mark();
//    const idi master_queue_start = local_queues_starts[num_threads_ - 1];
    const dataf *query_data = queries_load_ + query_id * dimension_;
    idi set_L_size;

    // Initialization Phase
    {
        //#pragma omp parallel for
        for (idi c_i = 0; c_i < L; ++c_i) {
            is_visited[init_ids[c_i]] = true;
        }

//#pragma omp parallel for
//        for (idi v_i = 0; v_i < L; ++v_i) {
//            idi v_id = init_ids[v_i];
//            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//        }

        // Get the distances of all candidates, store in the set set_L.
        uint64_t tmp_count_computation = 0;
        for (unsigned i = 0; i < L; i++) {
            unsigned v_id = init_ids[i];
            auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
            dataf norm = *v_data++;
            ++tmp_count_computation;
            distf dist = compute_distance_with_norm(v_data, query_data, norm);
            set_L[i] = Candidate(v_id, dist, false); // False means not checked.
        }
        count_distance_computation_ += tmp_count_computation;
//        threads_computations_[0] += tmp_count_computation;
//        tmp_count_computation = 0;
        std::sort(
                set_L.begin(),
                set_L.begin() + L);
        set_L_size = L;
    } // Initialization Phase
//    time_initialization_ += WallTimer::get_time_mark();

//    idi top_m_candidates_end = 0;

    idi iter = 0; // for debug
    idi M = 1;
    idi k = 0; // Index of first unchecked candidate.
    // Sequential Phase

    uint64_t tmp_count_computation = 0;
    while (k < L) {
//        double test_time_iter = -WallTimer::get_time_mark();
        ++iter;
        subsearch_top_m_for_one_iteration(
                iter,
                k,
                M,
                query_id,
                query_data,
                L,
                set_L,
                0,
                set_L_size,
                top_m_candidates,
                is_visited,
                tmp_count_computation);
        count_distance_computation_ += tmp_count_computation;
//            threads_computations_[0] += tmp_count_computation;
        tmp_count_computation = 0;

        {// Double M
            if (M < M_max) {
                M <<= 1U;
            }
        }
//        test_time_iter += WallTimer::get_time_mark();
//        if (iter > time_iterations_.size()) {
//            time_iterations_.push_back(test_time_iter);
//        } else {
//            time_iterations_[iter - 1] += test_time_iter;
//        }
    }

//#pragma omp parallel for
    for (idi k_i = 0; k_i < K; ++k_i) {
        set_K[k_i] = set_L[k_i].id_;
    }

    {// Reset
//        std::fill(is_visited.begin(), is_visited.end(), 0);
        is_visited.reset();
//        is_visited.clear_all();
//        std::fill(local_queues_sizes.begin(), local_queues_sizes.end(), 0);
//        std::fill(threads_computations_.begin(), threads_computations_.end(), 0);
    }
//    {//test
//        if (0 == query_id) {
//            exit(1);
//        }
//    }
}

void Searching::seq_search_with_top_m_pure(
        const idi M,
        const idi query_id,
        const idi K,
        const idi L,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K,
        std::vector<idi> &top_m_candidates,
        boost::dynamic_bitset<> &is_visited)
{
//    time_initialization_ -= WallTimer::get_time_mark();
//    const idi master_queue_start = local_queues_starts[num_threads_ - 1];
    const dataf *query_data = queries_load_ + query_id * dimension_;
    idi set_L_size;

    // Initialization Phase
    {
        //#pragma omp parallel for
        for (idi c_i = 0; c_i < L; ++c_i) {
            is_visited[init_ids[c_i]] = true;
        }

//#pragma omp parallel for
//        for (idi v_i = 0; v_i < L; ++v_i) {
//            idi v_id = init_ids[v_i];
//            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//        }

        // Get the distances of all candidates, store in the set set_L.
        uint64_t tmp_count_computation = 0;
        for (unsigned i = 0; i < L; i++) {
            unsigned v_id = init_ids[i];
            auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
            dataf norm = *v_data++;
            ++tmp_count_computation;
            distf dist = compute_distance_with_norm(v_data, query_data, norm);
            set_L[i] = Candidate(v_id, dist, false); // False means not checked.
        }
        count_distance_computation_ += tmp_count_computation;
//        threads_computations_[0] += tmp_count_computation;
//        tmp_count_computation = 0;
        std::sort(
                set_L.begin(),
                set_L.begin() + L);
        set_L_size = L;
    } // Initialization Phase
//    time_initialization_ += WallTimer::get_time_mark();

//    idi top_m_candidates_end = 0;

    idi iter = 0; // for debug
//    idi M = 1;
    idi k = 0; // Index of first unchecked candidate.
    // Sequential Phase

    uint64_t tmp_count_computation = 0;
    while (k < L) {
//        double test_time_iter = -WallTimer::get_time_mark();
        ++iter;
        subsearch_top_m_for_one_iteration(
                iter,
                k,
                M,
                query_id,
                query_data,
                L,
                set_L,
                0,
                set_L_size,
                top_m_candidates,
                is_visited,
                tmp_count_computation);
        count_distance_computation_ += tmp_count_computation;
//            threads_computations_[0] += tmp_count_computation;
        tmp_count_computation = 0;

//        test_time_iter += WallTimer::get_time_mark();
//        if (iter > time_iterations_.size()) {
//            time_iterations_.push_back(test_time_iter);
//        } else {
//            time_iterations_[iter - 1] += test_time_iter;
//        }
    }

//#pragma omp parallel for
    for (idi k_i = 0; k_i < K; ++k_i) {
        set_K[k_i] = set_L[k_i].id_;
    }

    {// Reset
//        std::fill(is_visited.begin(), is_visited.end(), 0);
        is_visited.reset();
//        is_visited.clear_all();
//        std::fill(local_queues_sizes.begin(), local_queues_sizes.end(), 0);
//        std::fill(threads_computations_.begin(), threads_computations_.end(), 0);
    }
//    {//test
//        if (0 == query_id) {
//            exit(1);
//        }
//    }
}

void Searching::seq_search_simple_search(
        const idi query_id,
        const idi K,
        const idi L,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K,
        boost::dynamic_bitset<> &is_visited)
{
//    time_initialization_ -= WallTimer::get_time_mark();
//    const idi master_queue_start = local_queues_starts[num_threads_ - 1];
    const dataf *query_data = queries_load_ + query_id * dimension_;
    idi set_L_size;

    // Initialization Phase
    initialize_set_L_seq(
            query_data,
            L,
            set_L,
            0,
            set_L_size,
            init_ids,
            is_visited);
//    {
//        //#pragma omp parallel for
//        for (idi c_i = 0; c_i < L; ++c_i) {
//            is_visited[init_ids[c_i]] = true;
//        }
//
////#pragma omp parallel for
////        for (idi v_i = 0; v_i < L; ++v_i) {
////            idi v_id = init_ids[v_i];
////            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
////        }
//
//        // Get the distances of all candidates, store in the set set_L.
//        uint64_t tmp_count_computation = 0;
//        for (unsigned i = 0; i < L; i++) {
//            unsigned v_id = init_ids[i];
//            auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
//            dataf norm = *v_data++;
//            ++tmp_count_computation;
//            distf dist = compute_distance_with_norm(v_data, query_data, norm);
//            set_L[i] = Candidate(v_id, dist, false); // False means not checked.
//        }
//        count_distance_computation_ += tmp_count_computation;
////        threads_computations_[0] += tmp_count_computation;
////        tmp_count_computation = 0;
//        std::sort(
//                set_L.begin(),
//                set_L.begin() + L);
//        set_L_size = L;
//    } // Initialization Phase
//    time_initialization_ += WallTimer::get_time_mark();

//    idi top_m_candidates_end = 0;

    idi iter = 0; // for debug
//    idi M = 1;
    idi k = 0; // Index of first unchecked candidate.
    const distf &last_dist = set_L[L - 1].distance_;
    // Sequential Phase

    uint64_t tmp_count_computation = 0;
    while (k < L) {
//    while (k < L) {
//        double test_time_iter = -WallTimer::get_time_mark();
        if (!set_L[k].is_checked_) {
            set_L[k].is_checked_ = true;
            ++iter;
            idi cand_id = set_L[k].id_;
            idi r = expand_one_candidate(
                    0,
                    cand_id,
                    query_data,
                    last_dist,
                    set_L,
                    0,
                    set_L_size,
                    L,
                    is_visited,
                    tmp_count_computation);
            if (r <= k) {
                k = r;
            } else {
                ++k;
            }
            count_distance_computation_ += tmp_count_computation;
            tmp_count_computation = 0;
        } else {
            ++k;
        }
//        test_time_iter += WallTimer::get_time_mark();
//        if (iter > time_iterations_.size()) {
//            time_iterations_.push_back(test_time_iter);
//        } else {
//            time_iterations_[iter - 1] += test_time_iter;
//        }
    }



//#pragma omp parallel for
    for (idi k_i = 0; k_i < K; ++k_i) {
        set_K[k_i] = set_L[k_i].id_;
    }

    {// Reset
//        std::fill(is_visited.begin(), is_visited.end(), 0);
        is_visited.reset();
//        is_visited.clear_all();
//        std::fill(local_queues_sizes.begin(), local_queues_sizes.end(), 0);
//        std::fill(threads_computations_.begin(), threads_computations_.end(), 0);
    }
//    {//test
//        if (0 == query_id) {
//            exit(1);
//        }
//    }
}

void Searching::seq_search_simple_search_early_termination(
//        const idi M,
        const idi query_id,
        const idi K,
        const idi L,
        const idi L_termn,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K,
//        std::vector<idi> &top_m_candidates,
        boost::dynamic_bitset<> &is_visited)
{
//    time_initialization_ -= WallTimer::get_time_mark();
//    const idi master_queue_start = local_queues_starts[num_threads_ - 1];
    const dataf *query_data = queries_load_ + query_id * dimension_;
    idi set_L_size;

    // Initialization Phase
    {
        //#pragma omp parallel for
        for (idi c_i = 0; c_i < L; ++c_i) {
            is_visited[init_ids[c_i]] = true;
        }

//#pragma omp parallel for
//        for (idi v_i = 0; v_i < L; ++v_i) {
//            idi v_id = init_ids[v_i];
//            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//        }

        // Get the distances of all candidates, store in the set set_L.
        uint64_t tmp_count_computation = 0;
        for (unsigned i = 0; i < L; i++) {
            unsigned v_id = init_ids[i];
            auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
            dataf norm = *v_data++;
            ++tmp_count_computation;
            distf dist = compute_distance_with_norm(v_data, query_data, norm);
            set_L[i] = Candidate(v_id, dist, false); // False means not checked.
        }
        count_distance_computation_ += tmp_count_computation;
//        threads_computations_[0] += tmp_count_computation;
//        tmp_count_computation = 0;
        std::sort(
                set_L.begin(),
                set_L.begin() + L);
        set_L_size = L;
    } // Initialization Phase
//    time_initialization_ += WallTimer::get_time_mark();

//    idi top_m_candidates_end = 0;

    idi iter = 0; // for debug
    idi M = 1;
    idi k = 0; // Index of first unchecked candidate.
    idi r;
    idi cand_id;
    const distf &last_dist = set_L[L - 1].distance_;
    // Sequential Phase

    uint64_t tmp_count_computation = 0;
    bool is_quota_done = false;
    while (k < L_termn) {
//    while (k < L) {
//        double test_time_iter = -WallTimer::get_time_mark();
        if (!set_L[k].is_checked_) {
            set_L[k].is_checked_ = true;
            cand_id = set_L[k].id_;
            r = expand_one_candidate(
                    0,
                    cand_id,
                    query_data,
                    last_dist,
                    set_L,
                    0,
                    set_L_size,
                    L,
                    is_visited,
                    tmp_count_computation);
            count_distance_computation_ += tmp_count_computation;
            tmp_count_computation = 0;
            ++iter;
        } else {
            r = L;
        }
        if (r <= k) {
            k = r;
        } else {
            ++k;
        }
//        subsearch_top_m_for_one_iteration(
//                iter,
//                k,
//                M,
//                query_id,
//                query_data,
//                L,
//                set_L,
//                0,
//                set_L_size,
//                top_m_candidates,
//                is_visited,
//                tmp_count_computation);
//        count_distance_computation_ += tmp_count_computation;
//            threads_computations_[0] += tmp_count_computation;
//        tmp_count_computation = 0;

//        test_time_iter += WallTimer::get_time_mark();
//        if (iter > time_iterations_.size()) {
//            time_iterations_.push_back(test_time_iter);
//        } else {
//            time_iterations_[iter - 1] += test_time_iter;
//        }
    }

//#pragma omp parallel for
    for (idi k_i = 0; k_i < K; ++k_i) {
        set_K[k_i] = set_L[k_i].id_;
    }

    {// Reset
//        std::fill(is_visited.begin(), is_visited.end(), 0);
        is_visited.reset();
//        is_visited.clear_all();
//        std::fill(local_queues_sizes.begin(), local_queues_sizes.end(), 0);
//        std::fill(threads_computations_.begin(), threads_computations_.end(), 0);
    }
//    {//test
//        if (0 == query_id) {
//            exit(1);
//        }
//    }
}

void Searching::seq_search_with_top_m_middle(
        const idi M,
        const idi seq_iter,
        const idi query_id,
        const idi K,
        const idi L,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K,
        std::vector<idi> &top_m_candidates,
        boost::dynamic_bitset<> &is_visited)
{
//    time_initialization_ -= WallTimer::get_time_mark();
//    const idi master_queue_start = local_queues_starts[num_threads_ - 1];
    const dataf *query_data = queries_load_ + query_id * dimension_;
    idi set_L_size;

    // Initialization Phase
    {
        //#pragma omp parallel for
        for (idi c_i = 0; c_i < L; ++c_i) {
            is_visited[init_ids[c_i]] = true;
        }

//#pragma omp parallel for
//        for (idi v_i = 0; v_i < L; ++v_i) {
//            idi v_id = init_ids[v_i];
//            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//        }

        // Get the distances of all candidates, store in the set set_L.
        uint64_t tmp_count_computation = 0;
        for (unsigned i = 0; i < L; i++) {
            unsigned v_id = init_ids[i];
            auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
            dataf norm = *v_data++;
            ++tmp_count_computation;
            distf dist = compute_distance_with_norm(v_data, query_data, norm);
            set_L[i] = Candidate(v_id, dist, false); // False means not checked.
        }
        count_distance_computation_ += tmp_count_computation;
//        threads_computations_[0] += tmp_count_computation;
//        tmp_count_computation = 0;
        std::sort(
                set_L.begin(),
                set_L.begin() + L);
        set_L_size = L;
    } // Initialization Phase
//    time_initialization_ += WallTimer::get_time_mark();


//    time_sequential_phase_ -= WallTimer::get_time_mark();
//    idi top_m_candidates_end = 0;

    idi iter = 0; // for debug
//    idi M = 1;
    idi k = 0; // Index of first unchecked candidate.
    // Sequential Phase

    uint64_t tmp_count_computation = 0;
    while (k < L && iter < seq_iter) {
//        double test_time_iter = -WallTimer::get_time_mark();
        ++iter;
        subsearch_top_m_for_one_iteration(
                iter,
                k,
                1,
                query_id,
                query_data,
                L,
                set_L,
                0,
                set_L_size,
                top_m_candidates,
                is_visited,
                tmp_count_computation);
        count_distance_computation_ += tmp_count_computation;
//            threads_computations_[0] += tmp_count_computation;
        tmp_count_computation = 0;

//        test_time_iter += WallTimer::get_time_mark();
//        if (iter > time_iterations_.size()) {
//            time_iterations_.push_back(test_time_iter);
//        } else {
//            time_iterations_[iter - 1] += test_time_iter;
//        }
    }
//    time_sequential_phase_ += WallTimer::get_time_mark();

//    time_parallel_phase_ -= WallTimer::get_time_mark();
    while (k < L) {
//        double test_time_iter = -WallTimer::get_time_mark();
        ++iter;
        subsearch_top_m_for_one_iteration(
                iter,
                k,
                M,
                query_id,
                query_data,
                L,
                set_L,
                0,
                set_L_size,
                top_m_candidates,
                is_visited,
                tmp_count_computation);
        count_distance_computation_ += tmp_count_computation;
//            threads_computations_[0] += tmp_count_computation;
        tmp_count_computation = 0;

//        test_time_iter += WallTimer::get_time_mark();
//        if (iter > time_iterations_.size()) {
//            time_iterations_.push_back(test_time_iter);
//        } else {
//            time_iterations_[iter - 1] += test_time_iter;
//        }
    }
//    time_parallel_phase_ += WallTimer::get_time_mark();

//    time_ending_ -= WallTimer::get_time_mark();
//#pragma omp parallel for
    for (idi k_i = 0; k_i < K; ++k_i) {
        set_K[k_i] = set_L[k_i].id_;
    }

    {// Reset
//        std::fill(is_visited.begin(), is_visited.end(), 0);
        is_visited.reset();
//        is_visited.clear_all();
//        std::fill(local_queues_sizes.begin(), local_queues_sizes.end(), 0);
//        std::fill(threads_computations_.begin(), threads_computations_.end(), 0);
    }
//    {//test
//        if (0 == query_id) {
//            exit(1);
//        }
//    }
//    time_ending_ += WallTimer::get_time_mark();
}

void Searching::para_search_with_top_m_pure_profiling(
        const idi M,
        const idi query_id,
        const idi K,
        const idi L,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K,
        const idi local_queue_capacity,
        const std::vector<idi> &local_queues_starts,
        std::vector<idi> &local_queues_sizes,
        std::vector<idi> &top_m_candidates,
        boost::dynamic_bitset<> &is_visited)
{
//    time_initialization_ -= WallTimer::get_time_mark();
    const idi master_queue_start = local_queues_starts[num_threads_ - 1];
    const dataf *query_data = queries_load_ + query_id * dimension_;
//    idi set_L_size;

    // Initialization Phase
    initialize_set_L_para(
            query_data,
            L,
            set_L,
            master_queue_start,
            local_queues_sizes[num_threads_ - 1],
            init_ids,
            is_visited);
//    time_initialization_ += WallTimer::get_time_mark();

    const distf &last_dist = set_L[master_queue_start + L - 1].distance_;
    idi top_m_candidates_size = 0;
    idi iter = 0; // for debug
//    idi M = 1;
    idi k = 0; // Index of first unchecked candidate.
    idi last_k;
    idi nk;
    // Sequential Phase

    uint64_t tmp_count_computation = 0;

    if (1 == num_threads_) {
        // Sequential Version
        while (true) {
//        double test_time_iter = -WallTimer::get_time_mark();
            ++iter;
            // Pick
            last_k = L;
            pick_top_m_unchecked(
                    M,
                    k,
                    set_L,
                    master_queue_start,
                    local_queues_sizes[num_threads_ - 1],
                    top_m_candidates,
                    top_m_candidates_size,
                    last_k);
            if (!top_m_candidates_size) {
                break;
            }
            // Expand
            nk = L;
//#pragma omp parallel for reduction(+ : tmp_count_computation)
            for (idi c_i = 0; c_i < top_m_candidates_size; ++c_i) {
                int queue_i = omp_get_thread_num();
                const idi queue_capacity = num_threads_ - 1 != queue_i ? local_queue_capacity : L;
                idi cand_id = top_m_candidates[c_i];
                idi r = expand_one_candidate(
                        queue_i,
                        cand_id,
                        query_data,
                        last_dist,
                        set_L,
                        local_queues_starts[queue_i],
                        local_queues_sizes[queue_i],
                        queue_capacity,
                        is_visited,
                        tmp_count_computation);
                if (num_threads_ - 1 == queue_i && r < nk) {
                    nk = r;
                }
            } // Expand
            top_m_candidates_size = 0;
            count_distance_computation_ += tmp_count_computation;
            tmp_count_computation = 0;

            // Merge
            ++count_merge_;
//            idi r = merge_all_queues_to_master(
//                    set_L,
//                    local_queues_starts,
//                    local_queues_sizes,
//                    local_queue_capacity,
//                    L);
//            if (r < nk) {
//                nk = r;
//            }
            if (nk <= last_k) {
                k = nk;
            } else {
                k = last_k + 1;
            }
        } // Iterations

//#pragma omp parallel for
        for (idi k_i = 0; k_i < K; ++k_i) {
            set_K[k_i] = set_L[master_queue_start + k_i].id_;
        }
    } else {
        // Parallel Version
        while (true) {
//        double test_time_iter = -WallTimer::get_time_mark();
            ++iter;
            // Pick
            last_k = L;
            pick_top_m_unchecked(
                    M,
                    k,
                    set_L,
                    master_queue_start,
                    local_queues_sizes[num_threads_ - 1],
                    top_m_candidates,
                    top_m_candidates_size,
                    last_k);
            if (!top_m_candidates_size) {
                break;
            }
            // Expand
            time_expand_ -= WallTimer::get_time_mark();
            nk = L;
#pragma omp parallel for reduction(+ : tmp_count_computation)
            for (idi c_i = 0; c_i < top_m_candidates_size; ++c_i) {
                int queue_i = omp_get_thread_num();
                const idi queue_capacity = num_threads_ - 1 != queue_i ? local_queue_capacity : L;
                idi cand_id = top_m_candidates[c_i];
                idi r = expand_one_candidate(
                        queue_i,
                        cand_id,
                        query_data,
                        last_dist,
                        set_L,
                        local_queues_starts[queue_i],
                        local_queues_sizes[queue_i],
                        queue_capacity,
                        is_visited,
                        tmp_count_computation);
                if (num_threads_ - 1 == queue_i && r < nk) {
                    nk = r;
                }
            } // Expand
            top_m_candidates_size = 0;
            count_distance_computation_ += tmp_count_computation;
            tmp_count_computation = 0;

            time_expand_ += WallTimer::get_time_mark();
            time_merge_ -= WallTimer::get_time_mark();

            // Merge
            ++count_merge_;
            idi r = merge_all_queues_to_master(
                    set_L,
                    local_queues_starts,
                    local_queues_sizes,
                    local_queue_capacity,
                    L);
            if (r < nk) {
                nk = r;
            }
            if (nk <= last_k) {
                k = nk;
            } else {
                k = last_k + 1;
            }
            time_merge_ += WallTimer::get_time_mark();
        } // Iterations

#pragma omp parallel for
        for (idi k_i = 0; k_i < K; ++k_i) {
            set_K[k_i] = set_L[master_queue_start + k_i].id_;
        }

    }

    {// Reset
//        std::fill(is_visited.begin(), is_visited.end(), 0);
        is_visited.reset();
//        is_visited.clear_all();
//        std::fill(threads_computations_.begin(), threads_computations_.end(), 0);
    }
//    {//test
//        if (0 == query_id) {
//            exit(1);
//        }
//    }
}

}// namespace PANNS