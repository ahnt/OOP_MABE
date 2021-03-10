//
//  AutoCarBrain.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 10/19/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "AutoCarBrain.hpp"
std::shared_ptr<Brain> AutoCarBrain::getTemplate(){
    std::shared_ptr<AutoCarBrain> B=std::make_shared<AutoCarBrain>();
    return std::dynamic_pointer_cast<Brain>(B);
}

std::shared_ptr<Brain> AutoCarBrain::makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts){
    std::shared_ptr<AutoCarBrain> B=std::make_shared<AutoCarBrain>();
    B->inputs.resize(nrOfIns);
    B->outputs.resize(nrOfOuts);
    return std::dynamic_pointer_cast<Brain>(B);
}

void AutoCarBrain::update(){
    //follow the dist to target
    /*
    if(inputs[27]>0.0){
        outputs[0]=1.0;
        outputs[1]=1.0;
    } else {
        outputs[0]=1.0;
        outputs[1]=0.0;
    }
    // */
    //*
    int I=bit(inputs[20])+bit(inputs[21])*2;
    switch(I){
        case 0:
            outputs[0]=Random::getInt(0,1);
            outputs[1]=Random::getInt(0,1);
            break;
        case 1:
        case 2:
        case 3:
            outputs[0]=1.0;
            outputs[1]=0.0;
            break;
    }
     //*/
}

void AutoCarBrain::setInput(int which, double toWhat){
    inputs[which]=toWhat;
}

double AutoCarBrain::getOutput(int which){
    return outputs[which];
}


