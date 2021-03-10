
#include "SimpleBerryWorld.hpp"
//#include "limits.h"
//#define PI 3.14159265359
//#include "../../Brain/MarkovBrain/MarkovBrain.h"
#include <cmath>

SimpleBerryWorld::SimpleBerryWorld() {

}

#define xDim 8
#define yDim 8
#define _wall 3
#define _empty 0
#define _foodA 1
#define _foodB 2

// score is number of outputs set to 1 (i.e. output > 0) squared
void SimpleBerryWorld::evaluateSolo(std::shared_ptr<Organism> organism, bool analyze, bool visualize) {
	//cout << "in test world evalSolo" << endl;
	auto brain = organism->brain;
	double score = 0.0;
	double TSC = 2.5;
	int area[xDim][yDim];
	int X, Y, D;
	int xm[9] = { 0,1,1,1,0,-1,-1,-1,0 };
	int ym[9] = { -1,-1,0,1,1,1,0,-1,0 };
	int lastFoodEaten = -1;
	int foodEaten[2] = { 0,0 };
	int switches = -1;
	//initialization
	for (int i = 0;i < xDim;i++)
		for (int j = 0;j < yDim;j++) {
			if ((i == 0) || (j == 0) || (i == xDim - 1) || (j == yDim - 1) || Random::P(0.1)) {
				area[i][j] = _wall;
			}
			else {
				if (Random::P(0.5))
					area[i][j] = _foodA;
				else
					area[i][j] = _foodB;
			}
		}
	X = Random::getIndex(xDim - 2) + 1;
	Y = Random::getIndex(yDim - 2) + 1;
	D = Random::getIndex(8);
	brain->resetBrain();
	for (int t = 0;t < 200;t++) {
		int I = 0;
		for (int i = -1;i < 2;i++) {
			brain->setInput(I, area[(X + xm[(D + i) & 7]) & (xDim - 1)][(Y + ym[(D + i) & 7]) & (yDim - 1)] & 1);
			I++;
			brain->setInput(I, (area[(X + xm[(D + i) & 7]) & (xDim - 1)][(Y + ym[(D + i) & 7]) & (yDim - 1)] >> 1) & 1);
			I++;
		}
		brain->setInput(I, area[X][Y] & 1);
		I++;
		brain->setInput(I, (area[X][Y] >> 1) & 1);
		I++;
		brain->update();
		int action = bit(brain->getOutput(0)) + (2 * bit(brain->getOutput(1))) + (4 * bit(brain->getOutput(2)));
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
			if (area[X + xm[D]][Y + ym[D]] != _wall) {
				if (area[X][Y] == 0) {
					if (Random::P(0.5))
						area[X][Y] = _foodA;
					else
						area[X][Y] = _foodB;
				}
				X += xm[D];
				Y += ym[D];
			}
			break;
		case 4: case 5:	case 6: case 7:
			switch (area[X][Y]) {
			case 0:
			case 3:
				break;
			case _foodA:
				if (lastFoodEaten == -1)
					lastFoodEaten = _foodA;
				if (lastFoodEaten == _foodA)
					score += 1.0;
				else {
					score -= TSC;
					lastFoodEaten = _foodA;
					switches++;
				}
				foodEaten[0]++;
				area[X][Y] = _empty;
				break;
			case _foodB:
				if (lastFoodEaten == -1)
					lastFoodEaten = _foodB;
				if (lastFoodEaten == _foodB)
					score += 1.0;
				else {
					score -= TSC;
					lastFoodEaten = _foodB;
					switches++;
				}
				foodEaten[1]++;
				area[X][Y] = _empty;
				break;
			}
			break;
		}
	}
	if (score < 0.0)
		score = 0.0;
	organism->dataCollector->setValue("score", score);
	organism->dataCollector->setValue("Food0", foodEaten[0]);
	organism->dataCollector->setValue("Food1", foodEaten[1]);
	organism->dataCollector->setValue("switches", switches);
	
}