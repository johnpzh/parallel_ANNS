//
// Created by Zhen Peng on 2/18/20.
//

#ifndef BATCH_SEARCHING_PARALLELIZATION_H
#define BATCH_SEARCHING_PARALLELIZATION_H

#include <vector>
#include <omp.h>

namespace PANNS {

class AtomicOps {
public:
    // Utility Functions
    // Compare and Swap
    template <typename V_T>
    static bool CAS(V_T *ptr, V_T old_val, V_T new_val)
//inline bool CAS(void *ptr, V_T old_val, V_T new_val)
    {
//	return __atomic_compare_exchange(ptr, &old_val, &new_val, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
        if (1 == sizeof(V_T)) {
            return __atomic_compare_exchange(
                    reinterpret_cast<uint8_t *>(ptr),
                    reinterpret_cast<uint8_t *>(&old_val),
                    reinterpret_cast<uint8_t *>(&new_val),
                    false,
                    __ATOMIC_SEQ_CST,
                    __ATOMIC_SEQ_CST);
        } else if (2 == sizeof(V_T)) {
            return __atomic_compare_exchange(
                    reinterpret_cast<uint16_t *>(ptr),
                    reinterpret_cast<uint16_t *>(&old_val),
                    reinterpret_cast<uint16_t *>(&new_val),
                    false,
                    __ATOMIC_SEQ_CST,
                    __ATOMIC_SEQ_CST);
        } else if (4 == sizeof(V_T)) {
            return __atomic_compare_exchange(
                    reinterpret_cast<uint32_t *>(ptr),
                    reinterpret_cast<uint32_t *>(&old_val),
                    reinterpret_cast<uint32_t *>(&new_val),
                    false,
                    __ATOMIC_SEQ_CST,
                    __ATOMIC_SEQ_CST);
        } else if (8 == sizeof(V_T)) {
            return __atomic_compare_exchange(
                    reinterpret_cast<uint64_t *>(ptr),
                    reinterpret_cast<uint64_t *>(&old_val),
                    reinterpret_cast<uint64_t *>(&new_val),
                    false,
                    __ATOMIC_SEQ_CST,
                    __ATOMIC_SEQ_CST);
        } else {
            printf("CAS cannot support the type.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Thread-save enqueue operation.
    template<typename T, typename Int>
    static inline void TS_enqueue(
            std::vector<T> &queue,
            Int &end_queue,
            const T &e)
    {
        volatile Int old_i = end_queue;
        volatile Int new_i = old_i + 1;
        while (!CAS(&end_queue, old_i, new_i)) {
            old_i = end_queue;
            new_i = old_i + 1;
        }
        queue[old_i] = e;
    }
};

class ParallelOps {
public:
    // Parallel Prefix Sum
    template<typename Int>
    static inline Int prefix_sum_for_offsets(
            std::vector<Int> &offsets)
    {
        size_t size_offsets = offsets.size();
        if (1 == size_offsets) {
            Int tmp = offsets[0];
            offsets[0] = 0;
            return tmp;
        } else if (size_offsets < 2048) {
            Int offset_sum = 0;
            size_t size = size_offsets;
            for (size_t i = 0; i < size; ++i) {
                Int tmp = offsets[i];
                offsets[i] = offset_sum;
                offset_sum += tmp;
            }
            return offset_sum;
        } else {
            // Parallel Prefix Sum, based on Guy E. Blelloch's Prefix Sums and Their Applications
            Int last_element = offsets[size_offsets - 1];
            //	idi size = 1 << ((idi) log2(size_offsets - 1) + 1);
            size_t size = 1 << ((size_t) log2(size_offsets));
            //	std::vector<idi> nodes(size, 0);
            Int tmp_element = offsets[size - 1];
            //#pragma omp parallel for
            //	for (idi i = 0; i < size_offsets; ++i) {
            //		nodes[i] = offsets[i];
            //	}

            // Up-Sweep (Reduce) Phase
            Int log2size = log2(size);
            for (Int d = 0; d < log2size; ++d) {
                Int by = 1 << (d + 1);
#pragma omp parallel for
                for (size_t k = 0; k < size; k += by) {
                    offsets[k + (1 << (d + 1)) - 1] += offsets[k + (1 << d) - 1];
                }
            }

            // Down-Sweep Phase
            offsets[size - 1] = 0;
            for (Int d = log2size - 1; d != (Int) -1; --d) {
                Int by = 1 << (d + 1);
#pragma omp parallel for
                for (size_t k = 0; k < size; k += by) {
                    Int t = offsets[k + (1 << d) - 1];
                    offsets[k + (1 << d) - 1] = offsets[k + (1 << (d + 1)) - 1];
                    offsets[k + (1 << (d + 1)) - 1] += t;
                }
            }

            //#pragma omp parallel for
            //	for (idi i = 0; i < size_offsets; ++i) {
            //		offsets[i] = nodes[i];
            //	}
            if (size != size_offsets) {
                Int tmp_sum = offsets[size - 1] + tmp_element;
                for (size_t i = size; i < size_offsets; ++i) {
                    Int t = offsets[i];
                    offsets[i] = tmp_sum;
                    tmp_sum += t;
                }
            }

            return offsets[size_offsets - 1] + last_element;
        }
    }

    // Parallelly collect elements of tmp_queue into the queue.
    template<typename T, typename Int>
    static inline void collect_into_queue(
//					std::vector<idi> &tmp_queue,
            std::vector<T> &tmp_queue,
            std::vector<Int> &offsets_tmp_queue, // the locations for reading tmp_queue
            std::vector<Int> &offsets_queue, // the locations for writing queue.
            const Int num_elements, // total number of elements which need to be added from tmp_queue to queue
//					std::vector<idi> &queue,
            std::vector<T> &queue,
            Int &end_queue)
    {
        if (0 == num_elements) {
            return;
        }
        size_t i_bound = offsets_tmp_queue.size();
#pragma omp parallel for
        for (size_t i = 0; i < i_bound; ++i) {
            Int i_q_start = end_queue + offsets_queue[i];
            Int i_q_bound;
            if (i_bound - 1 != i) {
                i_q_bound = end_queue + offsets_queue[i + 1];
            } else {
                i_q_bound = end_queue + num_elements;
            }
            if (i_q_start == i_q_bound) {
// If the group has no elements to be added, then continue to the next group
                continue;
            }
            Int end_tmp = offsets_tmp_queue[i];
            for (Int i_q = i_q_start; i_q < i_q_bound; ++i_q) {
                queue[i_q] = tmp_queue[end_tmp++];
            }
        }
        end_queue += num_elements;
    }

};

} // namespace PANNS

#endif //BATCH_SEARCHING_PARALLELIZATION_H