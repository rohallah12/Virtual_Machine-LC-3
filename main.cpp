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
#include "./keyboard.cpp"

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

//write to a memory location at address
void mem_write(uint16_t address, uint16_t value){
    memory[address] = value;
}

//load data from a location in memory
uint16_t mem_read(uint16_t location){
    //checking if we are reading from keyboard registers, this is because
    //we are going to do some WASD things in our program!
    if(location == MR_KBSR){
        if(check_key()){
            memory[MR_KBSR] = (1 << 15);
            memory[MR_KBDR] = getchar() ;
        } else{
            memory[MR_KBSR] = 0;
        }
    }
    //return the data from the location
    return memory[location];
}

//implementation of add opcode
void Add(uint16_t data){
    //first we need to extract the destination and source registers
    uint16_t rd = (data >> 9) & 0x7;//destination
    uint16_t rs = (data >> 6) & 0x7;//source

    //then we extarct the ADD mode (immediate mode or register mode)
    uint16_t imm_mode = (data >> 5) & 0x1;
    uint16_t left = reg[rs];

    //if immediate mode, then extract the value from last 4 bits, and extend it to a 16 bit value, be careful about negative
    //numbers (using two's complement)
    if(imm_mode){
        reg[rd] = left + (sign_extend(data & 0x1F, 5));
    }else{
        //if immediate mode is not enabled, then simply add up values at two registers (extarct the register containing the
        //second number from last 4 bits)
        reg[rd] = left + reg[data & 0x7];
    }
    //update the condition flag
    update_flag(rd);
}

//Load indirect, this is used to load data from an address into the destination register
void LDI(uint16_t data){
    //destination register
    uint16_t rd = (data >> 9) & 0x7;
    //extract the offset from program counter
    uint16_t offset = sign_extend(data & 0x1FF, 9);
    //load the memory address from the PC + offset location, then fetch the value from that memory address
    //and save it in destination register
    reg[rd] = mem_read(mem_read(reg[R_PC] + offset));
    update_flag(reg[rd]);
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
        switch(opcode){
            case OP_ADD:
                Add(*(instruction) >> 4);
                break;
        }
    }
}
