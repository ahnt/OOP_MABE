//
//  RNNBrain.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/28/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "RNNBrain.hpp"

std::shared_ptr<Brain> RNNBrain::getTemplate(){
    std::shared_ptr<RNNBrain> B=std::make_shared<RNNBrain>();
    return std::dynamic_pointer_cast<Brain>(B);
}

std::shared_ptr<Brain> RNNBrain::makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts){
    std::shared_ptr<RNNBrain> newBrain=std::make_shared<RNNBrain>();
    newBrain->recurrentSize=Parameters::getInt("RNNBrain-recurrentSize",12);
    newBrain->nrOfSensors=nrOfIns;
    newBrain->nrOfMotors=nrOfOuts;
    newBrain->inputLayer.assign(newBrain->nrOfSensors+newBrain->recurrentSize,0.0);
    newBrain->outputLayer.assign(newBrain->nrOfMotors+newBrain->recurrentSize,0.0);
    
    auto readHead=genome->getReadhead();
    
    switch(Parameters::getInt("RNNBrain-geneticEncodingMode",0)){
        case 0:// direct map encoded
            newBrain->weights.resize(newBrain->nrOfSensors+newBrain->recurrentSize);
            for(int i=0;i<newBrain->nrOfSensors+newBrain->recurrentSize;i++){
                newBrain->weights[i].resize(newBrain->nrOfMotors+newBrain->recurrentSize);
                for(int j=0;j<newBrain->nrOfMotors+newBrain->recurrentSize;j++){
                    newBrain->weights[i][j]=readHead->getRangedFloat(-4.0, 4.0);
                }
            }
            break;
        case 1: //"genetic" encoding
            newBrain->weights.resize(newBrain->nrOfSensors+newBrain->recurrentSize);
            for(int i=0;i<newBrain->nrOfSensors+newBrain->recurrentSize;i++){
                newBrain->weights[i].resize(newBrain->nrOfMotors+newBrain->recurrentSize);
                for(int j=0;j<newBrain->nrOfMotors+newBrain->recurrentSize;j++){
                    newBrain->weights[i][j]=0.0;
                }
            }
            int currentCodon=0;
            int lastCodon=0;
            int xDim=newBrain->nrOfSensors+newBrain->recurrentSize;
            int yDim=newBrain->nrOfMotors+newBrain->recurrentSize;
            
            while(!readHead->eog()){
                currentCodon=readHead->getByte();
                //add gates here depending on codons
                if((lastCodon<42)&&(currentCodon>213)){
                    int nrOfSites=1+(readHead->getUInt32()&15);
                    double value=(double)readHead->getRangedFloat(-1.0, 1.0);
                    for(int n=0;n<nrOfSites;n++){
                        int x=readHead->getUInt32()%xDim;
                        int y=readHead->getUInt32()%yDim;
                        newBrain->weights[x][y]=value;
                    }
                }
                lastCodon=currentCodon;
            }
            break;
    }
    return std::dynamic_pointer_cast<Brain>(newBrain);
}


void RNNBrain::update(){
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

std::vector<double> RNNBrain::getHidden(){
    return std::vector<double>(outputLayer.begin()+nrOfMotors,outputLayer.end());
}

void RNNBrain::setInput(int which, double toWhat){
    inputLayer[which]=toWhat;
}

double RNNBrain::getOutput(int which){
    return outputLayer[which];
}

void RNNBrain::resetBrain(){
    inputLayer.assign(inputLayer.size(),0.0);
}

std::string RNNBrain::serializeToString(){
    std::string S="RNN serialize to String has not been implemented yet";
    return S;
}

void RNNBrain::addBrainStatsToDataCollector(std::shared_ptr<DataCollector> dataCollector){
    std::string S="\"[RNN has no brain stats to report]\"";
    dataCollector->setValue("BrainStats", S);
}
