#ifndef VM_H
#define VM_H

#include <cstdint>

constexpr int32_t BYTECODE_VERSION = 1;

constexpr int32_t HEADER_SIZE = 8;
constexpr int32_t OPCODE_SLOT_SIZE = 8;
constexpr int32_t REAL_PART_SIZE = 8;
constexpr int32_t PUSH_OPERAND_SIZE = 16; // re (8) + im (8)

constexpr int32_t STACK_SIZE = 256;
constexpr int32_t MAX_BYTECODE_LEN = 16384;

/* --- Opcodes --------------------------------------- */

/* Control: 0x00-0x1F */

constexpr int32_t OP_PUSH = 0x00;
constexpr int32_t OP_HALT = 0x01;

constexpr int32_t OP_CONTROL_LO = 0x00;
constexpr int32_t OP_CONTROL_HI = 0x01F;

/* Binary Operators: 0x20-0x3F */

constexpr int32_t OP_ADD = 0x20;
constexpr int32_t OP_SUB = 0x21;
constexpr int32_t OP_MUL = 0x22;
constexpr int32_t OP_DIV = 0x23;
constexpr int32_t OP_POW = 0x24;

constexpr int32_t OP_BINARY_LO = 0x20;
constexpr int32_t OP_BINARY_HI = 0x3F;

/* Unary Operators: 0x40-0x5F */

constexpr int32_t OP_SIN = 0x40;
constexpr int32_t OP_COS = 0x41;
constexpr int32_t OP_TAN = 0x42;
constexpr int32_t OP_ASIN = 0x43;
constexpr int32_t OP_ACOS = 0x44;
constexpr int32_t OP_ATAN = 0x45;
constexpr int32_t OP_SINH = 0x46;
constexpr int32_t OP_COSH = 0x47;
constexpr int32_t OP_TANH = 0x48;
constexpr int32_t OP_LOG = 0x49;
constexpr int32_t OP_LN = 0x4A;
constexpr int32_t OP_EXP = 0x4B;
constexpr int32_t OP_SQRT = 0x4C;
constexpr int32_t OP_CONJ = 0x4D;
constexpr int32_t OP_ABS = 0x4E;
constexpr int32_t OP_ARG = 0x4F;
constexpr int32_t OP_NEG = 0x50;

constexpr int32_t OP_UNARY_LO = 0x40;
constexpr int32_t OP_UNARY_HI = 0x5F;

/* ----- Error Codes --------------------------------- */

using vm_error_t = enum {
  VM_OK = 0,
  VM_ERR_DIVISION_BY_ZERO = 1,
  VM_ERR_TRUNCATED = 2,
  VM_ERR_STACK_UNDERFLOW = 3,
  VM_ERR_STACK_OVERFLOW = 4,
  VM_ERR_UNKNOWN_OPCODE = 5,
  VM_INVALID_RESULT = 6,
};

extern "C" {

/* ----- Output Buffer ------------------------------- */

extern double vm_result[2];

/* ----- Main Entry Point ------------------- */

int32_t evaluate(const uint8_t *bytecode, int32_t length);
}

#endif /* VM_H */