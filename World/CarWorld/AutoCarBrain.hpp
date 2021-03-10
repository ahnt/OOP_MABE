//
//  AutoCarBrain.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 10/19/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef AutoCarBrain_hpp
#define AutoCarBrain_hpp

#include "../../Brain/Brain.hpp"

class AutoCarBrain:public Brain{
    std::vector<double> inputs,outputs;
public:
    static std::shared_ptr<Brain> getTemplate();
    virtual std::shared_ptr<Brain> makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts) override;

    virtual void update() override;

    virtual void setInput(int which, double toWhat) override;
    virtual double getOutput(int which) override;

};
#endif /* AutoCarBrain_hpp */
