//
//  DeceptiveMazeWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "DeceptiveMazeWorld.hpp"

DeceptiveMazeWorld::DeceptiveMazeWorld(){
    startX=Parameters::getDouble("DeceptiveMazeWorld-startX",0.1);
    startY=Parameters::getDouble("DeceptiveMazeWorld-startY",0.1);
    targetX=Parameters::getDouble("DeceptiveMazeWorld-targetX",0.1);
    targetY=Parameters::getDouble("DeceptiveMazeWorld-targetY",0.9);
    maze=std::make_shared<Maze>(startX,
                              startY,
                              targetX,
                              targetY,
                              std::vector<Line2D>({
                                    Line2D(0,0,1,0),
                                    Line2D(1,0,1,1),
                                    Line2D(1,1,0,1),
                                    Line2D(0,1,0,0),
        //*
                                    Line2D(0.0,0.75,0.25,0.70),
                                    Line2D(0.25,0.70,0.65,0.8),
                                    Line2D(0.25,0.70,0.25,0.25),
                                    Line2D(0.0,0.60,0.15,0.4),
                                    Line2D(0.35,0.0,0.55,0.2),
                                    Line2D(0.25,0.5,0.8,0.2),
                                    Line2D(1.0,0.4,0.5,0.6)
         //*/
        /*
                                    Line2D(0.0,0.8,0.8,0.8),
                                    Line2D(0.8,0.8,0.8,0.2),
                                    Line2D(0.8,0.2,0.2,0.2),
                                    Line2D(0.2,0.2,0.2,0.6),
                                    Line2D(0.2,0.6,0.6,0.6),
                                    Line2D(0.6,0.6,0.6,0.4),
                                    Line2D(0.6,0.4,0.4,0.4)
        */
         })
                              );
    maze->computeDistMatrix(101, 101);    
    trackFileHandle.open(Parameters::getString("DeceptiveMazeWorld-tracksFileName","trackFile.csv"));
    trackFileHandle<<"ID,x,y"<<std::endl;

    
    Parameters::setParameter("LODManager-parametersToSave",std::string("score,behavior,trackX,trackY"));
    //Parameters::setParameter("BioGenome-perSitePointMutationRate", 0.001);
    Parameters::setParameter(std::string("BioGenome-seedStartCodonsPerGene"),0);
}

void DeceptiveMazeWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) {
    double minDist=10000.0;
    std::vector<double> minX,minY;
    for(auto organism:population->population){
        evaluateSolo(organism,analyze,visualize);
        std::vector<double> X=organism->dataCollector->getDoubleVector("trackX");
        std::vector<double> Y=organism->dataCollector->getDoubleVector("trackY");
        for(int i=0;i<X.size();i++){
            double lx=X[i];
            double ly=Y[i];
            double currentDist=Point::dist(lx,ly,targetX,targetY);
            if(currentDist<minDist){
                minX=X;
                minY=Y;
                minDist=currentDist;
            }
        }
    }
    trackFileHandle<<Utilities::currentGeneration<<
                    ",\"["<<serializeVectorToString(minX,std::string(","))<<"]\",\"["
                        <<serializeVectorToString(minY,std::string(","))<<"]\""<<std::endl;
    trackFileHandle.flush();
}

void DeceptiveMazeWorld::evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize){
    auto brain = organism->brain;
    brain->resetBrain();
    double x=startX;
    double y=startY;
    double dir=PI;
    double visionRange=0.4;
    std::vector<double> trackX,trackY;
    for(int t=0;t<250;t++){
        auto I=maze->rayCastDistances(x, y, dir, visionRange, std::vector<double>({-PI*0.5,-PI*0.25,0.0,PI*0.25,PI*0.5,PI}));
        for(int i=0;i<6;i++)
            brain->setInput(i, I[i]);
        brain->update();
        double forward=brain->getOutput(0);
        double rotation=brain->getOutput(1);
        double centerDist=I[2];
        double backDist=I[5];
        forward*=0.1;
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
        dir+=rotation;
        trackX.push_back(x);
        trackY.push_back(y);
        //std::cout<<x<<","<<y<<std::endl;
        //if((x>1.0)||(x<0.0)||(y<0.0)||(y>1.0)){
        //    int a=5;
        //exit(0);

        //}
    }
    double d=Point::dist(x,y,targetX,targetY);
    //std::cout<<x<<" "<<y<<" "<<d<<std::endl;
    organism->dataCollector->setValue("score",1.5-d);
    organism->dataCollector->setValue("behavior",std::vector<double>({x,y}));
    organism->dataCollector->setValue("trackX",trackX);
    organism->dataCollector->setValue("trackY",trackY);
}
