#!/usr/local/bin/zsh
scpt_path="../scripts/knl"
for i in {39..53}; do
    zsh ./${scpt_path}/sh${i}*.sh
done