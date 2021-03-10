//
//  World.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef World_hpp
#define World_hpp

#include <memory>
#include "../Core/Population.hpp"

class World{
public:
    virtual void evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize){
        for(auto organism:population->population){
            evaluateSolo(organism,analyze,visualize);
        }
    }
    virtual void evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize){
        organism->dataCollector->setValue("score",0.0);
    }
    virtual int nrOfSensors(void){return 0;} //force this -> JS?
    virtual int nrOfMotors(void){return 0;}
    
};
#endif /* World_hpp */

