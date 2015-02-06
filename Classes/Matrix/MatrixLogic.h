#ifndef __SUPER_MATRIX_LOGIC_H__
#define __SUPER_MATRIX_LOGIC_H__
#include "cocos2d.h"
#include "TileBean.h"

USING_NS_CC;

class MatrixLogic
{
private:
	int** map;
	int width;
	int height;
	int spritSize;
	int countSize;
	int* tilesInt;
public:
	MatrixLogic(int width, int height, int spritSize);
	MatrixLogic(float width, float height, int spritSize);
	std::vector<Vec2> caculateSame(TileBean targetTile, Vec2 direction);
	std::vector<TileBean> addSpritInMap(int count);
	TileBean getSelectedTile(Vec2);
	bool isOutMap(Vec2 point);
	bool isSameValue(Vec2 point, int val);
	int** getMap(){return map;}
	int getWidth(){return width;}
	int getHeight(){return height;}
	int* getTilesInt(){return tilesInt;}
	int getCountSize(){return countSize;}
	void setCountSize(int size){this->countSize = size;}
	void reset();
	std::vector<Vec2> judgeFives(TileBean targetTile);
	~MatrixLogic();
};
#endif
