//
//  BioGenome.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "BioGenome.hpp"

Site Site::randomSite(){
    Site S;
    S.i=Random::getInt32();
    return S;
}

Site Site::zeroFloatSite(){
    Site S;
    S.i=UINT32_MAX/2;
    return S;
}



std::shared_ptr<Genome> BioGenome::getTemplate(){
    std::shared_ptr<BioGenome> G=std::make_shared<BioGenome>();
    return std::dynamic_pointer_cast<Genome>(G);
}

std::shared_ptr<Genome> BioGenome::makeRandomGenome(){
    std::shared_ptr<BioGenome> bioGenome=std::make_shared<BioGenome>();
    
    bioGenome->length=Parameters::getInt(std::string("BioGenome-startLength"), 5000);
    bioGenome->chromosomes=Parameters::getInt(std::string("BioGenome-chromosomes"),1);
    bioGenome->diploid=Parameters::getBool(std::string("BioGenome-isDiploid"),false);
    int seedStartCodonsPerGene=Parameters::getInt(std::string("BioGenome-seedStartCodonsPerGene"), 10);
    if(bioGenome->diploid)
        bioGenome->genome.resize(bioGenome->chromosomes*2);
    else
        bioGenome->genome.resize(bioGenome->chromosomes);
    for(int c=0;c<(int)bioGenome->genome.size();c++){
        bioGenome->genome[c].resize(bioGenome->length);
        if(Parameters::getBool(std::string("BioGenome-seedZeroFloat"), false)){
            for(int n=0;n<bioGenome->length;n++){
                bioGenome->genome[c][n]=Site::zeroFloatSite();
            }
        } else {
            for(int n=0;n<bioGenome->length;n++){
                bioGenome->genome[c][n]=Site::randomSite();
            }
        }
        for(int t=0;t<10;t++){
            for(int n=0;n<seedStartCodonsPerGene;n++){
                int where=Random::getInt(0, (int)bioGenome->genome[c].size()-200);
                bioGenome->genome[c][where].c[0]=42+t;
                bioGenome->genome[c][where+1].c[0]=213-t;
            }
        }
    }
    //std::cout << bioGenome->length << " " << bioGenome->chromosomes << " " <<bioGenome->diploid <<std::endl;
    return std::dynamic_pointer_cast<Genome>(bioGenome);
}

std::shared_ptr<Genome> BioGenome::makeMutatedCopy(){
    std::shared_ptr<BioGenome> bioGenome=std::make_shared<BioGenome>();
    double perSitePointMutationRate=Parameters::getDouble("BioGenome-perSitePointMutationRate", 0.005);
    double perChromosomeDeletionProbability=Parameters::getDouble("BioGenome-perChromosomeDeletionProbability",0.1);
    int perDeletionMaxSize=Parameters::getInt("BioGenome-perDeletionMaxSize",512);
    int perDeletionMinSize=Parameters::getInt("BioGenome-perDeletionMinSize",128);
    int minSizeOfChromosome=Parameters::getInt("BioGenome-minSizeOfChromosome",1000);
    int maxSizeOfChromosome=Parameters::getInt("BioGenome-maxSizeOfChromosome",20000);
    double perChromosomeDuplicationProbability=Parameters::getDouble("BioGenome-perChromosomeDuplicationProbability",0.1);
    int perDuplicationMaxSize=Parameters::getInt("BioGenome-perDuplicationMaxSize",512);
    int perDuplicationMinSize=Parameters::getInt("BioGenome-perDumplicationMinSize",128);
    
    bioGenome->genome.clear();
    for(int c=0;c<(int)genome.size();c++){
        //copy the genome
        bioGenome->genome.push_back(genome[c]);
        
        //apply point mutations
        int mutations=Random::getBinomial((int)genome[c].size(), perSitePointMutationRate);
        for(int m=0;m<mutations;m++){
            bioGenome->genome[c][Random::getInt(0,(int)genome[c].size()-1)]=Site::randomSite();
        }
        
        //apply deletions
        if((Random::getFloat(0.0,1.0)<perChromosomeDeletionProbability)&&(bioGenome->genome[c].size()>minSizeOfChromosome)){
            int deletionSize=Random::getInt(perDeletionMinSize,perDeletionMaxSize);
            int where=Random::getInt(0,(int)bioGenome->genome[c].size()-deletionSize-1);
            bioGenome->genome[c].erase(bioGenome->genome[c].begin()+where,bioGenome->genome[c].begin()+where+deletionSize);
        }
        //apply duplications
        if((Random::getFloat(0.0,1.0)<perChromosomeDuplicationProbability)&&(bioGenome->genome[c].size()<maxSizeOfChromosome)){
            int duplicationSize=Random::getInt(perDuplicationMinSize,perDuplicationMaxSize);
            int fromWhere=Random::getInt(0,(int)bioGenome->genome[c].size()-duplicationSize-1);
            int intoWhere=Random::getInt(0,(int)bioGenome->genome[c].size()-1);
            std::vector<Site> piece;
            piece.assign(bioGenome->genome[c].begin()+fromWhere,bioGenome->genome[c].begin()+fromWhere+duplicationSize);
            bioGenome->genome[c].insert(bioGenome->genome[c].begin()+intoWhere,piece.begin(),piece.end());
        }
    }
    return std::dynamic_pointer_cast<Genome>(bioGenome);
}


std::shared_ptr<Genome> BioGenome::makeMutatedAndRecombinedCopy(std::shared_ptr<Genome> donor,double foreignMaterialRatio){
    std::shared_ptr<BioGenome> bioGenome=std::dynamic_pointer_cast<BioGenome>(this->makeMutatedCopy());
    auto donorReadhead=donor->getReadhead();
    auto thisReadhead=this->getReadhead();
    int skipSites=Parameters::getInt("BioGenome-recombinationSkipSites",10);
    while(!donorReadhead->eog() and !thisReadhead->eog()){
        if(Random::getFloat(0.0, 1.0)>foreignMaterialRatio){
            thisReadhead->setUInt32((std::uint32_t)donorReadhead->getUInt32());
        }
        else{
            thisReadhead->tickForward();
            donorReadhead->tickForward();
        }
        for(int i=0;i<skipSites;i++){
            thisReadhead->tickForward();
            donorReadhead->tickForward();
        }
    }
    return std::dynamic_pointer_cast<Genome>(bioGenome);
}


std::shared_ptr<Genome::ReadHead> BioGenome::getReadhead(){
    //return nullptr;
    return std::dynamic_pointer_cast<Genome::ReadHead>(std::make_shared<BioGenome::BioReadHead>((Genome*)this));
}

BioGenome::BioReadHead::BioReadHead(Genome *outer){
    parent=(BioGenome*)outer;
    reset();
}

int BioGenome::BioReadHead::getUInt32(SiteType marker){
    int i=abs((int)parent->genome[currentChromosome][currentSite].i);
    tickForward();
    return i;
}

float BioGenome::BioReadHead::getFloatP(SiteType marker){
    float f=parent->genome[currentChromosome][currentSite].f();
    tickForward();
    return f;
}

double BioGenome::BioReadHead::getDoubleP(SiteType marker){
    double d=parent->genome[currentChromosome][currentSite].d();
    tickForward();
    return d;
}

unsigned char BioGenome::BioReadHead::getByte(SiteType marker){
    unsigned char uc=parent->genome[currentChromosome][currentSite].c[0];
    tickForward();
    return uc;
}

/*
int BioGenome::BioReadHead::getRangedInt(int minInclusive, int maxInclusive){
    int i=Utilities::scaleInt(parent->genome[currentChromosome][currentSite].i, 0, UINT32_MAX, minInclusive, maxInclusive);
    std::cout<<"rnaged int broken"<<std::endl;
    exit(0);

    tickForward();
    return i;
}
 */

float BioGenome::BioReadHead::getRangedFloat(float minInclusive, float maxInclusive,SiteType marker){
    float f=Utilities::scaleFloatP(getDoubleP(), minInclusive, maxInclusive);
    return f;
}

std::vector<int> BioGenome::BioReadHead::getUInt32Vector(int N,SiteType marker){
    std::vector<int> V;
    for(int n=0;n<N;n++){
        V.push_back((int)getUInt32());
    }
    return V;
}

std::vector<float> BioGenome::BioReadHead::getFloatPVector(int N,SiteType marker){
    std::vector<float> V;
    for(int n=0;n<N;n++){
        V.push_back((float)getDoubleP());
    }
    return V;
    
}


std::vector<double> BioGenome::BioReadHead::getDoublePVector(int N,SiteType marker){
    std::vector<double> V;
    for(int n=0;n<N;n++){
        V.push_back((double)getDoubleP());
    }
    return V;
    
}


std::vector<double> BioGenome::BioReadHead::getRangedDoubleVector(int N,int minInclusive, int maxInclusive,SiteType marker){
    std::vector<double> V;
    for(int n=0;n<N;n++){
        V.push_back((double)Utilities::scaleFloatP(getDoubleP(), minInclusive,maxInclusive));
    }
    return V;
    
}

void BioGenome::BioReadHead::setUInt32(std::uint32_t value){
    parent->genome[currentChromosome][currentSite].i=value;
    tickForward();
}


void BioGenome::BioReadHead::reset(){
    currentSite=0;
    currentChromosome=0;
    eogFlag=false;
}

bool BioGenome::BioReadHead::eog(){
    return eogFlag;
}

void BioGenome::BioReadHead::tickForward(){
    if((currentSite+1)==parent->genome[currentChromosome].size()){
        currentSite=0;
        if((currentChromosome+1)==parent->genome.size()){
            eogFlag=true;
            currentChromosome=0;
        }
        else
            currentChromosome++;
    }
    else
        currentSite++;
}

std::string BioGenome::BioReadHead::locationString(){
    return std::to_string(currentChromosome)+","+std::to_string(currentSite);
}



std::string BioGenome::serializeToString(){
    std::string S="[";
    for(int c=0;c<(int)genome.size();c++){
        if(c!=0)
            S+=",";
        S+="[";
        for(int n=0;n<(int)genome[c].size();n++){
            if(n!=0)
                S+=",";
            S+=std::to_string((int)genome[c][n].i);
            //S+=std::to_string((int)genome[c][n].c[0])+",";
            //S+=std::to_string((int)genome[c][n].c[1])+",";
            //S+=std::to_string((int)genome[c][n].c[2])+",";
            //S+=std::to_string((int)genome[c][n].c[3]);
            
        }
        S+="]";
    }
    S+="]";
    return S;
}

std::shared_ptr<Genome> BioGenome::deserializeFromString(std::string S){
    std::shared_ptr<BioGenome> G=std::make_shared<BioGenome>();
    std::vector<std::string> chromosomeStrings;
    std::string chr;
    int state=0;
    for(auto C:S){
        switch(state){
            case 0: //start read
                if(C=='['){
                    chr="";
                    state=1;
                }
                break;
            case 1: //reading chromosome
                switch(C){
                    case '[':
                        chr="";
                        state=1;
                        break;
                    case ']':
                        chromosomeStrings.push_back(chr);
                        chr="";
                        state=0;
                        break;
                    default:
                        chr=chr+C;
                }
                break;
        }
    }
    /*
    std::cout<<chromosomeStrings.size()<<" "<<chromosomeStrings[0].size()<<std::endl;
    for(auto S:chromosomeStrings)
        std::cout<<S<<std::endl;
     */
    G->genome.clear();
    for(auto theChromosome:chromosomeStrings){
        auto V=Utilities::splitStringIntoVectorOfString(theChromosome,',');
        std::vector<Site> newChromosome;
        for(auto E:V)
            newChromosome.push_back(Site(atoi(E.c_str())));
        G->genome.push_back(newChromosome);
    }
    return std::dynamic_pointer_cast<Genome>(G);
}
    
std::shared_ptr<Genome> BioGenome::clone(){
    std::shared_ptr<BioGenome> G=std::make_shared<BioGenome>();
    G->length=length;
    G->chromosomes=chromosomes;
    G->diploid=diploid;
    G->genome=genome;
    return std::dynamic_pointer_cast<Genome>(G);
}

    
