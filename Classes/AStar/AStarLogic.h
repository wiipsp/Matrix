#ifndef __ASTAR_LOGIC_H__
#define __ASTAR_LOGIC_H__
#include "cocos2d.h"
#include "AStarBean.h"
#include <math.h>
#include<vector>
USING_NS_CC;

class AStarLogic
{
private:
	int **map; //the map of AStar
	int row,col;
	int startTileX;
	int startTileY;
	int endTileX;
	int endTileY;
	std::vector<int> blocks;
	Vector<AStarBean*> openList;
	Vector<AStarBean*> closeList;
	bool isPassCheck();
public:
	AStarLogic(int **map, Vec2 startTile, Size size, std::vector<int> blocks);

	/** 
	*move to the end point and return the path.
	*/
	std::vector<Vec2> moveTo(Vec2 endTile);
	//for sm srtart
	void setStartTile(Vec2 startTile){this->startTileX = startTile.x;this->startTileY = startTile.y;}
	void setMap(int** map){this->map = map;}

	//for sm end
	std::vector<Vec2> mainProcess();
	/**
	* to decide whether the pint(x,y) can add to the open list
	*/
	void addToOpenList(int x, int y, AStarBean* fatherBean);

	bool isBlock(int x, int y);

	AStarBean* findMinBean();
	~AStarLogic(void);
	AStarBean* getInArray(const Vector<AStarBean*> &list, int x, int y);
};

#endif
