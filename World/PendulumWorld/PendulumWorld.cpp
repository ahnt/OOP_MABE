//
//  ExampleWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "PendulumWorld.hpp"
#include "limits.h"
#define PI 3.14159265359
#include <cmath>

class Pendulum{
public:
    double gravity,masscart,masspole,total_mass,length,polemass_length,force_mag,tau;
    double state[4],high[4];
    double theta_threshold_radians;
    double x_threshold;
    
    Pendulum(){
        gravity = 9.8;
        masscart = 1.0;
        masspole = 0.1;
        total_mass = masspole +masscart;
        length = 0.5;// # actually half the pole's length
        polemass_length = masspole * length;
        force_mag = 10.0;
        tau = 0.02;//  # seconds between state updates
        theta_threshold_radians = 12.0 * 2.0 * PI / 360.0;
        x_threshold = 2.4;
        // Angle limit set to 2 * theta_threshold_radians so failing observation is still within bounds
        high[0]=x_threshold*2.0;
        high[1]=std::numeric_limits<double>::max();
        high[2]=theta_threshold_radians * 2.0;
        high[3]=high[1];

    }
    ~Pendulum(){}
    
    void resetState(){
        for(int i=0;i<4;i++)
            state[i]=Random::getFloat(-0.05, 0.05);
    }
    void step(double action){
        double x, x_dot, theta, theta_dot;
        x=state[0]; x_dot=state[1]; theta=state[2]; theta_dot=state[3];
        double force=force_mag*action;
        double costheta = cos(theta);
        double sintheta = sin(theta);
        double temp = (force + polemass_length * theta_dot * theta_dot * sintheta) / total_mass;
        double thetaacc = (gravity * sintheta - costheta* temp) / (length * (4.0/3.0 - masspole * costheta * costheta / total_mass));
        double xacc  = temp - polemass_length * thetaacc * costheta / total_mass;
        x  = x + tau * x_dot;
        x_dot = x_dot + tau * xacc;
        theta = theta + tau * theta_dot;
        theta_dot = theta_dot + tau * thetaacc;
        state[0] = x; state[1]=x_dot; state[2]=theta; state[3]=theta_dot;
    }
    bool done(){
        //done =  x < -self.x_threshold \
        //or x > self.x_threshold \
        //or theta < -self.theta_threshold_radians \
        //or theta > self.theta_threshold_radians
        if ((state[0] < -x_threshold) || (state[0] > x_threshold) || (state[2]<-theta_threshold_radians) || (state[2]>theta_threshold_radians))
            return true;
        return false;
    }
};

PendulumWorld::PendulumWorld(){
}



void PendulumWorld::evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize){
    auto brain = organism->brain;
    Pendulum *P=new Pendulum();
    std::vector<double> answers;
    double score=0.0;
    for(int reps=0;reps<10;reps++){
        P->resetState();
        int t=0;
        brain->resetBrain();
        answers.clear();
        while((t<500)&&(!P->done())){
            for(int i=0;i<8;i++){
                for(int j=0;j<4;j++)
                    brain->setInput(j, P->state[j]);
                brain->update();
            }
            /*
            for(auto node:dynamic_pointer_cast<MarkovBrain>(org->brain)->nodes)
                printf("%f ",node);
            printf("\n");
             //*/
            double action=brain->getOutput(0);
            if((isnan(action))||(isinf(action)))
               P->step(0.0);
           else
               P->step(action);
            t++;
            answers.push_back(action);
        }
        score+=(double)t;
    }
    organism->dataCollector->setValue("score",score/10.0);//pow(1.00,score/10.0));
}

int PendulumWorld::nrOfSensors(){
    return 4;
}

int PendulumWorld::nrOfMotors(){
    return 1;
}
