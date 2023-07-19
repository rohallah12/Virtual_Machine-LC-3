#include <QCoreApplication>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
/* unix only */
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/mman.h>

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
    R_PC,//program counter, saves the next instruction in it, we read this instruction and perform it
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

//LC-3 computer
//memory, we have 2 ^ 16 slots each with a size of 16 bits
uint16_t memory[MEMORY_MAX];

//Regsiters, we have 10 registers, 8 are general purpose, and the last two are used
//as PC(program counter) and COND (condition)
uint16_t reg[R_COUNT];


//extend the number to a 16-bit
uint16_t sign_extend(uint16_t x, int bit_count){
    if(x >> (bit_count - 1) & 1){
        x |= (0xFFFF << bit_count);
    }
    return x;
}

//update the condition registery based on the value of a given registery
void update_flag(uint16_t r){
   if(reg[r] == 0){
       reg[R_COND] = FL_ZERO;
   }else if(reg[r] >> 15){
      reg[R_COND] = FL_NEG;
   }else{
       reg[R_COND] = FL_POS;
   }
}


int main(int argc, char *argv[])
{
    //set condition flag to zero
    reg[R_COND] = FL_ZERO;

    //set program counter to 0x3000 which is the default memory pointer
    enum {PC_START = 0x3000};
    reg[R_PC] = PC_START;

    int running = 1;
    while (running) {
        uint16_t *instruction = &reg[R_PC];
        uint16_t opcode = *(instruction) >> 12;//shift instruction to right by 12 bits to get 4 bits of the opcode
    }
}
