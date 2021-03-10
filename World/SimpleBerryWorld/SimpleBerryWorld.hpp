#pragma once


#include "../World.hpp"

#include <stdlib.h>
#include <thread>
#include <vector>

using namespace std;

class SimpleBerryWorld : public World {

public:



	//string groupName;
	string brainName;

	SimpleBerryWorld();
	virtual ~SimpleBerryWorld() = default;


	virtual void evaluateSolo(std::shared_ptr<Organism> organism, bool analyze, bool visualize) override;
	/*
	virtual unordered_map<string, unordered_set<string>> requiredGroups() override {
		return { { groupName,{ "B:" + brainName + ",8," + to_string(3) } } }; // default requires a root group and a brain (in root namespace) and no genome 
	}*/
	virtual int nrOfSensors(void) { return 8; } //force this -> JS?
	virtual int nrOfMotors(void) { return 3; }

};