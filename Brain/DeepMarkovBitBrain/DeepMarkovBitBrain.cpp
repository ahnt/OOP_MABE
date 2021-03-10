//
//  DeepMarkovBitBrain.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/28/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "DeepMarkovBitBrain.hpp"

std::shared_ptr<Brain> DeepMarkovBitBrain::getTemplate(){
    std::shared_ptr<DeepMarkovBitBrain> B=std::make_shared<DeepMarkovBitBrain>();
    return std::dynamic_pointer_cast<Brain>(B);
}

std::shared_ptr<Brain> DeepMarkovBitBrain::makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts){
    std::shared_ptr<DeepMarkovBitBrain> newBrain=std::make_shared<DeepMarkovBitBrain>();
    
    newBrain->nrOfHiddenStates=Parameters::getInt("DeepMarkovBitBrain-nrOfHidden",40);
    newBrain->nrOfSensors=nrOfIns;
    newBrain->nrOfMotors=nrOfOuts;
    
    std::vector<std::string> widthOfHiddenLayers=Utilities::splitStringIntoVectorOfString(Parameters::getString("DeepMarkovBitBrain-widthOfHiddenLayers",""), ',');
    newBrain->state.resize(2+(int)widthOfHiddenLayers.size());
    newBrain->state[0].resize(newBrain->nrOfSensors+newBrain->nrOfHiddenStates);
    newBrain->gates.resize(1+(int)widthOfHiddenLayers.size());
    for(int l=0;l<widthOfHiddenLayers.size();l++){
        int width=atoi(widthOfHiddenLayers[l].c_str());
        newBrain->state[l+1].resize(width);
        newBrain->gates[l].resize(width);
    }
    newBrain->state[newBrain->state.size()-1].resize(newBrain->nrOfMotors+newBrain->nrOfHiddenStates);
    newBrain->gates[newBrain->gates.size()-1].resize(newBrain->nrOfMotors+newBrain->nrOfHiddenStates);
    
    auto readHead=genome->getReadhead();
    //construction with Deterministic Logic Gates
    //*
    for(int l=0;l<newBrain->gates.size();l++){
        for(int g=0;g<newBrain->gates[l].size();g++){
            int nrOfInWires=4;
            int nrOfOutWires=1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires);
            std::vector<std::vector<int>> L;
            for(int i=0;i<(1<<nrOfInWires);i++){
                L.push_back(readHead->getUInt32Vector(nrOfOutWires));
            }
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%newBrain->state[l].size();
            for(int o=0;o<(int)O.size();o++)
                O[o]=g;
            for(int i=0;i<(int)L.size();i++){
                for(int o=0;o<(int)L[i].size();o++)
                    L[i][o]=L[i][o]&1;
            }
            newBrain->gates[l][g]=std::make_shared<LogicGate>(I,O,L);
        }
    }
//    std::cout<<readHead->locationString()<<std::endl;
//    exit(0);
    //*/
    /*
    //construction with Probabilistic Reinforcement Gates
    for(int l=0;l<newBrain->gates.size();l++){
        for(int g=0;g<newBrain->gates[l].size();g++){
            int nrOfInWires=(int)newBrain->state[l].size();
            //int nrOfInWires=(int)4;
            int nrOfOutWires=1;
            //std::vector<int> I=readHead->getUInt32Vector(nrOfInWires);
            std::vector<int> I;//=readHead->getUInt32Vector(nrOfInWires);
            for(int i=0;i<nrOfInWires;i++){
                I.push_back(i);
                //I.push_back(Random::getInt(0,newBrain->state[l].size()-1));
            }
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires);
            std::vector<std::vector<double>> T;
            for(int i=0;i<(1<<nrOfInWires);i++){
                T.push_back(readHead->getDoublePVector(1<<nrOfOutWires));
                for(int t=0;t<(int)T[i].size();t++)
                    T[i][t]=1.0;
            }
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%newBrain->state[l].size();
            for(int o=0;o<(int)O.size();o++)
                O[o]=g;
            newBrain->gates[l][g]=std::make_shared<ReinforcementLogicGate>(I,O,T);
        }
    }
    //*/
    /*
    int total=newBrain->nrOfHiddenStates+newBrain->nrOfSensors+newBrain->nrOfMotors;
    newBrain->state.resize(2);
    newBrain->state[0].resize(total);
    newBrain->state[1].resize(total);
    newBrain->currentVector=0;
    
    bool LogicGatesUsed=Parameters::getBool("MarkovBrain-useLogicGates", false);
    bool ProbabilisticLogicGatesUsed=Parameters::getBool("MarkovBrain-useProbabilisticLogicGates", false);
    bool RegularHyperGateUsed=Parameters::getBool("MarkovBrain-useRegularHyperGate",false);
    bool IrregularHyperGateUsed=Parameters::getBool("MarkovBrain-useIrregularHyperGate",true);
    bool IndividualRegularHyperGateUsed=Parameters::getBool("MarkovBrain-IndividualRegularHyperGate", false);
    bool IndividualIrregularHyperGateUsed=Parameters::getBool("MarkovBrain-IndividualIrregularHyperGate", false);
    int CPPNComplexity=Parameters::getInt("MarkovBrain-CPPNComplexity",4);
    auto readHead=genome->getReadhead();
    
    //do the CPPN stuff
    std::shared_ptr<CPPN> cppn=nullptr;
    if(RegularHyperGateUsed||IrregularHyperGateUsed||IndividualRegularHyperGateUsed||IndividualIrregularHyperGateUsed){
        cppn=std::make_shared<CPPN>(readHead,CPPNComplexity);
    }

    int currentCodon=0;
    int lastCodon=0;
    while(!readHead->eog()){
        currentCodon=readHead->getByte();
        //add gates here depending on codons
        if((LogicGatesUsed)&&(lastCodon==42)&&(currentCodon==213)){
            int nrOfInWires=(readHead->getUInt32()&3)+1;
            int nrOfOutWires=(readHead->getUInt32()&3)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires);
            std::vector<std::vector<int>> L;
            for(int i=0;i<(1<<nrOfInWires);i++){
                L.push_back(readHead->getUInt32Vector(nrOfOutWires));
            }
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%total;
            for(int o=0;o<(int)O.size();o++)
                O[o]=abs(O[o])%total;
            for(int i=0;i<(int)L.size();i++){
                for(int o=0;o<(int)L[i].size();o++)
                    L[i][o]=L[i][o]&1;
            }
            newBrain->gates.push_back(std::make_shared<LogicGate>(I,O,L));
        }
        if((ProbabilisticLogicGatesUsed)&&(lastCodon==43)&&(currentCodon==212)){
            int nrOfInWires=(readHead->getUInt32()&3)+1;
            int nrOfOutWires=(readHead->getUInt32()&3)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires);
            std::vector<std::vector<double>> T;
            for(int i=0;i<(1<<nrOfInWires);i++){
                T.push_back(readHead->getDoublePVector(1<<nrOfOutWires));
            }
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%total;
            for(int o=0;o<(int)O.size();o++)
                O[o]=abs(O[o])%total;
            newBrain->gates.push_back(std::make_shared<ProbabilisticLogicGate>(I,O,T));
        }
        if((RegularHyperGateUsed)&&(lastCodon==44)&&(currentCodon==211)){
            int nrOfInWires=(readHead->getUInt32()&3)+1;
            int nrOfOutWires=(readHead->getUInt32()&3)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires);
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%total;
            for(int o=0;o<(int)O.size();o++)
                O[o]=abs(O[o])%total;
            double xOffset=readHead->getRangedFloat(-PI, PI);
            double yOffset=readHead->getRangedFloat(-PI, PI);
            double xSpacing=readHead->getRangedFloat(0.0, 1.0);
            double ySpacing=readHead->getRangedFloat(0.0, 1.0);
//            printf("%f %f %f %f\n",xOffset,yOffset,xSpacing,ySpacing);
            newBrain->gates.push_back(std::make_shared<RegularHyperGate>(I,O,xOffset,yOffset,xSpacing,ySpacing,cppn));
        }
        if((IrregularHyperGateUsed)&&(lastCodon==45)&&(currentCodon==210)){
            int nrOfInWires=(readHead->getUInt32()&3)+1;
            int nrOfOutWires=(readHead->getUInt32()&3)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires);
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%total;
            for(int o=0;o<(int)O.size();o++)
                O[o]=abs(O[o])%total;
            std::vector<std::vector<std::pair<double,double>>> OM;
            OM.resize(1<<I.size());
            for(int i=0;i<(int)OM.size();i++){
                OM[i].resize(O.size());
                for(int o=0;o<(int)O.size();o++)
                    OM[i][o]=std::pair<double,double>(readHead->getRangedFloat(-PI, PI),readHead->getRangedFloat(-PI, PI));
            }
            newBrain->gates.push_back(std::make_shared<IrregularHyperGate>(I,O,OM,cppn));
        }
        if((IndividualRegularHyperGateUsed)&&(lastCodon==46)&&(currentCodon==209)){
            int nrOfInWires=(readHead->getUInt32()&3)+1;
            int nrOfOutWires=(readHead->getUInt32()&3)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires);
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%total;
            for(int o=0;o<(int)O.size();o++)
                O[o]=abs(O[o])%total;
            newBrain->gates.push_back(std::make_shared<IndividualRegularHyperGate>(I,O,readHead,CPPNComplexity));
        }
        if((IndividualIrregularHyperGateUsed)&&(lastCodon==47)&&(currentCodon==208)){
            int nrOfInWires=(readHead->getUInt32()&3)+1;
            int nrOfOutWires=(readHead->getUInt32()&3)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires);
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%total;
            for(int o=0;o<(int)O.size();o++)
                O[o]=abs(O[o])%total;
            newBrain->gates.push_back(std::make_shared<IndividualIrregularHyperGate>(I,O,readHead,CPPNComplexity));
        }
        lastCodon=currentCodon;
    }
     */
    newBrain->KOPattern=0;
    newBrain->resetBrain();
    return std::dynamic_pointer_cast<Brain>(newBrain);
}


void DeepMarkovBitBrain::update(){
    int n=0;
    for(int l=0;l<gates.size();l++){
        for(int g=0;g<(int)gates[l].size();g++){
            if(((KOPattern>>n)&1)==0)
                gates[l][g]->update(state[l],state[l+1]);
            n++;
        }
    }
    for(int h=0;h<nrOfHiddenStates;h++)
        state[0][h+nrOfSensors]=state[state.size()-1][h];
}

void DeepMarkovBitBrain::setInput(int which, double toWhat){
    state[0][which]=toWhat;
}

double DeepMarkovBitBrain::getOutput(int which){
    return state[state.size()-1][nrOfHiddenStates+which];
}

void DeepMarkovBitBrain::resetBrain(){
    for(int i=0;i<state.size();i++)
        state[i].assign(state[i].size(), 0.0);
}

std::string DeepMarkovBitBrain::serializeToString(){
    std::string S;
    for(int l=0;l<gates.size();l++){
        for(int g=0;g<(int)gates.size();g++){
            S+="Layer: "+std::to_string(l)+"Gate: "+std::to_string(g)+"\n"+gates[l][g]->serializeToString()+"\n";
        }
    }
    return S;
}

void DeepMarkovBitBrain::addBrainStatsToDataCollector(std::shared_ptr<DataCollector> dataCollector){
    std::string S="\"[";
    for(int l=0;l<gates.size();l++){
        for(int g=0;g<(int)gates[l].size();g++){
            if(g!=0)
                S+=",";
            S+=gates[l][g]->getType();
        }
    }
    S+="]\"";
    dataCollector->setValue("BrainStats", S);
}


std::vector<std::vector<std::pair<int,int>>> DeepMarkovBitBrain::getReinforcementRecord(){
    std::vector<std::vector<std::pair<int,int>>> record;
    for(int l=0;l<(int)gates.size();l++)
        for(int g=0;g<(int)gates[l].size();g++)
            record.push_back(gates[l][g]->getRecord());
    return record;
}

void DeepMarkovBitBrain::setReinforcementRecord(std::vector<std::vector<std::pair<int,int>>> record){
    int i=0;
    for(int l=0;l<(int)gates.size();l++)
        for(int g=0;g<(int)gates[l].size();g++){
            gates[l][g]->setRecord(record[i]);
            i++;
        }
}

void DeepMarkovBitBrain::resetReinforcementRecord(){
    for(int l=0;l<(int)gates.size();l++)
        for(int g=0;g<(int)gates[l].size();g++)
            gates[l][g]->clearReinforcement();
}

void DeepMarkovBitBrain::reinforce(double weight,std::vector<double> modulator){
    for(int l=0;l<(int)gates.size();l++)
        for(int g=0;g<(int)gates[l].size();g++)
            gates[l][g]->reinforceGate(weight,modulator);
}

void DeepMarkovBitBrain::show(){
    for(int l=0;l<(int)gates.size();l++){
        std::cout<<"layer: "<<l<<std::endl;
        for(int g=0;g<(int)gates[l].size();g++){
            gates[l][g]->show();
        }
    }
}

std::string DeepMarkovBitBrain::renderToDot(){
    std::string S="digraph brain{\n";
    std::set<int> I,O,G;
    bool f;
    int l;
    for(l=0;l<gates.size();l++){
        I.clear();
        O.clear();
        G.clear();
        for(int g=0;g<gates[l].size();g++){
            S+="\tG"+std::to_string(l)+"_"+std::to_string(g)+" [shape=box,label=\""+gates[l][g]->dotTag()+"\"];\n";
            for(auto i:gates[l][g]->ins){
                I.insert(i);
                G.insert(g);
                S+="\tN"+std::to_string(l)+"_"+std::to_string(i)+" -> G"+std::to_string(l)+"_"+std::to_string(g)+";\n";
            }
            for(auto o:gates[l][g]->outs){
                O.insert(o);
                G.insert(g);
                S+="\tG"+std::to_string(l)+"_"+std::to_string(g)+" -> N"+std::to_string(l+1)+"_"+std::to_string(o)+";\n";
            }
        }
        S+="\t{rank=same; ";
        f=true;
        for(auto i:I){
            if(f)
                f=false;
            else
                S+=",";
            S+="N"+std::to_string(l)+"_"+std::to_string(i);
        }
        S+="};\n";
        S+="\t{rank=same; ";
        f=true;
        for(auto i:G){
            if(f)
                f=false;
            else
                S+=",";
            S+="G"+std::to_string(l)+"_"+std::to_string(i);
        }
        S+="};\n";    }
    S+="\t{rank=max; ";
    f=true;
    for(auto i:O){
        if(f)
            f=false;
        else
            S+=",";
        S+="N"+std::to_string(l)+"_"+std::to_string(i);
    }
    S+="};\n";
    S+="}\n";
    return S;
}

std::string DeepMarkovBitBrain::getComponentList(){
    std::string S="\"[";
    bool first=true;
    for(int l=0;l<(int)gates.size();l++){
        for(int g=0;g<(int)gates[l].size();g++){
            if(first)
                first=false;
            else
                S+=",";
            S+=gates[l][g]->dotTag();
        }
    }
    S+="]\"";
    return S;
}

/*
std::shared_ptr<Brain> DeepMarkovBitGeneBrain::getTemplate(){
    std::shared_ptr<DeepMarkovBitGeneBrain> B=std::make_shared<DeepMarkovBitGeneBrain>();
    return std::dynamic_pointer_cast<Brain>(B);
}

std::shared_ptr<Brain> DeepMarkovBitGeneBrain::makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts){
    std::shared_ptr<DeepMarkovBitGeneBrain> newBrain=std::make_shared<DeepMarkovBitGeneBrain>();
    
    newBrain->nrOfHiddenStates=Parameters::getInt("DeepMarkovBitGeneBrain-nrOfHidden",4);
    newBrain->nrOfSensors=nrOfIns;
    newBrain->nrOfMotors=nrOfOuts;
    
    std::vector<std::string> widthOfHiddenLayers=Utilities::splitStringIntoVectorOfString(Parameters::getString("DeepMarkovBitGeneBrain-widthOfHiddenLayers","10"), ',');
    newBrain->state.resize(2+(int)widthOfHiddenLayers.size());
    newBrain->state[0].resize(newBrain->nrOfSensors+newBrain->nrOfHiddenStates);
    newBrain->gates.resize(1+(int)widthOfHiddenLayers.size());
    for(int l=0;l<widthOfHiddenLayers.size();l++){
        int width=atoi(widthOfHiddenLayers[l].c_str());
        newBrain->state[l+1].resize(width);
        //newBrain->gates[l].resize(width);
    }
    newBrain->state[newBrain->state.size()-1].resize(newBrain->nrOfMotors+newBrain->nrOfHiddenStates);
    //newBrain->gates[newBrain->gates.size()-1].resize(newBrain->nrOfMotors+newBrain->nrOfHiddenStates);
    
    auto readHead=genome->getReadhead();
    
    int currentCodon=0;
    int lastCodon=0;
    while(!readHead->eog()){
        currentCodon=readHead->getByte();
        //add gates here depending on codons
        if(lastCodon==42){//}&&(currentCodon>=213)){
            int layer=readHead->getUInt32()%newBrain->gates.size();
            int nrOfInWires=2;
            int nrOfOutWires=1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires);
            std::vector<std::vector<int>> L;
            for(int i=0;i<(1<<nrOfInWires);i++){
                L.push_back(readHead->getUInt32Vector(nrOfOutWires));
            }
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%newBrain->state[layer].size();
            for(int o=0;o<(int)O.size();o++)
                O[o]=abs(O[o])%newBrain->state[layer+1].size();
            for(int i=0;i<(int)L.size();i++){
                for(int o=0;o<(int)L[i].size();o++)
                    L[i][o]=L[i][o]&1;
            }
            newBrain->gates[layer].push_back(std::make_shared<LogicGate>(I,O,L));

        }
        lastCodon=currentCodon;
    }

    
    newBrain->KOPattern=0;
    newBrain->resetBrain();
    return std::dynamic_pointer_cast<Brain>(newBrain);
}

*/
