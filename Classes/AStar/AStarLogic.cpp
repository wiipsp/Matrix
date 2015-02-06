#include "AStarLogic.h"


AStarLogic::AStarLogic(int** map, Point startTile, Size mapSize, std::vector<int> blocks)
{	
	this->col = mapSize.height;
	this->row = mapSize.width;
	this->map = map;
	//this->map = new int[col*row];
	//memcpy(this->map, map, col*row*sizeof(int));
	this->startTileX = startTile.x;
	this->startTileY = startTile.y;
	this->blocks = blocks;
}

bool AStarLogic::isPassCheck(){
	if(endTileX == startTileX && endTileY == startTileY){
		return false;
	}
	if (endTileX < 0 || endTileY < 0){
		return false;
	}
	if(isBlock(endTileX, endTileY)){
		return false;
	}
	return true;
}

std::vector<Point> AStarLogic::moveTo(Point endTile){
	closeList.clear();
	openList.clear();
	this->endTileX = endTile.x;
	this->endTileY = endTile.y;
	if(isPassCheck()){
		return mainProcess();
	}else{
		std::vector<Point> temp;
		return temp;
	}
}

std::vector<Point> AStarLogic::mainProcess(){
	AStarBean* startBean = AStarBean::create(startTileX, startTileY, endTileX, endTileY);
	openList.pushBack(startBean);
	std::vector<Point> path;
	while(!(startBean->getX() == endTileX && startBean->getY() == endTileY)){
		addToOpenList(startBean->getX()-1, startBean->getY(), startBean);
		addToOpenList(startBean->getX()+1, startBean->getY(), startBean);
		addToOpenList(startBean->getX(), startBean->getY()-1, startBean);
		addToOpenList(startBean->getX(), startBean->getY()+1, startBean);						
		closeList.pushBack(startBean);
		openList.eraseObject(startBean);
		//to avoid move to no way target
		if(openList.size() == 0){
			return path;
			break;
		}

		startBean = findMinBean();		
	};
	closeList.pushBack(startBean);
	
	while(!(startBean->getX() == startTileX && startBean->getY() == startTileY)){
		path.push_back(Point(startBean->getX(), startBean->getY()));
		startBean = startBean->getFatherBean();
	}
	path.push_back(Point(startBean->getX(), startBean->getY()));

	startTileX = endTileX;
	startTileY = endTileY;
	return path;
}



void AStarLogic::addToOpenList(int x, int y, AStarBean* fatherBean){
	if(x<0 || y <0 || x >= row || y >= col){
		return;
	}
	if(getInArray(closeList,x,y) != nullptr){
		return;
	}
	if(isBlock(x, y)){
		return;
	}
	AStarBean* findObject = getInArray(openList, x, y);
	if(findObject == nullptr){
		openList.pushBack(AStarBean::create(x, y, fatherBean, endTileX, endTileY));
	}else{
		findObject->changeFather(fatherBean);
	}
}

AStarBean* AStarLogic::getInArray(const Vector<AStarBean*> &list, int x, int y){
	for (auto object : list){
		if (object->getX() == x && object->getY() == y){
			return object;
		}
	}
	return nullptr;
}

bool AStarLogic::isBlock(int x, int y){
	for(std::vector<int>::iterator it = blocks.begin() ; it !=blocks.end() ; it++){
		if(map[y][x] == *it){
			return true;
		}
	}
	return false;
}

AStarBean* AStarLogic::findMinBean(){
	AStarBean* temp = openList.at(0);
	for (auto object : openList){
		if (object->getF() < temp->getF()){
			if (object->getF() < temp->getF()){
				temp = object;
			}
		}
	}
	return temp;
}

AStarLogic::~AStarLogic(void)
{
	
}
