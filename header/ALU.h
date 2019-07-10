//
// Created by anna_Diao on 2019/7/10.
//

#ifndef RISCV_SIMULATOR_ALU_H
#define RISCV_SIMULATOR_ALU_H

#include "Buffer.h"

class ALU{
private:
    unsigned int rs1_content;
    unsigned int rs2_content;
    unsigned int unsigned_imm;
    unsigned int imm;
    InstT instt;

private:

    inline unsigned int ADDer(unsigned int n1,unsigned int n2);
    inline unsigned int SUBer(unsigned int n1,unsigned int n2);
    inline unsigned int XORer(unsigned int n1,unsigned int n2);
    inline unsigned int ANDer(unsigned int n1,unsigned int n2);
    inline unsigned int ORer(unsigned int n1,unsigned int n2);
public:
    ALU(Buffer& buffer);

    int LOAD_STORE_offset();
    unsigned int ARITHer();
    unsigned int LOGICer();
    unsigned int SHIFTer();
    unsigned int COMPer();
    int BRANCHer();
    int JAL();
    int JALR();
    int AUIPC();
};

#endif //RISCV_SIMULATOR_ALU_H
