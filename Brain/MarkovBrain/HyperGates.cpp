//
//  HyperGates.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/24/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "HyperGates.hpp"

RegularHyperGate::RegularHyperGate(std::vector<int> _ins,std::vector<int> _outs,double xOffset,double yOffset,double xSpacing, double ySpacing,std::shared_ptr<CPPN> cppn){
    ins=_ins;
    outs=_outs;
    logic.resize(1<<ins.size());
    for(int i=0;i<(int)logic.size();i++){
        logic[i].resize(outs.size());
        for(int o=0;o<(int)logic[i].size();o++){
            double value=cppn->update((double)i*xSpacing+xOffset, (double)o*ySpacing+yOffset);
            //printf("%f\n",value);
            logic[i][o]=bit(value);
        }
    }
}

IrregularHyperGate::IrregularHyperGate(std::vector<int> _ins,std::vector<int> _outs,std::vector<std::vector<std::pair<double,double>>> &_offsetMatrix,std::shared_ptr<CPPN> cppn){
    ins=_ins;
    outs=_outs;
    logic.resize(1<<ins.size());
    for(int i=0;i<(int)logic.size();i++){
        logic[i].resize(outs.size());
        for(int o=0;o<(int)logic[i].size();o++){
            std::pair<double,double> O=_offsetMatrix[i][o];
            double value=cppn->update(O.first,O.second);
            logic[i][o]=bit(value);
        }
    }

}

IndividualRegularHyperGate::IndividualRegularHyperGate(std::vector<int> _ins,std::vector<int> _outs,std::shared_ptr<Genome::ReadHead> readHead,int CPPNcomplexity){
    ins=_ins;
    outs=_outs;

    cppn=std::make_shared<CPPN>(readHead,CPPNcomplexity);
    double xOffset=readHead->getRangedFloat(-PI, PI);
    double yOffset=readHead->getRangedFloat(-PI, PI);
    double xSpacing=readHead->getRangedFloat(0.0, 1.0);
    double ySpacing=readHead->getRangedFloat(0.0, 1.0);

    logic.resize(1<<ins.size());
    for(int i=0;i<(int)logic.size();i++){
        logic[i].resize(outs.size());
        for(int o=0;o<(int)logic[i].size();o++){
            double value=cppn->update((double)i*xSpacing+xOffset, (double)o*ySpacing+yOffset);
            //printf("%f\n",value);
            logic[i][o]=bit(value);
        }
    }
}

IndividualIrregularHyperGate::IndividualIrregularHyperGate(std::vector<int> _ins,std::vector<int> _outs,std::shared_ptr<Genome::ReadHead> readHead,int CPPNcomplexity){
    ins=_ins;
    outs=_outs;
    
    cppn=std::make_shared<CPPN>(readHead,CPPNcomplexity);
    
    std::vector<std::vector<std::pair<double,double>>> OM;
    OM.resize(1<<ins.size());
    for(int i=0;i<(int)OM.size();i++){
        OM[i].resize(outs.size());
        for(int o=0;o<(int)outs.size();o++)
            OM[i][o]=std::pair<double,double>(readHead->getRangedFloat(-PI, PI),readHead->getRangedFloat(-PI, PI));
    }

    logic.resize(1<<ins.size());
    for(int i=0;i<(int)logic.size();i++){
        logic[i].resize(outs.size());
        for(int o=0;o<(int)logic[i].size();o++){
            double value=cppn->update(OM[i][o].first,OM[i][o].second);
            logic[i][o]=bit(value);
        }
    }
}
