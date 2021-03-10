//
//  DeceptiveMazeWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "NoneDeceptiveMazeWorld.hpp"

NoneDeceptiveMazeWorld::NoneDeceptiveMazeWorld(){
    startX=Parameters::getDouble("NoneDeceptiveMazeWorld-startX",0.1);
    startY=Parameters::getDouble("NoneDeceptiveMazeWorld-startY",0.1);
    targetX=Parameters::getDouble("NoneDeceptiveMazeWorld-targetX",0.1);
    targetY=Parameters::getDouble("NoneDeceptiveMazeWorld-targetY",0.9);
    maze.push_back(std::make_shared<Maze>(startX,
                                          startY,
                                          targetX,
                                          targetY,
                                          std::vector<Line2D>({
                                                Line2D(0,0,1,0),
                                                Line2D(1,0,1,1),
                                                Line2D(1,1,0,1),
                                                Line2D(0,1,0,0),
                                                Line2D(0.0,0.75,0.25,0.70),
                                                Line2D(0.25,0.70,0.65,0.8),
                                                Line2D(0.25,0.70,0.25,0.25),
                                                Line2D(0.0,0.60,0.15,0.4),
                                                Line2D(0.35,0.0,0.55,0.2),
                                                Line2D(0.25,0.5,0.8,0.2),
                                                Line2D(1.0,0.4,0.5,0.6)})
                                          ));
    maze.push_back(std::make_shared<Maze>(startX,
                                          startY,
                                          targetX,
                                          targetY,
                                          std::vector<Line2D>({
                                                Line2D(0,0,1,0),
                                                Line2D(1,0,1,1),
                                                Line2D(1,1,0,1),
                                                Line2D(0,1,0,0),
                                                Line2D(0.0,0.2,0.5,0.2),
                                                Line2D(0.2,0.4,1.0,0.4),
                                                Line2D(0.3,0.6,0.7,0.6),
                                                Line2D(0.2,0.4,0.2,0.6),
                                                Line2D(0.8,0.6,0.8,0.8),
                                                Line2D(0.6,0.2,0.8,0.2),
                                                Line2D(0.0,0.8,0.8,0.8)})
                                          ));
    maze[0]->computeDistMatrix(101, 101);
    maze[1]->computeDistMatrix(101, 101);
    
    //area->show();
    //exit(0);
    Parameters::setParameter("LODManager-parametersToSave",std::string("score,behavior,trackX,trackY"));
    Parameters::setParameter("GeneticAlgorithm-extraMeans",std::string("temp,maze"));
    //Parameters::setParameter("BioGenome-perSitePointMutationRate", 0.001);
    //Parameters::setParameter(std::string("BioGenome-seedStartCodonsPerGene"),0);
    temperatures[0]=Parameters::getDouble("NoneDeceptiveMazeWorld-tempA", 0.8);
    temperatures[1]=Parameters::getDouble("NoneDeceptiveMazeWorld-tempB", 0.5);

    currentMaze=0;
    currentTemp=0;
    Parameters::setParameter(std::string("NeuroEvoDevoBrain-maze"), (double)currentMaze);
    Parameters::setParameter("NeuroEvoDevoBrain-temperature", temperatures[currentTemp]);
    Parameters::setParameter("LODManager-GENOMEsaveIntervall",50);
    Parameters::setParameter("LODManager-LODsaveIntervall",50);
    
}

void NoneDeceptiveMazeWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) {
    currentTemp=Parameters::getDouble(std::string("NeuroEvoDevoBrain-temperature"), 0.5);
    currentMaze=(int)Parameters::getDouble(std::string("NeuroEvoDevoBrain-maze"), (double)1);
    for(auto organism:population->population){
        evaluateSolo(organism,analyze,visualize);
    }
    //we might need to flip the temps
    if(analyze){
        return;
    } else {
        switch(Parameters::getInt(std::string("NoneDeceptiveMazeWorld-experimentMode"), 0)){
            case 0: // const temp A const maze A
                currentMaze=0;
                currentTemp=0;
                break;
            case 1: // const temp B const maze B
                currentMaze=1;
                currentTemp=1;
                break;
            case 2: // rand temp const maze A
                if(Random::getFloat()<0.5){
                    currentTemp=0;
                } else {
                    currentTemp=1;
                }
                currentMaze=0;
                break;
            case 3: // rand temp const maze B
                if(Random::getFloat()<0.5){
                    currentTemp=0;
                } else {
                    currentTemp=1;
                }
                currentMaze=1;
                break;
            case 4: // rand temp rand maze
                if(Random::getFloat()<0.5){
                    currentTemp=0;
                } else {
                    currentTemp=1;
                }
                if(Random::getFloat()<0.5){
                    currentMaze=0;
                } else {
                    currentMaze=1;
                }
                break;
            case 5: // rand temp predcit maze (temp A = maze A)
                if(Random::getFloat()<0.5){
                    currentMaze=0;
                    currentTemp=0;
                } else {
                    currentMaze=1;
                    currentTemp=1;
                }
                break;
        }
        //std::cout<<currentMaze<<" "<<currentTemp<<std::endl;
        Parameters::setParameter(std::string("NeuroEvoDevoBrain-temperature"), temperatures[currentTemp]);
        Parameters::setParameter(std::string("NeuroEvoDevoBrain-maze"), (double)currentMaze);
    }
}

void NoneDeceptiveMazeWorld::evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize){
    auto brain = organism->brain;
    brain->resetBrain();
    double x=startX;
    double y=startY;
    double dir=PI;
    double visionRange=0.4;
    std::vector<double> trackX,trackY;
    double score=0.0;
    //std::cout<<currentMaze<<std::endl;
    for(int t=0;t<250;t++){
        auto I=maze[currentMaze]->rayCastDistances(x, y, dir, visionRange, std::vector<double>({-PI*0.5,-PI*0.25,0.0,PI*0.25,PI*0.5,PI}));
        for(int i=0;i<6;i++)
            brain->setInput(i, I[i]);
        brain->update();
        double forward=brain->getOutput(0);
        double rotation=brain->getOutput(1);
        double centerDist=I[2];
        double backDist=I[5];
        forward*=0.05;
        if(forward>0.0){
            if (forward>(visionRange*centerDist))
                forward=0.0;
        } else{
            if (-forward>(visionRange*backDist))
                forward=0.0;
        }
        if(fabs(forward)<0.01)
            forward=0.0;
        x=x+(cos(dir)*(forward*0.95));
        y=y+(sin(dir)*(forward*0.95));
        dir+=rotation*0.5;
        trackX.push_back(x);
        trackY.push_back(y);
        double d=maze[currentMaze]->getDistToTarget(x,y);
        score+=d;
    }
    //std::cout<<x<<" "<<y<<" "<<d<<std::endl;
    organism->dataCollector->setValue("score",score);
    organism->dataCollector->setValue("behavior",std::vector<double>({x,y}));
    organism->dataCollector->setValue("trackX",trackX);
    organism->dataCollector->setValue("trackY",trackY);
    organism->dataCollector->setValue("temp",Parameters::getDouble(std::string("NeuroEvoDevoBrain-temperature"),1.0));
    organism->dataCollector->setValue("maze",Parameters::getDouble(std::string("NeuroEvoDevoBrain-maze"),1.0));
    organism->brain->addBrainStatsToDataCollector(organism->dataCollector);
}

