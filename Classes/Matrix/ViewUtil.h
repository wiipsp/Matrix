#ifndef __VIEW_UTIL_H__
#define __VIEW_UTIL_H__
#include "cocos2d.h"

USING_NS_CC;

class ViewUtil
{
public:
	static std::vector<Vec2> getThinPath(std::vector<Vec2> path);
	static Vec2 getTheTilePos(Vec2 pos, Size size);
	static Vec2 getThePixPos(Vec2 pos, Size size);
	static int getCountFromPoint(Vec2 point, float width);
	static std::string intToString(int a);

	ViewUtil(void);
	~ViewUtil(void);
};
#endif
