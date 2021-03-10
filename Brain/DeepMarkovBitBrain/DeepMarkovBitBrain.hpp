//
//  DeepMarkovBitBrain.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/28/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef DeepMarkovBitBrain_hpp
#define DeepMarkovBitBrain_hpp

#include "../Brain.hpp"
#include "../MarkovBrain/Gate.hpp"
#include "../MarkovBrain/ReinforcementGates.hpp"
#include <set>

class DeepMarkovBitBrain:public Brain{
    
public:
    std::vector<std::vector<double>> state;
    std::vector<std::vector<std::shared_ptr<Gate>>> gates;
    int nrOfHiddenStates;
    
    static std::shared_ptr<Brain> getTemplate();
    virtual std::shared_ptr<Brain> makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts) override;
    virtual void update() override;
    
    virtual void setInput(int which, double toWhat) override;
    virtual double getOutput(int which) override;
    virtual void resetBrain() override;
    virtual std::string serializeToString() override;
    virtual void addBrainStatsToDataCollector(std::shared_ptr<DataCollector> dataCollector) override;
        
    std::vector<std::vector<std::pair<int,int>>> getReinforcementRecord();
    void setReinforcementRecord(std::vector<std::vector<std::pair<int,int>>> record);
    void resetReinforcementRecord();
    void reinforce(double weight,std::vector<double> modulator);
    virtual void show() override;
    virtual std::string renderToDot() override;
    
    virtual std::string getComponentList() override;

};

/*
class DeepMarkovBitGeneBrain: public DeepMarkovBitBrain{
public:
    static std::shared_ptr<Brain> getTemplate();
    virtual std::shared_ptr<Brain> makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts) override;
    
};
 */
 

#endif /* DeepMarkovBitBrain_hpp */
