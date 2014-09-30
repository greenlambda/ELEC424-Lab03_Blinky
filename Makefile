# CrazyFlie Blinky Makefile
# Jeremy Hunt
# Christopher Buck
# 9-30-14
# ELEC 424

# Makefile to build blinky firmware. 

CC=gcc
CFLAGS=-I. $(addprefix -I, $(LIBS) $(INC)) -g -Wall -Wextra
BIN=./bin
SRC=./src
INC=./inc
STM_LIB=./local_install/symlinks/stm_std_libs
LIBS=$(addprefix $(STM_LIB)/, Libraries/CMSIS/CM3/CoreSupport Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x Libraries/STM32F10x_StdPeriph_Driver/inc Utilities/STM32_EVAL)
DEPS = $(addprefix $(INC)/, blinky.h stm32f10x_conf.h)
OBJ = $(BIN)/blinky.o
ELF = $(BIN)/blinky.elf

all: $(ELF)

$(ELF): $(OBJ)
	$(CC) -T ./linker_script/stm32_flash.ld -o $@ $^ $(CFLAGS)

$(BIN)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

flash:
	@openocd -d0 -f interface/busblaster.cfg -f target/stm32f1x.cfg -c init -c targets -c "reset halt" -c "flash write_image erase blinky.elf" -c "verify_image blinky.elf" -c "reset run" -c shutdown

debug:
	@xterm -iconic -e openocd -f interface/busblaster.cfg -f target/stm32f1x.cfg -c 'init; targets; reset halt;' &
	@arm-none-eabi-gdb -ex 'target remote localhost:3333' blinky.elf
	@killall openocd

.PHONY: clean

clean:
	rm -f $(BIN)/*.o
	rm -f $(BIN)/*.elf
