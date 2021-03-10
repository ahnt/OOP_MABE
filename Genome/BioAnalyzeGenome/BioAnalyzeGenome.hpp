//
//  BioAnalyzeGenome.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 10/6/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef BioAnalyzeGenome_hpp
#define BioAnalyzeGenome_hpp

#include "../BioGenome/BioGenome.hpp"
#include <memory>
#include <vector>
#include <cstdint>

class BioAnalyzeGenome:public BioGenome{
    public:
       std::vector<std::vector<SiteType>> annotation;
       

       class BioAnalyzeReadHead: public BioGenome::BioReadHead{
       public:
           BioAnalyzeReadHead(){}
           BioAnalyzeReadHead(Genome *outer);

           virtual int getUInt32(SiteType marker=nothing) override {((BioAnalyzeGenome*)parent)->annotation[currentChromosome][currentSite]=marker; return BioGenome::BioReadHead::getUInt32(nothing);}
           virtual float getFloatP(SiteType marker=nothing) override {((BioAnalyzeGenome*)parent)->annotation[currentChromosome][currentSite]=marker; return BioGenome::BioReadHead::getFloatP(nothing);}
           virtual double getDoubleP(SiteType marker=nothing) override {((BioAnalyzeGenome*)parent)->annotation[currentChromosome][currentSite]=marker; return BioGenome::BioReadHead::getDoubleP(nothing);}
           virtual unsigned char getByte(SiteType marker=nothing) override {((BioAnalyzeGenome*)parent)->annotation[currentChromosome][currentSite]=marker; return BioGenome::BioReadHead::getByte(nothing);}
           virtual float getRangedFloat(float minInclusive, float maxInclusive, SiteType marker=nothing) override {((BioAnalyzeGenome*)parent)->annotation[currentChromosome][currentSite]=marker; return BioGenome::BioReadHead::getRangedFloat(minInclusive, maxInclusive, nothing);}
           virtual std::vector<int> getUInt32Vector(int N,SiteType marker=nothing) override {return BioGenome::BioReadHead::getUInt32Vector(N,marker);}
           virtual std::vector<double> getDoublePVector(int N, SiteType marker=nothing) override {return  BioGenome::BioReadHead::getDoublePVector(N,marker);}
           virtual std::vector<float> getFloatPVector(int N,SiteType marker=nothing) override {return BioGenome::BioReadHead::getFloatPVector(N,marker);}
           virtual std::vector<double> getRangedDoubleVector(int N,int minInclusive, int maxInclusive,SiteType marker=nothing) override {return BioGenome::BioReadHead::getRangedDoubleVector( N,minInclusive, maxInclusive,marker);}
        };
    

    static std::shared_ptr<Genome> getTemplate();
    virtual std::shared_ptr<Genome::ReadHead> getReadhead() override;
    virtual std::shared_ptr<Genome> clone() override;
    virtual std::shared_ptr<Genome> deserializeFromString(std::string S) override;
    virtual std::shared_ptr<Genome> makeMutatedCopy() override;
    
    void showAnnotation();
    int numberOfCodingSites();
    std::vector<std::pair<int,int>> getCodingSites();

};
#endif /* BioAnalyzeGenome_hpp */
