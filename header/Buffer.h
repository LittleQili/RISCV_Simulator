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

    inline void modify_bp(Ins_Base* xbp);
    inline Ins_Base* read_bp();

    inline int read_PC();
    inline void jumpcommon_PC(int step);
    inline void modify_PC(int xpc);

    inline int read_pc_beforeIF();
    inline void modify_pc_beforeIF(int);

    inline void modify_hazard(HazardT);
    inline HazardT read_hazard();
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
    inline void modify_PC(int xpc);
    inline int read_PC();

    inline void modify_instt(InstT xinstt);
    inline InstT read_instt();

    inline void modify_rs1_content(unsigned int x);
    inline unsigned int read_rs1_content();
    inline void modify_rs2_content(unsigned int x);
    inline unsigned int read_rs2_content();
    inline void modify_rd(unsigned int x);
    inline unsigned int read_rd();
    inline void modify_rs1(unsigned int x);
    inline unsigned int read_rs1();
    inline void modify_rs2(unsigned int x);
    inline unsigned int read_rs2();

    inline void modify_unsigned_imm(unsigned int x);
    inline unsigned int read_unsigned_imm();
    inline void modify_imm(unsigned int x);
    inline unsigned int read_imm();

    inline void modify_hazard(HazardT);
    inline HazardT read_hazard();

    inline int read_pc_pre();
    inline void modify_pc_pre(int);
    inline int read_pc_beforeIF();
    inline void modify_pc_beforeIF(int);
};

class Buffer_EX_MA{
private:
    int pc;
    unsigned int rd;

    unsigned int rd_value;
    InstT instt;
    int mem_offset;

public:
    inline void modify_PC(int xpc);
    inline void jumpcommon_PC(int step);
    inline int read_PC();
    inline void modify_rd(unsigned int x);
    inline unsigned int read_rd();

    inline void modify_rd_value(unsigned int x);
    inline unsigned int read_rd_value();

    inline void modify_instt(InstT xinstt);
    inline InstT read_instt();

    inline void modify_mem_offset(int x);
    inline int read_mem_offset();

    inline void send_rd_value(Buffer_ID_EX& buffer_id_ex);
};

class Buffer_MA_WB{
private:
    unsigned int rd_value;
    unsigned int rd;
    InstT instt;
public:
    inline void modify_rd_value(unsigned int x);
    inline unsigned int read_rd_value();
    inline void modify_rd(unsigned int x);
    inline unsigned int read_rd();

    inline void modify_instt(InstT xinstt);
    inline InstT read_instt();

    inline void send_rd_value(Buffer_ID_EX& buffer_id_ex);
};
#endif //RISCV_SIMULATOR_BUFFER_H
