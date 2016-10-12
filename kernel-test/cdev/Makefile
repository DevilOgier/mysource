
obj-m := cdev.o
obj-m := mycdev.o

PWD := $(shell pwd)

# export KDIR=/usr/src/linux-2.6.32-431.el6
#KDIR ?= /lib/modules/$(shell uname -r)/build
KDIR ?= /usr/src/linux-2.6.32-431.el6

all:
	$(MAKE) -C $(KDIR)  M=$(PWD)
	gcc cdev-user.c -o user

clean:
	rm -rf *.mod.c *.mod.o *.ko *.o *.tmp_versions *.order *symvers *.unsigned .*.cmd .tmp_versions user

