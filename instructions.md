# Instrucions

Each instruction has 8 bits.

# general type

First 2 bits are dedited for general type.

| values | meaning |
|-------:|:--------|
|     00 | invalid |
|     01 | math    |
|     10 | memory  |
|     11 | flow    |

Invalid shows instructions should kill program for now. In future, these will be used for more advances stuff, like displaying on screen.

# variant

Some operatoins might have multiple variants. For example move. It can either copy value from register to register, from register to memory, from memory to register, or from memory to memory.

| values | meaning   |
|-------:|:----------|
|     00 | variant 1 |
|     01 | variant 2 |
|     10 | variant 3 |
|     11 | variant 4 |


operation

Last 4 bits specify specific operation. For example `0110` might be move operation, which copies value from one register to another one.

## Memory instructions
| instruction |              name | explanation                            | argument 1 | argument 2 |
|------------:|------------------:|:---------------------------------------|------------|------------|
|  10-00-0000 | movar(arg to reg) | copies value from ragument to register | number     | register   |
|  10-01-0000 | movrr(reg to reg) | copies value from register to register | register   | register   |
|  10-10-0000 | movrm(reg to mem) | copies value from register to memory   | register   | number     |
|  10-11-0000 | movmr(mem to reg) | copies value from memory to register   | number     | register   |


## Flow instructions
| instruction | name | explanation       | argument 1 | argument 2 |
|------------:|-----:|:------------------|------------|------------|
|  11-00-0000 | exit | shuts down system | none       | none       |
|  11-00-0001 |  jmp | jumps to address  | number     | none       |


## Math instructions
| instruciton | name | explanation                                                           | argument 1 | argument 2 |
|------------:|-----:|:----------------------------------------------------------------------|------------|------------|
|  01-00-0000 |  inc | increases register by 1                                               | register   | none       |
|  01-00-0001 |  dec | decreases register by 1                                               | register   | none       |
|  01-00-0010 |  cmp | result register is set to 1 if register $1 equals register $2, else 0 | register   | register   |

