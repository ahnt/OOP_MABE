//
//  GPGates.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/24/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef GPGates_hpp
#define GPGates_hpp

#include "Gate.hpp"
#include <utility>

class GPGate:public LogicGate{
    int theOpperator;
    std::vector<double> theConstants;
public:
    GPGate(std::vector<int> _ins,std::vector<int> _outs,int opperator,std::vector<double> constants);
    virtual std::string getType() override {return std::string("GPGate");}
    virtual void update(std::vector<double> &from,std::vector<double> &to) override;
    virtual void reset() override {};
    virtual std::string serializeToString() override {return std::string(std::to_string(theOpperator));};
    
    virtual void show() override {std::cout<<getType()<<"Gpgate can't be shown right now"<<std::endl; exit(0);};
    virtual std::shared_ptr<Gate> clone() override;

};

#endif /* GPGates_hpp */
