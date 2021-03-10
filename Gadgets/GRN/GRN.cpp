//
//  GRN.cpp
//  NoCompromiseMABE
//
//  Created by Arend Hintze on 4/8/20.
//  Copyright © 2020 ArendHintze. All rights reserved.
//

#include "GRN.hpp"

GRN::GRN(int brainSize){
    G=brainSize;
    V.assign(brainSize,1.0/(double)G);
    M=std::vector<std::vector<double>>(G,std::vector<double>(G,0.0));
}

void GRN::setWeight(int x, int y,double value){
    M[x][y]=value;
}

void GRN::addWeight(int x, int y,double value){
    M[x][y]+=value;
    //no clamp in C++11
    if(M[x][y]>1.0)
        M[x][y]=1.0;
    if(M[x][y]<-1.0)
        M[x][y]=-1.0;
}

void GRN::update(double temperature){
    std::vector<double> nV;
    nV.resize(G);
    for(int n=0;n<G;n++){
        nV[n]=0.0;
        for(int m=0;m<G;m++){
            nV[n]+=V[m]*M[n][m];
        }
        nV[n]=(2.0/(1.0+exp(-nV[n])))-1.0;
    }
    for(int n=0;n<G;n++)
        V[n]=V[n]-((V[n]-nV[n])*temperature);
    nV.clear();
}
