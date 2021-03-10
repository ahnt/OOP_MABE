//
//  CPPN.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/24/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef CPPN_hpp
#define CPPN_hpp

#include <vector>
#include <math.h>
#include "../../Genome/Genome.hpp"

class CPPNNode{
public:
    std::vector<int> wire;
    std::vector<double> weight;
    double (*thresholdFunction)(double);
    
    CPPNNode(std::shared_ptr<Genome::ReadHead> readHead,int position);
    CPPNNode(int wire1, double weight1, int wire2, double weight2,int func,int position);
    CPPNNode(int position); //this creates the final node
    double update(std::vector<double> &V);
    void show();
};

class CPPN{
public:
    std::vector<double> V;
    std::vector<std::shared_ptr<CPPNNode>> node;
    CPPN(std::shared_ptr<Genome::ReadHead> readHead,int depthWithoutLastLayer);
    CPPN(std::vector<double> sourceVector,int depthWithoutLastLayer);
    double update(double x,double y);
    void show();
};
#endif /* CPPN_hpp */
