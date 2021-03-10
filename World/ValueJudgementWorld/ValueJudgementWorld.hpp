#pragma once


#include "../World.hpp"


#include <stdlib.h>
#include <thread>
#include <vector>
#include <random>

using namespace std;

class ValueJudgementWorld : public World {

public:

    ValueJudgementWorld();


    virtual void evaluateSolo(std::shared_ptr<Organism> organism, bool analyze, bool visualize) override;
       
    virtual int nrOfSensors(void) { return 2; } //force this -> JS?
    virtual int nrOfMotors(void) { return 2; }


};
