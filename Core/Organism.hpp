//
//  Organism.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef Organism_hpp
#define Organism_hpp

#include <stdio.h>
#include <memory>
#include "../Brain/Brain.hpp"
#include "../Genome/Genome.hpp"
#include "DataCollector.hpp"

class Organism{
public:
    std::shared_ptr<Organism> parent;
    int ID;
    int bornInGeneration;
    
    std::shared_ptr<DataCollector> dataCollector;
    std::shared_ptr<Brain> brain;
    std::shared_ptr<Genome> genome;
    Organism();
    
    Organism(std::shared_ptr<Organism> from);
    Organism(std::shared_ptr<Organism> from,std::shared_ptr<Organism> other,double foreignMaterialRatio=0.5);
    Organism(std::shared_ptr<Genome> templateGenome,std::shared_ptr<Brain> templateBrain,int nrOfIns,int nrOfOuts);
    Organism(std::string genomeString,std::shared_ptr<Genome> templateGenome,std::shared_ptr<Brain> templateBrain,int nrOfIns,int nrOfOuts);
    Organism(std::shared_ptr<Brain> templateBrain,int nrOfIns,int nrOfOuts);
    std::string serializeToString();
    std::shared_ptr<Organism> clone();
    std::shared_ptr<Organism> brainOnlyClone();
};

#endif /* Organism_hpp */
