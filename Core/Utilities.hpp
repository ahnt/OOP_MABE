//
//  Utilities.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/16/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef Utilities_hpp
#define Utilities_hpp

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>

#define PI 3.14159265359


class Utilities{
public:
    static int currentGeneration;
    static int ID;
    static int getID();
    //static std::string serializeVectorToString(std::vector<int> &V,std::string separator);
    //static std::string serializeVectorToString(std::vector<float> &V,std::string separator);
    //static std::string serializeVectorToString(std::vector<double> &V,std::string separator);
    //static int scaleInt(int theValue,int currentMin,int currentMax,int targetMin,int targetMax);
    static float scaleFloatP(float value,float targetMin,float targetMax);
    static std::vector<std::string> splitStringIntoVectorOfString(std::string S,char delimiter);
    static void saveStringToFile(std::string S,std::string filename);
    static std::vector<double> linspace(double start,double end,int nrOfElements);
    static double clamp(double value,double min, double max);
};

template<typename Type>
inline std::vector<std::vector<Type>> PowersetWithoutEmpty(std::vector<Type> &V){
    std::vector<std::vector<Type>> ps;
    for(int i=1;i<(int)pow(2.0,V.size());i++){
        std::vector<Type> subSet;
        for(int j=0;j<(int)V.size();j++){
            if(((i>>j)&1)==1)
                subSet.push_back(V[j]);
        }
        ps.push_back(subSet);
    }
    return ps;
}

template<typename Type>
inline std::vector<std::vector<Type>> PowersetWithEmpty(std::vector<Type> &V){
    std::vector<std::vector<Type>> ps;
    for(int i=0;i<(int)pow(2.0,V.size());i++){
        std::vector<Type> subSet;
        for(int j=0;j<(int)V.size();j++){
            if(((i>>j)&1)==1)
                subSet.push_back(V[j]);
        }
        ps.push_back(subSet);
    }
    return ps;
}

template<typename Type>
inline int bit(Type value){
    return value > 0.0;
}

template<typename Type>
inline std::string serializeVectorToString(std::vector<Type> &V,std::string separator){
    std::stringstream result;
    std::copy(V.begin(), V.end(), std::ostream_iterator<Type>(result, separator.c_str()));
    std::string S=result.str();
    S.pop_back();
    return S;
}

template<typename Type>
inline std::string serializeVecVecIntoPythonString(std::vector<std::vector<Type>> &M){
    std::string S="\"[";
    for(int i=0;i<M.size();i++){
        if(i!=0)
            S=S+",";
        S+="["+serializeVectorToString(M[i], ",")+"]";
    }
    S+="]\"";
    return S;
}

class Image{
public:
    int image[28][28];
    Image(std::string S);
    void show();
};


#endif /* Utilities_hpp */
