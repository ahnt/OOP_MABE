//
//  LODmanager.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "LODmanager.hpp"

LODManager::LODManager(std::string individualFilePreface){
    preface=individualFilePreface;
    std::string S=Parameters::getString("LODManager-parametersToSave","score");
    itemsKeysToSave=Utilities::splitStringIntoVectorOfString(S, ',');
    //for(auto i:itemsKeysToSave)
    //    std::cout<<i<<std::endl;
    //exit(0);
    LODFilename=preface+Parameters::getString("LODManager-LODFilename","LOD.csv");
    GENOMEFilename=preface+Parameters::getString("LODManager-GENOMEFilename","GEN.csv");
    LODFileHandle.open(LODFilename);
    LODFileHandle<<"generation,ID";
    for(auto S:itemsKeysToSave)
        LODFileHandle<<","<<S;
    LODFileHandle<<std::endl;
    
    GENOMEFileHandle.open(GENOMEFilename);
    GENOMEFileHandle<<"generation,ID,genome"<<std::endl;
    
    saveIntervallGENOME=Parameters::getInt("LODManager-GENOMEsaveIntervall",1000);
    saveIntervallLOD=Parameters::getInt("LODManager-LODsaveIntervall", 100);
    intervalToCheck=Parameters::getInt("LODManager-intervalToCheck",100);
}

void LODManager::performIntermediaryCheck(std::shared_ptr<Population> population){
    if((Utilities::currentGeneration%intervalToCheck)==0){
        std::set<std::shared_ptr<Organism>> nextSet,currentSet;
        currentSet=std::set<std::shared_ptr<Organism>>(population->population.begin(),population->population.end());
        bool noConvergence=false;
        do{
            noConvergence=false;
            nextSet.clear();
            for(auto O:currentSet)
                if(O->parent!=nullptr){
                    nextSet.insert(O->parent);
                    noConvergence=true;
                }
            currentSet=nextSet;
        }while((noConvergence)&&((int)currentSet.size()!=1));
        if(currentSet.size()==1){
            //std::cout<<(*currentSet.begin())->bornInGeneration<<std::endl;
            std::shared_ptr<Organism> O=(*currentSet.begin())->parent;
            std::vector<std::shared_ptr<Organism>> listOfAncestors;
            while(O!=nullptr){
                listOfAncestors.push_back(O);
                O=O->parent;
            }
            //save stuff to LOD and GENOME Files
            for (auto itO = listOfAncestors.rbegin(); itO != listOfAncestors.rend(); ++itO){
                if(((*itO)->bornInGeneration%saveIntervallGENOME)==0){
                    GENOMEFileHandle<<(*itO)->bornInGeneration<<","<<(*itO)->ID<<",\""<<(*itO)->genome->serializeToString()<< "\""<<std::endl;
                }
                if(((*itO)->bornInGeneration%saveIntervallLOD)==0){
                    LODFileHandle<<(*itO)->bornInGeneration<<","<<(*itO)->ID;
                    for(auto K:itemsKeysToSave)
                        LODFileHandle<<","<<(*itO)->dataCollector->getAsStringFromUnknownType(K);
                    LODFileHandle<<std::endl;
                }
            }
            std::cout<<"dumped "<<listOfAncestors.size()<<" organisms to file"<<std::endl;
            (*currentSet.begin())->parent=nullptr;
            for(auto DP:listOfAncestors)
                DP->parent=nullptr;
        }
    }
}

void LODManager::wrapUp(std::shared_ptr<Population> population){
    LODFileHandle.close();
    GENOMEFileHandle.close();
}


void LODManager::savePop(std::shared_ptr<Population> population,std::string filename){
    std::ofstream O;
    O.open(filename);
    O<<"generation,ID,genome"<<std::endl;
    for(auto org:population->population){
        O<<org->bornInGeneration<<",\""<<org->genome->serializeToString()<< "\""<<std::endl;
    }
    O.close();
}
