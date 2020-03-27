//
// Created by Zhen Peng on 3/4/20.
//

// OpenMP benchmark reference: http://coliru.stacked-crooked.com/a/ee174916fa035f97
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <omp.h>
#include "../include/bitvector.h"

double print_c(double *C, int n)
{
    double r = 0.0;
    int i_bound = n * n;
#pragma omp parallel for reduction (+ : r)
    for (int i = 0; i < i_bound; ++i) {
        r += C[i];
    }
    return r;
}

void transpose(double *A, double *B, int n) {
    int i,j;
    for(i=0; i<n; i++) {
        for(j=0; j<n; j++) {
            B[j*n+i] = A[i*n+j];
        }
    }
}

void gemm(double *A, double *B, double *C, int n)
{
    int i, j, k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            double dot  = 0;
            for (k = 0; k < n; k++) {
                dot += A[i*n+k]*B[k*n+j];
            }
            C[i*n+j ] = dot;
        }
    }
}

void gemm_omp(double *A, double *B, double *C, int n)
{
#pragma omp parallel
    {
        int i, j, k;
#pragma omp for
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                double dot  = 0;
                for (k = 0; k < n; k++) {
                    dot += A[i*n+k]*B[k*n+j];
                }
                C[i*n+j ] = dot;
            }
        }

    }
}

void gemm_omp_combined(double *A, double *B, double *C, int n)
{
//#pragma omp parallel
//    {
        int i, j, k;
#pragma omp parallel for
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                double dot  = 0;
                for (k = 0; k < n; k++) {
                    dot += A[i*n+k]*B[k*n+j];
                }
                C[i*n+j ] = dot;
            }
        }

//    }
}

void gemm_omp_combined_inside(double *A, double *B, double *C, int n)
{
//#pragma omp parallel
//    {
//    int i, j, k;
#pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double dot  = 0;
            for (int k = 0; k < n; k++) {
                dot += A[i*n+k]*B[k*n+j];
            }
            C[i*n+j ] = dot;
        }
    }

//    }
}


void gemmT(double *A, double *B, double *C, int n)
{
    int i, j, k;
    double *B2;
    B2 = (double*)malloc(sizeof(double)*n*n);
    transpose(B,B2, n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            double dot  = 0;
            for (k = 0; k < n; k++) {
                dot += A[i*n+k]*B2[j*n+k];
            }
            C[i*n+j ] = dot;
        }
    }
    free(B2);
}

void gemmT_omp(double *A, double *B, double *C, int n)
{
    double *B2;
    B2 = (double*)malloc(sizeof(double)*n*n);
    transpose(B,B2, n);
#pragma omp parallel
    {
        int i, j, k;
#pragma omp for
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                double dot  = 0;
                for (k = 0; k < n; k++) {
                    dot += A[i*n+k]*B2[j*n+k];
                }
                C[i*n+j ] = dot;
            }
        }

    }
    free(B2);
}

void gemmT_omp_combined(double *A, double *B, double *C, int n)
{
    double *B2;
    B2 = (double*)malloc(sizeof(double)*n*n);
    transpose(B,B2, n);
//#pragma omp parallel
//    {
        int i, j, k;
#pragma omp parallel for default(shared)
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                double dot  = 0;
                for (k = 0; k < n; k++) {
                    dot += A[i*n+k]*B2[j*n+k];
                }
                C[i*n+j ] = dot;
            }
        }

//    }
    free(B2);
}

void gemmT_omp_combined_inside(double *A, double *B, double *C, int n)
{
    double *B2;
    B2 = (double*)malloc(sizeof(double)*n*n);
    transpose(B,B2, n);
//#pragma omp parallel
//    {
//    int i, j, k;
#pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double dot  = 0;
            for (int k = 0; k < n; k++) {
                dot += A[i*n+k]*B2[j*n+k];
            }
            C[i*n+j ] = dot;
        }
    }

//    }
    free(B2);
}

void test_openmp_performance(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, nullptr); // Remove stdout buffer.
//    setlocale(LC_NUMERIC, ""); // For comma number format

    int num_threads = strtoull(argv[1], nullptr, 0);
    omp_set_num_threads(num_threads);
//    printf("num_threads: %d\n", num_threads);

    int i, n;
    double *A, *B, *C, dtime;

    n=1024;
    A = (double*)malloc(sizeof(double)*n*n);
    B = (double*)malloc(sizeof(double)*n*n);
    C = (double*)malloc(sizeof(double)*n*n);
    for(i=0; i<n*n; i++) { A[i] = 1.0 * rand()/RAND_MAX; B[i] = 1.0 * rand()/RAND_MAX;}

    int warmup_max = 4;
    for (int warmup = 0; warmup < warmup_max; ++warmup) {
        dtime = omp_get_wtime();
        gemm(A, B, C, n);
        dtime = omp_get_wtime() - dtime;
        printf("num_thread: %d gemm: %f result: %f\n",
               num_threads,
               dtime,
               print_c(C, n));
        memset(C, 0, sizeof(double) * n * n);
    }

    for (int warmup = 0; warmup < warmup_max; ++warmup) {
        dtime = omp_get_wtime();
        gemm_omp(A, B, C, n);
        dtime = omp_get_wtime() - dtime;
        printf("num_threads: %d gemm_omp: %f result: %f\n",
               num_threads,
               dtime,
               print_c(C, n));
        memset(C, 0, sizeof(double) * n * n);
    }

    for (int warmup = 0; warmup < warmup_max; ++warmup) {
        dtime = omp_get_wtime();
        gemm_omp_combined(A, B, C, n);
        dtime = omp_get_wtime() - dtime;
        printf("num_threads: %d gemm_omp_combined: %f result: %f\n",
               num_threads,
               dtime,
               print_c(C, n));
        memset(C, 0, sizeof(double) * n * n);
    }

    for (int warmup = 0; warmup < warmup_max; ++warmup) {
        dtime = omp_get_wtime();
        gemm_omp_combined_inside(A, B, C, n);
        dtime = omp_get_wtime() - dtime;
        printf("num_threads: %d gemm_omp_combined_inside: %f result: %f\n",
               num_threads,
               dtime,
               print_c(C, n));
        memset(C, 0, sizeof(double) * n * n);
    }

    for (int warmup = 0; warmup < warmup_max; ++warmup) {
        dtime = omp_get_wtime();
        gemmT(A, B, C, n);
        dtime = omp_get_wtime() - dtime;
        printf("num_threads: %d gemmT: %f result: %f\n",
               num_threads,
               dtime,
               print_c(C, n));
        memset(C, 0, sizeof(double) * n * n);
    }

    for (int warmup = 0; warmup < warmup_max; ++warmup) {
        dtime = omp_get_wtime();
        gemmT_omp(A, B, C, n);
        dtime = omp_get_wtime() - dtime;
        printf("num_threads: %d gemmT_omp: %f result: %f\n",
               num_threads,
               dtime,
               print_c(C, n));
        memset(C, 0, sizeof(double) * n * n);
    }

    for (int warmup = 0; warmup < warmup_max; ++warmup) {
        dtime = omp_get_wtime();
        gemmT_omp_combined(A, B, C, n);
        dtime = omp_get_wtime() - dtime;
        printf("num_threads: %d gemmT_omp_combined: %f result: %f\n",
               num_threads,
               dtime,
               print_c(C, n));
        memset(C, 0, sizeof(double) * n * n);
    }

    for (int warmup = 0; warmup < warmup_max; ++warmup) {
        dtime = omp_get_wtime();
        gemmT_omp_combined_inside(A, B, C, n);
        dtime = omp_get_wtime() - dtime;
        printf("num_threads: %d gemmT_omp_combined_inside: %f result: %f\n",
               num_threads,
               dtime,
               print_c(C, n));
        memset(C, 0, sizeof(double) * n * n);
    }
}

void test_bitvector(int argc, char *argv[])
{
    if (argc != 1) {
        fprintf(stderr, "Usage: ./%s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    PANNS::BitVector bv(3);
    bv.resize(15);
    int x = 14;
    printf("bv.is_bit_set(%d): %u (0)\n", x, bv.is_bit_set(x));
    bv.set_bit(x);
    printf("bv.atomic_is_bit_set(%d): %u (1)\n", x, bv.atomic_is_bit_set(x));
    bv.reset_bit(x);
    printf("bv.is_bit_set(%d): %u (0)\n", x, bv.is_bit_set(x));
    bv.atomic_set_bit(x);
    printf("bv.atomic_is_bit_set(%d): %u (1)\n", x, bv.atomic_is_bit_set(x));
    bv.atomic_reset_bit(x);
    printf("bv.is_bit_set(%d): %u (0)\n", x, bv.is_bit_set(x));
    bv.set_bit(x);
    printf("bv.atomic_is_bit_set(%d): %u (1)\n", x, bv.atomic_is_bit_set(x));
    bv.clear_all();
    printf("bv.is_bit_set(%d): %u (0)\n", x, bv.is_bit_set(x));
}

int main(int argc, char *argv[])
{

//    test_openmp_performance(argc, argv);
    test_bitvector(argc, argv);

    return 0;
}

////////////////////////////////////////////////////////////////
//// PAPI test
//
//#include <iostream>
//#include <cstdio>
//#include <vector>
//#include <chrono>
//#include <clocale>
////#include "../core/Searching.201912161559.set_for_queue.h"
////#include "../core/Searching.201912091448.map_for_queries_ids.h"
////#include "../core/Searching.202002101535.reorganization.h"
////#include "../core/Searching.202002141745.critical_omp_top_m.h"
////#include "../core/Searching.202002181409.local_queue_and_merge.h"
////#include "../core/Searching.202002250815.buckets_equal_width.h"
//#include "../core/Searching.202003021000.profile_para_top_m_search.h"
//
//void handle_error (int retval)
//{
//    printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
//    exit(1);
//}
//
//void do_flops(int num)
//{
//    double r = 1;
//    for (int i = 1; i < num + 1; ++i) {
//        r *= i;
//    }
//    printf("r: %f\n", r);
//}
//void test_papi()
//{
//    const int NUM_FLOPS = 100;
//    int retval, EventSet=PAPI_NULL;
//    long_long values[1];
//
///* Initialize the PAPI library */
//    retval = PAPI_library_init(PAPI_VER_CURRENT);
//    if (retval != PAPI_VER_CURRENT) {
//        fprintf(stderr, "PAPI library init error!\n");
//        exit(1);
//    }
//
///* Create the Event Set */
//    if (PAPI_create_eventset(&EventSet) != PAPI_OK)
//        handle_error(1);
//
///* Add Total Instructions Executed to our Event Set */
//    if (PAPI_add_event(EventSet, PAPI_TOT_INS) != PAPI_OK)
//        handle_error(1);
//
///* Start counting events in the Event Set */
//    if (PAPI_start(EventSet) != PAPI_OK)
//        handle_error(1);
//
///* Defined in tests/do_loops.c in the PAPI source distribution */
//    do_flops(NUM_FLOPS);
//
///* Read the counting events in the Event Set */
//    if (PAPI_read(EventSet, values) != PAPI_OK)
//        handle_error(1);
//
//    printf("After reading the counters: %lld\n",values[0]);
//
///* Reset the counting events in the Event Set */
//    if (PAPI_reset(EventSet) != PAPI_OK)
//        handle_error(1);
//
//    do_flops(NUM_FLOPS);
//
///* Add the counters in the Event Set */
//    if (PAPI_accum(EventSet, values) != PAPI_OK)
//        handle_error(1);
//    printf("After adding the counters: %lld\n",values[0]);
//
//    do_flops(NUM_FLOPS);
//
///* Stop the counting of events in the Event Set */
//    if (PAPI_stop(EventSet, values) != PAPI_OK)
//        handle_error(1);
//
//    printf("After stopping the counters: %lld\n",values[0]);
//}
//
//void test_papi2()
//{
//    const int NUM_FLOPS = 10000000;
//    PANNS::L3CacheMissRate profiler;
//
//    for (int i = 0; i < 2; ++i) {
//        profiler.measure_start();
//        do_flops(NUM_FLOPS);
//        profiler.measure_stop();
//        profiler.print();
//    }
//}
//
//int main()
//{
//    test_papi2();
//
//    return EXIT_SUCCESS;
//}