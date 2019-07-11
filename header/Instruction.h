//
// Created by anna_Diao on 2019/7/10.
//

#ifndef RISCV_SIMULATOR_INSTRUCTION_H
#define RISCV_SIMULATOR_INSTRUCTION_H

#include "Memory.h"
#include "Register.h"

enum InstT {
    Load_family = 0,LB,LH,LW,LBU,LHU,
    Store_family = 10,SB,SH,SW,
    Arith_family = 20,LUI,ADDI,ADD,SUB,
    Logic_family = 30,XORI,ORI,ANDI,XOR,OR,AND,
    Shift_family = 40,SLLI,SRLI,SRAI,SLL,SRL,SRA,
    Comp_family = 50,SLTI,SLTIU,SLT,SLTU,
    Command_family = 60,BEQ,BNE,BLT,BGE,BLTU,BGEU,
    alone_family = 70,AUIPC,JAL,JALR,
    UnDecoded = 80,
};

class Buffer_IF_ID;
class Buffer_ID_EX;
class Buffer_EX_MA;
class Buffer_MA_WB;
struct Ins_Base{
    unsigned int funct3,funct7;
    unsigned int rs2,rs1,rd;
    unsigned int imm,unsigned_imm;
    InstT instt;

    Ins_Base();
    virtual void Fetch(const unsigned int& inst_content){};

    inline void getfunct7(unsigned int& funct7,const unsigned int& instruction){
        funct7 = (instruction>>25);///here exists a warning
    }
    inline void getfunct3(unsigned int& funct3,const unsigned int& instruction){
        funct3 = ((instruction>>12)&0b111);
    }
    inline void getrd(unsigned int& rd,const unsigned int& instruction){
        rd = ((instruction>>7)&0b11111);
    }
    inline void getrs1(unsigned int& rs1,const unsigned int& instruction){
        rs1 = ((instruction>>15)&0b11111);
    }
    inline void getrs2(unsigned int& rs2,const unsigned int& instruction){
        rs2 = ((instruction>>20)&0b11111);
    }

    virtual void Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex){};
};

struct Ins_R: Ins_Base{
    Ins_R();
    virtual ~Ins_R(){};
    virtual void Fetch(const unsigned int& inst_content);
    virtual void Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex);
};
struct Ins_I: Ins_Base{
    Ins_I();
    virtual void Fetch(const unsigned int& inst_content);
    virtual void Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex);

};
struct Ins_S: Ins_Base{
    Ins_S();
    virtual void Fetch(const unsigned int& inst_content);
    virtual void Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex);

};
struct Ins_B: Ins_Base{
    Ins_B();
    virtual void Fetch(const unsigned int& inst_content);
    virtual void Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex);
};
struct Ins_U: Ins_Base{
    Ins_U();
    virtual void Fetch(const unsigned int& inst_content);
    virtual void Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex);

};
struct Ins_J: Ins_Base{
    Ins_J();
    virtual void Fetch(const unsigned int& inst_content);
    virtual void Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex);
};

#endif //RISCV_SIMULATOR_INSTRUCTION_H
