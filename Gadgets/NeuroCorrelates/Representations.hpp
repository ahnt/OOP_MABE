//
//  Representations.hpp
//  NoCompromiseMABE
//
//  Created by Arend Hintze on 4/8/20.
//  Copyright © 2020 ArendHintze. All rights reserved.
//

#ifndef Representations_hpp
#define Representations_hpp

#include <vector>
#include <math.h>
#include <map>
#include <numeric>
#include <algorithm>

class Representations{
public:
    static std::map<int,double> computeSymbolCount(std::vector<int> &randomVariable);
    static double computeEntropy(std::map<int,double> &symbolCount);
    static void normalizeSymbolCount(std::map<int,double> &symbolCount);
    static std::vector<int> extractBitPattern(std::vector<std::vector<int>> &measurements,std::vector<int> bitPattern);
    static double computeR(std::vector<int> world,std::vector<int> brain,std::vector<int> sensors);
    static double computeExclusiveR(std::vector<int> W,std::vector<int> O, std::vector<int> T, std::vector<int> S);
    static std::vector<std::vector<int>> medianBinarization(std::vector<std::vector<double>> &data,std::vector<int> thresholdColumns=std::vector<int>(),std::vector<double> thresholds=std::vector<double>());
    static std::vector<int> joinRandomVariables(std::vector<int> &R1,std::vector<int> &R2);
    
};

#endif /* Representations_hpp */
