//
// Created by anna_Diao on 2019/7/10.
//

#ifndef RISCV_SIMULATOR_REGISTER_H
#define RISCV_SIMULATOR_REGISTER_H

#include "Memory.h"

///发现了寄存器上锁问题：
///由于存在a = a + 4类似的问题，在执行本条语句的时候IF阶段不上锁，移到ID阶段
class Register{
private:
    unsigned int reg[32];
    int locker[32];
public:
    Register();

    unsigned int get_reg(int)const;

    void write_reg(unsigned int content,int pos);

    void lock_reg(int pos);

    void unlock_reg(int);

    bool isreglocked(int pos)const;
};

#endif //RISCV_SIMULATOR_REGISTER_H
