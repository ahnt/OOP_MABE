//
//  CarWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 10/16/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "CarWorld.hpp"
const int _empty=0;
const int _car=1;
const int _wall=2;
const int _stoppedCar=3;

const int maxDist=99;
const int viewX[4][13]={
    {-1, 0, 1,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2},
    { 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
    { 1, 0,-1, 2, 1, 0,-1,-2, 2, 1, 0,-1,-2},
    {-3,-3,-3,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1}
};
const int viewY[4][13]={
    {-3,-3,-3,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1},
    {-1, 0, 1,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2},
    { 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
    { 1, 0,-1, 2, 1, 0,-1,-2, 2, 1, 0, 1, 2}
};

CarWorld::CarWorld(){
    mode=Parameters::getInt("CarWorld-mode", 4);
    areas.clear();
    switch(mode){
        case 0:
            areas.push_back(makeCarWorld(8));
            areas.push_back(makeCarWorld(1));
            //areas[1]->set(6,6,_stoppedCar);
            //areas[1]->setAsStartConfig();
            break;
        case 1:
        case 2:
        case 3:
            areas.push_back(makeCarWorld(3));
            break;
        case 4:
            areas.push_back(makeCarWorld(8));
            areas.push_back(makeCarWorld(2));
            break;
    }
    Parameters::setParameter("LODManager-parametersToSave",std::string("score,targetsReached,totalAlive,totalReached,forwards,stops,longestWait,forwardsList,stopsList,longestWaitList,lexi_0_forwards,lexi_0_longestWait,lexi_0_score,lexi_0_stops,lexi_0_targetsReached,lexi_0_ticksAlive,lexi_0_crashes,lexi_4_forwards,lexi_4_longestWait,lexi_4_score,lexi_4_stops,lexi_4_targetsReached,lexi_4_ticksAlive,lexi_4_crashes"));
}

void CarWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize){
    if(analyze){
        moves=areas[0]->get2Dmatrix(0);
        stands=areas[0]->get2Dmatrix(0);
        for(int rep=0;rep<1;rep++)
            evaluatePop(population,analyze,visualize,areas[0],-1);
        Area::save2Dmatrix("moves.txt", moves);
        Area::save2Dmatrix("stands.txt", stands);
        if(visualize){
            areas[0]->save("pop.csv");
        }
        moves=areas[1]->get2Dmatrix(0);
        stands=areas[1]->get2Dmatrix(0);
        std::shared_ptr<Population> dummyPop=std::make_shared<Population>();
        dummyPop->population.resize(10);
        dummyPop->population[0]=population->population[0];
        for(int m=1;m<10;m++)
            dummyPop->population[m]=population->population[0]->clone();
        evaluatePop(dummyPop,analyze,visualize,areas[1],-1);
        Area::save2Dmatrix("indmoves.txt", moves);
        Area::save2Dmatrix("indstands.txt", stands);
        areas[1]->save("individual.csv");
    } else
    switch(mode){
        case 0:{
            //evaluate only on the large world and do a compositional fitness
            double groupLevelSelection=Parameters::getDouble("CarWorld-GLS", 0.1);
            std::vector<double> score,individualScore;
            for(int reps=0;reps<1;reps++)
                evaluatePop(population,analyze,visualize,areas[0],reps);
            double totalScore=0.0;
            for(auto org:population->population){
                double localScore=org->dataCollector->getDouble("localScore");
                score.push_back(localScore);
                totalScore+=localScore;
            }
            if(visualize)
                areas[0]->save("pop.csv");
            std::shared_ptr<Population> dummyPop=std::make_shared<Population>();
            dummyPop->population.resize(10);
            for(int n=0;n<(int)population->population.size();n++){
                dummyPop->population[0]=population->population[n];
                for(int m=1;m<10;m++)
                    dummyPop->population[m]=population->population[n]->clone();
                evaluatePop(dummyPop,analyze,visualize,areas[1],4);
                double localScore=dummyPop->population[0]->dataCollector->getDouble("localScore");
                individualScore.push_back(localScore);
            }
            for(int n=0;n<(int)population->population.size();n++){
                population->population[n]->dataCollector->setValue("score",groupLevelSelection*(totalScore/100.0)+(1.0-groupLevelSelection)*score[n]+individualScore[n]);
            }
            if(visualize)
                areas[1]->save("individual.csv");
        }
            break;
        case 4:{ //evaluate single and group -> sum it is
            double groupLevelSelection=Parameters::getDouble("CarWorld-GLS", 0.9);
            std::vector<double> score;
            evaluatePop(population,analyze,visualize,areas[0],0);
            double totalScore=0.0;
            for(auto org:population->population){
                double localScore=org->dataCollector->getDouble("localScore");
                score.push_back(localScore);
                totalScore+=localScore;
            }
            if(visualize)
                areas[0]->save("pop.csv");
            std::shared_ptr<Brain> dummyBrain=AutoCarBrain::getTemplate();
            std::shared_ptr<Population> dummyPop=std::make_shared<Population>();
            dummyPop->population.resize(1);
            for(int n=0;n<(int)population->population.size();n++){
                dummyPop->population[0]=population->population[n];
                //for(int m=1;m<dummyPop->population.size();m++)
                //    dummyPop->population[m]=population->population[n]->brainOnlyClone();
                if(n==0)
                    evaluatePop(dummyPop,analyze,true,areas[1],0);
                else
                    evaluatePop(dummyPop,analyze,false,areas[1],0);
                double localScore=population->population[n]->dataCollector->getDouble("localScore");
                //population->population[n]->dataCollector->setValue("score",groupLevelSelection*score[n]+(1.0-groupLevelSelection)*localScore);
                population->population[n]->dataCollector->setValue("score",groupLevelSelection*((double)totalScore/(double)population->population.size())+(1.0-groupLevelSelection)*localScore);
                //for(int m=1;m<dummyPop->population.size();m++)
                //    dummyPop->population[m]=nullptr;
            }
            if(visualize)
                areas[1]->save("individual.csv");

        }
            break;
    }
}

               
void CarWorld::evaluatePop(std::shared_ptr<Population> population,bool analyze,bool visualize,std::shared_ptr<Area> area,int replicate){
    area->resetToStartConfig();
    std::vector<int> xPos,yPos,dir,target,alive,action,order,furthest,reached,stops,forwards,ticksAlive,currentWait,longestWait,crashes;
    std::vector<double> score;
    std::vector<std::set<std::pair<int,int>>> visitedPlaces;
    int totalTicks=1000;
    
    whoMap.resize(area->get_xDim());
    for(int i=0;i<area->get_xDim();i++)
        whoMap[i].resize(area->get_yDim());

    visitedPlaces.resize((int)population->population.size());
    for(int n=0;n<(int)population->population.size();n++){
        int x,y,t;
        do{
            x=Random::getInt(3,area->get_xDim()-3);
            y=Random::getInt(3,area->get_yDim()-3);
            t=Random::getInt(0,(int)area->distTo.size()-1);
        }while((area->get(x,y)!=_empty));//||(distTo[t][x][y]<distTo.size()));
        area->set(x,y,_stoppedCar);
        whoMap[x][y]=n;
        xPos.push_back(x);
        yPos.push_back(y);
        dir.push_back(Random::getInt(0,3));
        target.push_back(t);
        alive.push_back(true);
        action.push_back(0);
        order.push_back(n);
        score.push_back(0.0);
        furthest.push_back(maxDist);
        reached.push_back(0);
        stops.push_back(0);
        forwards.push_back(0);
        ticksAlive.push_back(totalTicks);
        currentWait.push_back(0);
        longestWait.push_back(0);
        crashes.push_back(0);
    }
    /*
    if(population->population.size()<5){
        for(int i=0;i<5;i++){
            int x,y;
            do{
                x=Random::getInt(7,area->get_xDim()-6);
                y=Random::getInt(7,area->get_yDim()-6);
            }while(area->get(x,y)!=_empty);
            if((Random::getInt()&1)==0)
                area->set(x,y,_stoppedCar);
            else
                area->set(x,y,_car);
            whoMap[x][y]=0;
        }
    }
     //*/
    for(int n=0;n<(int)population->population.size();n++)
        population->population[n]->brain->resetBrain();
    for(int t=0;t<totalTicks;t++){
        area->tick();
        //do all sensor updates and brain updates
        for(int n=0;n<(int)population->population.size();n++){
            auto brain=population->population[n]->brain;
            for(int v=0;v<13;v++){
                int value=area->get(xPos[n]+viewX[dir[n]][v],yPos[n]+viewY[dir[n]][v])&3;
                brain->setInput((v*2), value&1);
                brain->setInput((v*2)+1,(value>>1)&1);
            }
            for(int d=-1;d<2;d++){
                if(area->distTo[target[n]][xPos[n]+xm[(dir[n]+d)&3]][yPos[n]+ym[(dir[n]+d)&3]]<area->distTo[target[n]][xPos[n]][yPos[n]]){
                    brain->setInput(26+1+d,1);
                } else {
                    brain->setInput(26+1+d,0);
                }
            }
            brain->update();
            action[n]=bit(brain->getOutput(0))+(bit(brain->getOutput(1))<<1);
            //action[n]=Random::getInt(0,3);
            //keep shuffling the execution order
            int dummy=order[n];
            int who=Random::getInt(0,(int)order.size()-1);
            order[n]=order[who];
            order[who]=dummy;
        }
        //got all actions, let's resolve
        for(auto n:order){
            if(alive[n]){
                area->set(xPos[n],yPos[n],0);
                std::pair<int,int> sP=std::pair<int,int>({xPos[n],yPos[n]});
                //if((visitedPlaces[n].count(sP)==0)&&(furthest[n]>distTo[target[n]][xPos[n]][yPos[n]])){
                if(visitedPlaces[n].count(sP)==0){
                    score[n]+=1.0/(double)(area->distTo[target[n]][xPos[n]][yPos[n]]+1);
                    visitedPlaces[n].insert(sP);
                    furthest[n]=area->distTo[target[n]][xPos[n]][yPos[n]];
                } else {
                    score[n]+=0.0001;
                }
                switch(action[n]){
                    case 0:
                        area->set(xPos[n],yPos[n],_stoppedCar+(target[n]<<2));
                        if(analyze)
                            stands[xPos[n]][yPos[n]]++;
                        stops[n]++;
                        currentWait[n]++;
                        break;
                    case 1:
                        dir[n]=(dir[n]+1)&3;
                        area->set(xPos[n],yPos[n],_car+(target[n]<<2));
                        currentWait[n]++;
                        break;
                    case 2:
                        dir[n]=(dir[n]-1)&3;
                        area->set(xPos[n],yPos[n],_car+(target[n]<<2));
                        currentWait[n]++;
                        break;
                    case 3:{
                        int forward=area->get(xPos[n]+xm[dir[n]],yPos[n]+ym[dir[n]])&3;
                        forwards[n]++;
                        switch(forward){
                            case _empty:
                                currentWait[n]=0;
                                xPos[n]+=xm[dir[n]];
                                yPos[n]+=ym[dir[n]];
                                if(analyze)
                                    moves[xPos[n]][yPos[n]]++;
                                area->set(xPos[n],yPos[n],_car+(target[n]<<2));
                                if(area->distTo[target[n]][xPos[n]][yPos[n]]==0){
                                    
                                    int old=target[n];
                                    do{
                                        target[n]=Random::getInt(0,(int)area->distTo.size()-1);
                                    }while((target[n]==old)||(area->distTo[target[n]][xPos[n]][yPos[n]]<area->distTo.size()));
                                    //target[n]=(target[n]+1)&31;
                                    score[n]+=10.0;
                                    visitedPlaces[n].clear();
                                    furthest[n]=maxDist;
                                    reached[n]++;
                                }
                                break;
                            case _car:
                                //alive[whoMap[xPos[n]+xm[dir[n]]][yPos[n]+ym[dir[n]]]]=false;
                                //alive[n]=false;
                                //ticksAlive[n]=t;
                                //area->set(xPos[n]+xm[dir[n]],yPos[n]+ym[dir[n]],_empty);
                                area->set(xPos[n],yPos[n],_car+(target[n]<<2));
                                crashes[n]++;
                                break;
                            case _wall:
                            case _stoppedCar:
                                //alive[n]=false;
                                //ticksAlive[n]=t;
                                area->set(xPos[n],yPos[n],_car+(target[n]<<2));
                                crashes[n]++;
                                break;
                        }
                            }
                        break;
                }
                if(currentWait[n]>longestWait[n])
                    longestWait[n]=currentWait[n];
                /*
                if(currentWait[n]>200){
                    alive[n]=false;
                    ticksAlive[n]=t;
                    area->set(xPos[n],yPos[n],_empty);
                }
                 */
            }
        }
    }
    double totalPayoff=0.0;
    int totalAlive=0;
    int totalReached=0;
    for(int p=0;p<score.size();p++){
        if(!alive[p])
            score[p]=0.0;
        else{
            totalAlive++;
            totalReached+=reached[p];
        }
        totalPayoff+=score[p];
    }
    totalPayoff/=(double)score.size();
    for(int n=0;n<score.size();n++){
        //population->population[n]->dataCollector->addValue("score",score[n]);
        //if(!alive[n])
        //    score[n]*=0.9;
        population->population[n]->dataCollector->setValue("localScore",score[n]);
        population->population[n]->dataCollector->appendValue("targetsReached",reached[n]);
        population->population[n]->dataCollector->addValue("targetsReachedFeature",reached[n]);
        population->population[n]->dataCollector->appendValue("totalAlive",totalAlive);
        population->population[n]->dataCollector->appendValue("totalReached",totalReached);
        population->population[n]->dataCollector->addValue("forwards",(int)forwards[n]);
        population->population[n]->dataCollector->addValue("stops",(int)stops[n]);
        population->population[n]->dataCollector->addValue("longestWait",(int)longestWait[n]);
        population->population[n]->dataCollector->appendValue("forwardsList",forwards[n]);
        population->population[n]->dataCollector->appendValue("stopsList",stops[n]);
        population->population[n]->dataCollector->appendValue("longestWaitList",longestWait[n]);
        population->population[n]->dataCollector->setValue("lexi_"+std::to_string(replicate)+"_score",(double)score[n]);
        population->population[n]->dataCollector->setValue("lexi_"+std::to_string(replicate)+"_targetsReached",(double)reached[n]);
        population->population[n]->dataCollector->setValue("lexi_"+std::to_string(replicate)+"_forwards",(double)forwards[n]);
        population->population[n]->dataCollector->setValue("lexi_"+std::to_string(replicate)+"_stops",(double)stops[n]);
        population->population[n]->dataCollector->setValue("lexi_"+std::to_string(replicate)+"_longestWait",(double)longestWait[n]);
        population->population[n]->dataCollector->setValue("lexi_"+std::to_string(replicate)+"_ticksAlive",(double)ticksAlive[n]);
        population->population[n]->dataCollector->setValue("lexi_"+std::to_string(replicate)+"_crashes",(double)crashes[n]);
    }
    if((visualize)){
        //area->save("testArea.csv");
        //exit(0);
    }
}

std::shared_ptr<Area> CarWorld::makeCarWorld(int size){
    std::shared_ptr<Area> area;
    std::vector<int> roads,startTiles;
    int theSize=3+3+3;
    for(int s=0;s<size;s++){
        theSize+=3+2;
        roads.push_back(6+(s*5));
        roads.push_back(6+(s*5)+1);
        startTiles.push_back(6+(s*5));
    }
    /*
    switch(size){
        case 1:
            theSize=3+3+2+3+3;
            roads=std::vector<int>({6,7});
            startTiles=std::vector<int>({6});
            break;
        case 8:
            theSize=51;
            roads=std::vector<int>({7,8,12,13,17,18,22,23,27,28,32,33,37,38,42,43});
            startTiles=std::vector<int>({7,12,17,22,27,32,37,42});
            break;
    }
     */
    area=std::make_shared<Area>(theSize,theSize,_wall);
    
    for(auto s:roads){
        for(int i=3;i<theSize-3;i++){
            area->set(s,i,_empty);
            area->set(i,s,_empty);
        }
    }
    /*
    for(auto j:std::vector<int>({24,25,26})){
        for(int i=3;i<22;i++){
            area->set(i,j,_wall);
            area->set(j,i,_wall);
        }
        for(int i=29;i<51;i++){
            area->set(i,j,_wall);
            area->set(j,i,_wall);
        }
    }
     */
    /*
    for(int i=9;i<22;i++)
        for(int j=9;j<22;j++){
            area->set(i,j,_wall);
            area->set(i+20,j,_wall);
            area->set(i,j+20,_wall);
            area->set(i+20,j+20,_wall);
        }
     */
    /*
    for(int i=7;i<43;i++)
        for(int j=7;j<43;j++)
            area->set(i,j,_empty);
     */
    int what=0;
    area->targetLocations.clear();
    for(auto s:startTiles){
        area->targetLocations.push_back(std::pair<int,int>({s,3}));
        area->set(s,1,_car+(what<<2));
        what++;
        area->targetLocations.push_back(std::pair<int,int>({3,s+1}));
        area->set(1,s+1,_car+(what<<2));
        what++;
        area->targetLocations.push_back(std::pair<int,int>({s+1,theSize-4}));
        area->set(s+1,theSize-2,_car+(what<<2));
        what++;
        area->targetLocations.push_back(std::pair<int,int>({theSize-4,s}));
        area->set(theSize-2,s,_car+(what<<2));
        what++;
    }
    for(auto loc:area->targetLocations){
        //std::cout<<loc.first<<","<<loc.second<<std::endl;
        std::vector<std::vector<int>> dist;
        dist.resize(theSize);
        for(int i=0;i<(int)dist.size();i++){
            dist[i].resize((int)dist.size());
            for(int j=0;j<(int)dist.size();j++)
                dist[i][j]=maxDist;
        }
        dist[loc.first][loc.second]=0;
        std::vector<std::pair<int,int>> currentLoc,nextLoc;
        currentLoc.push_back(loc);
        int currentDist=1;
        while(currentLoc.size()>0){
            nextLoc.clear();
            for(auto searchLoc:currentLoc){
                for(int i=0;i<4;i++){
                    int x=searchLoc.first+xm[i];
                    int y=searchLoc.second+ym[i];
                    if((area->get(x,y)==_empty)&&(dist[x][y]==maxDist)){
                        dist[x][y]=currentDist;
                        nextLoc.push_back(std::pair<int,int>({x,y}));
                    }
                }
            }
            currentDist++;
            currentLoc=nextLoc;
        }
        area->distTo.push_back(dist);
    }
    area->setAsStartConfig();
    //area->save("testArea.csv");
    return area;
}
