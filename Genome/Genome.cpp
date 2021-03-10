//
//  Genome.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "Genome.hpp"

std::shared_ptr<Genome> Genome::getTemplate(){
    return std::make_shared<Genome>();
}

std::shared_ptr<Genome> Genome::deserializeFromString(std::string S){
    return std::make_shared<Genome>();
}

std::shared_ptr<Genome> Genome::makeRandomGenome(){
    return std::make_shared<Genome>();
}

std::shared_ptr<Genome> Genome::makeMutatedCopy(){
    return std::make_shared<Genome>();
}

std::shared_ptr<Genome> Genome::makeMutatedAndRecombinedCopy(std::shared_ptr<Genome> donor,double foreignMaterialRatio){
    return makeMutatedCopy();
}

std::shared_ptr<Genome::ReadHead> Genome::getReadhead(){
    return std::make_shared<Genome::ReadHead>();
}

