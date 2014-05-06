ifneq ($(KERNELRELEASE),)
	obj-m := PIODriver.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
	
all: default

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc UserSpace/user-space-test.c -o  test -lm
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

endif
