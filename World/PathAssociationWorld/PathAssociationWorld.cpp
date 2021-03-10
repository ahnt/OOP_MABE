#include "PathAssociationWorld.hpp"
#include "../../Brain/MarkovBrain/MarkovBrain.hpp"
//#include "../../Genome/CircularGenome/CircularGenome.h"

const int xm[8] = { 0,1,1,1,0,-1,-1,-1 };
const int ym[8] = { -1,-1,0,1,1,1,0,-1 };
/*
shared_ptr<ParameterLink<double>> PathAssociationWorld::poisonValuePL = Parameters::register_parameter("WORLD_PATHASSOCIATION-poisonValue", 1.0, "the punishment for stepping off the path");
shared_ptr<ParameterLink<double>> PathAssociationWorld::probabilitySignChangePL = Parameters::register_parameter("WORLD_PATHASSOCIATION-probabilitySignChange", 0.0, "the probability the sign will change after a successful 2-sign navigation");

shared_ptr<ParameterLink<string>> PathAssociationWorld::groupNamePL = Parameters::register_parameter("WORLD_PATHASSOCIATION_NAMES-groupName", (string)"root", "name of group to be evaluated");
shared_ptr<ParameterLink<string>> PathAssociationWorld::brainNamePL = Parameters::register_parameter("WORLD_PATHASSOCIATION_NAMES-brainName", (string)"root", "name of brains used to control organisms\nroot = use empty name space\nGROUP:: = use group name space\n\"name\" = use \"name\" namespace at root level\nGroup::\"name\" = use GROUP::\"name\" name space");
*/



PathAssociationWorld::PathAssociationWorld() {
    
   // probabilitySignChange = (PT == nullptr) ? probabilitySignChangePL->lookup() : PT->lookupDouble("WORLD_PATHASSOCIATION-probabilitySignChange");
    probabilitySignChange = Parameters::getDouble("PathAssociationWorld - probabilitySignChange", 0.0);
   // poisonValue = (PT == nullptr) ? poisonValuePL->lookup() : PT->lookupDouble("WORLD_PATHASSOCIATION-poisonValue");
    poisonValue = Parameters::getDouble("PathAssociationWorld - poisonValue", 1.0);
    /*
    groupName = (PT == nullptr) ? groupNamePL->lookup() : PT->lookupString("WORLD_PATHASSOCIATION_NAMES-groupName");
    brainName = (PT == nullptr) ? brainNamePL->lookup() : PT->lookupString("WORLD_PATHASSOCIATION_NAMES-brainName");

    popFileColumns.clear();
    popFileColumns.push_back("score");
    */
   
}

//PathAssociationWorld::~PathAssociationWorld () ;

/*
void PathAssociationWorld::evaluate(shared_ptr<Group> group, bool groupEvaluation, bool analyse, bool visualize, bool debug){
    makeWorld();
    //showWorld();
    AbstractWorld::evaluate(group, groupEvaluation, analyse, visualize, debug);
}
 */

void PathAssociationWorld::evaluateSolo(std::shared_ptr<Organism> organism, bool analyze, bool visualize) {
    makeWorld();
    int X = startX;
    int Y = startY;
    int D = startD;
    double score = 0.0;
    int bestScore = 0;
    int misshaps = 0;
    int uniqueSignsSeen = 0;
    int changeAfterUniqueSignsSeen = 2;
    //bool just
    auto brain = organism->brain;
    brain->resetBrain();
    for (int t = 0;t < 150;t++) {
        int location = area[X][Y];
        if (dist[X][Y] == bestScore + 1) {
            bestScore = dist[X][Y];
            score += (double)bestScore;
            if (location < 0) uniqueSignsSeen++;
            if (uniqueSignsSeen >= changeAfterUniqueSignsSeen) {
                if (Random::P(probabilitySignChange)) {
                    changeAfterUniqueSignsSeen = uniqueSignsSeen + 2;
                    if (LRTruth[0] == 2) { /// hard-switch the symbol mapping (always different than previous one)
                        LRTruth[0] = 3;
                        LRTruth[1] = 2;
                    }
                    else {
                        LRTruth[0] = 2;
                        LRTruth[1] = 3;
                    }
                    //if(Random::P(0.5)) /// random-switch the symbol mapping (could switch to the same one)
                    //{
                    //    LRTruth[0]=2;
                    //    LRTruth[1]=3;
                    //} else {
                    //    LRTruth[0]=3;
                    //    LRTruth[1]=2;
                    //}
                }
            }
        }
        if (dist[X][Y] == 0) {
            score -= poisonValue;
            misshaps++;
        }
        if (location < 0) { /// if location is one of the L/R markers
            location = LRTruth[abs(location) - 1]; /// then interpret it according to the current mapping
        }
        for (int i = 0;i < 4;i++)
            brain->setInput(i, 0.0);
        brain->setInput(location, 1.0);
        brain->update();
        int action = bit(brain->getOutput(0)) + (bit(brain->getOutput(1)) << 1);
        switch (action) {
        case 0:
            break;
        case 1:
            D = (D + 1) & 7;
            break;
        case 2:
            D = (D - 1) & 7;
            break;
        case 3:
            X = (X + xm[D]) & (xDim - 1);
            Y = (Y + ym[D]) & (yDim - 1);
            break;
        }
    }
    if (score < 1.0)
        score = 1.0;
    organism->dataCollector->setValue("score", log(score) / log(1.05));
   // org->dataMap.Set("score", log(score) / log(1.05));
    //org->dataMap.Set("gateUsage", brain->description());
    
}

void PathAssociationWorld::makeWorld() {
    int LR[2] = { 2,3 };
    xDim = 32;
    yDim = 32;
    intervallMin = 2;
    intervallMax = 4;
    pathLength = 80;
    bool conflict = false;
    do {
        conflict = false;

        area.resize(xDim);
        for (int i = 0;i < xDim;i++) {
            area[i].resize(yDim);
            for (int j = 0;j < yDim;j++) {
                area[i][j] = 0;
            }
        }
        dist = area;
        startX = rand() & (xDim - 1);
        startY = rand() & (yDim - 1);
        startD = rand() & 7;
        int L = 0;
        int C = Random::getInt(intervallMin, intervallMax);
        int X = startX;
        int Y = startY;
        int D = startD;
        switch (Random::getIndex(2)) {
        case 0:
            LRTruth[0] = 2;
            LRTruth[1] = 3;
            break;
        case 1:
            LRTruth[0] = 3;
            LRTruth[1] = 2;
            break;
        default:
            if (Random::P(0.5))
            {
                LRTruth[0] = 2;
                LRTruth[1] = 3;
            }
            else {
                LRTruth[0] = 3;
                LRTruth[1] = 2;
            }
        }
        while (L < pathLength) {
            L++;
            area[X][Y] = 1;
            if (dist[X][Y] != 0)
                conflict = true;
            dist[X][Y] = L;
            C--;
            if (C == 0) {
                if (Random::P(0.5)) {
                    D = (D + 1) & 7;
                    area[X][Y] = -1;
                }
                else {
                    D = (D - 1) & 7;
                    area[X][Y] = -2;
                }
                C = Random::getInt(intervallMin, intervallMax);
                //here the marker
            }
            X = (X + xm[D]) & (xDim - 1);
            Y = (Y + ym[D]) & (yDim - 1);
        }
    } while (conflict);
    
}

void PathAssociationWorld::showWorld() {
    for (int i = 0;i < area.size();i++) {
        for (int j = 0;j < area[i].size();j++)
            printf("%i", area[i][j]);
        printf("\n");
    }
}


