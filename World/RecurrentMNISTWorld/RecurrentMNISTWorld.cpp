//
//  RecurrentMNISTWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "RecurrentMNISTWorld.hpp"
#include "../../Core/Geometry.hpp"
#include <cmath>


RecurrentMNISTWorld::RecurrentMNISTWorld(){
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
    Parameters::setParameter("LODManager-parametersToSave",std::string("score,tp,fp,tn,fn,crossEntropy"));
    Parameters::setParameter("GeneticAlgorithm-extraMeans",std::string("crossEntropy,tp,fp,tn,fn"));
    evaluationMode=Parameters::getInt("RecurrentMNIST-evaluationMode",0);
}

void RecurrentMNISTWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) {
    testSubSet.clear();
    testSubSet.resize(10);
    for(int n=0;n<10;n++)
        for(int m=0;m<10;m++)
            testSubSet[n].push_back(Random::getInt(0,(int)data[n].size()-1));
            
    for(auto organism:population->population){
        evaluateSolo(organism,analyze,visualize);
    }
}

void RecurrentMNISTWorld::evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize){
    auto brain = organism->brain;
    double score=0.0;
    int tp=0;
    int fp=0;
    int tn=0;
    int fn=0;
    
    for(int n=0;n<10;n++){
        organism->dataCollector->setValue("c"+std::to_string(n),0);
        for(int m=0;m<testSubSet[n].size();m++){
            auto I=data[n][testSubSet[n][m]].image;
            brain->resetBrain();
            std::vector<int> localAnswerRecord;
            for(int t=0;t<28;t++){
                for(int x=0;x<28;x++)
                    brain->setInput(x, I[t][x]);
                brain->update();
            }
            //double answerSpectrum[10]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
            //answerSpectrum[n]=1.0;
            //max entropy =49830
            switch(evaluationMode){
                case 0:  //cross entropy
                    for(int i=0;i<10;i++){
                        double answer=brain->getOutput(i);
                        //answer=answerSpectrum[i];
                        //answer=Random::getFloat(-1.0, 1.0);
                        if(answer<=0.0)
                            answer=0.000000000000001;
                        if(answer>=1.0)
                            answer=0.999999999999999;
                        if(i==n){
                            score+=log2(answer);
                        } else {
                            score+=log2(1.0-answer);
                        }
                        if(i==n){
                            if(answer>0.5)
                                tp++;
                            else
                                fn++;
                        } else {
                            if(answer>0.5)
                                fp++;
                            else
                                tn++;
                        }
                       //std::cout<<answer<<" "<<tp<<" "<<fp<<" "<<tn<<" "<<fn<<" "<<std::endl;
                    }
                    break;
            }
            /*
            std::vector<double> answer,cAnswer;
            for(int t=0;t<10;t++){
                answer.push_back(brain->getOutput(t));
                if(t==n)
                    cAnswer.push_back(1.0);
                else
                    cAnswer.push_back(0.0);
            }
            if((std::max_element(answer.begin(), answer.end())-answer.begin())==n)
                correct++;
            double d=Point::dist(answer,cAnswer)/sqrt(10.0);
            score+=1.0-d;
            */
        }
    }
    switch(evaluationMode){
        case 0: //cross entropy
            //std::cout<<score<<std::endl;
            organism->dataCollector->setValue("score",pow(1.02,(double)((tp*9)+tn)));//(double)(50000-score));//pow(1.01,(double)((tp*9)+tn)));//-score);
            organism->dataCollector->setValue("crossEntropy",-score);
            organism->dataCollector->setValue("tp",(double)tp);
            organism->dataCollector->setValue("fp",(double)fp);
            organism->dataCollector->setValue("tn",(double)tn);
            organism->dataCollector->setValue("fn",(double)fn);
            break;
    }
    
    
}
