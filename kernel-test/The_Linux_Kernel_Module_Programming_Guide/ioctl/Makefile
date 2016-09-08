
obj-m += chardev.o

KDIR ?= /usr/src/linux-2.6.32-431.el6

all:
	make -C $(KDIR) M=$(PWD) modules
	gcc ioctl.c -o ioctl

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f ioctl