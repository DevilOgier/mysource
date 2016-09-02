ifneq ($(KERNELRELEASE),)

mymodule_objs := sample2.o

obj-m := sample2.o

else

PWD := $(shell pwd)

# export KDIR=/usr/src/linux-2.6.32-431.el6
#KDIR ?= /lib/modules/$(shell uname -r)/build
KDIR ?= /usr/src/linux-2.6.32-431.el6

all:
	$(MAKE) -C $(KDIR)  M=$(PWD)

clean:
	rm -rf *.mod.c *.mod.o *.ko *.o *.tmp_versions *.order *symvers *.unsigned .*.cmd .tmp_versions

endif
