//
//  Brain.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef Brain_hpp
#define Brain_hpp

#include <memory>
#include "../Genome/Genome.hpp"
#include "../Core/DataCollector.hpp"

class Brain{
public:
    int nrOfSensors,nrOfMotors;
    static std::shared_ptr<Brain> getTemplate();
    virtual std::shared_ptr<Brain> makeBrainFromGenome(std::shared_ptr<Genome> genome,int nrOfIns,int nrOfOuts);
    virtual void update();
    virtual std::shared_ptr<Brain> clone(){ std::cout <<"clone function not implemented"<<std::endl; exit(0); return nullptr;}
    
    virtual void setInput(int which, double toWhat){}
    virtual double getOutput(int which){return 0.0;}
    virtual void setOutput(int which, double toWhat){}
    virtual void clearOutput(){std::cout<<"clear Output not implemented for this brain"<<std::endl; exit(0);}
    virtual std::vector<double> getHidden() {return std::vector<double>();}
    virtual void setHidden(int which,double toWhat) {std::cout<<"set Hidden not implemented for this brain"<<std::endl; exit(0);}
    virtual void resetBrain(){}
    virtual std::string serializeToString();
    virtual int getNrOfInputs(){return nrOfSensors;};
    virtual int getNrOfOutputs(){return nrOfMotors;};
    virtual void addBrainStatsToDataCollector(std::shared_ptr<DataCollector> dataCollector){};

    int KOPattern;
    virtual void setKOpattern(int thePattern){KOPattern=thePattern;}
    virtual void show(){};
    virtual std::string renderToDot() {return "digraph noGraph{}";}
    virtual std::string getComponentList(){ return "\"[]\"";}
    virtual std::vector<std::vector<int>> getConnectivityMatrix(){ return std::vector<std::vector<int>>();}
    virtual void retire(){std::cout<<"retire Brain not implemented for this brain"<<std::endl; exit(0);}
};

#endif /* Brain_hpp */
