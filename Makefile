#KERNELRELEASE这个变量，在内核源码的根目录下面的Makefile会初初始化的
 
ifeq ($(KERNELRELEASE),)
 
#改为你自己的内核源码目录
KERN_DIR :=/usr/src/linux-headers-6.8.0-45-generic/
 
#在Makefile中可以调用shell的命令，调用方法如下:
# $(shell  shell命令) -> 整个这个表达式，表示调用该shell命令的输出结果
PWD := $(shell pwd)
 
#改为你自己的交叉编译工具的目录
 
modules:
	make -C $(KERN_DIR)  M=$(PWD)  modules
clean:
	rm -rf *.o
	rm -rf  modules.order  .tmp_versions *.ko  Module.symvers
	rm -f  *.cmd .*.cmd  *.mod.c
 
else
	
	obj-m += filter.o
 
endif
