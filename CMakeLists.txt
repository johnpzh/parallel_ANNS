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
#find_package(Boost REQUIRED COMPONENTS thread)
find_package(Boost REQUIRED)

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
IF(DEFINED DEBUG_PRINT)
        string(TOLOWER ${DEBUG_PRINT} DEBUG_PRINT_lower)
        IF(NOT ${DEBUG_PRINT_lower} MATCHES "off")
            add_compile_definitions(DEBUG_PRINT)
        ENDIF()
ENDIF()

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

#add_executable(profile_seq_simple_search_two_queues app/profile_seq_simple_search_two_queues.cpp)
#if(FOUND_GCC930 AND FOUND_CPP6)
##    message(KNL!!!)
#    ## For KNL
#    target_compile_options(profile_seq_simple_search_two_queues PRIVATE ${CXX_OPTION} -gxx-name=${FOUND_GCC930})
#    target_link_libraries(profile_seq_simple_search_two_queues PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX ${FOUND_CPP6})
#else()
#    target_compile_options(profile_seq_simple_search_two_queues PRIVATE ${CXX_OPTION})
#    target_link_libraries(profile_seq_simple_search_two_queues PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#endif()
#
#add_executable(profile_para_simple_search_two_global_queues app/profile_para_simple_search_two_global_queues.cpp)
#if(FOUND_GCC930 AND FOUND_CPP6)
##    message(KNL!!!)
#    ## For KNL
#    target_compile_options(profile_para_simple_search_two_global_queues PRIVATE ${CXX_OPTION} -gxx-name=${FOUND_GCC930})
#    target_link_libraries(profile_para_simple_search_two_global_queues PRIVATE ${Boost_LIBRARIES} ${FOUND_CPP6})
##    target_link_libraries(profile_para_simple_search_two_global_queues PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX ${FOUND_CPP6})
#else()
#    target_compile_options(profile_para_simple_search_two_global_queues PRIVATE ${CXX_OPTION})
#    target_link_libraries(profile_para_simple_search_two_global_queues PRIVATE ${Boost_LIBRARIES})
##    target_link_libraries(profile_para_simple_search_two_global_queues PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#endif()
#
#
#add_executable(profile_para_simple_search_two_global_queues.workbuffer app/profile_para_simple_search_two_global_queues.workbuffer.cpp)
#if(FOUND_GCC930 AND FOUND_CPP6)
#    #    message(KNL!!!)
#    ## For KNL
#    target_compile_options(profile_para_simple_search_two_global_queues.workbuffer PRIVATE ${CXX_OPTION} -gxx-name=${FOUND_GCC930})
#    target_link_libraries(profile_para_simple_search_two_global_queues.workbuffer PRIVATE ${Boost_LIBRARIES} ${FOUND_CPP6})
#    #    target_link_libraries(profile_para_simple_search_two_global_queues PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX ${FOUND_CPP6})
#else()
#    target_compile_options(profile_para_simple_search_two_global_queues.workbuffer PRIVATE ${CXX_OPTION})
#    target_link_libraries(profile_para_simple_search_two_global_queues.workbuffer PRIVATE ${Boost_LIBRARIES})
#    #    target_link_libraries(profile_para_simple_search_two_global_queues PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#endif()

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

add_executable(test_nsg_index_findroot tools/test_nsg_index_findroot.cpp include/efanna2e/index_nsg.202104211705.findroot.cpp)
#target_compile_options(test_nsg_index PRIVATE ${CXX_OPTION} -xHost)
target_compile_options(test_nsg_index_findroot PRIVATE ${CXX_OPTION})
target_link_libraries(test_nsg_index_findroot PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX NSG)

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

add_executable(
        widget_get_fake_reorder_map
        tools/widget_get_fake_reorder_map.cpp)

add_executable(
        profile_para_single_query_search_PSS_v5_ranged_profiling
        app/profile_para_single_query_search_PSS_v5_ranged_profiling.cpp
        core/Searching.202101301609.PSS_v5.profiling.cpp)
target_compile_options(
        profile_para_single_query_search_PSS_v5_ranged_profiling
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_para_single_query_search_PSS_v5_ranged_profiling
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_find_L_para_single_query_search_PSS_v5_profile
        app/profile_find_L_para_single_query_search_PSS_v5_profile.cpp
        core/Searching.202101301609.PSS_v5.profiling.cpp)
target_compile_options(
        profile_find_L_para_single_query_search_PSS_v5_profile
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_find_L_para_single_query_search_PSS_v5_profile
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_find_L_para_single_query_search_PSS_v5_large_graph_profile
        app/profile_find_L_para_single_query_search_PSS_v5_large_graph_profile.cpp
        core/Searching.202101301614.PSS_v5.large_graph.profiling.cpp)
target_compile_options(
        profile_find_L_para_single_query_search_PSS_v5_large_graph_profile
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_find_L_para_single_query_search_PSS_v5_large_graph_profile
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_para_single_query_search_PSS_v6_ranged_profiling
        app/profile_para_single_query_search_PSS_v6_ranged_profiling.cpp
        core/Searching.202101311717.PSS_v5.global_queue.profiling.cpp)
target_compile_options(
        profile_para_single_query_search_PSS_v6_ranged_profiling
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_para_single_query_search_PSS_v6_ranged_profiling
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_para_single_query_search_PSS_v5_large_graph_dist_thresh_ranged_profiling
        app/profile_para_single_query_search_PSS_v5_large_graph_dist_thresh_ranged_profiling.cpp
        core/Searching.202102031939.PSS_v5.large_graph.dist_thresh.profiling.cpp)
target_compile_options(
        profile_para_single_query_search_PSS_v5_large_graph_dist_thresh_ranged_profiling
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_para_single_query_search_PSS_v5_large_graph_dist_thresh_ranged_profiling
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

#### Original
#add_executable(
#        profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling
#        app/profile_para_single_query_search_PSS_v4_ranged_profiling.cpp
#        core/Searching.202101081549.PSS_v4.profiling.cpp)
##        app/profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling.cpp
##        core/Searching.202102022027.PSS_v5.dist_thresh.profiling.cpp)
#target_compile_options(
#        profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling
#        PRIVATE ${CXX_OPTION})
#target_link_libraries(
#        profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling
#        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_para_single_query_search_PSS_v4_ranged_profiling
        app/profile_para_single_query_search_PSS_v4_ranged_profiling.cpp
        core/Searching.202101081549.PSS_v4.profiling.cpp)
target_compile_options(
        profile_para_single_query_search_PSS_v4_ranged_profiling
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_para_single_query_search_PSS_v4_ranged_profiling
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

#add_executable(
#        atest
#        app/atest.cpp
##        core/Searching.202102022027.PSS_v5.dist_thresh.profiling.cpp)
#        core/Searching.202101081549.PSS_v4.profiling.cpp)
#target_compile_options(
#        atest
#        PRIVATE ${CXX_OPTION})
#target_link_libraries(
#        atest
#        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)


####### Some crazy stuff ########
add_executable(
        profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profilinga
        app/profile_para_single_query_search_PSS_v4_ranged_profiling.cpp
        core/Searching.202101081549.PSS_v4.profiling.cpp)
target_compile_options(
        profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profilinga
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profilinga
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
add_executable(
        profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling
        app/profile_para_single_query_search_PSS_v4_ranged_profiling.cpp
        core/Searching.202101081549.PSS_v4.profiling.cpp)
target_compile_options(
        profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
add_executable(
        abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefgh
        app/profile_para_single_query_search_PSS_v4_ranged_profiling.cpp
        core/Searching.202101081549.PSS_v4.profiling.cpp)
target_compile_options(
        abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefgh
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefgh
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
add_executable(
        abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefg
        app/profile_para_single_query_search_PSS_v4_ranged_profiling.cpp
        core/Searching.202101081549.PSS_v4.profiling.cpp)
target_compile_options(
        abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefg
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefg
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
add_executable(
        profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profilin
        app/profile_para_single_query_search_PSS_v4_ranged_profiling.cpp
        core/Searching.202101081549.PSS_v4.profiling.cpp)
target_compile_options(
        profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profilin
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profilin
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
####### End ########

add_executable(
        PSS_v5_distance_threshold_profiling
        app/PSS_v5_distance_threshold_profiling.cpp
        core/Searching.202102022027.PSS_v5.dist_thresh.profiling.cpp)
target_compile_options(
        PSS_v5_distance_threshold_profiling
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_distance_threshold_profiling
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_LG_distance_threshold_profiling
        app/PSS_v5_LG_distance_threshold_profiling.cpp
        core/Searching.202102031939.PSS_v5.large_graph.dist_thresh.profiling.cpp)
target_compile_options(
        PSS_v5_LG_distance_threshold_profiling
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_LG_distance_threshold_profiling
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_LG_v_counter
        app/PSS_v5_LG_v_counter.cpp
        core/Searching.202103101606.PSS_v5.large_graph.v_counter.cpp)
target_compile_options(
        PSS_v5_LG_v_counter
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_LG_v_counter
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_LG_count_reorder
        app/PSS_v5_LG_count_reorder.cpp
        core/Searching.202103101852.PSS_v5.large_graph.count_reorder.cpp)
target_compile_options(
        PSS_v5_LG_count_reorder
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_LG_count_reorder
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        widget_neighbors_distance_distribution
        tools/widget_neighbors_distance_distribution.cpp
        core/Searching.202103101852.PSS_v5.large_graph.count_reorder.cpp)
target_compile_options(
        widget_neighbors_distance_distribution
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        widget_neighbors_distance_distribution
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

#add_executable(
#        NSG_neighbor_distance_step
#        app/NSG_neighbor_distance_step.cpp
#        include/efanna2e/index.cpp
#        include/efanna2e/index_nsg.202103121531.neighbor_distance_step.cpp)
#target_compile_options(
#        NSG_neighbor_distance_step
#        PRIVATE ${CXX_OPTION})
#target_link_libraries(
#        NSG_neighbor_distance_step
#        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        NSG_candidate_distance_step
        app/NSG_candidate_distance_step.cpp
        include/efanna2e/index.cpp
        include/efanna2e/index_nsg.202103121531.neighbor_distance_step.cpp)
target_compile_options(
        NSG_candidate_distance_step
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        NSG_candidate_distance_step
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_LG_distance_step
        app/PSS_v5_LG_distance_step.cpp
        core/Searching.202103141957.PSS_v5.large_graph.distance_step.cpp)
target_compile_options(
        PSS_v5_LG_distance_step
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_LG_distance_step
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_LG_degree_reorder
        app/PSS_v5_LG_degree_reorder.cpp
        core/Searching.202103152056.PSS_v5.large_graph.degree_reorder.cpp)
target_compile_options(
        PSS_v5_LG_degree_reorder
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_LG_degree_reorder
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_LG_in_degree
        app/PSS_v5_LG_in_degree.cpp
        core/Searching.202103101606.PSS_v5.large_graph.v_counter.cpp)
target_compile_options(
        PSS_v5_LG_in_degree
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_LG_in_degree
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_sequence_traverse
        app/PSS_v5_sequence_traverse.cpp
        core/Searching.202103191537.PSS_v5.sequence_traverse.cpp)
target_compile_options(
        PSS_v5_sequence_traverse
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_sequence_traverse
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_sequence_traverse_B
        app/PSS_v5_sequence_traverse_B.cpp
        core/Searching.202103191800.PSS_v5.sequence_traverse_B.cpp)
target_compile_options(
        PSS_v5_sequence_traverse_B
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_sequence_traverse_B
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_distance_threshold_profiling_no_search
        app/PSS_v5_distance_threshold_profiling_no_search.cpp
        core/Searching.202102022027.PSS_v5.dist_thresh.profiling.cpp)
target_compile_options(
        PSS_v5_distance_threshold_profiling_no_search
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_distance_threshold_profiling_no_search
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_LG_distance_threshold_profiling_no_search
        app/PSS_v5_LG_distance_threshold_profiling_no_search.cpp
        core/Searching.202102031939.PSS_v5.large_graph.dist_thresh.profiling.cpp)
target_compile_options(
        PSS_v5_LG_distance_threshold_profiling_no_search
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_LG_distance_threshold_profiling_no_search
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#IF(DEFINED DEBUG_PRINT)
#    target_compile_definitions(
#            PSS_v5_LG_distance_threshold_profiling_no_search
#            PRIVATE DEBUG_PRINT)
#ENDIF()

add_executable(
        PSS_v5_LG_degree_reorder_no_search
        app/PSS_v5_LG_degree_reorder_no_search.cpp
        core/Searching.202103152056.PSS_v5.large_graph.degree_reorder.cpp)
target_compile_options(
        PSS_v5_LG_degree_reorder_no_search
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_LG_degree_reorder_no_search
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_LG_percentile
        app/PSS_v5_LG_percentile.cpp
        core/Searching.202102031939.PSS_v5.large_graph.dist_thresh.profiling.cpp)
target_compile_options(
        PSS_v5_LG_percentile
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_LG_percentile
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(NSG_seq_LG_percentile
        app/NSG_seq_LG_percentile.cpp
        include/efanna2e/index.cpp
        include/efanna2e/index_nsg.cpp)
target_compile_options(NSG_seq_LG_percentile PRIVATE ${CXX_OPTION})
target_link_libraries(NSG_seq_LG_percentile PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_LG_nn_distribution
        app/PSS_v5_LG_nn_distribution.cpp
        core/Searching.202105281923.PSS_v5_LG_nn_distribution.cpp)
target_compile_options(
        PSS_v5_LG_nn_distribution
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_LG_nn_distribution
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

#add_executable(
#        PSS_v5_LG_core_subgraph
#        app/PSS_v5_LG_core_subgraph.cpp
#        core/Searching.202105302215.PSS_v5_LG_core_subgraph.cpp)
#target_compile_options(
#        PSS_v5_LG_core_subgraph
#        PRIVATE ${CXX_OPTION})
#target_link_libraries(
#        PSS_v5_LG_core_subgraph
#        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_LG_bfs_nn_subgraph
        app/PSS_LG_bfs_nn_subgraph.cpp
        core/Searching.202106051704.PSS_LG_bfs_subgraph.cpp)
target_compile_options(
        PSS_LG_bfs_nn_subgraph
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_LG_bfs_nn_subgraph
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_LG_monotonic_bfs_subgraph
        app/PSS_LG_monotonic_bfs_subgraph.cpp
        core/Searching.202106051704.PSS_LG_bfs_subgraph.cpp)
target_compile_options(
        PSS_LG_monotonic_bfs_subgraph
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_LG_monotonic_bfs_subgraph
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_LG_all_monotonic_bfs_subgraph
        app/PSS_LG_all_monotonic_bfs_subgraph.cpp
        core/Searching.202106051704.PSS_LG_bfs_subgraph.cpp)
target_compile_options(
        PSS_LG_all_monotonic_bfs_subgraph
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_LG_all_monotonic_bfs_subgraph
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_LG_connected_to_nn_all_monotonic_bfs_subgraph
        app/PSS_LG_connected_to_nn_all_monotonic_bfs_subgraph.cpp
        core/Searching.202106051704.PSS_LG_bfs_subgraph.cpp)
target_compile_options(
        PSS_LG_connected_to_nn_all_monotonic_bfs_subgraph
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_LG_connected_to_nn_all_monotonic_bfs_subgraph
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_LG_parallel_search_path_in_subgraph
        app/PSS_LG_parallel_search_path_in_subgraph.cpp
        core/Searching.202106170130.PSS_LG_bfs_parallel_path_subgraph.cpp)
target_compile_options(
        PSS_LG_parallel_search_path_in_subgraph
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_LG_parallel_search_path_in_subgraph
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_LG_parallel_search_path_in_subgraph_core_only
        app/PSS_LG_parallel_search_path_in_subgraph_core_only.cpp
        core/Searching.202106170130.PSS_LG_bfs_parallel_path_subgraph.cpp)
target_compile_options(
        PSS_LG_parallel_search_path_in_subgraph_core_only
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_LG_parallel_search_path_in_subgraph_core_only
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_LG_parallel_search_path_in_subgraph_w_nonmono
        app/PSS_LG_parallel_search_path_in_subgraph_w_nonmono.cpp
        core/Searching.202106251238.PSS_LG_bfs_parallel_path_w_nonmono_subgraph.cpp)
target_compile_options(
        PSS_LG_parallel_search_path_in_subgraph_w_nonmono
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_LG_parallel_search_path_in_subgraph_w_nonmono
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_LG_in_6_steps_connected_to_nn_all_monotonic_bfs_subgraph
        app/PSS_LG_in_6_steps_connected_to_nn_all_monotonic_bfs_subgraph.cpp
        core/Searching.202106051704.PSS_LG_bfs_subgraph.cpp)
target_compile_options(
        PSS_LG_in_6_steps_connected_to_nn_all_monotonic_bfs_subgraph
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_LG_in_6_steps_connected_to_nn_all_monotonic_bfs_subgraph
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_LG_parallel_search_path_2_steps_dist_prune
        app/PSS_LG_parallel_search_path_2_steps_dist_prune.cpp
        core/Searching.202106261435.PSS_LG_bfs_parallel_path_2_steps_dist_prune.cpp)
target_compile_options(
        PSS_LG_parallel_search_path_2_steps_dist_prune
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_LG_parallel_search_path_2_steps_dist_prune
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_LG_nn_distribution_2_steps_dist_prune
        app/PSS_v5_LG_nn_distribution_2_steps_dist_prune.cpp
        core/Searching.202106261539.PSS_v5_LG_nn_distribution_2_steps_dist_prune.cpp)
target_compile_options(
        PSS_v5_LG_nn_distribution_2_steps_dist_prune
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_LG_nn_distribution_2_steps_dist_prune
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v6_radius_bound
        app/PSS_v6_radius_bound.cpp
        core/Searching.202106271444.PSS_v6_radius_bound.cpp)
target_compile_options(
        PSS_v6_radius_bound
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v6_radius_bound
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v6_LG_radius_bound
        app/PSS_v6_LG_radius_bound.cpp
        core/Searching.202106271447.PSS_v6_LG_radius_bound.cpp)
target_compile_options(
        PSS_v6_LG_radius_bound
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v6_LG_radius_bound
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_LG_bfs_path_from_17_vertices_w_radius_bound
        app/PSS_LG_bfs_path_from_17_vertices_w_radius_bound.cpp
        core/Searching.202106271833.PSS_LG_bfs_path_from_17_vertices.cpp)
target_compile_options(
        PSS_LG_bfs_path_from_17_vertices_w_radius_bound
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_LG_bfs_path_from_17_vertices_w_radius_bound
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_LG_mini_cut_bfs_subgraph
        app/PSS_LG_mini_cut_bfs_subgraph.cpp
        core/Searching.202106051704.PSS_LG_bfs_subgraph.cpp)
target_compile_options(
        PSS_LG_mini_cut_bfs_subgraph
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_LG_mini_cut_bfs_subgraph
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_multi_queries
        app/PSS_v5_multi_queries.cpp
        core/Searching.202107291833.PSS_v5.multi_queries.cpp)
target_compile_options(
        PSS_v5_multi_queries
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_multi_queries
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_multi_queries_LG
        app/PSS_v5_multi_queries_LG.cpp
        core/Searching.202107292234.PSS_v5.multi_queries.LG.cpp)
target_compile_options(
        PSS_v5_multi_queries_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_multi_queries_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_multi_queries_load_balance
        app/PSS_v5_multi_queries_load_balance.cpp
        core/Searching.202107291833.PSS_v5.multi_queries.cpp)
target_compile_options(
        PSS_v5_multi_queries_load_balance
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_multi_queries_load_balance
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_multi_queries_load_balance_LG
        app/PSS_v5_multi_queries_load_balance_LG.cpp
        core/Searching.202107292234.PSS_v5.multi_queries.LG.cpp)
target_compile_options(
        PSS_v5_multi_queries_load_balance_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_multi_queries_load_balance_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_multi_queries_tuning_intra_LG
        app/PSS_v5_multi_queries_tuning_intra_LG.cpp
        core/Searching.202107292234.PSS_v5.multi_queries.LG.cpp)
target_compile_options(
        PSS_v5_multi_queries_tuning_intra_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_multi_queries_tuning_intra_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        app_para_searching
        app/app_para_searching.cpp)
target_compile_options(
        app_para_searching
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        app_para_searching
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        app_para_searching_LG
        app/app_para_searching_LG.cpp)
target_compile_options(
        app_para_searching_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        app_para_searching_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_LG_K_values
        app/PSS_v5_LG_K_values.cpp
        core/Searching.202108160800.PSS_v5_K_values_LG.cpp)
target_compile_options(
        PSS_v5_LG_K_values
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_LG_K_values
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(NSG_seq_LG_K_values
        app/NSG_seq_LG_K_values.cpp
        include/efanna2e/index.cpp
        include/efanna2e/index_nsg.cpp)
target_compile_options(NSG_seq_LG_K_values PRIVATE ${CXX_OPTION})
target_link_libraries(NSG_seq_LG_K_values PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_LG_separate_search
        app/PSS_v5_LG_separate_search.cpp
        core/Searching.202108171220.PSS_v5.separate_search.LG.cpp)
target_compile_options(
        PSS_v5_LG_separate_search
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_LG_separate_search
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_find_L_every_step_merge
        app/PSS_v5_find_L_every_step_merge.cpp
        core/Searching.202102022027.PSS_v5.dist_thresh.profiling.cpp)
target_compile_options(
        PSS_v5_find_L_every_step_merge
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_find_L_every_step_merge
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_find_L_every_step_merge_LG
        app/PSS_v5_find_L_every_step_merge_LG.cpp
        core/Searching.202102031939.PSS_v5.large_graph.dist_thresh.profiling.cpp)
target_compile_options(
        PSS_v5_find_L_every_step_merge_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_find_L_every_step_merge_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_find_L_every_step_merge_L_master_LG
        app/PSS_v5_find_L_every_step_merge_L_master_LG.cpp
        core/Searching.202110151700.PSS_v5.LG.every_step_merge.cpp)
target_compile_options(
        PSS_v5_find_L_every_step_merge_L_master_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_find_L_every_step_merge_L_master_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_worker_variation_LG
        app/PSS_v5_worker_variation_LG.cpp
        core/Searching.202111050014.PSS_v5.worker_variation.LG.cpp
        include/utils.cpp)
target_compile_options(
        PSS_v5_worker_variation_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_worker_variation_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_find_L_index_adaptive_merge_LG
        app/PSS_v5_find_L_index_adaptive_merge_LG.cpp
        core/Searching.202111110022.PSS_v5.index_adaptive_merge.LG.cpp
        include/utils.cpp)
target_compile_options(
        PSS_v5_find_L_index_adaptive_merge_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_find_L_index_adaptive_merge_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_find_L_index_adaptive_merge_L_master_LG
        app/PSS_v5_find_L_index_adaptive_merge_L_master_LG.cpp
        core/Searching.202111152031.PSS_v5.index_adaptive_merge_L_master.LG.cpp
        include/utils.cpp)
target_compile_options(
        PSS_v5_find_L_index_adaptive_merge_L_master_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_find_L_index_adaptive_merge_L_master_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_worker_std_deviation_LG
        app/PSS_v5_worker_std_deviation_LG.cpp
        core/Searching.202111171644.PSS_v5.worker_std_deviation.LG.cpp
        include/utils.cpp)
target_compile_options(
        PSS_v5_worker_std_deviation_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_worker_std_deviation_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_find_L_every_step_merge_fixed_local_L_LG
        app/PSS_v5_find_L_every_step_merge_fixed_local_L_LG.cpp
        core/Searching.202110151700.PSS_v5.LG.every_step_merge.cpp)
target_compile_options(
        PSS_v5_find_L_every_step_merge_fixed_local_L_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_find_L_every_step_merge_fixed_local_L_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_find_L_fixed_local_L_update_positions_LG
        app/PSS_v5_find_L_fixed_local_L_update_positions_LG.cpp
        core/Searching.202111210046.PSS_v5.LG.update_positions.cpp
        include/utils.cpp)
target_compile_options(
        PSS_v5_find_L_fixed_local_L_update_positions_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_find_L_fixed_local_L_update_positions_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_find_L_every_step_merge_LG_for_ratio_1st_half
        app/PSS_v5_find_L_every_step_merge_LG_for_ratio_1st_half.cpp
        core/Searching.202111251430.PSS_v5.LG.TQueue.ratio_1st_half.cpp
        include/utils.cpp)
target_compile_options(
        PSS_v5_find_L_every_step_merge_LG_for_ratio_1st_half
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_find_L_every_step_merge_LG_for_ratio_1st_half
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_find_L_no_staged_LG
        app/PSS_v5_find_L_no_staged_LG.cpp
        core/Searching.202111301625.PSS_v5.LG.no_staged.cpp)
target_compile_options(
        PSS_v5_find_L_no_staged_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_find_L_no_staged_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        NSG_omp_profile_multi_queues
        app/NSG_omp_profile_multi_queues.cpp
        include/efanna2e/index.cpp include/efanna2e/index_nsg.202204201951.omp_profile_multi_queues.cpp)
target_compile_options(
        NSG_omp_profile_multi_queues
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        NSG_omp_profile_multi_queues
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        NSG_omp_profile_multi_queues_LG
        app/NSG_omp_profile_multi_queues_LG.cpp
        include/efanna2e/index.cpp include/efanna2e/index_nsg.202204201951.omp_profile_multi_queues.cpp)
target_compile_options(
        NSG_omp_profile_multi_queues_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        NSG_omp_profile_multi_queues_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_find_L_path_wise_every_step_merge_LG
        app/PSS_v5_find_L_path_wise_every_step_merge_LG.cpp
        core/Searching.202204210403.PSS_v5.large_graph.path_wise_every_step_merge.cpp)
target_compile_options(
        PSS_v5_find_L_path_wise_every_step_merge_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_find_L_path_wise_every_step_merge_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

# PCM LIB
find_package(Threads)
set(PCM_SOURCES
        include/PCM/src/cpucounters.cpp
        include/PCM/src/utils.cpp
        include/PCM/src/msr.cpp
        include/PCM/src/pci.cpp
        include/PCM/src/mmio.cpp
        include/PCM/src/bw.cpp
        include/PCM/src/topology.cpp
        include/PCM/src/debug.cpp
        include/PCM/src/threadpool.cpp
        include/PCM/src/dashboard.cpp
        include/PCM/src/resctrl.cpp)

add_library(LIB_PCM_MEM
        include/pcm_mem.cpp
        ${PCM_SOURCES})
target_link_libraries(LIB_PCM_MEM PRIVATE Threads::Threads)

# PCM Demo
add_executable(pcm_demo
        app/pcm_demo.cpp)
target_link_libraries(pcm_demo PRIVATE
        LIB_PCM_MEM)

# PCM NSG
add_executable(pcm_mem_NSG_find_L_LG
        app/pcm_mem_NSG_find_L_LG.cpp)
target_compile_options(pcm_mem_NSG_find_L_LG PRIVATE
        ${CXX_OPTION})
target_link_libraries(pcm_mem_NSG_find_L_LG PRIVATE
        ${Boost_LIBRARIES} OpenMP::OpenMP_CXX NSG LIB_PCM_MEM)

# PCM NSG Inter-Query
add_executable(pcm_mem_NSG_inter_query_LG
        app/pcm_mem_NSG_inter_query_LG.cpp)
target_compile_options(pcm_mem_NSG_inter_query_LG PRIVATE
        ${CXX_OPTION})
target_link_libraries(pcm_mem_NSG_inter_query_LG PRIVATE
        ${Boost_LIBRARIES} OpenMP::OpenMP_CXX NSG LIB_PCM_MEM)

#PCM NSG Edge-Wise Parallel
add_executable(
        pcm_mem_NSG_omp_find_LG
        app/pcm_mem_NSG_omp_find_LG.cpp
        include/efanna2e/index.cpp include/efanna2e/index_nsg.202101172012.omp.cpp)
target_compile_options(
        pcm_mem_NSG_omp_find_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        pcm_mem_NSG_omp_find_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX LIB_PCM_MEM)

#PCM PSS_v5_index_adaptive_merge (iQAN)
add_executable(
        pcm_mem_PSS_v5_find_L_index_adaptive_merge_L_master_LG
        app/pcm_mem_PSS_v5_find_L_index_adaptive_merge_L_master_LG.cpp
        core/Searching.202111152031.PSS_v5.index_adaptive_merge_L_master.LG.cpp
        include/utils.cpp)
target_compile_options(
        pcm_mem_PSS_v5_find_L_index_adaptive_merge_L_master_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        pcm_mem_PSS_v5_find_L_index_adaptive_merge_L_master_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX LIB_PCM_MEM)

#PCM PSS_v5 (Exhaust)
add_executable(
        pcm_mem_PSS_v5_LG
        app/pcm_mem_PSS_v5_LG.cpp
        core/Searching.202102031939.PSS_v5.large_graph.dist_thresh.profiling.cpp)
target_compile_options(
        pcm_mem_PSS_v5_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        pcm_mem_PSS_v5_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX LIB_PCM_MEM)

#PCM PathWise (every step merge)
add_executable(
        pcm_mem_PathWise_every_step_merge_LG
        app/pcm_mem_PathWise_every_step_merge_LG.cpp
        core/Searching.202204210403.PSS_v5.large_graph.path_wise_every_step_merge.cpp)
target_compile_options(
        pcm_mem_PathWise_every_step_merge_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        pcm_mem_PathWise_every_step_merge_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX LIB_PCM_MEM)

#PCM NoStaged
add_executable(
        pcm_mem_NoStaged_LG
        app/pcm_mem_NoStaged_LG.cpp
        core/Searching.202111301625.PSS_v5.LG.no_staged.cpp)
target_compile_options(
        pcm_mem_NoStaged_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        pcm_mem_NoStaged_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX LIB_PCM_MEM)

#PCM NoSync
add_executable(
        pcm_mem_NoSync_merge_once_LG
        app/pcm_mem_NoSync_merge_once_LG.cpp
        core/Searching.202110151700.PSS_v5.LG.every_step_merge.cpp)
target_compile_options(
        pcm_mem_NoSync_merge_once_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        pcm_mem_NoSync_merge_once_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX LIB_PCM_MEM)