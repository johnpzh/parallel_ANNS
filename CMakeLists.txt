cmake_minimum_required(VERSION 3.10)
project(batch_searching LANGUAGES CXX)

# Verbose Makefile
set(CMAKE_VERBOSE_MAKEFILE ON)

# C++14
set(CMAKE_CXX_STANDARD 14)

# Boost
find_package(Boost)

# OpenMP
find_package(OpenMP)

# Debug Options
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(CXX_OPTION -O0)
endif()
if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(CXX_OPTION ${CXX_OPTION} -Wall -Wextra -pedantic -Werror)
endif()
#set(CXX_OPTION ${CXX_OPTION} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free)
set(LIBPAPI /usr/local/lib/libpapi.a)

# Build Library
add_library(PANNS
        ./core/Searching.cpp
        ./include/utils.cpp)
target_compile_options(PANNS PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(PANNS PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_library(NSG
        include/efanna2e/index_nsg.cpp
        ./include/efanna2e/index.cpp)
target_compile_options(NSG PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(NSG PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

# Bin
#add_executable(app_batch_searching app/app_batch_searching.cpp)
#target_compile_options(app_batch_searching PRIVATE ${CXX_OPTION} -march=native)
##target_link_libraries(batch_searching PRIVATE PANNS -ltcmalloc)
#target_link_libraries(app_batch_searching PRIVATE PANNS)

add_executable(app_seq_searching app/app_seq_searching.cpp)
target_compile_options(app_seq_searching PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(app_seq_searching PRIVATE PANNS ${LIBPAPI})

add_executable(app_top_m_batch_search app/app_top_m_batch_search.cpp)
target_compile_options(app_top_m_batch_search PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(app_top_m_batch_search PRIVATE PANNS ${LIBPAPI})

add_executable(test_nsg_optimized_search app/test_nsg_optimized_search.cpp)
target_compile_options(test_nsg_optimized_search PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(test_nsg_optimized_search PRIVATE NSG ${LIBPAPI})

add_executable(profile_top_m_search app/profile_top_m_search.cpp)
target_compile_options(profile_top_m_search PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(profile_top_m_search PRIVATE PANNS)

add_executable(profile_top_m_batch_search app/profile_top_m_batch_search.cpp)
target_compile_options(profile_top_m_batch_search PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(profile_top_m_batch_search PRIVATE PANNS)

add_executable(profile_nsg_optimized_search app/profile_nsg_optimized_search.cpp)
target_compile_options(profile_nsg_optimized_search PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(profile_nsg_optimized_search PRIVATE NSG PANNS ${LIBPAPI})