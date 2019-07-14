//
// Created by anna_Diao on 2019/7/13.
//

#include "../header/Parallel.h"

Parallel_Ctrler::Parallel_Ctrler(){
    m = new memory;
    m->inimem();
    for(int i = 0;i < 5;++i) isready[i] = false;
}
Parallel_Ctrler::~Parallel_Ctrler(){
    delete m;
}

void Parallel_Ctrler::FStep_Fetch(){
    Ins_Base *tmpbp = nullptr;

    unsigned int inst_content = m->get_inst(buffer_if_id.read_PC());
    unsigned int op = (inst_content&0x7f);

    fetch_sw(tmpbp,op,inst_content);

    buffer_if_id.modify_bp(tmpbp);

    //判断hazard类型
    ///这里把细判的版本改成这样的粗略判断。
     InstT tmpinstt = tmpbp->instt;

    if(tmpinstt == Command_family){
        if(r.isreglocked(tmpbp->rs1)||r.isreglocked(tmpbp->rs2))
            buffer_if_id.modify_hazard(BOTH);
        else buffer_if_id.modify_hazard(CONTROL);
    }
    if(tmpinstt == JALR){
        if(r.isreglocked(tmpbp->rs1))buffer_if_id.modify_hazard(BOTH);
        else buffer_if_id.modify_hazard(CONTROL);
    }

    if((tmpinstt == Store_family||op == 0b0110011)
       &&r.isreglocked(tmpbp->rs2)){
        buffer_if_id.modify_hazard(DATA);
    }else if((tmpinstt != LUI&&tmpinstt != AUIPC&&tmpinstt != JAL&&tmpinstt != Command_family&&tmpinstt != JALR)
       &&r.isreglocked(tmpbp->rs1))
        buffer_if_id.modify_hazard(DATA);

    //上锁
    buffer_if_id.modify_Locknext(false);
    if(tmpbp->instt != Command_family&&tmpbp->instt != Store_family){
        if(tmpbp->rd != tmpbp->rs1&&tmpbp->rd != tmpbp->rs2)r.lock_reg(tmpbp->rd);
        else buffer_if_id.modify_Locknext(true);
    }

    if(tmpinstt == JAL){
        buffer_if_id.jumpcommon_PC(tmpbp->imm);
    }else if(tmpinstt == AUIPC){
        buffer_if_id.jumpcommon_PC(tmpbp->unsigned_imm);
    }else buffer_if_id.jumpcommon_PC(4);
}
///这里留住了后几个指令类型的家族分类操作。不知道后面有用否？
///repaly 2019/7/14:非常有用，减少了判断hazard类型的时间。
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

void Parallel_Ctrler::Run_Parallel(){
    while(1){
        //WB
        if(isready[4]){
            Fstep_WriteBack();
            isready[4] = false;

            if(!isready[3]&&!isready[2]&&!isready[1]) {
                if (buffer_if_id.read_hazard() == BOTH) {
                    isready[1] = true;
                    buffer_if_id.modify_hazard(CONTROL);
                } else if (buffer_if_id.read_hazard() == DATA) {
                    buffer_if_id.modify_hazard(NON);
                    isready[1] = true;
                }
            }
        }
        //MA
        if(isready[3]){
            Fstep_MemoryAccess();
            isready[3] = false;
            isready[4] = true;
        }
        //EX
        if(isready[2]){
            try {
                Fstep_excute();
            }
            catch (terminate) {
                Fstep_WriteBack();
                std::cout << std::dec << ((int) r.get_reg(10) & 0XFF);
                delete m;
                return ;
            }
            if(buffer_id_ex.read_hazard() == CONTROL){
                buffer_if_id.modify_PC(buffer_ex_ma.read_PC());//MUX_GREEN写到这一步里了
                buffer_if_id.modify_hazard(NON);
            }
            isready[2] = false;
            isready[3] = true;
        }
        //ID
        ///这个步骤中使用了寄存器，进行上锁操作
        if(isready[1]){
            Fstep_Decode();
            isready[1] = false;
            isready[2] = true;
        }
        //IF
        ///原本这里不能接触register,但是为了方便给rd上锁，所以在FStep_Fetch（）对寄存器做了上锁的操作。
        ///该处Step_Fetch也对hazard做修改操作。
        if(buffer_if_id.read_hazard() == NON){
            FStep_Fetch();
            if(buffer_if_id.read_hazard() == NON||buffer_if_id.read_hazard() == CONTROL)///confirm
                isready[1] = true;
        }
    }
};


