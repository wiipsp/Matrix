#ifndef __ASTAR_H__
#define __ASTAR_H__
#include "cocos2d.h"
#include <math.h>

class AStarBean : public cocos2d::Ref
{
private:
	int x,y,f,h,g;
	int endTileX,endTileY; //the destionation x and y
	AStarBean * fatherBean;

	void countH();	
	int countG(AStarBean* fatherBean);
public:
	static AStarBean* create(int x, int y, AStarBean *fatherBean,
		int endTileX, int endTileY);
	static AStarBean* create(int x, int y, 
		int endTileX, int endTileY);
	AStarBean(int x, int y, AStarBean *fatherBean,
			int endTileX, int endTileY);
	AStarBean(int x, int y, 
			int endTileX, int endTileY);
	~AStarBean(void);
	void changeFather(AStarBean* fatherBean);
	void setWeight(int h, int g);

	AStarBean* getFatherBean(){return this->fatherBean;}
	int getG(){return this->g;}
	int getH(){return this->h;}
	int getF(){return this->f;}
	int getX(){return this->x;}
	int getY(){return this->y;}
};

#endif