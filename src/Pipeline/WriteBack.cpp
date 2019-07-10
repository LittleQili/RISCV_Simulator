//
// Created by anna_Diao on 2019/7/10.
//

#include "../../header/Pipeline.h"

inline void WriteBack_rd(Buffer& buffer,Register& r){
    if(buffer.read_bp()->rd == 0) return;//x0
    r.write_reg(buffer.read_rd_value(),buffer.read_bp()->rd);
}
void Fstep_WriteBack(Buffer& buffer,Register& r){
    int tmptype = static_cast<int>(buffer.read_bp()->instt);
    if(tmptype < 20&&tmptype >= 10)///STORE
        return;
    if(tmptype >= 60&&tmptype < 70)///BRANCH
        return;
    WriteBack_rd(buffer,r);
}