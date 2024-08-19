# registers

This virtual machine has few types of registers. Every single one of these is 8 bits big.
## Result register
It has 1 result register. It stores result of calculation, till it is overwriten.

|   name | representatoin |
|-------:|:---------------|
| result | 00000000       |

## General purpose registers
It has 16 general purpose registers.
| name | representation |
|-----:|:---------------|
|    a | 00000001       |
|    b | 00000010       |
|    c | 00000011       |
|    d | 00000100       |
|    e | 00000101       |
|    f | 00000110       |
|    g | 00000111       |
|    h | 00001000       |
|    i | 00001001       |
|    j | 00001010       |
|    k | 00001011       |
|    l | 00001100       |
|    m | 00001101       |
|    n | 00001110       |
|    o | 00001111       |
|    p | 00010000       |

# stack pointer
Stack pointer is register, holding address, which points to head of the stack. It is initialized in beginning of the program to point at biginning of stack.

| name | representation |
|-----:|:---------------|
|   sp | 00010001       |


# base pointer
Base pointer is register, holding address, which points to head of stack in state, when you entered function. It helps you easily find, what to deallocate, when exiting procedure.

| name | representation |
|-----:|:---------------|
|   bp | 00010010       |

# program counter
Program counter is register, holding value, which coresponds to byte, which will be interpreted next.

| name | representation |
|-----:|:---------------|
|   pc | 00010011       |
