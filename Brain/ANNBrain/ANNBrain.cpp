//
//  ANNBrain.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/28/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "ANNBrain.hpp"

std::shared_ptr<Brain> ANNBrain::getTemplate(){
    std::shared_ptr<ANNBrain> B=std::make_shared<ANNBrain>();
    return std::dynamic_pointer_cast<Brain>(B);
}

std::shared_ptr<Brain> ANNBrain::makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts){
    std::shared_ptr<ANNBrain> newBrain=std::make_shared<ANNBrain>();
    std::vector<std::string> vs=Utilities::splitStringIntoVectorOfString(Parameters::getString("ANNBrain-layerSizes",std::string("40")), ',');
    newBrain->nrOfSensors=nrOfIns;
    newBrain->nrOfMotors=nrOfOuts;
    newBrain->layerSize.push_back(nrOfIns);
    for(auto s:vs)
        newBrain->layerSize.push_back(atoi(s.c_str()));
    newBrain->layerSize.push_back(nrOfOuts);
    newBrain->weights.resize(newBrain->layerSize.size()-1);
    auto readHead=genome->getReadhead();
    for(int l=0;l<newBrain->weights.size();l++){
        newBrain->weights[l].resize(newBrain->layerSize[l]);
        for(int i=0;i<newBrain->weights[l].size();i++){
            newBrain->weights[l][i].resize(newBrain->layerSize[l+1]);
            for(int o=0;o<newBrain->weights[l][i].size();o++)
                newBrain->weights[l][i][o]=readHead->getRangedFloat(-4.0, 4.0);
        }
    }
    newBrain->values.resize(newBrain->layerSize.size());
    for(int l=0;l<newBrain->layerSize.size();l++)
        newBrain->values[l].assign(newBrain->layerSize[l],0.0);
        
    return std::dynamic_pointer_cast<Brain>(newBrain);
}


void ANNBrain::update(){
    for(int l=0;l<weights.size();l++){
        for(int o=0;o<layerSize[l+1];o++){
            double acc=0.0;
            for(int i=0;i<layerSize[l];i++)
                acc+=values[l][i]*weights[l][i][o];
            if(l!=weights.size()-1)
                values[l+1][o]=tanh(acc);
            else
                values[l+1][o]=1.0/(1.0*exp(-acc)); // Sigmoid
        }
    }
}

void ANNBrain::setInput(int which, double toWhat){
    values[0][which]=toWhat;
}

double ANNBrain::getOutput(int which){
    return values[values.size()-1][which];
}

void ANNBrain::resetBrain(){
    for(int l=0;l<values.size();l++)
        values[l].assign(values[l].size(), 0.0);
}

std::string ANNBrain::serializeToString(){
    std::string S="ANN serialize to String has not been implemented yet";
    return S;
}

void ANNBrain::addBrainStatsToDataCollector(std::shared_ptr<DataCollector> dataCollector){
    std::string S="\"[ANN has no brain stats to report]\"";
    dataCollector->setValue("BrainStats", S);
}
