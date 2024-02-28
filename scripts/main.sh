#!/bin/bash

GEM5_DIR=/gem5
DNDIR=$GEM5_DIR/darknet
SCRIPT_DIR=$GEM5_DIR/scripts
main_program=${1:-"main"}

cd $DNDIR
echo $PWD
make EXEC=$main_program -j17
./obj_scripts.sh
cd $SCRIPT_DIR
./run_darknet.sh $main_program
