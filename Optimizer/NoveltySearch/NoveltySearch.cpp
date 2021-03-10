//
//  Lexicase.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 11/13/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "NoveltySearch.hpp"

#define pureNovelty 0
#define archiveNovelty 1
#define sexualSelectionWithRecombination 2
#define sexualSelectionWithoutRecombination 3
#define randomSelectionAsexual 4
#define randomSelectionSexual 5
#define sexualSelectionDifferentTarget 6

NoveltySearch::NoveltySearch(){
    archiveSize=Parameters::getInt(std::string("NoveltySearch-archiveSize"), 100);
    archiveFileHandle.open(Parameters::getString(std::string("NoveltySearch-archiveFileName"),std::string("archiveFile.csv")));
    solutionsFileHandle.open(Parameters::getString(std::string("NoveltySearch-solutionsFileName"),std::string("solutionsFile.csv")));
    mode=pureNovelty;
    //mode=Parameters::getInt("NoveltySearch-mode",pureNovelty);
    //mode=Parameters::getInt("NoveltySearch-mode",sexualSelectionDifferentTarget);
    //sexualSelectionWithRecombination;
    //mode=randomSelectionAsexual;
    evaluators=nullptr;
}

double NoveltySearch::dist(std::vector<double> A,std::vector<double> B){
    double a=0.0;
    for(int i=0;i<A.size();i++){
        a+=(A[i]-B[i])*(A[i]-B[i]);
    }
    return sqrt(a);
}

std::vector<double> NoveltySearch::noveltyList(std::vector<std::vector<double>> &group,std::vector<double> &individual){
    std::vector<double> list;
    for(auto g:group)
        list.push_back(dist(g,individual));
    return list;
}

double NoveltySearch::noveltyValue(std::vector<double> &list,int k, int first){
    std::sort(list.begin(),list.end());
    return std::accumulate(list.begin()+first,list.begin()+k+first,0.0)/(double)k;
}


std::shared_ptr<Population> NoveltySearch::optimize(std::shared_ptr<Population> population){
    if(evaluators==nullptr)
        if((mode==sexualSelectionWithRecombination)||(mode==sexualSelectionWithoutRecombination)||(mode==randomSelectionSexual)||(mode==sexualSelectionDifferentTarget)){
            evaluators=Population::createStartPopulation(Parameters::getInt(std::string("Core-numberOfStartOrganisms"), 100), BioGenome::getTemplate(), ANNBrain::getTemplate(),2,1);
        }

    
    
    int targetPopSize=Parameters::getInt(std::string("Core-targetPopSize"),100);
    std::shared_ptr<Population> nextGenGenerators=std::make_shared<Population>();
    std::shared_ptr<Population> nextGenEvaluators=std::make_shared<Population>();
    std::vector<double> W;
    std::vector<std::vector<double>> pool;
    for(auto who:population->population){
        pool.push_back(who->dataCollector->getDoubleVector("behavior"));
    }
    //make sure to have an archive
    if(archive.size()<archiveSize){
        int i=0;
        for(int a=0;a<archiveSize;a++){
            archive.push_back(pool[i]);
            i=(i+1)%archiveSize;
        }
        updateaW();
    }
    
    //fitness evaluation
    std::vector<std::vector<double>> affinityMatrix;
    double aMax=0.0;
    switch(mode){
        case pureNovelty:
        case randomSelectionAsexual:
            for(auto who:pool){
                auto list=noveltyList(pool,who);
                double w=noveltyValue(list);
                W.push_back(w);
            }
            break;
        case archiveNovelty:
            for(auto who:pool){
                auto list=noveltyList(archive,who);
                double w=noveltyValue(list);
                W.push_back(w);
            }
            break;
        case sexualSelectionWithRecombination:
        case sexualSelectionWithoutRecombination:
        case randomSelectionSexual:
        case sexualSelectionDifferentTarget:
            // we need to update the archive later so we use this prelimiarily to compute stuff
            for(auto who:pool){
                auto list=noveltyList(archive,who);
                double w=noveltyValue(list);
                W.push_back(w);
            }
            affinityMatrix=std::vector<std::vector<double>>(evaluators->population.size(),std::vector<double>(population->population.size(),0.0));
            for(int e=0;e<evaluators->population.size();e++){
                for(int g=0;g<population->population.size();g++){
                    for(int i=0;i<pool[g].size();i++){
                        evaluators->population[e]->brain->setInput(i, pool[g][i]);
                    }
                    evaluators->population[e]->brain->update();
                    double res=evaluators->population[e]->brain->getOutput(0);
                    affinityMatrix[e][g]=res*res; //
                }
                double sum=std::accumulate(affinityMatrix[e].begin(), affinityMatrix[e].end(), 0.0);
                for(int g=0;g<population->population.size();g++){
                    affinityMatrix[e][g]/=sum;
                    if(affinityMatrix[e][g]>aMax)
                        aMax=affinityMatrix[e][g];
                }
            }
            break;
    }
    std::vector<double> separateFittnesSelectors;
    if(mode==sexualSelectionDifferentTarget){
        std::vector<std::pair<double,double>> points=std::vector<std::pair<double,double>>(
                            {std::pair<double,double>(0.1,0.6),
                            std::pair<double,double>(0.9,0.1),
                            std::pair<double,double>(0.4,0.6),
                            std::pair<double,double>(0.8,0.8),
                            std::pair<double,double>(0.1,0.9)});
        std::vector<double> pointScore=std::vector<double>({-1.0,0.2,0.4,0.8,1.0});
        for(auto who:evaluators->population){
            double score=0.0;
            auto brain=who->brain;
            for(int i=0;i<(int)points.size();i++){
                brain->setInput(0,points[i].first);
                brain->setInput(1,points[i].second);
                brain->update();
                double output=brain->getOutput(0);
                double delta=fabs(output-pointScore[i]);
                if(delta<0.000001)
                    delta=0.000001;
                score+=1.0/delta;
            }
            separateFittnesSelectors.push_back(score);
        }
    }
    
    //selection
    double Wmax=*std::max_element(W.begin(), W.end());
    switch(mode){
        case pureNovelty:
        case archiveNovelty:
        case randomSelectionAsexual:
            for(int n=0;n<targetPopSize;n++){
                int who;
                if(mode==randomSelectionAsexual)
                    who=Random::getInt(0, (int)population->population.size()-1);
                else
                    do{
                        who=Random::getInt(0, (int)population->population.size()-1);
                    }while(Random::getFloat(0, Wmax)>W[who]);
                nextGenGenerators->population.push_back(std::make_shared<Organism>(population->population[who]));
            }
            break;
        case sexualSelectionWithRecombination:
        case sexualSelectionWithoutRecombination:
        case randomSelectionSexual:
            for(int n=0;n<targetPopSize;n++){
                int whoG,whoE;
                if(mode==randomSelectionSexual){
                    whoG=Random::getInt(0, (int)population->population.size()-1);
                    whoE=Random::getInt(0, (int)evaluators->population.size()-1);
                } else
                    do{
                        whoG=Random::getInt(0, (int)population->population.size()-1);
                        whoE=Random::getInt(0, (int)evaluators->population.size()-1);
                    }while(Random::getFloat(0, aMax)>affinityMatrix[whoE][whoG]);
                if(mode==sexualSelectionWithRecombination){
                    nextGenGenerators->population.push_back(std::make_shared<Organism>(population->population[whoG],evaluators->population[whoE],0.5));
                    nextGenEvaluators->population.push_back(std::make_shared<Organism>(evaluators->population[whoE],population->population[whoG],0.5));
                }
                if((mode==sexualSelectionWithoutRecombination)||(mode==randomSelectionSexual)){
                    nextGenGenerators->population.push_back(std::make_shared<Organism>(population->population[whoG]));
                    nextGenEvaluators->population.push_back(std::make_shared<Organism>(evaluators->population[whoE]));
                }
            }
            evaluators=nextGenEvaluators;
            break;
        case sexualSelectionDifferentTarget:
            for(int n=0;n<targetPopSize;n++){
                int whoG,whoE;
                double sMax=*std::max_element(separateFittnesSelectors.begin(), separateFittnesSelectors.end());
                do{
                    whoG=Random::getInt(0, (int)population->population.size()-1);
                    whoE=Random::getInt(0, (int)evaluators->population.size()-1);
                }while(Random::getFloat(0, aMax)>affinityMatrix[whoE][whoG]);
                nextGenGenerators->population.push_back(std::make_shared<Organism>(population->population[whoG]));
                do{
                    whoE=Random::getInt(0, (int)evaluators->population.size()-1);
                }while(Random::getFloat(0, sMax)>separateFittnesSelectors[whoE]);
                nextGenEvaluators->population.push_back(std::make_shared<Organism>(evaluators->population[whoE]));
            }
            evaluators=nextGenEvaluators;
            break;
    }
    
    //update archive
    auto archiveMin=std::min_element(aW.begin(), aW.end());
    if(Wmax>*archiveMin){
        archive.erase(archive.begin()+(int)(archiveMin-aW.begin()));
        int maxElementIndex=(int)(std::max_element(W.begin(), W.end())-W.begin());
        archive.push_back(pool[maxElementIndex]);
        updateaW();
    }
    archiveFileHandle<<serializeVecVecIntoPythonString(archive)<<std::endl;
    //add marker sizes for fitness
    for(int p=0;p<W.size();p++)
        pool[p].push_back(W[p]);
    solutionsFileHandle<<serializeVecVecIntoPythonString(pool)<<std::endl;
    std::cout
            <<std::to_string(Utilities::currentGeneration)<<" "
            <<std::to_string(Wmax)<<" "
            <<std::to_string(std::accumulate(W.begin(), W.end(),0.0)/population->population.size())<<std::endl;

    return nextGenGenerators;
}

void NoveltySearch::updateaW(){
    aW.clear();
    for(auto individual:archive){
        std::vector<double> L=noveltyList(archive, individual);
        aW.push_back(noveltyValue(L));
    }
}
