obj-m := print_block_device.o
CURRENT_PATH=$(shell pwd)
LINUX_KERNEL=$(shell uname -r)
KDIR ?= /lib/modules/$(LINUX_KERNEL)/build

all:
	 make -C $(KDIR) M=$(CURRENT_PATH) modules
	 go build test.go
clean:
	 make -C $(KDIR) M=$(CURRENT_PATH) clean
	 rm test
