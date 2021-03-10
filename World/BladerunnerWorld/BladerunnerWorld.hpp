//
//  BladerunnerWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef BladerunnerWorld_hpp
#define BladerunnerWorld_hpp

#include "../World.hpp"
#include "../../Gadgets/NeuroCorrelates/Representations.hpp"

class BladerunnerWorld:public World{
    std::vector<std::vector<std::vector<double>>> inputPatterns;
    std::vector<std::vector<double>> targetPatterns;
    std::vector<std::vector<std::vector<int>>> worldState;
    std::ofstream extraRFile;
public:
    
    BladerunnerWorld(); //the constructor
    
    virtual void evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) override;
    virtual void evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize) override;
    
    void analyzeTransplants(std::shared_ptr<Organism> organism);
    virtual int nrOfSensors() override;
    virtual int nrOfMotors() override;
};


#endif /* TestWorld_hpp */
