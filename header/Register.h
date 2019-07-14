//
// Created by anna_Diao on 2019/7/10.
//

#ifndef RISCV_SIMULATOR_REGISTER_H
#define RISCV_SIMULATOR_REGISTER_H

#include "Memory.h"

class Register{
private:
    unsigned int reg[32];
public:
    Register();

    unsigned int get_reg(int)const;

    void write_reg(unsigned int content,int pos);
};

#endif //RISCV_SIMULATOR_REGISTER_H
