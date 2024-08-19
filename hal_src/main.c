#include "memory.h"
#include "interpreter.h"


int main(int argc, char **argv) {
  init_program_memory(argc, argv);
  init_user_memory();
  run();
  dump_registers();
  clean_user_memory();
  clean_program_memory();
}
