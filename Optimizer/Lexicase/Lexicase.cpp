//
//  Lexicase.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 11/13/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "Lexicase.hpp"

Lexicase::Lexicase(){
    //theKeys=Utilities::splitStringIntoVectorOfString(Parameters::getString("Lexicase-keys", "lexi_0_forwards,lexi_0_longestWait,lexi_0_score,lexi_0_stops,lexi_0_targetsReached,lexi_0_ticksAlive,lexi_1_forwards,lexi_1_longestWait,lexi_1_score,lexi_1_stops,lexi_1_targetsReached,lexi_1_ticksAlive,lexi_2_forwards,lexi_2_longestWait,lexi_2_score,lexi_2_stops,lexi_2_targetsReached,lexi_2_ticksAlive,lexi_3_forwards,lexi_3_longestWait,lexi_3_score,lexi_3_stops,lexi_3_targetsReached,lexi_3_ticksAlive,lexi_4_forwards,lexi_4_longestWait,lexi_4_score,lexi_4_stops,lexi_4_targetsReached,lexi_4_ticksAlive"),',');
    //auto maximizeStrVec=Utilities::splitStringIntoVectorOfString(Parameters::getString("Lexicase-maxOrMin", "+,-,+,-,+,+,+,-,+,-,+,+,+,-,+,-,+,+,+,-,+,-,+,+,+,-,+,-,+,+"),',');
    theKeys=Utilities::splitStringIntoVectorOfString(Parameters::getString("Lexicase-keys", "lexi_0_targetsReached,lexi_0_longestWait,lexi_0_crashes,lexi_4_targetsReached,lexi_4_longestWait,lexi_4_crashes"),',');
    auto maximizeStrVec=Utilities::splitStringIntoVectorOfString(Parameters::getString("Lexicase-maxOrMin", "+,-,-,+,-,-"),',');
    for(auto S:maximizeStrVec){
        if(S=="+")
            maximizeKey.push_back(true);
        else
            maximizeKey.push_back(false);
    }
}

std::shared_ptr<Population> Lexicase::optimize(std::shared_ptr<Population> population){
    int targetPopSize=Parameters::getInt(std::string("Core-targetPopSize"),100);
    std::shared_ptr<Population> nextGen=std::make_shared<Population>();
    std::map<std::string,std::vector<double>> allData;
    double maxFit=0.0;
    double meanFit=0.0;
    for(auto key:theKeys){
        std::vector<double> values;
        for(auto org:population->population)
            values.push_back(org->dataCollector->getDouble(key));
        allData[key]=values;
    }
    
    std::vector<int> order;
    for(int o=0;o<(int)theKeys.size();o++)
        order.push_back(o);
    
    for(int n=0;n<targetPopSize;n++){
        //create new order
        for(int i=0;i<(int)order.size();i++){
            int t=Random::getInt(0, (int)order.size()-1);
            int dummy=order[i];
            order[i]=order[t];
            order[t]=dummy;
        }
        //create current pop set
        std::vector<int> pool,newPool;
        for(int i=0;i<population->population.size();i++){
            float f=Random::getFloat();
            if(f>0.5)
                pool.push_back(i);
            //std::cout<<i<<" "<<f<<std::endl;
        }
        int currentKey=0;
        do{
            std::vector<double> V;
            for(auto p:pool){
                V.push_back(allData[theKeys[order[currentKey]]][p]);
            }
            double theValue;
            if(maximizeKey[order[currentKey]])
                theValue=*std::max_element(V.begin(), V.end());
            else
                theValue=*std::min_element(V.begin(), V.end());
            //std::cout<<"current key "<<theKeys[order[currentKey]]<<" value "<<theValue<< std::endl;
            newPool.clear();
            for(int i=0;i<(int)V.size();i++){
                if(V[i]==theValue)
                    newPool.push_back(pool[i]);
            }
            pool=newPool;
            currentKey++;
        }while((pool.size()>1)&&(currentKey<(int)order.size()));
        if(pool.size()==1)
            nextGen->population.push_back(std::make_shared<Organism>(population->population[pool[0]]));
        else
            nextGen->population.push_back(std::make_shared<Organism>(population->population[pool[Random::getInt(0,(int)pool.size()-1)]]));
    }
    for(auto org:population->population){
        double score=org->dataCollector->getDouble("score");
        meanFit+=score;
        if(score>maxFit)
            maxFit=score;
    }
    std::cout
            <<std::to_string(Utilities::currentGeneration)<<" "
            <<std::to_string(maxFit)<<" "
            <<std::to_string(meanFit/(double)population->population.size())<<std::endl;

    return nextGen;
}
