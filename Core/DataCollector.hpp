//
//  DataCollector.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef DataCollector_hpp
#define DataCollector_hpp

#include <memory>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "Utilities.hpp"

class DataCollector{
public:
    std::map<std::string,std::string> stringData;
    std::map<std::string,int> intData;
    std::map<std::string,double> doubleData;
    std::map<std::string,std::vector<double>> doubleVectorData;
    std::map<std::string,std::vector<int>> intVectorData;

    //DataCollector()=delete;
    
    void setValue(std::string key, std::string value);
    void setValue(std::string key, int value);
    void setValue(std::string key, double value);
    
    void setValue(std::string key, std::vector<double> value);
    void setValue(std::string key, std::vector<int> value);
    
    void addValue(std::string key, int value);
    void addValue(std::string key, double value);
    void multValue(std::string key, int value);
    void multValue(std::string key, double value);
    void appendValue(std::string key, int value);
    void appendValue(std::string key, double value);
    void appendValue(std::string key, std::string value);
    std::string getString(std::string key) {return stringData[key];}
    int getInt(std::string key) {return intData[key];}
    double getDouble(std::string key) {return doubleData[key];}
    std::vector<double> getDoubleVector(std::string key) {return doubleVectorData[key];}
    std::vector<int> getIntVector(std::string key) {return intVectorData[key];}
    std::string getAsStringFromUnknownType(std::string key);
    std::vector<std::string> getAllKeysDouble();
    void purge();
};

#endif /* DataCollector_hpp */
