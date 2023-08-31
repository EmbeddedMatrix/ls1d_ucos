
DIR	=#/opt/gcc-4.3-ls232/bin/
CROSS_COMPILE = mips-linux-gnu-
DEBUG = -Wall -Werror -g #-O3 
FLAGS = -EL -mips32 -msingle-float 

CC	= $(DIR)$(CROSS_COMPILE)gcc
LD	= $(DIR)$(CROSS_COMPILE)ld
OBJCOPY	= $(DIR)$(CROSS_COMPILE)objcopy
OBJDUMP	= $(DIR)$(CROSS_COMPILE)objdump
AR	= $(DIR)$(CROSS_COMPILE)ar

export AR CC DEBUG FLAGS

devices.o: devices/*
	make -C devices $@
	cp devices/devices.o ./

asm.o: ls1d_asm/*
	make -C ls1d_asm $@
	cp ls1d_asm/asm.o ./

cpu.o: ls1d_cpu/*
	make -C ls1d_cpu $@
	cp ls1d_cpu/cpu.o ./

source.o: source/*
	make -C source $@
	cp source/source.o ./

lib1d.o: lib1d/*
	make -C lib1d $@
	cp lib1d/lib1d.o ./

Func.o: Func/*
	make -C Func $@
	cp Func/Func.o ./

start.o:start.S include/*
	$(CC) $(DEBUG) $(FLAGS) -I include/asm/ -c -o $@ $<

main.o:main.c include/* 
	$(CC) $(DEBUG) $(FLAGS) -c -o $@ $< -I . -I include/

### 这里的 -L . -ldevices 顺序很重要，不然会在连接时出现 undefined reference  的错误~！！
### 依赖其他库的库一定要放到被依赖库的前面!!!
demo:user.lds start.o main.o devices.o asm.o cpu.o source.o Func.o lib1d.o 
	$(LD) -T user.lds -Ttext 0xBFC00000 -Map demo.map start.o main.o    Func.o source.o cpu.o asm.o lib1d.o devices.o -o $@
	rm -f *.o
	rm -f *.a

demo.bin:demo
	$(OBJCOPY) --gap-fill=0xff -O binary $< $@
	$(OBJDUMP) -Slz demo >demo.txt

all: demo.bin


clean:
	make -C devices clean
	make -C ls1d_asm clean
	make -C ls1d_cpu clean
	make -C source clean
	make -C lib1d clean
	make -C Func clean
	rm -f *.o *.a *.s 
	rm -f demo demo.map demo.txt demo.bin
