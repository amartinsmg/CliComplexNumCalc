#ifndef VM_H
#define VM_H

#include <stdint.h>


#define BYTECODE_VERSION 1

#define HEADER_SIZE 8
#define OPCODE_SLOT_SIZE 8
#define PUSH_OPERAND_SIZE 16  // re (8) + im (8)


#define STACK_SIZE 256
#define MAX_BYTECODE_LEN 16384


/* --- Opcodes --------------------------------------- */

/* Control: 0x00-0x1F */

#define OP_PUSH 0x00
#define OP_HALT 0x01

/* Binary Operators: 0x20-0x3F */

#define OP_ADD 0x20
#define OP_SUB 0x21
#define OP_MUL 0x22
#define OP_DIV 0x23
#define OP_POW 0x24

/* Unary Operators: 0x40-0x5F */

#define OP_SIN 0x40
#define OP_COS 0x41
#define OP_TAN 0x42
#define OP_ASIN 0x43
#define OP_ACOS 0x44
#define OP_ATAN 0x45
#define OP_SINH 0x46
#define OP_COSH 0x47
#define OP_TANH 0x48
#define OP_LOG 0x49
#define OP_LN 0x4A
#define OP_EXP 0x4B
#define OP_SQRT 0x4C
#define OP_CONJ 0x4D
#define OP_ABS 0x4E
#define OP_ARG 0x4F
#define OP_NEG 0x50


/* ----- Error Codes --------------------------------- */

typedef enum {
    VM_OK = 0,
    VM_ERR_DIVISION_BY_ZERO = 1
} vm_error_t;


/* ----- Output Buffer ------------------------------- */

extern double vm_result[2];


/* ----- Main Entry Point ------------------- */

int32_t evaluate(const uint8_t *bytecode, int32_t length);


#endif