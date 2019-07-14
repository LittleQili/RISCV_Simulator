//
// Created by anna_Diao on 2019/7/10.
//

#ifndef RISCV_SIMULATOR_REGISTER_H
#define RISCV_SIMULATOR_REGISTER_H

#include "Memory.h"

///发现了寄存器上锁问题：
///由于存在a = a + 4类似的问题，在执行本条语句的时候
//#define REG_LOCKER
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
#ifdef REG_LOCKER
    void debug(){
        static int i = 0;
        std::cout <<std::dec<< i++ << std::endl;

        for(int i = 0;i < 32;++i)
            std::cout <<std::hex <<reg[i] << ' ';

        std::cout << std::endl;
        std::cout << std::endl;
    }
#endif
};

#endif //RISCV_SIMULATOR_REGISTER_H
