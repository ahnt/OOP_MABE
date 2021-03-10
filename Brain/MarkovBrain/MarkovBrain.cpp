//
//  MarkovBrain.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "MarkovBrain.hpp"

std::shared_ptr<Brain> MarkovBrain::getTemplate(){
    std::shared_ptr<MarkovBrain> B=std::make_shared<MarkovBrain>();
    return std::dynamic_pointer_cast<Brain>(B);
}

std::shared_ptr<Brain> MarkovBrain::makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts){
    std::shared_ptr<MarkovBrain> newBrain=std::make_shared<MarkovBrain>();
    
    newBrain->nrOfHiddenStates=Parameters::getInt("MarkovBrain-nrOfHidden",21);//29);
    int nrInWiresAND=3;
    int nrOutWiresAND=3;
    newBrain->nrOfSensors=nrOfIns;
    newBrain->nrOfMotors=nrOfOuts;
    int total=newBrain->nrOfHiddenStates+newBrain->nrOfSensors+newBrain->nrOfMotors;
    newBrain->state.resize(2);
    newBrain->state[0].resize(total);
    newBrain->state[1].resize(total);
    newBrain->currentVector=0;
    newBrain->totalNrOfStates=total;
    
    //regular gates
    bool LogicGatesUsed=Parameters::getBool("MarkovBrain-useLogicGates", true);
    bool ProbabilisticLogicGatesUsed=Parameters::getBool("MarkovBrain-useProbabilisticLogicGates", true);
    //hyper gates
    bool RegularHyperGateUsed=Parameters::getBool("MarkovBrain-useRegularHyperGate",false);
    bool IrregularHyperGateUsed=Parameters::getBool("MarkovBrain-useIrregularHyperGate",false);
    bool IndividualRegularHyperGateUsed=Parameters::getBool("MarkovBrain-IndividualRegularHyperGate", false);
    bool IndividualIrregularHyperGateUsed=Parameters::getBool("MarkovBrain-IndividualIrregularHyperGate", false);
    bool CodeGatesUsed=Parameters::getBool("MarkovBrain-useCodeGates",false);
    int CPPNComplexity=Parameters::getInt("MarkovBrain-CPPNComplexity",12);
    bool GPGatesUsed=Parameters::getBool("MarkovBrain-useGPGates", true);
    //reinforcement gates
    bool ReinforcementLogicGateUsed=Parameters::getBool("MarkovBrain-useReinforcementGate",false);
    
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
            int nrOfInWires=(readHead->getUInt32(coding)&nrInWiresAND)+1;
            int nrOfOutWires=(readHead->getUInt32(coding)&nrOutWiresAND)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires,coding);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires,coding);
            std::vector<std::vector<int>> L;
            for(int i=0;i<(1<<nrOfInWires);i++){
                L.push_back(readHead->getUInt32Vector(nrOfOutWires,coding));
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
            int nrOfInWires=(readHead->getUInt32(coding)&nrInWiresAND)+1;
            int nrOfOutWires=(readHead->getUInt32(coding)&nrOutWiresAND)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires,coding);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires,coding);
            std::vector<std::vector<double>> T;
            for(int i=0;i<(1<<nrOfInWires);i++){
                T.push_back(readHead->getDoublePVector(1<<nrOfOutWires,coding));
            }
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%total;
            for(int o=0;o<(int)O.size();o++)
                O[o]=abs(O[o])%total;
            newBrain->gates.push_back(std::make_shared<ProbabilisticLogicGate>(I,O,T));
        }
        if((RegularHyperGateUsed)&&(lastCodon==44)&&(currentCodon==211)){
            int nrOfInWires=(readHead->getUInt32(coding)&nrInWiresAND)+1;
            int nrOfOutWires=(readHead->getUInt32(coding)&nrOutWiresAND)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires,coding);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires,coding);
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%total;
            for(int o=0;o<(int)O.size();o++)
                O[o]=abs(O[o])%total;
            double xOffset=readHead->getRangedFloat(-PI, PI,coding);
            double yOffset=readHead->getRangedFloat(-PI, PI,coding);
            double xSpacing=readHead->getRangedFloat(0.0, 1.0,coding);
            double ySpacing=readHead->getRangedFloat(0.0, 1.0,coding);
//            printf("%f %f %f %f\n",xOffset,yOffset,xSpacing,ySpacing);
            newBrain->gates.push_back(std::make_shared<RegularHyperGate>(I,O,xOffset,yOffset,xSpacing,ySpacing,cppn));
        }
        if((IrregularHyperGateUsed)&&(lastCodon==45)&&(currentCodon==210)){
            int nrOfInWires=(readHead->getUInt32(coding)&nrInWiresAND)+1;
            int nrOfOutWires=(readHead->getUInt32(coding)&nrOutWiresAND)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires,coding);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires,coding);
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%total;
            for(int o=0;o<(int)O.size();o++)
                O[o]=abs(O[o])%total;
            std::vector<std::vector<std::pair<double,double>>> OM;
            OM.resize(1<<I.size());
            for(int i=0;i<(int)OM.size();i++){
                OM[i].resize(O.size());
                for(int o=0;o<(int)O.size();o++)
                    OM[i][o]=std::pair<double,double>(readHead->getRangedFloat(-PI, PI,coding),readHead->getRangedFloat(-PI, PI,coding));
            }
            newBrain->gates.push_back(std::make_shared<IrregularHyperGate>(I,O,OM,cppn));
        }
        if((IndividualRegularHyperGateUsed)&&(lastCodon==46)&&(currentCodon==209)){
            int nrOfInWires=(readHead->getUInt32(coding)&nrInWiresAND)+1;
            int nrOfOutWires=(readHead->getUInt32(coding)&nrOutWiresAND)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires,coding);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires,coding);
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%total;
            for(int o=0;o<(int)O.size();o++)
                O[o]=abs(O[o])%total;
            newBrain->gates.push_back(std::make_shared<IndividualRegularHyperGate>(I,O,readHead,CPPNComplexity));
        }
        if((IndividualIrregularHyperGateUsed)&&(lastCodon==47)&&(currentCodon==208)){
            int nrOfInWires=(readHead->getUInt32(coding)&nrInWiresAND)+1;
            int nrOfOutWires=(readHead->getUInt32(coding)&nrOutWiresAND)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires,coding);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires,coding);
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%total;
            for(int o=0;o<(int)O.size();o++)
                O[o]=abs(O[o])%total;
            newBrain->gates.push_back(std::make_shared<IndividualIrregularHyperGate>(I,O,readHead,CPPNComplexity));
        }
        if((ReinforcementLogicGateUsed)&&(lastCodon==48)&&(currentCodon==207)){
            int nrOfInWires=(readHead->getUInt32(coding)&nrInWiresAND)+1;
            int nrOfOutWires=(readHead->getUInt32(coding)&nrOutWiresAND)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires,coding);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires,coding);
            std::vector<std::vector<double>> T;
            for(int i=0;i<(1<<nrOfInWires);i++){
                /*
                std::vector<double> V;
                for (int j=0;j<(1<<nrOfOutWires); j++)
                    V.push_back((double)0.01);
                V[readHead->getUInt32()%V.size()]=1.0;
                T.push_back(V);
                 */

                T.push_back(readHead->getDoublePVector(1<<nrOfOutWires,coding));
            }
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%total;
            for(int o=0;o<(int)O.size();o++)
                O[o]=abs(O[o])%total;
            newBrain->gates.push_back(std::make_shared<ReinforcementLogicGate>(I,O,T));
        }
        if((CodeGatesUsed)&&(lastCodon==49)&&(currentCodon==206)){
            int nrOfInWires=(readHead->getUInt32(coding)&nrInWiresAND)+1;
            int nrOfOutWires=(readHead->getUInt32(coding)&nrOutWiresAND)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires,coding);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires,coding);
            std::vector<int> CODE;//=readHead->getUInt32Vector(64,coding);
            std::vector<std::vector<int>> T;
            for(int i=0;i<(1<<nrOfInWires);i++){
                T.push_back(readHead->getUInt32Vector(1<<nrOfOutWires,coding));
            }
            CODE=readHead->getUInt32Vector(64,coding);
            for(int i=0;i<(int)I.size();i++)
                I[i]=abs(I[i])%total;
            for(int o=0;o<(int)O.size();o++)
                O[o]=abs(O[o])%total;
            newBrain->gates.push_back(std::make_shared<CodeGate>(I,O,T,CODE));
        }
        if((GPGatesUsed)&&(lastCodon==50)&&(currentCodon==205)){
            int nrOfInWires=(readHead->getUInt32(coding)&nrInWiresAND)+1;
            int nrOfOutWires=(readHead->getUInt32(coding)&nrOutWiresAND)+1;
            std::vector<int> I=readHead->getUInt32Vector(nrOfInWires,coding);
            std::vector<int> O=readHead->getUInt32Vector(nrOfOutWires,coding);
            int theOpp=readHead->getUInt32();
            std::vector<double> C=readHead->getRangedDoubleVector((int)O.size(), -256, 256);
            newBrain->gates.push_back(std::make_shared<GPGate>(I,O,theOpp,C));
        }
        lastCodon=currentCodon;
    }
    newBrain->resetBrain();
    return std::dynamic_pointer_cast<Brain>(newBrain);
}

std::shared_ptr<Brain> MarkovBrain::clone(){
    std::shared_ptr<MarkovBrain> newBrain=std::make_shared<MarkovBrain>();
    newBrain->nrOfHiddenStates=nrOfHiddenStates;
    newBrain->nrOfSensors=nrOfSensors;
    newBrain->nrOfMotors=nrOfMotors;
    newBrain->state=state;
    newBrain->currentVector=currentVector;
    newBrain->gates.clear();
    for(auto G:gates){
        newBrain->gates.push_back(G->clone());
    }
    return std::dynamic_pointer_cast<Brain>(newBrain);
}


void MarkovBrain::update(){
    state[1-currentVector].assign(state[1-currentVector].size(),0.0);
    for(int g=0;g<(int)gates.size();g++)
        gates[g]->update(state[currentVector],state[1-currentVector]);
    currentVector=1-currentVector;
}

void MarkovBrain::setInput(int which, double toWhat){
    state[currentVector][which]=toWhat;
}

double MarkovBrain::getOutput(int which){
    return state[currentVector][nrOfSensors+which];
}
void MarkovBrain::setOutput(int which, double toWhat){
    state[currentVector][nrOfSensors+which]=toWhat;
}

void MarkovBrain::clearOutput(){
    for(int o=0;o<nrOfMotors;o++){
        state[currentVector][nrOfSensors+o]=0.0;
    }
}

std::vector<double> MarkovBrain::getHidden(){
    std::vector<double> H;
    H.insert(H.begin(), state[currentVector].begin()+nrOfSensors+nrOfMotors,state[currentVector].end());
    return H;
}

void MarkovBrain::setHidden(int which,double toWhat){
    state[currentVector][nrOfSensors+nrOfMotors+which]=toWhat;
}

void MarkovBrain::resetBrain(){
    for(int i=0;i<state.size();i++)
        state[i].assign(state[i].size(), 0.0);
}

std::string MarkovBrain::serializeToString(){
    std::string S;
    for(int g=0;g<(int)gates.size();g++){
        S+="Gate: "+std::to_string(g)+"\n"+gates[g]->serializeToString()+"\n";
    }
    return S;
}

void MarkovBrain::addBrainStatsToDataCollector(std::shared_ptr<DataCollector> dataCollector){
    std::string S="\"[";
    for(int g=0;g<(int)gates.size();g++){
        if(g!=0)
            S+=",";
        S+=gates[g]->getType();
    }
    S+="]\"";
    dataCollector->setValue("BrainStats", S);
}


std::string MarkovBrain::renderToDot(){
    std::string S="digraph markovBrain{\n\trankdir = TB;\n";
    int g=0;
    std::set<int> I,O,G;
    for(auto gate:gates){
         S+="\tG"+std::to_string(g)+" [shape=box];\n";
        for(auto i:gate->ins){
            S+="\tt"+std::to_string(i)+" -> G"+std::to_string(g)+";\n";
            I.insert(i);
            G.insert(g);
        }
        for(auto o:gate->outs){
            S+="\tG"+std::to_string(g)+" -> tp"+std::to_string(o)+";\n";
            O.insert(o);
            G.insert(g);
        }
        g++;
    }
    S+="\t{rank=min; ";
    bool f=true;
    for(auto i:I){
        if(f)
            f=false;
        else
            S+=",";
        S+="t"+std::to_string(i);
    }
    S+="};\n";
    S+="\t{rank=same; ";
    f=true;
    for(auto i:G){
        if(f)
            f=false;
        else
            S+=",";
        S+="G"+std::to_string(i);
    }
    S+="};\n";
    S+="\t{rank=max; ";
    f=true;
    for(auto i:O){
        if(f)
            f=false;
        else
            S+=",";
        S+="tp"+std::to_string(i);
    }
    S+="};\n";
    S+="}\n";
    return S;
}

std::vector<std::vector<int>> MarkovBrain::getConnectivityMatrix(){
    std::vector<std::vector<int>> M;
    
    M.resize(totalNrOfStates);
    for(int t=0;t<totalNrOfStates;t++){
        M[t].resize(totalNrOfStates);
        for(int tt=0;tt<totalNrOfStates;tt++)
            M[t][tt]=0;
    }
    for(auto gate:gates){
        for(auto i:gate->ins){
            for(auto o:gate->outs){
                M[i][o]++;
            }
        }
    }
    return M;
}
