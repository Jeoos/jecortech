obj-m := Radix_demo.o
Radix_demo-objs := radix_demo.o

KEV := /lib/modules/`uname -r`/build
PWD :=$(shell pwd)

all:
	$(MAKE) -C $(KEV) M=$(PWD) modules
	gcc ioctl_test.c -o ioctl_test
clean:
	$(MAKE) -C $(KEV) M=$(PWD) clean 
	rm ioctl_test

