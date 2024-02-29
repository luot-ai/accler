#!/bin/bash

############ DIRECTORY VARIABLES: MODIFY ACCORDINGLY #############
GEM5_DIR=/gem5                     # Install location of gem5

DNET_DIR=$GEM5_DIR/darknet          # location of darknet
CFG_DIR=$DNET_DIR/cfg
DATA_DIR=$DNET_DIR/data
WEIGHT_DIR=$DNET_DIR/weights

RUN_DIR=$DNET_DIR
OUTPUT_DIR=$DNET_DIR/output
SCRIPT_OUT=$OUTPUT_DIR/runscript.log                                                                    # File log for this script's stdout henceforth
TESTBENCH=$1
################## REPORT SCRIPT CONFIGURATION ###################

echo "Command line:"                                | tee $SCRIPT_OUT
echo "$0 $*"                                        | tee -a $SCRIPT_OUT
echo "================= Hardcoded directories ==================" | tee -a $SCRIPT_OUT
echo "GEM5_DIR:                                     $GEM5_DIR" | tee -a $SCRIPT_OUT
echo "DNET_DIR:                                     $DNET_DIR" | tee -a $SCRIPT_OUT
echo "==========================================================" | tee -a $SCRIPT_OUT
##################################################################


#################### LAUNCH GEM5 SIMULATION ######################
echo ""
echo "Changing to DNET benchmark runtime directory: $RUN_DIR" | tee -a $SCRIPT_OUT
cd $RUN_DIR
rm -rf m5out
rm -f baseout

echo "" | tee -a $SCRIPT_OUT
echo "" | tee -a $SCRIPT_OUT
echo "--------- Starting gem5! ------------" | tee -a $SCRIPT_OUT
echo "" | tee -a $SCRIPT_OUT
echo "" | tee -a $SCRIPT_OUT


BOOM_FLAGS="--cpu-type=RiscvO3CPU \
--bp-type=BiModeBP \
--caches \
--l2cache \
--cacheline=64 \
--num-l2cache=1 \
--l1i_size=16kB \
--l1i_assoc=4 \
--l1d_size=16kB \
--l1d_assoc=4 \
--l2_size=256kB \
--l2_assoc=4 \
--mem-size=8GB \
--warmup-insts=10000000 \
-m 799159000 \
-I 100000000000 "



# Actually launch gem5!
#O3PipeView,O3CPUAll
$GEM5_DIR/build/RISCV/gem5.opt \
--outdir=$OUTPUT_DIR \
--debug-flags=O3PipeView,O3CPUAll \
--debug-file=trace.out \
--debug-start=796361500 \
$GEM5_DIR/configs/deprecated/example/dknet.py \
$BOOM_FLAGS \
--testbench=$TESTBENCH \
--mode='detect' \
--netConfig=$CFG_DIR/yolov3-spp.cfg \
--weight=$WEIGHT_DIR/yolov3-spp.weights \
--inputFeature=$DATA_DIR/dog.jpg \
--benchmark_stdout=$TESTBENCH.out \
--benchmark_stderr=$TESTBENCH.err  | tee -a $SCRIPT_OUT
