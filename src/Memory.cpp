//
// Created by anna_Diao on 2019/7/10.
//

#include "../header/Memory.h"

memory::memory() {
    mem = new char[MEM_MAX];
}

memory::~memory() {
    delete[] mem;
}

void memory::inimem(){
        char gap;

        unsigned int changer;
        int offset = 0;

        unsigned short tmpin = 0;
        const unsigned short zero = 0;
        int old = offset;
        std::cin.get(gap);
        while(gap != EOF){
            switch(gap){
                case '@':
                    std::cin >>std::hex>> changer;
                    old = offset;
                    offset = 0;
                    offset += static_cast<int>(changer);
                    for(int i = old;i < offset;++i){
                        memcpy(mem+i,&zero,sizeof(char));
                    }
                    break;
                case '\n':
                case ' ':
                    break;
                default:
                    //以下先输入第一个char里面的内容
                    std::cin >> std::hex >> tmpin;
                    if(gap <='9'&&gap>='0'){
                        tmpin += (gap - '0') * 16;
                    }else if(gap <='F'&&gap >= 'A'){
                        tmpin += (gap - 'A' + 10) * 16;
                    }else if(gap <='f'&&gap >= 'a'){
                        tmpin += (gap - 'a' + 10) * 16;
                    }else throw exception::Memory_invalid_input();
                    memcpy(mem+offset,&tmpin, sizeof(char));

                    //输入后三个char的内容
                    std::cin >> std::hex >> tmpin;
                    memcpy(mem+(++offset),&tmpin, sizeof(char));
                    std::cin >> std::hex >> tmpin;
                    memcpy(mem+(++offset),&tmpin, sizeof(char));
                    std::cin >> std::hex >> tmpin;
                    memcpy(mem+(++offset),&tmpin, sizeof(char));
                    ++offset;
                    break;
            }
            std::cin.get(gap);
        }
}

unsigned int memory::get_inst(int offset){
    unsigned int inst;
    memcpy(&inst,mem + offset, sizeof(char) * 4);
    return inst;
}

unsigned int memory::read_content(int offset,int length){
    unsigned int n = 0,tmpn = 0;
    memcpy(&tmpn,mem+offset, sizeof(char));
    n += tmpn;
    if(length == 1)return n;

    tmpn = 0;memcpy(&tmpn,mem+offset+1, sizeof(char));
    n += (tmpn << 8);
    if(length == 2)return n;

    tmpn = 0;memcpy(&tmpn,mem+offset+2, sizeof(char));
    n += (tmpn << 16);
    tmpn = 0;memcpy(&tmpn,mem+offset+3, sizeof(char));
    n += (tmpn << 24);

    return n;
}

void memory::write_content(int offset,size_t length,unsigned int content){
    unsigned tmpcon = (content&0xff);
    memcpy(mem + offset,&tmpcon, sizeof(char));
    if(length == 1)return;

    tmpcon = (content>>8)&0xff;
    memcpy(mem + offset + 1,&tmpcon,sizeof(char));
    if(length == 2)return;

    tmpcon = (content>>16)&0xff;
    memcpy(mem + offset + 2,&tmpcon,1);
    tmpcon = (content>>24)&0xff;
    memcpy(mem + offset + 3,&tmpcon,1);

}