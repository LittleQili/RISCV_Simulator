//
// Created by anna_Diao on 2019/7/10.
//

#include "../../header/Pipeline.h"

void Fstep_MemoryAccess(memory *m,Buffer_EX_MA& buffer_ex_ma,Buffer_MA_WB& buffer_ma_wb){
    buffer_ma_wb.modify_instt(buffer_ex_ma.read_instt());
    buffer_ma_wb.modify_rd(buffer_ex_ma.read_rd());
    buffer_ma_wb.modify_rd_value(buffer_ex_ma.read_rd_value());
    int tmptype = static_cast<int>(buffer_ex_ma.read_instt());
///LOAD
    if(tmptype < 10){
        unsigned int tmprd;
        switch (tmptype){
            case LB:
                tmprd = m->read_content(buffer_ex_ma.read_mem_offset(),1);
                tmprd = ((tmprd>>7) == 1?(tmprd|0xffffff00):tmprd);
                buffer_ma_wb.modify_rd_value(tmprd);
                break;
            case LH:
                tmprd = m->read_content(buffer_ex_ma.read_mem_offset(),2);
                tmprd = ((tmprd>>15) == 1?(tmprd|0xffff0000):tmprd);
                buffer_ma_wb.modify_rd_value(tmprd);
                break;
            case LW:
                tmprd = m->read_content(buffer_ex_ma.read_mem_offset(),4);
                buffer_ma_wb.modify_rd_value(tmprd);
                break;
            case LBU:
                tmprd = m->read_content(buffer_ex_ma.read_mem_offset(),1);
                buffer_ma_wb.modify_rd_value(tmprd&0xff);
                break;
            case LHU:
                tmprd = m->read_content(buffer_ex_ma.read_mem_offset(),2);
                buffer_ma_wb.modify_rd_value(tmprd&0xffff);
                break;
            default:throw exception::MA_InvalidLoad();
        }
    }else if(tmptype < 20){///STORE
        switch (tmptype){
            case SW:
                m->write_content(buffer_ex_ma.read_mem_offset(),4,buffer_ex_ma.read_rd_value());
                break;
            case SH:
                m->write_content(buffer_ex_ma.read_mem_offset(),2,buffer_ex_ma.read_rd_value());
                break;
            case SB:
                m->write_content(buffer_ex_ma.read_mem_offset(),1,buffer_ex_ma.read_rd_value());
                break;
            default:throw exception::MA_InvalidSTORE();
        }
    }
}