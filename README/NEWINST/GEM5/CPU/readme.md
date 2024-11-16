# Needed

* vrf的不去做过多的回顾了......
* 注意这里的op和decoder.isa中的op不是一个东西
  * 那里的是为了方便代码生成
  * 这里的就是纯粹区分不同的指令【可以简单理解为：**执行延迟相同的指令是同一类OP**】
* 这里的mod指令只需要有op就好，无需另外的func
  * 然后这里除了乘法的那个都算整数计算，按理来说可以共用一种op，但还是区分开来
* OP
  * cpu/op_class中添加
  * cpu/funcunit.py中还有opclass的枚举类型
  * decoder.isa中为不同种类赋予op
* 功能部件
  * minor的是在baseMinorCpu.py中添加功能部件，并加入到MinorDefaultFUPool
  * o3是类似的，funcunitconfig.py中增加功能部件，添加到fupool.py中的相应功能单元