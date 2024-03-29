//
// Created by anna_Diao on 2019/7/10.
//

#include "../header/Buffer.h"

Buffer_IF_ID::Buffer_IF_ID():bp(nullptr),pc(0),hazard(NON){};
Buffer_IF_ID::~Buffer_IF_ID(){
    delete bp;
}
void Buffer_IF_ID::modify_bp(Ins_Base* xbp){
    if(bp != nullptr&&bp != xbp) delete bp;
    bp = xbp;
}
Ins_Base* Buffer_IF_ID::read_bp(){return bp;}
int Buffer_IF_ID::read_PC(){return pc;}
void Buffer_IF_ID::jumpcommon_PC(int step){pc += step;}
void Buffer_IF_ID::modify_PC(int xpc){pc = xpc;}
void Buffer_IF_ID::modify_hazard(HazardT x){hazard = x;}
HazardT Buffer_IF_ID::read_hazard(){return hazard;}
int Buffer_IF_ID::read_pc_beforeIF(){return pc_beforeIF;}
void Buffer_IF_ID::modify_pc_beforeIF(int x){pc_beforeIF = x;}

void Buffer_ID_EX::modify_PC(int xpc){pc = xpc;}
int Buffer_ID_EX::read_PC(){return pc;}
void Buffer_ID_EX::modify_instt(InstT xinstt){instt = xinstt;}
InstT Buffer_ID_EX::read_instt(){return instt;}
void Buffer_ID_EX::modify_rs1_content(unsigned int x){rs1_content = x;}
unsigned int Buffer_ID_EX::read_rs1_content(){ return  rs1_content;}
void Buffer_ID_EX::modify_rs2_content(unsigned int x){rs2_content = x;}
unsigned int Buffer_ID_EX::read_rs2_content(){ return rs2_content;}
void Buffer_ID_EX::modify_rd(unsigned int x){rd = x;}
unsigned int Buffer_ID_EX::read_rd(){ return rd;}

void Buffer_ID_EX::modify_rs1(unsigned int x){rs1 = x;}
unsigned int Buffer_ID_EX::read_rs1(){ return rs1;}
void Buffer_ID_EX::modify_rs2(unsigned int x){rs2 = x;}
unsigned int Buffer_ID_EX::read_rs2(){ return rs2;}

int Buffer_ID_EX::read_pc_pre(){ return pc_pre;}
void Buffer_ID_EX::modify_pc_pre(int x){pc_pre = x;}
int Buffer_ID_EX::read_pc_beforeIF(){return pc_beforeIF;}
void Buffer_ID_EX::modify_pc_beforeIF(int x){pc_beforeIF = x;}

void Buffer_ID_EX::modify_unsigned_imm(unsigned int x){unsigned_imm = x;}
unsigned int Buffer_ID_EX::read_unsigned_imm(){return unsigned_imm;}
void Buffer_ID_EX::modify_imm(unsigned int x){imm = x;}
unsigned int Buffer_ID_EX::read_imm(){return imm;}
void Buffer_ID_EX::modify_hazard(HazardT x){hazard = x;}
HazardT Buffer_ID_EX::read_hazard(){return hazard;}

void Buffer_EX_MA::modify_PC(int xpc){pc = xpc;}
void Buffer_EX_MA::jumpcommon_PC(int step){pc += step;}
int Buffer_EX_MA::read_PC(){return pc;}
void Buffer_EX_MA::modify_rd(unsigned int x){rd = x;}
unsigned int Buffer_EX_MA::read_rd(){ return rd;}
void Buffer_EX_MA::modify_rd_value(unsigned int x){rd_value = x;}
unsigned int Buffer_EX_MA::read_rd_value(){ return rd_value;}
void Buffer_EX_MA::modify_instt(InstT xinstt){instt = xinstt;}
InstT Buffer_EX_MA::read_instt(){return instt;}
void Buffer_EX_MA::modify_mem_offset(int x){mem_offset = x;}
int Buffer_EX_MA::read_mem_offset(){return mem_offset;}
void Buffer_EX_MA::send_rd_value(Buffer_ID_EX& buffer_id_ex){
    if(rd == 0)return;
    if(rd == buffer_id_ex.read_rs1())
        buffer_id_ex.modify_rs1_content(rd_value);

    if(rd == buffer_id_ex.read_rs2())
        buffer_id_ex.modify_rs2_content(rd_value);
}

void Buffer_MA_WB::modify_rd_value(unsigned int x){rd_value = x;}
unsigned int Buffer_MA_WB::read_rd_value(){ return rd_value;}
void Buffer_MA_WB::modify_rd(unsigned int x){rd = x;}
unsigned int Buffer_MA_WB::read_rd(){ return rd;}
void Buffer_MA_WB::modify_instt(InstT xinstt){instt = xinstt;}
InstT Buffer_MA_WB::read_instt(){return instt;}
void Buffer_MA_WB::send_rd_value(Buffer_ID_EX& buffer_id_ex){
    if(rd == 0)return;
    if(rd == buffer_id_ex.read_rs1())
        buffer_id_ex.modify_rs1_content(rd_value);

    if(rd == buffer_id_ex.read_rs2())
        buffer_id_ex.modify_rs2_content(rd_value);
}
