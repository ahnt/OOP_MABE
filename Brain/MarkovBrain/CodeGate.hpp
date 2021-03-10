//
//  CodeGate.hpp
//  NoCompromiseMABE
//
//  Created by Arend Hintze on 2020-08-28.
//  Copyright © 2020 ArendHintze. All rights reserved.
//

#ifndef CodeGate_hpp
#define CodeGate_hpp

//#include <vector>
//#include <iostream>
#include "Gate.hpp"
#include <utility>
#include <deque>
//#include "../../Core/Utilities.hpp"
//#include "../../Core/Random.hpp"
//#include <math.h>
//#include <memory>
//#include <cfloat>
//#include <cstdint>


struct AddrField{
    unsigned char f0:1;
    unsigned char f1:1;
    unsigned char f2:1;
    unsigned char f3:1;
    unsigned char command:4;
    unsigned char e0:4;
    unsigned char addr0:4;
    unsigned char e1:4;
    unsigned char addr1:4;
    unsigned char e2:4;
    unsigned char addr2:4;
};

struct WideAddrField{
    unsigned char _aa,_bb,_cc,_dd;
};

union Instruction{
    int i;
    float f;
    AddrField addrField;
    WideAddrField wideAddrField;
    Instruction(int value=0){i=value;}
};



class CodeGate:public Gate{
    
//std::vector<Instruction> extraType;
    std::vector<std::vector<int>> table;
    std::vector<Instruction> instructions;
    std::vector<int> reg;
    std::vector<int> iStack,oStack;
    std::deque<int> stack;
public:
    CodeGate()=default;
    //CodeGate(){};
    CodeGate(std::vector<int> _ins,std::vector<int> _outs,std::vector<std::vector<int>> _logic,std::vector<int> _theCode);
    virtual void update(std::vector<double> &from,std::vector<double> &to) override;
    virtual void reset() override;
    virtual std::shared_ptr<Gate> clone() override;
    
    virtual std::string serializeToString() override {return std::string("<CodeGate><NOT IMPLEMENTED>");};
    virtual std::string getType() override {return std::string("CodeGate");}
    virtual void show() override;
    virtual std::string dotTag() override { return std::string("CodeGate");};
};


#endif /* CodeGate_hpp */
