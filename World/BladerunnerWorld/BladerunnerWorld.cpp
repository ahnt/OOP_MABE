//
//  ExampleWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "BladerunnerWorld.hpp"


BladerunnerWorld::BladerunnerWorld(){
    /*
    std::vector<std::vector<double>> miniPatterns=std::vector<std::vector<double>>({
        std::vector<double>({1.0, 1.0, 0.0, 0.0}),
        std::vector<double>({0.0, 0.0, 1.0, 1.0}),
        std::vector<double>({1.0, 1.0, 1.0, 0.0}),
        std::vector<double>({0.0, 1.0, 1.0, 1.0}),
        std::vector<double>({-1.0,-1.0, 0.0, 0.0}),
        std::vector<double>({0.0, 0.0,-1.0,-1.0}),
        std::vector<double>({-1.0,-1.0,-1.0, 0.0}),
        std::vector<double>({0.0,-1.0,-1.0,-1.0})
    });
     */
    std::vector<std::vector<double>> miniPatterns=std::vector<std::vector<double>>({
        std::vector<double>({ 1.0, 0.0, 0.0, 0.0}),
        std::vector<double>({ 0.0, 0.0, 0.0, 1.0}),
        std::vector<double>({ 1.0, 1.0, 0.0, 0.0}),
        std::vector<double>({ 0.0, 0.0, 1.0, 1.0}),
        //std::vector<double>({ 1.0, 1.0, 1.0, 0.0, 0.0}),
        //std::vector<double>({ 0.0, 0.0, 1.0, 1.0, 1.0}),
        //std::vector<double>({ 0.0, 1.0, 1.0, 1.0, 1.0}),
        //std::vector<double>({ 1.0, 1.0, 1.0, 1.0, 0.0})
    });
    std::vector<double> emptyRow=std::vector<double>({0.0,0.0,0.0,0.0});
    for(int first=0;first<(int)miniPatterns.size();first++)
        for(int second=0;second<(int)miniPatterns.size();second++)
            //for(int tFirst=0;tFirst<3;tFirst++)
            //for(int tSecond=4;tSecond<7;tSecond++){
            for(int tFirst=0;tFirst<3;tFirst+=2)
                for(int tSecond=4;tSecond<7;tSecond+=2){
                    std::vector<std::vector<double>> localPattern;
                    std::vector<std::vector<int>> localWorldState;
                    for(int i=0;i<8;i++){
                        if((i==tFirst)||(i==tFirst+1))
                            localPattern.push_back(miniPatterns[first]);
                        else{
                            if((i==tSecond)||(i==tSecond+1))
                                localPattern.push_back(miniPatterns[second]);
                            else
                                localPattern.push_back(std::vector<double> (emptyRow));
                        }
                        localWorldState.push_back(std::vector<int>({first&1,(first>>1)&1,tFirst==2,second&1,(second>>1)&1,tSecond==6}));
                    }
                    inputPatterns.push_back(localPattern);
                    worldState.push_back(localWorldState);
                    std::vector<double> localOutput;
                    for(int i=0;i<2;i++){
                        if( ((first>>i)&1)==((second>>i)&1))
                            localOutput.push_back(1.0);
                        else
                            localOutput.push_back(0.0);
                    }
                    if(((tFirst==0) && (tSecond==4)) || ((tFirst==2) && (tSecond==6)))
                        localOutput.push_back(1.0);
                    else
                        localOutput.push_back(0.0);
                    targetPatterns.push_back(localOutput);
        }
    /*
    for(auto a:inputPatterns)
        for(auto b:a){
            for(auto c:b)
                std::cout<<c<<" ";
            std::cout<<std::endl;
        }
     */
    Parameters::setParameter("LODManager-parametersToSave",std::string("score,percentage,T1,T2,T3"));
    Parameters::setParameter("GeneticAlgorithm-extraMeans",std::string("percentage,T1,T2,T3"));
    Parameters::setParameter("MarkovBrain-nrOfHidden",12);//9);
    
}

void BladerunnerWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize){
    
    if(analyze){
        extraRFile.open(Parameters::getString("BladerunnerWorld-AnalyzeRFile", std::string("Analyze_R.csv")));
        extraRFile<<"ID,R,Rfirst,Rsecond"<<std::endl;
        evaluateSolo(population->population[population->population.size()-1],analyze,visualize);
        extraRFile.flush();
        extraRFile.close();
    } else {
    
        // simple loop iterating over all indididuals of a population
        for(auto organism:population->population){
            evaluateSolo(organism,analyze,visualize);
            
        }
    }
    
     //*/
    //*
    for(auto organism:population->population){
        evaluateSolo(organism,false,visualize);
        std::cout<<organism->ID<<" "<<organism->dataCollector->getDouble("percentage")<<" "
        <<organism->dataCollector->getDouble("T1")<<" "
        <<organism->dataCollector->getDouble("T2")<<" "
        <<organism->dataCollector->getDouble("T3")<<" "<<std::endl;
    }
     //*/
    
    if(analyze){
        analyzeTransplants(population->population[population->population.size()-1]);
    }
}


void BladerunnerWorld::evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize){
    double score=0.0;
    auto brain=organism->brain;     //retrieve the brain from the organism
    double total=0.0;
    double scoreBit=0.0;
    int typeCorrect[3]={0,0,0};
    int typeTotal[3]={0,0,0};
    std::vector<std::vector<double>> stateCollector;
    std::vector<double> stateRow;
    for(int p=0;p<(int)inputPatterns.size();p++){
        brain->resetBrain();
        for(int t=0;t<8;t++){
            for(int i=0;i<(int)inputPatterns[p].size();i++)
                brain->setInput(i, inputPatterns[p][t][i]);
            brain->clearOutput();
            brain->update();
            if(analyze){
                stateRow.clear();
                for(int i=0;i<6;i++){
                    stateRow.push_back((double)worldState[p][t][i]);
                }
                auto h=brain->getHidden();
                stateRow.insert(stateRow.end(),h.begin(),h.end());
                for(int o=0;o<3;o++){
                    stateRow.push_back(brain->getOutput(o));
                }
                stateRow.insert(stateRow.end(),inputPatterns[p][t].begin(),inputPatterns[p][t].end());
                stateCollector.push_back(stateRow);
                //std::cout<<serializeVectorToString(stateRow, ",")<<std::endl;
            }
        }
        for(int o=0;o<3;o++){
            total+=1.0;
            double delta=targetPatterns[p][o]-brain->getOutput(o);
            score+=delta*delta;
            delta=targetPatterns[p][o]-bit(brain->getOutput(o));
            scoreBit+=delta*delta;
            //brain->setOutput(0,0.0);
            if(bit(targetPatterns[p][o])==bit(brain->getOutput(o))){
                typeCorrect[o]++;
            }
            typeTotal[o]++;
        }
    }
    //std::cout<<score<<" "<<total<<std::endl;
    double percentage=1.0-(double)(score/total);
    score=1.0+percentage;
    //score=score*score;
    percentage=1.0-(double)(scoreBit/total);
    score=score*(1.0+percentage);
    auto t1=(double)typeCorrect[0]/(double)typeTotal[0];
    auto t2=(double)typeCorrect[1]/(double)typeTotal[1];
    auto t3=(double)typeCorrect[2]/(double)typeTotal[2];
    organism->dataCollector->setValue("score",pow(2.0,(1.0+t1)*(1.0+t2)*(1.0+t3)));//pow(2.0,score*10.0));
    organism->dataCollector->setValue("percentage",percentage);
    organism->dataCollector->setValue("T1",t1);
    organism->dataCollector->setValue("T2",t2);
    organism->dataCollector->setValue("T3",t3);
    if(analyze){
        auto hiddenStateSet=std::vector<int>({6,7,8,9, 10,11,12,13, 14,15,16,17});
        auto worldStateSet=std::vector<int>({0,1,2,3,4,5});
        auto data=Representations::medianBinarization(stateCollector,std::vector<int>({0,1,2,3,4,5,21,22,23,24}),std::vector<double>({0.5,0.5,0.5,0.5,0.5,0.5,0.0,0.0,0.0,0.0}));
        auto allW=Representations::extractBitPattern(data, worldStateSet);
        auto firstW=Representations::extractBitPattern(data,std::vector<int>({0,1,2}));
        auto secondW=Representations::extractBitPattern(data,std::vector<int>({3,4,5}));
        auto allH=Representations::extractBitPattern(data, hiddenStateSet);
        auto allS=Representations::extractBitPattern(data, std::vector<int>({21,22,23,24}));
        double R=Representations::computeR(allW, allH, allS);
        double Rfirst=Representations::computeR(firstW, allH, allS);
        double Rsecond=Representations::computeR(secondW,allH,allS);
        std::cout<<R<<" "<<Rfirst<<" "<<Rsecond<<std::endl;
        extraRFile<<organism->ID<<","<<R<<","<<Rfirst<<","<<Rsecond<<std::endl;
        std::vector<std::vector<int>> hiddenStatePowerSets=PowersetWithoutEmpty(hiddenStateSet);
        auto allws=std::vector<int>({0,1,2,3,4,5});
        std::vector<std::vector<int>> worldStatePowerSets=PowersetWithoutEmpty(allws);
        //exclusive content R
        // H(world) H(target) H(other) H(sensor)
        // H(world) -> for all three concepts 0,3 1,4 2,5
        //for all target subsets and their other counterparts
        auto worldSubStates={Representations::extractBitPattern(data, std::vector<int>({0,3})),
                                Representations::extractBitPattern(data, std::vector<int>({1,4})),
                                Representations::extractBitPattern(data, std::vector<int>({2,5}))};
        std::ofstream E;
        E.open("Exclusive_"+Parameters::getString("BladerunnerWorld-AnalyseFileLead", std::string("AnalyzeEnryptionFile_"))+std::to_string(organism->ID)+".csv");
        E<<"targetSet,otherSet,R_world0,R_world1,R_world2"<<std::endl;
        for(auto target:hiddenStatePowerSets){
            auto otherSet=hiddenStateSet;
            auto targetBP=Representations::extractBitPattern(data, target);
            for(auto node:target){
                otherSet.erase(std::find(otherSet.begin(),otherSet.end(),node));
            }
            auto otherBP=Representations::extractBitPattern(data, otherSet);
            std::cout<<"\"["<<serializeVectorToString(target, ",")<<"]\",\"["<<serializeVectorToString(otherSet, ",")<<"]\"";
            E<<"\"["<<serializeVectorToString(target, ",")<<"]\",\"["<<serializeVectorToString(otherSet, ",")<<"]\"";
            for(auto worldSubState:worldSubStates){
                auto exclusiveR=Representations::computeExclusiveR(worldSubState, otherBP, targetBP, allS);
                std::cout<<","<<exclusiveR;
                E<<","<<exclusiveR;
            }
            std::cout<<std::endl;
            E<<std::endl;
            E.flush();
        }
        E.close();
        //all to all partition R
        std::ofstream O;
        O.open(Parameters::getString("BladerunnerWorld-AnalyseFileLead", std::string("AnalyzeEnryptionFile_"))+std::to_string(organism->ID)+".csv");
        O<<"partition";
        for(auto n:worldStatePowerSets){
            O<<",\"R["<<serializeVectorToString(n, ",")<<"]\"";
        }
        O<<std::endl;
        O.flush();
        //O<<S;
        for(int b=0;b<(int)hiddenStatePowerSets.size();b++){
            auto HS=Representations::extractBitPattern(data, hiddenStatePowerSets[b]);
            O<<"\"["<<serializeVectorToString(hiddenStatePowerSets[b], ",")<<"]\"";
            for(auto w:worldStatePowerSets){
                auto WS=Representations::extractBitPattern(data, w);
                double localR=Representations::computeR(WS, HS, allS);
                O<<","<<localR;
                //std::cout<<w<<",\"["<<serializeVectorToString(hiddenStatePowerSets[b], ",")<<"]\","<<localR<<std::endl;
            }
            O<<std::endl;
        }
        O.close();
    }
     //*/
}

int BladerunnerWorld::nrOfSensors(){
    return 4;
}

int BladerunnerWorld::nrOfMotors(){
    return 3;
}

void BladerunnerWorld::analyzeTransplants(std::shared_ptr<Organism> organism){
    
    std::ofstream O;
    O.open(Parameters::getString("BladerunnerWorld-StateTransplantFile", std::string("StateTransplantFile.csv")));
    O<<"transplantNodeSet,scoreOriginal,scoreTarget,subScoreOriginal0,subScoreTarget0,subScoreOriginal1,subScoreTarget1,subScoreOriginal2,subScoreTarget2,C000,C001,C010,C011,C100,C101,C110,C111"<<std::endl;
    //collect all brainstates in the middle:
    auto brain=organism->brain;     //retrieve the brain from the organism
    std::vector<std::vector<double>> hiddenStateCollector;
    std::vector<std::vector<int>> nodePowerset;
    std::vector<int> nodeList;
    
    for(int p=0;p<(int)inputPatterns.size();p++){
        brain->resetBrain();
        for(int t=0;t<4;t++){
            for(int i=0;i<(int)inputPatterns[p].size();i++)
                brain->setInput(i, inputPatterns[p][t][i]);
            brain->clearOutput();
            brain->update();
            }
        auto HS=brain->getHidden();
        hiddenStateCollector.push_back(HS);
        //std::cout<<serializeVectorToString(HS, ",")<<std::endl;
    }
    for(int i=0;i<(int)hiddenStateCollector[0].size();i++){
        nodeList.push_back(i);
    }
    for(int i=1;i<(int)pow(2.0,nodeList.size());i++){
        std::vector<int> subSet;
        for(int j=0;j<(int)nodeList.size();j++){
            if(((i>>j)&1)==1)
                subSet.push_back(nodeList[j]);
        }
        nodePowerset.push_back(subSet);
    }
    
    for(auto transplantNodes:nodePowerset)
    //auto transplantNodes=nodePowerset[nodePowerset.size()-1];
    {
        int subscore[2][2][2]={{{0,0},{0,0}},{{0,0},{0,0}}};
        double total=0.0;
        double scoreOriginal=0.0;
        double scoreTarget=0.0;
        double subScoreOriginal[3]={0.0,0.0,0.0};
        double subScoreTarget[3]={0.0,0.0,0.0};
        for(int originalPattern=0;originalPattern<(int)inputPatterns.size();originalPattern++){
            for(int targetPattern=0;targetPattern<(int)inputPatterns.size();targetPattern++)
            {
                std::vector<int> complexTargetPattern=std::vector<int>({
                    ((originalPattern>>2)&1)==((targetPattern>>4)&1),
                    ((originalPattern>>3)&1)==((targetPattern>>5)&1),
                    ((originalPattern)&1)==((targetPattern>>1)&1)});
                int t=0;
                for(int i=0;i<3;i++)
                    if(complexTargetPattern[i]!=bit(targetPatterns[originalPattern][i]))
                        t++;
                if(t==3)
                {
                    brain->resetBrain();
                    //std::cout<<originalPattern<<" "<<targetPattern<<std::endl;
                    for(int t=0;t<8;t++){
                        for(int i=0;i<(int)inputPatterns[originalPattern].size();i++){
                            brain->setInput(i, inputPatterns[originalPattern][t][i]);
                        }
                        //if(t<4)
                        //std::cout<<serializeVectorToString(inputPatterns[originalPattern][t],",")<<" | "
                        //         <<serializeVectorToString(inputPatterns[targetPattern][t],",")<<std::endl;
                        //else
                        //    std::cout<<serializeVectorToString(inputPatterns[originalPattern][t],",")<<std::endl;
                        if(t==4){
                            auto HS=brain->getHidden();
                            //std::cout<<"before: "<<serializeVectorToString(HS,",")<<std::endl;
                            //std::cout<<"target: "<<serializeVectorToString(transplantNodes,",")<<std::endl;
                            for(auto node:transplantNodes)
                                brain->setHidden(node, hiddenStateCollector[targetPattern][node]);
                            HS=brain->getHidden();
                            //std::cout<<"after:  "<<serializeVectorToString(HS,",")<<std::endl;
                        }
                        brain->clearOutput();
                        brain->update();
                    }
                    for(int o=0;o<3;o++){
                        if(bit(targetPatterns[originalPattern][o])==bit(brain->getOutput(o))){
                            scoreOriginal++;
                            subScoreOriginal[o]+=1.0;
                        }
                        if(complexTargetPattern[o]==bit(brain->getOutput(o))){
                            scoreTarget++;
                            subScoreTarget[o]+=1.0;
                        }
                        total+=1.0;
                        subscore[bit(targetPatterns[originalPattern][o])][bit(complexTargetPattern[o])][bit(brain->getOutput(o))]++;
                    }
                    //std::cout<<"("<<serializeVectorToString(targetPatterns[originalPattern],",")<<") | ("<<serializeVectorToString(targetPatterns[targetPattern],",")<<")"<<std::endl;
                   // std::cout<<bit(brain->getOutput(0))<<","<<bit(brain->getOutput(1))<<","<<bit(brain->getOutput(2))<<","<<std::endl;
                    //std::cout<<"T:"<<serializeVectorToString(complexTargetPattern, ",")<<std::endl;
                }
            }
        }
        std::cout<<"\"["<<serializeVectorToString(transplantNodes, ",")<<"]\","<<scoreOriginal/total<<","<<scoreTarget/total;
        for(int o=0;o<3;o++){
            double t=subScoreOriginal[o]+subScoreTarget[o];
            std::cout<<","<<subScoreOriginal[o]/t<<","<<subScoreTarget[o]/t;
        }
        for(int a=0;a<2;a++){
            for(int b=0;b<2;b++){
                for(int c=0;c<2;c++){
                    std::cout<<",("<<a<<","<<b<<","<<c<<"):"<<subscore[a][b][c];
                }
            }
        }
        std::cout<<std::endl;
        O<<"\"["<<serializeVectorToString(transplantNodes, ",")<<"]\","<<scoreOriginal/total<<","<<scoreTarget/total;
        for(int o=0;o<3;o++){
            double t=subScoreOriginal[o]+subScoreTarget[o];
            O<<","<<subScoreOriginal[o]/t<<","<<subScoreTarget[o]/t;
        }
        for(int a=0;a<2;a++){
            for(int b=0;b<2;b++){
                for(int c=0;c<2;c++){
                    O<<","<<subscore[a][b][c];
                }
            }
        }
       O<<std::endl;

    }
    O.close();
}


/*
*/
//
