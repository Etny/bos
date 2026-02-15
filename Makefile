
TARGET_PATH := target
BUILD_PATH := build
SRC_PATH := src
GRUB_PATH := grub

VERSION=0.0.1

TARGET_NAME := bos
TARGET_ARCH := ia32
TARGET := $(TARGET_PATH)/$(TARGET_NAME)
TARGET_GRUB := $(GRUB_PATH)/boot/$(TARGET_NAME)
TARGET_ISO := $(TARGET).iso

CC := ./toolchain/bin/i686-elf-gcc
AS := ./toolchain/bin/i686-elf-as

CFLAGS := 	-Wall -Wextra -Werror \
			-g \
			-O2 \
			-ffreestanding \
			-std=gnu99  \
			-D__VERSION=\"$(VERSION)\" \
			-Isrc/include -Isrc/$(TARGET_ARCH)/include \
			-fno-inline

LDFLAGS :=	-g \
			-O2 \
		  	-nostdlib \
		  	-ffreestanding \
		  	-lgcc 

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRC := $(foreach x, $(SRC_PATH), $(call rwildcard,src,*.c *.s))
OBJ := $(addprefix $(BUILD_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
VPATH = $(sort $(dir $(SRC)))

DISTCLEAN_LIST := $(OBJ)


CLEAN_LIST := $(TARGET) \
			  $(TARGET_GRUB) \
			  $(TARGET_ISO) \
			  $(DISTCLEAN_LIST)


.PHONY: all
all: $(TARGET)
	@grub2-file --is-x86-multiboot2 $(TARGET)
	@echo Finished building $(TARGET_NAME).

.PHONY: dirs
dirs:
	@mkdir -p $(TARGET_PATH) $(BUILD_PATH)

$(TARGET): dirs $(OBJ)
	$(CC) -T link.ld $(LDFLAGS) -o $@ $(OBJ)

$(BUILD_PATH)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_PATH)/%.o: %.s
	$(AS) -o $@ $<

.PHONY: run
run: run-grub

run-grub: iso
	qemu-system-i386 -drive format=raw,file=$(TARGET_ISO)

iso: $(TARGET_ISO)

$(TARGET_ISO): $(TARGET)
	@grub2-file --is-x86-multiboot2 $(TARGET)
	@cp -f $< $(TARGET_GRUB)
	@grub2-mkrescue -o $@ $(GRUB_PATH)


.PHONY: clean
clean:
	@echo Cleaning $(CLEAN_LIST)...
	-@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo Cleaning $(DISTCLEAN_LIST)...
	@rm -f $(DISTCLEAN_LIST)

