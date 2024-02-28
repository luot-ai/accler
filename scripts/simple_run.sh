#!/bin/bash

./build/RISCV/gem5.opt --outdir=simpleOut --debug-flags=Exec --debug-file=trace.out ./configs/learning_gem5/part1/simple-riscv.py
