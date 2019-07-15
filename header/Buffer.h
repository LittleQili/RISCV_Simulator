//
// Created by anna_Diao on 2019/7/10.
//

#ifndef RISCV_SIMULATOR_BUFFER_H
#define RISCV_SIMULATOR_BUFFER_H

#include "Instruction.h"

enum HazardT {
    NON = 0,CONTROL,__JALR,
};

///这里处理成类效率确实有些低
///但是我认为……如果不封装就失去了意义（因为他们同为controler的成员，都可以被每个阶段访问到）
class Buffer_IF_ID{
private:
    Ins_Base* bp;
    int pc;
    int pc_beforeIF;
    HazardT hazard;
public:
    Buffer_IF_ID();
    ~Buffer_IF_ID();

    void modify_bp(Ins_Base* xbp);
    Ins_Base* read_bp();

    int read_PC();
    void jumpcommon_PC(int step);
    void modify_PC(int xpc);

    int read_pc_beforeIF();
    void modify_pc_beforeIF(int);

    void modify_hazard(HazardT);
    HazardT read_hazard();
};

class Buffer_ID_EX{
private:
    int pc;
    unsigned rd;
    unsigned int rs1;
    unsigned int rs2;
    unsigned int rs1_content;
    unsigned int rs2_content;
    unsigned int imm;
    unsigned int unsigned_imm;
    InstT instt;

    HazardT hazard;
    int pc_pre;
    int pc_beforeIF;

public:
    void modify_PC(int xpc);
    int read_PC();

    void modify_instt(InstT xinstt);
    InstT read_instt();

    void modify_rs1_content(unsigned int x);
    unsigned int read_rs1_content();
    void modify_rs2_content(unsigned int x);
    unsigned int read_rs2_content();
    void modify_rd(unsigned int x);
    unsigned int read_rd();
    void modify_rs1(unsigned int x);
    unsigned int read_rs1();
    void modify_rs2(unsigned int x);
    unsigned int read_rs2();

    void modify_unsigned_imm(unsigned int x);
    unsigned int read_unsigned_imm();
    void modify_imm(unsigned int x);
    unsigned int read_imm();

    void modify_hazard(HazardT);
    HazardT read_hazard();

    int read_pc_pre();
    void modify_pc_pre(int);
    int read_pc_beforeIF();
    void modify_pc_beforeIF(int);
};

class Buffer_EX_MA{
private:
    int pc;
    unsigned int rd;

    unsigned int rd_value;
    InstT instt;
    int mem_offset;

public:
    void modify_PC(int xpc);
    void jumpcommon_PC(int step);
    int read_PC();
    void modify_rd(unsigned int x);
    unsigned int read_rd();

    void modify_rd_value(unsigned int x);
    unsigned int read_rd_value();

    void modify_instt(InstT xinstt);
    InstT read_instt();

    void modify_mem_offset(int x);
    int read_mem_offset();

    void send_rd_value(Buffer_ID_EX& buffer_id_ex);
};

class Buffer_MA_WB{
private:
    unsigned int rd_value;
    unsigned int rd;
    InstT instt;
public:
    void modify_rd_value(unsigned int x);
    unsigned int read_rd_value();
    void modify_rd(unsigned int x);
    unsigned int read_rd();

    void modify_instt(InstT xinstt);
    InstT read_instt();

    void send_rd_value(Buffer_ID_EX& buffer_id_ex);
};
#endif //RISCV_SIMULATOR_BUFFER_H
