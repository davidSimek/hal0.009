all: hal asm

hal: hal_src/*c
	gcc -o run hal_src/*.c

asm: asm_src/*c
	gcc -o asm asm_src/*.c
