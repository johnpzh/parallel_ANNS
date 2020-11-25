//
// Created by Zhen Peng on 08/28/2020.
//

#include "Searching.202008281536.error_count.h"

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
        std::unordered_map<unsigned, double> &recalls) const
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
    for (unsigned q_i = 0; q_i < num_queries_; ++q_i) {

//        double test_last_recall = recalls[100]; //test

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
//        {//test
//            printf("q_i: %u "
//                   "corrects: %u\n",
//                   q_i,
//                   (idi) (recalls[100] - test_last_recall));
//        }
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
//    else if (!queue2[queue2_start].is_checked_ && it_loc->is_checked_) {
//        it_loc->is_checked_ = false;
//    }
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
//            if (!queue2[q_i_2].is_checked_ && queue1[q_i_1].is_checked_) {
//                queue1[q_i_1].is_checked_ = false;
//            }
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
//    else if (!queue2[queue2_start].is_checked_ && it_loc->is_checked_) {
//        it_loc->is_checked_ = false;
//    }
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
//            if (!queue2[q_i_2].is_checked_ && queue1[q_i_1].is_checked_) {
//                queue1[q_i_1].is_checked_ = false;
//            }
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
        const idi L) const
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
//                idi r = merge_two_queues_into_1st_queue_seq_fixed(
//                        set_L,
//                        a_start,
//                        L,
//                        set_L,
//                        b_start,
//                        b_size);
                idi r = merge_two_queues_into_1st_queue_seq_incr(
                        set_L,
                        a_start,
                        a_size,
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
 * Function: pick master queue's top-M unchecked elements to top_m_candidates.
 * Used by interval merge.
 */
bool Searching::assign_to_workers(
//        const idi M,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &local_queues_starts,
        std::vector<idi> &local_queues_sizes,
        const idi local_queue_capacity) const
//        const idi k_uc) const
{
//    bool has_unchecked = false;
//    idi c_i_start = local_queues_starts[num_threads_ - 1];
//    idi c_i_bound = c_i_start + local_queues_sizes[num_threads_ - 1];
//    local_queues_sizes[num_threads_ - 1] = 0;
//    int dest_queue = 0;
//    for (idi c_i = c_i_start; c_i < c_i_bound; ++c_i) {
//        set_L[local_queues_starts[dest_queue] + local_queues_sizes[dest_queue]++] = set_L[c_i];
//        if (!has_unchecked && !set_L[c_i].is_checked_) {
//            has_unchecked = true;
//        }
//        if (local_queues_sizes[dest_queue] == local_queue_capacity) {
//            break;
//        }
//        if (num_threads_ - 1 != dest_queue) {
//            ++dest_queue;
//        } else {
//            dest_queue = 0;
//        }
//    }
//
//    return has_unchecked;

    const idi last_queue_start = local_queues_starts[num_threads_ - 1];
    idi c_i_start = last_queue_start;
    idi c_i_bound = c_i_start + local_queues_sizes[num_threads_ - 1];
    idi top_m_count = 0;
    int dest_queue = 0;
    for (idi c_i = c_i_start; c_i < c_i_bound; ++c_i) {
        if (set_L[c_i].is_checked_) {
            continue;
        }
        ++top_m_count;
        if (num_threads_ - 1 != dest_queue) {
            set_L[local_queues_starts[dest_queue] + local_queues_sizes[dest_queue]++] = set_L[c_i];
            set_L[c_i].is_checked_ = true;
            if (local_queues_sizes[dest_queue] == local_queue_capacity) {
                break;
            }
            ++dest_queue;
        } else {
            dest_queue = 0;
        }
    }
    return top_m_count;

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
        uint64_t &local_count_computation,
        bool &is_quota_done)
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
    uint64_t tmp_last_count_computation = tmp_count_computation;
    idi nk = local_queue_capacity;

//    idi test_unvisited_count = 0;

    for (idi e_i = 0; e_i < out_degree; ++e_i) {
        idi nb_id = out_edges[e_i];
        { // Sequential edition
            if (is_visited[nb_id]) {
                continue;
            }
            is_visited[nb_id] = true;
        }
//        {//test
//            ++test_unvisited_count;
//        }

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
    threads_computations_[q_i] += tmp_count_computation - tmp_last_count_computation;
    if (threads_computations_[q_i] >= thread_compuation_quota_) {
        is_quota_done = true;
    }
    local_count_computation += tmp_count_computation;

//    {//test
////        if (!test_unvisited_count) {
//        printf("w_i: %u "
//               "visited: %u "
//               "added: %s\n",
//               q_i,
//               test_unvisited_count,
//               nk == local_queue_capacity ? "NO" : "YES");
////        }
//    }
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

bool Searching::is_error_happened(
        const int worker_id,
        const std::vector<Candidate> &set_L,
        const std::vector<idi> &local_queues_starts,
        const std::vector<idi> &local_queues_sizes) const
{
//    const int dest_id = (num_threads_ - 1 != worker_id ? worker_id + 1 : 0);
//    idi size_d = local_queues_sizes[dest_id];
//    if (size_d) {
//        distf dist_d = set_L[local_queues_starts[dest_id] + (size_d >> 1U)].distance_; // half location
//        distf dist_w = set_L[local_queues_starts[worker_id]].distance_;
//        if (dist_w <= dist_d) {
//            return false;
//        }
//    }

    return true;
}

/*
 * 08/28/2020-15:43
 * Asynchronous search with global error count
 */
void Searching::para_search_with_error_count_v1(
//        const idi M,
//        const idi worker_M,
        const idi query_id,
        const idi K,
        const idi L,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector<idi> &set_K,
        const idi local_queue_capacity, // Maximum size of local queue
        const std::vector<idi> &local_queues_starts,
        std::vector<idi> &local_queues_sizes, // Sizes of local queue
        boost::dynamic_bitset<> &is_visited,
        const idi errors_bound)
//        const idi subsearch_iterations)
{
//    time_initialization_ -= WallTimer::get_time_mark();
    const idi master_queue_start = local_queues_starts[num_threads_ - 1];
    idi &master_queue_size = local_queues_sizes[num_threads_ - 1];
    const dataf *query_data = queries_load_ + query_id * dimension_;

    // Initialization Phase
    initialize_set_L_para(
            query_data,
            L,
            set_L,
            master_queue_start,
            master_queue_size,
            init_ids,
            is_visited);
//    time_initialization_ += WallTimer::get_time_mark();

//    idi top_m_candidates_end = 0;

    idi iter = 0; // for debug

    // Sequential Version
    if (num_threads_ == 1) {
        const distf &last_dist = set_L[master_queue_start + master_queue_size - 1].distance_;
        idi k = 0; // Index of first unchecked candidate.
        idi r;
        idi cand_id;
        bool is_quota_done = false;
        uint64_t tmp_count_computation = 0;
        while (k < L && !is_quota_done) {
            ++iter;
            auto &cand = set_L[master_queue_start + k];
            if (!cand.is_checked_) {
                cand.is_checked_ = true;
                cand_id = cand.id_;
                r = expand_one_candidate(
                        0,
                        cand_id,
                        query_data,
                        last_dist,
                        set_L,
                        master_queue_start,
                        master_queue_size,
                        L,
                        is_visited,
                        tmp_count_computation,
                        is_quota_done);
                count_distance_computation_ += tmp_count_computation;
                tmp_count_computation = 0;
            } else {
                r = L;
            }
            if (r <= k) {
                k = r;
            } else {
                ++k;
            }
        }
    } else { // Parallel Version
        // Divide computation cost from thread 0 to others
        std::fill(
                threads_computations_.begin(),
                threads_computations_.end(),
                threads_computations_[0] / num_threads_);

        std::atomic_uint count_errors = {0};
//        idi k_master = 0; // Index of first unchecked candidate.
        idi para_iter = 0;
        uint64_t tmp_count_computation = 0;
        uint8_t count_workers_done = 0;
        while (count_workers_done < num_threads_) {
            ++iter;
            ++para_iter;
            // Assign candidates to workers
            if (!assign_to_workers(
                    set_L,
                    local_queues_starts,
                    local_queues_sizes,
                    local_queue_capacity)) {
                break;
            }

            count_workers_done = 0;
            // Expand
#pragma omp parallel reduction(+ : tmp_count_computation, count_workers_done)
            {
                bool is_quota_done = false;
                int w_i = omp_get_thread_num();
                const idi local_queue_start = local_queues_starts[w_i];
                idi &local_queue_size = local_queues_sizes[w_i];
//                const idi queue_capacity = num_threads_ - 1 != w_i ? local_queue_capacity : L;
                const distf &last_dist_local = set_L[local_queue_start + local_queue_size].distance_;
                idi k_uc = 0;
                idi cand_id;
                idi r;
                idi worker_iter = 0;
                while (count_errors.load(std::memory_order_relaxed) < errors_bound
                        && k_uc < local_queue_size
                        && !is_quota_done) {
//                while (worker_iter < subsearch_iterations && k_uc < local_queue_size && !is_quota_done) {
                    auto &cand = set_L[local_queue_start + k_uc];
                    if (!cand.is_checked_) {
                        cand.is_checked_ = true;
                        ++worker_iter;
                        cand_id = cand.id_;
                        r = expand_one_candidate(
                                w_i,
                                cand_id,
                                query_data,
//                                last_dist,
                                last_dist_local,
                                set_L,
                                local_queue_start,
                                local_queue_size,
//                                queue_capacity,
                                local_queue_capacity,
                                is_visited,
                                tmp_count_computation,
                                is_quota_done);
                        if (r <= k_uc) {
                            k_uc = r;
                        } else {
                            ++k_uc;
                        }

                        // Error acculate
                        if (is_error_happened(
                                w_i,
                                set_L,
                                local_queues_starts,
                                local_queues_sizes)) {
                            ++count_errors;
                        }
                    } else {
                        ++k_uc;
                    }
                } // Expand Top-1
//                if (num_threads_ - 1 == w_i) {
//                    k_master = k_uc;
//                }
                if (k_uc == local_queue_size || is_quota_done) {
                    ++count_workers_done;
                }
            } // Workers
            count_errors = 0;
            count_distance_computation_ += tmp_count_computation;
            tmp_count_computation = 0;
            // Merge
            {
//                idi r =
                merge_all_queues_to_master(
                        set_L,
                        local_queues_starts,
                        local_queues_sizes,
                        local_queue_capacity,
                        L);
//                if (r <= k_master) {
//                    k_master = r;
//                }
            }
        } // Search Iterations
    } // Parallel Phase

#pragma omp parallel for
    for (idi k_i = 0; k_i < K; ++k_i) {
        set_K[k_i] = set_L[k_i + master_queue_start].id_;
    }

    {// Reset
//        std::fill(is_visited.begin(), is_visited.end(), 0);
        is_visited.reset();
//        is_visited.clear_all();
//        std::fill(local_queues_sizes.begin(), local_queues_sizes.end(), 0);
        std::fill(threads_computations_.begin(), threads_computations_.end(), 0);
    }
//    {//test
//        printf("query_id: %u "
//               "iter: %u\n",
//               query_id,
//               iter);
//    }
//    {//test
//        if (1 == query_id) {
//            exit(1);
//        }
//    }
}

}// namespace PANNS