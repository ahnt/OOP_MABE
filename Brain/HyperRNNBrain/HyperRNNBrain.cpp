//
//  HyperRNNBrain.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/28/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "HyperRNNBrain.hpp"

std::shared_ptr<Brain> HyperRNNBrain::getTemplate(){
    std::shared_ptr<HyperRNNBrain> B=std::make_shared<HyperRNNBrain>();
    return std::dynamic_pointer_cast<Brain>(B);
}

std::shared_ptr<Brain> HyperRNNBrain::makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts){
    std::shared_ptr<HyperRNNBrain> newBrain=std::make_shared<HyperRNNBrain>();
    newBrain->recurrentSize=Parameters::getInt("RNNBrain-recurrentSize",12);
    newBrain->nrOfSensors=nrOfIns;
    newBrain->nrOfMotors=nrOfOuts;
    newBrain->inputLayer.assign(newBrain->nrOfSensors+newBrain->recurrentSize,0.0);
    newBrain->outputLayer.assign(newBrain->nrOfMotors+newBrain->recurrentSize,0.0);
    newBrain->weights.resize(newBrain->nrOfSensors+newBrain->recurrentSize);

    auto readHead=genome->getReadhead();
    
    std::shared_ptr<CPPN> cppn=std::make_shared<CPPN>(readHead,32);
    auto xDim=Utilities::linspace(-PI, PI, newBrain->nrOfSensors+newBrain->recurrentSize);
    auto yDim=Utilities::linspace(-PI, PI, newBrain->nrOfMotors+newBrain->recurrentSize);
    for(int i=0;i<xDim.size();i++){
        newBrain->weights[i].resize(newBrain->nrOfMotors+newBrain->recurrentSize);
        for(int j=0;j<yDim.size();j++)
            newBrain->weights[i][j]=cppn->update(xDim[i],yDim[j]);
    }
    
    //for(int i=0;i<10;i++)
    //    std::cout<<newBrain->weights[0][i]<<" ";
    //std::cout<<std::endl;
    return std::dynamic_pointer_cast<Brain>(newBrain);
}


void HyperRNNBrain::update(){
    for(int o=0;o<outputLayer.size();o++){
        double acc=0.0;
        for(int i=0;i<inputLayer.size();i++){
            acc+=weights[i][o]*inputLayer[i];
        }
        outputLayer[o]=tanh(acc);
    }
    for(int h=0;h<recurrentSize;h++)
        inputLayer[nrOfSensors+h]=outputLayer[nrOfMotors+h];
}

std::vector<double> HyperRNNBrain::getHidden(){
    return std::vector<double>(outputLayer.begin()+nrOfMotors,outputLayer.end());
}

void HyperRNNBrain::setInput(int which, double toWhat){
    inputLayer[which]=toWhat;
}

double HyperRNNBrain::getOutput(int which){
    return outputLayer[which];
}

void HyperRNNBrain::resetBrain(){
    inputLayer.assign(inputLayer.size(),0.0);
}

std::string HyperRNNBrain::serializeToString(){
    std::string S="HyperRNN serialize to String has not been implemented yet";
    return S;
}

void HyperRNNBrain::addBrainStatsToDataCollector(std::shared_ptr<DataCollector> dataCollector){
    std::string S="\"[HyperRNN has no brain stats to report]\"";
    dataCollector->setValue("BrainStats", S);
}
