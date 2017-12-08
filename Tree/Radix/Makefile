obj-m := Radix_demo.o
Radix_demo-objs := radix_demo.o

KEV := /lib/modules/`uname -r`/build
PWD :=$(shell pwd)

all:
	$(MAKE) -C $(KEV) M=$(PWD) modules
clean:
	$(MAKE) -C $(KEV) M=$(PWD) clean 

