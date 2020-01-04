#ifndef INTERPRETER_H
#define INTERPRETER_H

#define NUM_OPCODES 32
#define STACK_LIMIT 1000
#define CODE_LIMIT 1000

typedef enum {
    NOOP = 0, OP_LA, OP_LV, OP_LC, OP_LI, OP_INT, OP_DCT,
    OP_J, OP_FJ, OP_HLT, OP_ST, OP_CALL, OP_EP, OP_EF,
    OP_RC, OP_RI, OP_WRC, OP_WRI, OP_WLN,
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_NEG, OP_CV,
    OP_EQ, OP_NE, OP_GT, OP_LT, OP_GE, OP_LE
} OpCode;

int NUM_ARGS[NUM_OPCODES] = {
    0, 2, 2, 1, 0, 1, 1,
    1, 1, 0, 0, 2, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
};

const char* ASM[NUM_OPCODES] = {
    "\0", "LA", "LV", "LC", "LI", "INT", "DCT",
    "J", "FJ", "HLT", "ST", "CALL", "EP", "EF",
    "RC", "RI", "WRC", "WRI", "WLN",
    "ADD", "SUB", "MUL", "DIV", "NEG", "CV",
    "EQ", "NE", "GT", "LT", "GE", "LE"
};

typedef struct{
    OpCode op;
    int p;
    int q;
} Instruction;

int s[STACK_LIMIT];
Instruction code[CODE_LIMIT];
int n; // number of instructions
int t; // stack top

#endif // INTERPRETER_H
