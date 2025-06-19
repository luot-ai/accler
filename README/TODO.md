# ALL
1. 编译那里改main_program就可用，那天只是因为.cc

# WINO

1. allowmemearlyissue必须为false?
2. setcreg/getcreg那里有个reg_class不能是[8]
3. 咱们的gem5默认是64位哎？？？？？？？？？

# MOD

1. 顺序的处理器肯定可以做到背靠背;然后模拟器其实没必要去写一个控制逻辑，也是可以保证正确性的
2. 符号的问题->打印trace看original，然后比对build/riscv/generated下的exec.cc啥的
    `主打的就是 保持 和原来的一致性`
    `有有-无 无无-有`
