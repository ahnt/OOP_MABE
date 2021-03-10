//
//  Brain.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "Brain.hpp"

std::shared_ptr<Brain> Brain::getTemplate(){
    std::cout << "tried to instantiate Brain" <<std::endl;
    exit(0);
    return std::make_shared<Brain>();
}

std::shared_ptr<Brain> Brain::makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts){
    std::cout << "tried to make Brain from Genome" <<std::endl;
    exit(0);
    return std::make_shared<Brain>();
}

std::string Brain::serializeToString(){
    return std::string("you called the baseclass serializer from Brain, you shouldn't");
}

void Brain::update(){
    std::cout<<"brain update not implemented in base class"<<std::endl;
    exit(0);
}
