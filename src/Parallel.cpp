//
// Created by anna_Diao on 2019/7/13.
//

#include "../header/Parallel.h"

Parallel_Ctrler::Parallel_Ctrler():isIFlocked(false){
    m = new memory;
    m->inimem();
    for(int i = 0;i < 5;++i) isready[i] = false;
}
Parallel_Ctrler::~Parallel_Ctrler(){
    delete m;
}

void Parallel_Ctrler::FStep_Fetch(){
    Ins_Base *tmpbp = nullptr;
#ifdef PC_WATCH
    int tmppc = buffer_if_id.read_PC();
    unsigned int inst_content = m->get_inst(tmppc);
    std::cout << "IN FETCH. NOW the PC is: " << std::hex <<tmppc << std::endl;
#else
    unsigned int inst_content = m->get_inst(buffer_if_id.read_PC());
#endif
    unsigned int op = (inst_content&0x7f);

    fetch_sw(tmpbp,op,inst_content);

    buffer_if_id.modify_bp(tmpbp);

    //判断hazard类型
    /**这里判断的时候先判断control，因为有可能同时具备两种
     * 判断data的时候用了短路求值的思路。之后再加两种功能的时候应该小心为上。
     */
     InstT tmpinstt = tmpbp->instt;
    if(tmpinstt == Command_family){
        if(r.isreglocked(tmpbp->rs1)){
            if(r.isreglocked(tmpbp->rs2))buffer_if_id.modify_hazard(BOTH_both);
            else buffer_if_id.modify_hazard(BOTH_rs1);
        }else if(r.isreglocked(tmpbp->rs2))
            buffer_if_id.modify_hazard(BOTH_rs2);
        else buffer_if_id.modify_hazard(CONTROL);
    }
    if(tmpinstt == JALR){
        if(r.isreglocked(tmpbp->rs1))buffer_if_id.modify_hazard(BOTH_rs1);
        else buffer_if_id.modify_hazard(CONTROL);
    }
    if((tmpinstt == Store_family||op == 0b0110011)
       &&r.isreglocked(tmpbp->rs2)){
        buffer_if_id.modify_hazard(DATA_rs2);
    }else if((tmpinstt != LUI&&tmpinstt != AUIPC&&tmpinstt != JAL&&tmpinstt != Command_family&&tmpinstt != JALR)
       &&r.isreglocked(tmpbp->rs1))
        if(buffer_if_id.read_hazard() == DATA_rs2)buffer_if_id.modify_hazard(DATA_both);
        else buffer_if_id.modify_hazard(DATA_rs1);

    //上锁
    buffer_if_id.modify_Locknext(false);
    if(tmpbp->instt != Command_family&&tmpbp->instt != Store_family){
        if(tmpbp->rd != tmpbp->rs1&&tmpbp->rd != tmpbp->rs2)r.lock_reg(tmpbp->rd);
        else buffer_if_id.modify_Locknext(true);
    }

    if(tmpinstt != JAL)buffer_if_id.jumpcommon_PC(4);
    else buffer_if_id.jumpcommon_PC(tmpbp->imm);
}
///这里留住了后几个指令类型的家族分类操作。不知道后面有用否？
void Parallel_Ctrler::fetch_sw(Ins_Base* &bp, const unsigned int& op,const unsigned int& inst_content){
    Ins_Base* tmpd = bp;
    bp = nullptr;
    delete tmpd;
    switch (op){
        //R-type
        case 0b0110011:
            bp = new Ins_R;
            bp->Fetch(inst_content);
            break;

            //I-type:
        case 0b0010011:
            bp = new Ins_I;
            bp->Fetch(inst_content);
            if(bp->funct3==0b001||bp->funct3==0b101)bp->instt = Shift_family;
            break;
        case 0b1100111://JALR
            bp = new Ins_I;
            bp->Fetch(inst_content);
            bp->instt = JALR;
            break;
        case 0b0000011://Load_family*5
            bp = new Ins_I;
            bp->Fetch(inst_content);
            bp->instt = Load_family;
            break;

            //s-type
        case 0b0100011://Store_family*3
            bp = new Ins_S;
            bp->Fetch(inst_content);
            bp->instt = Store_family;
            break;

            //B-type
        case 0b1100011://B-head command*6
            bp = new Ins_B;
            bp->Fetch(inst_content);
            bp->instt = Command_family;
            break;

            //U-type
        case 0b0110111://LUI
            bp = new Ins_U;
            bp->Fetch(inst_content);
            bp->instt = LUI;
            break;
        case 0b0010111://AUIPC
            bp = new Ins_U;
            bp->Fetch(inst_content);
            bp->instt = AUIPC;
            break;

            //J-type
        case 0b1101111://JAL
            bp = new Ins_J;
            bp->Fetch(inst_content);
            bp->instt = JAL;
            break;
            //case :
        default:
            std::cout << "the op is: "<<std::hex <<op << std::endl;
            throw exception::Finding();
    }
}
void Parallel_Ctrler::Fstep_Decode(){
    Ins_Base* bp = buffer_if_id.read_bp();
    bp->Decode(r,buffer_if_id,buffer_id_ex);
    buffer_id_ex.modify_hazard(buffer_if_id.read_hazard());

    if(buffer_if_id.read_Locknext())r.lock_reg(buffer_id_ex.read_rd());
}
///这个步骤中buffer_ex_ma仅有memoffset用到read操作。
void Parallel_Ctrler::Fstep_excute(){
    ALU exe(buffer_id_ex);
    buffer_ex_ma.modify_rd(buffer_id_ex.read_rd());
    buffer_ex_ma.modify_instt(buffer_id_ex.read_instt());
    buffer_ex_ma.modify_PC(buffer_id_ex.read_PC());
    //buffer_ex_ma.modify_hazard(buffer_id_ex.read_hazard());
    int tmptype = static_cast<int>(buffer_id_ex.read_instt());
    if(tmptype < 40){
        ///关于L和S的解释：
        ///L:在EX阶段仅仅计算出offset;
        ///S:计算offset，并且将rs2处理掉。
        if(tmptype < 20) {
            buffer_ex_ma.modify_mem_offset(exe.LOAD_STORE_offset());///???
            if(tmptype >= 10)buffer_ex_ma.modify_rd_value(exe.STOREr());
            if((tmptype >= 10)&&buffer_ex_ma.read_mem_offset() == (int)0x30004) throw terminate();
        }
        else{
            if(tmptype < 30)buffer_ex_ma.modify_rd_value(exe.ARITHer());
            else buffer_ex_ma.modify_rd_value(exe.LOGICer());
        }
    }else{
        if(tmptype < 60){
            if(tmptype < 50)buffer_ex_ma.modify_rd_value(exe.SHIFTer());
            else buffer_ex_ma.modify_rd_value(exe.COMPer());
        }
        else{
            if(tmptype < 70)buffer_ex_ma.jumpcommon_PC(exe.BRANCHer() - 4);
            else{
                if(tmptype == AUIPC){///AUIPC
                    buffer_ex_ma.jumpcommon_PC(exe.AUIPC() - 4);
                    buffer_ex_ma.modify_rd_value(static_cast<unsigned int>(buffer_ex_ma.read_PC()));
                }else if(tmptype == JAL){///JAL
                    buffer_ex_ma.modify_rd_value(buffer_ex_ma.read_PC() - buffer_id_ex.read_imm() + 4);
                }else if(tmptype == JALR){///JALR
                    buffer_ex_ma.modify_rd_value(static_cast<unsigned int>(buffer_ex_ma.read_PC()));
                    buffer_ex_ma.jumpcommon_PC(exe.JALR() - buffer_id_ex.read_PC());
                }
            }
        }
    }
}
void Parallel_Ctrler::Fstep_MemoryAccess(){
    buffer_ma_wb.modify_instt(buffer_ex_ma.read_instt());
    buffer_ma_wb.modify_rd(buffer_ex_ma.read_rd());
    buffer_ma_wb.modify_rd_value(buffer_ex_ma.read_rd_value());
   // buffer_ma_wb.modify_hazard(buffer_ex_ma.read_hazard());
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
void Parallel_Ctrler::Fstep_WriteBack(){
    int tmptype = static_cast<int>(buffer_ma_wb.read_instt());
    if(tmptype < 20&&tmptype >= 10)///STORE
        return;
    if(tmptype >= 60&&tmptype < 70)///BRANCH
        return;

    if(buffer_ma_wb.read_rd() == 0)return;//x0
    r.write_reg(buffer_ma_wb.read_rd_value(),buffer_ma_wb.read_rd());

    ///unlock
    r.unlock_reg(buffer_ma_wb.read_rd());
}


