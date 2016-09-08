
obj-m += sleep.o

KDIR ?= /usr/src/linux-2.6.32-431.el6

all:
	make -C $(KDIR) M=$(PWD) modules
	gcc cat_noblock.c -o cat_noblock

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f cat_noblock