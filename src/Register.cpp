//
// Created by anna_Diao on 2019/7/10.
//

#include "../header/Register.h"

Register::Register() {
    reg[0] = 0;
}

unsigned int Register::get_reg(int pos)const{
    return reg[pos];
}

void Register::write_reg(unsigned int content,int pos){
    reg[pos] = content;
}