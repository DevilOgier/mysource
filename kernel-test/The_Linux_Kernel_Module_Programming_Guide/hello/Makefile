
obj-m += hello-1.o
obj-m += hello-2.o

KDIR ?= /usr/src/linux-2.6.32-431.el6
all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean