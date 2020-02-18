#! /bin/bash

cd ../cmake-build-release || exit

max_threads=40

echo "---- SIFT ----"
for ((num_threads = 1; num_threads <= max_threads; num_threads *= 2)); do
    python3 ../scripts/module_run.py ./profile_para_single_query_top_m_search sift 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary ${num_threads}
done

echo "---- GIST ----"
for ((num_threads = 1; num_threads <= max_threads; num_threads *= 2)); do
    python3 ../scripts/module_run.py ./profile_para_single_query_top_m_search gist 128 /scratch/zpeng/gist1m/gist.true-100_NN.q-1000.binary ${num_threads}
done

echo "---- DEEP10M ----"
for ((num_threads = 1; num_threads <= max_threads; num_threads *= 2)); do
    python3 ../scripts/module_run.py ./profile_para_single_query_top_m_search deep10M 128 /scratch/zpeng/deep1b/deep10M.true-100_NN.q-10000.binary ${num_threads}
done
