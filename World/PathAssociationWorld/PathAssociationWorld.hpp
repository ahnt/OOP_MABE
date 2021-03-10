#pragma once

#include "../World.hpp"

#include <stdlib.h>
#include <thread>
#include <vector>

using namespace std;

class PathAssociationWorld : public World {

public:
 

    vector<vector<int>> area, dist;
    int xDim, yDim;
    int startX, startY, startD;
    int intervallMin, intervallMax;
    int LRTruth[2];
    int pathLength;
    double poisonValue;
    double hiddenNodeNoise, inputNodeNoise, outputNodeNoise;
    bool useHiddenNodeNoise, useInputNodeNoise, useOutputNodeNoise;
    double probabilitySignChange;
    string brainName;

    PathAssociationWorld();
    virtual ~PathAssociationWorld() = default;

    virtual void evaluateSolo(std::shared_ptr<Organism> organism, bool analyze, bool visualize) override;
    /*
    virtual unordered_map<string, unordered_set<string>> requiredGroups() override {
        return { { groupName,{ "B:" + brainName + ",4,2" } } }; // default requires a root group and a brain (in root namespace) and no genome
    }*/
    virtual int nrOfSensors(void) { return 4; } //force this -> JS?
    virtual int nrOfMotors(void) { return 2; }
    virtual void makeWorld();
    virtual void showWorld();
};