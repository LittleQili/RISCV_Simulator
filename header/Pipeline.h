//
// Created by anna_Diao on 2019/7/10.
//

#ifndef RISCV_SIMULATOR_PIPELINE_H
#define RISCV_SIMULATOR_PIPELINE_H

#include "ALU.h"
void fetch_sw(Ins_Base* &bp, const unsigned int& op,const unsigned int& inst_content);
void FStep_Fetch(memory* m,Buffer_IF_ID& buffer_if_id);

void Fstep_Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex);

///这个步骤中buffer_ex_ma仅有memoffset用到read操作。
void Fstep_excute(Buffer_ID_EX& buffer_id_ex,Buffer_EX_MA& buffer_ex_ma);

void Fstep_MemoryAccess(memory *m,Buffer_EX_MA& buffer_ex_ma,Buffer_MA_WB& buffer_ma_wb);

void WriteBack_rd(Buffer_MA_WB& buffer_ma_wb,Register& r);
void Fstep_WriteBack(Buffer_MA_WB& buffer_ma_wb,Register& r);

#endif //RISCV_SIMULATOR_PIPELINE_H
