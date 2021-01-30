//
// Created by Zhen Peng on 01/24/2021.
//

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <unordered_set>
#include <sys/resource.h>
#include "../include/hnswlib/hnswlib.h"

using idi = unsigned;

class StopW {
    std::chrono::steady_clock::time_point time_begin;
public:
    StopW() {
        time_begin = std::chrono::steady_clock::now();
    }

    double getElapsedTimeMicro() {
        std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
        return (std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_begin).count());
    }

    void reset() {
        time_begin = std::chrono::steady_clock::now();
    }

};

void load_true_NN(
        const char *filename,
        const unsigned num_queries,
        unsigned int *massQA)
//        std::vector< std::vector<idi> > &true_nn_list)
//        unsigned &t_K)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    idi t_query_num;
    idi t_K;
//    unsigned t_K;
    fin.read(reinterpret_cast<char *>(&t_query_num), sizeof(t_query_num));
    fin.read(reinterpret_cast<char *>(&t_K), sizeof(t_K));
//    if (t_query_num != query_num) {
//        fprintf(stderr, "Error: query_num %u is not equal to the record %u in true-NN file %s\n",
//                query_num, t_query_num, filename);
//        exit(EXIT_FAILURE);
//    }
    if (t_query_num != num_queries) {
        fprintf(stderr, "Error: t_query_num %u is not num_queries_ %u\n", t_query_num, num_queries);
        exit(EXIT_FAILURE);
    }
    if (t_K != 100) {
        fprintf(stderr, "Error: t_K %u is not 100.\n", t_K);
        exit(EXIT_FAILURE);
    }

////    data = new unsigned[(size_t) t_query_num * (size_t) t_K];
//    true_nn_list.resize(t_query_num);
//    for (idi q_i = 0; q_i < t_query_num; ++q_i) {
//        true_nn_list[q_i].resize(t_K);
//    }

    for (unsigned q_i = 0; q_i < t_query_num; ++q_i) {
        size_t offset = q_i * t_K;
        for (unsigned n_i = 0; n_i < t_K; ++n_i) {
            unsigned id;
            float dist;
            fin.read(reinterpret_cast<char *>(&id), sizeof(id));
            fin.read(reinterpret_cast<char *>(&dist), sizeof(dist));
            massQA[offset + n_i] = id;
//            true_nn_list[q_i][n_i] = id;
        }
    }

    fin.close();
}

void load_data(
        const char *filename,
        float *data,
        size_t &num,
        size_t &dim)
{  // load data with sift10K pattern
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
//    in.read((char*)&dim, 4);
    uint32_t t_d;
    in.read((char*) &t_d, 4);
    dim = (size_t) t_d;
//    std::cout << "data dimension: " << dim << std::endl;
    in.seekg(0, std::ios::end);
    std::ios::pos_type ss = in.tellg();
    size_t fsize = (size_t)ss;
    num = (fsize / (dim + 1) / 4);
//    data = new float[static_cast<uint64_t>(num) * static_cast<uint64_t>(dim)];

    in.seekg(0, std::ios::beg);
    for (size_t i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur);
        in.read((char*)(data + i * dim), dim * 4);
    }
    in.close();
}

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

static void get_gt(
        const unsigned int *massQA,
//        unsigned char *massQ,
//        unsigned char *mass,
//        size_t vecsize,
        size_t qsize,
//        hnswlib::L2SpaceI &l2space,
//        size_t vecdim,
        std::vector<std::priority_queue<std::pair<float, hnswlib::labeltype >>> &answers,
        const size_t k)
{
    (std::vector<std::priority_queue<std::pair<float, hnswlib::labeltype >>>(qsize)).swap(answers);
//    hnswlib::DISTFUNC<int> fstdistfunc_ = l2space.get_dist_func();
    size_t t_k = 100;
//    std::cout << qsize << "\n";
    for (int i = 0; i < qsize; i++) {
        for (int j = 0; j < k; j++) {
            answers[i].emplace(0.0f, massQA[t_k * i + j]);
        }
    }
}

static float test_approx(
        const float *massQ,
        size_t vecsize,
        size_t qsize,
        hnswlib::HierarchicalNSW<float> &appr_alg,
        size_t vecdim,
        const std::vector<std::priority_queue<std::pair<float, hnswlib::labeltype >>> &answers,
        size_t k) {
    size_t correct = 0;
    size_t total = 0;
    //uncomment to test in parallel mode:
    //#pragma omp parallel for
    for (int i = 0; i < qsize; i++) {

        std::priority_queue<std::pair<float, hnswlib::labeltype >> result = appr_alg.searchKnn(massQ + vecdim * i, k);
        std::priority_queue<std::pair<float, hnswlib::labeltype >> gt(answers[i]);
        std::unordered_set<hnswlib::labeltype> g;
        total += gt.size();

        while (gt.size()) {


            g.insert(gt.top().second);
            gt.pop();
        }

        while (result.size()) {
            if (g.find(result.top().second) != g.end()) {

                correct++;
            } else {
            }
            result.pop();
        }

    }
    return 1.0f * correct / total;
}

static void test_vs_recall(
        const float *massQ,
        size_t vecsize,
        size_t qsize,
        hnswlib::HierarchicalNSW<float> &appr_alg,
        size_t vecdim,
        const std::vector<std::priority_queue<std::pair<float, hnswlib::labeltype >>> &answers,
        size_t k)
{
//    std::vector<size_t> efs;// = { 10,10,10,10,10 };
//    for (int i = k; i < 30; i++) {
//        efs.push_back(i);
//    }
//    for (int i = 30; i < 100; i += 10) {
//        efs.push_back(i);
//    }
//    for (int i = 100; i < 500; i += 40) {
//        efs.push_back(i);
//    }
    std::vector<size_t> efs({500, 100, 300});
    for (size_t ef : efs) {
        appr_alg.setEf(ef);
        StopW stopw = StopW();

        float recall = test_approx(massQ, vecsize, qsize, appr_alg, vecdim, answers, k);
        float time_us_per_query = stopw.getElapsedTimeMicro() / qsize;

        std::cout << ef << "\t" << recall << "\t" << time_us_per_query << " us\n";
        if (recall > 1.0) {
            std::cout << recall << "\t" << time_us_per_query << " us\n";
            break;
        }
    }
}

void one_time_search_precision(
        const float *massQ,
        const size_t vecsize,
        const size_t qsize,
        hnswlib::HierarchicalNSW<float> &appr_alg,
        const size_t vecdim,
        const std::vector<std::priority_queue<std::pair<float, hnswlib::labeltype >>> &answers,
        const size_t k,
        const size_t ef,
        double &runtime,
        double &recall)
{
    appr_alg.setEf(ef);
    StopW stopw = StopW();

    recall = test_approx(massQ, vecsize, qsize, appr_alg, vecdim, answers, k);
    double time_us_per_query = stopw.getElapsedTimeMicro() / qsize;

    std::cout << ef << "\t" << recall << "\t" << time_us_per_query << " us\n";
    runtime = time_us_per_query * qsize / 1000000.0;
}

void search(
        hnswlib::HierarchicalNSW<float> *appr_alg,
        const std::vector<double> &P_targets,
        const size_t ef_lower_origin,
        const size_t ef_upper_origin,
        const float *massQ,
        const std::vector<std::priority_queue<std::pair<float, hnswlib::labeltype >>> &answers,
        const size_t vecsize,
        const size_t vecdim,
        const size_t qsize,
        const size_t k)
{
//    test_vs_recall(massQ, vecsize, qsize, *appr_alg, vecdim, answers, k);

    for (const double P_dest : P_targets) {

        size_t ef_upper = ef_upper_origin;
        size_t ef_lower = ef_lower_origin;
        size_t ef = ef_upper;
        double runtime;
        double recall;

        double last_runtime;
        double last_recall;
        size_t last_ef;

        while (ef_lower <= ef_upper) {
            printf("ef: %lu ef_lower: %lu ef_upper: %lu\n", ef, ef_lower, ef_upper);

            one_time_search_precision(
                    massQ,
                    vecsize,
                    qsize,
                    *appr_alg,
                    vecdim,
                    answers,
                    k,
                    ef,
                    runtime,
                    recall);

            if (recall < P_dest) {
                ef_lower = ef + 1;

            } else if (recall > P_dest) {
                ef_upper = ef - 1;
                last_runtime = runtime;
                last_recall = recall;
                last_ef = ef;
            } else {
                break;
            }
            if (ef_lower <= ef_upper) {
                ef = (ef_lower + ef_upper) / 2;
            }
        }

        if (recall < P_dest && ef < ef_upper_origin) {
            runtime = last_runtime;
            recall = last_recall;
            ef = last_ef;
        }

        printf("------- FINAL -------\n");
        printf("P_dest: %f "
               "runtime(s.): %f "
               "P@100: %f "
               "latency(ms.): %f "
               "ef: %lu ",
               P_dest,
               runtime,
               recall,
               runtime / qsize * 1000.0,
               ef);
        printf("\n");
    }
}

void usage(int argc, char *argv[])
{
    if (argc < 10) {
        fprintf(stderr,
                "Usage: %s <index_file> <query_file> <groundtrue_file> <size_in_millions> "
                "<dimension> <num_queries> <ef_lower> <ef_upper> "
                "<P@100> [<P@100> ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    usage(argc, argv);
    setbuf(stdout, nullptr); // Remove stdout buffer.

//    const char *path_data = argv[1]; // data file
    const char *path_index = argv[1]; // index
    const char *path_q = argv[2]; // query
    const char *path_gt = argv[3];
    const int subset_size_milllions = strtoull(argv[4], nullptr, 0); // number of vectors in millions
    const size_t vecdim = strtoull(argv[5], nullptr, 0); // dimention of vector
    const size_t qsize = strtoull(argv[6], nullptr, 0); // number of queries
    const size_t ef_lower_origin = strtoull(argv[7], nullptr, 0);
    const size_t ef_upper_origin = strtoull(argv[8], nullptr, 0);
    const unsigned base_loc_P_dest = 9;
    const unsigned num_P_target = argc - base_loc_P_dest;
    std::vector<double> P_targets(num_P_target);
    for (int a_i = 0; a_i < num_P_target; ++a_i) {
        P_targets[a_i] = strtod(argv[a_i + base_loc_P_dest], nullptr);
    }
//    const int M = strtoull(argv[5], nullptr, 0);
//    const int efConstruction = strtoull(argv[6], nullptr, 0);

    // Read ground truth
    printf("loading gt %s ...\n", path_gt);
    unsigned int *massQA = (unsigned *) malloc(qsize * 100 * sizeof(unsigned ));
    load_true_NN(
            path_gt,
            qsize,
            massQA);

    // Read queries
    printf("loading queries %s ...\n", path_q);
    float *massQ = (float *) malloc(qsize * vecdim * sizeof(float));
    {
        size_t num_q;
        size_t dim;
        load_data(
                path_q,
                massQ,
                num_q,
                dim);
        if (num_q != qsize) {
            fprintf(stderr, "Error: query file wrong num_q %lu (qsize %lu).\n", num_q, qsize);
            exit(EXIT_FAILURE);
        }
        if (dim != vecdim) {
            fprintf(stderr, "Error: query file wrong dim %lu (vecdim %lu).\n", dim, vecdim);
            exit(EXIT_FAILURE);
        }
    }

    hnswlib::L2Space l2space(vecdim);
    printf("loading index %s ...\n", path_index);
    hnswlib::HierarchicalNSW<float> *appr_alg = new hnswlib::HierarchicalNSW<float>(&l2space, path_index, false);
    std::vector<std::priority_queue<std::pair<float, hnswlib::labeltype >>> answers;
    const size_t k = 100;
    std::cout << "Parsing gt...\n";
    get_gt(
            massQA,
            qsize,
            answers,
            k);

    size_t vecsize = subset_size_milllions * 1000000;

    search(
            appr_alg,
            P_targets,
            ef_lower_origin,
            ef_upper_origin,
            massQ,
            answers,
            vecsize,
            vecdim,
            qsize,
            k);

    // Cleanup
    {
        free(massQA);
        free(massQ);
        delete appr_alg;
    }

    return EXIT_SUCCESS;
}

