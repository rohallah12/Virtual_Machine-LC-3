#include <QCoreApplication>

#define MEMORY_MAX (1 << 16)

//registers
enum {
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC,
    R_COND,//registers the information about most recently executed calculation
    R_COUNT,
};

//Flags
enum {
  FL_POS = 1 << 0,
  FL_ZERO = 1 << 1,
  FL_NEG = 1<< 2,
};

//RICS instructions
enum
{
    OP_BR = 0, /* branch */
    OP_ADD,    /* add  */
    OP_LD,     /* load */
    OP_ST,     /* store */
    OP_JSR,    /* jump register */
    OP_AND,    /* bitwise and */
    OP_LDR,    /* load register */
    OP_STR,    /* store register */
    OP_RTI,    /* unused */
    OP_NOT,    /* bitwise not */
    OP_LDI,    /* load indirect */
    OP_STI,    /* store indirect */
    OP_JMP,    /* jump */
    OP_RES,    /* reserved (unused) */
    OP_LEA,    /* load effective address */
    OP_TRAP    /* execute trap */
};


int main(int argc, char *argv[])
{
    //LC-3 computer
    //memory, we have 2 ^ 16 slots each with a size of 16 bits
    uint16_t memory[MEMORY_MAX];

    //Regsiters, we have 10 registers, 8 are general purpose, and the last two are used
    //as PC(program counter) and COND (condition)
    uint16_t registers[R_COUNT];
}
