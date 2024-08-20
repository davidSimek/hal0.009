all: hal hasm

hal: hal_src/*c
	gcc -o hal hal_src/*.c

hasm: hasm_src/*c
	gcc -o hasm hasm_src/*.c
