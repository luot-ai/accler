# Needed

* 无论是vrf，还是modreg，本质上更接近miscreg【硬件意义上的特殊寄存器】，而不是intregs【架构寄存器】；并且编译器也无法分配它们的编号

  * 寄存器的实现

    * 参照那次提交
    * 完成寄存器类的声明，将他添加入regfile/regclass中
  * 读写函数的声明【查看exec-ns.cc.inc是最好的】

    * 适配入intregs担心会参加 记分牌什么的

      * 当然我其实也尝试了，只不过不好验证，可以关注operands.isa/isa_parser.py/operands_type.py
      * 【其实也可以继续研究普通寄存器的读取实现，但是脑子疼，算了】
    * 适配入miscreg其实应该是合理的，但是也担心会有其他副作用

      * 好像也尝试了按miscreg来，但是失败了，懒得去探究了
    * 所以还是单列出来，也算是简化实现

      * src/cpu/thread_context声明读写的虚函数
      * 不同种类的cpu必须 都 进行函数的override，详见提交记录 也可以ctrl-f setmodereg
        * 在这里重载，无需实现的cpu只需return，需要实现的cpu调用thread.xxFunc
      * src/cpu/simple_thread中进行真正实现，【~~注意这里不用写override，因为他不继承自context，是不同cpu继承context~~】
        * regfiles目前就简单认为是按那个class排序的
* 生成汇编

  * 目前的mod扩展指令是复用regop的，其实没问题，只不过 mod寄存器不会出现在汇编代码中