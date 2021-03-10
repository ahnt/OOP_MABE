//
//  CarWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 10/16/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef CarWorld_hpp
#define CarWorld_hpp

#include <memory>
#include <set>
#include "../World.hpp"
#include "../../Core/Population.hpp"
#include "../../Core/Area.hpp"
#include "AutoCarBrain.hpp"

class CarWorld:public World{
    int xm[4]={0,1,0,-1};
    int ym[4]={-1,0,1,0};
    std::vector<std::vector<int>> moves,stands;
public:
    int mode;
    std::vector<std::shared_ptr<Area>> areas;
    std::vector<std::vector<int>> whoMap;
    CarWorld();
    
    virtual void evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) override;
    virtual void evaluatePop(std::shared_ptr<Population> population,bool analyze,bool visualize,std::shared_ptr<Area> area,int replicate);
    
    virtual int nrOfSensors(void) override {return 29;} //force this -> JS?
    virtual int nrOfMotors(void) override {return 2;}
    virtual std::shared_ptr<Area> makeCarWorld(int size);
};

#endif /* CarWorld_hpp */
