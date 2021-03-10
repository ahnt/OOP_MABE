//
//  BlockyCatchWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "BlockyCatchWorld.hpp"

int BlockyCatchWorld::runSingleDrop(std::shared_ptr<Brain> brain,int startLocation, int worldWidth, int tileWidth, int tileDirection, int dropLength,bool analyze,bool visualize){
    std::vector<int> world;
    int botLocation=0;
    int tileLocation=startLocation;
    int ret=0;
    world.resize(worldWidth);
    for(int t=0;t<dropLength;t++){
        world.assign(worldWidth, 0.0);
        for(int w=0;w<tileWidth;w++)
            world[(tileLocation+w)%worldWidth]=1;
        brain->setInput(0, world[(botLocation)]);
        brain->setInput(1, world[(botLocation+1)%worldWidth]);
        brain->setInput(2, world[(botLocation+4)%worldWidth]);
        brain->setInput(3, world[(botLocation+5)%worldWidth]);
        if(analyze){
            std::cout<< world[(botLocation)]<<","
            << world[(botLocation+1)%worldWidth]<<","
            << world[(botLocation+4)%worldWidth]<<","
            << world[(botLocation+5)%worldWidth]<<",";
        }
        brain->update();
        int output=bit(brain->getOutput(0))+(bit(brain->getOutput(1))<<1);
        int action=0;
        switch(output){
            case 0:
            case 3:
                action=0;
                break;
            case 1:
                action=1;
                botLocation=(botLocation+1+worldWidth)%worldWidth;
                break;
            case 2:
                action=-1;
                botLocation=(botLocation-1+worldWidth)%worldWidth;
                break;
        }
        if(analyze){
            std::cout<<action<<std::endl;
        }
        tileLocation=(tileLocation+tileDirection+worldWidth)%worldWidth;
    }
    ret=0;
    for(int w=0;w<6;w++)
        ret|=world[(botLocation+w+worldWidth)%worldWidth];
    return ret;
}

void BlockyCatchWorld::evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize){
    auto brain = organism->brain;
    brain->addBrainStatsToDataCollector(organism->dataCollector);
    int correct=0,incorrect=0;
    std::vector<int> tiles=std::vector<int>({2,4});
    std::vector<int> directions=std::vector<int>({-1,1});
    std::vector<double> behavior;
    int width=16;
    int height=32;
    hitOrMiss.clear();
    for(auto tile:tiles){
        for(auto dir:directions){
            for(int startLocation=0;startLocation<width;startLocation++){
                brain->resetBrain();
                int res=runSingleDrop(brain,startLocation, width, tile, dir, height,analyze,visualize);
                behavior.push_back(res);
                switch(res){
                    case 0: //miss
                        if(tile==2){
                            incorrect++;
                            for(int hom=0;hom<height;hom++)
                                hitOrMiss.push_back(0.0);
                        }
                        else{
                            correct++;
                            for(int hom=0;hom<height;hom++)
                                hitOrMiss.push_back(1.0);
                        }
                        break;
                    case 1: //catch
                        if(tile==2){
                            correct++;
                            for(int hom=0;hom<height;hom++)
                                hitOrMiss.push_back(1.0);
                        }
                        else{
                            incorrect++;
                            for(int hom=0;hom<height;hom++)
                                hitOrMiss.push_back(0.0);
                        }
                        break;
                }
            }
        }
    }
    //organism->dataCollector->setValue("score", pow(1.05,correct));
    //organism->dataCollector->setValue("score", pow(1.05,correct-incorrect));
    organism->dataCollector->setValue("score", (double)correct);
    organism->dataCollector->setValue("behavior",behavior);

    /*
    if(correct==64){
        printf("N,W0,W1,W2,W3,W4,W5,W6,W7,W8,W9,W10,W11,W12,W13,W14,W15,I0,I1,I2,I3,A\n");
        for(int i=0;i<worldStateRecorder.size();i++){
            printf("%i",i);
            for(int j=0;j<16;j++)
                printf(",%i",worldStateRecorder[i][j]);
            for(int j=0;j<4;j++)
                printf(",%i",sensorStateRecorder[i][j]);
            for(int j=0;j<1;j++)
                printf(",%i",actionStateRecorder[i][j]);
            printf("\n");
        }
        exit(0);
    }
     */
    // FILL in the rest ...
}
