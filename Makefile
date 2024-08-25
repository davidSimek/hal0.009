all: hal hasm

hal: hal_src/*c
	gcc -fsanitize=undefined -std=c2x -g -Wall -Wextra -pedantic -o hal hal_src/*.c

hasm: hasm_src/*c
	gcc -fsanitize=undefined -std=c2x -g -Wall -Wextra -pedantic -o hasm hasm_src/*.c
