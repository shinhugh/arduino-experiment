# Makefile

# --------------------------------------------------

# Paths
PATH_ROOT := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
PATH_BUILD := $(PATH_ROOT)/build
PATH_SRC := $(PATH_ROOT)/src

# Targets
OBJ := $(PATH_BUILD)/*.o
EXE := $(PATH_BUILD)/main
HEX := $(PATH_BUILD)/*.hex

# Commands and options
CC := avr-gcc
CFLAGS := -c -std=c11 -mmcu=atmega328p -Os -I $(PATH_SRC)
LFLAGS := -mmcu=atmega328p
OC := avr-objcopy
OCFLAGS := -O ihex -R .eeprom
UP := avrdude
UPFLAGS := -p m328p -c arduino -P /dev/ttyACM0 -F -V

# --------------------------------------------------

# Unconditional targets
.PHONY: clean default

# --------------------------------------------------

# Default target
default:
	@echo "Options:"
	@echo "- make compile"
	@echo "- make flash"
	@echo "- make clean"

# --------------------------------------------------

# Compile into machine code for target hardware
compile:

	@echo "Compiling."
	@$(CC) $(CFLAGS) -o $(PATH_BUILD)/main.o $(PATH_SRC)/main.c
	@$(CC) $(LFLAGS) -o $(PATH_BUILD)/main $(PATH_BUILD)/main.o
	@$(OC) $(OCFLAGS) $(PATH_BUILD)/main $(PATH_BUILD)/main.hex

# --------------------------------------------------

# Flash compiled machine code to target hardware
flash:

	@echo "Flashing."
	@$(UP) $(UPFLAGS) -U flash:w:$(PATH_BUILD)/main.hex

# --------------------------------------------------

# Remove compiled build
clean:

	@echo "Removing build."
	@rm -rf $(OBJ) $(EXE) $(HEX)
