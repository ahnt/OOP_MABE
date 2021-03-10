//
//  PendulumWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 11/26/20.
//  Copyright © 2020 ArendHintze. All rights reserved.
//

#ifndef PendulumWorld_hpp
#define PendulumWorld_hpp

#include "../World.hpp"

class PendulumWorld:public World{
    //this is a private variable wrapping a vector<vector<int>>
    //insuch a way that we can log all changes to it
    //this simplifies visualization greatly
public:
    
    PendulumWorld(); //the constructor
    
    virtual void evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize) override;
    virtual int nrOfSensors() override;
    virtual int nrOfMotors() override;
};


#endif /* PendulumWorld_hpp */
