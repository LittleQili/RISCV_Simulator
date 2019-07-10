//
// Created by anna_Diao on 2019/7/10.
//

#ifndef RISCV_SIMULATOR_MEMORY_H
#define RISCV_SIMULATOR_MEMORY_H

#include <iostream>
#include <cstring>
#include "exception.h"

class memory{
private:
    static const int MEM_MAX = 0x400000;
    char *mem;
public:
    memory();
    ~memory();
    void inimem();

    unsigned int get_inst(int);

    unsigned read_content(int offset,int length);

    void write_content(int offset,size_t length,unsigned int content);
};

#endif //RISCV_SIMULATOR_MEMORY_H
