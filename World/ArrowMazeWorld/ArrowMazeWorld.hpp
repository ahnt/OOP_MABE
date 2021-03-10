//
//  ArrowMazeWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/1/20.
//  Copyright © 2020 ArendHintze. All rights reserved.
//

#ifndef ArrowMazeWorld_hpp
#define ArrowMazeWorld_hpp

#include "../World.hpp"
#include "../../Core/Area.hpp"


class ArrowMazeWorld:public World{
    std::shared_ptr<Area> area;
    std::vector<std::pair<int,int>> startLocations;
    int mappings[24][4]={{0,1,2,3},{0,1,3,2},{0,2,1,3},{0,2,3,1},{0,3,1,2},{0,3,2,1},
                         {1,0,2,3},{1,0,3,2},{1,2,0,3},{1,2,3,0},{1,3,0,2},{1,3,2,0},
                         {2,0,1,3},{2,0,3,1},{2,1,0,3},{2,1,3,0},{2,3,0,1},{2,3,1,0},
                         {3,0,1,2},{3,0,2,1},{3,1,0,2},{3,1,2,0},{3,2,0,1},{3,2,1,0}};
    int targetX,targetY;
    std::vector<std::vector<int>> dist;
public:
    ArrowMazeWorld();
    virtual void evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) override;
    void evaluateSolo(std::shared_ptr<Organism> organism,int mappingID,bool analyze,bool visualize);
    virtual int nrOfSensors() override;
    virtual int nrOfMotors() override;
};


#endif /* ArrowMazeWorld_hpp */
