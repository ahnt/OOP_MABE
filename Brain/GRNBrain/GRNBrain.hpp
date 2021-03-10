//
//  GRNBrain.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/28/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef GRNBrain_hpp
#define GRNBrain_hpp

#include "../Brain.hpp"

class GRNBrain:public Brain{
    std::vector<std::vector<double>> M;
    std::vector<double> V;
    int brainSize,ticksPerBrainUpdate;
    
public:
    static std::shared_ptr<Brain> getTemplate();
    virtual std::shared_ptr<Brain> makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts) override;
    virtual void update() override;

    virtual void setInput(int which, double toWhat) override;
    virtual double getOutput(int which) override;
    virtual void resetBrain() override;
    virtual std::string serializeToString() override;
    virtual void addBrainStatsToDataCollector(std::shared_ptr<DataCollector> dataCollector) override;

};

#endif /* GRNBrain_hpp */
