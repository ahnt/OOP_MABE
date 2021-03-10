//
//  GeneticAlgorithm.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef GeneticAlgorithm_hpp
#define GeneticAlgorithm_hpp

#include "../Optimizer.hpp"

class GeneticAlgorithm:public Optimizer{
    std::string objective;
    int nrOfEliteOrganism;
    std::vector<std::string> extraMeans;
public:
    GeneticAlgorithm();
    virtual std::shared_ptr<Population> optimize(std::shared_ptr<Population> population) override;
};


#endif /* GeneticAlgorithm_hpp */
