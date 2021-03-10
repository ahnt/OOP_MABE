//
//  BioAnalyzeGenome.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 10/6/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "BioAnalyzeGenome.hpp"

std::shared_ptr<Genome> BioAnalyzeGenome::getTemplate(){
    std::shared_ptr<BioAnalyzeGenome> G=std::make_shared<BioAnalyzeGenome>();
    return std::dynamic_pointer_cast<Genome>(G);
}


BioAnalyzeGenome::BioAnalyzeReadHead::BioAnalyzeReadHead(Genome *outer){
    parent=(BioGenome*)outer;
    reset();
}

std::shared_ptr<Genome::ReadHead> BioAnalyzeGenome::getReadhead(){
    annotation.resize(genome.size());
    for(int i=0;i<(int)genome.size();i++)
        annotation[i].assign(genome[i].size(),nothing);
    return std::dynamic_pointer_cast<Genome::ReadHead>(std::make_shared<BioAnalyzeGenome::BioAnalyzeReadHead>((Genome*)this));
}

std::shared_ptr<Genome> BioAnalyzeGenome::clone(){
    std::shared_ptr<BioAnalyzeGenome> G=std::make_shared<BioAnalyzeGenome>();
    G->length=length;
    G->chromosomes=chromosomes;
    G->diploid=diploid;
    G->genome=genome;
    G->annotation=annotation;
    return std::dynamic_pointer_cast<Genome>(G);

}

void BioAnalyzeGenome::showAnnotation(){
    for(int c=0;c<(int)annotation.size();c++)
        for(int s=0;s<(int)annotation[c].size();s++)
            std::cout<<(int)genome[c][s].c[0]<<"\t"<< annotation[c][s]<<std::endl;
}

int BioAnalyzeGenome::numberOfCodingSites(){
    return (int)getCodingSites().size();
}

std::shared_ptr<Genome> BioAnalyzeGenome::deserializeFromString(std::string S){
    std::shared_ptr<BioAnalyzeGenome> G=std::make_shared<BioAnalyzeGenome>();
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

std::vector<std::pair<int,int>> BioAnalyzeGenome::getCodingSites(){
    std::vector<std::pair<int,int>> r;
    for(int c=0;c<(int)annotation.size();c++)
        for(int s=0;s<(int)annotation[c].size();s++)
            if(annotation[c][s]==coding)
                r.push_back(std::pair<int,int>(c,s));
    return r;
}

std::shared_ptr<Genome> BioAnalyzeGenome::makeMutatedCopy(){
    std::shared_ptr<BioAnalyzeGenome> newGenome=std::make_shared<BioAnalyzeGenome>();
    newGenome->genome=genome;
    std::vector<std::pair<int,int>> sites=getCodingSites();
    std::pair<int,int> whichSite=sites[Random::getInt(0, (int)sites.size()-1)];
    newGenome->genome[whichSite.first][whichSite.second]=Site::randomSite();
    return std::dynamic_pointer_cast<Genome>(newGenome);
}
