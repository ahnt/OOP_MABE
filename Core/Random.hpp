//
//  Random.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/16/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef Random_hpp
#define Random_hpp

#include <random>
#include <cstdint>
#include <iostream>
#include <limits.h>

//using Generator=std::mt19937;

class Random{
public:
    static std::mt19937 generator;
    static std::int32_t getInt32();
    static std::int32_t getInt32(std::int32_t inclusiveMin,std::int32_t inclusiveMax);
    static int getInt();
    static int getInt(int inclusiveMin,int inclusiveMax);
    static int getIndex(int size);
    static float getFloat();
    static float getFloat(float inclusiveMin,float inclusiveMax);
    static void seedRandom();
    static int getBinomial(int samples, double P);
    static bool P(double p);
};
    
#endif /* Random_hpp */
