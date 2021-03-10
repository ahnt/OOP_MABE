#include "NoisyForagingWorld.hpp"
#include "../../Brain/MarkovBrain/MarkovBrain.hpp"
//#include "../../Genome/CircularGenome/CircularGenome.h"
#include <math.h>
#define cPI 3.14159265

//helper function inside
bool inside(int X, int Y, int tx, int ty, int d) {
    if ((X >= tx - d) && (X <= tx + d) && (Y >= ty - d) && (Y <= ty + d))
        return true;
    return false;
}

NoisyForagingWorld::NoisyForagingWorld() {
    doHammer=Parameters::getBool("NF_doHammer", false);
    randomizeFood=Parameters::getBool("NF_randomizeFood", false);
    randomizeHome=Parameters::getBool("NF_randomizeHome", false);
    fixedDistance=Parameters::getBool("NF_fixedDistance", false);
    startDistance=Parameters::getInt("NF_startDistance", 3);
    xDim=Parameters::getInt("NF_xDim", 128);
    yDim=Parameters::getInt("NF_yDim", 128);
    pathFileName=Parameters::getString("NF_pathFileName", std::string("defaultPathFile.csv"));

    hiddenNodeNoise = 0.0;
    inputNodeNoise = 0.0;
    outputNodeNoise = 0.0;
    useHiddenNodeNoise = false;
    useInputNodeNoise = false;
    useOutputNodeNoise = false;
    /*
    // columns to be added to ave file
    popFileColumns.clear();


    // aveFileColumns.push_back("score");
    popFileColumns.clear();
    popFileColumns.push_back("score");
    popFileColumns.push_back("score_VAR"); // specifies to also record the variance (performed automatically because _VAR)

//    aveFileColumns.push_back("score");*/
}


//void NoisyForagingWorld::runWorldSolo(shared_ptr<Organism> org, bool analyse, bool visualize, bool debug) {
void NoisyForagingWorld::evaluateSolo(std::shared_ptr<Organism> organism, bool analyze, bool visualize) {
    double fitness = 0.0;
    int i, j;
    int endTime = 9900 + (rand() & 255);
    int dX, dY, pX, pY;
    int agentState = 0;
    int stepsSinceFoodIntake = 0;
    double angle[8] = { 0.0, 45.0, 90.0,135.0,180.0,225.0,270.0,315.0 };
    int xm[9] = { 0,1,1,1,0,-1,-1,-1,0 };
    int ym[9] = { -1,-1,0,1,1,1,0,-1,0 };
    double foodCollected = 0.0;
    double alpha;
    bool gotHammered, gotBlessed, gotTeased;
    int totalNrReturns = 0;
    int timeOut = 0, timeIn = 0;
    vector<int> timeOutCollector, timeInCollector;
    int food = 0;
    timeOutCollector.clear();
    timeInCollector.clear();
    int foodDist = startDistance;
    int foodCollections = 0;
    int dir = rand() & 7;
    int xPos = xDim / 2;
    int yPos = yDim / 2;
    alpha = 360.0 * (double)rand() / (double)RAND_MAX;
    dX = (int)(sin(alpha / 180.0 * 3.1415) * (double)foodDist);
    dY = (int)(cos(alpha / 180.0 * 3.1415) * (double)foodDist);
    pX = (xDim / 2) + dX;
    pY = (yDim / 2) + dY;
    //need to initialize fitness
    fitness = 0.0;
    auto brain = organism->brain;
    brain->resetBrain();
    FILE* D;
    if (visualize) {
        D = fopen(pathFileName.c_str(), "w+t");
        fprintf(D, "t,X,Y,D,A,tx,ty,fed,hammered,blessed,teased\n");
    }
    //the core loop
    for (int t = 0;t < endTime;t++) {
        gotHammered = false;
        gotBlessed = false;
        gotTeased = false;
        if (doHammer)
            if ((rand() % 25) == 1) {
                dir = rand() & 7;
                gotHammered = true;
            }
        brain->setInput(0, 0); // on food
        brain->setInput(1, 0); // on home
        brain->setInput(2, 0); // facing food
        brain->setInput(3, 0); // facing home
        if (inside(xPos, yPos, xDim / 2, yDim / 2, 1))
            brain->setInput(1, 1);
        if (inside(xPos + xm[dir], yPos + ym[dir], xDim / 2, yDim / 2, 1))
            brain->setInput(3, 1);
        if (inside(xPos, yPos, pX, pY, 1)) { //inside food
            brain->setInput(0, 1);
        }
        if (inside(xPos + xm[dir], yPos + ym[dir], pX, pY, 1)) { //facing food
            brain->setInput(2, 1);
        }
        for (i = 0;i < 8;i++) {
            brain->setInput(4 + i, 0);
            brain->setInput(12 + i, 0);
        }

        //home mapped into the retina:
        double dx = (xDim / 2) - xPos;
        double dy = (yDim / 2) - yPos;
        double d = sqrt((dx * dx) + (dy * dy));
        double Ux, Uy, Vx, Vy;
        Ux = dx / d;
        Uy = dy / d;
        Vx = cos(angle[dir] * (cPI / 180.0));
        Vy = sin(angle[dir] * (cPI / 180.0));
        double theAngle = atan2(((Ux * Vy) - (Uy * Vx)), ((Ux * Vx) + (Uy * Vy))) * 180.0 / cPI;
        //		if(isnan(theAngle))
        if (theAngle != theAngle)
            theAngle = 0.0;
        if ((!randomizeHome) || ((float)rand() / (float)RAND_MAX < (1.0 / d))) {
            //if(d<5.0)
            brain->setInput(4 + (int)((theAngle + 180.0) / 360.0 * 8.0), 1);
            gotBlessed = true;
        }
        //food mapped into the second retina
        dx = pX - xPos;
        dy = pY - yPos;
        d = sqrt((dx * dx) + (dy * dy));
        Ux = dx / d;
        Uy = dy / d;
        Vx = cos(angle[dir] * (cPI / 180.0));
        Vy = sin(angle[dir] * (cPI / 180.0));
        theAngle = atan2(((Ux * Vy) - (Uy * Vx)), ((Ux * Vx) + (Uy * Vy))) * 180.0 / cPI;
        //        if(isnan(theAngle))
        if (theAngle != theAngle)
            theAngle = 0.0;
        if ((!randomizeFood) || ((float)rand() / (float)RAND_MAX < (1.0 / d))) {
            brain->setInput(12 + (int)((theAngle + 180.0) / 360.0 * 8.0), 1);
            gotTeased = true;
        }
        brain->update();
        int action = (bit(brain->getOutput(0)) << 2) +
            (bit(brain->getOutput(1)) << 1) +
            bit(brain->getOutput(2));
        if (agentState == 0) {
            timeOut++;
        }
        else {
            timeIn++;
        }
        switch (action) {
        case 0:
            //idle -> do nothing
            break;
        case 1:
            //turn left
            dir = (dir + 1) & 7;
            break;
        case 2:
            //turn right
            dir = (dir - 1) & 7;
            break;
        case 3:
            //go forward
            stepsSinceFoodIntake++;
            if (inside(xPos, yPos, xDim / 2, yDim / 2, 1)) {
                if (agentState == 1) { //you come from food :)
                    //                            agent->fitness+=foodCollected/(double)stepsSinceFoodIntake*(double)foodDist;
                    //                        agent->fitness+=foodCollected;
                    fitness += (1.0 / (double)stepsSinceFoodIntake) * (double)foodDist * (double)foodDist;
                    foodCollected = 0.0;
                    if (!fixedDistance) {
                        foodDist += 1;
                        if (foodDist > 100)
                            foodDist = 100;
                    }
                    else {
                        foodDist = startDistance;
                    }
                    foodCollections++;
                    alpha = 360.0 * (double)rand() / (double)RAND_MAX;
                    dX = (int)(sin(alpha / 180.0 * 3.1415) * (double)foodDist);
                    dY = (int)(cos(alpha / 180.0 * 3.1415) * (double)foodDist);
                    pX = (xDim / 2) + dX;
                    pY = (yDim / 2) + dY;
                    agentState = 0;
                    totalNrReturns++;
                    timeInCollector.push_back(timeIn);
                    timeOutCollector.push_back(timeOut);
                    timeIn = 0;timeOut = 0;
                    food = foodDist;
                }
            }
            xPos += xm[dir];
            yPos += ym[dir];
            break;
        case 4: case 5: case 6: case 7:
            //collect
            if (inside(xPos, yPos, pX, pY, 1)) {
                foodCollected = 1.0;
                agentState = 1; //you just collected food...
                stepsSinceFoodIntake = 0;
            }
            break;
        }
        if (visualize) {
            fprintf(D, "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i\n", t, xPos, yPos, dir, action, pX, pY, stepsSinceFoodIntake, (int)gotHammered, (int)gotBlessed, (int)gotTeased);
        }
    }
    double meanIn = 0.0;
    double meanOut = 0.0;
    if ((timeInCollector.size() > 0) && (timeOutCollector.size() > 0)) {
        for (int z = 0;z < timeInCollector.size();z++)
            meanIn += (double)timeInCollector[z];
        for (int z = 0;z < timeOutCollector.size();z++)
            meanOut += (double)timeOutCollector[z];
        meanIn /= (double)timeInCollector.size();
        meanOut /= (double)timeOutCollector.size();
    }
    if(fitness<1.0)
        fitness=1.0;
    organism->dataCollector->setValue("score", log(fitness) / log(1.05));
   // org->dataMap.Set("score", log(fitness) / log(1.05));
   // org->dataMap.Set("gateUsage", brain->description());
    
}



