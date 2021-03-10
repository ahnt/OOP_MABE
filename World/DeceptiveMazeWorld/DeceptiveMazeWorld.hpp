//
//  DeceptiveMazeWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef DeceptiveMazeWorld_hpp
#define DeceptiveMazeWorld_hpp

#include <algorithm>
#include "../World.hpp"
#include "../../Core/Utilities.hpp"
#include "../../Core/Geometry.hpp"
#include "../../Gadgets/Maze/Maze.hpp"

class DeceptiveMazeWorld:public World{
public:
    
    std::shared_ptr<Maze> maze;
    std::ofstream trackFileHandle;
    double targetX,targetY,startX,startY;
    DeceptiveMazeWorld();
    
    virtual void evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize) override;
    virtual void evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) override;

    virtual int nrOfSensors(void) override {return 6;} //force this -> JS?
    virtual int nrOfMotors(void) override {return 2;}
};

#endif /* DeceptiveMazeWorld_hpp */

