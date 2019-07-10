//
// Created by anna_Diao on 2019/7/10.
//

#include "../../header/Pipeline.h"

void FStep_Fetch(memory *m,Register& r,Buffer& buffer){
    Ins_Base *tmpbp = nullptr;
    unsigned int inst_content = m->get_inst(buffer.read_PC());

    unsigned int op = (inst_content&0x7f);
    fetch_sw(tmpbp,op,inst_content);
    buffer.modify_bp(tmpbp);

    buffer.jumpcommon_PC(4);
}

void fetch_sw(Ins_Base* &bp, const unsigned int& op,const unsigned int& inst_content){
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

void Ins_R::Fetch(const unsigned int& inst_content){
    Ins_Base::getfunct7(funct7,inst_content);
    Ins_Base::getfunct3(funct3,inst_content);
    Ins_Base::getrd(rd,inst_content);
    Ins_Base::getrs1(rs1,inst_content);
    Ins_Base::getrs2(rs2,inst_content);
}
void Ins_I::Fetch(const unsigned int& inst_content){
    Ins_Base::getfunct3(funct3,inst_content);
    Ins_Base::getrd(rd,inst_content);
    Ins_Base::getrs1(rs1,inst_content);
    unsigned_imm = (inst_content>>20)&0xfff;
    imm = (((unsigned_imm>>11) == 0b1)?(unsigned_imm|0xfffff000):(unsigned_imm));
}
void Ins_S::Fetch(const unsigned int& inst_content){
    Ins_Base::getfunct3(funct3,inst_content);
    Ins_Base::getrs1(rs1,inst_content);
    Ins_Base::getrs2(rs2,inst_content);
    unsigned_imm = (((inst_content>>25)<<5) + ((inst_content>>7)&0x1f))&0xfff;
    imm = (((unsigned_imm>>11) == 1)?(unsigned_imm|0xfffff000):(unsigned_imm));
}
void Ins_B::Fetch(const unsigned int& inst_content){
    Ins_Base::getfunct3(funct3,inst_content);
    Ins_Base::getrs1(rs1,inst_content);
    Ins_Base::getrs2(rs2,inst_content);
    unsigned_imm = (((inst_content>>7)&0x1e)//4:1
                    + ((inst_content&0x80)<<4)//11
                    +((inst_content>>20)&0x7e0)//5:10
                    + ((inst_content>>19)&0x1000))//12
                   &0x1fff;
    imm = (((unsigned_imm>>12) == 1)?(unsigned_imm|0xffffe000):(unsigned_imm));
}
void Ins_U::Fetch(const unsigned int& inst_content){
    Ins_Base::getrd(rd,inst_content);
    imm = unsigned_imm = inst_content&0xfffff000;
}
void Ins_J::Fetch(const unsigned int& inst_content){
    Ins_Base::getrd(rd,inst_content);
    unsigned_imm = ((inst_content&0xff000)//12:19
                    + ((inst_content>>9)&0b100000000000)//11
                    + ((inst_content>>20)&0x7fe)//1:10
                    + (inst_content>>31<<20))//20
                   &0x1fffff;
    imm = (((unsigned_imm>>20) == 0b1)?(unsigned_imm|0xffe00000):(unsigned_imm));
}