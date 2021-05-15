
## SIFT1M
shnum=91
data=sift1m
output_tag="NSG_omp_bdw_${data}_T64"
pcm-memory.x -- zsh ../scripts/knl/sh${shnum}.NSG_omp_${data}_for_bdw.sh 2>&1 | tee output.${output_tag}.txt

## GIST1M
shnum=92
data=gist1m
output_tag="NSG_omp_bdw_${data}_T64"
pcm-memory.x -- zsh ../scripts/knl/sh${shnum}.NSG_omp_${data}_for_bdw.sh 2>&1 | tee output.${output_tag}.txt

## DEEP10M
shnum=93
data=deep10m
output_tag="NSG_omp_bdw_${data}_T64"
pcm-memory.x -- zsh ../scripts/knl/sh${shnum}.NSG_omp_${data}_for_bdw.sh 2>&1 | tee output.${output_tag}.txt

## SIFT100M
shnum=94
data=sift100m
output_tag="NSG_omp_bdw_${data}_T64"
pcm-memory.x -- zsh ../scripts/knl/sh${shnum}.NSG_omp_${data}_for_bdw.sh 2>&1 | tee output.${output_tag}.txt

## DEEP100M
shnum=95
data=deep100m
output_tag="NSG_omp_bdw_${data}_T64"
pcm-memory.x -- zsh ../scripts/knl/sh${shnum}.NSG_omp_${data}_for_bdw.sh 2>&1 | tee output.${output_tag}.txt