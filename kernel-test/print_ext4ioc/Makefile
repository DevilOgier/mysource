obj-m := printext4ioc.o
CURRENT_PATH=$(shell pwd)
LINUX_KERNEL=$(shell uname -r)
KDIR ?= /lib/modules/$(LINUX_KERNEL)/build
EXTRA_CFLAGS += -I/usr/src/kernels/4.4.0-104-generic  ##私有内核头文件路径

all:
	 make -C $(KDIR) M=$(CURRENT_PATH) modules
clean:
	 make -C $(KDIR) M=$(CURRENT_PATH) clean
