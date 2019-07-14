//
// Created by anna_Diao on 2019/7/10.
//

#ifndef RISCV_SIMULATOR_BUFFER_H
#define RISCV_SIMULATOR_BUFFER_H

#include "Instruction.h"

enum HazardT {
    NON = 0,CONTROL,
    DATA,DATA_rs1,DATA_rs2,DATA_both,
    BOTH,BOTH_rs1,BOTH_rs2,BOTH_both,
};
class Buffer_IF_ID{
private:
    Ins_Base* bp;
    int pc;

    HazardT hazard;
public:
    Buffer_IF_ID();
    ~Buffer_IF_ID();

    void modify_bp(Ins_Base* xbp);
    Ins_Base* read_bp();

    int read_PC();
    void jumpcommon_PC(int step);
    void modify_PC(int xpc);

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
};

class Buffer_EX_MA{
private:
    int pc;
    unsigned int rd;

    unsigned int rd_value;
    InstT instt;
    int mem_offset;

    HazardT hazard;
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

    void modify_hazard(HazardT);
    HazardT read_hazard();

    bool send_rd_value(Buffer_ID_EX& buffer_id_ex){
        bool flag = false;
        if(rd == 0)return false;
        if(rd == buffer_id_ex.read_rs1()){
            buffer_id_ex.modify_rs1_content(rd_value);
            flag = true;
        }
        if(rd == buffer_id_ex.read_rs2()){
            buffer_id_ex.modify_rs2_content(rd_value);
            flag = true;
        }

        return flag;
    }
};

class Buffer_MA_WB{
private:
    unsigned int rd_value;
    unsigned int rd;
    InstT instt;

    HazardT hazard;
public:
    void modify_rd_value(unsigned int x);
    unsigned int read_rd_value();
    void modify_rd(unsigned int x);
    unsigned int read_rd();

    void modify_instt(InstT xinstt);
    InstT read_instt();

    void modify_hazard(HazardT);
    HazardT read_hazard();

    bool send_rd_value(Buffer_ID_EX& buffer_id_ex){
        bool flag = false;
        if(rd == 0)return false;
        if(rd == buffer_id_ex.read_rs1()){
            buffer_id_ex.modify_rs1_content(rd_value);
            flag = true;
        }
        if(rd == buffer_id_ex.read_rs2()){
            buffer_id_ex.modify_rs2_content(rd_value);
            flag = true;
        }

        return flag;
    }
};
#endif //RISCV_SIMULATOR_BUFFER_H
