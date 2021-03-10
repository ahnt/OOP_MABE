//
//  Geometry.hpp
//  NoCompromiseMABE
//
//  Created by Arend Hintze on 3/15/20.
//  Copyright © 2020 ArendHintze. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#include <vector>
//#include <math.h>

class Point{
public:
    std::vector<double> loc;
    bool valid;
    Point(double sx, double sy){
        loc=std::vector<double>({sx,sy});
        valid=true;
    }
    Point(std::vector<double> &sLoc){
        loc=sLoc;
        valid=true;
    }
    std::vector<double> V(){
        return loc;
    }
    double x(){
        return loc[0];
    }
    double y(){
        return loc[1];
    }
    double z(){
        return loc[2];
    }
    
    static double dist(Point &A,Point &B){
        double d=0.0;
        for(int i=0;i<A.loc.size();i++){
            double delta=A.loc[i]-B.loc[i];
            d+=delta*delta;
        }
        return sqrt(d);
    }
    
    static double dist(std::vector<double> &A,std::vector<double> &B){
        double d=0.0;
        for(int i=0;i<A.size();i++){
            double delta=A[i]-B[i];
            d+=delta*delta;
        }
        return sqrt(d);
    }
    
    static double dist(double x1,double y1, double x2, double y2){
        double dx=x1-x2;
        double dy=y1-y2;
        return sqrt((dx*dx)+(dy*dy));
    }
};

class Line2D{
public:
    double Sx,Sy,Ex,Ey;
    Line2D(Point &P1,Point &P2){
        Sx=P1.x();
        Sy=P1.y();
        Ex=P2.x();
        Ey=P2.y();
    }
    
    Line2D(double x1,double y1,double x2, double y2){
        Sx=x1;
        Sy=y1;
        Ex=x2;
        Ey=y2;
    }
    
    Point intersectsLine(Line2D &other){
        Point P(0.0,0.0);
        double rTop = (Sy - other.Sy) * (other.Ex - other.Sx) - (Sx - other.Sx) * (other.Ey - other.Sy);
        double rBot = (Ex - Sx) * (other.Ey - other.Sy) - (Ey - Sy) * (other.Ex - other.Sx);
        double sTop = (Sy - other.Sy) * (Ex - Sx) - (Sx - other.Sx) * (Ey - Sy);
        double sBot = (Ex - Sx) * (other.Ey - other.Sy) - (Ey - Sy) * (other.Ex - other.Sx);
        if ((rBot == 0.0) || (sBot ==0)){
            P.valid=false;
            return P;
        }
        double r = rTop / rBot;
        double s = sTop / sBot;
        if ((r>0) && (r<1) && (s>0) && (s<1)){
            P=Point(Sx + (r * (Ex - Sx)), Sy + (r * (Ey - Sy)));
            return P;
        }
        P.valid=false;
        return P;
    }
};

#endif /* Geometry_hpp */
