BINARY      := mosvector
KERNEL      := /lib/modules/$(shell uname -r)/build
KMOD_DIR    := $(shell pwd)
TARGET_PATH := /lib/modules/$(shell uname -r)/kernel/drivers/char

# Source files for the module
obj-m += $(BINARY).o

# Set compiler flags
ccflags-y := -Wall

all:
	$(MAKE) -C $(KERNEL) M=$(KMOD_DIR) modules

install:
	sudo cp $(BINARY).ko $(TARGET_PATH)
	sudo depmod -a

uninstall:
	sudo rm -f $(TARGET_PATH)/$(BINARY).ko
	sudo depmod -a

clean:
	$(MAKE) -C $(KERNEL) M=$(KMOD_DIR) clean