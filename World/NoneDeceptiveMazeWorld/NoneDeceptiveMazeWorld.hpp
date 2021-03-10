//
//  NoneDeceptiveMazeWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef NoneDeceptiveMazeWorld_hpp
#define NoneDeceptiveMazeWorld_hpp

#include <algorithm>
#include "../World.hpp"
#include "../../Core/Utilities.hpp"
#include "../../Core/Geometry.hpp"
#include "../../Core/Area.hpp"
#include "../../Gadgets/Maze/Maze.hpp"

class NoneDeceptiveMazeWorld:public World{
public:
    std::vector<std::shared_ptr<Maze>> maze;
    double temperatures[2];
    int currentMaze,currentTemp;
    int mazeType[2];
    double targetX,targetY,startX,startY;
    NoneDeceptiveMazeWorld();
    
    virtual void evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize) override;
    virtual void evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) override;

    virtual int nrOfSensors(void) override {return 6;} //force this -> JS?
    virtual int nrOfMotors(void) override {return 2;}
};

#endif /* NoneDeceptiveMazeWorld_hpp */

