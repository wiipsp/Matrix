#ifndef __ASTAR_VIEW_H__
#define __ASTAR_VIEW_H__
#include "cocos2d.h"
#include "Matrix/Player.h"
#include "AStar/AStarLogic.h"
#include "Matrix/ViewUtil.h"
USING_NS_CC;

class AStarView : public Layer
{
private:
	Player* player;
	AStarLogic* astar;
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static Scene* createScene();

	~AStarView();

	// a selector callback
	void menuCloseCallback(Ref* pSender);

	bool touchBegan(Touch* touch, Event* event);
	void touchMoved(Touch* touch, Event* event);
	void touchEnded(Touch* touch, Event* event);
	void changeSpriteDir(Vec2 absPo);
	Vector<FiniteTimeAction*> getActionArray(std::vector<Point> path);

	//get the position that make sprite in center and have no outside
	Point getRightPoint(Size mapSize, Size winSize, Point spritePos);

	void update(float a);
	int** getMapMatrix(TMXTiledMap* map);
	// implement the "static node()" method manually
	CREATE_FUNC(AStarView);
};

#endif