编译内核模块经验：

1.内核模块的编译，由于内核编译架构问题，使用makefile进行编译。

2.编译命令为：
make -C "内核源码树" M="模块目录" modules

3.一般而言
"内核源码树" = /lib/modules/$(shell uname -r)/build
"模块目录" = $(shell pwd)
模块的名称"yamod"设置为: obj-m := yamod.o
模块依赖的源码为: yamod-y := $(src).o
编译选项可以使用: ccflags-y := -DDEBUG=1 -I$(src)
 