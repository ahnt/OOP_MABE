//
//  ExampleWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "OFAAFOWorld.hpp"
#include <numeric>

//four tile types defined
#define _empty 0
#define _gras 1
#define _agent 2
#define _wall 3

OFAAFOWorld::OFAAFOWorld(){
    Parameters::setParameter("LODManager-parametersToSave",std::string("score,rawScores,ownScore"));
    metrics=std::vector<std::string>({"scoreIndividual","scoreGroup","rank","score0","score1","score2","score3","beepSent","beepReceived","turnsLeft","turnsRight",
        "didNothing","movedForward","pickedGras","droppedGras","handedGrasOver","sawNothing",
        "sawGras","sawAgent","sawWall"});
}
void OFAAFOWorld::initializeWorld(){
    //initialize the area container to size 16x16 and contain only grass
    area=std::make_shared<Area>(16,16,_gras);
    //adding all walls
    for(int i=0;i<16;i++){
        area->set(0,i,_wall);
        area->set(15,i,_wall);
        area->set(i,0,_wall);
        area->set(i,15,_wall);
    }
    //every generation we create a different map
    for(int i=0;i<50;i++){
        area->set(Random::getInt(1, 14),Random::getInt(1, 14),_empty);
    }
    area->setAsStartConfig(); // we mark this state as the start configuration, makes resetting easier later
}

void OFAAFOWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize){
    initializeWorld();
    
    //we want two basic evaluation conditions, one where we clone each organism four times, on where we pick four
    //organisms randomly from the population (as the organisms are already "unordered" wen can pick four at a time
    //this will be dependent on a commandline parameter
    std::vector<std::shared_ptr<Organism>> organisms;
    int popSize=(int)population->population.size();
    if(analyze){
        auto organism=population->population[population->population.size()-1];
        organisms.push_back(organism);
        for(int i=0;i<3;i++)
            organisms.push_back(organism->brainOnlyClone());
        /*regular testing and visualization*/
        auto scores=evaluateGroup(organisms,analyze,visualize,true);
        /*only testing for beeps*/
        /*
        std::ofstream rFile;
        rFile.open(Parameters::getString("OFAAFO-beepFileName", "beep.csv"));
        rFile<<"replicate,m0,m1,m2,m3,b0,b1,b2,b3"<<std::endl;
        for(int r=0;r<100;r++){
            rFile<<r;
            for(int j=0;j<2;j++){
                auto scores=evaluateGroup(organisms,analyze,visualize,(bool)j);
                rFile<<","<<scores[0]<<","<<scores[1]<<","<<scores[2]<<","<<scores[3];
            }
            rFile<<std::endl;
        }
        rFile.close();
         */
    } else {
        switch(Parameters::getInt("OFAAFOWorld-groupMode", 0)){
            case 0: // clone mode
                for(auto organism:population->population){
                    organisms.clear();
                    organisms.push_back(organism);
                    for(int i=0;i<3;i++)
                        organisms.push_back(organism->brainOnlyClone());
                    auto scores=evaluateGroup(organisms,analyze,visualize,true);
                    
                    for(auto s:scores)
                        organism->dataCollector->appendValue("rawScores", s);
                    //we have different reward modes
                    switch( Parameters::getInt("OFAAFOWorld-rewardMode", 0) ){
                        case 0: // individual reward
                            break;
                        case 1: // every member gets the average payoff
                            scores.assign(scores.size(),std::accumulate(scores.begin(),scores.end(),0.0)/(double)scores.size());
                            break;
                        case 2: // every member gets the max of the group (all for one)
                            scores.assign(scores.size(),*std::max_element(scores.begin(),scores.end()));
                            break;
                        case 3: // every member gets the min of the group (one for all)
                            scores.assign(scores.size(),*std::min_element(scores.begin(),scores.end()));
                            break;
                    }

                    
                    auto finalScore=(double)std::accumulate(scores.begin(), scores.end(), 0.0)/(double)scores.size();
                    organism->dataCollector->setValue("score", finalScore);
                    organism->dataCollector->setValue("ownScore",finalScore);

                }
                break;
            case 1: // random sample mode
                for(int i=0;i<popSize;i+=4){
                    organisms.clear();
                    for(int j=0;j<4;j++)
                        organisms.push_back(population->population[(i+j)%popSize]);
                    auto scores=evaluateGroup(organisms,analyze,visualize,true);
                    for(int o=0;o<4;o++){
                        for(int s=0;s<4;s++){
                            organisms[o]->dataCollector->appendValue("rawScores", scores[s]);
                        }
                        organisms[o]->dataCollector->setValue("ownScore",scores[o]);
                    }
                    //we have different reward modes
                    switch( Parameters::getInt("OFAAFOWorld-rewardMode", 3) ){
                        case 0: // individual reward
                            break;
                        case 1: // every member gets the average payoff
                            scores.assign(scores.size(),std::accumulate(scores.begin(),scores.end(),0.0)/(double)scores.size());
                            break;
                        case 2: // every member gets the max of the group (all for one)
                            scores.assign(scores.size(),*std::max_element(scores.begin(),scores.end()));
                            break;
                        case 3: // every member gets the min of the group (one for all)
                            scores.assign(scores.size(),*std::min_element(scores.begin(),scores.end()));
                            break;
                    }
                    for(int j=0;j<(int)scores.size();j++)
                        population->population[(i+j)%popSize]->dataCollector->addValue("score",scores[j]);
                }
                break;
            case 2: // we randomly sample, but repeat that a couple of times
                // basically, instead of picking four in each generation and evaluating them
                // we repeat this process several times, so that each organism got, say, 4 evaluations
                // each time with a different group
                // the reason  behind this is, that we want to avoid that agents get lucky or are in groups
                // that don't really work, which could happen due to mutations
                break;
        }
    }
    //we save the last' organisms behavior.
    //area->save("areaRecord.csv");
    //exit(0);
}


//Key function to implement the evaluation of a single organism
std::vector<double> OFAAFOWorld::evaluateGroup(std::vector<std::shared_ptr<Organism>> organisms,bool analyze,bool visualize,bool allowBeeping){
    std::vector<double> scores;
    std::vector<std::shared_ptr<Brain>> brains;
    std::vector<int> xPos,yPos,dir,act;              // define three variables defining the position and facing direction of the agent
    std::shared_ptr<Area> who;
    std::ofstream rFile;
    if(analyze){
        rFile.open(Parameters::getString("OFAAFO-locFileNameLead", "default.csv"));
        rFile<<"t,x0,y0,d0,e0,a0,b0,x1,y1,d1,e1,a1,b1,x2,y2,d2,e2,a2,b2,x3,y3,d3,e3,a3,b3"<<std::endl;
    }
    
    for(auto s:metrics){
        for(int i=0;i<4;i++){
            actionLog[i][s]=0;
        }
    }
    
    who=std::make_shared<Area>(16,16,-1);
    std::vector<int> order({0,1,2,3});
    scores.assign(4,0.0);
    xPos.assign(4,0);
    yPos.assign(4,0);
    dir.assign(4,0);
    act.assign(4,0);
    brains.clear();
    for(auto org:organisms){
        org->brain->resetBrain();
        brains.push_back(org->brain);
    }
    area->resetToStartConfig();
    //we position each agent in a corner
    xPos[0]= 1; yPos[0]= 1; dir[0]= 2;
    xPos[1]=14; yPos[1]= 1; dir[1]= 3;
    xPos[2]=14; yPos[2]=14; dir[2]= 0;
    xPos[3]= 1; yPos[3]=14; dir[3]= 1;

    for(int i=0;i<4;i++){
        area->set(xPos[i],yPos[i],_agent);    //marking the start location of the agent
        who->set(xPos[i],yPos[i],i);         //we use the who array to kee track of which agent is where, after all they are going to give each other resources
    }
    //looping over all agents for the time they are allowed in the area
    double beepsNext[4],beepsNow[4]={0.0,0.0,0.0,0.0};
    for(int t=0;t<500;t++){
        std::random_shuffle ( order.begin(), order.end() );
        for(auto o:order){
            int inFront=area->getFromDirDist(xPos[o], yPos[o], dir[o], 1);   //this gives you the tile in front of the  agent
            brains[o]->setInput(0, inFront&1);          //bit magic to set the inputs
            brains[o]->setInput(1, (inFront>>1)&1);     //bit magic to set the inputs
            switch(inFront){
                case _empty:
                    actionLog[o]["sawNothing"]++;
                    break;
                case _gras:
                    actionLog[o]["sawGras"]++;
                    break;
                case _agent:
                    actionLog[o]["sawAgent"]++;
                    break;
                case _wall:
                    actionLog[o]["sawWall"]++;
                    break;
            }
            brains[o]->setInput(2, scores[o]>0.0);
            
            for(int h=0;h<4;h++){
                brains[o]->setInput(3+h,beepsNow[h]);
                if(beepsNow[h]>0){
                    actionLog[h]["beepReceived"]++;
                }
            }
            
            //now you need to allow the brain a single update "it is thinking now"
            brains[o]->update();
            //now the actions from the brain are querried and translated into a single "action" number
            int action=(bit(brains[o]->getOutput(0))<<2)+(bit(brains[o]->getOutput(1))<<1)+bit(brains[o]->getOutput(2));
            //action=Random::getInt(0,7);
            act[o]=action;
            double isBeeping=brains[o]->getOutput(3);
            beepsNext[o]=isBeeping;
            if(isBeeping>0.0)
                actionLog[o]["beepSent"]++;
            //clear the tile the agent stands on
            area->set(xPos[o],yPos[o],_empty);
            who->set(xPos[o],yPos[o],-1);
            //executing the actions
            //we now have eight actions, because we have three outputs
            //however, we interpret the highst "bit" as  "give food" or "put food" depending of someone being in front
            switch(action){
                case 0:
                    //do nothing
                    actionLog[o]["didNothing"]++;
                    break;
                case 1:
                    //turn left
                    dir[o]=(dir[o]-1)&3;
                    actionLog[o]["turnsLeft"]++;
                    break;
                case 2:
                    //turn right
                    dir[o]=(dir[o]+1)&3;
                    actionLog[o]["turnsRight"]++;
                    break;
                case 3:
                    //move forward
                    //that depends on what tile was inFront
                    switch(inFront){
                        case _gras:
                            scores[o]+=1.0; //gras was eaten
                            // NO break -> to run into the next case which moves the agent
                            actionLog[o]["pickedGras"]++;
                        case _empty:
                            actionLog[o]["movedForward"]++;
                            //move forward
                            xPos[o]+=area->xm[dir[o]];
                            yPos[o]+=area->ym[dir[o]];
                            break;
                        case _agent:
                        case _wall:
                            //agent tried to move into a wall or an agent (which should not exist)
                            //do nothing
                            break;
                    }
                    break;
                case 4: case 5: case 6: case 7:
                    if(scores[o]>0.0)// does the agent collect food in the first place, if not, this is a mute point
                    //give put food (a default: would have done equally good...
                    switch(inFront){
                        case _gras: case _wall:
                            //well nothing happens, we can't put gras on top of gras or into a wall
                            break;
                        case _empty:
                            scores[o]-=1.0;
                            area->set(xPos[o]+area->xm[dir[o]],yPos[o]+area->ym[dir[o]],_gras);
                            actionLog[o]["droppedGras"]++;
                            break;
                        case _agent:
                            //the who array helps us to find the right scores
                            scores[o]-=1.0;
                            scores[who->get(xPos[o]+area->xm[dir[o]],yPos[o]+area->ym[dir[o]])]+=1.0;
                            actionLog[o]["handedGrasOver"]++;
                            break;
                    }
                    break;
            }
        //update the area where we stand
        area->set(xPos[o],yPos[o],_agent);
        who->set(xPos[o],yPos[o],o);
        }
        //add a time tick to the area, so it knows that time progressed
        area->tick();
        if(allowBeeping)
            for(int h=0;h<4;h++){
                beepsNow[h]=beepsNext[h];
            }
        //analyze
        if(analyze){
            rFile<<t;
            for(int h=0;h<4;h++){
                rFile<<","<<xPos[h]<<","<<yPos[h]<<","<<dir[h]<<","<<scores[h]<<","<<act[h]<<","<<beepsNow[h];
            }
            rFile<<std::endl;
        }
    }
    if(analyze){
        rFile.close();
        for(int i=0;i<4;i++){
            actionLog[i]["scoreIndividual"]=scores[i];
            actionLog[i]["scoreGroup"]=scores[0]+scores[1]+scores[2]+scores[3];
            actionLog[i]["score0"]=scores[0];
            actionLog[i]["score1"]=scores[1];
            actionLog[i]["score2"]=scores[2];
            actionLog[i]["score3"]=scores[3];
            //std::cout<<actionLog[i]["scoreIndividual"]<<std::endl;
            //RANK is missing!!!
            for(auto S:metrics){
                organisms[i]->dataCollector->setValue(S, (int)actionLog[i][S]);
                organisms[i]->dataCollector->appendValue(S, (int)actionLog[i][S]);
            }
        }
        
    }
    //organism->dataCollector->setValue("score",score);
    return scores;
}

int OFAAFOWorld::nrOfSensors(){
    return 7;
}

int OFAAFOWorld::nrOfMotors(){
    return 4;
}
void OFAAFOWorld::analyzeAllKOs(){
    std::ofstream iFile;
    iFile.open(Parameters::getString("OFAAFO-iFileName","iFile.csv"));
    /*
    iFile<<"scoreIndividual,scoreGroup,rank,beepSent,beepReceived,turnsLeft,turnsRight,didNothing,movedForward,pickedGras,droppedGras,handedGrasOver,sawNothing,sawGras,sawAgent,sawWall"<<std::endl;

    for(auto s:{"scoreIndividual","scoreGroup","rank","beepSent","beepReceived","turnsLeft","turnsRight",
                "didNothing","movedForward","pickedGras","droppedGras","handedGrasOver","sawNothing",
            "sawGras","sawAgent","sawWall"}){
        for(int i=0;i<4;i++){
            actionLog[i][s]=0;
        }
    }

    
    bool first=true;
    for(auto s:{"scoreIndividual","scoreGroup","rank","beepSent","beepReceived","turnsLeft","turnsRight",
                "didNothing","movedForward","pickedGras","droppedGras","handedGrasOver","sawNothing",
            "sawGras","sawAgent","sawWall"}){
                if(!first){
                    iFile<<",";
                }
                first=false;
                iFile<<actionLog[0][s];
            }
    iFile<<std::endl;
     */
}
