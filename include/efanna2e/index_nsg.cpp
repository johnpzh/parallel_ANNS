#include "index_nsg.h"

#include <omp.h>
#include <bitset>
#include <chrono>
#include <cmath>
#include <boost/dynamic_bitset.hpp>

//#include "efanna2e/exceptions.h"
//#include "efanna2e/parameters.h"

// Added by Johnpzh
//#include  "extras/utils.h"
// Ended by Johnpzh

namespace efanna2e {
#define _CONTROL_NUM 100

IndexNSG::IndexNSG(const size_t dimension, const size_t n, Metric m,
                   Index *initializer)
        : Index(dimension, n, m), initializer_{initializer}
{}

IndexNSG::~IndexNSG()
{}

void IndexNSG::Save(const char *filename)
{
    std::ofstream out(filename, std::ios::binary | std::ios::out);
    assert(final_graph_.size() == nd_);

    out.write((char *) &width, sizeof(unsigned));
    out.write((char *) &ep_, sizeof(unsigned));
    for (unsigned i = 0; i < nd_; i++) {
        unsigned GK = (unsigned) final_graph_[i].size();
        out.write((char *) &GK, sizeof(unsigned));
        out.write((char *) final_graph_[i].data(), GK * sizeof(unsigned));
    }
    out.close();
}

void IndexNSG::Load(const char *filename)
{
    std::ifstream in(filename, std::ios::binary);
    in.read((char *) &width, sizeof(unsigned));
    in.read((char *) &ep_, sizeof(unsigned));
    // width=100;
    unsigned cc = 0;
    while (!in.eof()) {
        unsigned k;
        in.read((char *) &k, sizeof(unsigned));
        if (in.eof()) break;
        cc += k;
        std::vector<unsigned> tmp(k);
        in.read((char *) tmp.data(), k * sizeof(unsigned));
        final_graph_.push_back(tmp);
    }
    cc /= nd_;
    // std::cout<<cc<<std::endl;
}

void IndexNSG::Load_nn_graph(const char *filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    unsigned k;
    in.read((char *) &k, sizeof(unsigned));
    in.seekg(0, std::ios::end);
    std::ios::pos_type ss = in.tellg();
    size_t fsize = (size_t) ss;
    size_t num = (unsigned) (fsize / (k + 1) / 4);
    in.seekg(0, std::ios::beg);

    final_graph_.resize(num);
    final_graph_.reserve(num);
    unsigned kk = (k + 3) / 4 * 4;
    for (size_t i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur);
        final_graph_[i].resize(k);
        final_graph_[i].reserve(kk);
        in.read((char *) final_graph_[i].data(), k * sizeof(unsigned));
    }
    in.close();
}

void IndexNSG::get_neighbors(const float *query, const Parameters &parameter,
                             std::vector<Neighbor> &retset,
                             std::vector<Neighbor> &fullset)
{
    unsigned L = parameter.Get<unsigned>("L");

    retset.resize(L + 1);
    std::vector<unsigned> init_ids(L);
    // initializer_->Search(query, nullptr, L, parameter, init_ids.data());

    boost::dynamic_bitset<> flags{nd_, 0};
    L = 0;
    for (unsigned i = 0; i < init_ids.size() && i < final_graph_[ep_].size(); i++) {
        init_ids[i] = final_graph_[ep_][i];
        flags[init_ids[i]] = true;
        L++;
    }
    while (L < init_ids.size()) {
        unsigned id = rand() % nd_;
        if (flags[id]) continue;
        init_ids[L] = id;
        L++;
        flags[id] = true;
    }

    L = 0;
    for (unsigned i = 0; i < init_ids.size(); i++) {
        unsigned id = init_ids[i];
        if (id >= nd_) continue;
        // std::cout<<id<<std::endl;
        float dist = distance_->compare(data_ + dimension_ * (size_t) id, query,
                                        (unsigned) dimension_);
        retset[i] = Neighbor(id, dist, true);
        // flags[id] = 1;
        L++;
    }

    std::sort(retset.begin(), retset.begin() + L);
    int k = 0;
    while (k < (int) L) {
        int nk = L;

        if (retset[k].flag) {
            retset[k].flag = false;
            unsigned n = retset[k].id;

            for (unsigned m = 0; m < final_graph_[n].size(); ++m) {
                unsigned id = final_graph_[n][m];
                if (flags[id]) continue;
                flags[id] = 1;

                float dist = distance_->compare(query, data_ + dimension_ * (size_t) id,
                                                (unsigned) dimension_);
                Neighbor nn(id, dist, true);
                fullset.push_back(nn);
                if (dist >= retset[L - 1].distance) continue;
                int r = InsertIntoPool(retset.data(), L, nn);

                if (L + 1 < retset.size()) ++L;
                if (r < nk) nk = r;
            }
        }
        if (nk <= k)
            k = nk;
        else
            ++k;
    }
}

void IndexNSG::get_neighbors(const float *query, const Parameters &parameter,
                             boost::dynamic_bitset<> &flags,
                             std::vector<Neighbor> &retset,
                             std::vector<Neighbor> &fullset)
{
    unsigned L = parameter.Get<unsigned>("L");

    retset.resize(L + 1);
    std::vector<unsigned> init_ids(L);
    // initializer_->Search(query, nullptr, L, parameter, init_ids.data());

    L = 0;
    for (unsigned i = 0; i < init_ids.size() && i < final_graph_[ep_].size(); i++) {
        init_ids[i] = final_graph_[ep_][i];
        flags[init_ids[i]] = true;
        L++;
    }
    while (L < init_ids.size()) {
        unsigned id = rand() % nd_;
        if (flags[id]) continue;
        init_ids[L] = id;
        L++;
        flags[id] = true;
    }

    L = 0;
    for (unsigned i = 0; i < init_ids.size(); i++) {
        unsigned id = init_ids[i];
        if (id >= nd_) continue;
        // std::cout<<id<<std::endl;
        float dist = distance_->compare(data_ + dimension_ * (size_t) id, query,
                                        (unsigned) dimension_);
        retset[i] = Neighbor(id, dist, true);
        fullset.push_back(retset[i]);
        // flags[id] = 1;
        L++;
    }

    std::sort(retset.begin(), retset.begin() + L);
    int k = 0;
    while (k < (int) L) {
        int nk = L;

        if (retset[k].flag) {
            retset[k].flag = false;
            unsigned n = retset[k].id;

            for (unsigned m = 0; m < final_graph_[n].size(); ++m) {
                unsigned id = final_graph_[n][m];
                if (flags[id]) continue;
                flags[id] = 1;

                float dist = distance_->compare(query, data_ + dimension_ * (size_t) id,
                                                (unsigned) dimension_);
                Neighbor nn(id, dist, true);
                fullset.push_back(nn);
                if (dist >= retset[L - 1].distance) continue;
                int r = InsertIntoPool(retset.data(), L, nn);

                if (L + 1 < retset.size()) ++L;
                if (r < nk) nk = r;
            }
        }
        if (nk <= k)
            k = nk;
        else
            ++k;
    }
}

void IndexNSG::init_graph(const Parameters &parameters)
{
    float *center = new float[dimension_];
    for (unsigned j = 0; j < dimension_; j++) center[j] = 0;
    for (unsigned i = 0; i < nd_; i++) {
        for (unsigned j = 0; j < dimension_; j++) {
            center[j] += data_[i * dimension_ + j];
        }
    }
    for (unsigned j = 0; j < dimension_; j++) {
        center[j] /= nd_;
    }
    std::vector<Neighbor> tmp, pool;
    ep_ = rand() % nd_;  // random initialize navigating point
    get_neighbors(center, parameters, tmp, pool);
    ep_ = tmp[0].id;
}

void IndexNSG::sync_prune(unsigned q, std::vector<Neighbor> &pool,
                          const Parameters &parameter,
                          boost::dynamic_bitset<> &flags,
                          SimpleNeighbor *cut_graph_)
{
    unsigned range = parameter.Get<unsigned>("R");
    unsigned maxc = parameter.Get<unsigned>("C");
    width = range;
    unsigned start = 0;

    for (unsigned nn = 0; nn < final_graph_[q].size(); nn++) {
        unsigned id = final_graph_[q][nn];
        if (flags[id]) continue;
        float dist =
                distance_->compare(data_ + dimension_ * (size_t) q,
                                   data_ + dimension_ * (size_t) id, (unsigned) dimension_);
        pool.push_back(Neighbor(id, dist, true));
    }

    std::sort(pool.begin(), pool.end());
    std::vector<Neighbor> result;
    if (pool[start].id == q) start++;
    result.push_back(pool[start]);

    while (result.size() < range && (++start) < pool.size() && start < maxc) {
        auto &p = pool[start];
        bool occlude = false;
        for (unsigned t = 0; t < result.size(); t++) {
            if (p.id == result[t].id) {
                occlude = true;
                break;
            }
            float djk = distance_->compare(data_ + dimension_ * (size_t) result[t].id,
                                           data_ + dimension_ * (size_t) p.id,
                                           (unsigned) dimension_);
            if (djk < p.distance /* dik */) {
                occlude = true;
                break;
            }
        }
        if (!occlude) result.push_back(p);
    }

    SimpleNeighbor *des_pool = cut_graph_ + (size_t) q * (size_t) range;
    for (size_t t = 0; t < result.size(); t++) {
        des_pool[t].id = result[t].id;
        des_pool[t].distance = result[t].distance;
    }
    if (result.size() < range) {
        des_pool[result.size()].distance = -1;
    }
}

void IndexNSG::InterInsert(unsigned n, unsigned range,
                           std::vector<std::mutex> &locks,
                           SimpleNeighbor *cut_graph_)
{
    SimpleNeighbor *src_pool = cut_graph_ + (size_t) n * (size_t) range;
    for (size_t i = 0; i < range; i++) {
        if (src_pool[i].distance == -1) break;

        SimpleNeighbor sn(n, src_pool[i].distance);
        size_t des = src_pool[i].id;
        SimpleNeighbor *des_pool = cut_graph_ + des * (size_t) range;

        std::vector<SimpleNeighbor> temp_pool;
        int dup = 0;
        {
            LockGuard guard(locks[des]);
            for (size_t j = 0; j < range; j++) {
                if (des_pool[j].distance == -1) break;
                if (n == des_pool[j].id) {
                    dup = 1;
                    break;
                }
                temp_pool.push_back(des_pool[j]);
            }
        }
        if (dup) continue;

        temp_pool.push_back(sn);
        if (temp_pool.size() > range) {
            std::vector<SimpleNeighbor> result;
            unsigned start = 0;
            std::sort(temp_pool.begin(), temp_pool.end());
            result.push_back(temp_pool[start]);
            while (result.size() < range && (++start) < temp_pool.size()) {
                auto &p = temp_pool[start];
                bool occlude = false;
                for (unsigned t = 0; t < result.size(); t++) {
                    if (p.id == result[t].id) {
                        occlude = true;
                        break;
                    }
                    float djk = distance_->compare(data_ + dimension_ * (size_t) result[t].id,
                                                   data_ + dimension_ * (size_t) p.id,
                                                   (unsigned) dimension_);
                    if (djk < p.distance /* dik */) {
                        occlude = true;
                        break;
                    }
                }
                if (!occlude) result.push_back(p);
            }
            {
                LockGuard guard(locks[des]);
                for (unsigned t = 0; t < result.size(); t++) {
                    des_pool[t] = result[t];
                }
            }
        } else {
            LockGuard guard(locks[des]);
            for (unsigned t = 0; t < range; t++) {
                if (des_pool[t].distance == -1) {
                    des_pool[t] = sn;
                    if (t + 1 < range) des_pool[t + 1].distance = -1;
                    break;
                }
            }
        }
    }
}

void IndexNSG::Link(const Parameters &parameters, SimpleNeighbor *cut_graph_)
{
    /*
    std::cout << " graph link" << std::endl;
    unsigned progress=0;
    unsigned percent = 100;
    unsigned step_size = nd_/percent;
    std::mutex progress_lock;
    */
    unsigned range = parameters.Get<unsigned>("R");
    std::vector<std::mutex> locks(nd_);

#pragma omp parallel
    {
        // unsigned cnt = 0;
        std::vector<Neighbor> pool, tmp;
        boost::dynamic_bitset<> flags{nd_, 0};
#pragma omp for schedule(dynamic, 100)
        for (unsigned n = 0; n < nd_; ++n) {
            pool.clear();
            tmp.clear();
            flags.reset();
            get_neighbors(data_ + dimension_ * n, parameters, flags, tmp, pool);
            sync_prune(n, pool, parameters, flags, cut_graph_);
            /*
          cnt++;
          if(cnt % step_size == 0){
            LockGuard g(progress_lock);
            std::cout<<progress++ <<"/"<< percent << " completed" << std::endl;
            }
            */
        }
    }

#pragma omp for schedule(dynamic, 100)
    for (unsigned n = 0; n < nd_; ++n) {
        InterInsert(n, range, locks, cut_graph_);
    }
}

void IndexNSG::Build(size_t n, const float *data, const Parameters &parameters)
{
    std::string nn_graph_path = parameters.Get<std::string>("nn_graph_path");
    unsigned range = parameters.Get<unsigned>("R");
//    // Added by Johnpzh
//    time_load_graph = -omp_get_wtime();
//    // Ended by Johnpzh
    Load_nn_graph(nn_graph_path.c_str());
//    // Added by Johnpzh
//    time_load_graph += omp_get_wtime();
//    time_init_graph = -omp_get_wtime();
//    // Ended by Johnpzh
    data_ = data;
    init_graph(parameters);
//    // Added by Johnpzh
//    time_init_graph += omp_get_wtime();
//    time_link = -omp_get_wtime();
//    // Ended by Johnpzh
    SimpleNeighbor *cut_graph_ = new SimpleNeighbor[nd_ * (size_t) range];
    Link(parameters, cut_graph_);
    final_graph_.resize(nd_);
//    // Added by Johnpzh
//    time_link += omp_get_wtime();
//    time_mrng = -omp_get_wtime();
//    // Ended by Johnpzh

    for (size_t i = 0; i < nd_; i++) {
        SimpleNeighbor *pool = cut_graph_ + i * (size_t) range;
        unsigned pool_size = 0;
        for (unsigned j = 0; j < range; j++) {
            if (pool[j].distance == -1) break;
            pool_size = j;
        }
        pool_size++;
        final_graph_[i].resize(pool_size);
        for (unsigned j = 0; j < pool_size; j++) {
            final_graph_[i][j] = pool[j].id;
        }
    }
//    // Added by Johnpzh
//    time_mrng += omp_get_wtime();
//    time_tree_grow = -omp_get_wtime();
//    // Ended by Johnpzh

    tree_grow(parameters);

//    // Added by Johnpzh
//    time_tree_grow += omp_get_wtime();
//    // Ended by Johnpzh

    unsigned max = 0, min = 1e6, avg = 0;
    for (size_t i = 0; i < nd_; i++) {
        auto size = final_graph_[i].size();
        max = max < size ? size : max;
        min = min > size ? size : min;
        avg += size;
    }
    avg /= 1.0 * nd_;
    printf("Degree Statistics: Max = %d, Min = %d, Avg = %d\n", max, min, avg);

    has_built = true;
}

void IndexNSG::Search(const float *query, const float *x, size_t K,
                      const Parameters &parameters, unsigned *indices)
{
    const unsigned L = parameters.Get<unsigned>("L_search");
    data_ = x; // Base data
    std::vector<Neighbor> retset(L + 1); // Return set
    std::vector<unsigned> init_ids(L); // Store initial candidates (vertex IDs)
    boost::dynamic_bitset<> flags{nd_, 0}; // Check flags
    // std::mt19937 rng(rand());
    // GenRandom(rng, init_ids.data(), L, (unsigned) nd_);

    // Store ep_'s neighbors as candidates
    unsigned tmp_l = 0;
    for (; tmp_l < L && tmp_l < final_graph_[ep_].size(); tmp_l++) {
        init_ids[tmp_l] = final_graph_[ep_][tmp_l];
        flags[init_ids[tmp_l]] = true;
    }

    // If ep_'s neighbors are not enough, add other random vertices
    while (tmp_l < L) {
        unsigned id = rand() % nd_;
        if (flags[id]) continue;
        flags[id] = true;
        init_ids[tmp_l] = id;
        tmp_l++;
    }

    // Get the distances of all candidates, store in the set retset.
    for (unsigned i = 0; i < init_ids.size(); i++) {
        unsigned id = init_ids[i];
        float dist =
                distance_->compare(data_ + dimension_ * id, query, (unsigned) dimension_);
        retset[i] = Neighbor(id, dist, true);
        // flags[id] = true;
    }

    std::sort(retset.begin(), retset.begin() + L);
    int k = 0; // the index of the 1st unchecked vertices in retset.
    while (k < (int) L) {
        int nk = L; // the minimum insert location of new candidates

        if (retset[k].flag) {
            retset[k].flag = false;
            unsigned n = retset[k].id;

            for (unsigned m = 0; m < final_graph_[n].size(); ++m) {
                unsigned id = final_graph_[n][m];
                if (flags[id]) continue;
                flags[id] = 1;
                float dist =
                        distance_->compare(query, data_ + dimension_ * id, (unsigned) dimension_);
                if (dist >= retset[L - 1].distance) continue;
                Neighbor nn(id, dist, true);
                int r = InsertIntoPool(retset.data(), L, nn); // insert location

                if (r < nk) nk = r;
            }
        }
        if (nk <= k)
            k = nk;
        else
            ++k;
    }
    for (size_t i = 0; i < K; i++) { // That's why L >= K required.
        indices[i] = retset[i].id;
    }
}

void IndexNSG::SearchWithOptGraph(const float *query, size_t K,
                                  const Parameters &parameters, unsigned *indices)
{
//    cache_miss_kernel.measure_start();
    unsigned L = parameters.Get<unsigned>("L_search");
    DistanceFastL2 *dist_fast = (DistanceFastL2 *) distance_;

    std::vector<Neighbor> retset(L + 1); // Return set
    std::vector<unsigned> init_ids(L); // Store initial candidates (vertex IDs)
    // std::mt19937 rng(rand());
    // GenRandom(rng, init_ids.data(), L, (unsigned) nd_);

    boost::dynamic_bitset<> flags{nd_, 0};// Check flags
    unsigned tmp_l = 0;
    unsigned *neighbors = (unsigned *) (opt_graph_ + node_size * ep_ + data_len);
    unsigned MaxM_ep = *neighbors;
    neighbors++;

    // Store ep_'s neighbors as candidates
    for (; tmp_l < L && tmp_l < MaxM_ep; tmp_l++) {
        init_ids[tmp_l] = neighbors[tmp_l];
        flags[init_ids[tmp_l]] = true;
    }

    // If ep_'s neighbors are not enough, add other random vertices
    // Added by Johnpzh
    unsigned tmp_id = ep_ + 1; // use tmp_id to replace rand().
    while (tmp_l < L) {
        tmp_id %= nd_;
        unsigned id = tmp_id++;
        if (flags[id]) continue;
        flags[id] = true;
        init_ids[tmp_l] = id;
        tmp_l++;
    }
    /////////////////////////////
    //
//    while (tmp_l < L) {
//        unsigned id = rand() % nd_;
//        if (flags[id]) continue;
//        flags[id] = true;
//        init_ids[tmp_l] = id;
//        tmp_l++;
//    }
    //
    /////////////////////////////
    // Ended y Johnpzh

    for (unsigned i = 0; i < init_ids.size(); i++) {
        unsigned id = init_ids[i];
        if (id >= nd_) continue;
        _mm_prefetch(opt_graph_ + node_size * id, _MM_HINT_T0);
    }
    L = 0;
    // Get the distances of all candidates, store in the set retset.
    for (unsigned i = 0; i < init_ids.size(); i++) {
        unsigned id = init_ids[i];
        if (id >= nd_) continue;
        float *x = (float *) (opt_graph_ + node_size * id);
        float norm_x = *x;
        x++;
//        time_distance_computation -= efanna2e::Utils::get_time_mark();
//        ++count_distance_computation;
        float dist = dist_fast->compare(x, query, norm_x, (unsigned) dimension_);
//        time_distance_computation += efanna2e::Utils::get_time_mark();
        retset[i] = Neighbor(id, dist, true);
        flags[id] = true;
        L++;
    }
    // std::cout<<L<<std::endl;

    std::sort(retset.begin(), retset.begin() + L);
//    cache_miss_kernel.measure_stop();
//    cache_miss_kernel.measure_start();
    int k = 0; // the index of the 1st unchecked vertices in retset.
    while (k < (int) L) {
        int nk = L; // the minimum insert location of new candidates

        if (retset[k].flag) {
            retset[k].flag = false;
            unsigned n = retset[k].id;

            _mm_prefetch(opt_graph_ + node_size * n + data_len, _MM_HINT_T0);
            unsigned *ngbrs = (unsigned *) (opt_graph_ + node_size * n + data_len);
            unsigned MaxM = *ngbrs;
            ngbrs++;
            for (unsigned m = 0; m < MaxM; ++m)
                _mm_prefetch(opt_graph_ + node_size * ngbrs[m], _MM_HINT_T0);
            for (unsigned m = 0; m < MaxM; ++m) {
                unsigned id = ngbrs[m];
                if (flags[id]) continue;
                flags[id] = 1;
                float *data = (float *) (opt_graph_ + node_size * id);
                float norm = *data;
                data++;
//                time_distance_computation -= efanna2e::Utils::get_time_mark();
//                ++count_distance_computation;
                float dist = dist_fast->compare(query, data, norm, (unsigned) dimension_);
//                time_distance_computation += efanna2e::Utils::get_time_mark();
                if (dist >= retset[L - 1].distance) continue;
                Neighbor nn(id, dist, true);
                int r = InsertIntoPool(retset.data(), L, nn); // insert location

                // if(L+1 < retset.size()) ++L;
                if (r < nk) nk = r;
            }
        }
        if (nk <= k) {
            k = nk;
        } else {
            ++k;
        }
    }
//    cache_miss_kernel.measure_stop();
    for (size_t i = 0; i < K; i++) {
        indices[i] = retset[i].id;
    }
}

//// Saved: measurement of some metrics
//// distance computation; average hops; average latency for top-1st and top-Kth
//void IndexNSG::SearchWithOptGraph(const float *query, size_t K,
//                                  const Parameters &parameters, unsigned *indices) {
//    unsigned L = parameters.Get<unsigned>("L_search");
//    DistanceFastL2 *dist_fast = (DistanceFastL2 *) distance_;
//
//    std::vector <Neighbor> retset(L + 1); // Return set
//    std::vector<unsigned> init_ids(L); // Store initial candidates (vertex IDs)
//    // std::mt19937 rng(rand());
//    // GenRandom(rng, init_ids.data(), L, (unsigned) nd_);
//
//    boost::dynamic_bitset<> flags{nd_, 0};// Check flags
//    unsigned tmp_l = 0;
//    unsigned *neighbors = (unsigned *) (opt_graph_ + node_size * ep_ + data_len);
//    unsigned MaxM_ep = *neighbors;
//    neighbors++;
//
//    // Store ep_'s neighbors as candidates
//    for (; tmp_l < L && tmp_l < MaxM_ep; tmp_l++) {
//        init_ids[tmp_l] = neighbors[tmp_l];
//        flags[init_ids[tmp_l]] = true;
//    }
//
//    // If ep_'s neighbors are not enough, add other random vertices
//    // Added by Johnpzh
//    unsigned tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    while (tmp_l < L) {
//        tmp_id %= nd_;
//        unsigned id = tmp_id++;
//        if (flags[id]) continue;
//        flags[id] = true;
//        init_ids[tmp_l] = id;
//        tmp_l++;
//    }
//    /////////////////////////////
//    //
////        while (tmp_l < L) {
////        unsigned id = rand() % nd_;
////        if (flags[id]) continue;
////        flags[id] = true;
////        init_ids[tmp_l] = id;
////        tmp_l++;
////        }
//    //
//    /////////////////////////////
//    // Ended y Johnpzh
//
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        _mm_prefetch(opt_graph_ + node_size * id, _MM_HINT_T0);
//    }
//    L = 0;
//    // Get the distances of all candidates, store in the set retset.
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        float *x = (float *) (opt_graph_ + node_size * id);
//        float norm_x = *x;
//        x++;
//        float dist = dist_fast->compare(x, query, norm_x, (unsigned) dimension_);
//        {// Added by Johnpzh
//            ++count_distance_computation;
//        }
//        retset[i] = Neighbor(id, dist, true);
//        flags[id] = true;
//        L++;
//    }
//    // std::cout<<L<<std::endl;
//
//    std::sort(retset.begin(), retset.begin() + L);
//    uint32_t hops = 0;
//    int k = 0; // the index of the 1st unchecked vertices in retset.
//    while (k < (int) L) {
//        int nk = L; // the minimum insert location of new candidates
//
//        if (retset[k].flag) {
//            retset[k].flag = false;
//            unsigned n = retset[k].id;
//            ++hops;
//
//            _mm_prefetch(opt_graph_ + node_size * n + data_len, _MM_HINT_T0);
//            unsigned *neighbors = (unsigned *) (opt_graph_ + node_size * n + data_len);
//            unsigned MaxM = *neighbors;
//            neighbors++;
//            for (unsigned m = 0; m < MaxM; ++m)
//                _mm_prefetch(opt_graph_ + node_size * neighbors[m], _MM_HINT_T0);
//            for (unsigned m = 0; m < MaxM; ++m) {
//                unsigned id = neighbors[m];
//                if (flags[id]) continue;
//                flags[id] = 1;
//                float *data = (float *) (opt_graph_ + node_size * id);
//                float norm = *data;
//                data++;
//                float dist = dist_fast->compare(query, data, norm, (unsigned) dimension_);
//                {// Added by Johnpzh
//                    ++count_distance_computation;
//                }
//                if (dist >= retset[L - 1].distance) continue;
//                Neighbor nn(id, dist, true);
//                int r = InsertIntoPool(retset.data(), L, nn); // insert location
//
//                // if(L+1 < retset.size()) ++L;
//                if (r < nk) nk = r;
//            }
//        }
//        if (nk <= k) {
//            {// Added by Johnpzh
//                time_neighbors_latencies[nk].second = efanna2e::Utils::get_time_mark() - time_neighbors_latencies[nk].first;
//                count_neighbors_hops[nk] = hops;
//            }
//            k = nk;
//        } else {
//            {// Added by Johnpzh
//                time_neighbors_latencies[k].second = efanna2e::Utils::get_time_mark() - time_neighbors_latencies[k].first;
//                count_neighbors_hops[k] = hops;
//            }
//            ++k;
//        }
//    }
//    for (size_t i = 0; i < K; i++) {
//        indices[i] = retset[i].id;
//    }
//}

void IndexNSG::OptimizeGraph(float *data)
{  // use after build or load

    data_ = data;
    data_len = (dimension_ + 1) * sizeof(float);
    neighbor_len = (width + 1) * sizeof(unsigned);
    node_size = data_len + neighbor_len;
    opt_graph_ = (char *) malloc(node_size * nd_);
    DistanceFastL2 *dist_fast = (DistanceFastL2 *) distance_;
    for (unsigned i = 0; i < nd_; i++) {
        char *cur_node_offset = opt_graph_ + i * node_size;
        float cur_norm = dist_fast->norm(data_ + i * dimension_, dimension_);
        std::memcpy(cur_node_offset, &cur_norm, sizeof(float));
        std::memcpy(cur_node_offset + sizeof(float), data_ + i * dimension_,
                    data_len - sizeof(float));

        cur_node_offset += data_len;
        unsigned k = final_graph_[i].size();
        std::memcpy(cur_node_offset, &k, sizeof(unsigned));
        std::memcpy(cur_node_offset + sizeof(unsigned), final_graph_[i].data(),
                    k * sizeof(unsigned));
        std::vector<unsigned>().swap(final_graph_[i]);
    }
    CompactGraph().swap(final_graph_);
}

void IndexNSG::DFS(boost::dynamic_bitset<> &flag, unsigned root, unsigned &cnt)
{
    unsigned tmp = root;
    std::stack<unsigned> s;
    s.push(root);
    if (!flag[root]) cnt++;
    flag[root] = true;
    while (!s.empty()) {
        unsigned next = nd_ + 1;
        for (unsigned i = 0; i < final_graph_[tmp].size(); i++) {
            if (flag[final_graph_[tmp][i]] == false) {
                next = final_graph_[tmp][i];
                break;
            }
        }
        // std::cout << next <<":"<<cnt <<":"<<tmp <<":"<<s.size()<< '\n';
        if (next == (nd_ + 1)) {
            s.pop();
            if (s.empty()) break;
            tmp = s.top();
            continue;
        }
        tmp = next;
        flag[tmp] = true;
        s.push(tmp);
        cnt++;
    }
}

void IndexNSG::findroot(boost::dynamic_bitset<> &flag, unsigned &root,
                        const Parameters &parameter)
{
    unsigned id = nd_;
    for (unsigned i = 0; i < nd_; i++) {
        if (flag[i] == false) {
            id = i;
            break;
        }
    }

    if (id == nd_) return;  // No Unlinked Node

    std::vector<Neighbor> tmp, pool;
    get_neighbors(data_ + dimension_ * id, parameter, tmp, pool);
    std::sort(pool.begin(), pool.end());

    unsigned found = 0;
    for (unsigned i = 0; i < pool.size(); i++) {
        if (flag[pool[i].id]) {
            // std::cout << pool[i].id << '\n';
            root = pool[i].id;
            found = 1;
            break;
        }
    }
    if (found == 0) {
        while (true) {
            unsigned rid = rand() % nd_;
            if (flag[rid]) {
                root = rid;
                break;
            }
        }
    }
    final_graph_[root].push_back(id);
}

void IndexNSG::tree_grow(const Parameters &parameter)
{
    unsigned root = ep_;
    boost::dynamic_bitset<> flags{nd_, 0};
    unsigned unlinked_cnt = 0;
    while (unlinked_cnt < nd_) {
        DFS(flags, root, unlinked_cnt);
        // std::cout << unlinked_cnt << '\n';
        if (unlinked_cnt >= nd_) break;
        findroot(flags, root, parameter);
        // std::cout << "new root"<<":"<<root << '\n';
    }
    for (size_t i = 0; i < nd_; ++i) {
        if (final_graph_[i].size() > width) {
            width = final_graph_[i].size();
        }
    }
}

//// Get the true top-K nearest neighbors of the query.
//void IndexNSG::get_true_NN(
//        const float *query,
//        unsigned K,
//        std::vector<std::pair<unsigned, float> > &ngbrs)
////        unsigned *indices)
//{
//    if (nd_ < K + 1) {
//        fprintf(stderr, "Error: nd_ %lu is smaller than K + 1 %u\n", nd_, K + 1);
//        exit(EXIT_FAILURE);
//    }
////    unsigned L = parameters.Get<unsigned>("L_search"); // Actually Top-L
//    DistanceFastL2 *dist_fast = (DistanceFastL2 *) distance_;
//    std::vector<Neighbor> retset(K + 1); // Return set
////    std::vector<Neighbor> test_set(nd_); // For testing
//
//    // Initail retset by the first L vertices
//    for (unsigned v_id = 0; v_id < K; ++v_id) {
//        // Get the distance from the vertex to the query (refer to IndexNSG::SearchWithOptGraph()).
//        float *x = (float *) (opt_graph_ + node_size * v_id);
//        float norm_x = *x;
//        ++x;
//        float dist = dist_fast->compare(x, query, norm_x, (unsigned) dimension_);
//        retset[v_id] = Neighbor(v_id, dist, true);
////        test_set[v_id] = retset[v_id];
//    }
//
//    // Sort retset
//    std::sort(retset.begin(), retset.begin() + K);
//
//    // Traverse all rest vertices (data)
//    for (unsigned v_id = K; v_id < nd_; ++v_id) {
//        // Get the distance from the vertex to the query (refer to IndexNSG::SearchWithOptGraph()).
//        float *x = (float *) (opt_graph_ + node_size * v_id);
//        float norm_x = *x;
//        ++x;
//        float dist = dist_fast->compare(x, query, norm_x, (unsigned) dimension_);
//        Neighbor tmp_n(v_id, dist, true);
//        InsertIntoPool(retset.data(), K, tmp_n);
////        test_set[v_id] = tmp_n;
//    }
//
////    std::sort(test_set.begin(), test_set.end());
////    for (unsigned i = 0; i < K; ++i) {
////        if (test_set[i].id != retset[i].id
////            && test_set[i].distance != test_set[i].distance) {
////            printf("Wrong: test_set[%u]: [%u, %f] retset[%u]: [%u, %f]\n",
////                    i, test_set[i].id, test_set[i].distance, i, retset[i].id, retset[i].distance);
////            break;
////        }
////    }
//    // Copy IDs to indices
//    ngbrs.resize(K);
//    for (unsigned i = 0; i < K; ++i) {
//        ngbrs[i].first = retset[i].id;
//        ngbrs[i].second = retset[i].distance;
//    }
////    for (unsigned i = 0; i < K; ++i) {
////        indices[i] = retset[i].id;
////    }
//}

//// Overloading of IndexNSG::SearchWithOptGraph:
//void IndexNSG::SearchWithOptGraph(
//        const float *query,
//        size_t K,
//        const Parameters &parameters,
//        std::vector<std::pair<unsigned, float> > &ngbrs)
//{
//    unsigned L = parameters.Get<unsigned>("L_search");
//    DistanceFastL2 *dist_fast = (DistanceFastL2 *) distance_;
//
//    std::vector<Neighbor> retset(L + 1); // Return set
//    std::vector<unsigned> init_ids(L); // Store initial candidates (vertex IDs)
//    // std::mt19937 rng(rand());
//    // GenRandom(rng, init_ids.data(), L, (unsigned) nd_);
//
//    boost::dynamic_bitset<> flags{nd_, 0};// Check flags
//    unsigned tmp_l = 0;
//    unsigned *neighbors = (unsigned *) (opt_graph_ + node_size * ep_ + data_len);
//    unsigned MaxM_ep = *neighbors;
//    neighbors++;
//
//    // Store ep_'s neighbors as candidates
//    for (; tmp_l < L && tmp_l < MaxM_ep; tmp_l++) {
//        init_ids[tmp_l] = neighbors[tmp_l];
//        flags[init_ids[tmp_l]] = true;
//    }
//
//    // If ep_'s neighbors are not enough, add other random vertices
//    // Added by Johnpzh
//    unsigned tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    while (tmp_l < L) {
//        tmp_id %= nd_;
//        unsigned id = tmp_id++;
//        if (flags[id]) continue;
//        flags[id] = true;
//        init_ids[tmp_l] = id;
//        tmp_l++;
//    }
//    /////////////////////////////
//    //
////        while (tmp_l < L) {
////        unsigned id = rand() % nd_;
////        if (flags[id]) continue;
////        flags[id] = true;
////        init_ids[tmp_l] = id;
////        tmp_l++;
////        }
//    //
//    /////////////////////////////
//    // Ended y Johnpzh
//
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        _mm_prefetch(opt_graph_ + node_size * id, _MM_HINT_T0);
//    }
//    L = 0;
//    // Get the distances of all candidates, store in the set retset.
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        float *x = (float *) (opt_graph_ + node_size * id);
//        float norm_x = *x;
//        x++;
//        float dist = dist_fast->compare(x, query, norm_x, (unsigned) dimension_);
//        retset[i] = Neighbor(id, dist, true);
//        flags[id] = true;
//        L++;
//    }
//    // std::cout<<L<<std::endl;
//
//    std::sort(retset.begin(), retset.begin() + L);
//    int k = 0; // the index of the 1st unchecked vertices in retset.
//    while (k < (int) L) {
//        int nk = L; // the minimum insert location of new candidates
//
//        if (retset[k].flag) {
//            retset[k].flag = false;
//            unsigned n = retset[k].id;
//
//            _mm_prefetch(opt_graph_ + node_size * n + data_len, _MM_HINT_T0);
//            unsigned *neighbors = (unsigned *) (opt_graph_ + node_size * n + data_len);
//            unsigned MaxM = *neighbors;
//            neighbors++;
//            for (unsigned m = 0; m < MaxM; ++m)
//                _mm_prefetch(opt_graph_ + node_size * neighbors[m], _MM_HINT_T0);
//            for (unsigned m = 0; m < MaxM; ++m) {
//                unsigned id = neighbors[m];
//                if (flags[id]) continue;
//                flags[id] = 1;
//                float *data = (float *) (opt_graph_ + node_size * id);
//                float norm = *data;
//                data++;
//                float dist = dist_fast->compare(query, data, norm, (unsigned) dimension_);
//                if (dist >= retset[L - 1].distance) continue;
//                Neighbor nn(id, dist, true);
//                int r = InsertIntoPool(retset.data(), L, nn); // insert location
//
//                // if(L+1 < retset.size()) ++L;
//                if (r < nk) nk = r;
//            }
//        }
//        if (nk <= k)
//            k = nk;
//        else
//            ++k;
//    }
////    for (size_t i = 0; i < K; i++) {
////        indices[i] = retset[i].id;
////    }
//    ngbrs.resize(K);
////    ngbrs.assign(retset.begin(), retset.begin() + K);
//    for (unsigned i = 0; i < K; ++i) {
//        ngbrs[i].first = retset[i].id;
//        ngbrs[i].second = retset[i].distance;
//    }
//}
//
///**
// * Overloading of SearchWithOptGraph: this version take all the same query at once (i.e. joint traversal)
// * @param query_load Query set
// * @param query_num Number of queries
// * @param query_dim Dimension of queries
// * @param K Length of final candidate queue
// * @param parameters Program parameters
// * @param res Vector of the final candidate queue for every query
// */
//void IndexNSG::SearchWithOptGraph(
//        const float *query_load,
//        unsigned query_num,
//        unsigned query_dim,
//        size_t K,
//        const Parameters &parameters,
//        std::vector<std::vector<unsigned> > &res)
//{
//    unsigned L = parameters.Get<unsigned>("L_search");
//    DistanceFastL2 *dist_fast = (DistanceFastL2 *) distance_;
//
//    std::vector<Neighbor> retset(L + 1); // Return set
//    std::vector<unsigned> init_ids(L); // Store initial candidates (vertex IDs)
//    // std::mt19937 rng(rand());
//    // GenRandom(rng, init_ids.data(), L, (unsigned) nd_);
//
//    boost::dynamic_bitset<> flags{nd_, 0};// Check flags
////    std::vector< boost::dynamic_bitset<> > is_visited_list(query_num, boost::dynamic_bitset<>(nd_, 0));
////    std::vector< boost::dynamic_bitset<> > is_visited_list(nd_, boost::dynamic_bitset<>(query_num, 0));
//    unsigned tmp_l = 0;
//    unsigned *neighbors = (unsigned *) (opt_graph_ + node_size * ep_ + data_len);
//    unsigned MaxM_ep = *neighbors;
//    neighbors++;
//
//    // Store ep_'s neighbors as candidates
//    for (; tmp_l < L && tmp_l < MaxM_ep; tmp_l++) {
//        init_ids[tmp_l] = neighbors[tmp_l];
//        flags[init_ids[tmp_l]] = true;
////        for (unsigned q_i = 0; q_i < query_num; ++q_i) {
////            is_visited_list[init_ids[tmp_l]][q_i] = true;
////        }
//    }
//
//    // If ep_'s neighbors are not enough, add other random vertices
//    // Added by Johnpzh
//    unsigned tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    while (tmp_l < L) {
//        tmp_id %= nd_;
//        unsigned id = tmp_id++;
//        if (flags[id]) continue;
//        flags[id] = true;
////        if (is_visited_list[id][0]) {
////            continue;
////        }
////        for (unsigned q_i = 0; q_i < query_num; ++q_i) {
////            is_visited_list[id][q_i] = true;
////        }
//        init_ids[tmp_l] = id;
//        tmp_l++;
//    }
//    /////////////////////////////
//    //
////        while (tmp_l < L) {
////        unsigned id = rand() % nd_;
////        if (flags[id]) continue;
////        flags[id] = true;
////        init_ids[tmp_l] = id;
////        tmp_l++;
////        }
//    //
//    /////////////////////////////
//    // Ended y Johnpzh
//
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        _mm_prefetch(opt_graph_ + node_size * id, _MM_HINT_T0);
//    }
//    L = 0;
//    // Get the distances of all candidates, store in the set retset.
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        float *x = (float *) (opt_graph_ + node_size * id);
//        float norm_x = *x;
//        x++;
////        float dist = dist_fast->compare(x, query, norm_x, (unsigned) dimension_);
////        retset[i] = Neighbor(id, dist, true);
//        flags[id] = true;
//        float dist = 0.0;
//        for (unsigned q_i = 0; q_i < query_num; ++q_i) {
//            dist = dist_fast->compare(x, query_load + q_i * query_dim, norm_x, (unsigned) dimension_);
////            is_visited_list[id][q_i] = true;
//        }
//        retset[i] = Neighbor(id, dist, true);
//        L++;
//    }
//    // std::cout<<L<<std::endl;
//
//    std::sort(retset.begin(), retset.begin() + L);
//    int k = 0; // the index of the 1st unchecked vertices in retset.
//    while (k < (int) L) {
//        int nk = L; // the minimum insert location of new candidates
//
//        if (retset[k].flag) {
//            retset[k].flag = false;
//            unsigned n = retset[k].id;
//
//            _mm_prefetch(opt_graph_ + node_size * n + data_len, _MM_HINT_T0);
//            unsigned *ngbrs = (unsigned *) (opt_graph_ + node_size * n + data_len);
//            unsigned MaxM = *ngbrs;
//            ngbrs++;
//            for (unsigned m = 0; m < MaxM; ++m)
//                _mm_prefetch(opt_graph_ + node_size * ngbrs[m], _MM_HINT_T0);
//            for (unsigned m = 0; m < MaxM; ++m) {
//                unsigned id = ngbrs[m];
//                if (flags[id]) continue;
//                flags[id] = 1;
////                float *data = (float *) (opt_graph_ + node_size * id);
////                float norm = *data;
////                data++;
////                float dist = dist_fast->compare(query, data, norm, (unsigned) dimension_);
////                if (is_visited_list[id][0]) {
////                    continue;
////                }
//                float *data = (float *) (opt_graph_ + node_size * id);
//                float norm = *data;
//                data++;
//                float dist = 0;
//                for (unsigned q_i = 0; q_i < query_num; ++q_i) {
////                    is_visited_list[id][q_i] = true;
//                    dist = dist_fast->compare(query_load + q_i * query_dim, data, norm, (unsigned) dimension_);
//                }
//                if (dist >= retset[L - 1].distance) continue;
//                Neighbor nn(id, dist, true);
//                int r = InsertIntoPool(retset.data(), L, nn); // insert location
//
//                // if(L+1 < retset.size()) ++L;
//                if (r < nk) nk = r;
//            }
//        }
//        if (nk <= k) {
//            k = nk;
//        } else {
//            ++k;
//        }
//    }
//
//    for (unsigned q_i = 0; q_i < query_num; ++q_i) {
//        for (size_t i = 0; i < K; i++) {
//            res[q_i][i] = retset[i].id;
////            indices[i] = retset[i].id;
//        }
//    }
//}
//
///**
// * Overloading of SearchWithOptGraph: this version take a batch of the same query at once (i.e. joint traversal)
// * @param query_load Query set
// * @param query_num Number of queries
// * @param query_dim Dimension of queries
// * @param query_batch_start Start query ID of this batch
// * @param query_batch_size Batch size of queries
// * @param K Length of final candidate queue
// * @param parameters Program parameters
// * @param res Vector of the final candidate queue for every query
// */
//void IndexNSG::SearchWithOptGraph(
//        const float *query_load,
//        unsigned query_num,
//        unsigned query_dim,
//        unsigned query_batch_start,
//        unsigned query_batch_size,
//        size_t K,
//        const Parameters &parameters,
//        std::vector<std::vector<unsigned> > &res)
//{
//    unsigned query_batch_bound = query_batch_start + query_batch_size;
//    unsigned L = parameters.Get<unsigned>("L_search");
//    DistanceFastL2 *dist_fast = (DistanceFastL2 *) distance_;
//
//    std::vector<Neighbor> retset(L + 1); // Return set
//    std::vector<unsigned> init_ids(L); // Store initial candidates (vertex IDs)
//    // std::mt19937 rng(rand());
//    // GenRandom(rng, init_ids.data(), L, (unsigned) nd_);
//
//    boost::dynamic_bitset<> flags{nd_, 0};// Check flags
////    std::vector< boost::dynamic_bitset<> > is_visited_list(query_num, boost::dynamic_bitset<>(nd_, 0));
////    std::vector< boost::dynamic_bitset<> > is_visited_list(nd_, boost::dynamic_bitset<>(query_num, 0));
//    unsigned tmp_l = 0;
//    unsigned *neighbors = (unsigned *) (opt_graph_ + node_size * ep_ + data_len);
//    unsigned MaxM_ep = *neighbors;
//    neighbors++;
//
//    // Store ep_'s neighbors as candidates
//    for (; tmp_l < L && tmp_l < MaxM_ep; tmp_l++) {
//        init_ids[tmp_l] = neighbors[tmp_l];
//        flags[init_ids[tmp_l]] = true;
////        for (unsigned q_i = 0; q_i < query_num; ++q_i) {
////            is_visited_list[init_ids[tmp_l]][q_i] = true;
////        }
//    }
//
//    // If ep_'s neighbors are not enough, add other random vertices
//    // Added by Johnpzh
//    unsigned tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    while (tmp_l < L) {
//        tmp_id %= nd_;
//        unsigned id = tmp_id++;
//        if (flags[id]) continue;
//        flags[id] = true;
////        if (is_visited_list[id][0]) {
////            continue;
////        }
////        for (unsigned q_i = 0; q_i < query_num; ++q_i) {
////            is_visited_list[id][q_i] = true;
////        }
//        init_ids[tmp_l] = id;
//        tmp_l++;
//    }
//    /////////////////////////////
//    //
////        while (tmp_l < L) {
////        unsigned id = rand() % nd_;
////        if (flags[id]) continue;
////        flags[id] = true;
////        init_ids[tmp_l] = id;
////        tmp_l++;
////        }
//    //
//    /////////////////////////////
//    // Ended y Johnpzh
//
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        _mm_prefetch(opt_graph_ + node_size * id, _MM_HINT_T0);
//    }
//    L = 0;
//    // Get the distances of all candidates, store in the set retset.
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        float *x = (float *) (opt_graph_ + node_size * id);
//        float norm_x = *x;
//        x++;
////        float dist = dist_fast->compare(x, query, norm_x, (unsigned) dimension_);
////        retset[i] = Neighbor(id, dist, true);
//        flags[id] = true;
//        float dist = 0.0;
//        for (unsigned q_i = query_batch_start; q_i < query_batch_bound; ++q_i) {
//            dist = dist_fast->compare(x, query_load + q_i * query_dim, norm_x, (unsigned) dimension_);
////            is_visited_list[id][q_i] = true;
//        }
//        retset[i] = Neighbor(id, dist, true);
//        L++;
//    }
//    // std::cout<<L<<std::endl;
//
//    std::sort(retset.begin(), retset.begin() + L);
//    int k = 0; // the index of the 1st unchecked vertices in retset.
//    while (k < (int) L) {
//        int nk = L; // the minimum insert location of new candidates
//
//        if (retset[k].flag) {
//            retset[k].flag = false;
//            unsigned n = retset[k].id;
//
//            _mm_prefetch(opt_graph_ + node_size * n + data_len, _MM_HINT_T0);
//            unsigned *ngbrs = (unsigned *) (opt_graph_ + node_size * n + data_len);
//            unsigned MaxM = *ngbrs;
//            ngbrs++;
//            for (unsigned m = 0; m < MaxM; ++m)
//                _mm_prefetch(opt_graph_ + node_size * ngbrs[m], _MM_HINT_T0);
//            for (unsigned m = 0; m < MaxM; ++m) {
//                unsigned id = ngbrs[m];
//                if (flags[id]) continue;
//                flags[id] = 1;
////                float *data = (float *) (opt_graph_ + node_size * id);
////                float norm = *data;
////                data++;
////                float dist = dist_fast->compare(query, data, norm, (unsigned) dimension_);
////                if (is_visited_list[id][0]) {
////                    continue;
////                }
//                float *data = (float *) (opt_graph_ + node_size * id);
//                float norm = *data;
//                data++;
//                float dist = 0;
//                for (unsigned q_i = query_batch_start; q_i < query_batch_bound; ++q_i) {
////                    is_visited_list[id][q_i] = true;
//                    dist = dist_fast->compare(query_load + q_i * query_dim, data, norm, (unsigned) dimension_);
//                }
//                if (dist >= retset[L - 1].distance) continue;
//                Neighbor nn(id, dist, true);
//                int r = InsertIntoPool(retset.data(), L, nn); // insert location
//
//                // if(L+1 < retset.size()) ++L;
//                if (r < nk) nk = r;
//            }
//        }
//        if (nk <= k) {
//            k = nk;
//        } else {
//            ++k;
//        }
//    }
//
//    for (unsigned q_i = 0; q_i < query_num; ++q_i) {
//        for (size_t i = 0; i < K; i++) {
//            res[q_i][i] = retset[i].id;
////            indices[i] = retset[i].id;
//        }
//    }
//}


//void IndexNSG::SearchWithOptGraphMeasurement(
//        const float *query,
//        size_t K,
//        const Parameters &parameters,
//        unsigned *indices)
//{
////    time_initialization -= efanna2e::Utils::get_time_mark();
//    unsigned L = parameters.Get<unsigned>("L_search");
//    DistanceFastL2 *dist_fast = (DistanceFastL2 *) distance_;
//
//    std::vector<Neighbor> retset(L + 1); // Return set
//    std::vector<unsigned> init_ids(L); // Store initial candidates (vertex IDs)
//    // std::mt19937 rng(rand());
//    // GenRandom(rng, init_ids.data(), L, (unsigned) nd_);
//
//    boost::dynamic_bitset<> flags{nd_, 0};// Check flags
//    unsigned tmp_l = 0;
//    unsigned *neighbors = (unsigned *) (opt_graph_ + node_size * ep_ + data_len);
//    unsigned MaxM_ep = *neighbors;
//    neighbors++;
//
//    // Store ep_'s neighbors as candidates
//    for (; tmp_l < L && tmp_l < MaxM_ep; tmp_l++) {
//        init_ids[tmp_l] = neighbors[tmp_l];
//        flags[init_ids[tmp_l]] = true;
//    }
//
//    // If ep_'s neighbors are not enough, add other random vertices
//    // Added by Johnpzh
//    unsigned tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    while (tmp_l < L) {
//        tmp_id %= nd_;
//        unsigned id = tmp_id++;
//        if (flags[id]) continue;
//        flags[id] = true;
//        init_ids[tmp_l] = id;
//        tmp_l++;
//    }
//    /////////////////////////////
//    //
////        while (tmp_l < L) {
////        unsigned id = rand() % nd_;
////        if (flags[id]) continue;
////        flags[id] = true;
////        init_ids[tmp_l] = id;
////        tmp_l++;
////        }
//    //
//    /////////////////////////////
//    // Ended y Johnpzh
//
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        _mm_prefetch(opt_graph_ + node_size * id, _MM_HINT_T0);
//    }
//    L = 0;
//    // Get the distances of all candidates, store in the set retset.
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        float *x = (float *) (opt_graph_ + node_size * id);
//        float norm_x = *x;
//        x++;
//        float dist = dist_fast->compare(x, query, norm_x, (unsigned) dimension_);
//        retset[i] = Neighbor(id, dist, true);
//        flags[id] = true;
//        L++;
//    }
//    // std::cout<<L<<std::endl;
////    time_initialization += efanna2e::Utils::get_time_mark();
//
//    time_queue_operation -= efanna2e::Utils::get_time_mark();
//    std::sort(retset.begin(), retset.begin() + L);
//    time_queue_operation += efanna2e::Utils::get_time_mark();
////    uint32_t hops = 0;
////    time_misc_operations -= efanna2e::Utils::get_time_mark();
//    int k = 0; // the index of the 1st unchecked vertices in retset.
////    time_misc_operations += efanna2e::Utils::get_time_mark();
//    while (k < (int) L) {
////        time_misc_operations -= efanna2e::Utils::get_time_mark();
//        int nk = L; // the minimum insert location of new candidates
////        time_misc_operations += efanna2e::Utils::get_time_mark();
//        if (retset[k].flag) {
////            time_queue_status -= efanna2e::Utils::get_time_mark();
//            retset[k].flag = false;
//            unsigned n = retset[k].id;
////            time_queue_status += efanna2e::Utils::get_time_mark();
////            ++hops;
//
//            time_prefetch -= efanna2e::Utils::get_time_mark();
//            _mm_prefetch(opt_graph_ + node_size * n + data_len, _MM_HINT_T0);
//            time_prefetch += efanna2e::Utils::get_time_mark();
//            time_small_data_load -= efanna2e::Utils::get_time_mark();
//            unsigned *ngbrs = (unsigned *) (opt_graph_ + node_size * n + data_len);
//            unsigned MaxM = *ngbrs;
//            ngbrs++;
//            time_small_data_load += efanna2e::Utils::get_time_mark();
//            time_prefetch -= efanna2e::Utils::get_time_mark();
//            for (unsigned m = 0; m < MaxM; ++m)
//                _mm_prefetch(opt_graph_ + node_size * ngbrs[m], _MM_HINT_T0);
//            time_prefetch += efanna2e::Utils::get_time_mark();
//
//            for (unsigned m = 0; m < MaxM; ++m) {
//                time_neighbor_traverse -= efanna2e::Utils::get_time_mark();
//                unsigned id = ngbrs[m];
//                if (flags[id]) {
//                    time_neighbor_traverse += efanna2e::Utils::get_time_mark();
//                    continue;
//                }
//                time_neighbor_traverse += efanna2e::Utils::get_time_mark();
//                time_small_data_load -= efanna2e::Utils::get_time_mark();
//                flags[id] = 1;
//                float *data = (float *) (opt_graph_ + node_size * id);
//                float norm = *data;
//                data++;
//                time_small_data_load += efanna2e::Utils::get_time_mark();
//                time_distance_computation -= efanna2e::Utils::get_time_mark();
//                float dist = dist_fast->compare(query, data, norm, (unsigned) dimension_);
//                time_distance_computation += efanna2e::Utils::get_time_mark();
////                time_misc_operations -= efanna2e::Utils::get_time_mark();
//                if (dist >= retset[L - 1].distance) {
////                    time_misc_operations += efanna2e::Utils::get_time_mark();
//                    continue;
//                }
//                Neighbor nn(id, dist, true);
////                time_misc_operations += efanna2e::Utils::get_time_mark();
//                time_queue_operation -= efanna2e::Utils::get_time_mark();
//                int r = InsertIntoPool(retset.data(), L, nn); // insert location
//                time_queue_operation += efanna2e::Utils::get_time_mark();
//
//                // if(L+1 < retset.size()) ++L;
////                time_misc_operations -= efanna2e::Utils::get_time_mark();
//                if (r < nk) nk = r;
////                time_misc_operations += efanna2e::Utils::get_time_mark();
//            }
//        }
////        time_queue_status -= efanna2e::Utils::get_time_mark();
//        if (nk <= k) {
//            k = nk;
//        } else {
//            ++k;
//        }
////        time_queue_status += efanna2e::Utils::get_time_mark();
//    }
////    time_result_copy -= efanna2e::Utils::get_time_mark();
//    for (size_t i = 0; i < K; i++) {
//        indices[i] = retset[i].id;
//    }
////    time_result_copy += efanna2e::Utils::get_time_mark();
//}

//// Function: Record all candidate queues
//void IndexNSG::get_candidate_queues(
//        const float *query,
//        size_t K,
//        const Parameters &parameters,
//        std::vector<std::vector<unsigned> > &queues)
//{
//    // Lambda: use ids in current retset to form a queue and push it into queues.
//    auto push_back_retset = [&](std::vector<Neighbor> &retset) {
//        std::vector<unsigned> ids(retset.size());
//        for (unsigned id_i = 0; id_i < retset.size(); ++id_i) {
//            ids[id_i] = retset[id_i].id;
//        }
//        queues.push_back(ids);
//    };
//    unsigned L = parameters.Get<unsigned>("L_search");
//    DistanceFastL2 *dist_fast = (DistanceFastL2 *) distance_;
//
//    std::vector<Neighbor> retset(L + 1); // Return set
//    std::vector<unsigned> init_ids(L); // Store initial candidates (vertex IDs)
//    // std::mt19937 rng(rand());
//    // GenRandom(rng, init_ids.data(), L, (unsigned) nd_);
//
//    boost::dynamic_bitset<> flags{nd_, 0};// Check flags
//    unsigned tmp_l = 0;
//    unsigned *neighbors = (unsigned *) (opt_graph_ + node_size * ep_ + data_len);
//    unsigned MaxM_ep = *neighbors;
//    neighbors++;
//
//    // Store ep_'s neighbors as candidates
//    for (; tmp_l < L && tmp_l < MaxM_ep; tmp_l++) {
//        init_ids[tmp_l] = neighbors[tmp_l];
//        flags[init_ids[tmp_l]] = true;
//    }
//
//    // If ep_'s neighbors are not enough, add other random vertices
//    // Added by Johnpzh
//    unsigned tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    while (tmp_l < L) {
//        tmp_id %= nd_;
//        unsigned id = tmp_id++;
//        if (flags[id]) continue;
//        flags[id] = true;
//        init_ids[tmp_l] = id;
//        tmp_l++;
//    }
//    /////////////////////////////
//    //
////        while (tmp_l < L) {
////        unsigned id = rand() % nd_;
////        if (flags[id]) continue;
////        flags[id] = true;
////        init_ids[tmp_l] = id;
////        tmp_l++;
////        }
//    //
//    /////////////////////////////
//    // Ended y Johnpzh
//
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        _mm_prefetch(opt_graph_ + node_size * id, _MM_HINT_T0);
//    }
//    L = 0;
//    // Get the distances of all candidates, store in the set retset.
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        float *x = (float *) (opt_graph_ + node_size * id);
//        float norm_x = *x;
//        x++;
//        float dist = dist_fast->compare(x, query, norm_x, (unsigned) dimension_);
//        retset[i] = Neighbor(id, dist, true);
//        flags[id] = true;
//        L++;
//    }
//    // std::cout<<L<<std::endl;
//    {
//        push_back_retset(retset);
//    }
//
//    std::sort(retset.begin(), retset.begin() + L);
//    int k = 0; // the index of the 1st unchecked vertices in retset.
//    while (k < (int) L) {
//        int nk = L; // the minimum insert location of new candidates
////        bool is_queue_updated = false;
//        if (retset[k].flag) {
//            retset[k].flag = false;
//            unsigned n = retset[k].id;
//
//            _mm_prefetch(opt_graph_ + node_size * n + data_len, _MM_HINT_T0);
//            unsigned *ngbrs = (unsigned *) (opt_graph_ + node_size * n + data_len);
//            unsigned MaxM = *ngbrs;
//            ngbrs++;
//            for (unsigned m = 0; m < MaxM; ++m)
//                _mm_prefetch(opt_graph_ + node_size * ngbrs[m], _MM_HINT_T0);
//            for (unsigned m = 0; m < MaxM; ++m) {
//                unsigned id = ngbrs[m];
//                if (flags[id]) continue;
//                flags[id] = 1;
//                float *data = (float *) (opt_graph_ + node_size * id);
//                float norm = *data;
//                data++;
//                float dist = dist_fast->compare(query, data, norm, (unsigned) dimension_);
//                if (dist >= retset[L - 1].distance) continue;
//                Neighbor nn(id, dist, true);
//                int r = InsertIntoPool(retset.data(), L, nn); // insert location
//
//                // if(L+1 < retset.size()) ++L;
//                if (r < nk) nk = r;
//
////                {
////                    if (r < (int) L) {
////                        is_queue_updated = true;
////                    }
////                }
//            }
//        }
//        if (nk <= k) {
//            k = nk;
//        } else {
//            ++k;
//        }
//        {
////            if (is_queue_updated) {
////                push_back_retset(retset);
////            }
//            push_back_retset(retset);
//        }
//    }
//}

///**
// * Load those true top-K neighbors (ground truth) of quries
// * @param filename
// * @param[out] data
//// * @param query_num
// * @param[out] t_K Value of K for top-K true neighbors
// */
//void IndexNSG::load_true_NN(
//        const char *filename,
//        unsigned *&data,
////        unsigned query_num,
//        unsigned &t_K)
//{
//    std::ifstream fin(filename);
//    if (!fin.is_open()) {
//        fprintf(stderr, "Error: cannot open file %s\n", filename);
//        exit(EXIT_FAILURE);
//    }
//    unsigned t_query_num;
////    unsigned t_K;
//    fin.read(reinterpret_cast<char *>(&t_query_num), sizeof(t_query_num));
//    fin.read(reinterpret_cast<char *>(&t_K), sizeof(t_K));
////    if (t_query_num != query_num) {
////        fprintf(stderr, "Error: query_num %u is not equal to the record %u in true-NN file %s\n",
////                query_num, t_query_num, filename);
////        exit(EXIT_FAILURE);
////    }
//
//    data = new unsigned[(size_t) t_query_num * (size_t) t_K];
//
//    for (unsigned q_i = 0; q_i < t_query_num; ++q_i) {
//        size_t offset = q_i * t_K;
//        for (unsigned n_i = 0; n_i < t_K; ++n_i) {
//            unsigned id;
//            float dist;
//            fin.read(reinterpret_cast<char *>(&id), sizeof(id));
//            fin.read(reinterpret_cast<char *>(&dist), sizeof(dist));
//            data[offset + n_i] = id;
//        }
//    }
//
//    fin.close();
//}

//void IndexNSG::get_recall_for_all_queries(
//        unsigned query_num,
//        unsigned t_K,
//        const unsigned *query_true_NN,
//        const std::vector<std::vector<unsigned>> &queries_result_NN,
//        std::unordered_map<unsigned, double> &recalls)
//{
//    if (t_K < 100) {
//        fprintf(stderr, "Error: t_K %u is smaller than 100.\n", t_K);
//        exit(EXIT_FAILURE);
//    }
//
//    for (unsigned q_i = 0; q_i < query_num; ++q_i) {
//        size_t offset = q_i * t_K;
//        for (unsigned top_i = 0; top_i < 100; ++top_i) {
//            unsigned true_id = query_true_NN[offset + top_i];
//            for (unsigned n_i = 0; n_i < 100; ++n_i) {
//                if (queries_result_NN[q_i][n_i] == true_id) {
//                    if (n_i < 5) recalls[5] += 1;
//                    if (n_i < 10) recalls[10] += 1;
//                    if (n_i < 20) recalls[20] += 1;
//                    if (n_i < 50) recalls[50] += 1;
//                    if (n_i < 100) recalls[100] += 1;
//                }
//            }
//        }
//    }
//    recalls[5] /= 5.0 * query_num;
//    recalls[10] /= 10.0 * query_num;
//    recalls[20] /= 20.0 * query_num;
//    recalls[50] /= 50.0 * query_num;
//    recalls[100] /= 100.0 * query_num;
//}

///**
// * Build the NSG graph but with reversed edges.
// * @param n
// * @param data
// * @param parameters
// */
//void IndexNSG::BuildReverse(
//        size_t n,
//        const float *data,
//        const Parameters &parameters)
//{
//    std::string nn_graph_path = parameters.Get<std::string>("nn_graph_path");
//    unsigned range = parameters.Get<unsigned>("R");
//    Load_nn_graph(nn_graph_path.c_str());
//    data_ = data;
//    init_graph(parameters);
//    SimpleNeighbor *cut_graph_ = new SimpleNeighbor[nd_ * (size_t) range];
//    Link(parameters, cut_graph_);
//    final_graph_.resize(nd_);
//
//    for (size_t i = 0; i < nd_; i++) {
//        SimpleNeighbor *pool = cut_graph_ + i * (size_t) range;
//        unsigned pool_size = 0;
//        for (unsigned j = 0; j < range; j++) {
//            if (pool[j].distance == -1) break;
//            pool_size = j;
//        }
//        pool_size++;
//        final_graph_[i].resize(pool_size);
//        for (unsigned j = 0; j < pool_size; j++) {
//            final_graph_[i][j] = pool[j].id;
//        }
//    }
//    tree_grow(parameters);
//
//    {// Reverse the graph
//        std::vector<std::vector<unsigned> > reversed_list(nd_);
//
//        for (unsigned head = 0; head < nd_; ++head) {
//            for (unsigned i_tail = 0; i_tail < final_graph_[head].size(); ++i_tail) {
//                unsigned tail = final_graph_[head][i_tail];
//                reversed_list[tail].push_back(head);
//            }
//        }
//        final_graph_.swap(reversed_list);
//
//        unsigned max_degree = 0;
//        unsigned min_degree = (unsigned) -1;
//        unsigned avg_degree = 0;
//        for (const auto &row : reversed_list) {
//            auto degree = row.size();
//            max_degree = degree > max_degree ? degree : max_degree;
//            min_degree = degree < min_degree ? degree : min_degree;
//            avg_degree += degree;
//        }
//        avg_degree /= nd_;
//        printf("Original_Degree_Statistics: Max: %u Min: %u Avg: %u\n", max_degree, min_degree, avg_degree);
//    }
//    unsigned max = 0, min = 1e6, avg = 0;
//    for (size_t i = 0; i < nd_; i++) {
//        auto size = final_graph_[i].size();
//        max = max < size ? size : max;
//        min = min > size ? size : min;
//        avg += size;
//    }
//    if (max > width) {
//        width = max; // Careful!
//    }
//    avg /= 1.0 * nd_;
//    printf("Reversed_Degree_Statistics: Max: %d Min: %d Avg: %d\n", max, min, avg);
//
//    has_built = true;
//}

///**
// * Search, record the top-ranked candidate in every iteration.
// * @param query
// * @param K
// * @param parameters
// * @param[out] tops All top-ranked candidates.
// */
//void IndexNSG::get_top_ranks(
//        const float *query,
//        size_t K,
//        const Parameters &parameters,
//        std::vector<unsigned> &tops)
//{
//    unsigned L = parameters.Get<unsigned>("L_search");
//    DistanceFastL2 *dist_fast = (DistanceFastL2 *) distance_;
//
//    std::vector<Neighbor> retset(L + 1); // Return set
//    std::vector<unsigned> init_ids(L); // Store initial candidates (vertex IDs)
//    // std::mt19937 rng(rand());
//    // GenRandom(rng, init_ids.data(), L, (unsigned) nd_);
//
//    boost::dynamic_bitset<> flags{nd_, 0};// Check flags
//    unsigned tmp_l = 0;
//    unsigned *neighbors = (unsigned *) (opt_graph_ + node_size * ep_ + data_len);
//    unsigned MaxM_ep = *neighbors;
//    neighbors++;
//
//    // Store ep_'s neighbors as candidates
//    for (; tmp_l < L && tmp_l < MaxM_ep; tmp_l++) {
//        init_ids[tmp_l] = neighbors[tmp_l];
//        flags[init_ids[tmp_l]] = true;
//    }
//
//    // If ep_'s neighbors are not enough, add other random vertices
//    // Added by Johnpzh
//    unsigned tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    while (tmp_l < L) {
//        tmp_id %= nd_;
//        unsigned id = tmp_id++;
//        if (flags[id]) continue;
//        flags[id] = true;
//        init_ids[tmp_l] = id;
//        tmp_l++;
//    }
//    /////////////////////////////
//    //
////        while (tmp_l < L) {
////        unsigned id = rand() % nd_;
////        if (flags[id]) continue;
////        flags[id] = true;
////        init_ids[tmp_l] = id;
////        tmp_l++;
////        }
//    //
//    /////////////////////////////
//    // Ended y Johnpzh
//
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        _mm_prefetch(opt_graph_ + node_size * id, _MM_HINT_T0);
//    }
//    L = 0;
//    // Get the distances of all candidates, store in the set retset.
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        float *x = (float *) (opt_graph_ + node_size * id);
//        float norm_x = *x;
//        x++;
//        float dist = dist_fast->compare(x, query, norm_x, (unsigned) dimension_);
//        retset[i] = Neighbor(id, dist, true);
//        flags[id] = true;
//        L++;
//    }
//    // std::cout<<L<<std::endl;
//
//    std::sort(retset.begin(), retset.begin() + L);
//    uint32_t hops = 0;
//    int k = 0; // the index of the 1st unchecked vertices in retset.
//    while (k < (int) L) {
//        int nk = L; // the minimum insert location of new candidates
//
//        if (retset[k].flag) {
//            retset[k].flag = false;
//            unsigned n = retset[k].id;
//            {
//                tops.push_back(n);
//            }
//            ++hops;
//
//            _mm_prefetch(opt_graph_ + node_size * n + data_len, _MM_HINT_T0);
//            unsigned *ngbrs = (unsigned *) (opt_graph_ + node_size * n + data_len);
//            unsigned MaxM = *ngbrs;
//            ngbrs++;
//            for (unsigned m = 0; m < MaxM; ++m)
//                _mm_prefetch(opt_graph_ + node_size * ngbrs[m], _MM_HINT_T0);
//            for (unsigned m = 0; m < MaxM; ++m) {
//                unsigned id = ngbrs[m];
//                if (flags[id]) continue;
//                flags[id] = 1;
//                float *data = (float *) (opt_graph_ + node_size * id);
//                float norm = *data;
//                data++;
//                float dist = dist_fast->compare(query, data, norm, (unsigned) dimension_);
//                if (dist >= retset[L - 1].distance) continue;
//                Neighbor nn(id, dist, true);
//                int r = InsertIntoPool(retset.data(), L, nn); // insert location
//
//                // if(L+1 < retset.size()) ++L;
//                if (r < nk) nk = r;
//            }
//        }
//        if (nk <= k) {
//            k = nk;
//        } else {
//            ++k;
//        }
//    }
////    for (size_t i = 0; i < K; i++) {
////        indices[i] = retset[i].id;
////    }
//}
//
///**
// * Get graph degree distribution.
// * @param[out] degree_to_count
// */
//void IndexNSG::DegreeDistribution(
//        std::vector<unsigned> &degree_to_count)
//{
//    unsigned max_degree = 0;
//    for (unsigned v_i = 0; v_i < nd_; ++v_i) {
//        unsigned degree = final_graph_[v_i].size();
//        if (degree > max_degree) {
//            max_degree = degree;
//        }
//    }
//
//    degree_to_count.resize(max_degree + 1);
//    for (unsigned v_i = 0; v_i < nd_; ++v_i) {
//        unsigned degree = final_graph_[v_i].size();
//        ++degree_to_count[degree];
//    }
//}
//
///**
// * Record the data trace when doing the searching.
// * Then the trace could be used to do the ideal computation without graph traverse.
// * @param query
// * @param K
// * @param parameters
// * @param indices
// * @param[out] trace Trace recorded
// */
//void IndexNSG::SearchWithOptGraphToRecordTrace(
//        const float *query,
//        const Parameters &parameters,
//        std::vector<unsigned> &trace_ids,
//        std::vector<float> &trace)
//{
//    unsigned L = parameters.Get<unsigned>("L_search");
//    DistanceFastL2 *dist_fast = (DistanceFastL2 *) distance_;
//
//    std::vector<Neighbor> retset(L + 1); // Return set
//    std::vector<unsigned> init_ids(L); // Store initial candidates (vertex IDs)
//    // std::mt19937 rng(rand());
//    // GenRandom(rng, init_ids.data(), L, (unsigned) nd_);
//
//    boost::dynamic_bitset<> flags{nd_, 0};// Check flags
//    unsigned tmp_l = 0;
//    unsigned *neighbors = (unsigned *) (opt_graph_ + node_size * ep_ + data_len);
//    unsigned MaxM_ep = *neighbors;
//    neighbors++;
//
//    // Store ep_'s neighbors as candidates
//    for (; tmp_l < L && tmp_l < MaxM_ep; tmp_l++) {
//        init_ids[tmp_l] = neighbors[tmp_l];
//        flags[init_ids[tmp_l]] = true;
//    }
//
//    // If ep_'s neighbors are not enough, add other random vertices
//    // Added by Johnpzh
//    unsigned tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    while (tmp_l < L) {
//        tmp_id %= nd_;
//        unsigned id = tmp_id++;
//        if (flags[id]) continue;
//        flags[id] = true;
//        init_ids[tmp_l] = id;
//        tmp_l++;
//    }
//    /////////////////////////////
//    //
////        while (tmp_l < L) {
////        unsigned id = rand() % nd_;
////        if (flags[id]) continue;
////        flags[id] = true;
////        init_ids[tmp_l] = id;
////        tmp_l++;
////        }
//    //
//    /////////////////////////////
//    // Ended y Johnpzh
//
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        _mm_prefetch(opt_graph_ + node_size * id, _MM_HINT_T0);
//    }
//    L = 0;
//    // Get the distances of all candidates, store in the set retset.
//    for (unsigned i = 0; i < init_ids.size(); i++) {
//        unsigned id = init_ids[i];
//        if (id >= nd_) continue;
//        float *x = (float *) (opt_graph_ + node_size * id);
//        {// Record the trace
//            trace_ids.push_back(id);
//            float *d = x;
//            for (unsigned loc = 0; loc < dimension_ + 1; ++loc) {
//                trace.push_back(*d++);
//            }
//        }
//        float norm_x = *x;
//        x++;
//        float dist = dist_fast->compare(x, query, norm_x, (unsigned) dimension_);
////        {
////            fprintf(stdout, "query: %f i: %u id: %u dist: %f\n", *query, i, id, dist);
////        }
//        retset[i] = Neighbor(id, dist, true);
//        flags[id] = true;
//        L++;
//    }
//    // std::cout<<L<<std::endl;
//
//    std::sort(retset.begin(), retset.begin() + L);
//    uint32_t hops = 0;
//    int k = 0; // the index of the 1st unchecked vertices in retset.
//    while (k < (int) L) {
//        int nk = L; // the minimum insert location of new candidates
//
//        if (retset[k].flag) {
//            retset[k].flag = false;
//            unsigned n = retset[k].id;
//            ++hops;
//
//            _mm_prefetch(opt_graph_ + node_size * n + data_len, _MM_HINT_T0);
//            unsigned *ngbrs = (unsigned *) (opt_graph_ + node_size * n + data_len);
//            unsigned MaxM = *ngbrs;
//            ngbrs++;
//            for (unsigned m = 0; m < MaxM; ++m)
//                _mm_prefetch(opt_graph_ + node_size * ngbrs[m], _MM_HINT_T0);
//            for (unsigned m = 0; m < MaxM; ++m) {
//                unsigned id = ngbrs[m];
//                if (flags[id]) continue;
//                flags[id] = 1;
//                float *data = (float *) (opt_graph_ + node_size * id);
//                {// Record the trace
//                    trace_ids.push_back(id);
//                    float *d = data;
//                    for (unsigned loc = 0; loc < dimension_ + 1; ++loc) {
//                        trace.push_back(*d++);
//                    }
//                }
//                float norm = *data;
//                data++;
//                float dist = dist_fast->compare(query, data, norm, (unsigned) dimension_);
////                {
////                    fprintf(stdout, "query: %f id: %u dist: %f\n", *query, id, dist);
////                }
//                if (dist >= retset[L - 1].distance) continue;
//                Neighbor nn(id, dist, true);
//                int r = InsertIntoPool(retset.data(), L, nn); // insert location
//
//                // if(L+1 < retset.size()) ++L;
//                if (r < nk) nk = r;
//            }
//        }
//        if (nk <= k) {
//            k = nk;
//        } else {
//            ++k;
//        }
//    }
//}
//
///**
// * According to trace, do the same computation but without graph traverse.
// * @param query
// * @param K
// * @param trace_ids
// * @param trace
// * @param trace_size
// * @param parameters
// * @param indices
// */
//void IndexNSG::SearchWithOptGraphAndTrace(
//        const float *query,
//        size_t K,
//        unsigned *trace_ids,
//        float *trace,
//        size_t trace_id_size,
//        const Parameters &parameters,
//        unsigned *indices)
//{
//    unsigned L = parameters.Get<unsigned>("L_search");
//    DistanceFastL2 *dist_fast = (DistanceFastL2 *) distance_;
//
//    std::vector<Neighbor> retset(L + 1); // Return set
//    size_t trace_i = 0;
//    for (unsigned i = 0; i < L; ++i) {
//        ++trace_i;
//        unsigned id = *trace_ids++;
//        float norm_x = *trace;
//        ++trace;
////        time_distance_computation -= efanna2e::Utils::get_time_mark();
//        float dist = dist_fast->compare(query, trace, norm_x, (unsigned) dimension_);
////        time_distance_computation += efanna2e::Utils::get_time_mark();
//        trace += dimension_;
//        retset[i] = Neighbor(id, dist, true);
//    }
//
//    std::sort(retset.begin(), retset.begin() + L);
//    while (trace_i < trace_id_size) {
//        ++trace_i;
//        unsigned id = *trace_ids++;
//        float norm = *trace;
//        ++trace;
////        time_distance_computation -= efanna2e::Utils::get_time_mark();
//        float dist = dist_fast->compare(query, trace, norm, (unsigned) dimension_);
////        time_distance_computation += efanna2e::Utils::get_time_mark();
//        trace += dimension_;
//        if (dist >= retset[L - 1].distance) continue;
//        Neighbor nn(id, dist, true);
//        InsertIntoPool(retset.data(), L, nn); // insert location
//    }
//    for (size_t i = 0; i < K; i++) {
//        indices[i] = retset[i].id;
//    }
//}
//
//void IndexNSG::SearchWithOptGraphAndTrace(
//        const float *query,
//        size_t K,
//        char *trace,
//        size_t trace_id_size,
//        const Parameters &parameters,
//        unsigned *indices)
//{
//    unsigned L = parameters.Get<unsigned>("L_search");
//    DistanceFastL2 *dist_fast = (DistanceFastL2 *) distance_;
//
//    std::vector<Neighbor> retset(L + 1); // Return set
//    size_t trace_i = 0;
//    for (unsigned i = 0; i < L; ++i) {
//        ++trace_i;
//        unsigned id = *((unsigned *) trace);
//        trace += 4;
//        float norm_x = *((float *) trace);
//        trace += 4;
////        time_distance_computation -= efanna2e::Utils::get_time_mark();
////        ++count_distance_computation;
//        float dist = dist_fast->compare(query, (float *) trace, norm_x, (unsigned) dimension_);
////        time_distance_computation += efanna2e::Utils::get_time_mark();
//        trace += 4 * dimension_;
////        unsigned id = *trace_ids++;
////        float norm_x = *trace;
////        ++trace;
////        trace += dimension_;
//        retset[i] = Neighbor(id, dist, true);
//    }
//
//    std::sort(retset.begin(), retset.begin() + L);
//    while (trace_i < trace_id_size) {
//        ++trace_i;
//        unsigned id = *((unsigned *) trace);
//        trace += 4;
//        float norm = *((float *) trace);
//        trace += 4;
////        time_distance_computation -= efanna2e::Utils::get_time_mark();
////        ++count_distance_computation;
//        float dist = dist_fast->compare(query, (float *) trace, norm, (unsigned) dimension_);
////        time_distance_computation += efanna2e::Utils::get_time_mark();
//        trace += 4 * dimension_;
////        unsigned id = *trace_ids++;
////        float norm = *trace;
////        ++trace;
////        float dist = dist_fast->compare(query, trace, norm, (unsigned) dimension_);
////        trace += dimension_;
//        if (dist >= retset[L - 1].distance) continue;
//        Neighbor nn(id, dist, true);
//        InsertIntoPool(retset.data(), L, nn); // insert location
//    }
//    for (size_t i = 0; i < K; i++) {
//        indices[i] = retset[i].id;
//    }
//}

///**
// * Prepare init_ids and flags, as they are constant for all queries.
// * @param[out] init_ids
// * @param[out] flags
// * @param L
// */
//void IndexNSG::PrepareInitIDs(
//        std::vector<unsigned> &init_ids,
//        boost::dynamic_bitset<> &flags,
//        unsigned L)
//{
//    unsigned *neighbors = (unsigned *) (opt_graph_ + node_size * ep_ + data_len);
//    unsigned MaxM_ep = *neighbors;
//    neighbors++;
//
//    // Store ep_'s neighbors as candidates
//    unsigned tmp_l = 0;
//    for (; tmp_l < L && tmp_l < MaxM_ep; tmp_l++) {
//        init_ids[tmp_l] = neighbors[tmp_l];
//        flags[init_ids[tmp_l]] = true;
//    }
//
//    // If ep_'s neighbors are not enough, add other random vertices
//    // Added by Johnpzh
//    unsigned tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    while (tmp_l < L) {
//        tmp_id %= nd_;
//        unsigned id = tmp_id++;
//        if (flags[id]) continue;
//        flags[id] = true;
//        init_ids[tmp_l] = id;
//        tmp_l++;
//    }
//}
//
///**
// * Initialize retset_list by init_ids; initialize is_visited_list by flags.
// * @param query_load
// * @param[out] retset_list
// * @param[out] is_visited_list
// * @param init_ids
// * @param flags
// * @param batch_start
// * @param batch_size
// * @param L
// */
//void IndexNSG::PrepareRetsetList(
//        const float *query_load,
//        std::vector<std::vector<Neighbor> > &retset_list,
//        std::vector<boost::dynamic_bitset<> > &is_visited_list,
//        const std::vector<unsigned> &init_ids,
//        const boost::dynamic_bitset<> &flags,
//        unsigned batch_start,
//        unsigned batch_size,
//        unsigned L)
//{
//    auto *dist_fast = reinterpret_cast<DistanceFastL2 *>(distance_);
//    for (unsigned q_i = 0; q_i < batch_size; ++q_i) {
//        is_visited_list[q_i] = flags;
//    }
////        for (unsigned i = 0; i < init_ids.size(); i++) {
////            unsigned id = init_ids[i];
//////            if (id >= nd_) continue;
////            _mm_prefetch(opt_graph_ + node_size * id, _MM_HINT_T0);
////        }
//    for (unsigned q_i = 0; q_i < batch_size; ++q_i) {
//        // Get the distances of all candidates, store in the set retset.
//        const float *query = query_load + (q_i + batch_start) * dimension_;
//        for (unsigned i = 0; i < L; i++) {
//            unsigned id = init_ids[i];
//            float *x = (float *) (opt_graph_ + node_size * id);
//            float norm_x = *x;
//            x++;
////                ++count_distance_computation;
//            float dist = dist_fast->compare(x, query, norm_x, (unsigned) dimension_);
//            retset_list[q_i][i] = Neighbor(id, dist, true);
//        }
////        std::sort(retset_list[q_i].begin(), retset_list[q_i].begin() + L);
//    }
//
//    for (auto &set : retset_list) {
//        std::sort(set.begin(), set.begin() + L);
//    }
//}

// /**
//  * Batched search, given query_load is already batched.
//  * @param query_load
//  * @param K
//  * @param L
//  * @param retset_list
//  * @param is_visited_list
//  * @param batch_start
//  * @param batch_size
//  * @param[out] indices_list
//  */
//void IndexNSG::SearchWithOptGraphInBatch(
//        const float *query_load,
//        size_t K,
//        size_t L,
////        const Parameters &parameters,
//        std::vector<std::vector<Neighbor> > &retset_list,
//        std::vector<boost::dynamic_bitset<> > &is_visited_list,
////        const std::vector<unsigned> &init_ids,
////        const boost::dynamic_bitset<> &flags,
//        unsigned batch_start,
//        unsigned batch_size,
//        std::vector<std::vector<unsigned> > &indices_list)
//{
////    unsigned L = parameters.Get<unsigned>("L_search");
//    auto *dist_fast = (DistanceFastL2 *) distance_;
//
////    std::vector< std::vector<Neighbor> > retset_list(batch_size, std::vector<Neighbor>(L + 1)); // Return set
////    std::vector< boost::dynamic_bitset<> > is_visited_list(batch_size, boost::dynamic_bitset<>(nd_));// Check flags
////    std::vector <Neighbor> retset(L + 1); // Return set
////    std::vector<unsigned> init_ids(L); // Store initial candidates (vertex IDs)
////    boost::dynamic_bitset<> flags{nd_, 0};// Check flags
//    { // Initialize retset_list
////        std::vector<unsigned> init_ids(L);
////        boost::dynamic_bitset<> flags(nd_);
////        unsigned *neighbors = (unsigned *) (opt_graph_ + node_size * ep_ + data_len);
////        unsigned MaxM_ep = *neighbors;
////        neighbors++;
////
////        // Store ep_'s neighbors as candidates
////        unsigned tmp_l = 0;
////        for (; tmp_l < L && tmp_l < MaxM_ep; tmp_l++) {
////            init_ids[tmp_l] = neighbors[tmp_l];
////            flags[init_ids[tmp_l]] = true;
////        }
////
////        // If ep_'s neighbors are not enough, add other random vertices
////        // Added by Johnpzh
////        unsigned tmp_id = ep_ + 1; // use tmp_id to replace rand().
////        while (tmp_l < L) {
////            tmp_id %= nd_;
////            unsigned id = tmp_id++;
////            if (flags[id]) continue;
////            flags[id] = true;
////            init_ids[tmp_l] = id;
////            tmp_l++;
////        }
//        // Ended y Johnpzh
//        // Copy array to list
////        for (unsigned q_i = 0; q_i < batch_size; ++q_i) {
////            is_visited_list[q_i] = flags;
////        }
//
////        for (unsigned i = 0; i < init_ids.size(); i++) {
////            unsigned id = init_ids[i];
//////            if (id >= nd_) continue;
////            _mm_prefetch(opt_graph_ + node_size * id, _MM_HINT_T0);
////        }
////    L = 0;
//
////        for (unsigned q_i = 0; q_i < batch_size; ++q_i) {
////            // Get the distances of all candidates, store in the set retset.
////            const float *query = query_load + (q_i + batch_start) * dimension_;
////            for (unsigned i = 0; i < L; i++) {
////                unsigned id = init_ids[i];
//////                if (id >= nd_) continue;
////                float *x = (float *) (opt_graph_ + node_size * id);
////                float norm_x = *x;
////                x++;
//////                ++count_distance_computation;
////                float dist = dist_fast->compare(x, query, norm_x, (unsigned) dimension_);
////                retset_list[q_i][i] = Neighbor(id, dist, true);
//////                retset[i] = Neighbor(id, dist, true);
//////            flags[id] = true;
//////        L++;
////            }
////            std::sort(retset_list[q_i].begin(), retset_list[q_i].begin() + L);
////        }
//
////        std::sort(retset.begin(), retset.begin() + L);
//    }
//
//    std::vector<unsigned> ks(batch_size, 0);
//    boost::dynamic_bitset<> is_finished(batch_size);
//    while (!is_finished.all()) {
//        // Traverse all every query in the batch
//        for (unsigned q_i = 0; q_i < batch_size; ++q_i) {
//            if (is_finished[q_i]) {
//                continue;
//            }
//            const float *query = query_load + (q_i + batch_start) * dimension_;
//            auto &retset = retset_list[q_i];
//            unsigned k = ks[q_i];
//            unsigned nk = L;
//            if (retset[k].flag) {
//                retset[k].flag = false;
//                unsigned n = retset[k].id;
//
//                _mm_prefetch(opt_graph_ + node_size * n + data_len, _MM_HINT_T0);
//                auto *ngbrs = (unsigned *) (opt_graph_ + node_size * n + data_len);
//                unsigned MaxM = *ngbrs;
//                ngbrs++;
//                // Prefetch is not necessarily better for performance.
////                for (unsigned m = 0; m < MaxM; ++m)
////                    _mm_prefetch(opt_graph_ + node_size * ngbrs[m], _MM_HINT_T0);
//                for (unsigned m = 0; m < MaxM; ++m) {
//                    unsigned id = ngbrs[m];
//                    if (is_visited_list[q_i][id]) continue;
//                    is_visited_list[q_i][id] = true;
//                    auto *data = (float *) (opt_graph_ + node_size * id);
//                    float norm = *data;
//                    data++;
////                    ++count_distance_computation;
//                    float dist = dist_fast->compare(query, data, norm, (unsigned) dimension_);
//                    if (dist >= retset[L - 1].distance) continue;
//                    Neighbor nn(id, dist, true);
//                    unsigned r = InsertIntoPool(retset.data(), L, nn); // insert location
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
//            indices_list[q_i + batch_start][k_i] = retset_list[q_i][k_i].id;
//        }
//    }
//}

} // namespace efanna2e