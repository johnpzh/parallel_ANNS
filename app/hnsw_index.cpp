//
// Created by Zhen Peng on 01/22/2021.
//

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/resource.h>
#include "../include/hnswlib/hnswlib.h"

class StopW {
    std::chrono::steady_clock::time_point time_begin;
public:
    StopW() {
        time_begin = std::chrono::steady_clock::now();
    }

    float getElapsedTimeMicro() {
        std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
        return (std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_begin).count());
    }

    void reset() {
        time_begin = std::chrono::steady_clock::now();
    }

};

/**
* Returns the peak (maximum so far) resident set size (physical
* memory use) measured in bytes, or zero if the value cannot be
* determined on this OS.
*/
static size_t getPeakRSS() {
#if defined(_WIN32)
    /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    return (size_t)info.PeakWorkingSetSize;

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
    /* AIX and Solaris ------------------------------------------ */
    struct psinfo psinfo;
    int fd = -1;
    if ((fd = open("/proc/self/psinfo", O_RDONLY)) == -1)
        return (size_t)0L;      /* Can't open? */
    if (read(fd, &psinfo, sizeof(psinfo)) != sizeof(psinfo))
    {
        close(fd);
        return (size_t)0L;      /* Can't read? */
    }
    close(fd);
    return (size_t)(psinfo.pr_rssize * 1024L);

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* BSD, Linux, and OSX -------------------------------------- */
    struct rusage rusage;
    getrusage(RUSAGE_SELF, &rusage);
#if defined(__APPLE__) && defined(__MACH__)
    return (size_t)rusage.ru_maxrss;
#else
    return (size_t) (rusage.ru_maxrss * 1024L);
#endif

#else
    /* Unknown OS ----------------------------------------------- */
    return (size_t)0L;          /* Unsupported. */
#endif
}


/**
* Returns the current resident set size (physical memory use) measured
* in bytes, or zero if the value cannot be determined on this OS.
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
    FILE *fp = NULL;
    if ((fp = fopen("/proc/self/statm", "r")) == NULL)
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

inline bool exists_test(const std::string &name) {
    std::ifstream f(name.c_str());
    return f.good();
}

void build(
        const int subset_size_milllions,
        const size_t vecdim,
        const char *path_data,
        const char *path_index,
        const int M,
        const int efConstruction)
{


//    int subset_size_milllions = 200;
//    int efConstruction = 40; // ?
//    int M = 16; // ?

    size_t vecsize = subset_size_milllions * 1000000;

//    size_t qsize = 10000;
//    size_t vecdim = 128;
//    char path_index[1024];
//    char path_gt[1024];
//    char *path_q = "../bigann/bigann_query.bvecs";
//    char *path_data = "../bigann/bigann_base.bvecs";
//    sprintf(path_index, "sift1b_%dm_ef_%d_M_%d.bin", subset_size_milllions, efConstruction, M);

//    sprintf(path_gt, "../bigann/gnd/idx_%dM.ivecs", subset_size_milllions);


//    unsigned char *massb = new unsigned char[vecdim];
//    float massb = new float[vecdim];
//
//    std::cout << "Loading GT:\n";
//    std::ifstream inputGT(path_gt, std::ios::binary);
//    unsigned int *massQA = new unsigned int[qsize * 1000];
//    for (int i = 0; i < qsize; i++) {
//        int t;
//        inputGT.read((char *) &t, 4);
//        inputGT.read((char *) (massQA + 1000 * i), t * 4);
//        if (t != 1000) {
//            std::cout << "err";
//            return;
//        }
//    }
//    inputGT.close();

//    std::cout << "Loading queries:\n";
//    unsigned char *massQ = new unsigned char[qsize * vecdim];
//    std::ifstream inputQ(path_q, std::ios::binary);
//
//    for (int i = 0; i < qsize; i++) {
//        int in = 0;
//        inputQ.read((char *) &in, 4);
//        if (in != 128) {
//            std::cout << "file error";
//            exit(1);
//        }
//        inputQ.read((char *) massb, in);
//        for (int j = 0; j < vecdim; j++) {
//            massQ[i * vecdim + j] = massb[j];
//        }
//
//    }
//    inputQ.close();


//    unsigned char *mass = new unsigned char[vecdim];
    std::ifstream input(path_data, std::ios::binary);
    if (!input.is_open()) {
        fprintf(stderr, "Error: cannot open file %s .\n", path_data);
        exit(EXIT_FAILURE);
    }
    int in = 0;
//    hnswlib::L2SpaceI l2space(vecdim);
    hnswlib::L2Space l2space(vecdim);

//    hnswlib::HierarchicalNSW<int> *appr_alg;
    hnswlib::HierarchicalNSW<float> *appr_alg;
    if (exists_test(path_index)) {
//        std::cout << "Loading index from " << path_index << ":\n";
//        appr_alg = new hnswlib::HierarchicalNSW<int>(&l2space, path_index, false);
//        std::cout << "Actual memory usage: " << getCurrentRSS() / 1000000 << " Mb \n";
        printf("File %s exits.\n", path_index);
    } else {
        std::cout << "Building index:\n";
//        appr_alg = new hnswlib::HierarchicalNSW<int>(&l2space, vecsize, M, efConstruction);
        appr_alg = new hnswlib::HierarchicalNSW<float>(&l2space, vecsize, M, efConstruction);

        {
            float *mass = (float *) malloc(vecdim * sizeof(float));
            input.read((char *) &in, 4);
        if (in != vecdim) {
            std::cout << "file error";
            exit(1);
        }
//        input.read((char *) massb, in);
//
//        for (int j = 0; j < vecdim; j++) {
//            mass[j] = massb[j] * (1.0f);
//        }
//        appr_alg->addPoint((void *) (massb), (size_t) 0); // ??? massb or mass? Strange
            input.read(reinterpret_cast<char *>(mass), 4 * vecdim);
            appr_alg->addPoint((void *) mass, (size_t) 0);
            free(mass);
        }
        int j1 = 0;
        StopW stopw = StopW();
        StopW stopw_full = StopW();
        size_t report_every = 100000;
//        float *mass = (float *) malloc(vecdim * sizeof(float));
#pragma omp parallel for
        for (int i = 1; i < vecsize; i++) {
//            unsigned char mass[128];
            float *mass = (float *) malloc(vecdim * sizeof(float));
            int j2=0;
#pragma omp critical
            {

                input.read((char *) &in, 4);
                if (in != vecdim) {
                    std::cout << "file error";
                    exit(1);
                }
//                input.read((char *) massb, in);
//                for (int j = 0; j < vecdim; j++) {
//                    mass[j] = massb[j];
//                }
                input.read(reinterpret_cast<char *>(mass), 4 * vecdim);
                j1++;
                j2=j1;
                if (j1 % report_every == 0) {
                    std::cout << j1 / (0.01 * vecsize) << " %, "
                         << report_every / (1000.0 * 1e-6 * stopw.getElapsedTimeMicro()) << " kips " << " Mem: "
                         << getCurrentRSS() / 1000000 << " Mb \n";
                    stopw.reset();
                }
            }
            appr_alg->addPoint((void *) (mass), (size_t) j2);

            free(mass);
        }
//        free(mass);
        input.close();
        std::cout << "Build time:" << 1e-6 * stopw_full.getElapsedTimeMicro() << "  seconds\n";
        appr_alg->saveIndex(path_index);

        delete appr_alg;
    }


//    vector<std::priority_queue<std::pair<int, labeltype >>> answers;
//    size_t k = 1;
//    std::cout << "Parsing gt:\n";
//    get_gt(massQA, massQ, mass, vecsize, qsize, l2space, vecdim, answers, k);
//    std::cout << "Loaded gt\n";
//    for (int i = 0; i < 1; i++)
//        test_vs_recall(massQ, vecsize, qsize, *appr_alg, vecdim, answers, k);
//    std::cout << "Actual memory usage: " << getCurrentRSS() / 1000000 << " Mb \n";
}

void usage(int argc, char *argv[])
{
    if (argc != 7) {
        fprintf(stderr, "Usage: %s <data_file> <output_index> <size_in_millions> <dimension> <M> <efConstruction>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    usage(argc, argv);
    setbuf(stdout, nullptr); // Remove stdout buffer.

    const char *path_data = argv[1]; // data file
    const char *path_index = argv[2]; // output index
    const int subset_size_milllions = strtoull(argv[3], nullptr, 0); // number of vectors in millions
    const size_t vecdim = strtoull(argv[4], nullptr, 0); // dimention of vector
    const int M = strtoull(argv[5], nullptr, 0);
    const int efConstruction = strtoull(argv[6], nullptr, 0);

    build(
        subset_size_milllions,
        vecdim,
        path_data,
        path_index,
        M,
        efConstruction);

    return EXIT_SUCCESS;
}