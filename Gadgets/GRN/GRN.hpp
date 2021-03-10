//
//  GRN.hpp
//  NoCompromiseMABE
//
//  Created by Arend Hintze on 4/8/20.
//  Copyright © 2020 ArendHintze. All rights reserved.
//

#ifndef GRN_hpp
#define GRN_hpp

#include <vector>
#include <math.h>
#include "../../Genome/Genome.hpp"

class GRN{
public:
    std::vector<std::vector<double>> M;
    std::vector<double> V;
    int G;
    GRN(int brainSize);
    
    void setWeight(int x, int y,double value);
    void addWeight(int x, int y,double value);
    virtual void update(double temperature);
};

#endif /* GRN_hpp */
