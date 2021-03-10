//
//  Utilities.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/16/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "Utilities.hpp"
int Utilities::currentGeneration=-1;
int Utilities::ID=0;

int Utilities::getID(){
    int I=ID;
    ID++;
    return I;
}

/*
int Utilities::scaleInt(int theValue,int currentMin,int currentMax,int targetMin,int targetMax){
    double percentage = ((double)theValue-(double)currentMin)/((double)currentMin-(double)currentMax);
    return (int)((percentage) * (double)(targetMin-targetMax))+targetMin;
}
 */

float Utilities::scaleFloatP(float value,float targetMin,float targetMax){
    return ((targetMax-targetMin)*value)+targetMin;
}

std::vector<std::string> Utilities::splitStringIntoVectorOfString(std::string S,char delimiter){
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(S);
    while (std::getline(tokenStream, token, delimiter)){
        tokens.push_back(token);
    }
    return tokens;
}

void Utilities::saveStringToFile(std::string S,std::string filename){
    std::ofstream O;
    O.open(filename);
    O<<S;
    O.close();
}

std::vector<double> Utilities::linspace(double start,double end,int nrOfElements){
    std::vector<double> R;
    double delta=(end-start)/(nrOfElements-1);
    for(int i=0;i<nrOfElements;i++)
        R.push_back(start+(i*delta));
    return R;
}

double Utilities::clamp(double value,double min, double max){
    if (value<min)
        return min;
    if(value>max)
        return max;
    return value;
}

Image::Image(std::string S){
    int i=0;
    for(int x=0;x<28;x++)
        for(int y=0;y<28;y++){
            image[x][y]=(int)S[i]-48;
            i++;
        }
}

void Image::show(){
    for(int x=0;x<28;x++){
        for(int y=0;y<28;y++){
            std::cout<<image[x][y];
        }
        std::cout<<std::endl;
    }
}
