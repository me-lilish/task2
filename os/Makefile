
AS_SOURCES = $(wildcard kernel/core/*.asm kernel/drivers/*.asm)
C_SOURCES  = $(wildcard kernel/*.cpp kernel/core/*.cpp kernel/drivers/*.cpp)
HEADERS    = $(wildcard kernel/*.h kernel/core/*.h kernel/drivers/*.h)


OBJ   = ${C_SOURCES:.cpp=.o}
ASOBJ = ${AS_SOURCES:.asm=.o}


GCC  = i686-elf-g++
LD   = i686-elf-ld
NASM = nasm


GCCFLAGS = -Wall -m32 -ffreestanding -fno-exceptions -fno-rtti \
           -fno-asynchronous-unwind-tables -fno-pie -std=c++14

all: os.img

run: all
	qemu-system-i386 -fda os.img -monitor stdio


os.img: bin/bootsect.bin bin/kernel.bin
	@echo "[IMG] Building disk image..."
	dd if=/dev/zero        of=$@   bs=512 count=2880      status=none
	dd if=bin/bootsect.bin of=$@   bs=512 count=1   seek=0 conv=notrunc status=none
	dd if=bin/kernel.bin   of=$@   bs=512 count=100  seek=1 conv=notrunc status=none
	@echo "[IMG] Done -> $@"


bin/kernel.bin: kernel/kernel_entry.o $(ASOBJ) $(OBJ)
	@mkdir -p bin
	@echo "[LD ] Linking kernel..."
	$(LD) -m elf_i386 -T link.ld --oformat binary $^ -o $@
	@echo "[LD ] Done -> $@"


%.o: %.cpp $(HEADERS)
	@echo "[GCC] $<"
	$(GCC) $(GCCFLAGS) -c $< -o $@


%.o: %.asm
	@echo "[ASM] $<"
	$(NASM) -f elf32 $< -o $@


bin/%.bin: boot/%.asm
	@mkdir -p bin
	@echo "[BIN] $<"
	$(NASM) $< -f bin -I. -o $@

clean:
	@rm -f bin/*.bin os.img
	@rm -f kernel/kernel_entry.o $(OBJ) $(ASOBJ)

image.dis: os.img
	ndisasm -b 32 $< > $@

.PHONY: all run clean
