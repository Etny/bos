
TARGET_PATH := target
BUILD_PATH := build
SRC_PATH := src
GRUB_PATH := grub

TARGET_NAME := bos
TARGET := $(TARGET_PATH)/$(TARGET_NAME)
TARGET_GRUB := $(GRUB_PATH)/boot/$(TARGET_NAME)
TARGET_ISO := $(TARGET).iso

CC := /home/ynte/opt/cross/bin/i686-elf-gcc
AS := /home/ynte/opt/cross/bin/i686-elf-as

CFLAGS := -Wall -Wextra \
		 -O2 \
		 -ffreestanding \
		 --std=gnu99 

LDFLAGS := -O2 \
		  -nostdlib \
		  -ffreestanding \
		  -lgcc

ASM := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.s*)))
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(BUILD_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC) $(ASM)))))
VPATH = $(sort $(dir $(SRC) $(ASM)))

DISTCLEAN_LIST := $(OBJ)


CLEAN_LIST := $(TARGET) \
			  $(TARGET_GRUB) \
			  $(TARGET_ISO) \
			  $(DISTCLEAN_LIST)


.PHONY: all
all: dirs $(TARGET)
	@grub2-file --is-x86-multiboot $(TARGET)
	@echo Finished building $(TARGET_NAME).

.PHONY: dirs
dirs:
	@mkdir -p $(TARGET_PATH) $(BUILD_PATH)

$(TARGET): $(OBJ)
	$(CC) -T link.ld $(LDFLAGS) -o $@ $^

$(BUILD_PATH)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_PATH)/%.o: %.s
	$(AS) -o $@ $<

.PHONY: run
run: $(TARGET)
	qemu-system-i386 -kernel $(TARGET)

.PHONY: run-grub
run-grub: iso
	qemu-system-i386 -cdrom $(TARGET_ISO)

iso: $(TARGET_ISO)

$(TARGET_ISO): $(TARGET)
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
