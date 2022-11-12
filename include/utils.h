//
// Created by Zhen Peng on 11/11/19.
//

#ifndef BATCH_SEARCHING_UTILS_H
#define BATCH_SEARCHING_UTILS_H

#include <vector>
//#include <papi.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <clocale>
#include <fstream>
#include <cmath>
#include <sys/time.h>
#include <numeric>
#include <algorithm>
//#include <sys/resource.h>
#include <unistd.h>
#include <immintrin.h>
#include "definitions.h"

namespace PANNS {
//
//void load_data(
//        char *filename,
////        std::vector<dataf> &data,
////        std::vector< std::vector<dataf> > &data,
//        dataf *&data,
//        idi &number,
//        uint64_t &dimension);
//
//void save_result(
//        const char *filename,
//        const std::vector<std::vector<unsigned>> &results);
//
//double get_time_mark();

class Mem {
public:
    /**
     * reference: https://github.com/nmslib/hnswlib/blob/master/sift_1b.cpp
     * @return memory footprint in bytes.
     */
    static size_t getCurrentRSS() {
#if defined(_WIN32)
        /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    return (size_t)info.WorkingSetSize;

#elif defined(__APPLE__) && defined(__MACH__)
        /* OSX ------------------------------------------------------ */
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO,
        (task_info_t)&info, &infoCount) != KERN_SUCCESS)
        return (size_t)0L;      /* Can't access? */
    return (size_t)info.resident_size;

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
        /* Linux ---------------------------------------------------- */
        long rss = 0L;
        FILE *fp = nullptr;
        if ((fp = fopen("/proc/self/statm", "r")) == nullptr)
            return (size_t) 0L;      /* Can't open? */
        if (fscanf(fp, "%*s%ld", &rss) != 1) {
            fclose(fp);
            return (size_t) 0L;      /* Can't read? */
        }
        fclose(fp);
        return (size_t) rss * (size_t) sysconf(_SC_PAGESIZE);

#else
        /* AIX, BSD, Solaris, and Unknown OS ------------------------ */
    return (size_t)0L;          /* Unsupported. */
#endif
    }
};

/**
 * Class for Disk IO, such as load data from file and save result to file.
 */
class DiskIO {
public:
    /**
     * load data with sift10K pattern: 1 dimension number (4 bytes), float data (4 * dimension bytes), [repeat].
     * Reference: https://github.com/ZJULearning/nsg/blob/master/tests/test_nsg_optimized_search.cpp
     * @param filename
     * @param[out] data
     * @param[out] number
     * @param[out] dimension
     */
    static void load_data(
            const char *filename,
//        std::vector<dataf> &data,
//        std::vector< std::vector<dataf> > &data,
            dataf *&data,
            idi &number,
//            int &dimension)
            uint64_t &dimension)
    {
        std::ifstream fin(filename);
        if (!fin.is_open()) {
            fprintf(stderr, "Error: cannot open file %s\n", filename);
            exit(EXIT_FAILURE);
        }
        dimension = 0;
        fin.read(reinterpret_cast<char *>(&dimension), 4); // Read the dimension
        fin.seekg(0, std::ios_base::end);
        uint64_t file_size = fin.tellg();
        number = file_size / (4 + 4 * dimension);
//    data.resize(static_cast<size_t>(number) * static_cast<size_t>(dimension));
//    data.resize(number);
        data = (dataf *) malloc(static_cast<uint64_t>(number) * static_cast<uint64_t>(dimension) * sizeof(dataf));
//    data = (dataf *) _mm_malloc(static_cast<size_t>(number) * static_cast<size_t>(dimension) * sizeof(dataf), 64);
        if (!data) {
            fprintf(stderr, "Error: cannot malloc %lu bytes.\n", static_cast<size_t>(number) * static_cast<size_t>(dimension) * sizeof(dataf));
            exit(EXIT_FAILURE);
        }

        fin.seekg(0, std::ios_base::beg);
        for (size_t i = 0; i < number; i++) {
//        data[i].resize(dimension);

            fin.seekg(4, std::ios_base::cur);
//        fin.read(reinterpret_cast<char *>(&data[i * dimension]), 4 * dimension);
//        fin.read(reinterpret_cast<char *>(data[i].data()), 4 * dimension);
            fin.read(reinterpret_cast<char *>(data + i * dimension), 4 * dimension);
#ifdef DEBUG_PRINT
            if (!(i & 0xFFFFFF)) {
                printf("load_data: v_id: %lu ", i);
                printf("memory: %lu GB\n", Mem::getCurrentRSS() >> 30);
            }
#endif
        }
    }

    /**
     * Save candidates ID for all queries.
     * Reference: https://github.com/ZJULearning/nsg/blob/master/tests/test_nsg_optimized_search.cpp
     * @param filename
     * @param results
     */
    static void save_result(const char *filename,
                     const std::vector<std::vector<unsigned>> &results)
    {
        std::ofstream fout(filename);

//    for (size_t i = 0; i < results.size(); i++) {
//        unsigned GK = results[i].size();
//        fout.write(reinterpret_cast<char *>(&GK), sizeof(unsigned));
//        fout.write(reinterpret_cast<const char *>(results[i].data()), GK * sizeof(unsigned));
        for (const auto &qs : results) {
            unsigned GK = qs.size();
            fout.write(reinterpret_cast<char *>(&GK), sizeof(unsigned));
            fout.write(reinterpret_cast<const char *>(qs.data()), GK * sizeof(unsigned));
        }
    }
};

/**
 * Class WallTimer to get the wall time.
 */
class WallTimer {
public:
    /**
     *
     * @return time stamp in seconds.
     */
    static double get_time_mark()
    {
        timeval t;
        gettimeofday(&t, nullptr);
        return t.tv_sec + t.tv_usec * 0.000001;
    }
};


//// For PAPI, cache miss rate and instruction counts
//// PAPI version >= 6.0.0
//class L3CacheMissRate {
//public:
//    L3CacheMissRate()
//    {
//
//        int retval;
//        // Initialize PAPI
//        if ((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT) {
//            test_fail(__FILE__, __LINE__, "L3CacheMissRate()", retval);
//        }
//
//        // Create the event set
//        if ((retval = PAPI_create_eventset(&event_set_)) != PAPI_OK) {
//            test_fail(__FILE__, __LINE__, "L3CacheMissRate()", retval);
//        }
//
//        // Add events
//        if ((retval = PAPI_add_events(event_set_, events_, num_events_)) != PAPI_OK) {
//            test_fail(__FILE__, __LINE__, "L3CacheMissRate()", retval);
//        }
//    }
//    void measure_start()
//    {
//        int retval;
//        if ((retval = PAPI_start(event_set_)) != PAPI_OK) {
//            test_fail(__FILE__, __LINE__, "measure_start()", retval);
//        }
//    }
//    void measure_stop()
//    {
//        int retval;
//        long long counts[num_events_];
//        if ((retval = PAPI_stop(event_set_, counts)) != PAPI_OK) {
//            test_fail(__FILE__, __LINE__, "measure_stop()", retval);
//        } else {
//            for (int i = 0; i < num_events_; ++i) {
//                values[i] += counts[i];
//            }
//        }
//    }
//    void print(unsigned metrics = (unsigned) -1)
//    {
//        setlocale(LC_NUMERIC, "");
//        if (metrics == (unsigned) -1) {
//            printf("L3_cache_access: %'lu cache_misses: %'lu miss_rate: %.2f%% ", values[0], values[1], values[1] * 100.0 / values[0]);
//            printf("Total_instructions_executed: %'lu\n", values[2]);
//        } else {
//            printf("%u %.2f\n", metrics, 1.0 * values[1]/values[0]);
//            printf("%u %'lu\n", metrics, values[2]);
//        }
//    }
//
//private:
//    int event_set_ = PAPI_NULL;
//    static const int num_events_ = 3;
//    int events_[num_events_] = {PAPI_L3_TCA, PAPI_L3_TCM, PAPI_TOT_INS};
//    std::vector<uint64_t> values = std::vector<uint64_t>(num_events_, 0);
//
//    void test_fail(const char *file, int line, const char *call, int retval)
//    {
//        printf("%s\tFAILED\nLine # %d\n", file, line);
//        if ( retval == PAPI_ESYS ) {
//            char buf[1024];
//            memset( buf, '\0', sizeof(buf) );
//            sprintf(buf, "System error in %s: %s\n", call, PAPI_strerror(retval));
//            perror(buf);
//        }
//        else if ( retval > 0 ) {
//            printf("Error calculating: %s (%s)\n", call, PAPI_strerror(retval));
//        }
//        else {
//            printf("Error in %s: %s\n", call, PAPI_strerror(retval) );
//        }
//        printf("\n");
//        exit(1);
//    }
//};
//// Legacy PAPI API ( < 5.7.0)
//class L3CacheMissRate {
//public:
//    void measure_start()
//    {
//        int retval;
//        if ((retval = PAPI_start_counters(events, num_events)) < PAPI_OK) {
//            test_fail(__FILE__, __LINE__, "measure_start", retval);
//        }
//    }
//    void measure_stop()
//    {
//        int retval;
////		long long counts[2];
//        uint64_t counts[num_events];
//        if ((retval = PAPI_stop_counters((long long *) counts, num_events)) < PAPI_OK) {
//            test_fail(__FILE__, __LINE__, "measure_stop", retval);
//        } else {
//            for (int i = 0; i < num_events; ++i) {
//                values[i] += counts[i];
//            }
//        }
//    }
//    void print(unsigned metrics = (unsigned) -1)
//    {
//        setlocale(LC_NUMERIC, "");
//        if (metrics == (unsigned) -1) {
//            printf("L3_cache_access: %'lu cache_misses: %'lu miss_rate: %.2f%% ", values[0], values[1], values[1] * 100.0 / values[0]);
//            printf("Total_instructions_executed: %'lu\n", values[2]);
//        } else {
//            printf("%u %.2f\n", metrics, 1.0 * values[1]/values[0]);
//            printf("%u %'lu\n", metrics, values[2]);
//        }
//    }
//
//private:
//    static const int num_events = 3;
//    int events[num_events] = {PAPI_L3_TCA, PAPI_L3_TCM, PAPI_TOT_INS};
//    std::vector<uint64_t> values = std::vector<uint64_t>(num_events, 0);
//
//    void test_fail(const char *file, int line, const char *call, int retval)
//    {
//        printf("%s\tFAILED\nLine # %d\n", file, line);
//        if ( retval == PAPI_ESYS ) {
//            char buf[128];
//            memset( buf, '\0', sizeof(buf) );
//            sprintf(buf, "System error in %s:", call );
//            perror(buf);
//        }
//        else if ( retval > 0 ) {
//            printf("Error calculating: %s\n", call );
//        }
//        else {
//            printf("Error in %s: %s\n", call, PAPI_strerror(retval) );
//        }
//        printf("\n");
//        exit(1);
//    }
//};
// End class L2CacheMissRate

/**
 * Get the distance variation for the given vector.
 * @param distances
 * @return
 */
double get_dist_variation(const std::vector<distf> &distances);

/**
 * Get the distance standard deviation for the given vector.
 * @param distances
 * @return
 */
double get_dist_standard_deviation(const std::vector<distf> &distances);

/**
 * Get the distance's std_dev/mean for the given vector.
 * @param distances
 * @return
 */
double get_normalized_std_dev(const std::vector<distf> &distances);

/**
 * Get the median of all given indices
 * @param ids
 * @return
 */
idi get_index_median(const std::vector<idi> &ids);

/**
 * Get the mean of all given indices
 * @param ids
 * @return
 */
idi get_index_mean(const std::vector<idi> &ids);
} // PANNS

#endif //BATCH_SEARCHING_UTILS_H
