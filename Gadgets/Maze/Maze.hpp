//
//  Maze.hpp
//  NoCompromiseMABE
//
//  Created by Arend Hintze on 4/12/20.
//  Copyright © 2020 ArendHintze. All rights reserved.
//

#ifndef Maze_hpp
#define Maze_hpp

#include <vector>
#include <math.h>
#include <stdio.h>
#include <memory>

#include "../../Core/Utilities.hpp"
#include "../../Core/Geometry.hpp"
#include "../../Core/Area.hpp"

class Maze{
    std::vector<Line2D> walls;
    std::shared_ptr<Area> area;
    std::vector<std::vector<double>> distMatrix;
    double startX,startY,targetX,targetY;
    int xResolution,yResolution;
public:
    Maze(double _startX,double _startY,double _targetX, double _targetY,std::vector<Line2D> startWalls);
    void computeDistMatrix(int _xResolution,int _yResolution);
    std::vector<double> rayCastDistances(double x, double y, double dir, double visionRange,std::vector<double> offsets);
    double getDistToTarget(double x, double y);
};

#endif /* Maze_hpp */
