cmake_minimum_required(VERSION 3.9)
project(PANNS LANGUAGES CXX)

# Verbose Makefile
set(CMAKE_VERBOSE_MAKEFILE ON)

## C++20
#set(CMAKE_CXX_STANDARD 20)
# C++14
#set_property(GLOBAL PROPERTY CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

# Boost
find_package(Boost REQUIRED COMPONENTS thread)

# OpenMP
find_package(OpenMP REQUIRED)

# Debug Options
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(CXX_OPTION ${CXX_OPTION} -O0)
endif()
if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(CXX_OPTION ${CXX_OPTION} -Wall -Wextra -pedantic -Werror)
endif()
#set(CXX_OPTION ${CXX_OPTION} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free)
#set(LIBPAPI /usr/local/lib/libpapi.a)
set(CXX_OPTION ${CXX_OPTION} -march=native)

## Build Library
#add_library(PANNS
##        ./core/Searching.cpp
##        ./core/Searching.201912061447.parallel_in_a_batch.cpp
##        ./core/Searching.201912091448.map_for_queries_ids.cpp
##        ./core/Searching.201912161559.set_for_queue.cpp
##        ./core/Searching.202002101535.reorganization.cpp)
##        ./include/utils.cpp)
#target_compile_options(PANNS PRIVATE ${CXX_OPTION} -march=native)
#target_link_libraries(PANNS PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_library(NSG
        include/efanna2e/index_nsg.cpp
        include/efanna2e/index.cpp)
#target_compile_options(NSG PRIVATE ${CXX_OPTION} -march=native -mavx)
target_compile_options(NSG PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(NSG PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

# Bin

add_executable(test_nsg_optimized_search tools/test_nsg_optimized_search.cpp)
#target_compile_options(test_nsg_optimized_search PRIVATE ${CXX_OPTION} -march=native -mavx)
#target_compile_options(test_nsg_optimized_search PRIVATE ${CXX_OPTION} -march=native)
#target_compile_options(test_nsg_optimized_search PRIVATE ${CXX_OPTION} -xHost)
target_compile_options(test_nsg_optimized_search PRIVATE ${CXX_OPTION})
target_link_libraries(test_nsg_optimized_search PRIVATE NSG)
#target_link_libraries(test_nsg_optimized_search PRIVATE NSG ${LIBPAPI})

add_executable(test_nsg_search tools/test_nsg_search.cpp)
target_compile_options(test_nsg_search PRIVATE ${CXX_OPTION} -march=native)
#target_compile_options(test_nsg_search PRIVATE ${CXX_OPTION} -xHost)
target_link_libraries(test_nsg_search PRIVATE NSG)


add_executable(test_nsg_optimized_search_para tools/test_nsg_optimized_search_para.cpp)
#target_compile_options(test_nsg_optimized_search_para PRIVATE ${CXX_OPTION} -march=native -mavx)
#target_compile_options(test_nsg_optimized_search_para PRIVATE ${CXX_OPTION} -march=native)
#target_compile_options(test_nsg_optimized_search_para PRIVATE ${CXX_OPTION} -xHost)
target_compile_options(test_nsg_optimized_search_para PRIVATE ${CXX_OPTION})
target_link_libraries(test_nsg_optimized_search_para PRIVATE NSG OpenMP::OpenMP_CXX)
#target_link_libraries(test_nsg_optimized_search_para PRIVATE NSG ${LIBPAPI} OpenMP::OpenMP_CXX)

add_executable(widget_get_deep200M tools/widget_get_deep200M.cpp)
find_program(FOUND_GCC930 g++-9.3.0 /home/zpeng/lib/bin)
find_file(FOUND_CPP6 libstdc++.so.6.0.28 /data/zpeng/local/gcc-9.3.0/lib64)
if(FOUND_GCC930 AND FOUND_CPP6)
#    message(KNL!!!)
    ## For KNL
#    target_compile_options(widget_get_deep200M PRIVATE ${CXX_OPTION} -xHost -gxx-name=${FOUND_GCC930})
    target_compile_options(widget_get_deep200M PRIVATE ${CXX_OPTION} -gxx-name=${FOUND_GCC930})
#    target_compile_options(widget_get_deep200M PRIVATE ${CXX_OPTION} -march=native -mavx -gxx-name=${FOUND_GCC930})
    target_link_libraries(widget_get_deep200M PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX ${FOUND_CPP6})
else()
#    message("Not KNL!!! ${FOUND_GCC930} ${FOUND_CPP6}")
    ## For Skylake
#    target_compile_options(widget_get_deep200M PRIVATE ${CXX_OPTION} -xHost)
    target_compile_options(widget_get_deep200M PRIVATE ${CXX_OPTION})
#    target_compile_options(widget_get_deep200M PRIVATE ${CXX_OPTION} -march=native -mavx)
    target_link_libraries(widget_get_deep200M PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
endif()

add_executable(test.app app/test.app.cpp)
if(FOUND_GCC930 AND FOUND_CPP6)
    #target_compile_options(test PRIVATE ${CXX_OPTION} -march=native)
    #target_compile_options(test PRIVATE ${CXX_OPTION} -march=native -mavx)
    #target_compile_options(test PRIVATE ${CXX_OPTION} -march=native)
    #target_compile_options(test PRIVATE ${CXX_OPTION} -xHost)
    target_compile_options(test.app PRIVATE ${CXX_OPTION} -gxx-name=${FOUND_GCC930})
    #target_compile_options(test PRIVATE ${CXX_OPTION} -march=native -qopt-report -no-vec -unroll=0)
    #target_link_libraries(test PRIVATE ${LIBPAPI} ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
    target_link_libraries(test.app PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX ${FOUND_CPP6})
else()
    target_compile_options(test.app PRIVATE ${CXX_OPTION})
    target_link_libraries(test.app PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
endif()

add_executable(profile_para_single_query_top_m_search_middle_m app/profile_para_single_query_top_m_search_middle_m.cpp)
#target_compile_options(profile_para_single_query_top_m_search_middle_m PRIVATE ${CXX_OPTION} -march=native -mavx)
#target_compile_options(profile_para_single_query_top_m_search_better_merge PRIVATE ${CXX_OPTION} -march=native)
#target_compile_options(profile_para_single_query_top_m_search_middle_m PRIVATE ${CXX_OPTION} -xHost -lpapi)
#target_compile_options(profile_para_single_query_top_m_search_middle_m PRIVATE ${CXX_OPTION} -xHost)
target_compile_options(profile_para_single_query_top_m_search_middle_m PRIVATE ${CXX_OPTION})
#target_link_libraries(profile_para_single_query_top_m_search_middle_m PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX -lpapi)
target_link_libraries(profile_para_single_query_top_m_search_middle_m PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_seq_simple_search_two_queues app/profile_seq_simple_search_two_queues.cpp)
if(FOUND_GCC930 AND FOUND_CPP6)
#    message(KNL!!!)
    ## For KNL
    target_compile_options(profile_seq_simple_search_two_queues PRIVATE ${CXX_OPTION} -gxx-name=${FOUND_GCC930})
    target_link_libraries(profile_seq_simple_search_two_queues PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX ${FOUND_CPP6})
else()
    target_compile_options(profile_seq_simple_search_two_queues PRIVATE ${CXX_OPTION})
    target_link_libraries(profile_seq_simple_search_two_queues PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
endif()

add_executable(profile_para_simple_search_two_global_queues app/profile_para_simple_search_two_global_queues.cpp)
if(FOUND_GCC930 AND FOUND_CPP6)
#    message(KNL!!!)
    ## For KNL
    target_compile_options(profile_para_simple_search_two_global_queues PRIVATE ${CXX_OPTION} -gxx-name=${FOUND_GCC930})
    target_link_libraries(profile_para_simple_search_two_global_queues PRIVATE ${Boost_LIBRARIES} ${FOUND_CPP6})
#    target_link_libraries(profile_para_simple_search_two_global_queues PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX ${FOUND_CPP6})
else()
    target_compile_options(profile_para_simple_search_two_global_queues PRIVATE ${CXX_OPTION})
    target_link_libraries(profile_para_simple_search_two_global_queues PRIVATE ${Boost_LIBRARIES})
#    target_link_libraries(profile_para_simple_search_two_global_queues PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
endif()


add_executable(profile_para_simple_search_two_global_queues.workbuffer app/profile_para_simple_search_two_global_queues.workbuffer.cpp)
if(FOUND_GCC930 AND FOUND_CPP6)
    #    message(KNL!!!)
    ## For KNL
    target_compile_options(profile_para_simple_search_two_global_queues.workbuffer PRIVATE ${CXX_OPTION} -gxx-name=${FOUND_GCC930})
    target_link_libraries(profile_para_simple_search_two_global_queues.workbuffer PRIVATE ${Boost_LIBRARIES} ${FOUND_CPP6})
    #    target_link_libraries(profile_para_simple_search_two_global_queues PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX ${FOUND_CPP6})
else()
    target_compile_options(profile_para_simple_search_two_global_queues.workbuffer PRIVATE ${CXX_OPTION})
    target_link_libraries(profile_para_simple_search_two_global_queues.workbuffer PRIVATE ${Boost_LIBRARIES})
    #    target_link_libraries(profile_para_simple_search_two_global_queues PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
endif()

add_executable(profile_para_single_query_top_m_search_m_width.v2 app/profile_para_single_query_top_m_search_m_width.v2.cpp core/Searching.202008251657.m_width.v2.cpp)
target_compile_options(profile_para_single_query_top_m_search_m_width.v2 PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_top_m_search_m_width.v2 PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_seq_single_query_simple_search app/profile_find_L_seq_single_query_simple_search.cpp)
target_compile_options(profile_find_L_seq_single_query_simple_search PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_seq_single_query_simple_search PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX NSG)

add_executable(profile_para_single_query_async_search_error_count app/profile_para_single_query_async_search_error_count.cpp core/Searching.202008281536.error_count.cpp)
target_compile_options(profile_para_single_query_async_search_error_count PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_async_search_error_count PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_para_single_query_search_simple_v3 app/profile_para_single_query_search_simple_v3.cpp core/Searching.202008310636.simple_v3.cpp)
target_compile_options(profile_para_single_query_search_simple_v3 PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_simple_v3 PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_search_simple_v3 app/profile_find_L_para_single_query_search_simple_v3.cpp core/Searching.202008310636.simple_v3.cpp)
target_compile_options(profile_find_L_para_single_query_search_simple_v3 PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_search_simple_v3 PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(save_true_nearest_neighbors tools/save_true_nearest_neighbors.cpp core/Searching.202009171601.simple_v3.large_graph.cpp)
target_compile_options(save_true_nearest_neighbors PRIVATE ${CXX_OPTION})
#target_compile_options(save_true_nearest_neighbors PRIVATE ${CXX_OPTION} -xHost)
target_link_libraries(save_true_nearest_neighbors PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX NSG)

add_executable(test_nsg_index tools/test_nsg_index.cpp include/efanna2e/index_nsg.cpp)
#target_compile_options(test_nsg_index PRIVATE ${CXX_OPTION} -xHost)
target_compile_options(test_nsg_index PRIVATE ${CXX_OPTION})
target_link_libraries(test_nsg_index PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX NSG)

add_executable(test.true_NN tools/test.true_NN.cpp include/efanna2e/index_nsg.cpp)
#target_compile_options(test.true_NN PRIVATE ${CXX_OPTION} -xHost)
target_compile_options(test.true_NN PRIVATE ${CXX_OPTION})
target_link_libraries(test.true_NN PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX NSG)

add_executable(profile_para_single_query_search_simple_v3_large_graph app/profile_para_single_query_search_simple_v3_large_graph.cpp core/Searching.202009171601.simple_v3.large_graph.cpp)
target_compile_options(profile_para_single_query_search_simple_v3_large_graph PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_simple_v3_large_graph PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_seq_single_query_simple_search_large_graph app/profile_find_L_seq_single_query_simple_search_large_graph.cpp)
target_compile_options(profile_find_L_seq_single_query_simple_search_large_graph PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_seq_single_query_simple_search_large_graph PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX NSG)

add_executable(profile_para_single_query_search_simple_v3_large_graph_ranged app/profile_para_single_query_search_simple_v3_large_graph_ranged.cpp core/Searching.202009171601.simple_v3.large_graph.cpp)
target_compile_options(profile_para_single_query_search_simple_v3_large_graph_ranged PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_simple_v3_large_graph_ranged PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(check_true_nn tools/check_true_nn.cpp core/Searching.202009171601.simple_v3.large_graph.cpp)
target_compile_options(check_true_nn PRIVATE ${CXX_OPTION})
target_link_libraries(check_true_nn PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(bfs_test_nsg tools/bfs_test_nsg.cpp core/Searching.202009221831.bfs_test_nsg.cpp)
target_compile_options(bfs_test_nsg PRIVATE ${CXX_OPTION})
target_link_libraries(bfs_test_nsg PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(widget_get_sift100M tools/widget_get_sift100M.cpp)
target_compile_options(widget_get_sift100M PRIVATE ${CXX_OPTION})
target_link_libraries(widget_get_sift100M PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(test_nsg_index_for_bvecs tools/test_nsg_index_for_bvecs.cpp include/efanna2e/index_nsg.cpp)
#target_compile_options(test_nsg_index PRIVATE ${CXX_OPTION} -xHost)
target_compile_options(test_nsg_index_for_bvecs PRIVATE ${CXX_OPTION})
target_link_libraries(test_nsg_index_for_bvecs PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX NSG)

add_executable(convert_bvecs_to_fvecs tools/convert_bvecs_to_fvecs.cpp)
target_compile_options(convert_bvecs_to_fvecs PRIVATE ${CXX_OPTION})
target_link_libraries(convert_bvecs_to_fvecs PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_search_simple_v3_large_graph app/profile_find_L_para_single_query_search_simple_v3_large_graph.cpp core/Searching.202009171601.simple_v3.large_graph.cpp)
target_compile_options(profile_find_L_para_single_query_search_simple_v3_large_graph PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_search_simple_v3_large_graph PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_para_single_query_search_simple_v3_ranged app/profile_para_single_query_search_simple_v3_ranged.cpp core/Searching.202008310636.simple_v3.cpp)
target_compile_options(profile_para_single_query_search_simple_v3_ranged PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_simple_v3_ranged PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_search_simple_v3_one_merge app/profile_find_L_para_single_query_search_simple_v3_one_merge.cpp core/Searching.202011131441.simple_v3.one_merge.cpp)
target_compile_options(profile_find_L_para_single_query_search_simple_v3_one_merge PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_search_simple_v3_one_merge PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_search_simple_v3_large_graph_one_merge app/profile_find_L_para_single_query_search_simple_v3_large_graph_one_merge.cpp core/Searching.202011131454.simple_v3.large_graph.one_merge.cpp)
target_compile_options(profile_find_L_para_single_query_search_simple_v3_large_graph_one_merge PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_search_simple_v3_large_graph_one_merge PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(test_compare_groundtruth_and_true_nn tools/test_compare_groundtruth_and_true_nn.cpp)
target_compile_options(test_compare_groundtruth_and_true_nn PRIVATE ${CXX_OPTION})
target_link_libraries(test_compare_groundtruth_and_true_nn PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_top_m_search_pure app/profile_find_L_para_single_query_top_m_search_pure.cpp core/Searching.202011241533.top_m_pure.cpp)
target_compile_options(profile_find_L_para_single_query_top_m_search_pure PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_top_m_search_pure PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_top_m_search_pure_large_graph app/profile_find_L_para_single_query_top_m_search_pure_large_graph.cpp core/Searching.202011241615.top_m_pure_large_graph.cpp)
target_compile_options(profile_find_L_para_single_query_top_m_search_pure_large_graph PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_top_m_search_pure_large_graph PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_scale_m_search_pure app/profile_find_L_para_single_query_scale_m_search_pure.cpp core/Searching.202011241651.scale_m_pure.cpp)
target_compile_options(profile_find_L_para_single_query_scale_m_search_pure PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_scale_m_search_pure PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_scale_m_search_pure_large_graph app/profile_find_L_para_single_query_scale_m_search_pure_large_graph.cpp core/Searching.202011241958.scale_m_pure_large_graph.cpp)
target_compile_options(profile_find_L_para_single_query_scale_m_search_pure_large_graph PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_scale_m_search_pure_large_graph PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_top_m_search_pure_large_graph_num_unchecked app/profile_find_L_para_single_query_top_m_search_pure_large_graph_num_unchecked.cpp core/Searching.202011301508.top_m_pure_large_graph.num_unchecked.cpp)
target_compile_options(profile_find_L_para_single_query_top_m_search_pure_large_graph_num_unchecked PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_top_m_search_pure_large_graph_num_unchecked PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_seq_single_query_simple_search_large_graph_num_unchecked app/profile_find_L_seq_single_query_simple_search_large_graph_num_unchecked.cpp include/efanna2e/index.cpp include/efanna2e/index_nsg.202011301613.num_unchecked.cpp)
target_compile_options(profile_find_L_seq_single_query_simple_search_large_graph_num_unchecked PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_seq_single_query_simple_search_large_graph_num_unchecked PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_top_m_search_pure_large_graph_rank_0 app/profile_find_L_para_single_query_top_m_search_pure_large_graph_rank_0.cpp core/Searching.202011301508.top_m_pure_large_graph.num_unchecked.cpp)
target_compile_options(profile_find_L_para_single_query_top_m_search_pure_large_graph_rank_0 PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_top_m_search_pure_large_graph_rank_0 PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_seq_single_query_simple_search_large_graph_rank_0 app/profile_find_L_seq_single_query_simple_search_large_graph_rank_0.cpp include/efanna2e/index.cpp include/efanna2e/index_nsg.202011301613.num_unchecked.cpp)
target_compile_options(profile_find_L_seq_single_query_simple_search_large_graph_rank_0 PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_seq_single_query_simple_search_large_graph_rank_0 PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_top_m_search_pure_large_graph_last_update_step app/profile_find_L_para_single_query_top_m_search_pure_large_graph_last_update_step.cpp core/Searching.202012041559.top_m_pure_large_graph.last_update_step.cpp)
target_compile_options(profile_find_L_para_single_query_top_m_search_pure_large_graph_last_update_step PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_top_m_search_pure_large_graph_last_update_step PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_seq_single_query_simple_search_large_graph_last_update_step app/profile_find_L_seq_single_query_simple_search_large_graph_last_update_step.cpp include/efanna2e/index.cpp include/efanna2e/index_nsg.202012041718.last_update_step.cpp)
target_compile_options(profile_find_L_seq_single_query_simple_search_large_graph_last_update_step PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_seq_single_query_simple_search_large_graph_last_update_step PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_scale_m_search_pure_large_graph_num_unchecked app/profile_find_L_para_single_query_scale_m_search_pure_large_graph_num_unchecked.cpp core/Searching.202012071500.scale_m_pure_large_graph.num_unchecked.cpp)
target_compile_options(profile_find_L_para_single_query_scale_m_search_pure_large_graph_num_unchecked PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_scale_m_search_pure_large_graph_num_unchecked PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_scale_m_search_pure_large_graph_last_update_step app/profile_find_L_para_single_query_scale_m_search_pure_large_graph_last_update_step.cpp core/Searching.202012071500.scale_m_pure_large_graph.num_unchecked.cpp)
target_compile_options(profile_find_L_para_single_query_scale_m_search_pure_large_graph_last_update_step PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_scale_m_search_pure_large_graph_last_update_step PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_seq_single_query_simple_search_large_graph_backtracking app/profile_find_L_seq_single_query_simple_search_large_graph_backtracking.cpp include/efanna2e/index.cpp include/efanna2e/index_nsg.202012111616.backtracking.cpp)
target_compile_options(profile_find_L_seq_single_query_simple_search_large_graph_backtracking PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_seq_single_query_simple_search_large_graph_backtracking PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_top_m_search_pure_large_graph_backtracking app/profile_find_L_para_single_query_top_m_search_pure_large_graph_backtracking.cpp core/Searching.202012122112.top_m_pure_large_graph.backtracking.cpp)
target_compile_options(profile_find_L_para_single_query_top_m_search_pure_large_graph_backtracking PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_top_m_search_pure_large_graph_backtracking PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_para_single_query_search_PSS_v4_ranged app/profile_para_single_query_search_PSS_v4_ranged.cpp core/Searching.202012141643.simple_v3.scale_M.cpp)
target_compile_options(profile_para_single_query_search_PSS_v4_ranged PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_PSS_v4_ranged PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_para_single_query_search_PSS_v4_large_graph_ranged app/profile_para_single_query_search_PSS_v4_large_graph_ranged.cpp core/Searching.202012141857.simple_v3.large_graph.scale_M.cpp)
target_compile_options(profile_para_single_query_search_PSS_v4_large_graph_ranged PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_PSS_v4_large_graph_ranged PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_find_L_para_single_query_top_m_search_Ligra
        app/profile_find_L_para_single_query_top_m_search_Ligra.cpp
        core/Searching.202012191846.top_m_Ligra.cpp)
target_compile_options(profile_find_L_para_single_query_top_m_search_Ligra PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_top_m_search_Ligra PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_para_single_query_search_PSS_v4_ranged_profiling
        app/profile_para_single_query_search_PSS_v4_ranged_profiling.cpp
        core/Searching.202101081549.PSS_v4.profiling.cpp)
target_compile_options(profile_para_single_query_search_PSS_v4_ranged_profiling PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_PSS_v4_ranged_profiling PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_para_single_query_search_PSS_v4_large_graph_ranged_profiling
        app/profile_para_single_query_search_PSS_v4_large_graph_ranged_profiling.cpp
        core/Searching.202101081641.PSS_v4.large_graph.profiling.cpp)
target_compile_options(profile_para_single_query_search_PSS_v4_large_graph_ranged_profiling PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_PSS_v4_large_graph_ranged_profiling PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_find_L_para_single_query_top_m_search_pure_profiling
        app/profile_find_L_para_single_query_top_m_search_pure_profiling.cpp
        core/Searching.202101081737.top_m_pure.profiling.cpp)
target_compile_options(profile_find_L_para_single_query_top_m_search_pure_profiling PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_top_m_search_pure_profiling PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_find_L_para_single_query_top_m_search_pure_large_graph_profiling
        app/profile_find_L_para_single_query_top_m_search_pure_large_graph_profiling.cpp
        core/Searching.202101081753.top_m_pure_large_graph.profiling.cpp)
target_compile_options(profile_find_L_para_single_query_top_m_search_pure_large_graph_profiling PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_top_m_search_pure_large_graph_profiling PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_find_L_para_single_query_scale_m_search_pure_profiling
        app/profile_find_L_para_single_query_scale_m_search_pure_profiling.cpp
        core/Searching.202101081806.scale_m_pure.profiling.cpp)
target_compile_options(profile_find_L_para_single_query_scale_m_search_pure_profiling PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_scale_m_search_pure_profiling PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_find_L_para_single_query_scale_m_search_pure_large_graph_profiling
        app/profile_find_L_para_single_query_scale_m_search_pure_large_graph_profiling.cpp
        core/Searching.202101081821.scale_m_pure_large_graph.profiling.cpp)
target_compile_options(profile_find_L_para_single_query_scale_m_search_pure_large_graph_profiling PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_scale_m_search_pure_large_graph_profiling PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_find_L_para_single_query_NSG_omp
        app/profile_find_L_para_single_query_NSG_omp.cpp
        include/efanna2e/index.cpp include/efanna2e/index_nsg.202101172012.omp.cpp)
target_compile_options(
        profile_find_L_para_single_query_NSG_omp
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_find_L_para_single_query_NSG_omp
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_find_L_para_single_query_NSG_omp_large_graph
        app/profile_find_L_para_single_query_NSG_omp_large_graph.cpp
        include/efanna2e/index.cpp include/efanna2e/index_nsg.202101172012.omp.cpp)
target_compile_options(
        profile_find_L_para_single_query_NSG_omp_large_graph
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_find_L_para_single_query_NSG_omp_large_graph
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_find_L_para_single_query_NSG_omp_profile
        app/profile_find_L_para_single_query_NSG_omp_profile.cpp
        include/efanna2e/index.cpp include/efanna2e/index_nsg.202101181119.omp_profile.cpp)
target_compile_options(
        profile_find_L_para_single_query_NSG_omp_profile
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_find_L_para_single_query_NSG_omp_profile
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_find_L_para_single_query_NSG_omp_large_graph_profile
        app/profile_find_L_para_single_query_NSG_omp_large_graph_profile.cpp
        include/efanna2e/index.cpp include/efanna2e/index_nsg.202101181119.omp_profile.cpp)
target_compile_options(
        profile_find_L_para_single_query_NSG_omp_large_graph_profile
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_find_L_para_single_query_NSG_omp_large_graph_profile
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        hnsw_index
        app/hnsw_index.cpp)
target_link_libraries(
        hnsw_index
        PRIVATE OpenMP::OpenMP_CXX)

add_executable(
        hnsw_search
        app/hnsw_search.cpp)
target_link_libraries(
        hnsw_search
        PRIVATE OpenMP::OpenMP_CXX)

add_executable(
        profile_find_L_para_single_query_search_PSS_v4_profile
        app/profile_find_L_para_single_query_search_PSS_v4_profile.cpp
        core/Searching.202101081549.PSS_v4.profiling.cpp)
target_compile_options(
        profile_find_L_para_single_query_search_PSS_v4_profile
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_find_L_para_single_query_search_PSS_v4_profile
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_find_L_para_single_query_search_PSS_v4_large_graph_profile
        app/profile_find_L_para_single_query_search_PSS_v4_large_graph_profile.cpp
        core/Searching.202101081641.PSS_v4.large_graph.profiling.cpp)
target_compile_options(
        profile_find_L_para_single_query_search_PSS_v4_large_graph_profile
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_find_L_para_single_query_search_PSS_v4_large_graph_profile
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        widget_transpose
        tools/widget_transpose.cpp)