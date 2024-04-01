#!/bin/bash

GEM5_DIR=/gem5
DNDIR=$GEM5_DIR/darknet
SCRIPT_DIR=$GEM5_DIR/scripts
run_mode=${1:-"pipe"}
main_program=${2:-"winomain"}



cd $DNDIR
echo $PWD
make EXEC=$main_program -j17
./obj_scripts.sh
cd $SCRIPT_DIR
#在c源码中添加reset查看起始时间
#当mode是0时 跑0号脚本，测时间(run_mode在该脚本中没用)
#然后删去reset进行分析
#否则直接省略，跑两遍1号脚本，得到两种trace
if [ "$run_mode" = "0" ]; then
    ./run_darknet.sh $run_mode $main_program
else
    {
        ./run_darknet1.sh e $main_program
        ./run_darknet1.sh $run_mode $main_program
    }
fi

