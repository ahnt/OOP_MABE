//
//  BladerunnerWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef NBackWorld_hpp
#define NBackWorld_hpp

#include "../World.hpp"
#include "../../Gadgets/NeuroCorrelates/Representations.hpp"

class NBackWorld:public World{
    std::ofstream extraRFile;
    int nBack;
public:
    
    NBackWorld(); //the constructor
    
    virtual void evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) override;
    virtual void evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize,std::vector<int> nodeNoise);
    
    virtual int nrOfSensors() override;
    virtual int nrOfMotors() override;
};


#endif /* TestWorld_hpp */
