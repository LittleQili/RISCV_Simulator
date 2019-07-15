//
// Created by anna_Diao on 2019/7/16.
//

#include "../header/Predictor.h"

TmpRecord::TmpRecord(int xpc):pc(xpc),sum_num(0),success_num(0){};

Predictor::Predictor():branch_all(0),branch_success(0){};

void Predictor::find_tmprec(int pc,std::vector<TmpRecord>::iterator& it){
    std::vector<TmpRecord>::iterator endit = pre_predictor.end();
    while(it != endit)
        if(it->pc == pc) return;
        else ++it;
}

bool Predictor::Judge(int pc){
    if(!predictor.count(pc)){//预处理未完毕
        //在vector中找pc对应的记录
        std::vector<TmpRecord>::iterator it = pre_predictor.begin();
        find_tmprec(pc,it);
        if(it == pre_predictor.end()){//第一次访问到该pc的跳转指令
            pre_predictor.push_back(TmpRecord(pc));///他又提醒我用empalce_back?????
        }//非第一次等待修改即可
        ///这里默认为跳转,可以根据具体测试文件修改
        return true;
    }else return predictor.find(pc)->second;
}

void Predictor::Writeback_result(int pc,bool isequal){
    ++branch_all;
    if(isequal)++branch_success;

    if(!predictor.count(pc)){//涉及vector的更正和写回
        //在vector中找pc对应的记录
        std::vector<TmpRecord>::iterator it = pre_predictor.begin();
        find_tmprec(pc,it);

        if(isequal)++it->success_num;
        if(it->success_num > MAX_guess/2) {
            predictor.insert(std::pair<int, bool>(pc, true));
            pre_predictor.erase(it);
            return;
        }
        ++it->sum_num;
        if(it->sum_num - it->success_num > MAX_guess/2){
            predictor.insert(std::pair<int, bool>(pc, false));
            pre_predictor.erase(it);
            return;
        }
    }//else 仅需将结果记录在总数中。
}

void Predictor::display_result(){
    std::cout << "Branch Predictor's data: \n"
              <<"Total Branch commands' number: " << branch_all << '\n'
              <<"Success Predictions' number: " << branch_success << '\n'
              <<"Rate: "<< static_cast<double>(branch_success) / static_cast<double>(branch_all) * 100
              << '%' ;
}