# This is a make file file for RF20000 werewolf controller
SHELL = C:\WINDOWS\system32\cmd.exe
IAR_DIR ='C:\Program Files\IAR Systems\Embedded Workbench 6.5'
#BUILD_DIR=C:\

# The compiler is
CC= $(IAR_DIR)\arm\bin\iccarm.exe

#Compiler flags
CFLAGS =-D IAR \
-D DEBUG_CAN \
-D REV5\
--no_cse \
--no_unroll \
--no_inline \
--no_code_motion \
--no_tbaa \
--no_clustering \
--no_scheduling \
--debug \
--endian=little \
--cpu=Cortex-M4 \
-e --fpu=None \
--dlib_config $(IAR_DIR)\arm\INC\c\DLib_Config_Normal.h

#The linker is 
LD=$(IAR_DIR)\arm\bin\ilinkarm.exe

# Linker flags
LDFLAGS=--keep application \
--image_input='$(CURDIR)\..\51677\PIC Debug'\Exe\51677.bin,application,.app,32 \
--map '$(CURDIR)\debug'\53826.map \
--config '$(CURDIR)\iar'\K60_512KB.icf  \
--place_holder __checksum_main,2,.checksum_main,4 \
--define_symbol __checksum_begin_main=0x412 \
--keep=__checksum_main \
--semihosting \
--entry __iar_program_start $(IAR_DIR)\arm\CMSIS\Lib\IAR\iar_cortexM4lf_math.a \
--vfe 

#The assembler is
AS=$(IAR_DIR)\arm\bin\iasmarm.exe

#Assembler flags
ASFLAGS= -s+ \
-M'<>' \
-w+ \
-r \
--cpu Cortex-M4 \
--fpu None

#Elf tool (converts .out to .hex)
ELF="C:/Program Files/IAR Systems/Embedded Workbench 6.5/arm/bin/ielftool.exe"

#ELF Flags
ELF_FLAGS=--ihex \
--verbose
#ELF Flags for filling with 0xFF
ELF_FLAGS_0xFF=--fill 0xFF;0x412-checksum_end+3 --checksum __checksum_main:2,crc16,0x0;0x412-checksum_end+3 --verbose

# other flags
OPTIMIZATION=-Ol

# include directories
INCLUDES=-I '$(CURDIR)\asm' \
-I '$(CURDIR)\c' \
-I '$(CURDIR)\iar' \
-I '$(CURDIR)\include' \
$(OPTIMIZATION) \
-I $(IAR_DIR)\arm\CMSIS\Include -D ARM_MATH_CM4
#-I $(CURDIR)\lib \

#NAME OF TARGET
TARGET_NAME=53826


# C files are present in the 'c' folder
C_FILES := $(wildcard c/*.c)
C_FILES += $(wildcard include/*.c)

# Assembly files are present in the 'asm' folder
ASM_FILES := $(wildcard c/*.s) 
ASM_FILES += $(wildcard asm/*.s)

# We are going to put the object files in the 'debug' folder
OBJ_FILES := $(addprefix debug/,$(notdir $(C_FILES:.c=.o)))
OBJ_FILES += $(addprefix debug/,$(notdir $(ASM_FILES:.s=.o)))

# Default goal
all: debug/$(TARGET_NAME).hex debug/$(TARGET_NAME).bin

debug/$(TARGET_NAME).hex: debug/$(TARGET_NAME).out
	$(ELF) --ihex --verbose  $< $@

debug/$(TARGET_NAME).bin: debug/$(TARGET_NAME).out
	$(ELF) --bin --verbose $< $@ 

debug/$(TARGET_NAME).out: debug/temp.out
	$(ELF) $(ELF_FLAGS_0xFF) $< $@

# .out file
debug/temp.out: $(OBJ_FILES)
	$(LD) $(LDFLAGS) -o $@ $^

# making object files from c files in /c
debug/%.o: c/%.c 
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

# making object files from c files in /include
debug/%.o: include/%.c 
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

# making object files from asm files in asm folder
debug/%.o: asm/%.s
	$(AS) $< -o $@ $(ASFLAGS)

# making object files from asm files in c folder
debug/%.o: c/%.s
	$(AS) $< -o $@ $(ASFLAGS)

#clean
.PHONY:clean
clean:
	@echo "Cleaning files out"
	-rm -rf debug
	-mkdir debug

