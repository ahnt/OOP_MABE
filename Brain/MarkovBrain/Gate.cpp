//
//  Gate.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/16/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "Gate.hpp"

/** logic gate **/

LogicGate::LogicGate(std::vector<int> _ins,std::vector<int> _outs,std::vector<std::vector<int>> _logic){
    ins=_ins;
    outs=_outs;
    logic=_logic;
}

std::shared_ptr<Gate> LogicGate::clone() {
    std::shared_ptr<LogicGate> G=std::make_shared<LogicGate>(ins,outs,logic);
    return std::dynamic_pointer_cast<Gate>(G);
}

void LogicGate::update(std::vector<double> &from,std::vector<double> &to){
    int I=0;
    for(int i=0;i<(int)ins.size();i++)
        I=(I<<1)+bit(from[ins[i]]);
    for(int o=0;o<(int)outs.size();o++)
        to[outs[o]]+=logic[I][o];
}

void LogicGate::reset(){
    
}

std::string LogicGate::serializeToString(){
    std::string S="<Logic>";
    S+="Ins: "+serializeVectorToString(ins,",")+"\n";
    S+="Outs: "+serializeVectorToString(outs, ",")+"\n";
    S+="Logic:\n";
    for(int v=0;v<(int)logic.size();v++){
        S+=serializeVectorToString(logic[v], ",")+"\n";
    }
    return S;
}

void LogicGate::show(){
    std::cout<<"ins: ";
    for(auto I:ins){
        std::cout<<I<<" ";
    }
    std::cout<<std::endl<<"outs :";
    for(auto O:outs){
        std::cout<<O<<" ";
    }
    std::cout<<std::endl;
}

std::string LogicGate::dotTag(){
    //A0101
    //B0011
    // 0000 ZERO
    // 0001 AND
    // 0010 B&!A
    // 0011 B
    // 0100 A&!B
    // 0101 A
    // 0110 XOR
    // 0111 OR
    // 1000 NOR
    // 1001 EQU
    // 1010 !A
    // 1011 RA
    // 1100 !B
    // 1101 RB
    // 1110 NAND
    // 1111 ONE
    if((ins.size()==2)&&(outs.size()==1)){
        std::vector<std::string> tags=std::vector<std::string>({"ZERO","AND","B&!A","B","A&!B","A","XOR","OR","NOR","EQU","!A","!RA","!B","!RB","NAND","ONE"});
        int a=0;
        for(int b=0;b<4;b++)
            a=(a<<1)+logic[b][0];
        return tags[a];
    }
    return std::string("detGate");
}


/** probabilistic logic gate **/
ProbabilisticLogicGate::ProbabilisticLogicGate(std::vector<int> _ins,std::vector<int> _outs,std::vector<std::vector<double>> _logic){
    ins=_ins;
    outs=_outs;
    table=_logic;
    for(int i=0;i<(int)table.size();i++){
        double sum=0.0;
        for(int o=0;o<(int)table[i].size();o++){
            table[i][o]=abs(table[i][o]);
            sum+=table[i][o];
        }
        if(sum>0.0)
            for(int o=0;o<(int)table[i].size();o++)
                table[i][o]/=sum;
        else
            for(int o=0;o<(int)table[i].size();o++)
                table[i][o]=1.0/(double)table[i].size();

    }
}

std::shared_ptr<Gate> ProbabilisticLogicGate::clone() {
    std::shared_ptr<ProbabilisticLogicGate> G=std::make_shared<ProbabilisticLogicGate>(ins,outs,table);
    return std::dynamic_pointer_cast<Gate>(G);
}

void ProbabilisticLogicGate::update(std::vector<double> &from,std::vector<double> &to){
    int I=0;
    int O=0;
    for(int i=0;i<(int)ins.size();i++)
        I=(I<<1)+bit(from[ins[i]]);

    double r=Random::getFloat(0.0,1.0);
    while(r>table[I][O]){
        r-=table[I][O];
        O=(O+1)%table[I].size();
    }
    for(int o=0;o<(int)outs.size();o++)
        to[outs[o]]+=(double)((O>>o)&1);
}

void ProbabilisticLogicGate::reset(){
}

std::string ProbabilisticLogicGate::serializeToString(){
    std::string S="<ProbabilisticLogic>";
    S+="Ins: "+serializeVectorToString(ins,",")+"\n";
    S+="Outs: "+serializeVectorToString(outs, ",")+"\n";
    S+="Table:\n";
    for(int v=0;v<(int)table.size();v++){
        S+=serializeVectorToString(table[v], ",")+"\n";
    }
    return S;
}
