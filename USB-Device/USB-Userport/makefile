#
#             LUFA Library
#     Copyright (C) Dean Camera, 2017.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------
#
# Additional contribution by
# * The USB-Userport *
# René Trapp (rene [dot] trapp (-at-) web [dot] de)

# Run "make help" for target help.
# Run "make" to compile the code.
# Run "make docu" to build the documentation.

MCU          = atmega32u4
ARCH         = AVR8
BOARD        = BOARD_USER
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = Userport
SRC          = $(TARGET).c
SRC         += Descriptors.c
SRC         += Board/ft.c
SRC         += $(LUFA_SRC_USB)
SRC         += $(LUFA_SRC_USBCLASS)
LUFA_PATH    = ../LUFA
RESULT_PATH  = Release
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/
LD_FLAGS     =

# Default target
all:
# Copy result files
	mv *.hex $(RESULT_PATH)/
	mv *.eep $(RESULT_PATH)/

docu:
	c:\Program Files\doxygen\bin\doxygen.exe ./Doxyfile_eng
	c:\Program Files\doxygen\bin\doxygen.exe ./Doxyfile_deu

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH      ?= $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/doxygen.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/hid.mk
include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk
