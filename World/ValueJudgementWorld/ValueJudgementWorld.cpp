//#include "limits.h"
//#define PI 3.14159265359
//#include "../../Brain/MarkovBrain/MarkovBrain.h"
#include "ValueJudgementWorld.hpp"
#include <cmath>



ValueJudgementWorld::ValueJudgementWorld() {

  
}


// score is number of outputs set to 1 (i.e. output > 0) squared
void ValueJudgementWorld::evaluateSolo(std::shared_ptr<Organism> organism, bool analyze, bool visualize) {
    //cout << "in test world evalSolo" << endl;
    auto brain = organism->brain;
    double score = 0.0;
    double p = 0.55;
    for (int r = 0;r < 100;r++) {
        int whereIsHigh= Random::getIndex(2);
        //int whereIsHigh = Random(2);
        double P[2] = { 0.0,0.0 };
        P[whereIsHigh] = p;
        P[1 - whereIsHigh] = 1.0 - p;
        brain->resetBrain();
        bool gaveAnswer = false;
        for (int i = 0;i < 100;i++) {
            for (int j = 0;j < 2;j++) {
                if (Random::P(P[j]))
                    brain->setInput(j, 1.0);
                else
                    brain->setInput(j, 0.0);
            }
            brain->update();
            if (i > 80) {
                int action = bit(brain->getOutput(0)) + (2 * bit(brain->getOutput(1)));
                switch (action) {
                case 0: case 3:
                    break;
                case 1:
                    gaveAnswer = true;
                    if (whereIsHigh == 0)
                        score += 1.0;
                    i = 100;
                    break;
                case 2:
                    gaveAnswer = true;
                    if (whereIsHigh == 1)
                        score += 1.0;
                    i = 100;
                    break;
                }
            }
        }
    }
    organism->dataCollector->setValue("score", score);
    
}
