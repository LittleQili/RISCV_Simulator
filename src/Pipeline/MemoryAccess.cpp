//
// Created by anna_Diao on 2019/7/10.
//

#include "../../header/Pipeline.h"

void Fstep_MemoryAccess(Buffer& buffer,memory *m){
    int tmptype = static_cast<int>(buffer.read_bp()->instt);
///LOAD
    if(tmptype < 10){
        unsigned int tmprd;
        switch (tmptype){
            case LB:
                tmprd = m->read_content(buffer.read_mem_offset(),1);
                tmprd = ((tmprd>>7) == 1?(tmprd|0xffffff00):tmprd);
                buffer.modify_rd_value(tmprd);
                break;
            case LH:
                tmprd = m->read_content(buffer.read_mem_offset(),2);
                tmprd = ((tmprd>>15) == 1?(tmprd|0xffff0000):tmprd);
                buffer.modify_rd_value(tmprd);
                break;
            case LW:
                tmprd = m->read_content(buffer.read_mem_offset(),4);
                buffer.modify_rd_value(tmprd);
                break;
            case LBU:
                tmprd = m->read_content(buffer.read_mem_offset(),1);
                buffer.modify_rd_value(tmprd&0xff);
                break;
            case LHU:
                tmprd = m->read_content(buffer.read_mem_offset(),2);
                buffer.modify_rd_value(tmprd&0xffff);
                break;
            default:throw exception::MA_InvalidLoad();
        }
    }else if(tmptype < 20){///STORE
        switch (tmptype){
            case SW:
                m->write_content(buffer.read_mem_offset(),4,buffer.read_rs2_content());
                break;
            case SH:
                m->write_content(buffer.read_mem_offset(),2,buffer.read_rs2_content());
                break;
            case SB:
                m->write_content(buffer.read_mem_offset(),1,buffer.read_rs2_content());
                break;
            default:throw exception::MA_InvalidSTORE();
        }
    }
}