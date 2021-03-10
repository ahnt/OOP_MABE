//
//  HideAndSeekWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/28/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef HideAndSeekWorld_hpp
#define HideAndSeekWorld_hpp

#include "../World.hpp"
#include "../../Core/Utilities.hpp"
#include "../../Core/Area.hpp"

class HideAndSeekWorld:public World{
public:
    virtual void evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) override;
    virtual void evaluate(std::shared_ptr<Population> hiders,std::shared_ptr<Population> seekers,bool analyze,bool visualize);
    void evaluateGroup(std::vector<std::shared_ptr<Organism>> group,bool analyze,bool visualize);

    virtual int nrOfSensors(void) override {return 11;} //force this -> JS?
    virtual int nrOfMotors(void) override {return 3;}
    
};

#endif /* HideAndSeekWorld_hpp */
