#
# Copyright (c) 2015 MaiKe Labs
#
#

# name for the target project
TARGET		= app

# which modules (subdirectories) of the project to include in compiling
MODULES		= .

BUILD_DIR	= build
FW_BASE		= build

# base directory of the SDK package, absolute
SDK_BASE		?= ../..

# read the configuration file
include $(SDK_BASE)/.config
sinclude .config

# libraries used in this project, mainly provided by the ESPSDK
#LIBS		+=

# toolchain
#CC		:= $(TOOLCHAIN_DIR)/xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc
#AR		:= $(TOOLCHAIN_DIR)/xtensa-lx106-elf/bin/xtensa-lx106-elf-ar
#LD		:= $(TOOLCHAIN_DIR)/xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc

# compiler flags using during compilation of source files
#CFLAGS		+=

# linker flags used to generate the main object file
#LDFLAGS		+=

ifeq ($(CONFIG_OTA), y)
include $(SDK_BASE)/scripts/Makefile.ota
else
include $(SDK_BASE)/scripts/Makefile
endif
