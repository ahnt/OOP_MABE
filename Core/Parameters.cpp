//
//  Parameters.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "Parameters.hpp"

std::map<std::string,std::string> Parameters::parameters;
std::map<std::string,std::string> Parameters::parameterContext;


void Parameters::initialize(int argc, const char * argv[]){
    std::vector<std::string> argList(argv + 1, argv + argc);
    Parameters::parameters.clear();
    for(int i=0;i<(int)argList.size();i+=2){
        Parameters::parameters[argList[i]]=argList[i+1];
    }
#ifdef debugFlag
    Parameters::parameterContext.clear();
#endif
}

std::string Parameters::getString(const std::string parameterName,std::string defaultValue,std::string context){
#ifdef debugFlag
    if(Parameters::parameterContext.find(parameterName)==Parameters::parameterContext.end()){
        if (context.compare(Parameters::parameterContext[parameterName])!=0){
            std::cout << "you querried a parameter from a different context then before!"<< std::endl;
            exit(0);
        } else {
            //you checked a parameter for the second time now
        }
    } else {
        Parameters::parameterContext[parameterName]=context;
    }
#endif
    
    if(Parameters::parameters.find(parameterName)==Parameters::parameters.end())
        return defaultValue;
    return Parameters::parameters[parameterName];
}

void Parameters::setParameter(std::string parameterName,std::string value){
    Parameters::parameters[parameterName]=value;
}

void Parameters::setParameter(std::string parameterName,int value){
    Parameters::parameters[parameterName]=std::to_string(value);
}

void Parameters::setParameter(std::string parameterName,double value){
    Parameters::parameters[parameterName]=std::to_string(value);
}

void Parameters::setParameter(std::string parameterName,bool value){
    Parameters::parameters[parameterName]=std::to_string(value);
}


int Parameters::getInt(const std::string parameterName,int defaultValue,std::string context){
    if(Parameters::parameters.find(parameterName)==Parameters::parameters.end())
        return defaultValue;
    return atoi(Parameters::parameters[parameterName].c_str());

}

double Parameters::getDouble(const std::string parameterName,double defaultValue,std::string context){
    if(Parameters::parameters.find(parameterName)==Parameters::parameters.end())
        return defaultValue;
    return atof(Parameters::parameters[parameterName].c_str());
}

bool Parameters::getBool(const std::string parameterName,bool defaultValue,std::string context){
    std::string S="0";
    if(Parameters::parameters.find(parameterName)==Parameters::parameters.end())
        return defaultValue;
    else
        S=Parameters::parameters[parameterName];
    return (S=="1") || (S=="True") || (S=="true");
}



