//
//  Organism.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "Organism.hpp"

Organism::Organism(){
    dataCollector=std::make_shared<DataCollector>();
    parent=nullptr;
    ID=-1;
    brain=nullptr;
    genome=nullptr;
    bornInGeneration=Utilities::currentGeneration;
}

Organism::Organism(std::shared_ptr<Organism> from){
    dataCollector=std::make_shared<DataCollector>();
    parent=from;
    ID=Utilities::getID();
    bornInGeneration=Utilities::currentGeneration;
    genome=from->genome->makeMutatedCopy();
    brain=from->brain->makeBrainFromGenome(genome, from->brain->getNrOfInputs(),from->brain->getNrOfOutputs());
}

Organism::Organism(std::shared_ptr<Organism> from,std::shared_ptr<Organism> other,double foreignMaterialRatio){
    dataCollector=std::make_shared<DataCollector>();
    parent=from;
    ID=Utilities::getID();
    bornInGeneration=Utilities::currentGeneration;
    genome=from->genome->makeMutatedAndRecombinedCopy(other->genome,foreignMaterialRatio);
    brain=from->brain->makeBrainFromGenome(genome, from->brain->getNrOfInputs(),from->brain->getNrOfOutputs());
}


Organism::Organism(std::shared_ptr<Genome> templateGenome,std::shared_ptr<Brain> templateBrain,int nrOfIns,int nrOfOuts){
    dataCollector=std::make_shared<DataCollector>();
    parent=nullptr;
    ID=Utilities::getID();
    bornInGeneration=Utilities::currentGeneration;
    genome=templateGenome->makeRandomGenome();
    brain=templateBrain->makeBrainFromGenome(genome,nrOfIns,nrOfOuts);
}

Organism::Organism(std::string genomeString,std::shared_ptr<Genome> templateGenome,std::shared_ptr<Brain> templateBrain,int nrOfIns,int nrOfOuts){
    dataCollector=std::make_shared<DataCollector>();
    parent=nullptr;
    ID=Utilities::getID();
    bornInGeneration=Utilities::currentGeneration;
    genome=templateGenome->deserializeFromString(genomeString);
    brain=templateBrain->makeBrainFromGenome(genome,nrOfIns,nrOfOuts);
}

Organism::Organism(std::shared_ptr<Brain> templateBrain,int nrOfIns,int nrOfOuts){
    dataCollector=std::make_shared<DataCollector>();
    parent=nullptr;
    genome=nullptr;
    brain=templateBrain->makeBrainFromGenome(genome,nrOfIns,nrOfOuts);
}

std::string Organism::serializeToString(){
    std::string S;
    //S+="Genome:\n"+genome->serializeToString()+"\n";
    S+="Brain:\n"+brain->serializeToString();
    return S;
}

std::shared_ptr<Organism> Organism::clone(){
    std::shared_ptr<Organism> theClone=std::make_shared<Organism>();
    theClone->parent=nullptr;
    theClone->ID=-1;
    theClone->bornInGeneration=-1;
    theClone->genome=genome->clone();
    theClone->brain=brain->makeBrainFromGenome(theClone->genome, brain->getNrOfInputs(),brain->getNrOfOutputs());
    return theClone;
}

std::shared_ptr<Organism> Organism::brainOnlyClone(){
    std::shared_ptr<Organism> theClone=std::make_shared<Organism>();
    theClone->parent=nullptr;
    theClone->ID=-1;
    theClone->bornInGeneration=-1;
    theClone->genome=nullptr;
    theClone->brain=brain->clone();
    return theClone;
}
