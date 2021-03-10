//
//  BlockyCatchWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef BlockyCatchWorld_hpp
#define BlockyCatchWorld_hpp

#include "../World.hpp"
#include "../../Core/Utilities.hpp"

class BlockyCatchWorld:public World{
public:
    virtual void evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize);
    int runSingleDrop(std::shared_ptr<Brain> brain,int startLocation, int worldWidth, int tileWidth, int tileDirection, int dropLength,bool analyze,bool visualize);

    virtual int nrOfSensors(void){return 4;} //force this -> JS?
    virtual int nrOfMotors(void){return 2;}
    std::vector<double> hitOrMiss;
};

#endif /* BlockyCatchWorld_hpp */
