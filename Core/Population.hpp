//
//  Population.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef Population_hpp
#define Population_hpp

#include <stdio.h>
#include <memory>
#include <vector>
#include <string>

#include "Organism.hpp"
#include "../Brain/Brain.hpp"
#include "../Genome/Genome.hpp"
//#include "../World/World.hpp"
#include "Parameters.hpp"
#include <fstream>

class Population{
public:
    std::vector<std::shared_ptr<Organism>> population;
    
    static std::shared_ptr<Population> createStartPopulation(int N,std::shared_ptr<Genome> templateGenome,std::shared_ptr<Brain> templateBrain,int nrOfIns,int nrOfOuts);
    static std::shared_ptr<Population> loadPopulation(std::string filename,std::shared_ptr<Genome> templateGenome,std::shared_ptr<Brain> templateBrain,int nrOfIns,int nrOfOuts);
    std::string serializeToString();
    
    std::shared_ptr<Population> singleOrgTestPopulation(int who);
};

#endif /* Population_hpp */
