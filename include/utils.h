//
// Created by Zhen Peng on 11/11/19.
//

#ifndef BATCH_SEARCHING_UTILS_H
#define BATCH_SEARCHING_UTILS_H

#include <vector>
#include <papi.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <clocale>
#include <fstream>
#include <sys/time.h>
#include <immintrin.h>
#include "definitions.h"

namespace PANNS {

void load_data(
        char *filename,
//        std::vector<dataf> &data,
//        std::vector< std::vector<dataf> > &data,
        dataf *&data,
        idi &number,
        dimi &dimension);

void save_result(
        const char *filename,
        const std::vector<std::vector<unsigned>> &results);

double get_time_mark();

// For PAPI, cache miss rate and instruction counts
// PAPI test results
class L3CacheMissRate {
public:
    void measure_start()
    {
        int retval;
        if ((retval = PAPI_start_counters(events, num_events)) < PAPI_OK) {
            test_fail(__FILE__, __LINE__, "measure_start", retval);
        }
    }
    void measure_stop()
    {
        int retval;
//		long long counts[2];
        uint64_t counts[num_events];
        if ((retval = PAPI_stop_counters((long long *) counts, num_events)) < PAPI_OK) {
            test_fail(__FILE__, __LINE__, "measure_stop", retval);
        } else {
            for (int i = 0; i < num_events; ++i) {
                values[i] += counts[i];
            }
        }
    }
    void print(unsigned metrics = (unsigned) -1)
    {
        setlocale(LC_NUMERIC, "");
        if (metrics == (unsigned) -1) {
            printf("L3_cache_access: %'lu cache_misses: %'lu miss_rate: %.2f%% ", values[0], values[1], values[1] * 100.0 / values[0]);
            printf("Total_instructions_executed: %'lu\n", values[2]);
        } else {
            printf("%u %.2f\n", metrics, 1.0 * values[1]/values[0]);
            printf("%u %'lu\n", metrics, values[2]);
        }
    }

private:
    static const int num_events = 3;
    int events[num_events] = {PAPI_L3_TCA, PAPI_L3_TCM, PAPI_TOT_INS};
    std::vector<uint64_t> values = std::vector<uint64_t>(num_events, 0);

    void test_fail(const char *file, int line, const char *call, int retval)
    {
        printf("%s\tFAILED\nLine # %d\n", file, line);
        if ( retval == PAPI_ESYS ) {
            char buf[128];
            memset( buf, '\0', sizeof(buf) );
            sprintf(buf, "System error in %s:", call );
            perror(buf);
        }
        else if ( retval > 0 ) {
            printf("Error calculating: %s\n", call );
        }
        else {
            printf("Error in %s: %s\n", call, PAPI_strerror(retval) );
        }
        printf("\n");
        exit(1);
    }
};
// End class L2CacheMissRate

} // PANNS

#endif //BATCH_SEARCHING_UTILS_H
