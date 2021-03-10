//
//  GPGates.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/24/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "GPGates.hpp"

GPGate::GPGate(std::vector<int> _ins,std::vector<int> _outs,int opperator,std::vector<double> constants){
    ins=_ins;
    outs=_outs;
    theOpperator=opperator&7;
    theConstants=constants;
}

void GPGate::update(std::vector<double> &from,std::vector<double> &to){
    int result=0.0;
    switch(theOpperator){
        case 0: //all 0.0
            break;
        case 1: //all 1.0
            for(int o=0;o<(int)outs.size();o++)
                to[outs[o]%(int)to.size()]+=1.0;
            break;
        case 2: // const values
            for(int o=0;o<(int)outs.size();o++)
                to[outs[o]%to.size()]+=theConstants[o];
            break;
        case 3: //sum
            for(auto i:ins)
                result+=from[i%(int)from.size()];
            for(auto o:outs)
                to[o%to.size()]+=result;
            break;
        case 4: //product
            result=1.0;
            for(auto i:ins)
                result*=from[i%(int)from.size()];
            for(auto o:outs)
                to[o%to.size()]+=result;
            break;
        case 5: //subtraction
            result=from[ins[0]%(int)from.size()];
            for(int i=1;i<(int)ins.size();i++)
                result-=from[ins[i]%(int)from.size()];
            for(auto o:outs)
                to[o%to.size()]+=result;
            break;
        case 6: //division
            result=from[ins[0]%(int)from.size()];
            for(int i=1;i<(int)ins.size();i++)
                if(from[ins[i]%(int)from.size()]!=0.0){
                    result/=from[ins[i]%(int)from.size()];
                }
            for(auto o:outs)
                to[o%to.size()]+=result;
            break;
        case 7: //comperator
            result=from[ins[0]%(int)from.size()];
            {
            bool b=true;
            for(int i=1;i<(int)ins.size();i++)
                if(result!=from[ins[i]%(int)from.size()])
                    b=false;

            if(b)
                for(int o=0;o<(int)outs.size();o++)
                    to[outs[o]%(int)to.size()]+=theConstants[o];
            }
            break;
    }
}

std::shared_ptr<Gate> GPGate::clone(){
    std::shared_ptr<GPGate> G=std::make_shared<GPGate>(ins,outs,theOpperator,theConstants);
    return std::dynamic_pointer_cast<Gate>(G);
}
