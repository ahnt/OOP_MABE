//
//  RNNBrain.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/28/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "NeuroEvoDevoBrain.hpp"

std::shared_ptr<Brain> NeuroEvoDevoBrain::getTemplate(){
    std::shared_ptr<NeuroEvoDevoBrain> B=std::make_shared<NeuroEvoDevoBrain>();
    return std::dynamic_pointer_cast<Brain>(B);
}

std::shared_ptr<Brain> NeuroEvoDevoBrain::makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts){
    std::shared_ptr<NeuroEvoDevoBrain> newBrain=std::make_shared<NeuroEvoDevoBrain>();
    newBrain->recurrentSize=Parameters::getInt("NeuroEvoDevoBrain-recurrentSize",6);
    newBrain->nrOfSensors=nrOfIns;
    newBrain->nrOfMotors=nrOfOuts;
    int iDim=newBrain->nrOfSensors+newBrain->recurrentSize;
    int jDim=newBrain->nrOfMotors+newBrain->recurrentSize;
    newBrain->inputLayer.assign(iDim,0.0);
    newBrain->outputLayer.assign(jDim,0.0);
    newBrain->weights=std::vector<std::vector<double>>(iDim,std::vector<double>(jDim,0.0));
    newBrain->GRNweights.clear();
    newBrain->GRNtimeSeries.clear();
    auto readHead=genome->getReadhead();
    switch(Parameters::getInt("NeuroEvoDevoBrain-geneticEncodingMode",3)){
        case 0:{// direct weight encoded
            for(int i=0;i<iDim;i++)
                for(int j=0;j<jDim;j++)
                    newBrain->weights[i][j]=readHead->getRangedFloat(-4.0, 4.0);
        }
            break;
        case 1: //developmental GRN encoding
        {
            int sizeOfGRN=Parameters::getInt("NeuroEvoDevoBrain-sizeOfGRN",(iDim*jDim)+20);
            std::shared_ptr<GRN> localGRN=std::make_shared<GRN>(sizeOfGRN);
            for(int i=0;i<sizeOfGRN;i++)
                for(int j=0;j<sizeOfGRN;j++)
                    localGRN->setWeight(i, j, readHead->getRangedFloat(-1.0, 1.0));
            double temp=Parameters::getDouble("NeuroEvoDevoBrain-temperature", 0.5);
            newBrain->GRNtimeSeries.push_back(localGRN->V);
            newBrain->GRNweights=localGRN->M;
            for(int i=0;i<200;i++){
                localGRN->update(temp);
                GRNtimeSeries.push_back(localGRN->V);
            }
            int z=0;
            for(int i=0;i<iDim;i++)
                for(int j=0;j<jDim;j++){
//                    newBrain->weights[i][j]=(localGRN->V[z]*8.0)-4.0;
                    newBrain->weights[i][j]=localGRN->V[z]*4.0;
                    z++;
                }
        }
            break;
        case 2: //indirect Hyper Encoding
        {
            int depthOfCPPN=Parameters::getInt("NeuroEvoDevoBrain-sizeOfCPPN",16);
            std::shared_ptr<CPPN> localCPPN=std::make_shared<CPPN>(readHead,depthOfCPPN);
            auto Vi=Utilities::linspace(-PI, PI, iDim);
            auto Vj=Utilities::linspace(-PI, PI, jDim);
            for(int i=0;i<iDim;i++)
                for(int j=0;j<jDim;j++){
                    newBrain->weights[i][j]=(localCPPN->update(Vi[i], Vj[j])*8.0)-4.0;
                    //newBrain->weights[i][j]=localCPPN->update(Vi[i], Vj[j])*4.0;
                }
        }
            break;
        case 3: //indirect hyper developmental Encoding
        {
            int depthOfCPPN=Parameters::getInt("NeuroEvoDevoBrain-sizeOfCPPN",16);
            std::shared_ptr<CPPN> localCPPN=std::make_shared<CPPN>(readHead,depthOfCPPN);
            int sizeOfGRN=Parameters::getInt("NeuroEvoDevoBrain-sizeOfGRN",(iDim*jDim)+20);
            std::shared_ptr<GRN> localGRN=std::make_shared<GRN>(sizeOfGRN);
            auto Gi=Utilities::linspace(-PI, PI, sizeOfGRN);
            auto Gj=Utilities::linspace(-PI, PI, sizeOfGRN);

            for(int i=0;i<sizeOfGRN;i++)
                for(int j=0;j<sizeOfGRN;j++)
                    //localGRN->setWeight(i, j, localCPPN->update(Gi[i], Gj[j]));
                    localGRN->setWeight(i, j, (localCPPN->update(Gi[i], Gj[j])*2.0)-1.0);
            double temp=Parameters::getDouble("NeuroEvoDevoBrain-temperature", 0.5);
            newBrain->GRNtimeSeries.push_back(localGRN->V);
            newBrain->GRNweights=localGRN->M;
            for(int i=0;i<200;i++){
                localGRN->update(temp);
                newBrain->GRNtimeSeries.push_back(localGRN->V);
            }
            int z=0;
            for(int i=0;i<iDim;i++)
                for(int j=0;j<jDim;j++){
                    newBrain->weights[i][j]=localGRN->V[z]*4.0;
                    //newBrain->weights[i][j]=(localGRN->V[z]*8.0)-4.0;
                    z++;
                }
        }
            break;
        case 4: //GRN->CPPN->RNN
        {
            int sizeOfGRN=Parameters::getInt("NeuroEvoDevoBrain-sizeOfGRN",(Parameters::getInt("NeuroEvoDevoBrain-sizeOfCPPN",16)*5)+20);
            std::shared_ptr<GRN> localGRN=std::make_shared<GRN>(sizeOfGRN);
            for(int i=0;i<sizeOfGRN;i++)
                for(int j=0;j<sizeOfGRN;j++)
                    localGRN->setWeight(i, j, readHead->getRangedFloat(-1.0, 1.0));
            double temp=Parameters::getDouble("NeuroEvoDevoBrain-temperature", 0.5);
            newBrain->GRNtimeSeries.push_back(localGRN->V);
            newBrain->GRNweights=localGRN->M;
            for(int i=0;i<200;i++){
                localGRN->update(temp);
                newBrain->GRNtimeSeries.push_back(localGRN->V);
            }

            int depthOfCPPN=Parameters::getInt("NeuroEvoDevoBrain-sizeOfCPPN",16);
            std::shared_ptr<CPPN> localCPPN=std::make_shared<CPPN>(localGRN->V,depthOfCPPN);

            auto Vi=Utilities::linspace(-PI, PI, iDim);
            auto Vj=Utilities::linspace(-PI, PI, jDim);
            for(int i=0;i<iDim;i++)
                for(int j=0;j<jDim;j++){
                    newBrain->weights[i][j]=(localCPPN->update(Vi[i], Vj[j])*8.0)-4.0;
                    //newBrain->weights[i][j]=localCPPN->update(Vi[i], Vj[j])*4.0;
                }

        }
        break;
    }
    return std::dynamic_pointer_cast<Brain>(newBrain);
}

void NeuroEvoDevoBrain::retire(){
    weights.clear();
    GRNweights.clear();
    GRNtimeSeries.clear();
}

void NeuroEvoDevoBrain::update(){
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

void NeuroEvoDevoBrain::setInput(int which, double toWhat){
    inputLayer[which]=toWhat;
}

double NeuroEvoDevoBrain::getOutput(int which){
    return outputLayer[which];
}

void NeuroEvoDevoBrain::resetBrain(){
    inputLayer.assign(inputLayer.size(),0.0);
}

std::string NeuroEvoDevoBrain::serializeToString(){
    std::string S="NeuroEvoDevoBrain serialize to String has not been implemented yet";
    return S;
}

void NeuroEvoDevoBrain::addBrainStatsToDataCollector(std::shared_ptr<DataCollector> dataCollector){
    std::string S=serializeVecVecIntoPythonString(weights);
    //dataCollector->setValue("BrainStats", S);
    //dataCollector->setValue("BrainStats_RNNweights", serializeVecVecIntoPythonString(weights));
    //dataCollector->setValue("BrainStats_GRNtimeSeries", serializeVecVecIntoPythonString(GRNtimeSeries));
    //dataCollector->setValue("BrainStats_GRNweights", serializeVecVecIntoPythonString(GRNweights));
}



