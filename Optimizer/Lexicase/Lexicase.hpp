//
//  Lexicase.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 11/13/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef Lexicase_hpp
#define Lexicase_hpp

#include "../Optimizer.hpp"

class Lexicase:public Optimizer{
    std::vector<std::string> theKeys;
    std::vector<bool> maximizeKey;
public:
    Lexicase();
    virtual std::shared_ptr<Population> optimize(std::shared_ptr<Population> population) override;
};

#endif /* Lexicase_hpp */
