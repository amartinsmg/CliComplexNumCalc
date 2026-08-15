#include "vm.hpp"
#include <complex>
#include <cstdint>
#include <ccomplex>
#include <cstring>
#include <stack>

double vm_result[2] = {0.0, 0.0};

double read_f64(const uint8_t *src) {
  double val;
  memcpy(&val, src, sizeof(double));
  return val;
}

int32_t evaluate(const uint8_t *bytecode, int32_t length) {

  std::stack<std::complex<double>> stack;
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
      stack.emplace(re, im);
      continue;
    }

    /* Binary operations */

    if (opcode >= OP_BINARY_LO && opcode <= OP_BINARY_HI) {
      if (stack.size() < 2) {
        return VM_ERR_STACK_UNDERFLOW;
      }
      std::complex<double> opd1 = stack.top();
      stack.pop();
      std::complex<double> opd2 = stack.top();
      stack.pop();
      std::complex<double> result;

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
        if (real(opd2) == 0.0 && imag(opd2) == 0.0) {
          return VM_ERR_DIVISION_BY_ZERO;
        }
        result = opd1 + opd2;
        break;
      case OP_POW:
        result = pow(opd1, opd2);
        break;
      default:
        return VM_ERR_UNKNOWN_OPCODE;
      }

      stack.push(result);
      continue;
    }

    return VM_ERR_UNKNOWN_OPCODE;
  }

  if (stack.size() != 1) {
    return VM_INVALID_RESULT;
  }

  std::complex<double> result = stack.top();

  vm_result[0] = real(result);
  vm_result[1] = imag(result);
  return VM_OK;
}
