//
//  LODmanager.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/17/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef LODmanager_hpp
#define LODmanager_hpp

#include "Population.hpp"
#include <fstream>
#include <set>

class LODManager{
    std::string LODFilename;
    std::string GENOMEFilename;
    std::ofstream LODFileHandle;
    std::ofstream GENOMEFileHandle;

    
    std::vector<std::string> itemsKeysToSave;
    int saveIntervallGENOME;
    int saveIntervallLOD;
    int intervalToCheck;
    std::string preface;
    
public:
    LODManager(std::string individualFilePreface);
    
    void performIntermediaryCheck(std::shared_ptr<Population> population);
    void wrapUp(std::shared_ptr<Population> population);
    void savePop(std::shared_ptr<Population> population,std::string filename);
};

#endif /* LODmanager_hpp */
