
obj-m += kthread.o
obj-m += completion1.o
obj-m += completion2.o

KDIR ?= /usr/src/linux-2.6.32-431.el6

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean