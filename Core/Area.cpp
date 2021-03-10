//
//  Area.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/29/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//


#include "Area.hpp"

Area::Area(int _xDim,int _yDim,int startTile){
    area.resize(_xDim);
    for(int i=0;i<_xDim;i++){
        area[i].resize(_yDim);
        for(int j=0;j<_yDim;j++)
            area[i][j]=startTile;
    }
    theRecord.clear();
    currentTick=0;
    xDim=_xDim;
    yDim=_yDim;
}

void Area::reset(){
    for(int i=0;i<(int)area.size();i++){
        for(int j=0;j<(int)area[i].size();j++)
            area[i][j]=0;
    }
    theRecord.clear();
    currentTick=0;
}
void Area::set(int x,int y, int what){
    area[x][y]=what;
    theRecord.push_back(cell(x,y,what,currentTick));
};

int Area::get(int x, int y){
    return area[x][y];
}

void Area::show(){
    //observe that we indeed print X Y!
    for(int i=0;i<area.size();i++){
        for(int j=0;j<area[i].size();j++){
            printf("%i ",area[j][i]);
        }
        printf("\n");
    }
}

void Area::save(std::string filename){
    std::ofstream F;
    F.open(filename);
    F<<"t,x,y,what"<<std::endl;
    for(int i=0;i<(int)theRecord.size();i++){
        F<<theRecord[i].tick<<","<<theRecord[i].x<<","<<theRecord[i].y<<","<<theRecord[i].what<<std::endl;
    }
    F.close();
}
void Area::tick(){
    currentTick++;
}
bool Area::isSurroundedBy(int whereX,int whereY,int what){
    for(int i=0;i<4;i++)
        if(area[whereX+xm[i]][whereY+ym[i]]!=what)
            return false;
    return true;
}

int Area::getFromDirDist(int whereX,int whereY,int dir,int dist){
    return area[whereX+(xm[dir]*dist)][whereY+(ym[dir]*dist)];
}

void Area::setFromDirDist(int whereX,int whereY,int dir,int dist,int what){
    set(whereX+(xm[dir]*dist),whereY+(ym[dir]*dist),what);
}


bool Area::lineOfSight(double startX,double startY,double targetX,double targetY,int transparentTile){
    int steps=(int)area.size();
    double xSlope=(targetX-startX)/(double)steps;
    double ySlope=(targetY-startY)/(double)steps;
    for(int step=0;step<steps;step++){
        int X=startX+xSlope*step;
        int Y=startY+ySlope*step;
        if (!((X==startX)&&(Y==startY))&&!((X==targetX)&&(Y==targetY))){
            if(area[X][Y]!=transparentTile)
                return false;
        }
    }
    return true;
}

void Area::resetRecordingToFirstTick(){
    int i=0;
    while( (i<theRecord.size()) && (theRecord[i].tick==0))
        i++;
    theRecord.erase(theRecord.begin()+i,theRecord.end());
    currentTick=1;
}


void Area::setAsStartConfig(){
    startConfig=area;
}

void Area::resetToStartConfig(bool clearRecorder){
    if(clearRecorder)
        resetRecordingToFirstTick();
    for(int i=0;i<(int)area.size();i++)
        for(int j=0;j<(int)area[i].size();j++)
            set(i,j,startConfig[i][j]);
}

int Area::get_xDim(){
    return xDim;
}

int Area::get_yDim(){
    return yDim;
}

std::vector<std::vector<int>> Area::get2Dmatrix(int initialValue){
    std::vector<std::vector<int>> M;
    M.resize(xDim);
    for(int x=0;x<xDim;x++){
        M[x].resize(yDim);
        for(int y=0;y<yDim;y++){
            M[x][y]=initialValue;
        }
    }
    return M;
}

void Area::save2Dmatrix(std::string filename,std::vector<std::vector<int>> M){
    std::ofstream F;
    F.open(filename);
    F<<"[";
    for(int x=0;x<M.size();x++){
        if(x!=0)
            F<<",";
        F<<"[";
        for(int y=0;y<M[x].size();y++){
            if(y!=0)
                F<<",";
            F<<std::to_string(M[x][y]);
        }
        F<<"]";
    }
    F<<"]\n";
    F.close();
}

std::vector<std::vector<int>> Area::computeDijkstraPath(int fromX,int fromY,int emptyTileCode){
    std::vector<std::vector<int>> dist;
    std::set<std::pair<int,int>> currentLocations,nextLocations;
    int currentDist=0;
    dist.resize(area.size());
    for(int i=0;i<(int)dist.size();i++)
        dist[i].assign(area[i].size(),-1);
    currentLocations.insert(std::pair<int,int>(fromX,fromY));
    do{
        nextLocations.clear();
        for(auto loc:currentLocations){
            dist[loc.first][loc.second]=currentDist;
            for(int i=0;i<4;i++)
                if((area[loc.first+xm[i]][loc.second+ym[i]]==emptyTileCode)&&(dist[loc.first+xm[i]][loc.second+ym[i]]==-1))
                    nextLocations.insert(std::pair<int,int>(loc.first+xm[i],loc.second+ym[i]));
        }
        currentLocations=nextLocations;
        currentDist++;
    } while(currentLocations.size()>0);
    return dist;
}
