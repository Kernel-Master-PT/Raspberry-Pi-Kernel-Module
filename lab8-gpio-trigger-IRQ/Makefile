PWD := $(shell pwd)
KVERSION := $(shell uname -r)
KERNEL_DIR := /lib/modules/$(shell uname -r)/build

MODULE_NAME = arduino-irq
obj-m += $(MODULE_NAME).o

all: dtb
	make -C $(KERNEL_DIR) M=$(PWD) modules

dtb:
	dtc -@ -I dts -O dtb -o $(MODULE_NAME).dtbo $(MODULE_NAME).dts

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
	rm $(MODULE_NAME).dtbo
