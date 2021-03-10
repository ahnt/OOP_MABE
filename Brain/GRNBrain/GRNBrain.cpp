//
//  GRNBrain.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/28/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "GRNBrain.hpp"

std::shared_ptr<Brain> GRNBrain::getTemplate(){
    std::shared_ptr<GRNBrain> B=std::make_shared<GRNBrain>();
    return std::dynamic_pointer_cast<Brain>(B);
}

std::shared_ptr<Brain> GRNBrain::makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts){
    std::shared_ptr<GRNBrain> newBrain=std::make_shared<GRNBrain>();
    int brainSize=Parameters::getInt("GRNBrain-brainSize",20);
    newBrain->ticksPerBrainUpdate=Parameters::getInt("GRNBrain-ticksPerBrainUpdate",20);
    newBrain->brainSize=brainSize;
    newBrain->nrOfSensors=nrOfIns;
    newBrain->nrOfMotors=nrOfOuts;
    newBrain->V.assign(brainSize,1.0/(double)brainSize);
    newBrain->M.resize(brainSize);
    
    auto readHead=genome->getReadhead();
    
    for(int i=0;i<brainSize;i++){
        newBrain->M[i].resize(brainSize);
        for(int j=0;j<brainSize;j++){
            newBrain->M[i][j]=readHead->getRangedFloat(-1.0, 1.0);
        }
    }
    /*
    newBrain->nrOfHiddenStates=Parameters::getInt("MarkovBrain-nrOfHidden",10);
    newBrain->nrOfSensors=nrOfIns;
    newBrain->nrOfMotors=nrOfOuts;
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
    
    return std::dynamic_pointer_cast<Brain>(newBrain);
}


void GRNBrain::update(){
    /*
    std::vector<double> nV;
    double grnAlpha=0.5;
    nV.resize(brainSize);
    for(int t=0;t<ticksPerBrainUpdate;t++){
        for(int n=nrOfSensors;n<brainSize;n++){
            nV[n]=0.0;
            for(int m=0;m<brainSize;m++){
                nV[n]+=V[m]*M[n][m];
            }
            nV[n]=(2.0/(1.0+exp(-nV[n])))-1.0;
        }
        for(int n=0;n<brainSize;n++)
            V[n]=V[n]-((V[n]-nV[n])*grnAlpha);
    }
    nV.clear();
     */
    std::vector<double> nV;
    double grnAlpha=0.5;
    nV.resize(brainSize);
    for(int t=0;t<ticksPerBrainUpdate;t++){
        for(int n=nrOfSensors;n<brainSize;n++){
            nV[n]=0.0;
            for(int m=0;m<brainSize;m++){
                if(((KOPattern>>m)&1)==0)
                    nV[n]+=V[m]*M[n][m];
            }
            nV[n]=(2.0/(1.0+exp(-nV[n])))-1.0;
        }
        for(int n=0;n<brainSize;n++)
            V[n]=V[n]-((V[n]-nV[n])*grnAlpha);
    }
    nV.clear();

}

void GRNBrain::setInput(int which, double toWhat){
    V[which]=(toWhat*2.0)-1.0;
}

double GRNBrain::getOutput(int which){
    return V[nrOfSensors+which];
}

void GRNBrain::resetBrain(){
    V.assign(brainSize,1.0/(double)brainSize);
}

std::string GRNBrain::serializeToString(){
    std::string S="GRN serialize to String has not been implemented yet";
    return S;
}

void GRNBrain::addBrainStatsToDataCollector(std::shared_ptr<DataCollector> dataCollector){
    std::string S="\"[GRN has no brain stats to report]\"";
    dataCollector->setValue("BrainStats", S);
}
