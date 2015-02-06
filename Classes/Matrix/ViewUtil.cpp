#include "ViewUtil.h"


ViewUtil::ViewUtil(void)
{
}


ViewUtil::~ViewUtil(void)
{
}

Vec2 ViewUtil::getTheTilePos(Vec2 pos, Size size){
	Vec2 tem;
	tem.setPoint((int)(pos.x/size.width), (int)(pos.y/size.height));
	return tem;
}

Vec2 ViewUtil::getThePixPos(Vec2 pos, Size size){
	Vec2 tem;
	tem.setPoint((pos.x)*size.width, (pos.y)*size.height);
	return tem;
}

int ViewUtil::getCountFromPoint(Vec2 point, float width){
	return ((int)point.y)*width + ((int)point.x);
}

std::string ViewUtil::intToString(int a){
	std::string aa;
	std::stringstream os;
	os << a;
	aa = os.str();
	return aa;
}

std::vector<Vec2> ViewUtil::getThinPath(std::vector<Vec2> path){
	if(path.size() == 2){
		Vec2 temp = path[0];
		path[0] = path[1];
		path[1] = temp;
		return path;
	}
	std::vector<Vec2> thinPath;
	int countX = 0;
	int countY = 0;
	Vec2 temp = path[path.size()-1];
	Vec2 tempCurrent;
	thinPath.push_back(temp);
	for(int i = (path.size() - 2); i >= 0; i--){
		tempCurrent = path[i];
		if(temp.x == tempCurrent.x)
			countX++;
		else
			countX = 0;
		if(temp.y == tempCurrent.y)
			countY++;
		else
			countY = 0;		
		if(countX > 1 || countY > 1){
			thinPath.pop_back();
			thinPath.push_back(tempCurrent);			
		}else{
			thinPath.push_back(tempCurrent);
		}
		temp = tempCurrent;
	}
	return thinPath;
}