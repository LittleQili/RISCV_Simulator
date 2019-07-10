//
// Created by anna_Diao on 2019/7/10.
//

#ifndef RISCV_SIMULATOR_BUFFER_H
#define RISCV_SIMULATOR_BUFFER_H

#include "Instruction.h"

class Buffer{
private:
    Ins_Base *bp;

    unsigned int rs1_content;
    unsigned int rs2_content;
    int pc;

    unsigned int rd_value;

    int mem_offset;

public:
    Buffer();
    ~Buffer();

    void modify_bp(Ins_Base*);
    Ins_Base* read_bp();

    int read_PC();
    void jumpcommon_PC(int);

    void modify_rs1_content(unsigned int x);
    unsigned int read_rs1_content();
    void modify_rs2_content(unsigned int x);
    unsigned int read_rs2_content();
    void modify_rd_value(unsigned int x);
    unsigned int read_rd_value();

    void modify_mem_offset(int x);
    int read_mem_offset();
};

#endif //RISCV_SIMULATOR_BUFFER_H
