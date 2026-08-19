#include "vm.hpp"
#include <complex>
#include <cstdint>
#include <cstring>
#include <stack>
#include <vector>

double vm_result[2] = {0.0, 0.0};

namespace {

using Complex = std::complex<double>;

double read_f64(const uint8_t *src) {
  double val;
  std::memcpy(&val, src, sizeof(double));
  return val;
}

} // namespace

extern "C" int32_t evaluate(const uint8_t *bytecode, int32_t length) {

  std::vector<Complex> storage;
  storage.reserve(STACK_SIZE);

  std::stack<Complex, std::vector<Complex>> stack(std::move(storage));

  int32_t pc = HEADER_SIZE; // program counter

  while (pc < length) {
    uint8_t opcode = bytecode[pc];
    pc += OPCODE_SLOT_SIZE;

    if (opcode == OP_HALT) {
      break;
    }

    if (opcode == OP_PUSH) {
      if (pc + PUSH_OPERAND_SIZE > length) {
        return VM_ERR_TRUNCATED;
      }
      if (static_cast<int32_t>(stack.size()) >= STACK_SIZE) {
        return VM_ERR_STACK_OVERFLOW;
      }
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
      Complex opd2 = stack.top();
      stack.pop();
      Complex opd1 = stack.top();
      stack.pop();
      Complex result;

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
        if (opd2.real() == 0.0 && opd2.imag() == 0.0) {
          return VM_ERR_DIVISION_BY_ZERO;
        }
        result = opd1 / opd2;
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

  Complex result = stack.top();

  vm_result[0] = result.real();
  vm_result[1] = result.imag();
  return VM_OK;
}
