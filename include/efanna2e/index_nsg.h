#ifndef EFANNA2E_INDEX_NSG_H
#define EFANNA2E_INDEX_NSG_H

#include "util.h"
#include "parameters.h"
#include "neighbor.h"
#include "index.h"
//#include "../utils.h"
#include <cassert>
#include <unordered_map>
#include <string>
#include <sstream>
#include <boost/dynamic_bitset.hpp>
#include <stack>

namespace efanna2e {

class IndexNSG : public Index {
public:
    explicit IndexNSG(const size_t dimension, const size_t n, Metric m, Index *initializer);


    virtual ~IndexNSG();

    virtual void Save(const char *filename) override;

    virtual void Load(const char *filename) override;


    virtual void Build(size_t n, const float *data, const Parameters &parameters) override;

    virtual void Search(
            const float *query,
            const float *x,
            size_t k,
            const Parameters &parameters,
            unsigned *indices) override;

    void SearchWithOptGraph(
            const float *query,
            size_t K,
            const Parameters &parameters,
            unsigned *indices);

    void OptimizeGraph(float *data);


    // Added by Johnpzh
    void get_true_NN(
            const float *query,
            unsigned K,
            std::vector<std::pair<unsigned, float> > &ngbrs);

//    void SearchWithOptGraph(
//            const float *query,
//            size_t K,
//            const Parameters &parameters,
//            std::vector<std::pair<unsigned, float> > &ngbrs);
//
//    void SearchWithOptGraph(
//            const float *query_load,
//            unsigned query_num,
//            unsigned query_dim,
//            size_t K,
//            const Parameters &parameters,
//            std::vector<std::vector<unsigned> > &res);
//
//    void SearchWithOptGraph(
//            const float *query_load,
//            unsigned query_num,
//            unsigned query_dim,
//            unsigned query_batch_start,
//            unsigned query_batch_size,
//            size_t K,
//            const Parameters &parameters,
//            std::vector<std::vector<unsigned> > &res);

//    void SearchWithOptGraphMeasurement(
//            const float *query,
//            size_t K,
//            const Parameters &parameters,
//            unsigned *indices);

//    void get_candidate_queues(
//            const float *query,
//            size_t K,
//            const Parameters &parameters,
//            std::vector<std::vector<unsigned> > &queues);
//
//    void load_true_NN(
//            const char *filename,
//            unsigned *&data,
////            unsigned query_num,
//            unsigned &t_K);
//
//    void get_recall_for_all_queries(
//            unsigned query_num,
//            unsigned K,
//            const unsigned *query_true_NN,
//            const std::vector<std::vector<unsigned>> &queries_result_NN,
//            std::unordered_map<unsigned, double> &recalls);
//
//    void BuildReverse(
//            size_t n,
//            const float *data,
//            const Parameters &parameters);
//
//    void get_top_ranks(
//            const float *query,
//            size_t K,
//            const Parameters &parameters,
//            std::vector<unsigned> &tops);
//
//    void DegreeDistribution(
//            std::vector<unsigned> &degree_to_count);
//
//    void SearchWithOptGraphToRecordTrace(
//            const float *query,
//            const Parameters &parameters,
//            std::vector<unsigned> &trace_ids,
//            std::vector<float> &trace);
//
//    void SearchWithOptGraphAndTrace(
//            const float *query,
//            size_t K,
//            unsigned *trace_ids,
//            float *trace,
//            size_t trace_size,
//            const Parameters &parameters,
//            unsigned *indices);
//
//    void SearchWithOptGraphAndTrace(
//            const float *query,
//            size_t K,
//            char *trace,
//            size_t trace_id_size,
//            const Parameters &parameters,
//            unsigned *indices);
//    void PrepareInitIDs(
//            std::vector<unsigned> &init_ids,
//            boost::dynamic_bitset<> &flags,
//            unsigned L);
//    void PrepareRetsetList(
//            const float *query_load,
//            std::vector<std::vector<Neighbor> > &retset_list,
//            std::vector<boost::dynamic_bitset<> > &is_visited_list,
//            const std::vector<unsigned> &init_ids,
//            const boost::dynamic_bitset<> &flags,
//            unsigned batch_start,
//            unsigned batch_size,
//            unsigned L);
//    void SearchWithOptGraphInBatch(
//            const float *query_load,
//            size_t K,
//            size_t L,
////            const Parameters &parameters,
//            std::vector< std::vector<Neighbor> > &retset_list,
//            std::vector< boost::dynamic_bitset<> > &is_visited_list,
////            const std::vector<unsigned> &init_ids,
////            const boost::dynamic_bitset<> &flags,
//            unsigned batch_start,
//            unsigned batch_size,
//            std::vector<std::vector<unsigned> > &indices_list);
//    void SearchWithOptGraphOnCUDA(
//            bool *d_switch);

    // For profiling.
//    double time_medoid;
    uint64_t count_distance_computation = 0;
//    double time_load_graph;
//    double time_init_graph;
//    double time_link;
//    double time_mrng;
//    double time_tree_grow;
//    std::vector< std::pair<double, double> > time_neighbors_latencies; // Latency of top-L neighbors
//    std::vector<uint32_t> count_neighbors_hops; // Hops of top-L neighbors
    double time_distance_computation = 0.0;
    double time_queue_operation = 0.0;
//    double time_initialization = 0.0; // too short
    double time_prefetch = 0.0;
    double time_small_data_load = 0.0;
//    double time_queue_status = 0.0; // too short
    double time_neighbor_traverse = 0.0;
//    double time_result_copy = 0.0; // too short
//    double time_misc_operations = 0.0; // too fine-grained

//    PANNS::L3CacheMissRate cache_miss_kernel;


    // Ended by Johnpzh

protected:
    typedef std::vector<std::vector<unsigned> > CompactGraph;
    typedef std::vector<SimpleNeighbors> LockGraph;
    typedef std::vector<nhood> KNNGraph;

    CompactGraph final_graph_;

    Index *initializer_;

    void init_graph(const Parameters &parameters);

    void get_neighbors(
            const float *query,
            const Parameters &parameter,
            std::vector<Neighbor> &retset,
            std::vector<Neighbor> &fullset);

    void get_neighbors(
            const float *query,
            const Parameters &parameter,
            boost::dynamic_bitset<> &flags,
            std::vector<Neighbor> &retset,
            std::vector<Neighbor> &fullset);

    //void add_cnn(unsigned des, Neighbor p, unsigned range, LockGraph& cut_graph_);
    void InterInsert(unsigned n, unsigned range, std::vector<std::mutex> &locks, SimpleNeighbor *cut_graph_);

    void
    sync_prune(unsigned q, std::vector<Neighbor> &pool, const Parameters &parameter, boost::dynamic_bitset<> &flags,
               SimpleNeighbor *cut_graph_);

    void Link(const Parameters &parameters, SimpleNeighbor *cut_graph_);

    void Load_nn_graph(const char *filename);

    void tree_grow(const Parameters &parameter);

    void DFS(boost::dynamic_bitset<> &flag, unsigned root, unsigned &cnt);

    void findroot(boost::dynamic_bitset<> &flag, unsigned &root, const Parameters &parameter);


private:
    unsigned width;
    unsigned ep_;
    std::vector<std::mutex> locks;
    char *opt_graph_;
    size_t node_size;
    size_t data_len;
    size_t neighbor_len;
    KNNGraph nnd_graph;
};
}

#endif //EFANNA2E_INDEX_NSG_H
