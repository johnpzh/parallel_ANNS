cmake_minimum_required(VERSION 3.9)
project(faiss_test LANGUAGES CXX CUDA)

# Verbose Makefile
set(CMAKE_VERBOSE_MAKEFILE ON)

# C++ version
set(CMAKE_CXX_STANDARD 14)

# MKL
#find_package(BLAS)

add_executable(1-Flat 1-Flat.cc)
# target_link_libraries(1-Flat PRIVATE faiss_avx2 ${BLAS_LIBRARIES})
# target_link_libraries(1-Flat PRIVATE faiss)
#target_link_directories(1-Flat PRIVATE /usr/local/lib)
#target_link_directories(1-Flat PRIVATE /scratch/zpeng/local/openblas/lib)
target_link_directories(1-Flat PRIVATE /scratch/zpeng/local/miniconda3/envs/faiss/lib)
target_link_libraries(1-Flat PRIVATE faiss_avx2)

# find_package(MKL REQUIRED)
add_executable(4-GPU 4-GPU.cu)
target_link_directories(4-GPU PRIVATE /scratch/zpeng/local/miniconda3/envs/faiss/lib)
target_link_libraries(4-GPU PRIVATE faiss)

