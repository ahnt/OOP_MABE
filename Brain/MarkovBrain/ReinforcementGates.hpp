//
//  ReinforcementGates.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/30/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef ReinforcementGates_hpp
#define ReinforcementGates_hpp

#include "Gate.hpp"
#include "../../Gadgets/CPPN/CPPN.hpp"
#include <utility>

class ReinforcementLogicGate:public ProbabilisticLogicGate{
    std::vector<std::pair<int,int>> record;
    
public:
    ReinforcementLogicGate(std::vector<int> _ins,std::vector<int> _outs,std::vector<std::vector<double>> _logic);
    virtual std::string getType() override {return std::string("ReinforcementLogicGate");}
    virtual void reinforceGate(double weight,std::vector<double> modulator) override;
    virtual void clearReinforcement() override;
    virtual void update(std::vector<double> &from,std::vector<double> &to) override;

    virtual std::vector<std::pair<int,int>> getRecord() override;
    virtual void setRecord(std::vector<std::pair<int,int>> r) override;
};

#endif /* ReinforcementGates_hpp */
