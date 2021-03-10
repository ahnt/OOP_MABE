//
//  HyperGates.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/24/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef HyperGates_hpp
#define HyperGates_hpp

#include "Gate.hpp"
#include "../../Gadgets/CPPN/CPPN.hpp"
#include <utility>

class RegularHyperGate:public LogicGate{
public:
    RegularHyperGate(std::vector<int> _ins,std::vector<int> _outs,double xOffset,double yOffset,double xSpacing, double ySpacing,std::shared_ptr<CPPN> cppn);
    virtual std::string getType(){return std::string("RegularHyperGate");}
};

class IrregularHyperGate:public LogicGate{
public:
    IrregularHyperGate(std::vector<int> _ins,std::vector<int> _outs,std::vector<std::vector<std::pair<double,double>>> &_offsetMatrix,std::shared_ptr<CPPN> cppn);
    virtual std::string getType(){return std::string("IrregularHyperGate");}
};

class IndividualRegularHyperGate:public LogicGate{
    std::shared_ptr<CPPN> cppn;
public:
    IndividualRegularHyperGate(std::vector<int> _ins,std::vector<int> _outs,std::shared_ptr<Genome::ReadHead> readHead,int CPPNcomplexity);
    virtual std::string getType(){return std::string("IndividualRegularHyperGate");}
};

class IndividualIrregularHyperGate:public LogicGate{
    std::shared_ptr<CPPN> cppn;
public:
    IndividualIrregularHyperGate(std::vector<int> _ins,std::vector<int> _outs,std::shared_ptr<Genome::ReadHead> readHead,int CPPNcomplexity);
    virtual std::string getType(){return std::string("IndividualIrregularHyperGate");}
};
#endif /* HyperGates_hpp */
