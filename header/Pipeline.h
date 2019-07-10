//
// Created by anna_Diao on 2019/7/10.
//

#ifndef RISCV_SIMULATOR_PIPELINE_H
#define RISCV_SIMULATOR_PIPELINE_H

#include "ALU.h"
#include "Buffer.h"

void FStep_Fetch(memory *m,Register& r,Buffer& buffer);
void fetch_sw(Ins_Base* &bp, const unsigned int& op,const unsigned int& inst_content);

void Fstep_Decode(Buffer& buffer,Register& r);

void Fstep_excute(Buffer& buffer);

void Fstep_MemoryAccess(Buffer& buffer,memory *m);

inline void WriteBack_rd(Buffer& buffer,Register& r);

void Fstep_WriteBack(Buffer& buffer,Register& r);

#endif //RISCV_SIMULATOR_PIPELINE_H
