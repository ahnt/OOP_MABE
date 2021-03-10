//
//  RNNBrain.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/28/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef RNNBrain_hpp
#define RNNBrain_hpp

#include "../Brain.hpp"

class RNNBrain:public Brain{
    std::vector<double> inputLayer,outputLayer;
    std::vector<std::vector<double>> weights;
    int recurrentSize;
public:
    static std::shared_ptr<Brain> getTemplate();
    virtual std::shared_ptr<Brain> makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts) override;
    virtual void update() override;

    virtual void setInput(int which, double toWhat) override;
    virtual double getOutput(int which) override;
    virtual void resetBrain() override;
    virtual std::string serializeToString() override;
    virtual void addBrainStatsToDataCollector(std::shared_ptr<DataCollector> dataCollector) override;
    virtual std::vector<double> getHidden() override;
};

#endif /* GRNBrain_hpp */
