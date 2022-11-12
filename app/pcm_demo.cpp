//
// Created by Zhen Peng on 6/11/2022.
//

#include <iostream>
#include <stdint.h>
//#include <stdio.h>

// PCM Headers
#include "../include/PCM/src/cpucounters.h"
#include "../include/PCM/src/utils.h"
#include "../include/pcm_mem.h"


void process()
{
    int numbers[INT32_MAX];
    for (int i = 0; i < INT32_MAX; ++i) {
        numbers[i] = i;
    }

    int64_t sum = 0;
    for (int i = 0; i < INT32_MAX; ++i) {
        sum += numbers[i];
    }
    printf("sum: %lu\n", sum);
}

void test_pcm()
{
    // PCM Mem
    bool csv = false, csvheader = false, show_channel_output = true, print_update = false;
    pcm::uint32 no_columns = 2; // Default number of columns is 2
    pcm::PCM * m = pcm::PCM::getInstance();
    pcm::ServerUncoreMemoryMetrics metrics;
    metrics = m->PMMTrafficMetricsAvailable() ? pcm::Pmem : pcm::PartialWrites;
    m->disableJKTWorkaround();
    pcm::print_cpu_details();

    pcm::ServerUncoreCounterState * BeforeState = new pcm::ServerUncoreCounterState[m->getNumSockets()];
    pcm::ServerUncoreCounterState * AfterState = new pcm::ServerUncoreCounterState[m->getNumSockets()];
    pcm::uint64 BeforeTime = 0, AfterTime = 0;

    // Not sure which one should be used.
    m->setBlocked(true);
//    m->setBlocked(false);

    for(pcm::uint32 i=0; i<m->getNumSockets(); ++i)
        BeforeState[i] = m->getServerUncoreCounterState(i);

    BeforeTime = m->getTickCount();
    // End PCM Mem

    process();

    // PCM Mem
    AfterTime = m->getTickCount();
    for(pcm::uint32 i=0; i<m->getNumSockets(); ++i)
        AfterState[i] = m->getServerUncoreCounterState(i);

    calculate_bandwidth(m,BeforeState,AfterState,AfterTime-BeforeTime,csv,csvheader, no_columns, metrics,
                        show_channel_output, print_update);

    delete[] BeforeState;
    delete[] AfterState;
    // End PCM Mem
}

int main()
{
    std::cout << "PCM Demo to Show Memory Bandwidth." << std::endl;
    test_pcm();
    return 0;
}
