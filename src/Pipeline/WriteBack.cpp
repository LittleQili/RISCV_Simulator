//
// Created by anna_Diao on 2019/7/10.
//

#include "../../header/Pipeline.h"

void WriteBack_rd(Buffer_MA_WB& buffer_ma_wb,Register& r){
    if(buffer_ma_wb.read_rd() == 0) return;//x0
    r.write_reg(buffer_ma_wb.read_rd_value(),buffer_ma_wb.read_rd());
}
void Fstep_WriteBack(Buffer_MA_WB& buffer_ma_wb,Register& r){
    int tmptype = static_cast<int>(buffer_ma_wb.read_instt());
    if(tmptype < 20&&tmptype >= 10)///STORE
        return;
    if(tmptype >= 60&&tmptype < 70)///BRANCH
        return;
    WriteBack_rd(buffer_ma_wb,r);
}