//
//  HideAndSeekWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/28/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "HideAndSeekWorld.hpp"

#define _empty 0
#define _block 1
#define _ramp 2
#define _locked 4
#define _seeker 8
#define _hider 16

int xm[4]={0,1,0,-1};
int ym[4]={-1,0,1,0};

int maze[20][20]={
    {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
    {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
    {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
    {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
    {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
    {6,6,6,6,6,0,0,0,0,6,0,0,0,0,0,6,6,6,6,6},
    {6,6,6,6,6,0,0,0,0,6,0,0,0,0,0,6,6,6,6,6},
    {6,6,6,6,6,0,0,0,0,6,0,0,0,0,0,6,6,6,6,6},
    {6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6},
    {6,6,6,6,6,0,0,0,0,6,0,0,0,0,0,6,6,6,6,6},
    {6,6,6,6,6,0,0,0,0,6,6,0,6,6,6,6,6,6,6,6},
    {6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6},
    {6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6},
    {6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6},
    {6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6},
    {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
    {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
    {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
    {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
    {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6}
};


void HideAndSeekWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize){
    std::cout<<"this is not implemented"<<std::endl;
}

void HideAndSeekWorld::evaluate(std::shared_ptr<Population> hiders,std::shared_ptr<Population> seekers,bool analyze,bool visualize){
    std::vector<std::shared_ptr<Organism>> team;
    std::vector<int> order;
    for(int i=0;i<hiders->population.size();i++)
        order.push_back(i);
    for(int rep=0;rep<5;rep++){
        for(int i=0;i<hiders->population.size();i++){
            int a=Random::getInt(0, (int)hiders->population.size()-1);
            int b=Random::getInt(0, (int)hiders->population.size()-1);
            int dummy=order[a];
            order[a]=order[b];
            order[b]=dummy;
        }
        for(int i=0;i<hiders->population.size();i+=2){
            team.clear();
            team.push_back(hiders->population[order[i]]);
            team.push_back(hiders->population[order[i+1]]);
            team.push_back(seekers->population[order[i]]);
            team.push_back(seekers->population[order[i+1]]);
            /*
            for(int j=0;j<4;j++)
                team[j]->dataCollector->addValue("score",(double)Random::getFloat(0, 100));
             */
            evaluateGroup(team, false,false);
        }
    }
}

void HideAndSeekWorld::evaluateGroup(std::vector<std::shared_ptr<Organism>> group,bool analyze,bool visualize){
    std::shared_ptr<Area> area=std::make_shared<Area>(20,20);
    std::shared_ptr<Area> whoArea=std::make_shared<Area>(20,20);
    std::vector<int> xPos,yPos,dir,action;
    std::vector<double> score;
    int executionOrder[4]={0,1,2,3};
    int ID[4]={_hider,_hider,_seeker,_seeker};
    int runTime=200;
    
    for(int i=0;i<20;i++)
        for(int j=0;j<20;j++){
            area->set(i,j,maze[i][j]);
            whoArea->set(i,j,-1);
        }
    std::vector<int> tilesToAdd=std::vector<int>({_block,_block,_ramp,_seeker,_seeker,_hider,_hider});
    for(auto tile:tilesToAdd){
        int x,y;
        do{
            if(tile!=_seeker)
                y=Random::getInt(5,14);
            else
                y=Random::getInt(5,9);
            x=Random::getInt(5,14);
        }while(!area->isSurroundedBy(x,y,_empty)||area->get(x,y)!=_empty);
        area->set(x,y,tile);
        switch(tile){
            case _seeker:
                area->set(x,y,area->get(x,y)+_locked);
            case _hider:
                whoArea->set(x,y,(int)dir.size());
                xPos.push_back(x);
                yPos.push_back(y);
                dir.push_back(Random::getInt(0,3));
                action.push_back(0);
                score.push_back(0.0);
                break;
        }
    }
    
    for(int t=0;t<runTime;t++){
        //for(int i=0;i<4;i++){
        //    std::cout<<xPos[i]<<"/"<<yPos[i]<<" ";
        //}
        //std::cout<<std::endl;
        //area->show();
        //create all inputs
        for(int who=0;who<4;who++){
            //first bit -> seeker or hider
            group[who]->brain->setInput(0, (who>>1)&1);
            int frontTile=area->get(xPos[who]+xm[dir[who]],yPos[who]+ym[dir[who]]);
            for(int i=0;i<5;i++)
                group[who]->brain->setInput(i+1, (frontTile>>i)&1);
            for(int i=2;i<5;i++){
                int visionTile=area->getFromDirDist(xPos[who], yPos[who],dir[who],i);
                if(visionTile!=_empty){
                    group[who]->brain->setInput(i+5,1);
                    /*
                    if((ID[who]==_seeker)&&(visionTile==_hider)){
                        score[who]++;
                        score[whoArea->getFromDirDist(xPos[who], yPos[who],dir[who],i)]++;
                    }
                     */
                } else{
                    group[who]->brain->setInput(i+5,0);
                }
            }
            if(ID[who]==_hider){
                for(int other=2;other<4;other++)
                    if(area->lineOfSight(xPos[other], yPos[other], xPos[who], yPos[who], _empty)){
                        score[who]++;
                        score[other]++;
                    }
            }
            group[who]->brain->update();
            action[who]=bit(group[who]->brain->getOutput(0))+(bit(group[who]->brain->getOutput(1))<<1)+(bit(group[who]->brain->getOutput(2))<<2);
            action[who]=Random::getInt(0,7);
        }
        /*
        for(int i=0;i<4;i++){
            std::cout<<xPos[i]<<"-"<<yPos[i]<<std::endl;
        }
         */
        //area->lineOfSight(xPos[2], yPos[2], xPos[1], yPos[1], _empty);
        //shuffle execition order
        for(int who=0;who<4;who++){
            int a=Random::getInt(0,3);
            int b=Random::getInt(0,3);
            int d=executionOrder[a];
            executionOrder[a]=executionOrder[b];
            executionOrder[b]=d;
        }
        //*
        //execute actions
        for(auto who:executionOrder){
            int whatAmI=area->get(xPos[who], yPos[who]);
            whoArea->set(xPos[who],yPos[who],-1);
            area->set(xPos[who],yPos[who],_empty);
            switch(action[who]){
                case 0:
                    break;
                case 1:
                    dir[who]=(dir[who]+1)&3;
                    break;
                case 2:
                    dir[who]=(dir[who]-1)&3;
                    break;
                case 3:{
                    int target=area->getFromDirDist(xPos[who], yPos[who], dir[who], 1);
                    switch(target){
                        case _empty:
                            xPos[who]+=xm[dir[who]];
                            yPos[who]+=ym[dir[who]];
                            break;
                        case _block:
                            if(area->getFromDirDist(xPos[who], yPos[who], dir[who], 2)==_empty){
                                area->setFromDirDist(xPos[who], yPos[who], dir[who], 1,_block);
                                xPos[who]+=xm[dir[who]];
                                yPos[who]+=ym[dir[who]];
                            }
                            break;
                        case _ramp:
                            if(area->getFromDirDist(xPos[who], yPos[who], dir[who], 2)==_empty){
                                area->setFromDirDist(xPos[who], yPos[who], dir[who], 1,_ramp);
                                xPos[who]+=xm[dir[who]];
                                yPos[who]+=ym[dir[who]];
                            }
                            break;
                    }
                }
                    break;
                case 7:
                if(whatAmI==_hider)
                {
                    int target=area->getFromDirDist(xPos[who], yPos[who], dir[who],1);
                    switch(target){
                        case _block:
                            area->setFromDirDist(xPos[who], yPos[who], dir[who],1,_block+_locked);
                            break;
                        case _block+_locked:
                            area->setFromDirDist(xPos[who], yPos[who], dir[who],1,_block);
                            break;
                        case _ramp:
                            area->setFromDirDist(xPos[who], yPos[who], dir[who],1,_ramp+_locked);
                            break;
                        case _ramp+_locked:
                            area->setFromDirDist(xPos[who], yPos[who], dir[who],1,_ramp);
                            break;
                    }
                } else {
                    //jumping
                    //*
                    int target=area->getFromDirDist(xPos[who],yPos[who],dir[who],1);
                    if((target==_ramp)||(target==_ramp+_locked)){
                        int superTarget=area->getFromDirDist(xPos[who],yPos[who],dir[who],3);
                        if(superTarget==_empty){
                            xPos[who]+=xm[dir[who]]*3;
                            yPos[who]+=ym[dir[who]]*3;
                        }
                    }
                     //*/
                }
                    break;
            }
            whoArea->set(xPos[who],yPos[who],who);
            area->set(xPos[who],yPos[who],whatAmI);
        }
         //*/
        area->tick();
    }
     //*
    for(int i=0;i<4;i++){
        if(ID[i]==_seeker)
            group[i]->dataCollector->addValue("score", (double)score[i]);
        else
            group[i]->dataCollector->addValue("score",(double)(2*runTime)-(double)score[i]);
    }
    //if(analyze)
        area->save(Parameters::getString("HideAndSeekWorld-recordName", "record.csv"));
    //exit(0);
      //*/
    /*
    for(auto R:area){
        for(auto E:R)
            std::cout<<E;
        std::cout<<std::endl;
    }
    exit(0);
    //*/
    //if((group[0]->ID%100000)==0)
    //    area->save("record"+std::to_string(group[0]->ID)+".csv");
    //area->reset();
}

