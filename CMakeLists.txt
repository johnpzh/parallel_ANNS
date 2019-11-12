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

# Build Library
add_library(PANNS
        core/searching.cpp
        ./include/utils.cpp
        include/efanna2e/index.cpp
        include/efanna2e/index_nsg.cpp)
target_compile_options(PANNS PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(PANNS PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

# Bin
add_executable(app_batch_searching app/app_batch_searching.cpp)
target_compile_options(app_batch_searching PRIVATE ${CXX_OPTION} -march=native)
#target_link_libraries(batch_searching PRIVATE PANNS -ltcmalloc)
target_link_libraries(app_batch_searching PRIVATE PANNS)