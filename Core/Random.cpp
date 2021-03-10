//
//  Random.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/16/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "Random.hpp"

std::mt19937 Random::generator;

std::int32_t Random::getInt32(){
    return std::uniform_int_distribution<int>(INT32_MIN,INT32_MAX)(generator);
}

std::int32_t Random::getInt32(std::int32_t inclusiveMin,std::int32_t inclusiveMax){
    return std::uniform_int_distribution<int>(inclusiveMin,inclusiveMax)(generator);
}

int Random::getInt(){
    return std::uniform_int_distribution<int>(INT_MIN,INT_MAX)(generator);
}
    
int Random::getInt(int inclusiveMin,int inclusiveMax){
    return std::uniform_int_distribution<int>(inclusiveMin,inclusiveMax)(generator);
}

int Random::getIndex(int size){
    return std::uniform_int_distribution<int>(0,size-1)(generator);
}


float Random::getFloat(){
    return std::uniform_real_distribution<float>()(generator);
}

float Random::getFloat(float inclusiveMin,float inclusiveMax){
    return std::uniform_real_distribution<float>(inclusiveMin,inclusiveMax)(generator);
}

bool Random::P(double p){
    double r=std::uniform_real_distribution<float>(0.0,1.0)(generator);
    if(r<p)
        return true;
    return false;
}


void Random::seedRandom(){
    std::random_device actualRandomDevice;
    int theSeed = actualRandomDevice();
    generator.seed(theSeed);
    std::cout << "Random seed changed to: " <<theSeed<<std::endl;
}

int Random::getBinomial(int samples, double P){
    return std::binomial_distribution<>(samples, P)(generator);
}
//Random::Generator=common;
/*
inline Generator& getCommonGenerator() {
    // to seed, do get_common_generator().seed(value);
    static Generator common;
    return common;
}*/

