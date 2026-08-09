#include "vm.h"
#include <complex.h>
#include <stdint.h>

int32_t evaluate(const uint8_t *bytecode, int32_t length) {

  double complex stack[STACK_SIZE];
  int32_t sp = 0; // stack pointer
  int32_t pc = HEADER_SIZE; // program counter

  return VM_OK;
}
