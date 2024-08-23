#include "memory.h"
#include "interpreter.h"

#include <signal.h>
#include <stdio.h>

void handle_interupt(int signal) {
  printf("\nSIGINT caught, killing hal...\n");
  kill_hal();
}

int main(int argc, char **argv) {
  signal(SIGINT, handle_interupt);
  init_program_memory(argc, argv);
  init_user_memory();
  
  run();
  dump_registers();
  
  clean_user_memory();
  clean_program_memory();
  printf("Hal 0.009 successfully shuted down.\n");
}
