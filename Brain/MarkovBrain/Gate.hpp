//
//  Gate.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/16/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef Gate_hpp
#define Gate_hpp

#include <vector>
#include <iostream>
#include "../../Core/Utilities.hpp"
#include "../../Core/Random.hpp"
#include <math.h>
#include <memory>


class Gate{
public:
    std::vector<int> ins,outs;
    
    Gate()=default;
    virtual void update(std::vector<double> &from,std::vector<double> &to)=0;
    virtual void reset()=0;
    virtual std::string serializeToString()=0;
    virtual std::string getType()=0;
    
    virtual void reinforceGate(double weight,std::vector<double> modulator){}
    virtual void clearReinforcement(){}
    virtual std::vector<std::pair<int,int>> getRecord(){ return std::vector<std::pair<int,int>>();}
    virtual void setRecord(std::vector<std::pair<int,int>> r){}
    virtual void show(){}
    virtual std::string dotTag()=0;
    virtual std::shared_ptr<Gate> clone() {std::cout<<getType()<<" can't be cloned"<<std::endl; exit(0); return nullptr;}
};

class LogicGate:public Gate{
public:
    std::vector<std::vector<int>> logic;
    LogicGate()=default;
    LogicGate(std::vector<int> _ins,std::vector<int> _outs,std::vector<std::vector<int>> _logic);
    virtual void update(std::vector<double> &from,std::vector<double> &to) override;
    virtual void reset() override;
    virtual std::string serializeToString() override;
    virtual std::string getType() override {return std::string("LogicGate");}
    virtual void show() override;
    virtual std::string dotTag() override;
    virtual std::shared_ptr<Gate> clone() override;
};

class ProbabilisticLogicGate:public Gate{
public:
    std::vector<std::vector<double>> table;
    
    ProbabilisticLogicGate()=default;
    ProbabilisticLogicGate(std::vector<int> _ins,std::vector<int> _outs,std::vector<std::vector<double>> _logic);
    virtual void update(std::vector<double> &from,std::vector<double> &to) override;
    virtual void reset() override;
    virtual std::string serializeToString() override;
    virtual std::string getType() override {return std::string("ProbabilisticLogicGate");}
    virtual std::string dotTag() override { return std::string("prabGate");}
    virtual std::shared_ptr<Gate> clone() override;
};


#endif /* Gate_hpp */
