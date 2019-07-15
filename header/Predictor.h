//
// Created by anna_Diao on 2019/7/15.
//

#ifndef RISCV_SIMULATOR_PREDICTOR_H
#define RISCV_SIMULATOR_PREDICTOR_H

#include <vector>
#include <map>
#include <iostream>
///要注意，，如果预测一条错误的指令，并且在判断出来结果之前又有一条需要预测的指令，
/// 一定要将后者的结果清掉，并且不做记录，不然将会惹出小麻烦（哈
///所以说在ex之后做result操作，再将结果写回map或者是vector
///这种情况真实恶心，我只能在judge和result的时候都做查找.........速度可怜
///vector暂时顺序查找...= =

struct TmpRecord{
    int pc;
    long long sum_num;
    long long success_num;
    TmpRecord(int xpc);
};

class Predictor{
private:
    const int MAX_guess = 9;

    std::vector<TmpRecord> pre_predictor;
    std::map<int,bool> predictor;
    long long branch_all;
    long long branch_success;

    void find_tmprec(int pc,std::vector<TmpRecord>::iterator& it);
public:
    Predictor();
    bool Judge(int pc);
    void Writeback_result(int pc,bool isequal);
    void display_result();
};

#endif //RISCV_SIMULATOR_PREDICTOR_H
