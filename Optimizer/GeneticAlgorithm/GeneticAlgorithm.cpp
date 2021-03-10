//
//  GeneticAlgorithm.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "GeneticAlgorithm.hpp"

GeneticAlgorithm::GeneticAlgorithm(){
    objective=Parameters::getString(std::string("GeneticAlgorithm-objective"), std::string("score"));
    nrOfEliteOrganism=Parameters::getInt(std::string("GeneticAlgorithm-nrOfEliteOrganisms"),1);
    extraMeans=Utilities::splitStringIntoVectorOfString(Parameters::getString("GeneticAlgorithm-extraMeans", ""), ',');
}

std::shared_ptr<Population> GeneticAlgorithm::optimize(std::shared_ptr<Population> population){
    std::shared_ptr<Population> nextGen=std::make_shared<Population>();
    std::map<std::string,double> extraValues;
    std::map<std::string,double> extraMax;
    double maxFit=0.0;
    int maxFitWho=0;
    int popSize=(int)population->population.size();
    double meanFit=0.0;
    std::vector<double> W;
    maxFit=population->population[0]->dataCollector->getDouble(objective);
    for(auto S:extraMeans){
        extraValues[S]=0.0;
        extraMax[S]=0.0;
    }
    for(int n=0;n<(int)population->population.size();n++){
        double w=population->population[n]->dataCollector->getDouble(objective);
        W.push_back(w);
        meanFit+=w;
        if(w>maxFit){
            maxFit=w;
            maxFitWho=n;
        }
        for(auto S:extraMeans){
            double value=population->population[n]->dataCollector->getDouble(S);
            extraValues[S]+=value;
            if(value>extraMax[S])
                extraMax[S]=value;
        }
    }
    for(int elite=0;elite<nrOfEliteOrganism;elite++){
        nextGen->population.push_back(std::make_shared<Organism>(population->population[maxFitWho]));
    }
    int targetPopSize=Parameters::getInt(std::string("Core-targetPopSize"),100);
    while(nextGen->population.size()<targetPopSize){
        int who;
        do{
            who=Random::getInt32(0, popSize-1);
        }while(Random::getFloat(0, (float)maxFit)>W[who]);
        nextGen->population.push_back(std::make_shared<Organism>(population->population[who]));
    }
    
    std::cout
            <<std::to_string(Utilities::currentGeneration)<<" "
            <<std::to_string(maxFit)<<" "
            <<std::to_string(meanFit/(double)popSize);
    for(auto S:extraMeans)
        std::cout<<"\t"<<extraValues[S]/(double)popSize<<"/"<<extraMax[S];
    std::cout<<std::endl;
    for(auto who:population->population){
        who->brain->retire();
    }
    return nextGen;
}
