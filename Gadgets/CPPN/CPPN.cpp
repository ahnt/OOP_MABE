//
//  CPPN.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/24/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "CPPN.hpp"

double sigmoid(double x){ return 2.0*((1.0 / (1.0 + exp(-1.0 * x)))-.5);}
double (*fcnPtr[4])(double) = {tanh,sin,cos,sigmoid};

CPPNNode::CPPNNode(std::shared_ptr<Genome::ReadHead> readHead,int position){
    wire.clear();
    weight.clear();
    for(int i=0;i<2;i++){
        wire.push_back(((int)readHead->getUInt32(coding))%position);
        weight.push_back(readHead->getRangedFloat(-4.0, 4.0,coding));
    }
    thresholdFunction=fcnPtr[readHead->getUInt32(coding)&3];
}

CPPNNode::CPPNNode(int wire1, double weight1, int wire2, double weight2,int func,int position){
    wire.clear();
    weight.clear();
    wire.push_back(wire1%position);
    weight.push_back(weight1);
    wire.push_back(wire2%position);
    weight.push_back(weight2);
    thresholdFunction=fcnPtr[func&3];
}


CPPNNode::CPPNNode(int position){
    wire.clear();
    weight.clear();
    for(int i=0;i<position;i++){
        wire.push_back(i);
        weight.push_back(1.0);
    }
    thresholdFunction=sigmoid;
}

double CPPNNode::update(std::vector<double> &V){
    double I=0.0;
    for(int i=0;i<(int)wire.size();i++)
        I+=V[wire[i]]*weight[i];
    return thresholdFunction(I);
}

void CPPNNode::show(){
    printf("[");
    for(int i=0;i<wire.size();i++)
        printf(" %i",wire[i]);
    printf("]\n");
}

CPPN::CPPN(std::shared_ptr<Genome::ReadHead> readHead,int depthWithoutLastLayer){
    V.resize(3+depthWithoutLastLayer);
    node.clear();
    for(int i=0;i<depthWithoutLastLayer;i++)
        node.push_back(std::make_shared<CPPNNode>(readHead,i+2));
    node.push_back(std::make_shared<CPPNNode>(depthWithoutLastLayer+2));
}

CPPN::CPPN(std::vector<double> sourceVector,int depthWithoutLastLayer){
    V.resize(3+depthWithoutLastLayer);
    node.clear();
    int z=0;
    for(int i=0;i<depthWithoutLastLayer;i++){
        int wire1,wire2,func;
        double weight1,weight2;
        wire1=(int)(sourceVector[z++]+1.0)*depthWithoutLastLayer;
        weight1=sourceVector[z++]*4.0;
        wire2=(int)(sourceVector[z++]+1.0)*depthWithoutLastLayer;
        weight2=sourceVector[z++]*4.0;
        func=(int)(sourceVector[z++])&3;
        node.push_back(std::make_shared<CPPNNode>(wire1,weight1,wire2,weight2,func,i+2));
    }
    node.push_back(std::make_shared<CPPNNode>(depthWithoutLastLayer+2));
}


double CPPN::update(double x,double y){
    V[0]=x;
    V[1]=y;
    for(int i=0;i<node.size();i++)
        V[2+i]=node[i]->update(V);
    return V[V.size()-1];
}


void CPPN::show(){
    for(int i=0;i<node.size();i++){
        printf("%i ",i+2);
        node[i]->show();
    }
}
