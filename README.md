# 基于docker开发

* 在lyc服务器上使用官方提供的image构建container
  * 使用commit保存为镜像，save为tar
* 可以在公司服务器上load tar，挂载的gem5最好直接从github官网直接压缩
  * 捋一下：华为云->turbox连接luoteng->luoteng里连ssh arch-13
    * 账号下有gem5和tar，挂载操作，要改代码只能在arch-13下改
  * git的事情还没想好
* cmd
  * docker load -i gem5.tar
  * docker image ls
  * docker run -it -v 放置gem5代码的路径:/gem5 映像文件名

## Gem5文档下啥都有

### Gem5 standard library

* 感觉和那个tutorial有点像
  * 目前做到第三节

### Gem5资源

* 包含磁盘、操作系统引导程序、**测试程序**等

### Gem5 API

* stdlib大概就是用于构建Gem5的较复杂功能集，API就是提供的一些简单的小函数吧

# record

1. 编写脚本(来自网站)
   1. configs/dknet.py负责声明配置,参数
   2. /run_darknet.sh方便路径的设定,以及一些小参数(例如-I，最大指令数)
      1. 在里头会进入到run_dir，所以执行程序在该目录下即可
      2. 会指定gem5.opt的路径
      3. 输出在run_dir下的output中
      4. 命令行中还是会有一些用于【darknet】的参数，但没关系
   3. 切换到gem5/scripts下./run_darknet.sh xxx【xxx得在darknet下】
      1. xxx可以是darknet main，前者是跑 `大程序`，后者就是咱们自己写的 `demo`
2. 修改makefile
   1. 原脚本中既有动态库，又有静态库
   2. 使用静态库，编译选项加上-static
   3. 静态库中只有用到的算子.o文件会被链接，这就是静态库的意义
      1. exeobj就是相当于main函数，可以改成自己需要的就行
      2. exeobj 在EXEC=darknet时会是原脚本中的一堆，否则为指定程序xx.o
         1. make时指定EXEC，否则默认为main
3. 合并winograd后的脚本
   1. darknet中的makefile新增 条件变量 REF
      1. 注意变量后不能有注释
      2. 编译变量COMMON新增 convoFlavor -I.h【.h搜索路径】
      3. VPATH大坑，增加相关.c路径
      4. OBJ增加自己需要的文件，这些文件会一起被ar进入.a库
      5. DEPS是要被当做依赖项的.h文件
      6. **必须得注意同名文件，否则会出现只包含先出现的那个的情况**
   2. gem5/scrpits中有main.sh
      1. ./main.sh xxx   xxx默认为main
      2. 进入darknet中编译xxx，反汇编入./obj/important
      3. 回到本目录，执行./run xxx
4. 开始深入汇编
   1. m5_dump_reset_stats函数
      1. 使用
         1. .c文件中include头文件，在需要的地方加入m5_dump_reset_stats
         2. makefile中COMMON加入-I，LDFLAGS加入相应库的路径-Lxxx -lm5
      2. 效果就是stat会被切分，结合debug的trace
         1. 可以观察自己关注的程序段
         2. 查看循环每次执行的时间，关注执行时间较长的某次迭代
5. 查看流水
   1. Ds是无法issue的意思，可能是[dependenci] [barrier or non-speculative]
      1. m5op是那种指令，所以导致serialize，它commit的下一周期才真正离开rob，下一条指令需要两周期进行rename
6. 建立仓库
   1. 先建立远程，包括子仓库
   2. 本地设置好url，包括在父目录下设置子仓库的url
   3. 从最孙仓库开始，逐层往上提交
7. 脚本拆分
   1. 记得在main.sh里改动
      1. 不带1的用来测时间
      2. 带1的看Exec和Pipe
8. 新脚本【真得找机会合并一下...】
   * ./test.sh搭配命令行->使用myt.sh对单层神经网络进行参数化测试
   * 顶层 main.sh【darknet】 myt.sh【winomain】
     * run_mode

       * =0，测时间
         * 设置使用run_darknet 还是 run_darknet_fast
       * =e，run_darknet1.sh测指令序列trace
       * =default (pipe)，run_darknet1.sh测流水线trace
         * **可在次层脚本修改DEBUGFLAG**
     * main_program

       * 设置testbench
     * size等params

       * myt【run_fast & run】专属
       * ***在test.sh中指示***
   * 次层_none[opt测时间] _1[opt测trace] __fast[fast测时间]
     * _1最下面的命令行无用
     * none与_fast都测时间

       * RUNMODE参数无用
       * 最下命令行接了SIZE等参数，传到底层，可能有用
     * 使用的底层脚本

       * fast->gem5.fast dknet_fast.py
       * other->gem5.opt dknet.py
   * 底层
     * none _fast【记得在对应的脚本里改！】
     * cmd
       * 若使用darknet，需要在cmd中选择合适的检测模式/权重/图像【已有demo】
       * 若使用winomain，cmd里传递参数，当然winomain源码中不一定需要这些参数
     * cpu等参数
   * winomain中的参数：使用传递的参数 or 直接写死