!!! under development !!!
# Hal 0.009
simple virtual machine
## Features:
- its instruction set
- its assembly language (hasm)
- its assembler

## How to use:
1. Check instruction.md for avalible instructions.
2. Check registers.md for avalible registers.
3. Check memory.md (in progress) for memory structure.
4. clone OR download and unzip project.  
   `git clone <link to this repo>`
5. build it for your machine  
   `make`
6. write hasm file ending with `.hasm`  
   move_to_register.hasm
   ```
   MOVMR 123 C
   EXIT
   ```
7. assemble  
   `./hasm move_to_register.hasm`
8. run your code  
   `./hal move_to_register.bin`

You should see register dump.
