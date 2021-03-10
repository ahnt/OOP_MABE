//
//  BioGenome.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef BioGenome_hpp
#define BioGenome_hpp

#include "../Genome.hpp"
#include <memory>
#include <vector>
#include <cstdint>

union Site{
    std::uint32_t i;
    unsigned char c[4];
    static Site randomSite();
    static Site zeroFloatSite();
    Site()=default;
    Site(int I){ i=(std::int32_t)I;};
    double d(){ return (double)i/(double)UINT32_MAX;}
    float f(){return (float)i/(float)UINT32_MAX;}
};

class BioGenome:public Genome{
    public:
    std::vector<std::vector<Site>> genome;
    int length,chromosomes;
    bool diploid;
    

    class BioReadHead: public Genome::ReadHead{
    public:
        bool eogFlag;
        virtual void tickForward() override;
        BioGenome *parent;
        
        BioReadHead(){};
        BioReadHead(Genome *outer);
        int currentSite,currentChromosome;
        virtual int getUInt32(SiteType marker=nothing) override;
        virtual float getFloatP(SiteType marker=nothing) override;
        virtual double getDoubleP(SiteType marker=nothing) override;
        virtual unsigned char getByte(SiteType marker=nothing) override;
        virtual float getRangedFloat(float minInclusive, float maxInclusive, SiteType marker=nothing) override;
        virtual std::vector<int> getUInt32Vector(int N,SiteType marker=nothing) override;
        virtual std::vector<double> getDoublePVector(int N, SiteType marker=nothing) override;
        virtual std::vector<float> getFloatPVector(int N,SiteType marker=nothing) override;
        virtual std::vector<double> getRangedDoubleVector(int N,int minInclusive, int maxInclusive,SiteType marker=nothing) override;
              
        virtual void setUInt32(std::uint32_t value) override;

        virtual void reset() override;
        virtual bool eog() override;
        virtual std::string locationString() override;

    };
    BioGenome(){}

    static std::shared_ptr<Genome> getTemplate();
    virtual std::shared_ptr<Genome> deserializeFromString(std::string S) override;

    virtual std::shared_ptr<Genome> makeRandomGenome() override;
    virtual std::shared_ptr<Genome> makeMutatedCopy() override;
    virtual std::shared_ptr<Genome> makeMutatedAndRecombinedCopy(std::shared_ptr<Genome> donor,double foreignMaterialRatio=0.5) override;

    virtual std::shared_ptr<Genome::ReadHead> getReadhead() override;
    std::string serializeToString() override;
    
    virtual std::shared_ptr<Genome> clone() override;

};

#endif /* BioGenome_hpp */
