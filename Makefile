HARDWARE_FILES = build/hardware/pic/pic.o build/hardware/port/port.o build/hardware/port/port.asm.o build/hardware/ps2/ps2.o
IDT_FILES = build/idt/idt.o build/idt/idt.asm.o build/idt/int_handlers.asm.o build/idt/int_service_routines.o
IO_FILES = build/io/vga/vga.o
MEMORY_FILES = build/memory/memory.o

FILES = build/kernel.asm.o build/kernel.o $(HARDWARE_FILES) $(IDT_FILES) $(IO_FILES) $(MEMORY_FILES)
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: bin/boot.bin bin/kernel.bin
	rm -rf bin/os.bin
	dd if=bin/boot.bin >> bin/os.bin
	dd if=bin/kernel.bin >> bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> bin/os.bin

bin/kernel.bin: $(FILES)
	mkdir -p $(@D)
	i686-elf-ld -g -relocatable $(FILES) -o build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T src/linker.ld -ffreestanding -O0 -nostdlib build/kernelfull.o -o $@

bin/boot.bin: src/boot/boot.asm
	mkdir -p $(@D)
	nasm -f bin $^ -o $@

build/%.asm.o: src/%.asm
	mkdir -p $(@D)
	nasm -f elf -g $^ -o $@

build/%.o: src/%.c
	mkdir -p $(@D)
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $^ -o $@

clean:
	rm -rf bin build