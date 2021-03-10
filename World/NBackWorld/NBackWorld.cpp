//
//  ExampleWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "NBackWorld.hpp"


NBackWorld::NBackWorld(){
    nBack=Parameters::getInt("NBackWorld-n", 5);
    Parameters::setParameter("LODManager-parametersToSave",std::string("score,percentage"));
    Parameters::setParameter("GeneticAlgorithm-extraMeans",std::string("percentage"));
    Parameters::setParameter("MarkovBrain-nrOfHidden",nBack);
}

void NBackWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize){
    if(analyze){
        int analyzeMode=1;
        switch(analyzeMode){
            case 0:
                for(auto organism:population->population){
                    evaluateSolo(organism,analyze,visualize,std::vector<int>({}));
                }
                break;
            case 1:{
                std::vector<int> hiddenStateSet;
                for(int i=0;i<nBack+1;i++){
                    hiddenStateSet.push_back(i);
                }
                
                std::vector<std::vector<int>> hiddenStatePowerSets;
                for(int i=0;i<(int)pow(2.0,hiddenStateSet.size());i++){
                    std::vector<int> subSet;
                    for(int j=0;j<(int)hiddenStateSet.size();j++){
                        if(((i>>j)&1)==1)
                            subSet.push_back(hiddenStateSet[j]);
                    }
                    hiddenStatePowerSets.push_back(subSet);
                }
                auto org=population->population[(int)population->population.size()-1];
                double baseW=1.0;
                std::ofstream wFile;
                wFile.open(Parameters::getString("nBackWorld-wNoiseFileName", "default"));
                wFile<<"W"<<std::endl;
                for(auto nodeNoise:hiddenStatePowerSets){
                    evaluateSolo(org,false,false,nodeNoise);
                    for(auto n:nodeNoise){
                        std::cout<<n<<" ";
                    }
                    if(nodeNoise.size()==0)
                        baseW=org->dataCollector->getDouble("percentage");
                    else{
                        std::cout<<baseW-org->dataCollector->getDouble("percentage")<<std::endl;
                        wFile<<baseW-org->dataCollector->getDouble("percentage")<<std::endl;
                    }
                }
                wFile.close();
            }
                break;
        }
    }
    else{
        for(auto organism:population->population){
            evaluateSolo(organism,analyze,visualize,std::vector<int>({}));
        }

    }
}


void NBackWorld::evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize,std::vector<int> nodeNoise){
    double score=0.0;
    double total=0.0;
    auto brain=organism->brain;     //retrieve the brain from the organism
    std::vector<double> worldStates;
    std::vector<std::vector<double>> stateCollector;
    std::vector<double> stateRow;

    for(int i=0;i<nBack;i++){
        worldStates.push_back(0.0);
    }
    for(int t=0;t<1000;t++){
        double currentWorldState=Random::getInt(0, 1);
        worldStates.push_back(currentWorldState);
        brain->setInput(0, (double) currentWorldState);
        for(int i=0;i<nBack;i++){
            brain->setOutput(i, 0.0);
        }
        for(auto node:nodeNoise)
            brain->setHidden(node, (double)Random::getFloat(-1.0, 1.0));
        brain->update();
        if(analyze){
            stateRow.clear();
            for(int i=0;i<nBack;i++){
                stateRow.push_back((double)worldStates[t+nBack-i]);
            }
            auto h=brain->getHidden();
            stateRow.insert(stateRow.end(),h.begin(),h.end());
            stateRow.push_back(bit(brain->getOutput(nBack-1)));
            stateRow.push_back((double)currentWorldState);
            stateCollector.push_back(stateRow);
        }
        for(int i=0;i<nBack;i++){
            if(bit(brain->getOutput(i))==bit(worldStates[t+nBack-i]))
                score+=1.0;
            total+=1.0;
        }
    }
    organism->dataCollector->setValue("score",score*score);
    organism->dataCollector->setValue("percentage",score/total);
    if(analyze){
        std::cout<<"ID: "<<organism->ID<<" score: "<<score/total<<std::endl;
        std::vector<int> hiddenStateSet;
        std::vector<int> worldStateSet;
        for(int i=0;i<nBack;i++){
            worldStateSet.push_back(i);
        }
        for(int i=0;i<nBack+1;i++){
            hiddenStateSet.push_back(nBack+i);
        }
        
        std::vector<std::vector<int>> hiddenStatePowerSets;
        for(int i=1;i<(int)pow(2.0,hiddenStateSet.size());i++){
            std::vector<int> subSet;
            for(int j=0;j<(int)hiddenStateSet.size();j++){
                if(((i>>j)&1)==1)
                    subSet.push_back(hiddenStateSet[j]);
            }
            hiddenStatePowerSets.push_back(subSet);
        }
        
        auto data=Representations::medianBinarization(stateCollector,std::vector<int>({0,1,2,3,4,5,6,7,8,9,10,11}),std::vector<double>({0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5}));
        auto allS=Representations::extractBitPattern(data, std::vector<int>({nBack+nBack+1}));
        //std::cout<<serializeVecVecIntoPythonString(data).c_str()<<std::endl;
        std::ofstream rFile;
        rFile.open(Parameters::getString("nBackWorld-rFileNameLead", "default")+"_"+std::to_string(organism->ID)+".csv");
        rFile<<"powerset";
        for(auto w:worldStateSet){
            rFile<<","<<w;
        }
        rFile<<std::endl;
        for(int b=0;b<(int)hiddenStatePowerSets.size();b++){
            auto HS=Representations::extractBitPattern(data, hiddenStatePowerSets[b]);
            for(auto w:worldStateSet){
                auto WS=Representations::extractBitPattern(data, std::vector<int>({w}));
                double localR=Representations::computeR(WS, HS, allS);
                if(localR<0.0)
                    localR=0.0;
                if(w==worldStateSet[0]){
                    rFile<<"\"["<<serializeVectorToString(hiddenStatePowerSets[b], ",")<<"]\"";
                }
                rFile<<","<<localR;
            }
            rFile<<std::endl;
        }
        rFile.close();
    }
}

int NBackWorld::nrOfSensors(){
    return 1;
}

int NBackWorld::nrOfMotors(){
    return nBack;
}
