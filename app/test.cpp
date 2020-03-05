//
// Created by Zhen Peng on 3/4/20.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>
#include <clocale>
//#include "../core/Searching.201912161559.set_for_queue.h"
//#include "../core/Searching.201912091448.map_for_queries_ids.h"
//#include "../core/Searching.202002101535.reorganization.h"
//#include "../core/Searching.202002141745.critical_omp_top_m.h"
//#include "../core/Searching.202002181409.local_queue_and_merge.h"
//#include "../core/Searching.202002250815.buckets_equal_width.h"
#include "../core/Searching.202003021000.profile_para_top_m_search.h"

void handle_error (int retval)
{
    printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
    exit(1);
}

void do_flops(int num)
{
    double r = 1;
    for (int i = 1; i < num + 1; ++i) {
        r *= i;
    }
    printf("r: %f\n", r);
}
void test_papi()
{
    const int NUM_FLOPS = 100;
    int retval, EventSet=PAPI_NULL;
    long_long values[1];

/* Initialize the PAPI library */
    retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        fprintf(stderr, "PAPI library init error!\n");
        exit(1);
    }

/* Create the Event Set */
    if (PAPI_create_eventset(&EventSet) != PAPI_OK)
        handle_error(1);

/* Add Total Instructions Executed to our Event Set */
    if (PAPI_add_event(EventSet, PAPI_TOT_INS) != PAPI_OK)
        handle_error(1);

/* Start counting events in the Event Set */
    if (PAPI_start(EventSet) != PAPI_OK)
        handle_error(1);

/* Defined in tests/do_loops.c in the PAPI source distribution */
    do_flops(NUM_FLOPS);

/* Read the counting events in the Event Set */
    if (PAPI_read(EventSet, values) != PAPI_OK)
        handle_error(1);

    printf("After reading the counters: %lld\n",values[0]);

/* Reset the counting events in the Event Set */
    if (PAPI_reset(EventSet) != PAPI_OK)
        handle_error(1);

    do_flops(NUM_FLOPS);

/* Add the counters in the Event Set */
    if (PAPI_accum(EventSet, values) != PAPI_OK)
        handle_error(1);
    printf("After adding the counters: %lld\n",values[0]);

    do_flops(NUM_FLOPS);

/* Stop the counting of events in the Event Set */
    if (PAPI_stop(EventSet, values) != PAPI_OK)
        handle_error(1);

    printf("After stopping the counters: %lld\n",values[0]);
}

void test_papi2()
{
    const int NUM_FLOPS = 10000000;
    PANNS::L3CacheMissRate profiler;

    for (int i = 0; i < 2; ++i) {
        profiler.measure_start();
        do_flops(NUM_FLOPS);
        profiler.measure_stop();
        profiler.print();
    }
}

int main()
{
    test_papi2();

    return EXIT_SUCCESS;
}