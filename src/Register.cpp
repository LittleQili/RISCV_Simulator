//
// Created by anna_Diao on 2019/7/10.
//

#include "../header/Register.h"

Register::Register() {
    for(int i = 0;i < 32;++i){
        reg[i] = 0;
        //locker[i] = 0;
    }
}

unsigned int Register::get_reg(int pos)const{
    if(pos < 0||pos > 31)
        throw exception::Register_Reg_outofbound();
    return reg[pos];
}

void Register::write_reg(unsigned int content,int pos){
    reg[pos] = content;
}
/*
void Register::lock_reg(int pos){
    if(pos != 0)++locker[pos];
}
void Register::unlock_reg(int pos){
    if(pos != 0)--locker[pos];
}

bool Register::isreglocked(int pos)const{
    return locker[pos] != 0;
}*/