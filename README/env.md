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
* commit
  * commit有个命令，可以在容器里先删一些大文件
    * 我删了build 和 rv-tool-chain的.git
* 上传
  * podman login docker.io
  * podman tag gem5_rvtool:v1 amn0428/gem5_rvtool:v1
  * podman push amn0428/gem5_rvtool:v1

## Gem5文档下啥都有

### Gem5 standard library

* 感觉和那个tutorial有点像
  * 目前做到第三节

### Gem5资源

* 包含磁盘、操作系统引导程序、**测试程序**等

### Gem5 API

* stdlib大概就是用于构建Gem5的较复杂功能集，API就是提供的一些简单的小函数吧