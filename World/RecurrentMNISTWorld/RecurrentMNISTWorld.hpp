//
//  RecurrentMNISTWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef RecurrentMNISTWorld_hpp
#define RecurrentMNISTWorld_hpp

#include <algorithm>
#include "../World.hpp"
#include "../../Core/Utilities.hpp"
#include "../../Core/Geometry.hpp"
#include <map>
#include <vector>

class RecurrentMNISTWorld:public World{
    std::map<int,std::vector<Image>> data;
    std::vector<std::vector<int>> testSubSet;
    int evaluationMode;
public:
    RecurrentMNISTWorld();
    
    virtual void evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize) override;
    virtual void evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) override;

    virtual int nrOfSensors(void) override {return 28;} //force this -> JS?
    virtual int nrOfMotors(void) override {return 10;}
};

#endif /* RovingEyeMINISTWorld_hpp */

