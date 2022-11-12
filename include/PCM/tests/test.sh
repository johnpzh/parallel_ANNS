modprobe msr

export BIN_DIR="build/bin"

pushd $BIN_DIR

echo Enable NMI watchdog
echo 1 > /proc/sys/kernel/nmi_watchdog

echo Testing pcm with PCM_NO_PERF=1
PCM_NO_PERF=1 ./pcm -r -- sleep 1
if [ "$?" -ne "0" ]; then
   echo "Error in pcm"
   exit 1
fi

echo Testing pcm with PCM_USE_UNCORE_PERF=1
PCM_USE_UNCORE_PERF=1 ./pcm -r -- sleep 1
if [ "$?" -ne "0" ]; then
   echo "Error in pcm"
   exit 1
fi

echo Testing pcm w/o env vars
./pcm -r -- sleep 1
if [ "$?" -ne "0" ]; then
   echo "Error in pcm"
   exit 1
fi

echo Testing pcm with -pid
perl -e ' do {} until (0)' &
test_pid="$!"
./pcm -pid $test_pid -- sleep 1
if [ "$?" -ne "0" ]; then
   echo "Error in pcm"
   kill $perl_pid
   exit 1
fi
kill $test_pid

echo Testing pcm with PCM_KEEP_NMI_WATCHDOG=1
PCM_KEEP_NMI_WATCHDOG=1 ./pcm -r -- sleep 1
if [ "$?" -ne "0" ]; then
   echo "Error in pcm"
   exit 1
fi

echo Testing pcm with -csv
./pcm -r 0.1 -csv=pcm.csv -- sleep 5
if [ "$?" -ne "0" ]; then
   echo "Error in pcm"
   exit 1
fi

echo Testing pcm-memory
./pcm-memory -- sleep 1
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-memory"
    exit 1
fi

echo Testing pcm-memory with -rank
./pcm-memory -rank=1 -- sleep 1
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-memory"
    exit 1
fi

echo Testing pcm-memory with -rank and -csv
./pcm-memory -rank=1 -csv -- sleep 1
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-memory"
    exit 1
fi

echo Testing pcm-iio
./pcm-iio -i=1
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-iio"
    exit 1
fi

echo Testing pcm-raw
./pcm-raw -e core/config=0x30203,name=LD_BLOCKS.STORE_FORWARD/ -e cha/config=0,name=UNC_CHA_CLOCKTICKS/ -e imc/fixed,name=DRAM_CLOCKS -e thread_msr/config=0x10,config1=1 -e thread_msr/config=0x19c,config1=0 -- sleep 1
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-raw"
    exit 1
fi

echo Testing pcm-mmio
./pcm-mmio 0x0
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-mmio"
    exit 1
fi

echo Testing pcm-pcicfg
./pcm-pcicfg 0 0 0 0 0
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-pcicfg"
    exit 1
fi

echo Testing pcm-numa
./pcm-numa -- sleep 1
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-numa"
    exit 1
fi

echo Testing pcm-core
./pcm-core -e cpu/umask=0x01,event=0x0e,name=UOPS_ISSUED.STALL_CYCLES/ -- sleep 1
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-core"
    exit 1
fi

echo Testing c_example
./examples/c_example
if [ "$?" -ne "0" ]; then
    echo "Error in c_example"
    exit 1
fi

echo Testing c_example_shlib
./examples/c_example_shlib
if [ "$?" -ne "0" ]; then
    echo "Error in c_example_shlib"
    exit 1
fi

echo Testing pcm-msr \(read only\)
./pcm-msr -a 0x30A
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-msr"
    exit 1
fi

echo Testing pcm-power
./pcm-power -- sleep 1
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-power"
    exit 1
fi

echo Testing pcm-pcie
./pcm-pcie -- sleep 1
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-pcie"
    exit 1
fi

echo Testing pcm-latency
./pcm-latency -i=1
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-latency"
    exit 1
fi

echo Testing pcm-tsx
./pcm-tsx -- sleep 1
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-tsx"
    exit 1
fi

# TODO add more tests
# e.g for ./pcm-sensor-server, ./pcm-sensor, ...

echo Testing urltest
./tests/urltest
# We have 2 expected errors, anything else is a bug
if [ "$?" != 2 ]; then
    echo "Error in urltest, 2 expected errors but found $?!"
    exit 1
fi

echo Testing pcm-raw with event files
echo   Download necessary files
if [ ! -f "mapfile.csv" ]; then
    echo "Downloading https://download.01.org/perfmon/mapfile.csv"
    wget -q --timeout=10 https://download.01.org/perfmon/mapfile.csv
    if [ "$?" -ne "0" ]; then
        echo "Could not download mapfile.csv"
        exit 1
    fi
fi

VENDOR=$(lscpu | grep "Vendor ID:" | awk '{print $3}')
FAMILY=$(lscpu | grep "CPU family:" | awk '{print $3}')
MODEL=$(lscpu | grep "Model:" | awk '{printf("%x", $2)}')
STRING="${VENDOR}-${FAMILY}-${MODEL}-"
FILES=$(grep $STRING "mapfile.csv" | awk -F "\"*,\"*" '{print $3}')
DIRS=

for FILE in $FILES
do
    DIR="$(dirname $FILE)"
    DIR="${DIR#?}"
    if [[ ! " ${DIRS[*]} " =~ " ${DIR} " ]]; then
        DIRS+="${DIR} "
    fi
done

for DIR in $DIRS
do
    if [ ! -d $DIR ]; then
        mkdir $DIR
        cd $DIR

        DIRPATH="https://download.01.org/perfmon/${DIR}/"
        echo "Downloading all files from ${DIRPATH}"

        wget -q --timeout=10 -r -l1 --no-parent -A "*.json" $DIRPATH
        if [ "$?" -ne "0" ]; then
            cd ..
            echo "Could not download $DIR"
            exit 1
        fi
        wget -q --timeout=10 -r -l1 --no-parent -A "*.tsv" $DIRPATH
        mv download.01.org/perfmon/${DIR}/* .
        rm -rf download.01.org
        cd ..
    fi
done

echo   Now check pcm-raw with JSON files from mapFile.csv
./pcm-raw -r -e LD_BLOCKS.STORE_FORWARD -e CPU_CLK_UNHALTED.THREAD_ANY -e INST_RETIRED.ANY -e UNC_CHA_CLOCKTICKS -- sleep 1

if [ "$?" -ne "0" ]; then
    echo "Error in pcm-raw"
    exit 1
fi

echo   Now get corresponding TSV files and replace JSON files in mapFile.csv with them
cp "mapfile.csv" "mapfile.csv_orig"
for FILE in $FILES
do
    DIR="$(dirname $FILE)"
    DIR="${DIR#?}"
    cd $DIR
    BASE="$(basename $FILE)"
    TYPE="$(echo $BASE | sed 's/_v[0-9].*json//g')"
    # TYPE can be for example: skylakex_core or skylakex_uncore.
    CMD="find . -type f -regex '\.\/${TYPE}_v[0-9]*\.[0-9]*.tsv'"
    TSVFILE=$(eval $CMD)
    TSVFILE="${TSVFILE:2}"
    cd ..
    CMD="sed -i 's/${BASE}/${TSVFILE}/g' mapfile.csv"
    eval $CMD
done


# echo Test pcm-raw with TSV files
#./pcm-raw -r -e LD_BLOCKS.STORE_FORWARD -e CPU_CLK_UNHALTED.THREAD_ANY -e INST_RETIRED.ANY -e UNC_CHA_CLOCKTICKS -- sleep 1

#if [ "$?" -ne "0" ]; then
#    echo "Error in pcm-raw"
#    rm -rf mapfile.csv
#    cp "mapfile.csv_orig" "mapfile.csv"
#    exit 1
#fi
rm -rf mapfile.csv
cp "mapfile.csv_orig" "mapfile.csv"


if [ ! -f "event_file_test.txt" ]; then
    cat <<EOF > event_file_test.txt
# group 1
INST_RETIRED.ANY
CPU_CLK_UNHALTED.REF_TSC
UNC_CHA_DIR_LOOKUP.SNP
UNC_CHA_DIR_LOOKUP.NO_SNP
UNC_M_CAS_COUNT.RD
UNC_M_CAS_COUNT.WR
UNC_UPI_CLOCKTICKS
UNC_UPI_TxL_FLITS.ALL_DATA
UNC_UPI_TxL_FLITS.NON_DATA
UNC_UPI_L1_POWER_CYCLES
MSR_EVENT:msr=0x19C:type=STATIC:scope=THREAD
MSR_EVENT:msr=0x1A2:type=STATIC:scope=THREAD
MSR_EVENT:msr=0x34:type=FREERUN:scope=PACKAGE
MSR_EVENT:msr=0x34:type=static:scope=PACKAGE
package_msr/config=0x34,config1=0
thread_msr/config=0x10,config1=1,name=TSC_DELTA
thread_msr/config=0x10,config1=0,name=TSC
;
# group 2
OFFCORE_REQUESTS_BUFFER.SQ_FULL
UNC_CHA_DIR_UPDATE.HA
UNC_CHA_DIR_UPDATE.TOR
UNC_M2M_DIRECTORY_UPDATE.ANY
UNC_M_CAS_COUNT.RD
UNC_M_CAS_COUNT.WR
imc/fixed,name=DRAM_CLOCKS
UNC_M_PRE_COUNT.PAGE_MISS
UNC_UPI_TxL0P_POWER_CYCLES
UNC_UPI_RxL0P_POWER_CYCLES
UNC_UPI_RxL_FLITS.ALL_DATA
UNC_UPI_RxL_FLITS.NON_DATA
MSR_EVENT:msr=0x10:type=FREERUN:scope=thread
MSR_EVENT:msr=0x10:type=static:scope=thread
;
EOF

fi

echo Testing pcm-raw with -el event_file_test.txt -tr -csv
./pcm-raw -el event_file_test.txt -tr -csv=raw_tr_wo_ext.csv -i=4 0.25
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-raw"
    exit 1
fi

echo Testing pcm-raw with -el event_file_test.txt -tr -ext -csv
./pcm-raw -el event_file_test.txt -tr -ext -csv=raw_tr_wi_ext.csv -i=4 0.25
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-raw"
    exit 1
fi

echo Testing pcm-raw with -el event_file_test.txt -tr -ext -single-header -csv
./pcm-raw -el event_file_test.txt -tr -ext -single-header -csv=raw_tr_wi_ext_single_header.csv -i=4 0.25
if [ "$?" -ne "0" ]; then
    echo "Error in pcm-raw"
    exit 1
fi

popd
