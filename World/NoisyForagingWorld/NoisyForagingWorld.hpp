#pragma once

#include "../World.hpp"


#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <utility>

using namespace std;

class NoisyForagingWorld : public World {

public:



    bool doHammer, randomizeFood, randomizeHome, fixedDistance;
    double hiddenNodeNoise, inputNodeNoise, outputNodeNoise;
    bool useHiddenNodeNoise, useInputNodeNoise, useOutputNodeNoise;
    int startDistance, xDim, yDim;
    string pathFileName;
    string brainName;

    NoisyForagingWorld();
    virtual ~NoisyForagingWorld() = default;

    virtual void evaluateSolo(std::shared_ptr<Organism> organism, bool analyze, bool visualize) override;

    virtual int nrOfSensors(void) { return 21; } //force this -> JS?
    virtual int nrOfMotors(void) { return 3; }

};
