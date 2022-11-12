//
// Created by Zhen Peng on 6/11/2022.
//

#include "pcm_mem.h"

inline bool anyPmem(const pcm::ServerUncoreMemoryMetrics & metrics)
{
    return (metrics == pcm::Pmem) || (metrics == pcm::PmemMixedMode) || (metrics == pcm::PmemMemoryMode);
}

void print_help(const std::string prog_name)
{
    std::cerr << "\n Usage: \n " << prog_name
              << " --help | [delay] [options] [-- external_program [external_program_options]]\n";
    std::cerr << "   <delay>                           => time interval to sample performance counters.\n";
    std::cerr << "                                        If not specified, or 0, with external program given\n";
    std::cerr << "                                        will read counters only after external program finishes\n";
    std::cerr << " Supported <options> are: \n";
    std::cerr << "  -h    | --help  | /h               => print this help and exit\n";
    std::cerr << "  -rank=X | /rank=X                  => monitor DIMM rank X. At most 2 out of 8 total ranks can be monitored simultaneously.\n";
    std::cerr << "  -pmm | /pmm | -pmem | /pmem        => monitor PMM memory bandwidth and DRAM cache hit rate in Memory Mode (default on systems with PMM support).\n";
    std::cerr << "  -mm                                => monitor detailed PMM Memory Mode metrics per-socket.\n";
    std::cerr << "  -mixed                             => monitor PMM mixed mode (AppDirect + Memory Mode).\n";
    std::cerr << "  -partial                           => monitor partial writes instead of PMM (default on systems without PMM support).\n";
    std::cerr << "  -nc   | --nochannel | /nc          => suppress output for individual channels.\n";
    std::cerr << "  -csv[=file.csv] | /csv[=file.csv]  => output compact CSV format to screen or\n"
              << "                                        to a file, in case filename is provided\n";
    std::cerr << "  -columns=X | /columns=X            => Number of columns to display the NUMA Nodes, defaults to 2.\n";
    std::cerr << "  -all | /all                        => Display all channels (even with no traffic)\n";
    std::cerr << "  -i[=number] | /i[=number]          => allow to determine number of iterations\n";
    std::cerr << "  -s                                 => silence information output and print only measurements\n";
    std::cerr << "  -u                                 => update measurements instead of printing new ones\n";
#ifdef _MSC_VER
    std::cerr << "  --uninstallDriver | --installDriver=> (un)install driver\n";
#endif
    std::cerr << " Examples:\n";
    std::cerr << "  " << prog_name << " 1                  => print counters every second without core and socket output\n";
    std::cerr << "  " << prog_name << " 0.5 -csv=test.log  => twice a second save counter values to test.log in CSV format\n";
    std::cerr << "  " << prog_name << " /csv 5 2>/dev/null => one sampe every 5 seconds, and discard all diagnostic output\n";
    std::cerr << "\n";
}

void printSocketBWHeader(pcm::uint32 no_columns, pcm::uint32 skt, const bool show_channel_output)
{
    for (pcm::uint32 i=skt; i<(no_columns+skt); ++i) {
        std::cout << "|---------------------------------------|";
    }
    std::cout << "\n";
    for (pcm::uint32 i=skt; i<(no_columns+skt); ++i) {
        std::cout << "|--             Socket " << std::setw(2) << i << "             --|";
    }
    std::cout << "\n";
    for (pcm::uint32 i=skt; i<(no_columns+skt); ++i) {
        std::cout << "|---------------------------------------|";
    }
    std::cout << "\n";
    if (show_channel_output) {
        for (pcm::uint32 i=skt; i<(no_columns+skt); ++i) {
            std::cout << "|--     Memory Channel Monitoring     --|";
        }
        std::cout << "\n";
        for (pcm::uint32 i=skt; i<(no_columns+skt); ++i) {
            std::cout << "|---------------------------------------|";
        }
        std::cout << "\n";
    }
}


void printSocketRankBWHeader(pcm::uint32 no_columns, pcm::uint32 skt)
{
    for (pcm::uint32 i=skt; i<(no_columns+skt); ++i) {
        std::cout << "|-------------------------------------------|";
    }
    std::cout << "\n";
    for (pcm::uint32 i=skt; i<(no_columns+skt); ++i) {
        std::cout << "|--               Socket " << std::setw(2) << i << "               --|";
    }
    std::cout << "\n";
    for (pcm::uint32 i=skt; i<(no_columns+skt); ++i) {
        std::cout << "|-------------------------------------------|";
    }
    std::cout << "\n";
    for (pcm::uint32 i=skt; i<(no_columns+skt); ++i) {
        std::cout << "|--           DIMM Rank Monitoring        --|";
    }
    std::cout << "\n";
    for (pcm::uint32 i=skt; i<(no_columns+skt); ++i) {
        std::cout << "|-------------------------------------------|";
    }
    std::cout << "\n";
}

void printSocketRankBWHeader_cvt(const pcm::uint32 numSockets, const pcm::uint32 num_imc_channels, const int rankA, const int rankB)
{
    printDateForCSV(pcm::Header1);
    for (pcm::uint32 skt = 0 ; skt < (numSockets) ; ++skt) {
        for (pcm::uint32 channel = 0; channel < num_imc_channels; ++channel) {
            if (rankA >= 0)
                std::cout << "SKT" << skt << "," << "SKT" << skt << ",";
            if (rankB >= 0)
                std::cout << "SKT" << skt << "," << "SKT" << skt << ",";
        }
    }
    std::cout << std::endl;

    printDateForCSV(pcm::Header2);
    for (pcm::uint32 skt = 0 ; skt < (numSockets) ; ++skt) {
        for (pcm::uint32 channel = 0; channel < num_imc_channels; ++channel) {
            if (rankA >= 0) {
                std::cout << "Mem_Ch" << channel << "_R" << rankA << "_reads,"
                          << "Mem_Ch" << channel << "_R" << std::setw(1) << rankA << "_writes,";
            }
            if (rankB >= 0) {
                std::cout << "Mem_Ch" << channel << "_R" << rankB << "_reads,"
                          << "Mem_Ch" << channel << "_R" << std::setw(1) << rankB << "_writes,";
            }
        }
    }
    std::cout << std::endl;
}

void printSocketChannelBW(pcm::PCM * /*m*/, memdata_t *md, pcm::uint32 no_columns, pcm::uint32 skt)
{
    for (pcm::uint32 channel = 0; channel < max_imc_channels; ++channel) {
        // check all the sockets for bad channel "channel"
        unsigned bad_channels = 0;
        for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
            if (md->iMC_Rd_socket_chan[i][channel] < 0.0 || md->iMC_Wr_socket_chan[i][channel] < 0.0) //If the channel read neg. value, the channel is not working; skip it.
                ++bad_channels;
        }
        if (bad_channels == no_columns) { // the channel is missing on all sockets in the row
            continue;
        }
        for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
            std::cout << "|-- Mem Ch " << std::setw(2) << channel << ": Reads (MB/s): " << std::setw(8) << md->iMC_Rd_socket_chan[i][channel] << " --|";
        }
        std::cout << "\n";
        for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
            std::cout << "|--            Writes(MB/s): " << std::setw(8) << md->iMC_Wr_socket_chan[i][channel] << " --|";
        }
        std::cout << "\n";
        if (md->metrics == pcm::Pmem)
        {
            for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
                std::cout << "|--      PMM Reads(MB/s)   : " << std::setw(8) << md->iMC_PMM_Rd_socket_chan[i][channel] << " --|";
            }
            std::cout << "\n";
            for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
                std::cout << "|--      PMM Writes(MB/s)  : " << std::setw(8) << md->iMC_PMM_Wr_socket_chan[i][channel] << " --|";
            }
            std::cout << "\n";
        }
    }
}

void printSocketChannelBW(pcm::uint32 no_columns, pcm::uint32 skt, pcm::uint32 num_imc_channels, const pcm::ServerUncoreCounterState * uncState1, const pcm::ServerUncoreCounterState * uncState2, pcm::uint64 elapsedTime, int rankA, int rankB)
{
    for (pcm::uint32 channel = 0; channel < num_imc_channels; ++channel) {
        if(rankA >= 0) {
            for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
                std::cout << "|-- Mem Ch " << std::setw(2) << channel << " R " << std::setw(1) << rankA << ": Reads (MB/s): " << std::setw(8) << (float) (getMCCounter(channel,pcm::ServerPCICFGUncore::EventPosition::READ_RANK_A,uncState1[i],uncState2[i]) * 64 / 1000000.0 / (elapsedTime/1000.0)) << " --|";
            }
            std::cout << "\n";
            for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
                std::cout << "|--                Writes(MB/s): " << std::setw(8) << (float) (getMCCounter(channel,pcm::ServerPCICFGUncore::EventPosition::WRITE_RANK_A,uncState1[i],uncState2[i]) * 64 / 1000000.0 / (elapsedTime/1000.0)) << " --|";
            }
            std::cout << "\n";
        }
        if(rankB >= 0) {
            for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
                std::cout << "|-- Mem Ch " << std::setw(2) << channel << " R " << std::setw(1) << rankB << ": Reads (MB/s): " << std::setw(8) << (float) (getMCCounter(channel,pcm::ServerPCICFGUncore::EventPosition::READ_RANK_B,uncState1[i],uncState2[i]) * 64 / 1000000.0 / (elapsedTime/1000.0)) << " --|";
            }
            std::cout << "\n";
            for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
                std::cout << "|--                Writes(MB/s): " << std::setw(8) << (float) (getMCCounter(channel,pcm::ServerPCICFGUncore::EventPosition::WRITE_RANK_B,uncState1[i],uncState2[i]) * 64 / 1000000.0 / (elapsedTime/1000.0)) << " --|";
            }
            std::cout << "\n";
        }
    }
}

void printSocketChannelBW_cvt(const pcm::uint32 numSockets, const pcm::uint32 num_imc_channels, const pcm::ServerUncoreCounterState * uncState1,
                              const pcm::ServerUncoreCounterState * uncState2, const pcm::uint64 elapsedTime, const int rankA, const int rankB)
{
    printDateForCSV(pcm::Data);
    for (pcm::uint32 skt = 0 ; skt < numSockets; ++skt) {
        for (pcm::uint32 channel = 0 ; channel < num_imc_channels ; ++channel) {
            if(rankA >= 0) {
                std::cout << (float) (getMCCounter(channel,pcm::ServerPCICFGUncore::EventPosition::READ_RANK_A,uncState1[skt],uncState2[skt]) * 64 / 1000000.0 / (elapsedTime/1000.0))
                          << "," << (float) (getMCCounter(channel,pcm::ServerPCICFGUncore::EventPosition::WRITE_RANK_A,uncState1[skt],uncState2[skt]) * 64 / 1000000.0 / (elapsedTime/1000.0)) << ",";
            }
            if(rankB >= 0) {
                std::cout << (float) (getMCCounter(channel,pcm::ServerPCICFGUncore::EventPosition::READ_RANK_B,uncState1[skt],uncState2[skt]) * 64 / 1000000.0 / (elapsedTime/1000.0))
                          << "," << (float) (getMCCounter(channel,pcm::ServerPCICFGUncore::EventPosition::WRITE_RANK_B,uncState1[skt],uncState2[skt]) * 64 / 1000000.0 / (elapsedTime/1000.0)) << ",";
            }
        }
    }
    std::cout << std::endl;
}

float AD_BW(const memdata_t *md, const pcm::uint32 skt)
{
    const auto totalPMM = md->iMC_PMM_Rd_socket[skt] + md->iMC_PMM_Wr_socket[skt];
    return (std::max)(totalPMM - md->iMC_PMM_MemoryMode_Miss_socket[skt], float(0.0));
}

float PMM_MM_Ratio(const memdata_t *md, const pcm::uint32 skt)
{
    const auto dram = md->iMC_Rd_socket[skt] + md->iMC_Wr_socket[skt];
    return md->iMC_PMM_MemoryMode_Miss_socket[skt] / dram;
}

void printSocketBWFooter(pcm::uint32 no_columns, pcm::uint32 skt, const memdata_t *md)
{
    for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
        std::cout << "|-- NODE" << std::setw(2) << i << " Mem Read (MB/s) : " << std::setw(8) << md->iMC_Rd_socket[i] << " --|";
    }
    std::cout << "\n";
    for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
        std::cout << "|-- NODE" << std::setw(2) << i << " Mem Write(MB/s) : " << std::setw(8) << md->iMC_Wr_socket[i] << " --|";
    }
    std::cout << "\n";
    if (anyPmem(md->metrics))
    {
        for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
            std::cout << "|-- NODE" << std::setw(2) << i << " PMM Read (MB/s):  " << std::setw(8) << md->iMC_PMM_Rd_socket[i] << " --|";
        }
        std::cout << "\n";
        for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
            std::cout << "|-- NODE" << std::setw(2) << i << " PMM Write(MB/s):  " << std::setw(8) << md->iMC_PMM_Wr_socket[i] << " --|";
        }
        std::cout << "\n";
    }
    if (md->metrics == pcm::PmemMixedMode)
    {
        for (pcm::uint32 i = skt; i < (skt + no_columns); ++i)
        {
            std::cout << "|-- NODE" << std::setw(2) << i << " PMM AD Bw(MB/s):  " << std::setw(8) << AD_BW(md, i) << " --|";
        }
        std::cout << "\n";
        for (pcm::uint32 i = skt; i < (skt + no_columns); ++i)
        {
            std::cout << "|-- NODE" << std::setw(2) << i << " PMM MM Bw(MB/s):  " << std::setw(8) << md->iMC_PMM_MemoryMode_Miss_socket[i] << " --|";
        }
        std::cout << "\n";
        for (pcm::uint32 i = skt; i < (skt + no_columns); ++i)
        {
            std::cout << "|-- NODE" << std::setw(2) << i << " PMM MM Bw/DRAM Bw:" << std::setw(8) << PMM_MM_Ratio(md, i) << " --|";
        }
        std::cout << "\n";
    }
    else if (md->metrics == pcm::Pmem && md->M2M_NM_read_hit_rate_supported)
    {
        for (pcm::uint32 ctrl = 0; ctrl < max_imc_controllers; ++ctrl)
        {
            for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
                std::cout << "|-- NODE" << std::setw(2) << i << "." << ctrl << " NM read hit rate :" << std::setw(6) << md->M2M_NM_read_hit_rate[i][ctrl] << " --|";
            }
            std::cout << "\n";
        }
    }
    if (md->metrics == pcm::PmemMemoryMode && md->iMC_NM_hit_rate_supported)
    {
        for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
            std::cout << "|-- NODE" << std::setw(2) << i << " NM hit rate:        " << std::setw(6) << md->iMC_NM_hit_rate[i] << " --|";
        }
        std::cout << "\n";
        for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
            std::cout << "|-- NODE" << std::setw(2) << i << " NM hits   (M/s):   " << std::setw(7) << (md->iMC_PMM_MemoryMode_Hit_socket[i])/1000000. << " --|";
        }
        std::cout << "\n";
        for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
            std::cout << "|-- NODE" << std::setw(2) << i << " NM misses (M/s):   " << std::setw(7) << (md->iMC_PMM_MemoryMode_Miss_socket[i])/1000000. << " --|";
        }
        std::cout << "\n";
    }
    if (md->metrics == pcm::PartialWrites)
    {
        for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
            std::cout << "|-- NODE" << std::setw(2) << i << " P. Write (T/s): " << std::dec << std::setw(10) << md->partial_write[i] << " --|";
        }
        std::cout << "\n";
    }
    for (pcm::uint32 i=skt; i<(skt+no_columns); ++i) {
        std::cout << "|-- NODE" << std::setw(2) << i << " Memory (MB/s): " << std::setw(11) << std::right << (md->iMC_Rd_socket[i]+md->iMC_Wr_socket[i]+
                                                                                                              md->iMC_PMM_Rd_socket[i]+md->iMC_PMM_Wr_socket[i]) << " --|";
    }
    std::cout << "\n";
    for (pcm::uint32 i=skt; i<(no_columns+skt); ++i) {
        std::cout << "|---------------------------------------|";
    }
    std::cout << "\n";
}

void display_bandwidth(pcm::PCM *m, memdata_t *md, const pcm::uint32 no_columns, const bool show_channel_output, const bool print_update)
{
    float sysReadDRAM = 0.0, sysWriteDRAM = 0.0, sysReadPMM = 0.0, sysWritePMM = 0.0;
    pcm::uint32 numSockets = m->getNumSockets();
    pcm::uint32 skt = 0;
    std::cout.setf(std::ios::fixed);
    std::cout.precision(2);

    if (print_update)
        pcm::clear_screen();

    while (skt < numSockets)
    {
        auto printRow = [&skt,&show_channel_output,&m,&md,&sysReadDRAM,&sysWriteDRAM, &sysReadPMM, &sysWritePMM](const pcm::uint32 no_columns)
        {
            printSocketBWHeader(no_columns, skt, show_channel_output);
            if (show_channel_output)
                printSocketChannelBW(m, md, no_columns, skt);
            printSocketBWFooter(no_columns, skt, md);
            for (pcm::uint32 i = skt; i < (skt + no_columns); i++)
            {
                sysReadDRAM += md->iMC_Rd_socket[i];
                sysWriteDRAM += md->iMC_Wr_socket[i];
                sysReadPMM += md->iMC_PMM_Rd_socket[i];
                sysWritePMM += md->iMC_PMM_Wr_socket[i];
            }
            skt += no_columns;
        };
        // Full row
        if ((skt + no_columns) <= numSockets)
        {
            printRow(no_columns);
        }
        else //Display the remaining sockets in this row
        {
            if (m->MCDRAMmemoryTrafficMetricsAvailable() == false)
            {
                printRow(numSockets - skt);
            }
            else
            {
                std::cout << "\
                    \r|---------------------------------------||---------------------------------------|\n\
                    \r|--                              Processor socket "
                          << skt << "                            --|\n\
                    \r|---------------------------------------||---------------------------------------|\n\
                    \r|--       DDR4 Channel Monitoring     --||--      MCDRAM Channel Monitoring    --|\n\
                    \r|---------------------------------------||---------------------------------------|\n\
                    \r";
                const pcm::uint32 max_channels = (std::max)(max_edc_channels, max_imc_channels);
                if (show_channel_output)
                {
                    float iMC_Rd, iMC_Wr, EDC_Rd, EDC_Wr;
                    for (pcm::uint64 channel = 0; channel < max_channels; ++channel)
                    {
                        if (channel < max_imc_channels)
                        {
                            iMC_Rd = md->iMC_Rd_socket_chan[skt][channel];
                            iMC_Wr = md->iMC_Wr_socket_chan[skt][channel];
                        }
                        else
                        {
                            iMC_Rd = -1.0;
                            iMC_Wr = -1.0;
                        }
                        if (channel < max_edc_channels)
                        {
                            EDC_Rd = md->EDC_Rd_socket_chan[skt][channel];
                            EDC_Wr = md->EDC_Wr_socket_chan[skt][channel];
                        }
                        else
                        {
                            EDC_Rd = -1.0;
                            EDC_Wr = -1.0;
                        }

                        if (iMC_Rd >= 0.0 && iMC_Wr >= 0.0 && EDC_Rd >= 0.0 && EDC_Wr >= 0.0)
                            std::cout << "|-- DDR4 Ch " << channel << ": Reads (MB/s):" << std::setw(9) << iMC_Rd
                                      << " --||-- EDC Ch " << channel << ": Reads (MB/s):" << std::setw(10) << EDC_Rd
                                      << " --|\n|--            Writes(MB/s):" << std::setw(9) << iMC_Wr
                                      << " --||--           Writes(MB/s):" << std::setw(10) << EDC_Wr
                                      << " --|\n";
                        else if ((iMC_Rd < 0.0 || iMC_Wr < 0.0) && EDC_Rd >= 0.0 && EDC_Wr >= 0.0)
                            std::cout << "|--                                  "
                                      << " --||-- EDC Ch " << channel << ": Reads (MB/s):" << std::setw(10) << EDC_Rd
                                      << " --|\n|--                                  "
                                      << " --||--           Writes(MB/s):" << std::setw(10) << EDC_Wr
                                      << " --|\n";

                        else if (iMC_Rd >= 0.0 && iMC_Wr >= 0.0 && (EDC_Rd < 0.0 || EDC_Wr < 0.0))
                            std::cout << "|-- DDR4 Ch " << channel << ": Reads (MB/s):" << std::setw(9) << iMC_Rd
                                      << " --||--                                  "
                                      << " --|\n|--            Writes(MB/s):" << std::setw(9) << iMC_Wr
                                      << " --||--                                  "
                                      << " --|\n";
                        else
                            continue;
                    }
                }
                std::cout << "\
                    \r|-- DDR4 Mem Read  (MB/s):"
                          << std::setw(11) << md->iMC_Rd_socket[skt] << " --||-- MCDRAM Read (MB/s):" << std::setw(14) << md->EDC_Rd_socket[skt] << " --|\n\
                    \r|-- DDR4 Mem Write (MB/s):"
                          << std::setw(11) << md->iMC_Wr_socket[skt] << " --||-- MCDRAM Write(MB/s):" << std::setw(14) << md->EDC_Wr_socket[skt] << " --|\n\
                    \r|-- DDR4 Memory (MB/s)   :"
                          << std::setw(11) << md->iMC_Rd_socket[skt] + md->iMC_Wr_socket[skt] << " --||-- MCDRAM (MB/s)     :" << std::setw(14) << md->EDC_Rd_socket[skt] + md->EDC_Wr_socket[skt] << " --|\n\
                    \r|---------------------------------------||---------------------------------------|\n\
                    \r";

                sysReadDRAM += (md->iMC_Rd_socket[skt] + md->EDC_Rd_socket[skt]);
                sysWriteDRAM += (md->iMC_Wr_socket[skt] + md->EDC_Wr_socket[skt]);
                skt += 1;
            }
        }
    }
    {
        std::cout << "\
            \r|---------------------------------------||---------------------------------------|\n";
        if (anyPmem(md->metrics))
        {
            std::cout << "\
            \r|--            System DRAM Read Throughput(MB/s):" << std::setw(14) << sysReadDRAM <<                                     "                --|\n\
            \r|--           System DRAM Write Throughput(MB/s):" << std::setw(14) << sysWriteDRAM <<                                    "                --|\n\
            \r|--             System PMM Read Throughput(MB/s):" << std::setw(14) << sysReadPMM <<                                      "                --|\n\
            \r|--            System PMM Write Throughput(MB/s):" << std::setw(14) << sysWritePMM <<                                     "                --|\n";
        }
        std::cout << "\
            \r|--                 System Read Throughput(MB/s):" << std::setw(14) << sysReadDRAM+sysReadPMM <<                          "                --|\n\
            \r|--                System Write Throughput(MB/s):" << std::setw(14) << sysWriteDRAM+sysWritePMM <<                        "                --|\n\
            \r|--               System Memory Throughput(MB/s):" << std::setw(14) << sysReadDRAM+sysReadPMM+sysWriteDRAM+sysWritePMM << "                --|\n\
            \r|---------------------------------------||---------------------------------------|\n";
    }
}

void display_bandwidth_csv(pcm::PCM *m, memdata_t *md, pcm::uint64 /*elapsedTime*/, const bool show_channel_output, const pcm::CsvOutputType outputType)
{
    const pcm::uint32 numSockets = m->getNumSockets();
    printDateForCSV(outputType);

    float sysReadDRAM = 0.0, sysWriteDRAM = 0.0, sysReadPMM = 0.0, sysWritePMM = 0.0;

    for (pcm::uint32 skt = 0; skt < numSockets; ++skt)
    {
        auto printSKT = [skt](int c = 1) {
            for (int i = 0; i < c; ++i)
                std::cout << "SKT" << skt << ',';
        };
        if (show_channel_output)
        {
            for (pcm::uint64 channel = 0; channel < max_imc_channels; ++channel)
            {
                if (md->iMC_Rd_socket_chan[skt][channel] < 0.0 && md->iMC_Wr_socket_chan[skt][channel] < 0.0) //If the channel read neg. value, the channel is not working; skip it.
                    continue;

                choose(outputType,
                       [printSKT]() {
                           printSKT(2);
                       },
                       [&channel]() {
                           std::cout << "Ch" << channel << "Read,"
                                     << "Ch" << channel << "Write,";
                       },
                       [&md, &skt, &channel]() {
                           std::cout << std::setw(8) << md->iMC_Rd_socket_chan[skt][channel] << ','
                                     << std::setw(8) << md->iMC_Wr_socket_chan[skt][channel] << ',';
                       });

                if (md->metrics == pcm::Pmem)
                {
                    choose(outputType,
                           [printSKT]() {
                               printSKT(2);
                           },
                           [&channel]() {
                               std::cout << "Ch" << channel << "PMM_Read,"
                                         << "Ch" << channel << "PMM_Write,";
                           },
                           [&skt, &md, &channel]() {
                               std::cout << std::setw(8) << md->iMC_PMM_Rd_socket_chan[skt][channel] << ','
                                         << std::setw(8) << md->iMC_PMM_Wr_socket_chan[skt][channel] << ',';
                           });
                }
            }
        }
        choose(outputType,
               [printSKT]() {
                   printSKT(2);
               },
               []() {
                   std::cout << "Mem Read (MB/s),Mem Write (MB/s),";
               },
               [&md, &skt]() {
                   std::cout << std::setw(8) << md->iMC_Rd_socket[skt] << ','
                             << std::setw(8) << md->iMC_Wr_socket[skt] << ',';
               });

        if (anyPmem(md->metrics))
        {
            choose(outputType,
                   [printSKT]() {
                       printSKT(2);
                   },
                   []() {
                       std::cout << "PMM_Read (MB/s), PMM_Write (MB/s),";
                   },
                   [&md, &skt]() {
                       std::cout << std::setw(8) << md->iMC_PMM_Rd_socket[skt] << ','
                                 << std::setw(8) << md->iMC_PMM_Wr_socket[skt] << ',';
                   });
        }
        if (md->metrics == pcm::PmemMemoryMode && md->iMC_NM_hit_rate_supported)
        {
            choose(outputType,
                   [printSKT]() {
                       printSKT(3);
                   },
                   []() {
                       std::cout << "NM hit rate,";
                       std::cout << "NM hits (M/s),";
                       std::cout << "NM misses (M/s),";
                   },
                   [&md, &skt]() {
                       std::cout << std::setw(8) << md->iMC_NM_hit_rate[skt]<< ',';
                       std::cout << std::setw(8) << md->iMC_PMM_MemoryMode_Hit_socket[skt]/1000000. << ',';
                       std::cout << std::setw(8) << md->iMC_PMM_MemoryMode_Miss_socket[skt]/1000000. << ',';
                   });
        }
        if (md->metrics == pcm::Pmem && md->M2M_NM_read_hit_rate_supported)
        {
            for (pcm::uint32 c = 0; c < max_imc_controllers; ++c)
            {
                choose(outputType,
                       [printSKT]() {
                           printSKT();
                       },
                       [c]() {
                           std::cout << "iMC" << c << " NM read hit rate,";
                       },
                       [&md, &skt, c]() {
                           std::cout << std::setw(8) << md->M2M_NM_read_hit_rate[skt][c] << ',';
                       });
            }
        }
        if (md->metrics == pcm::PmemMixedMode)
        {
            choose(outputType,
                   [printSKT]() {
                       printSKT(3);
                   },
                   []() {
                       std::cout << "PMM_AD (MB/s), PMM_MM (MB/s), PMM_MM_Bw/DRAM_Bw,";
                   },
                   [&md, &skt]() {
                       std::cout << std::setw(8) << AD_BW(md, skt) << ','
                                 << std::setw(8) << md->iMC_PMM_MemoryMode_Miss_socket[skt] << ','
                                 << std::setw(8) << PMM_MM_Ratio(md, skt) << ',';
                   });
        }
        if (m->MCDRAMmemoryTrafficMetricsAvailable() == false)
        {
            if (md->metrics == pcm::PartialWrites)
            {
                choose(outputType,
                       [printSKT]() {
                           printSKT();
                       },
                       []() {
                           std::cout << "P. Write (T/s),";
                       },
                       [&md, &skt]() {
                           std::cout << std::setw(10) << std::dec << md->partial_write[skt] << ',';
                       });
            }
        }

        choose(outputType,
               [printSKT]() {
                   printSKT();
               },
               []() {
                   std::cout << "Memory (MB/s),";
               },
               [&]() {
                   std::cout << std::setw(8) << md->iMC_Rd_socket[skt] + md->iMC_Wr_socket[skt] << ',';

                   sysReadDRAM += md->iMC_Rd_socket[skt];
                   sysWriteDRAM += md->iMC_Wr_socket[skt];
                   sysReadPMM += md->iMC_PMM_Rd_socket[skt];
                   sysWritePMM += md->iMC_PMM_Wr_socket[skt];
               });

        if (m->MCDRAMmemoryTrafficMetricsAvailable())
        {
            if (show_channel_output)
            {
                for (pcm::uint64 channel = 0; channel < max_edc_channels; ++channel)
                {
                    if (md->EDC_Rd_socket_chan[skt][channel] < 0.0 && md->EDC_Wr_socket_chan[skt][channel] < 0.0) //If the channel read neg. value, the channel is not working; skip it.
                        continue;

                    choose(outputType,
                           [printSKT]() {
                               printSKT(2);
                           },
                           [&channel]() {
                               std::cout << "EDC_Ch" << channel << "Read,"
                                         << "EDC_Ch" << channel << "Write,";
                           },
                           [&md, &skt, &channel]() {
                               std::cout << std::setw(8) << md->EDC_Rd_socket_chan[skt][channel] << ','
                                         << std::setw(8) << md->EDC_Wr_socket_chan[skt][channel] << ',';
                           });
                }
            }

            choose(outputType,
                   [printSKT]() {
                       printSKT(3);
                   },
                   []() {
                       std::cout << "MCDRAM Read (MB/s), MCDRAM Write (MB/s), MCDRAM (MB/s),";
                   },
                   [&]() {
                       std::cout << std::setw(8) << md->EDC_Rd_socket[skt] << ','
                                 << std::setw(8) << md->EDC_Wr_socket[skt] << ','
                                 << std::setw(8) << md->EDC_Rd_socket[skt] + md->EDC_Wr_socket[skt] << ',';

                       sysReadDRAM += md->EDC_Rd_socket[skt];
                       sysWriteDRAM += md->EDC_Wr_socket[skt];
                   });
        }
    }

    if (anyPmem(md->metrics))
    {
        choose(outputType,
               []() {
                   std::cout << "System,System,System,System,";
               },
               []() {
                   std::cout << "DRAMRead,DRAMWrite,PMMREAD,PMMWrite,";
               },
               [&]() {
                   std::cout << std::setw(10) << sysReadDRAM << ','
                             << std::setw(10) << sysWriteDRAM << ','
                             << std::setw(10) << sysReadPMM << ','
                             << std::setw(10) << sysWritePMM << ',';
               });
    }

    choose(outputType,
           []() {
               std::cout << "System,System,System\n";
           },
           []() {
               std::cout << "Read,Write,Memory\n";
           },
           [&]() {
               std::cout << std::setw(10) << sysReadDRAM + sysReadPMM << ','
                         << std::setw(10) << sysWriteDRAM + sysWritePMM << ','
                         << std::setw(10) << sysReadDRAM + sysReadPMM + sysWriteDRAM + sysWritePMM << "\n";
           });
}


void calculate_bandwidth(pcm::PCM *m,
                         const pcm::ServerUncoreCounterState uncState1[],
                         const pcm::ServerUncoreCounterState uncState2[],
                         const pcm::uint64 elapsedTime,
                         const bool csv,
                         bool & csvheader,
                         pcm::uint32 no_columns,
                         const pcm::ServerUncoreMemoryMetrics & metrics,
                         const bool show_channel_output,
                         const bool print_update)
{
    //const pcm::uint32 num_imc_channels = m->getMCChannelsPerSocket();
    //const pcm::uint32 num_edc_channels = m->getEDCChannelsPerSocket();
    memdata_t md;
    md.metrics = metrics;
    md.M2M_NM_read_hit_rate_supported = (m->getCPUModel() == pcm::PCM::SKX);
    md.iMC_NM_hit_rate_supported = (m->getCPUModel() == pcm::PCM::ICX);
    static bool mm_once = true;
    if (metrics == pcm::Pmem && md.M2M_NM_read_hit_rate_supported == false && md.iMC_NM_hit_rate_supported == true && mm_once)
    {
        std::cerr << "INFO: Use -mm option to monitor NM Memory Mode metrics\n";
        mm_once = false;
    }
    static bool mm_once1 = true;
    if (metrics == pcm::PmemMemoryMode && md.M2M_NM_read_hit_rate_supported == true && md.iMC_NM_hit_rate_supported == false && mm_once1)
    {
        std::cerr << "INFO: Use -pmem option to monitor NM Memory Mode metrics\n";
        mm_once1 = false;
    }

    for(pcm::uint32 skt = 0; skt < max_sockets; ++skt)
    {
        md.iMC_Rd_socket[skt] = 0.0;
        md.iMC_Wr_socket[skt] = 0.0;
        md.iMC_PMM_Rd_socket[skt] = 0.0;
        md.iMC_PMM_Wr_socket[skt] = 0.0;
        md.iMC_PMM_MemoryMode_Miss_socket[skt] = 0.0;
        md.iMC_PMM_MemoryMode_Hit_socket[skt] = 0.0;
        md.iMC_NM_hit_rate[skt] = 0.0;
        md.EDC_Rd_socket[skt] = 0.0;
        md.EDC_Wr_socket[skt] = 0.0;
        md.partial_write[skt] = 0;
        for (pcm::uint32 i = 0; i < max_imc_controllers; ++i)
        {
            md.M2M_NM_read_hit_rate[skt][i] = 0.;
        }
    }

    for(pcm::uint32 skt = 0; skt < m->getNumSockets(); ++skt)
    {
        const pcm::uint32 numChannels1 = (pcm::uint32)m->getMCChannels(skt, 0); // number of channels in the first controller

        auto toBW = [&elapsedTime](const pcm::uint64 nEvents)
        {
            return (float)(nEvents * 64 / 1000000.0 / (elapsedTime / 1000.0));
        };

        if (m->MCDRAMmemoryTrafficMetricsAvailable())
        {
            for (pcm::uint32 channel = 0; channel < max_edc_channels; ++channel)
            {
                if (skipInactiveChannels && getEDCCounter(channel, pcm::ServerPCICFGUncore::EventPosition::READ, uncState1[skt], uncState2[skt]) == 0.0 && getEDCCounter(channel, pcm::ServerPCICFGUncore::EventPosition::WRITE, uncState1[skt], uncState2[skt]) == 0.0)
                {
                    md.EDC_Rd_socket_chan[skt][channel] = -1.0;
                    md.EDC_Wr_socket_chan[skt][channel] = -1.0;
                    continue;
                }

                md.EDC_Rd_socket_chan[skt][channel] = toBW(getEDCCounter(channel, pcm::ServerPCICFGUncore::EventPosition::READ, uncState1[skt], uncState2[skt]));
                md.EDC_Wr_socket_chan[skt][channel] = toBW(getEDCCounter(channel, pcm::ServerPCICFGUncore::EventPosition::WRITE, uncState1[skt], uncState2[skt]));

                md.EDC_Rd_socket[skt] += md.EDC_Rd_socket_chan[skt][channel];
                md.EDC_Wr_socket[skt] += md.EDC_Wr_socket_chan[skt][channel];
            }
        }

        {
            for (pcm::uint32 channel = 0; channel < max_imc_channels; ++channel)
            {
                pcm::uint64 reads = 0, writes = 0, pmmReads = 0, pmmWrites = 0, pmmMemoryModeCleanMisses = 0, pmmMemoryModeDirtyMisses = 0;
                pcm::uint64 pmmMemoryModeHits = 0;
                reads = getMCCounter(channel, pcm::ServerPCICFGUncore::EventPosition::READ, uncState1[skt], uncState2[skt]);
                writes = getMCCounter(channel, pcm::ServerPCICFGUncore::EventPosition::WRITE, uncState1[skt], uncState2[skt]);
                if (metrics == pcm::Pmem)
                {
                    pmmReads = getMCCounter(channel, pcm::ServerPCICFGUncore::EventPosition::PMM_READ, uncState1[skt], uncState2[skt]);
                    pmmWrites = getMCCounter(channel, pcm::ServerPCICFGUncore::EventPosition::PMM_WRITE, uncState1[skt], uncState2[skt]);
                }
                else if (metrics == pcm::PmemMixedMode || metrics == pcm::PmemMemoryMode)
                {
                    pmmMemoryModeCleanMisses = getMCCounter(channel, pcm::ServerPCICFGUncore::EventPosition::PMM_MM_MISS_CLEAN, uncState1[skt], uncState2[skt]);
                    pmmMemoryModeDirtyMisses = getMCCounter(channel, pcm::ServerPCICFGUncore::EventPosition::PMM_MM_MISS_DIRTY, uncState1[skt], uncState2[skt]);
                }
                if (metrics == pcm::PmemMemoryMode)
                {
                    pmmMemoryModeHits = getMCCounter(channel, pcm::ServerPCICFGUncore::EventPosition::NM_HIT, uncState1[skt], uncState2[skt]);
                }
                if (skipInactiveChannels && (reads + writes == 0))
                {
                    if ((metrics != pcm::Pmem) || (pmmReads + pmmWrites == 0))
                    {
                        if ((metrics != pcm::PmemMixedMode) || (pmmMemoryModeCleanMisses + pmmMemoryModeDirtyMisses == 0))
                        {

                            md.iMC_Rd_socket_chan[skt][channel] = -1.0;
                            md.iMC_Wr_socket_chan[skt][channel] = -1.0;
                            continue;
                        }
                    }
                }

                if (metrics != pcm::PmemMemoryMode)
                {
                    md.iMC_Rd_socket_chan[skt][channel] = toBW(reads);
                    md.iMC_Wr_socket_chan[skt][channel] = toBW(writes);

                    md.iMC_Rd_socket[skt] += md.iMC_Rd_socket_chan[skt][channel];
                    md.iMC_Wr_socket[skt] += md.iMC_Wr_socket_chan[skt][channel];
                }

                if (metrics == pcm::Pmem)
                {
                    md.iMC_PMM_Rd_socket_chan[skt][channel] = toBW(pmmReads);
                    md.iMC_PMM_Wr_socket_chan[skt][channel] = toBW(pmmWrites);

                    md.iMC_PMM_Rd_socket[skt] += md.iMC_PMM_Rd_socket_chan[skt][channel];
                    md.iMC_PMM_Wr_socket[skt] += md.iMC_PMM_Wr_socket_chan[skt][channel];

                    md.M2M_NM_read_hit_rate[skt][(channel < numChannels1) ? 0 : 1] += (float)reads;
                }
                else if (metrics == pcm::PmemMixedMode)
                {
                    md.iMC_PMM_MemoryMode_Miss_socket_chan[skt][channel] = toBW(pmmMemoryModeCleanMisses + 2 * pmmMemoryModeDirtyMisses);
                    md.iMC_PMM_MemoryMode_Miss_socket[skt] += md.iMC_PMM_MemoryMode_Miss_socket_chan[skt][channel];
                }
                else if (metrics == pcm::PmemMemoryMode)
                {
                    md.iMC_PMM_MemoryMode_Miss_socket[skt] += (float)((pmmMemoryModeCleanMisses + pmmMemoryModeDirtyMisses) / (elapsedTime / 1000.0));
                    md.iMC_PMM_MemoryMode_Hit_socket[skt] += (float)((pmmMemoryModeHits) / (elapsedTime / 1000.0));
                }
                else
                {
                    md.partial_write[skt] += (pcm::uint64)(getMCCounter(channel, pcm::ServerPCICFGUncore::EventPosition::PARTIAL, uncState1[skt], uncState2[skt]) / (elapsedTime / 1000.0));
                }
            }
        }
        if (metrics == pcm::PmemMemoryMode)
        {
            md.iMC_Rd_socket[skt] += toBW(getFreeRunningCounter(pcm::ServerUncoreCounterState::ImcReads, uncState1[skt], uncState2[skt]));
            md.iMC_Wr_socket[skt] += toBW(getFreeRunningCounter(pcm::ServerUncoreCounterState::ImcWrites, uncState1[skt], uncState2[skt]));
        }
        if (metrics == pcm::PmemMixedMode || metrics == pcm::PmemMemoryMode)
        {
            const pcm::int64 pmmReads = getFreeRunningCounter(pcm::ServerUncoreCounterState::PMMReads, uncState1[skt], uncState2[skt]);
            if (pmmReads >= 0)
            {
                md.iMC_PMM_Rd_socket[skt] += toBW(pmmReads);
            }
            else for(pcm::uint32 c = 0; c < max_imc_controllers; ++c)
                {
                    md.iMC_PMM_Rd_socket[skt] += toBW(getM2MCounter(c, pcm::ServerPCICFGUncore::EventPosition::PMM_READ, uncState1[skt],uncState2[skt]));
                }

            const pcm::int64 pmmWrites = getFreeRunningCounter(pcm::ServerUncoreCounterState::PMMWrites, uncState1[skt], uncState2[skt]);
            if (pmmWrites >= 0)
            {
                md.iMC_PMM_Wr_socket[skt] += toBW(pmmWrites);
            }
            else for(pcm::uint32 c = 0; c < max_imc_controllers; ++c)
                {
                    md.iMC_PMM_Wr_socket[skt] += toBW(getM2MCounter(c, pcm::ServerPCICFGUncore::EventPosition::PMM_WRITE, uncState1[skt],uncState2[skt]));;
                }
        }
        if (metrics == pcm::Pmem)
        {
            for(pcm::uint32 c = 0; c < max_imc_controllers; ++c)
            {
                if(md.M2M_NM_read_hit_rate[skt][c] != 0.0)
                {
                    md.M2M_NM_read_hit_rate[skt][c] = ((float)getM2MCounter(c, pcm::ServerPCICFGUncore::EventPosition::NM_HIT, uncState1[skt],uncState2[skt]))/ md.M2M_NM_read_hit_rate[skt][c];
                }
            }
        }
        const auto all = md.iMC_PMM_MemoryMode_Miss_socket[skt] + md.iMC_PMM_MemoryMode_Hit_socket[skt];
        if (metrics == pcm::PmemMemoryMode && all != 0.0)
        {
            md.iMC_NM_hit_rate[skt] = md.iMC_PMM_MemoryMode_Hit_socket[skt] / all;
        }
    }

    if (csv)
    {
        if (csvheader)
        {
            display_bandwidth_csv(m, &md, elapsedTime, show_channel_output, pcm::Header1);
            display_bandwidth_csv(m, &md, elapsedTime, show_channel_output, pcm::Header2);
            csvheader = false;
        }
        display_bandwidth_csv(m, &md, elapsedTime, show_channel_output, pcm::Data);
    }
    else
    {
        display_bandwidth(m, &md, no_columns, show_channel_output, print_update);
    }
}

void calculate_bandwidth_rank(pcm::PCM *m, const pcm::ServerUncoreCounterState uncState1[], const pcm::ServerUncoreCounterState uncState2[],
                              const pcm::uint64 elapsedTime, const bool csv, bool &csvheader, const pcm::uint32 no_columns, const int rankA, const int rankB)
{
    pcm::uint32 skt = 0;
    std::cout.setf(std::ios::fixed);
    std::cout.precision(2);
    pcm::uint32 numSockets = m->getNumSockets();

    if (csv) {
        if (csvheader) {
            printSocketRankBWHeader_cvt(numSockets, max_imc_channels, rankA, rankB);
            csvheader = false;
        }
        printSocketChannelBW_cvt(numSockets, max_imc_channels, uncState1, uncState2, elapsedTime, rankA, rankB);
    } else {
        while(skt < numSockets) {
            auto printRow = [&skt, &uncState1, &uncState2, &elapsedTime, &rankA, &rankB](const pcm::uint32 no_columns) {
                printSocketRankBWHeader(no_columns, skt);
                printSocketChannelBW(no_columns, skt, max_imc_channels, uncState1, uncState2, elapsedTime, rankA, rankB);
                for (pcm::uint32 i = skt; i < (no_columns + skt); ++i)
                    std::cout << "|-------------------------------------------|";
                std::cout << "\n";
                skt += no_columns;
            };
            // Full row
            if ((skt + no_columns) <= numSockets)
                printRow(no_columns);
            else //Display the remaining sockets in this row
                printRow(numSockets - skt);
        }
    }
}

