//
// Created by anna_Diao on 2019/7/16.
//

#include "../header/Predictor.h"

Record::Record():isjump(true),jump(0),unjump(0){};
Record& Record::operator=(const Record& other){
    isjump = other.isjump;
    jump = other.jump;
    unjump = other.unjump;
    recent_record = other.recent_record;
    return *this;
}

_Predictor::_Predictor():branch_all(0),branch_success(0){};
bool _Predictor::Judge(int pc){
    if(!predictor.count(pc)){
        predictor.insert(std::pair<int, Record>(pc, Record()));
        return true;
    }
    else return predictor.find(pc)->second.isjump;
}
void _Predictor::Writeback_result(int pc,bool isequal){
    ++branch_all;
    if(isequal)++branch_success;
    std::map<int,Record>::iterator it = predictor.find(pc);
    Record tmpr = it->second;

    if(isequal){
        tmpr.recent_record.push(tmpr.isjump);
        if(tmpr.isjump)++tmpr.jump;
        else ++tmpr.unjump;
    }else {
        tmpr.recent_record.push(!tmpr.isjump);
        if(tmpr.isjump)++tmpr.unjump;
        else ++tmpr.jump;
    }
    //dequeue
    if(tmpr.recent_record.size() == MAX_guess){
        if(tmpr.recent_record.front()) --tmpr.jump;
        else --tmpr.unjump;
        tmpr.recent_record.pop();
    }

    tmpr.isjump = (tmpr.jump >= tmpr.unjump);

    it->second = tmpr;
}
void _Predictor::display_result(){
    std::cout << "Branch Predictor's data: \n"
              <<"Total Branch commands' number: " << branch_all << '\n'
              <<"Success Predictions' number: " << branch_success << '\n'
              <<"Rate: "<< static_cast<double>(branch_success) / static_cast<double>(branch_all) * 100
              << '%' ;
}