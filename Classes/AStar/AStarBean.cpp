#include "AStarBean.h"

AStarBean * AStarBean::create(int x, int y, AStarBean *fatherBean,
	int endTileX, int endTileY){
		AStarBean* astarBean = new AStarBean(x, y, fatherBean, endTileX, endTileY);
		astarBean->autorelease();
		return astarBean;
}

AStarBean * AStarBean::create(int x, int y,	int endTileX, int endTileY){
		AStarBean* astarBean = new AStarBean(x, y, nullptr, endTileX, endTileY);
		astarBean->autorelease();
		return astarBean;
}

AStarBean::AStarBean(int x, int y, AStarBean *fatherBean,
	int endTileX, int endTileY)
{
	this->x = x;
	this->y = y;

	this->fatherBean = fatherBean;

	this->endTileX = endTileX;
	this->endTileY = endTileY;
	countH();
	this->g = countG(fatherBean);
	this->f = g + h;
}

AStarBean::AStarBean(int x, int y, int endTileX, int endTileY) {
	this->x = x;
	this->y = y;

	this->fatherBean = nullptr;

	this->endTileX = endTileX;
	this->endTileY = endTileY;
	countH();
	this->g = countG(fatherBean);
	this->f = g + h;
}

AStarBean::~AStarBean(void)
{
}

void AStarBean::changeFather(AStarBean *fatherBean) {
	int g2 = countG(fatherBean);
	if (g > g2){
		this->fatherBean = fatherBean;
		g = g2;
		f = g + h;
	}
}

void AStarBean::countH(){
	h = abs(x - endTileX) + abs(y - endTileY);
}

int AStarBean::countG(AStarBean *fatherBean){
	int g;
	if (fatherBean == nullptr){
		g = 0;
		return g; 
	}
	g = fatherBean->getG() +1;
	return g;
}

void AStarBean::setWeight(int h, int g){
	this->f = g + h;
	this->h = h;
	this->g = g;
}

