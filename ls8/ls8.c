#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;
  if (argc != 2) {
    fprintf(stderr, "You didn't provide the right number of args. Please include the name of the file you want to run.\n");
    exit(1);
  }
  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);
  return 0;
}