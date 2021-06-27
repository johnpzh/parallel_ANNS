# PANNS
Parallel Approximate Nearest Neighbor Search

# Environment


- Compiler: Recommend Intel C++ Compiler (`icpc`). Sometimes `gcc 7.x` also works.
- Requires CMake ≥ 3.9 .
- Requires Python 3.x for `.py` scripts.
- Requires boost for C++ to support `boost::dynamic_bitset`. Might use this command to install boost.
```bash
$ sudo apt install libboost-dev
```
- Requires OpenMP.
- Requires `bash` or `zsh` for those `.sh` scripts.
- `KMP_AFFINITY` right now is set in the `test51.PSS_v5_dt_profiling_ranged_L.py` and `test48.find_L_simple_search.py`. Its value might need to be **modified** according to the running machine.

# Prepare


Right now, the dataset's relative path and file names are hard code.

- **DEEP1B dataset** should be put under `/path/to/some/data/directory/deep1b`, and only the `deep1b` is **necessary**.
- Under the `deep1b/` folder, there should be `deep1B_base.fvecs`, `deep1B.nsg`, `deep1B_query.fvecs`, and `deep1B.true-100_NN.v2.binary`.

# Build


In the repository's root directory, use `cmake` to build.

```bash
$ mkdir cmake-build-release
$ cd cmake-build-release
$ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=icc -DCMAKE_CXX_COMPILER=icpc -DDEBUG_PRINT=off
$ make -j
```

# Run


After building is finished, under directory `cmake-build-release/`, use the following command to run the shell script `sh70.PSS_v5_dt_profiling_deep1b.sh` to test the **Parallel Search**'s performance. Please note to use in the command the directory `/path/to/some/data/directory`, not `/path/to/some/data/directory/deep1b`. And the number of threads (`num_t`) is set in the script. In default, it will run 1 to 64 threads.

```bash
$ bash ../scripts/bg7/sh70.PSS_v5_dt_profiling_deep1b.sh /path/to/some/data/directory deep1b 
```

Similarly, use the following command to run the shell script `sh74.NSG_seq_deep1b.sh` to test **NSG** sequential search's performance.

```bash
$ bash ../scripts/bg7/sh74.NSG_seq_deep1b.sh /path/to/some/data/directory deep1b
```

Results will be printed into text files `output.xxx_xxx_collected.xxx.txt` under current directory `cmake-build-release/`.

- Parallel Search's results are in `output.deep1b.PSS_v5_dt_Txx_collected.selected_0.txt` where `xx` is the number of threads. The 2nd, 3rd, and 4th columns are **computation**, **accuracy@100**, and **latency** in millisecond, respectively.
- NSG sequential search's results is in `output.deep1b.NSG_SEQ_find_L_collected.table.txt`. The 3rd, 4th, and 5th columns are **computation**, **accuracy@100**, and **latency** in millisecond, respectively.