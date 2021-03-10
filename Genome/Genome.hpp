//
//  Genome.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef Genome_hpp
#define Genome_hpp

#include <memory>
#include "../Core/Parameters.hpp"
#include <cfloat>
#include "../Core/Random.hpp"
#include "../Core/Utilities.hpp"

enum SiteType { nothing=0, coding=1};
class Genome{
    public:
    class ReadHead{
        public:
        ReadHead(){};
        ReadHead(Genome *outer){};
        virtual int getUInt32(SiteType marker=nothing){return 0;}
        virtual float getFloatP(SiteType marker=nothing) {return 0;}
        virtual double getDoubleP(SiteType marker=nothing) {return 0;}
        virtual unsigned char getByte(SiteType marker=nothing) {return 0;}
        //virtual int getRangedInt(int minInclusive, int maxInclusive) {return 0;}
        virtual float getRangedFloat(float minInclusive, float maxInclusive, SiteType marker=nothing) {return 0;}
        virtual std::vector<int> getUInt32Vector(int N, SiteType marker=nothing) {return std::vector<int>({0});}
        virtual std::vector<double> getDoublePVector(int N, SiteType marker=nothing){return std::vector<double>({0.0});}
        virtual std::vector<float> getFloatPVector(int N, SiteType marker=nothing){return std::vector<float>({0.0});}
        //virtual std::vector<int> getRangedIntVector(int N,int minInclusive, int maxInclusive) {return std::vector<int>({0});}
        virtual std::vector<double> getRangedDoubleVector(int N,int minInclusive, int maxInclusive, SiteType marker=nothing){return std::vector<double>({0.0});}
        
        virtual void setUInt32(std::uint32_t value){}
        
        virtual void reset() {}
        virtual bool eog() {return false;}
        virtual std::string locationString(){ return "";}
        virtual void tickForward(){}
    };
    Genome(){};

    static std::shared_ptr<Genome> getTemplate();//{ return std::make_shared<Genome>(); }
    virtual std::shared_ptr<Genome> deserializeFromString(std::string S);
    virtual std::shared_ptr<Genome> makeRandomGenome();
    virtual std::shared_ptr<Genome> makeMutatedCopy();
    virtual std::shared_ptr<Genome> makeMutatedAndRecombinedCopy(std::shared_ptr<Genome> donor,double foreignMaterialRatio=0.5);
    virtual std::shared_ptr<Genome::ReadHead> getReadhead();
    virtual std::string serializeToString(){ return std::string("GENOME class <should not be called!>\n");}
    virtual std::shared_ptr<Genome> clone(){ return std::make_shared<Genome>(); }
};

#endif /* Genome_hpp */
