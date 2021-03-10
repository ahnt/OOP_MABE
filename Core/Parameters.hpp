//
//  Parameters.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef Parameters_hpp
#define Parameters_hpp

#include <string>
#include <vector>
#include <map>
#include <iostream>

#define debugFlagNOTSET

class Parameters{
public:
    static std::map<std::string,std::string> parameters;
    static std::map<std::string,std::string> parameterContext; //only used if debugFrag is set
    
    static void initialize(int argc, const char * argv[]);
    static std::string getString(std::string parameterName,std::string defaultValue,std::string context=std::string("defaultContext"));
    static int getInt(std::string parameterName,int defaultValue,std::string context=std::string("defaultContext"));
    static double getDouble(std::string parameterName,double defaultValue,std::string context=std::string("defaultContext"));
    static bool getBool(std::string parameterName,bool defaultValue,std::string context=std::string("defaultContext"));
    static void setParameter(std::string parameterName,std::string value);
    static void setParameter(std::string parameterName,int value);
    static void setParameter(std::string parameterName,double value);
    static void setParameter(std::string parameterName,bool value);

};
#endif /* Parameters_hpp */
