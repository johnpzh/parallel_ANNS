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

#add_executable(profile_para_single_query_search_simple_v3_ranged app/profile_para_single_query_search_simple_v3_ranged.cpp core/Searching.202008310636.simple_v3.cpp)
#target_compile_options(profile_para_single_query_search_simple_v3_ranged PRIVATE ${CXX_OPTION})
#target_link_libraries(profile_para_single_query_search_simple_v3_ranged PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

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

add_executable(profile_para_single_query_search_simple_v3_large_graph_ranged_with_metric_mean app/profile_para_single_query_search_simple_v3_large_graph_ranged_with_metric_mean.cpp core/Searching.202010212125.simple_v3.metric_mean.cpp)
target_compile_options(profile_para_single_query_search_simple_v3_large_graph_ranged_with_metric_mean PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_simple_v3_large_graph_ranged_with_metric_mean PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

#add_executable(profile_para_single_query_search_simple_v3_ranged_heuristic_X app/profile_para_single_query_search_simple_v3_ranged_heuristic_X.cpp core/Searching.202010231417.simple_v3.heuristic_X.cpp)
#target_compile_options(profile_para_single_query_search_simple_v3_ranged_heuristic_X PRIVATE ${CXX_OPTION})
#target_link_libraries(profile_para_single_query_search_simple_v3_ranged_heuristic_X PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

#add_executable(profile_find_L_para_single_query_search_simple_v3_large_graph_increase_X app/profile_find_L_para_single_query_search_simple_v3_large_graph_increase_X.cpp core/Searching.202010231647.simple_v3.large_graph.heuristic_X.cpp)
#target_compile_options(profile_find_L_para_single_query_search_simple_v3_large_graph_increase_X PRIVATE ${CXX_OPTION})
#target_link_libraries(profile_find_L_para_single_query_search_simple_v3_large_graph_increase_X PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

#add_executable(profile_find_L_para_single_query_search_simple_v3_large_graph_decrease_X app/profile_find_L_para_single_query_search_simple_v3_large_graph_decrease_X.cpp core/Searching.202010231647.simple_v3.large_graph.heuristic_X.cpp)
#target_compile_options(profile_find_L_para_single_query_search_simple_v3_large_graph_decrease_X PRIVATE ${CXX_OPTION})
#target_link_libraries(profile_find_L_para_single_query_search_simple_v3_large_graph_decrease_X PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

#add_executable(profile_find_L_para_single_query_search_simple_v3_increase_X app/profile_find_L_para_single_query_search_simple_v3_increase_X.cpp core/Searching.202010231417.simple_v3.heuristic_X.cpp)
#target_compile_options(profile_find_L_para_single_query_search_simple_v3_increase_X PRIVATE ${CXX_OPTION})
#target_link_libraries(profile_find_L_para_single_query_search_simple_v3_increase_X PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#
#add_executable(profile_find_L_para_single_query_search_simple_v3_decrease_X app/profile_find_L_para_single_query_search_simple_v3_decrease_X.cpp core/Searching.202010231417.simple_v3.heuristic_X.cpp)
#target_compile_options(profile_find_L_para_single_query_search_simple_v3_decrease_X PRIVATE ${CXX_OPTION})
#target_link_libraries(profile_find_L_para_single_query_search_simple_v3_decrease_X PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#
#add_executable(profile_find_L_para_single_query_search_simple_v3_double_X app/profile_find_L_para_single_query_search_simple_v3_double_X.cpp core/Searching.202010231417.simple_v3.heuristic_X.cpp)
#target_compile_options(profile_find_L_para_single_query_search_simple_v3_double_X PRIVATE ${CXX_OPTION})
#target_link_libraries(profile_find_L_para_single_query_search_simple_v3_double_X PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#
#add_executable(profile_find_L_para_single_query_search_simple_v3_half_X app/profile_find_L_para_single_query_search_simple_v3_half_X.cpp core/Searching.202010231417.simple_v3.heuristic_X.cpp)
#target_compile_options(profile_find_L_para_single_query_search_simple_v3_half_X PRIVATE ${CXX_OPTION})
#target_link_libraries(profile_find_L_para_single_query_search_simple_v3_half_X PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_para_single_query_search_simple_v3_large_graph_ranged_num_checked app/profile_para_single_query_search_simple_v3_large_graph_ranged_num_checked.cpp core/Searching.202010301220.simple_v3.large_graph.num_checked.cpp)
target_compile_options(profile_para_single_query_search_simple_v3_large_graph_ranged_num_checked PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_simple_v3_large_graph_ranged_num_checked PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

############# num_checked
add_executable(profile_para_single_query_search_simple_v3_ranged_num_checked app/profile_para_single_query_search_simple_v3_ranged_num_checked.cpp core/Searching.202010291522.simple_v3.num_checked.cpp)
target_compile_options(profile_para_single_query_search_simple_v3_ranged_num_checked PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_simple_v3_ranged_num_checked PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#add_executable(profile_para_single_query_search_simple_v3_ranged_num_checked app/profile_para_single_query_search_simple_v3_ranged.cpp core/Searching.202008310636.simple_v3.cpp)
#target_compile_options(profile_para_single_query_search_simple_v3_ranged_num_checked PRIVATE ${CXX_OPTION})
#target_link_libraries(profile_para_single_query_search_simple_v3_ranged_num_checked PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

############## ranged
add_executable(profile_para_single_query_search_simple_v3_ranged app/profile_para_single_query_search_simple_v3_ranged.cpp core/Searching.202008310636.simple_v3.cpp)
target_compile_options(profile_para_single_query_search_simple_v3_ranged PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_simple_v3_ranged PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#add_executable(profile_para_single_query_search_simple_v3_ranged app/profile_para_single_query_search_simple_v3_ranged_num_checked.cpp core/Searching.202010291522.simple_v3.num_checked.cpp)
#target_compile_options(profile_para_single_query_search_simple_v3_ranged PRIVATE ${CXX_OPTION})
#target_link_libraries(profile_para_single_query_search_simple_v3_ranged PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

############# num_checked
add_executable(B app/profile_para_single_query_search_simple_v3_ranged.cpp core/Searching.202008310636.simple_v3.cpp)
target_compile_options(B PRIVATE ${CXX_OPTION})
target_link_libraries(B PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

############## ranged
add_executable(A app/profile_para_single_query_search_simple_v3_ranged.cpp core/Searching.202008310636.simple_v3.cpp)
target_compile_options(A PRIVATE ${CXX_OPTION})
target_link_libraries(A PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_para_single_query_search_simple_v3_standard_deviation app/profile_para_single_query_search_simple_v3_standard_deviation.cpp core/Searching.202011011451.simple_v3.std_dev.cpp)
target_compile_options(profile_para_single_query_search_simple_v3_standard_deviation PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_simple_v3_standard_deviation PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_para_single_query_search_simple_v3_standard_deviation_samples app/profile_para_single_query_search_simple_v3_standard_deviation_samples.cpp core/Searching.202011011451.simple_v3.std_dev.cpp)
target_compile_options(profile_para_single_query_search_simple_v3_standard_deviation_samples PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_simple_v3_standard_deviation_samples PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_para_single_query_search_simple_v3_stddev_metric app/profile_para_single_query_search_simple_v3_stddev_metric.cpp core/Searching.202011011451.simple_v3.std_dev.cpp)
target_compile_options(profile_para_single_query_search_simple_v3_stddev_metric PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_simple_v3_stddev_metric PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_para_single_query_search_simple_v3_stddev_metric_samples app/profile_para_single_query_search_simple_v3_stddev_metric_samples.cpp core/Searching.202011051142.simple_v3.large_graph.stddev_metric.cpp)
target_compile_options(profile_para_single_query_search_simple_v3_stddev_metric_samples PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_simple_v3_stddev_metric_samples PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_para_single_query_search_simple_v3_large_graph_num_checked_x app/profile_para_single_query_search_simple_v3_large_graph_num_checked_x.cpp core/Searching.202010301220.simple_v3.large_graph.num_checked.cpp)
target_compile_options(profile_para_single_query_search_simple_v3_large_graph_num_checked_x PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_simple_v3_large_graph_num_checked_x PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_para_single_query_search_simple_v3_large_graph_worst_and_best_unchecked app/profile_para_single_query_search_simple_v3_large_graph_worst_and_best_unchecked.cpp core/Searching.202011111604.simple_v3.large_graph.worst_and_best_unchecked.cpp)
target_compile_options(profile_para_single_query_search_simple_v3_large_graph_worst_and_best_unchecked PRIVATE ${CXX_OPTION})
target_link_libraries(profile_para_single_query_search_simple_v3_large_graph_worst_and_best_unchecked PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_search_simple_v3_one_merge app/profile_find_L_para_single_query_search_simple_v3_one_merge.cpp core/Searching.202011131441.simple_v3.one_merge.cpp)
target_compile_options(profile_find_L_para_single_query_search_simple_v3_one_merge PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_search_simple_v3_one_merge PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_find_L_para_single_query_search_simple_v3_large_graph_one_merge app/profile_find_L_para_single_query_search_simple_v3_large_graph_one_merge.cpp core/Searching.202011131454.simple_v3.large_graph.one_merge.cpp)
target_compile_options(profile_find_L_para_single_query_search_simple_v3_large_graph_one_merge PRIVATE ${CXX_OPTION})
target_link_libraries(profile_find_L_para_single_query_search_simple_v3_large_graph_one_merge PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(test_compare_groundtruth_and_true_nn tools/test_compare_groundtruth_and_true_nn.cpp)
target_compile_options(test_compare_groundtruth_and_true_nn PRIVATE ${CXX_OPTION})
target_link_libraries(test_compare_groundtruth_and_true_nn PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)