obj-m := dump_pagetables.o
CURRENT_PATH=$(shell pwd)
LINUX_KERNEL=$(shell uname -r)
KDIR ?= /lib/modules/$(LINUX_KERNEL)/build

all:
	 make -C $(KDIR) M=$(CURRENT_PATH) modules
clean:
	 make -C $(KDIR) M=$(CURRENT_PATH) clean
