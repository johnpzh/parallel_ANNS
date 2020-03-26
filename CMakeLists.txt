cmake_minimum_required(VERSION 3.10)
project(PANNS LANGUAGES CXX)

# Verbose Makefile
set(CMAKE_VERBOSE_MAKEFILE ON)

## C++20
#set(CMAKE_CXX_STANDARD 20)
# C++14
set(CMAKE_CXX_STANDARD 14)

# Boost
find_package(Boost)

# OpenMP
find_package(OpenMP)

# Debug Options
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(CXX_OPTION ${CXX_OPTION} -O0)
endif()
if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(CXX_OPTION ${CXX_OPTION} -Wall -Wextra -pedantic -Werror)
endif()
#set(CXX_OPTION ${CXX_OPTION} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free)
set(LIBPAPI /usr/local/lib/libpapi.a)

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
target_compile_options(NSG PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(NSG PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

# Bin
#add_executable(app_batch_searching app/app_batch_searching.cpp)
#target_compile_options(app_batch_searching PRIVATE ${CXX_OPTION} -march=native)
##target_link_libraries(batch_searching PRIVATE PANNS -ltcmalloc)
#target_link_libraries(app_batch_searching PRIVATE PANNS)

add_executable(app_seq_searching app/app_seq_searching.cpp)
target_compile_options(app_seq_searching PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(app_seq_searching PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#target_link_libraries(app_seq_searching PRIVATE ${LIBPAPI} ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#target_link_libraries(app_seq_searching PRIVATE PANNS ${LIBPAPI})

add_executable(app_para_searching app/app_para_searching.cpp)
target_compile_options(app_para_searching PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(app_para_searching PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

#add_executable(app_top_m_batch_search app/deprecated/app_top_m_batch_search.cpp)
#target_compile_options(app_top_m_batch_search PRIVATE ${CXX_OPTION} -march=native)
#target_link_libraries(app_top_m_batch_search PRIVATE PANNS ${LIBPAPI})

add_executable(test_nsg_optimized_search app/test_nsg_optimized_search.cpp)
#set_property(TARGET test_nsg_optimized_search PROPERTY POSITION_INDEPENDENT_CODE FALSE)
target_compile_options(test_nsg_optimized_search PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(test_nsg_optimized_search PRIVATE NSG)
#target_link_libraries(test_nsg_optimized_search PRIVATE NSG ${LIBPAPI})


add_executable(test_nsg_optimized_search_para app/test_nsg_optimized_search_para.cpp)
target_compile_options(test_nsg_optimized_search_para PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(test_nsg_optimized_search_para PRIVATE NSG OpenMP::OpenMP_CXX)
#target_link_libraries(test_nsg_optimized_search_para PRIVATE NSG ${LIBPAPI} OpenMP::OpenMP_CXX)

add_executable(app_top_m_seq_search app/app_top_m_seq_search.cpp)
target_compile_options(app_top_m_seq_search PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(app_top_m_seq_search PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(app_top_m_para_search app/app_top_m_para_search.cpp)
#target_compile_options(app_top_m_para_search PRIVATE ${CXX_OPTION} -march=native -S -o tmp.ass)
#target_compile_options(app_top_m_para_search PRIVATE ${CXX_OPTION} -march=native -S -o tmp.ass)
#target_compile_options(app_top_m_para_search PRIVATE ${CXX_OPTION} -march=native -qopt-report=5)
target_compile_options(app_top_m_para_search PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(app_top_m_para_search PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

#add_executable(profile_top_m_search app/profile_top_m_search.cpp)
#target_compile_options(profile_top_m_search PRIVATE ${CXX_OPTION} -march=native)
#target_link_libraries(profile_top_m_search PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#target_link_libraries(profile_top_m_search PRIVATE PANNS)

add_executable(app_batch_top_m_seq_search app/app_batch_top_m_seq_search.cpp)
target_compile_options(app_batch_top_m_seq_search PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(app_batch_top_m_seq_search PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

#add_executable(profile_top_m_batch_search_value_m app/profile_top_m_batch_search_value_m.cpp)
#target_compile_options(profile_top_m_batch_search_value_m PRIVATE ${CXX_OPTION} -march=native)
#target_link_libraries(profile_top_m_batch_search_value_m PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#target_link_libraries(profile_top_m_batch_search_value_m PRIVATE PANNS)

#add_executable(profile_para_single_query_top_m_search app/profile_para_single_query_top_m_search.cpp)
#target_compile_options(profile_para_single_query_top_m_search PRIVATE ${CXX_OPTION} -march=native)
#target_link_libraries(profile_para_single_query_top_m_search PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(widget_get_deep200M app/widget_get_deep200M.cpp)
target_compile_options(widget_get_deep200M PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(widget_get_deep200M PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

#add_executable(profile_top_m_batch_search_batch_size app/deprecated/profile_top_m_batch_search_batch_size.cpp)
#target_compile_options(profile_top_m_batch_search_batch_size PRIVATE ${CXX_OPTION} -march=native)
#target_link_libraries(profile_top_m_batch_search_batch_size PRIVATE PANNS)

#add_executable(profile_top_m_batch_search_find_L app/deprecated/profile_top_m_batch_search_find_L.cpp)
#target_compile_options(profile_top_m_batch_search_find_L PRIVATE ${CXX_OPTION} -march=native)
#target_link_libraries(profile_top_m_batch_search_find_L PRIVATE PANNS)
#
#add_executable(profile_nsg_optimized_search app/deprecated/profile_nsg_optimized_search.cpp)
#target_compile_options(profile_nsg_optimized_search PRIVATE ${CXX_OPTION} -march=native)
#target_link_libraries(profile_nsg_optimized_search PRIVATE NSG PANNS ${LIBPAPI})

#add_executable(profile_seq_searching_with_set_queue app/profile_seq_searching_with_set_queue.cpp)
#target_compile_options(profile_seq_searching_with_set_queue PRIVATE ${CXX_OPTION} -march=native)
#target_link_libraries(profile_seq_searching_with_set_queue PRIVATE PANNS ${LIBPAPI})

#add_executable(profile_para_single_query_top_m_search.202002250815.buckets_equal_width app/profile_para_single_query_top_m_search.202002250815.buckets_equal_width.cpp)
#target_compile_options(profile_para_single_query_top_m_search.202002250815.buckets_equal_width PRIVATE ${CXX_OPTION} -march=native)
#target_link_libraries(profile_para_single_query_top_m_search.202002250815.buckets_equal_width PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_para_single_query_top_m_search app/profile_para_single_query_top_m_search.cpp)
target_compile_options(profile_para_single_query_top_m_search PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(profile_para_single_query_top_m_search PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_seq_searching_find_value_L app/profile_seq_searching_find_value_L.cpp)
target_compile_options(profile_seq_searching_find_value_L PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(profile_seq_searching_find_value_L PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_top_m_para_search app/profile_top_m_para_search.cpp)
target_compile_options(profile_top_m_para_search PRIVATE ${CXX_OPTION} -march=native)
#target_compile_options(profile_top_m_para_search PRIVATE ${CXX_OPTION} -march=native -qopt-report)
target_link_libraries(profile_top_m_para_search PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#target_link_libraries(profile_top_m_para_search PRIVATE ${LIBPAPI} ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

#add_executable(profile_no_omp_top_m_seq_search app/profile_no_omp_top_m_seq_search.cpp)
#target_compile_options(profile_no_omp_top_m_seq_search PRIVATE ${CXX_OPTION} -march=native)
##target_compile_options(profile_no_omp_top_m_seq_search PRIVATE ${CXX_OPTION} -march=native -qopt-report)
#target_link_libraries(profile_no_omp_top_m_seq_search PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
##target_link_libraries(profile_no_omp_top_m_seq_search PRIVATE  ${LIBPAPI} ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#
#add_executable(profile_yes_omp_top_m_seq_search app/profile_yes_omp_top_m_seq_search.cpp)
#target_compile_options(profile_yes_omp_top_m_seq_search PRIVATE ${CXX_OPTION} -march=native)
##target_compile_options(profile_yes_omp_top_m_seq_search PRIVATE ${CXX_OPTION} -march=native -qopt-report)
#target_link_libraries(profile_yes_omp_top_m_seq_search PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
##target_link_libraries(profile_yes_omp_top_m_seq_search PRIVATE ${LIBPAPI} ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_visited_array_top_m_seq_search app/profile_visited_array_top_m_seq_search.cpp)
target_compile_options(profile_visited_array_top_m_seq_search PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(profile_visited_array_top_m_seq_search PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
#target_link_libraries(profile_visited_array_top_m_seq_search PRIVATE ${LIBPAPI} ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(test app/test.cpp)
#target_compile_options(test PRIVATE ${CXX_OPTION} -march=native)
target_compile_options(test PRIVATE ${CXX_OPTION} -march=native)
#target_compile_options(test PRIVATE ${CXX_OPTION} -march=native -qopt-report -no-vec -unroll=0)
#target_link_libraries(test PRIVATE ${LIBPAPI} ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
target_link_libraries(test PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(test_omp_yes app/test_omp_yes.cpp)
target_compile_options(test_omp_yes PRIVATE ${CXX_OPTION} -march=native -qopt-report)
target_link_libraries(test_omp_yes PRIVATE OpenMP::OpenMP_CXX)

add_executable(test_omp_no app/test_omp_no.cpp)
target_compile_options(test_omp_no PRIVATE ${CXX_OPTION} -march=native -qopt-report)
target_link_libraries(test_omp_no PRIVATE OpenMP::OpenMP_CXX)

add_executable(profile_batch_top_m_search_embarrassing_para app/profile_batch_top_m_search_embarrassing_para.cpp)
target_compile_options(profile_batch_top_m_search_embarrassing_para PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(profile_batch_top_m_search_embarrassing_para PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_para_single_query_top_m_search_sync_or_not app/profile_para_single_query_top_m_search_sync_or_not.cpp)
target_compile_options(profile_para_single_query_top_m_search_sync_or_not PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(profile_para_single_query_top_m_search_sync_or_not PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(profile_top_m_seq_search_bit_CAS app/profile_top_m_seq_search_bit_CAS.cpp)
target_compile_options(profile_top_m_seq_search_bit_CAS PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(profile_top_m_seq_search_bit_CAS PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)