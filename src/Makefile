CROSS_COMPILE ?= riscv64-unknown-linux-gnu-

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld

CFLAGS += -mabi=ilp32 -march=rv32imac -MMD -ffreestanding -g -O
ASFLAGS += -mabi=ilp32 -march=rv32imac -MMD -ffreestanding -g
OBJECTS = start.o main.o

.PRECIOUS: %.o

.PHONY: all
all: demo.elf

.PHONY: clean
clean:
	rm -f *.o *.d *.elf

%.elf: %.lds $(OBJECTS)
	$(LD) -o $@ -T $^

-include $(wildcard *.d)
