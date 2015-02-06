#ifndef __TILE_BEAN_H__
#define __TILE_BEAN_H__
#include "cocos2d.h"

USING_NS_CC;

class TileBean
{
private:
    Vec2 location;
	int val;
public:
	//static TileBean* create(Vec2 location, int val);
	void setLocation(Vec2 location);
	void setVal(int val);
	Vec2 getLocation();
	int getVal();
	TileBean();
	TileBean(Vec2 location, int val);
	~TileBean();
};
#endif