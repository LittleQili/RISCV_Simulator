//
// Created by 13683 on 2019/7/10.
//

#include "../header/Buffer.h"

Buffer::Buffer():pc(0),bp(nullptr){}

Buffer::~Buffer(){delete bp;}

void Buffer::modify_bp(Ins_Base* xbp){
    if(bp != nullptr&&bp != xbp) delete bp;
    bp = xbp;
}
Ins_Base* Buffer::read_bp(){return bp;}

int Buffer::read_PC(){return pc;}
void Buffer::jumpcommon_PC(int step = 4){
    if(pc + step < 0||pc + step > 0x400000 - 4)
        throw exception::Buffer_PC_outofbound();
    pc += step;
}

void Buffer::modify_rs1_content(unsigned int x){rs1_content = x;}
unsigned int Buffer::read_rs1_content(){return rs1_content;}

void Buffer::modify_rs2_content(unsigned int x){rs2_content = x;}
unsigned int Buffer::read_rs2_content(){return rs2_content;}

void Buffer::modify_rd_value(unsigned int x){rd_value = x;}
unsigned int Buffer::read_rd_value(){return rd_value;}

void Buffer::modify_mem_offset(int x){mem_offset = x;}
int Buffer::read_mem_offset(){return mem_offset;}