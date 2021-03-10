//
//  ReinforcementGates.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/30/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "ReinforcementGates.hpp"

ReinforcementLogicGate::ReinforcementLogicGate(std::vector<int> _ins,std::vector<int> _outs,std::vector<std::vector<double>> _logic){
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

void ReinforcementLogicGate::update(std::vector<double> &from,std::vector<double> &to){
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
    record.push_back(std::pair<int,int>(I,O));
}

void ReinforcementLogicGate::reinforceGate(double weight,std::vector<double> modulator){
    //apply the feedback
    int c=0;
    for(auto r:record){
        table[r.first][r.second]+=weight*modulator[c];
        c++;
    }
    //renormalize the table
    for(int i=0;i<(int)table.size();i++){
        double sum=0.0;
        for(int o=0;o<(int)table[i].size();o++)
            sum+=table[i][o];
        for(int o=0;o<(int)table[i].size();o++)
            table[i][o]/=sum;
    }
}

void ReinforcementLogicGate::clearReinforcement(){
    record.clear();
}


std::vector<std::pair<int,int>> ReinforcementLogicGate::getRecord(){
    return record;
}

void ReinforcementLogicGate::setRecord(std::vector<std::pair<int,int>> r){
    record=r;
}
