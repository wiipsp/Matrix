#ifndef __MATRIX_SCENE_H__
#define __MATRIX_SCENE_H__
#include "cocos2d.h"
//#include "SimpleAudioEngine.h"
#include "MatrixLogic.h"
#include "AStar/AStarLogic.h"
#include "ViewUtil.h"
#include "base/CCVector.h"
//#include "NoTouchLayer.h"

#define SCALE_SIZE 1.0f
#define SCALE_TO   0.8f
#define SaveStringToXML UserDefault::getInstance()->setStringForKey

#define SaveIntegerToXML UserDefault::getInstance()->setIntegerForKey

#define SaveBooleanToXML UserDefault::getInstance()->setBoolForKey

#define LoadStringFromXML UserDefault::getInstance()->getStringForKey

#define LoadIntegerFromXML UserDefault::getInstance()->getIntegerForKey

#define LoadBooleanFromXML UserDefault::getInstance()->getBoolForKey


class SuperMatrix :	public cocos2d::Layer
{
private:
	MatrixLogic* matrixLogic;
	AStarLogic* astar;
	bool isSelectOne;
	TileBean selectedTile;
	Sprite* sprite;
	int scores;
public:

	SuperMatrix();
	~SuperMatrix();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init() override;
	virtual void onEnter() override;

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::Scene* createScene();

	// a selector callback
	void menuCloseCallback(Ref* pSender);
	void menuCallback(Ref* pSender);
	void stopScaleAction(Node* pSender);
	void restart();
	Vec2 getConvertPos(TileBean tile);
	void displayMap(std::vector<TileBean> addTiles);

	Sprite* setActionToSelectTile(Vec2 tilePos);
	Vector<FiniteTimeAction*> getActionArray(std::vector<Vec2> path);
	bool runMatrixLogic(Node* pSender);
	void runMatrixLogicAndDisplay(Node* pSender);
	void fiveSuccess(std::vector<Vec2> points);
	void removeTileAndUpMap(Node* pSender);
	void countScore(std::vector<Vec2> points);


	CREATE_FUNC(SuperMatrix);
};
#endif
