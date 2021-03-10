//
//  Population.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "Population.hpp"

std::shared_ptr<Population> Population::createStartPopulation(int N,std::shared_ptr<Genome> templateGenome,std::shared_ptr<Brain> templateBrain,int nrOfIns,int nrOfOuts){
    std::shared_ptr<Population> P=std::make_shared<Population>();
    P->population.clear();
    for(int n=0;n<N;n++)
        P->population.push_back(std::make_shared<Organism>(templateGenome,templateBrain,nrOfIns,nrOfOuts));
    return P;
}

std::shared_ptr<Population> Population::loadPopulation(std::string filename,std::shared_ptr<Genome> templateGenome,std::shared_ptr<Brain> templateBrain,int nrOfIns,int nrOfOuts){
    std::shared_ptr<Population> P=std::make_shared<Population>();
    P->population.clear();
    std::ifstream G;
    G.open(filename);
    if (G.is_open()){
        std::string line;
        bool firstLine=true;
        while ( std::getline (G,line) ){
            if(firstLine)
                firstLine=false;
            else{
                //std::cout << line << std::endl;
                std::string genomeString;
                std::vector<std::string> V=Utilities::splitStringIntoVectorOfString(line, '"');
                //std::cout<<V.size()<<std::endl;
                genomeString=V[1];
                genomeString.pop_back();
                std::shared_ptr<Organism> O=std::make_shared<Organism>(genomeString,templateGenome,templateBrain,nrOfIns,nrOfOuts);
                P->population.push_back(O);
                std::cout<<"loaded org: "<<(int) P->population.size()<<std::endl;
            }
        }
       G.close();
    }
    else {
        std::cout << "Unable to open file "<<filename<<std::endl;
        exit(0);
    }
    
    return P;

}


std::string Population::serializeToString(){
    std::string S;
    for(int o=0;o<(int)population.size();o++){
        S+="Organism: "+std::to_string(o)+"\n"+population[o]->serializeToString()+"\n";
    }
    return S;
}

std::shared_ptr<Population> Population::singleOrgTestPopulation(int who){
    std::shared_ptr<Population> P=std::make_shared<Population>();
    P->population.resize(1);
    P->population[0]=population[who];
    return P;
}
