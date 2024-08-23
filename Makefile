all: hal hasm

hal: hal_src/*c
	gcc -g -o hal hal_src/*.c

hasm: hasm_src/*c
	gcc -g -o hasm hasm_src/*.c
