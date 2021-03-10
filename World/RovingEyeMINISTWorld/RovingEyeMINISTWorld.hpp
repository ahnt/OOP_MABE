//
//  RovingEyeMINISTWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef RovingEyeMINISTWorld_hpp
#define RovingEyeMINISTWorld_hpp

#include <algorithm>
#include "../World.hpp"
#include "../../Core/Utilities.hpp"
#include "../../Core/Geometry.hpp"
#include <map>
#include <vector>

class RovingEyeMINISTWorld:public World{
    std::map<int,std::vector<Image>> data;
    std::vector<std::vector<int>> testSubSet;
public:
    RovingEyeMINISTWorld();
    
    virtual void evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize) override;
    virtual void evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) override;

    virtual int nrOfSensors(void) override {return 7;} //force this -> JS?
    virtual int nrOfMotors(void) override {return 12;}
};

#endif /* RovingEyeMINISTWorld_hpp */

