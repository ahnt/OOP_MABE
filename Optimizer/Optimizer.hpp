//
//  Optimizer.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef Optimizer_hpp
#define Optimizer_hpp

#include "../Core/Population.hpp"

class Optimizer{
public:
    virtual std::shared_ptr<Population> optimize(std::shared_ptr<Population> population)=0;
};

#endif /* Optimizer_hpp */
