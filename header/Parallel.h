//
// Created by anna_Diao on 2019/7/13.
//

#ifndef RISCV_SIMULATOR_PARALLEL_H
#define RISCV_SIMULATOR_PARALLEL_H

#include "ALU.h"

#define DEBUG_PARALLEL

class Parallel_Ctrler{
private:
    //enum HazradT {NON,DATA,CONTROL};
    memory *m;
    Register r;

    Buffer_IF_ID buffer_if_id;
    Buffer_ID_EX buffer_id_ex;
    Buffer_EX_MA buffer_ex_ma;
    Buffer_MA_WB buffer_ma_wb;

    bool isready[5];
    bool isIFlocked;

    char hazard;//n:NON,d:DATA,c:CONTROL,b:both

    void fetch_sw(Ins_Base* &bp, const unsigned int& op,const unsigned int& inst_content);
    void FStep_Fetch();
    void Fstep_Decode();
    ///这个步骤中buffer_ex_ma仅有memoffset用到read操作。
    void Fstep_excute();
    void Fstep_MemoryAccess();
    void Fstep_WriteBack();
public:
    Parallel_Ctrler();
    ~Parallel_Ctrler();

    void Run_Parallel(){
#ifdef DEBUG_PARALLEL
        bool flag;
#endif
        while(1){
#ifdef DEBUG_PARALLEL
           flag = false;
#endif
            //WB
            if(isready[4]){
#ifdef DEBUG_PARALLEL
                flag = true;
#endif
                Fstep_WriteBack();
                isready[4] = false;
            }
            //MA
            if(isready[3]){
#ifdef DEBUG_PARALLEL
                flag = true;
#endif
                Fstep_MemoryAccess();
                isready[3] = false;
                isready[4] = true;
            }
            //EX
            if(isready[2]){
#ifdef DEBUG_PARALLEL
                flag = true;
#endif
                try {
                    Fstep_excute();
                }
                catch (terminate) {
                    std::cout << std::dec << ((int) r.get_reg(10) & 0XFF);
                    delete m;
                    return ;
                }
                buffer_if_id.modify_PC(buffer_ex_ma.read_PC());//MUX_GREEN写到这一步里了
                isready[2] = false;
                isready[3] = true;
            }
            //ID
            if(isready[1]){
#ifdef DEBUG_PARALLEL
                flag = true;
#endif
                Fstep_Decode();
                isready[1] = false;
                isready[2] = true;
            }
            //IF
            ///原本这里不能接触register,但是为了方便给rd上锁，所以在FStep_Fetch（）对寄存器做了上锁的操作。
            ///该处Step_Fetch也对hazard做修改操作。
            if(hazard == 'n'){
#ifdef DEBUG_PARALLEL
                flag = true;
#endif
                FStep_Fetch();
                isready[1] = true;
            }
#ifdef DEBUG_PARALLEL

if(flag == false)throw exception::Parallel_nonactive();
#endif
        }
    };
};

#endif //RISCV_SIMULATOR_PARALLEL_H
