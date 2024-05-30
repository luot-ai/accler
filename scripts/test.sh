#!/bin/bash

GEM5_DIR=/gem5                     
DNET_DIR=$GEM5_DIR/darknet          # location of darknet
OUTPUT_DIR=$DNET_DIR/outputFast

# 尺寸参数列表
sizes=(16 32 128 256)
# start=4
# end=512
# for ((i = start; i <= end; i += 4)); do
#     sizes+=($i)
# done
ifc=(3 16 32 64 128 512)
kc=(32 64 128 512)

# 循环遍历ifc数组
for i in "${ifc[@]}"; do
    # 循环遍历kc数组
    for j in "${kc[@]}"; do
        for size in "${sizes[@]}"; do
            ./myt.sh 0 winomain "$size" "$i" "$j"
            # 检查winomain.out文件第五行是否包含"same"
            if grep -q "same" "$OUTPUT_DIR/winomain.out"; then
                # 如果第五行包含"same"，则继续执行后续操作
                # 在stats.txt中查找第二次出现的"numCycles"关键字后面的值，并存入value变量
                value1=$(awk '/numCycles/ {c++; if (c==2) print $2}' $OUTPUT_DIR/stats.txt)
                value2=$(awk '/numCycles/ {c++; if (c==4) print $2}' $OUTPUT_DIR/stats.txt)
                echo "$size $i $j $value1 $value2" >> $DNET_DIR/sortdata/test.txt
            else
                # 如果第五行不包含"same"，则输出错误信息并退出循环
                echo "Error: winomain.out does not contain 'same'." >> $DNET_DIR/sortdata/test.txt
            fi
        done
    done
done