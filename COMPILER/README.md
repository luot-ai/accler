# 环境说明

* 目前rvtoolchain是在lt服务器上的一个容器crazy_heyrovsky中构建的
  * 路径是/riscv-gnu-toolchain
  * 因为是进入容器之后clone的仓库，因此该部分代码是lt服务器中没有的
  * 容器内只有vim，可以拷贝到gem5目录下使用vscode编写

## 添加指令方法

* 获取opcodes-custom，将opcodes.txt的内容放入后，重新运行脚本得到encoding.h
* 根据encoding.h中得到的输出，添加到riscv-opc.h[3行] riscv-opc.c
  * 要添加到这两个文件的内容见add.c
* 在build-binutils-linux中单独编译
