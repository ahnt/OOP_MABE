//
//  RovingEyeMINISTWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "RovingEyeMINISTWorld.hpp"
#include <cmath>



RovingEyeMINISTWorld::RovingEyeMINISTWorld(){
    std::ifstream infile;
    infile.open("numbers.csv");
    std::string line;
    while (std::getline(infile, line)){
        auto elements=Utilities::splitStringIntoVectorOfString(line,',');
        int label=atoi(elements[0].c_str());
        Image I=Image(elements[1]);
        data[label].push_back(I);
        //I.show();
    }
    std::cout<<"numbers.csv file loaded"<<std::endl;
    Parameters::setParameter("LODManager-parametersToSave",std::string("score,scoreV"));
    Parameters::setParameter("GeneticAlgorithm-extraMeans",std::string(""));
}

void RovingEyeMINISTWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) {
    testSubSet.clear();
    testSubSet.resize(10);
    for(int n=0;n<10;n++)
        for(int m=0;m<10;m++)
            testSubSet[n].push_back(Random::getInt(0,(int)data[n].size()-1));
            
    for(auto organism:population->population){
        evaluateSolo(organism,analyze,visualize);
    }
}

void RovingEyeMINISTWorld::evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize){
    auto brain = organism->brain;
    int xmd5[4][5]={{-2,-1,0,1,2},{1,1,1,1,1},{2,1,0,1,2},{-1,-1,-1,-1,-1}};
    int ymd5[4][5]={{-1,-1,-1,-1,-1},{-2,-1,0,1,2},{1,1,1,1,1},{2,1,0,-1,-2}};
    std::vector<std::vector<int>> allAnswerRecord;
    std::vector<std::vector<int>> highestValueCount;
    highestValueCount.resize(10);
    for(int a=0;a<10;a++){
        highestValueCount[a].resize(1024);
        for(int b=0;b<1024;b++)
            highestValueCount[a][b]=0;
    }
    for(int n=0;n<10;n++){
        organism->dataCollector->setValue("c"+std::to_string(n),0);
        for(int m=0;m<testSubSet[n].size();m++){
            int x=2;
            int y=2;
            int d=1;
            auto I=data[n][testSubSet[n][m]].image;
            brain->resetBrain();
            std::vector<int> localAnswerRecord;
            for(int t=0;t<250;t++){
                for(int dm=0;dm<5;dm++)
                    brain->setInput(dm,I[x+xmd5[d][dm]][y+ymd5[d][dm]]);
                brain->setInput(5,d&1);
                brain->setInput(6,(d>>1)&1);
                brain->update();
                int action=bit(brain->getOutput(0))+(bit(brain->getOutput(1))<<1);
                switch(action){
                    case 0: break;
                    case 1: d=(d+1)&3; break;
                    case 2: d=(d-1)&3; break;
                    case 3:{
                        int tx=x+xmd5[d][2];
                        int ty=y+ymd5[d][2];
                        if((tx>1)&&(ty>1)&&(tx<26)&&(ty<26)){
                            x=tx;
                            y=ty;
                        }
                    } break;
                }
                if(t>=150){
                    int output=0;
                    for(int o=0;o<10;o++){
                        output=(output<<1)+bit(brain->getOutput(2+o));
                    }
                    //output=n; 230 is max
                    localAnswerRecord.push_back(output);
                    highestValueCount[n][output]++;
                }
            }
            allAnswerRecord.push_back(localAnswerRecord);
        }
    }
    double score=0.0;
    std::vector<double> scoreV;
    for(int t=0;t<100;t++){
        double pMatrix[10][1024];
        double marginalPa[10],marginalPb[1024];
        for(int a=0;a<10;a++)
            for(int b=0;b<1024;b++)
                pMatrix[a][b]=0.0;
        for(int a=0;a<10;a++)
            marginalPa[a]=0.0;
        for(int b=0;b<1024;b++)
            marginalPb[b]=0.0;
        
        int z=0;
        for(int n=0;n<10;n++)
            for(int m=0;m<10;m++){
                int answer=allAnswerRecord[z][t];
                z++;
                marginalPa[n]+=(double)0.01;
                marginalPb[answer]+=(double)0.01;
                pMatrix[n][answer]+=(double)0.01;
            }
        double H=0.0;
        for(int a=0;a<10;a++)
            for(int b=0;b<1024;b++)
                if ( (pMatrix[a][b]>0.0) && ((marginalPa[a]*marginalPb[b])!=0.0))
                    H+=pMatrix[a][b]*log(pMatrix[a][b]/(marginalPa[a]*marginalPb[b]));
        if(H<0.0){
            H=0.0;
        }
        /*
        if(std::isnan(H)){
            H=0.0;
            for(int a=0;a<10;a++)
                for(int b=0;b<1024;b++)
                    if((marginalPa[a]*marginalPb[b])!=0.0){
                        std::cout<<a<<" "<<b<<" "<<pMatrix[a][b]<<" "<<marginalPa[a]<<" "<<marginalPb[b]<<" ";
                        H+=pMatrix[a][b]*log(pMatrix[a][b]/(marginalPa[a]*marginalPb[b]));
                        std::cout<<H<<std::endl;
                    }
            std::cout<<H<<std::endl;
        }*/
        score+=H;
        scoreV.push_back(H);
    }
    organism->dataCollector->setValue("score",score);
    organism->dataCollector->setValue("scoreV",scoreV);
    /*
    for(int a=0;a<10;a++){
        int maxValue=*std::max_element(highestValueCount[a].begin(), highestValueCount[a].end());
        int total=std::accumulate(highestValueCount[a].begin(), highestValueCount[a].end(),0);
        organism->dataCollector->setValue("hvc"+std::to_string(a),(double) maxValue/(double)total);
    }
     */
}
