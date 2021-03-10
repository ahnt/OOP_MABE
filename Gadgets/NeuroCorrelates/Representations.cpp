//
//  Representations.cpp
//  NoCompromiseMABE
//
//  Created by Arend Hintze on 4/8/20.
//  Copyright © 2020 ArendHintze. All rights reserved.
//

#include "Representations.hpp"


double median(std::vector<double> &v){
    size_t n = v.size() / 2;
    std::nth_element(v.begin(), v.begin()+n, v.end());
    return v[n];
}

std::map<int,double> Representations::computeSymbolCount(std::vector<int> &randomVariable){
    std::map<int,double> collector;
    double quantile=1.0/(double)randomVariable.size();
    for(auto element:randomVariable){
        if(collector.find(element)==collector.end())
            collector[element]=0.0;
        collector[element]+=quantile;
    }
    return collector;
}

double Representations::computeEntropy(std::map<int,double> &symbolCount){
    double H=0.0;
    for(auto it:symbolCount){
        H+=it.second*log2(it.second);
    }
    return -1.0*H;
}

void Representations::normalizeSymbolCount(std::map<int,double> &symbolCount){
    double sum=0.0;
    for(auto it:symbolCount)
        sum+=it.second;
    for(auto it:symbolCount){
        it.second/=sum;
    }
}

std::vector<int> Representations::extractBitPattern(std::vector<std::vector<int>> &measurements,std::vector<int> bitPattern){
    std::vector<int> ret;
    for(auto row:measurements){
        int value=0;
        for(auto i:bitPattern)
            value+=row[i]<<i;
        ret.push_back(value);
    }
    return ret;
}

double Representations::computeR(std::vector<int> world,std::vector<int> brain,std::vector<int> sensors){

    auto sensorBrainRV=joinRandomVariables(sensors,brain);
    auto sensorWorldRV=joinRandomVariables(sensors, world);
    auto worldSensorBrainRV=joinRandomVariables(sensorWorldRV, brain);
    auto sensorPD=computeSymbolCount(sensors);
    auto sensorBrainPD=computeSymbolCount(sensorBrainRV);
    auto sensorWorldPD=computeSymbolCount(sensorWorldRV);
    auto worldSensorBrainPD=computeSymbolCount(worldSensorBrainRV);
    return computeEntropy(sensorBrainPD) + computeEntropy(sensorWorldPD) - computeEntropy(sensorPD) - computeEntropy(worldSensorBrainPD);
}

double Representations::computeExclusiveR(std::vector<int> W,std::vector<int> O, std::vector<int> T, std::vector<int> S){
    auto jointOS=joinRandomVariables(O, S);
    auto jointOSW=joinRandomVariables(jointOS, W);
    auto jointOST=joinRandomVariables(jointOS, T);
    auto jointOSTW=joinRandomVariables(jointOST, W);
    auto OS=computeSymbolCount(jointOS);
    auto OSW=computeSymbolCount(jointOSW);
    auto OST=computeSymbolCount(jointOST);
    auto OSTW=computeSymbolCount(jointOSTW);
    return -computeEntropy(OS)+computeEntropy(OSW)+computeEntropy(OST)-computeEntropy(OSTW);
}

std::vector<std::vector<int>> Representations::medianBinarization(std::vector<std::vector<double>> &data,std::vector<int> thresholdColumns,std::vector<double> thresholds){
    std::vector<double> medians;
    for(int i=0;i<(int)data[0].size();i++){
        std::vector<double> column;
        for(int j=0;j<(int)data.size();j++)
            column.push_back(data[j][i]);
        medians.push_back(median(column));
    }
    for(int i=0;i<(int)thresholdColumns.size();i++){
        medians[thresholdColumns[i]]=thresholds[i];
    }
    std::vector<std::vector<int>> ret;
    for(auto row:data){
        std::vector<int> newRow;
        for(int e=0;e<(int)row.size();e++){
            newRow.push_back(row[e]>=medians[e]);
        }
        ret.push_back(newRow);
    }
    return ret;
}

std::vector<int> Representations::joinRandomVariables(std::vector<int> &R1,std::vector<int> &R2){
    std::vector<int> ret;
    for(int i=0;i<(int)R1.size();i++){
        ret.push_back(R1[i]|R2[i]);
    }
    return ret;
}
