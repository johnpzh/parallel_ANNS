//
// Created by Zhen Peng on 6/11/2022.
//

#ifndef PCM_DEMO_PCM_MEM_H
#define PCM_DEMO_PCM_MEM_H

#include <iostream>
#include <string>
#include <algorithm>

#include "PCM/src/cpucounters.h"
#include "PCM/src/utils.h"

constexpr pcm::uint32 max_sockets = 256;
const pcm::uint32 max_imc_channels = pcm::ServerUncoreCounterState::maxChannels;
const pcm::uint32 max_edc_channels = pcm::ServerUncoreCounterState::maxChannels;
const pcm::uint32 max_imc_controllers = pcm::ServerUncoreCounterState::maxControllers;
const bool skipInactiveChannels = true;

typedef struct memdata {
    float iMC_Rd_socket_chan[max_sockets][pcm::ServerUncoreCounterState::maxChannels]{};
    float iMC_Wr_socket_chan[max_sockets][pcm::ServerUncoreCounterState::maxChannels]{};
    float iMC_PMM_Rd_socket_chan[max_sockets][pcm::ServerUncoreCounterState::maxChannels]{};
    float iMC_PMM_Wr_socket_chan[max_sockets][pcm::ServerUncoreCounterState::maxChannels]{};
    float iMC_PMM_MemoryMode_Miss_socket_chan[max_sockets][pcm::ServerUncoreCounterState::maxChannels]{};
    float iMC_Rd_socket[max_sockets]{};
    float iMC_Wr_socket[max_sockets]{};
    float iMC_PMM_Rd_socket[max_sockets]{};
    float iMC_PMM_Wr_socket[max_sockets]{};
    float iMC_PMM_MemoryMode_Miss_socket[max_sockets]{};
    bool iMC_NM_hit_rate_supported{};
    float iMC_PMM_MemoryMode_Hit_socket[max_sockets]{};
    bool M2M_NM_read_hit_rate_supported{};
    float iMC_NM_hit_rate[max_sockets]{};
    float M2M_NM_read_hit_rate[max_sockets][max_imc_controllers]{};
    float EDC_Rd_socket_chan[max_sockets][max_edc_channels]{};
    float EDC_Wr_socket_chan[max_sockets][max_edc_channels]{};
    float EDC_Rd_socket[max_sockets]{};
    float EDC_Wr_socket[max_sockets]{};
    pcm::uint64 partial_write[max_sockets]{};
    pcm::ServerUncoreMemoryMetrics metrics{};
} memdata_t;

bool anyPmem(const pcm::ServerUncoreMemoryMetrics & metrics);

void print_help(const std::string prog_name);

void printSocketBWHeader(pcm::uint32 no_columns, pcm::uint32 skt, const bool show_channel_output);

void printSocketRankBWHeader(pcm::uint32 no_columns, pcm::uint32 skt);

void printSocketRankBWHeader_cvt(const pcm::uint32 numSockets, const pcm::uint32 num_imc_channels, const int rankA, const int rankB);

void printSocketChannelBW(pcm::PCM * /*m*/, memdata_t *md, pcm::uint32 no_columns, pcm::uint32 skt);

void printSocketChannelBW(pcm::uint32 no_columns, pcm::uint32 skt, pcm::uint32 num_imc_channels, const pcm::ServerUncoreCounterState * uncState1, const pcm::ServerUncoreCounterState * uncState2, pcm::uint64 elapsedTime, int rankA, int rankB);

void printSocketChannelBW_cvt(const pcm::uint32 numSockets, const pcm::uint32 num_imc_channels, const pcm::ServerUncoreCounterState * uncState1,
                              const pcm::ServerUncoreCounterState * uncState2, const pcm::uint64 elapsedTime, const int rankA, const int rankB);

float AD_BW(const memdata_t *md, const pcm::uint32 skt);

float PMM_MM_Ratio(const memdata_t *md, const pcm::uint32 skt);

void printSocketBWFooter(pcm::uint32 no_columns, pcm::uint32 skt, const memdata_t *md);

void display_bandwidth(pcm::PCM *m, memdata_t *md, const pcm::uint32 no_columns, const bool show_channel_output, const bool print_update);

void display_bandwidth_csv(pcm::PCM *m, memdata_t *md, pcm::uint64 /*elapsedTime*/, const bool show_channel_output, const pcm::CsvOutputType outputType);

void calculate_bandwidth(pcm::PCM *m,
                         const pcm::ServerUncoreCounterState uncState1[],
                         const pcm::ServerUncoreCounterState uncState2[],
                         const pcm::uint64 elapsedTime,
                         const bool csv,
                         bool & csvheader,
                         pcm::uint32 no_columns,
                         const pcm::ServerUncoreMemoryMetrics & metrics,
                         const bool show_channel_output,
                         const bool print_update);

void calculate_bandwidth_rank(pcm::PCM *m,
                              const pcm::ServerUncoreCounterState uncState1[],
                              const pcm::ServerUncoreCounterState uncState2[],
                              const pcm::uint64 elapsedTime,
                              const bool csv,
                              bool &csvheader,
                              const pcm::uint32 no_columns,
                              const int rankA,
                              const int rankB);

#endif //PCM_DEMO_PCM_MEM_H
