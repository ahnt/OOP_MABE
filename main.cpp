//
//  main.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "Core/Population.hpp"
#include "Brain/MarkovBrain/MarkovBrain.hpp"
#include "Brain/DeepMarkovBitBrain/DeepMarkovBitBrain.hpp"
#include "Brain/GRNBrain/GRNBrain.hpp"
#include "Genome/BioGenome/BioGenome.hpp"
#include "World/TestWorld/TestWorld.hpp"
#include "World/BlockyCatchWorld/BlockyCatchWorld.hpp"
#include "World/HideAndSeekWorld/HideAndSeekWorld.hpp"
#include "World/EdlundMazeWorld/EdlundMazeWorld.hpp"
#include "World/CarWorld/CarWorld.hpp"
#include "World/ExampleWorld/ExampleWorld.hpp"
#include "World/TheoryOfMindWorld/TheoryOfMindWorld.hpp"
#include "World/OFAAFOWorld/OFAAFOWorld.hpp"
#include "World/PendulumWorld/PendulumWorld.hpp"
#include "World/SymbolicRegressorWorld/SymbolicRegressorWorld.hpp"
#include "Optimizer/GeneticAlgorithm/GeneticAlgorithm.hpp"
#include "Core/LODmanager.hpp"

int main(int argc, const char * argv[]) {
    //necessary Items, do not change
    Parameters::initialize(argc, argv);
    Random::seedRandom();
    
    //the world needs to be instantiated first, otherwise the input/output buffer sizes would not be known
    std::shared_ptr<World> world=std::dynamic_pointer_cast<World>(std::make_shared<OFAAFOWorld>());

    
    //std::shared_ptr<World> world=std::dynamic_pointer_cast<World>(std::make_shared<OFAAFOWorld>());
    //std::shared_ptr<World> world=std::dynamic_pointer_cast<World>(std::make_shared<ExampleWorld>());

    if(Parameters::getBool(std::string("World-analyzeMode"), false)){
        auto population=Population::loadPopulation(Parameters::getString("fileNameToLoad","GEN_example.csv"), BioGenome::getTemplate(), MarkovBrain::getTemplate(),world->nrOfSensors(),world->nrOfMotors());
        //run either in analyse mode
        world->evaluate(population,true,Parameters::getBool(std::string("World-visualize"),false));//population,Parameters::getBool(std::string("World-analyzeMode"), false),false);
        //different things can be done here
        //for(int i=0;i<10000;i++)
        //    std::cout<<population->population[i]->dataCollector->getDouble("score")<<std::endl;
        //population->population[0]->brain->show();
    } else {
        auto population=Population::createStartPopulation(Parameters::getInt(std::string("Core-numberOfStartOrganisms"), 100),
                                                          BioGenome::getTemplate(), // default genome
                                                          MarkovBrain::getTemplate(), //standard Markov Brain
                                                          world->nrOfSensors(),world->nrOfMotors());
        //or in evolve mode
        //an LOD manager is needed
        std::shared_ptr<LODManager> LM=std::make_shared<LODManager>("");
        //an optimizer is needed:
        auto optimizer=std::make_shared<GeneticAlgorithm>();
        int nrOfGenerationsToEvolve=Parameters::getInt(std::string("Core-nrOfGenerationsToEvolve"),5000);
        //main evolution loop
        for(Utilities::currentGeneration=0;Utilities::currentGeneration<nrOfGenerationsToEvolve;Utilities::currentGeneration++){
            //evaluate the population in the world
            world->evaluate(population,Parameters::getBool(std::string("World-analyzeMode"), false),Parameters::getBool("World-visualize", false));
            //call the optimizer to get the next generation
            population=optimizer->optimize(population);
            //call the LOD manager
            LM->performIntermediaryCheck(population);
        }
        LM->wrapUp(population);
        //LM->savePop(population, Parameters::getString("POPfilename", "POPfile.csv"));
    }
    return 0;
}
