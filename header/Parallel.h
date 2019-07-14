//
// Created by anna_Diao on 2019/7/13.
//

#ifndef RISCV_SIMULATOR_PARALLEL_H
#define RISCV_SIMULATOR_PARALLEL_H

#include "ALU.h"
//#define PC_WATCH
//#define DEBUG_FOWARDING
class Parallel_Ctrler{
private:
    memory *m;
    Register r;

    Buffer_IF_ID buffer_if_id;
    Buffer_ID_EX buffer_id_ex;
    Buffer_EX_MA buffer_ex_ma;
    Buffer_MA_WB buffer_ma_wb;

    bool isready[5];

    void fetch_sw(Ins_Base* &bp, const unsigned int& op,const unsigned int& inst_content);
    void FStep_Fetch();
    ///这个步骤中使用了寄存器，进行上锁操作
    void Fstep_Decode();
    ///这个步骤中buffer_ex_ma仅有memoffset用到read操作。
    void Fstep_excute();
    void Fstep_MemoryAccess();
    void Fstep_WriteBack();
public:
    Parallel_Ctrler();
    ~Parallel_Ctrler();

    void Run_Parallel();

    void Run_Forwarding();
};

#endif //RISCV_SIMULATOR_PARALLEL_H
