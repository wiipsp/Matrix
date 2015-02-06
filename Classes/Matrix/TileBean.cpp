#include "TileBean.h"

USING_NS_CC;

TileBean::TileBean(Vec2 location, int val)
{
	this->location = location;
	this->val = val;
}

TileBean::TileBean(){
}

TileBean::~TileBean()
{
}

void TileBean::setLocation(Vec2 location){
	this->location = location;
}

void TileBean::setVal(int val){
	this->val = val;
}

Vec2 TileBean::getLocation(){
	return location;
}

int TileBean::getVal(){
	return val;
}