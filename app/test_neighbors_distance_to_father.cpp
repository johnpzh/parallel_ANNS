//
// Created by Zhen Peng on 5/15/2020.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include <clocale>
#include <omp.h>
//#include "../core/Searching.202002101535.reorganization.h"
//#include "../core/Searching.201912161559.set_for_queue.h"
//#include "../core/Searching.201912091448.map_for_queries_ids.h"
//#include "../core/Searching.h"
//#include "../include/utils.h"
//#include "../include/efanna2e/index_nsg.h"
//#include "../core/Searching.202002141745.critical_omp_top_m.h"
//#include "../core/Searching.202002181409.local_queue_and_merge.h"
//#include "../core/Searching.202002201424.parallel_merge_local_queues.h"
//#include "../core/Searching.202003021000.profile_para_top_m_search.h"
#include "../core/Searching.202004131634.better_merge.h"

void usage(char *argv[])
{
    fprintf(stderr,
            "Usage: %s <data_file> <nsg_path> <num_selected>\n",
//            "Usage: %s <data_file> <query_file> <nsg_path> <search_L> <search_K> <result_path> <value_M_max> <true_NN_file> <num_threads> <value_M_middle>\n",
            argv[0]);
}

int main(int argc, char **argv)
{
    if (argc != 4) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    setbuf(stdout, nullptr); // Remove stdout buffer.
    setlocale(LC_NUMERIC, ""); // For comma number format

    PANNS::Searching engine;
    engine.load_data_load(argv[1]);
    engine.load_nsg_graph(argv[2]);
    unsigned num_selected = std::strtoull(argv[3], nullptr, 0);
    if (num_selected > engine.num_v_) {
        fprintf(stderr,
                "Error: num_selected %u is larger then num of vertices %u.\n",
                num_selected, engine.num_v_);
        exit(EXIT_FAILURE);
    }

    engine.test_neighbors_normalized_distance_to_father(num_selected);
//    engine.test_neighbors_distance_to_father(num_selected);
    return 0;
}
