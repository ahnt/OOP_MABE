//
//  NoveltySearch.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 11/13/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef NoveltySearch_hpp
#define NoveltySearch_hpp

#include <algorithm>
#include <numeric>
#include <fstream>
#include "../Optimizer.hpp"

#include "../../Genome/BioGenome/BioGenome.hpp"
#include "../../Brain/ANNBrain/ANNBrain.hpp"


class NoveltySearch:public Optimizer{
    std::vector<std::vector<double>> archive;
    std::vector<double> aW;
    std::ofstream archiveFileHandle;
    std::ofstream solutionsFileHandle;

    int archiveSize;
    int mode;
    void updateaW();
    
    std::shared_ptr<Population> evaluators;
    
public:
    NoveltySearch();
    virtual std::shared_ptr<Population> optimize(std::shared_ptr<Population> population) override;
    static double dist(std::vector<double> A,std::vector<double> B);
    std::vector<double> noveltyList(std::vector<std::vector<double>> &group,std::vector<double> &individual);
    double noveltyValue(std::vector<double> &list,int k=5, int first=1);
};

#endif /* NoveltySearch_hpp */
