//
//  Area.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/29/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef Area_hpp
#define Area_hpp

#include <vector>
#include <set>
#include <string>
#include <fstream>



struct cell{
    int x,y,what,tick;
    cell(int _x,int _y,int _what,int _tick){ x=_x; y=_y;what=_what;tick=_tick;}
};

class Area{
private:
    std::vector<std::vector<int>> area,startConfig;
    std::vector<cell> theRecord;
    int currentTick;
    int xDim,yDim;
public:
    int xm[4]={0,1,0,-1};
    int ym[4]={-1,0,1,0};
    std::vector<std::vector<std::vector<int>>> distTo;
    std::vector<std::pair<int,int>> targetLocations;

    Area(int _xDim,int _yDim,int startTile=0);
    void reset();
    void resetRecordingToFirstTick();
    void setAsStartConfig();
    void resetToStartConfig(bool clearRecorder=true);
    void set(int x,int y, int what);
    int get(int x, int y);
    int getFromDirDist(int whereX,int whereY,int dir,int dist);
    void setFromDirDist(int whereX,int whereY,int dir,int dist,int what);
    bool isSurroundedBy(int whereX,int whereY,int what);
    bool lineOfSight(double startX,double startY,double targetX,double targetY,int transparentTile);
    void tick();
    void show();
    void save(std::string filename);
    int get_xDim();
    int get_yDim();
    std::vector<std::vector<int>> get2Dmatrix(int initialValue);
    static void save2Dmatrix(std::string filename,std::vector<std::vector<int>> M);
    std::vector<std::vector<int>> computeDijkstraPath(int fromX,int fromY,int emptyTileCode);
};



#endif /* Area_hpp */
