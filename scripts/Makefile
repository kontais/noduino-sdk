#
# Copyright (c) 2015 MaiKe Labs
#
#

ESPSDK_BASE		?= $(SDK_BASE)/sdk
TOOLCHAIN_DIR	?= $(SDK_BASE)/toolchain

# paths from the SDK used in this project
SDK_INCDIR	+= include include/driver
SDK_LIBDIR = lib
SDK_LDDIR = ld

# various paths from the ESPSDK used in this project
ESPSDK_INCDIR	+= include include/json
ESPSDK_LIBDIR	= lib
ESPSDK_LDDIR	= ld

# libraries used in this project, mainly provided by the ESPSDK
LIBS		+= gcc
LIBS		+= crypto espnow json lwip_536 lwip mesh net80211 phy pp pwm smartconfig ssl upgrade wpa wps main
LIBS		+= driver

# toolchain
CC		:= $(TOOLCHAIN_DIR)/xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc
AR		:= $(TOOLCHAIN_DIR)/xtensa-lx106-elf/bin/xtensa-lx106-elf-ar
LD		:= $(TOOLCHAIN_DIR)/xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc

# compiler flags using during compilation of source files
CFLAGS		+= -g -O2 -Wpointer-arith -Wundef -Werror -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals  -D__ets__ -DICACHE_FLASH

# linker flags used to generate the main object file
LDFLAGS		+= -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static

# linker script used for the above linkier step
LD_SCRIPT	= $(CONFIG_LD_SCRIPT)


# we create two different files for uploading into the flash
# these are the names and options to generate them
FW_FILE_1		= $(CONFIG_IRAM_ADDR)
FW_FILE_1_ARGS	= -bo $@ -bm $(CONFIG_FLASH_MODE) -bf $(CONFIG_FLASH_FREQ)	\
				  -bz $(FLASH_SIZE) -bs .text -bs .data -bs .rodata -bc -ec
FW_FILE_2		= $(CONFIG_IROM_ADDR)
FW_FILE_2_ARGS	= -es .irom0.text $@ -ec

# esptool path and port
ifeq ($(OS),Windows_NT)
	ESPTOOL		?= $(TOOLCHAIN_DIR)/bin/esptool.exe
	FW_TOOL		?= $(TOOLCHAIN_DIR)/bin/esptool.exe
	ESPPORT		?= COM3
else
	ESPTOOL		?= $(TOOLCHAIN_DIR)/bin/esptool
	FW_TOOL		?= $(TOOLCHAIN_DIR)/bin/esptool

	UNAME_S		:= $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		ESPPORT		?= /dev/cu.SLAB_USBtoUART
	endif
	ifeq ($(UNAME_S),Linux)
		ESPPORT		?= /dev/ttyUSB0
	endif
endif

# firmware generating tool
FW_TOOL		?= $(TOOLCHAIN_DIR)/bin/esptool
ESPTOOL_PY	?= $(TOOLCHAIN_DIR)/bin/esptool.py

ifeq ($(CONFIG_FFLASH_SIZELASH_SIZE), 512KB)
CONFIG_RFINIT_ADDR	?= 0x7C000
CONFIG_SYSCONF_ADDR	?= 0x7E000
CONFIG_YUNCONF_ADDR	?= 0x74000
CONFIG_LD_SCRIPT	?= $(CONFIG_CPU).512.ld
FLASH_SIZE = 512K
endif
ifeq ($(CONFIG_FLASH_SIZE), 1MB)
CONFIG_RFINIT_ADDR	?= 0xFC000
CONFIG_SYSCONF_ADDR	?= 0xFE000
CONFIG_YUNCONF_ADDR	?= 0xF4000
CONFIG_LD_SCRIPT ?= $(CONFIG_CPU).1024.ld
FLASH_SIZE = 1M
endif
ifeq ($(CONFIG_FLASH_SIZE), 2MB)
CONFIG_RFINIT_ADDR ?= 0x1FC000
CONFIG_SYSCONF_ADDR ?= 0x1FE000
CONFIG_YUNCONF_ADDR ?= 0x1F4000
CONFIG_LD_SCRIPT ?= $(CONFIG_CPU).2048.ld
FLASH_SIZE = 2M
endif
ifeq ($(CONFIG_FLASH_SIZE), 4MB)
CONFIG_RFINIT_ADDR ?= 0x3FC000
CONFIG_SYSCONF_ADDR ?= 0x3FE000
CONFIG_YUNCONF_ADDR ?= 0x3F4000
CONFIG_LD_SCRIPT ?= $(CONFIG_CPU).4096.ld
FLASH_SIZE = 4M
endif

CONFIG_ESPBAUD		?= 230400

##############################################
#### no user configurable options below here
###############################################
SRC_DIR		:= $(MODULES)
BUILD_BASE	:= $(addprefix $(BUILD_DIR)/,$(MODULES))

SDK_INCDIR		:= $(addprefix -I$(SDK_BASE)/,$(SDK_INCDIR))
SDK_LIBDIR		:= $(addprefix -L$(SDK_BASE)/,$(SDK_LIBDIR))

ESPSDK_INCDIR	:= $(addprefix -I$(ESPSDK_BASE)/,$(ESPSDK_INCDIR))
ESPSDK_LIBDIR	:= $(addprefix -L$(ESPSDK_BASE)/,$(ESPSDK_LIBDIR))
LD_SCRIPT		:= $(addprefix -T$(SDK_BASE)/$(SDK_LDDIR)/,$(LD_SCRIPT))

INCDIR			:= $(addprefix -I,$(SRC_DIR))
MODULE_INCDIR	:= -I include

SRC			:= $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
OBJ			:= $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))
LIBS		:= $(addprefix -l,$(LIBS))
APP_AR		:= $(addprefix $(BUILD_DIR)/,$(TARGET)_app.a)
TARGET_OUT	:= $(addprefix $(BUILD_DIR)/,$(TARGET).out)

FW_FILE_1	:= $(addprefix $(FW_BASE)/,$(FW_FILE_1).bin)
FW_FILE_2	:= $(addprefix $(FW_BASE)/,$(FW_FILE_2).bin)

V ?= $(VERBOSE)
ifeq ("$(V)","1")
Q :=
vecho := @true
else
Q := @
vecho := @echo
endif

vpath %.c $(SRC_DIR)

define compile-objects
$1/%.o: %.c
	$(vecho) "CC $$<"
	$(Q) $(CC) $(INCDIR) $(MODULE_INCDIR) $(ESPSDK_INCDIR) $(SDK_INCDIR) $(CFLAGS)  -c $$< -o $$@
endef

.PHONY: all checkdirs flash clean

all: checkdirs $(TARGET_OUT) $(FW_FILE_1) $(FW_FILE_2)

$(TARGET_OUT): $(APP_AR)
	$(vecho) "LD $@"
	$(Q) $(LD) $(ESPSDK_LIBDIR) $(SDK_LIBDIR) $(LD_SCRIPT) $(LDFLAGS) -Wl,--start-group $(LIBS) $(APP_AR) -Wl,--end-group -o $@

$(APP_AR): $(OBJ)
	$(vecho) "AR $@"
	$(Q) $(AR) cru $@ $^

checkdirs: $(BUILD_BASE) $(FW_BASE)

$(BUILD_BASE):
	$(Q) mkdir -p $@

firmware:
	$(Q) mkdir -p $@

clean:
	$(Q) rm -f $(APP_AR)
	$(Q) rm -f $(TARGET_OUT)
	$(Q) rm -rf $(BUILD_DIR)
	$(Q) rm -f $(FW_FILE_1)
	$(Q) rm -f $(FW_FILE_2)
	$(Q) rm -rf $(FW_BASE)

$(foreach bdir,$(BUILD_BASE),$(eval $(call compile-objects,$(bdir))))

$(FW_FILE_1): $(TARGET_OUT)
	$(vecho) "FW $@"
	$(Q) $(FW_TOOL) -eo $(TARGET_OUT) $(FW_FILE_1_ARGS)

$(FW_FILE_2): $(TARGET_OUT)
	$(vecho) "FW $@"
	$(Q) $(FW_TOOL) -eo $(TARGET_OUT) $(FW_FILE_2_ARGS)
 
flash: all
	-$(ESPTOOL) -cd $(CONFIG_BOARD) -cb $(CONFIG_ESPBAUD) -cp $(ESPPORT)				\
				-ca $(CONFIG_IRAM_ADDR) -cf $(FW_BASE)/$(CONFIG_IRAM_ADDR).bin	\
				-ca $(CONFIG_IROM_ADDR) -cf $(FW_BASE)/$(CONFIG_IROM_ADDR).bin

rfinit:
	-$(ESPTOOL) -cd $(CONFIG_BOARD) -cb $(CONFIG_ESPBAUD) -cp $(ESPPORT) 			\
				-ca $(CONFIG_RFINIT_ADDR) -cf $(ESPSDK_BASE)/bin/$(CONFIG_RF_INIT_DATA)

wificlean:
	-$(ESPTOOL) -cd $(CONFIG_BOARD) -cb $(CONFIG_ESPBAUD) -cp $(ESPPORT)		\
				-ca $(CONFIG_SYSCONF_ADDR) -cf $(ESPSDK_BASE)/bin/blank.bin		\
				-ca $(shell printf "0x%x" $$(($(CONFIG_SYSCONF_ADDR)+0x1000))) -cf $(ESPSDK_BASE)/bin/blank.bin

yunclean:
	-$(ESPTOOL) -cd $(CONFIG_BOARD) -cb $(CONFIG_ESPBAUD) -cp $(ESPPORT)		\
				-ca $(CONFIG_YUNCONF_ADDR) -cf $(ESPSDK_BASE)/bin/blank.bin		\
				-ca $(shell printf "0x%x" $$(($(CONFIG_YUNCONF_ADDR)+0x1000))) -cf $(ESPSDK_BASE)/bin/blank.bin

zero: wificlean yunclean

# Reset the device id to $(DID), for testing
# make devid DID=WotD0000000208
devid:
	-$(ESPTOOL_PY) read_flash $(CONFIG_YUNCONF_ADDR) 0x1000 jackxx.bin
	-printf $(DID) | dd conv=notrunc of=jackxx.bin bs=1 seek=35
	-$(ESPTOOL) -cd $(CONFIG_BOARD) -cb $(CONFIG_ESPBAUD) -cp $(ESPPORT)	\
				-ca $(CONFIG_YUNCONF_ADDR) -cf jackxx.bin					\
				-ca $(shell printf "0x%x" $$(($(CONFIG_YUNCONF_ADDR)+0x1000))) -cf jackxx.bin
	-rm -f jackxx.bin

# analysis the memory usage of our firmware
mem:
	-$(ESPTOOL_PY) elf2image -o /tmp/ $(TARGET_OUT)

# erase the whole flash!
erase:
	-$(ESPTOOL_PY) --port $(ESPPORT) erase_flash
