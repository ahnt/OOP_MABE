//
//  Maze.cpp
//  NoCompromiseMABE
//
//  Created by Arend Hintze on 4/12/20.
//  Copyright © 2020 ArendHintze. All rights reserved.
//

#include "Maze.hpp"

Maze::Maze(double _startX,double _startY,double _targetX, double _targetY,std::vector<Line2D> startWalls){
    startX=_startX; startY=_startY; targetX=_targetX; targetY=_targetY;
    for(auto L:startWalls)
        walls.push_back(L);

}

void Maze::computeDistMatrix(int _xResolution,int _yResolution){
    xResolution=_xResolution; yResolution=_yResolution;
    area=std::make_shared<Area>(xResolution,yResolution,0);
    for(auto w:walls){
        int steps=200;
        double xm=(w.Ex-w.Sx)/(double)steps;
        double ym=(w.Ey-w.Sy)/(double)steps;
        for(int i=0;i<steps;i++){
            area->set(floor((w.Sx+(xm*(double)i))*(double)(xResolution-1)),floor((w.Sy+(ym*(double)i))*(double)(yResolution-1)),-1);
            area->set(ceil((w.Sx+(xm*(double)i))*(double)(xResolution-1)),ceil((w.Sy+(ym*(double)i))*(double)(yResolution-1)),-1);
        }
    }
    area->set(targetX*(xResolution-1),targetY*(yResolution-1),1);
    std::vector<std::pair<int,int>> currentPoints;
    currentPoints.push_back(std::pair<int,int>(targetX*(xResolution-1),targetY*(yResolution-1)));
    int currentDist=2;
    while(currentPoints.size()>0){
        std::vector<std::pair<int,int>> newPoints;
        for(auto p:currentPoints){
            int xm[4]={0,1,0,-1};
            int ym[4]={-1,0,1,0};
            for(int i=0;i<4;i++){
                int tx=p.first+xm[i];
                int ty=p.second+ym[i];
                if(area->get(tx,ty)==0){
                    area->set(tx,ty,currentDist);
                    newPoints.push_back(std::pair<int,int>(tx,ty));
                }
            }
        }
        currentPoints=newPoints;
        currentDist++;
    }
    for(int x=0;x<xResolution;x++)
        for(int y=0;y<yResolution;y++){
            if(area->get(x,y)==-1){
                area->set(x,y,0);
            } else {
                area->set(x,y,currentDist-area->get(x,y));
            }
        }
}

std::vector<double> Maze::rayCastDistances(double x, double y, double dir, double visionRange,std::vector<double> offsets){
    std::vector<double> I;
    for(auto angle: offsets){
        Line2D ray(x,y,x+(cos(dir+angle)*visionRange),y+(sin(dir+angle)*visionRange));
        std::vector<double> D;
        for(auto wall:walls){
            Point P=wall.intersectsLine(ray);
            if(P.valid){
                D.push_back(Point::dist(x,y,P.x(),P.y()));
            } else {
                D.push_back(visionRange);
            }
        }
        double least=*std::min_element(D.begin(),D.end());
        I.push_back(least/visionRange);
    }
    return I;
}


double Maze::getDistToTarget(double x, double y){
    return area->get(x*(xResolution-1),y*(yResolution-1));
}
