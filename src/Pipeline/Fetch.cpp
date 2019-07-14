//
// Created by anna_Diao on 2019/7/10.
//

#include "../../header/Pipeline.h"

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

