#! python3
import subprocess

sift_base = "/scratch/zpeng/sift1m/sift_base.fvecs"
sift_query = "/scratch/zpeng/sift1m/sift_query.fvecs"
sift_nsg = "/scratch/zpeng/sift1m/sift.nsg"
sift_true_nn = "/scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary"

gist_base = "/scratch/zpeng/gist1m/gist_base.fvecs"
gist_query = "/scratch/zpeng/gist1m/gist_query.fvecs"
gist_nsg = "/scratch/zpeng/gist1m/gist.nsg"
gist_true_nn = "/scratch/zpeng/gist1m/gist.true-100_NN.q-1000.binary"

deep10M_base = "/scratch/zpeng/deep1b/deep10M_base.fvecs"
deep10M_query = "/scratch/zpeng/deep1b/deep10M_query.fvecs"
deep10M_nsg = "/scratch/zpeng/deep1b/deep10M.nsg"
deep10M_true_nn = "/scratch/zpeng/deep1b/deep10M.true-100_NN.q-10000.binary"

sift_values = [
    [1, 200],
    [2, 200],
    [4, 198],
    [8, 196],
    [16, 191],
    [32, 181],
    [64, 155],
    [128, 114],
    [256, 114]
]

gist_values = [
    [1, 400],
    [2, 400],
    [4, 400],
    [8, 399],
    [16, 394],
    [32, 385],
    [64, 368],
    [128, 314],
    [256, 227],
    [512, 227]
]

deep10M_values = [
    [1, 400],
    [2, 400],
    [4, 398],
    [8, 395],
    [16, 389],
    [32, 380],
    [64, 361],
    [128, 309],
    [256, 228],
    [512, 228]
]

print("---- SIFT1M ----", flush=True)
for value in sift_values:
    value_M = value[0]
    value_L = value[1]
    command = ["./profile_top_m_batch_search_value_m",
               sift_base,
               sift_query,
               sift_nsg,
               str(value_L),
               "200",
               "output.ivecs",
               str(value_M),
               "256",
               sift_true_nn,
               "1"]
    subprocess.run(' '.join(command), shell=True, check=True)

print("---- GIST1M ----", flush=True)
for value in gist_values:
    value_M = value[0]
    value_L = value[1]
    command = ["./profile_top_m_batch_search_value_m",
               gist_base,
               gist_query,
               gist_nsg,
               str(value_L),
               "400",
               "output.ivecs",
               str(value_M),
               "512",
               gist_true_nn,
               "1"]
    subprocess.run(' '.join(command), shell=True, check=True)

print("---- DEEP10M ----", flush=True)
for value in deep10M_values:
    value_M = value[0]
    value_L = value[1]
    command = ["./profile_top_m_batch_search_value_m",
               deep10M_base,
               deep10M_query,
               deep10M_nsg,
               str(value_L),
               "400",
               "output.ivecs",
               str(value_M),
               "512",
               deep10M_true_nn,
               "1"]
    subprocess.run(' '.join(command), shell=True, check=True)