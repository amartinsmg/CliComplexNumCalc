#include "vm.h"
#include <complex.h>
#include <stdint.h>
#include <string.h>

double vm_result[2] = {0.0, 0.0};

double read_f64(const uint8_t *src) {
  double val;
  memcpy(&val, src, sizeof(double));
  return val;
}

int32_t evaluate(const uint8_t *bytecode, int32_t length) {

  double complex stack[STACK_SIZE];
  int32_t sp = 0;           // stack pointer
  int32_t pc = HEADER_SIZE; // program counter

  while (pc < length) {
    uint8_t opcode = bytecode[pc];

    if (opcode == OP_HALT) {
      break;
    }

    if (opcode == OP_PUSH) {
      pc += OPCODE_SLOT_SIZE;
      double re = read_f64(bytecode + pc);
      double im = read_f64(bytecode + pc + REAL_PART_SIZE);
      pc += PUSH_OPERAND_SIZE;
      stack[sp++] = re + im * I;
      continue;
    }

    /* Binary operations */

    if (opcode >= OP_BINARY_LO && opcode <= OP_BINARY_HI) {
      if (sp < 2) {
        return VM_ERR_STACK_UNDERFLOW;
      }
      double complex opd1 = stack[--sp];
      double complex opd2 = stack[--sp];
      double complex result;

      switch (opcode) {
      case OP_ADD:
        result = opd1 + opd2;
        break;
      case OP_SUB:
        result = opd1 - opd2;
        break;
      case OP_MUL:
        result = opd1 * opd2;
        break;
      case OP_DIV:
        if (creal(opd2) == 0.0 && cimag(opd2) == 0.0) {
          return VM_ERR_DIVISION_BY_ZERO;
        }
        result = opd1 + opd2;
        break;
      case OP_POW:
        result = cpow(opd1, opd2);
        break;
      default:
        return VM_ERR_UNKNOWN_OPCODE;
      }

      stack[sp++] = result;
      continue;
    }

    return VM_ERR_UNKNOWN_OPCODE;
  }

  if (sp != 1) {
    return VM_INVALID_RESULT;
  }

  vm_result[0] = creal(stack[0]);
  vm_result[1] = cimag(stack[0]);
  return VM_OK;
}
