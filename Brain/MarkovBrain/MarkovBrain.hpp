//
//  MarkovBrain.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef MarkovBrain_hpp
#define MarkovBrain_hpp

#include "../Brain.hpp"
#include "Gate.hpp"
#include "HyperGates.hpp"
#include "ReinforcementGates.hpp"
#include "CodeGate.hpp"
#include "../../Gadgets/CPPN/CPPN.hpp"
#include "GPGates.hpp"
#include <set>

class MarkovBrain:public Brain{
    int currentVector;
    std::vector<std::vector<double>> state;
    int nrOfHiddenStates;
    int totalNrOfStates;
    
public:
    std::vector<std::shared_ptr<Gate>> gates;
    
    static std::shared_ptr<Brain> getTemplate();
    virtual std::shared_ptr<Brain> makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts) override;
    virtual std::shared_ptr<Brain> clone() override;
    virtual void update() override;

    virtual void setInput(int which, double toWhat) override;
    virtual double getOutput(int which) override;
    virtual void setOutput(int which, double toWhat) override;
    virtual std::vector<double> getHidden() override;
    virtual void setHidden(int which,double toWhat) override;
    virtual void clearOutput() override;

    
    virtual void resetBrain() override;
    virtual std::string serializeToString() override;
    virtual void addBrainStatsToDataCollector(std::shared_ptr<DataCollector> dataCollector) override;
    virtual std::string renderToDot() override;
    virtual std::vector<std::vector<int>> getConnectivityMatrix() override;
    
    
};

#endif /* MarkovBrain_hpp */
