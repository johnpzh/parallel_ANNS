//
// Created by Zhen Peng on 7/20/20.
//

#ifndef PANNS_SPINLOCK_H
#define PANNS_SPINLOCK_H

#include <atomic>
//#include <boost/atomic/atomic_flag.hpp>
//#include <boost/memory_order.hpp>

namespace PANNS {

class Spinlock {
private:
    std::atomic<bool> lock_{false};
    // For C++20, or Boost 1.74, this could be atomic_flag

public:
    void lock()
    {
        for (;;) {
            if (!lock_.exchange(true, std::memory_order_acquire)) {
                break;
            }
            while (lock_.load(std::memory_order_relaxed)) {
                ;
            }
        }
    }

    void unlock()
    {
        lock_.store(false, std::memory_order_release);
    }
};

} // namespace PANNS

#endif //PANNS_SPINLOCK_H
