#include "MatrixScene.h"
#include "NoTouchLayer.h"

USING_NS_CC;

enum {
	kTagScore=-1,
	kTagTileMap,
	kTagTileSprite,
	kTagTileSpriteSheet,
	kTagTileLabel,
	kTagTileHighLabel,
	kTagSpriteAct,
	kTagSpriteSeq,
	kTagActionInterval
	
};

SuperMatrix::SuperMatrix()
{
	//super();
}


SuperMatrix::~SuperMatrix()
{
	delete astar;
	delete matrixLogic;
}


Scene* SuperMatrix::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SuperMatrix::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

int getHightScoreStr(){
	if ( !LoadBooleanFromXML("_IS_EXISTED"))
	{  
		SaveIntegerToXML("highScoreString",0);
		SaveBooleanToXML("_IS_EXISTED", true);  
		UserDefault::getInstance()->flush();
	} 
	return LoadIntegerFromXML("highScoreString");
}

void SuperMatrix::onEnter(){
	Layer::onEnter();
	// 创建一个事件监听器 OneByOne 为单点触摸
	auto listener1 = EventListenerTouchOneByOne::create();
	// 设置是否吞没事件，在 onTouchBegan 方法返回 true 时吞没
	listener1->setSwallowTouches(true);



	// 使用 lambda 实现 onTouchBegan 事件回调函数
	listener1->onTouchEnded = [this](Touch* touch, Event* event){
				
		TMXTiledMap* map = (TMXTiledMap*)getChildByTag(kTagTileMap);
		SpriteBatchNode* spriteSheet = (SpriteBatchNode*)getChildByTag(kTagTileSpriteSheet);

		Vec2 touchPoint = map->convertTouchToNodeSpace(touch);
		Vec2 tilePos = ViewUtil::getTheTilePos(touchPoint, map->getTileSize());
		TileBean selectTile = matrixLogic->getSelectedTile(tilePos);

		if (selectTile.getVal() == -1 || (isSelectOne && tilePos.x == selectedTile.getLocation().x && tilePos.y == selectedTile.getLocation().y)){
			return;
		}
		if (sprite != nullptr && (sprite->getActionByTag(kTagSpriteSeq) != nullptr || sprite->getActionByTag(kTagActionInterval) != nullptr)){//to check the action is or not run if yes then return 
			return;
		}

		if (!isSelectOne && selectTile.getVal() != 0){//first step select one tile
			isSelectOne = true;
			sprite = setActionToSelectTile(tilePos);
			astar->setStartTile(tilePos);
			selectedTile = selectTile;
		}
		else if (isSelectOne && selectTile.getVal() == 0){// the first tile is selected and the sec one is null
			std::vector<Vec2> path = astar->moveTo(tilePos);
			if (path.size() < 2){
				return;
			}
			//updatet the map;
			int** p = matrixLogic->getMap();
			p[(int)selectedTile.getLocation().y][(int)selectedTile.getLocation().x] = 0;
			p[(int)tilePos.y][(int)tilePos.x] = selectedTile.getVal();
			//update the tilesInt
			int* tilesInt = matrixLogic->getTilesInt();
			tilesInt[ViewUtil::getCountFromPoint(tilePos, map->getMapSize().width)] = 0;
			tilesInt[ViewUtil::getCountFromPoint(selectedTile.getLocation(), map->getMapSize().width)] = 1;
			//update the sprite tag
			sprite->setTag(ViewUtil::getCountFromPoint(tilePos, map->getMapSize().width));
			//set the action of path
			path = ViewUtil::getThinPath(path);
			Vector<FiniteTimeAction*> actionArray = getActionArray(path);
			FiniteTimeAction* seq = Sequence::create(actionArray);
			seq->setTag(kTagSpriteSeq);
			sprite->runAction(seq);

			isSelectOne = false;
		}
		else if (isSelectOne && selectTile.getVal() != 0){
			stopScaleAction(sprite);
			sprite = setActionToSelectTile(tilePos);
			astar->setStartTile(tilePos);
			selectedTile = selectTile;
		}
	};
	
	listener1->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	auto _eventDispatcher = Director::getInstance()->getEventDispatcher();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
}

// on "init" you need to initialize your instance
bool SuperMatrix::init()
{
	bool bRet = false;
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! Layer::init());
		
		//////////////////////////////////////////////////////////////////////////
		// add your codes below...
		//////////////////////////////////////////////////////////////////////////

		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		/////////////////////////////
		// 2. add a menu item with "X" image, which is clicked to quit the program
		//    you may modify it.

		// add a "close" icon to exit the progress. it's an autorelease object
		auto closeItem = MenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			CC_CALLBACK_1(SuperMatrix::menuCallback, this));

		closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
			origin.y + closeItem->getContentSize().height / 2));

		// create menu, it's an autorelease object
		auto menu = Menu::create(closeItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
		
		//*********** super matrix start  **********
		this->setScale(SCALE_SIZE);
		this->setAnchorPoint(Vec2::ZERO);
		//this->setScaleY(0.755f);this->setPosition(PointMake(0, 14));//to make sumsung fit the screen 800:480
		//init self
		sprite = NULL;
		isSelectOne = false;
		scores = 0;
		//init background
		TMXTiledMap* map = TMXTiledMap::create("superMatrix/background.tmx");
		map->setPosition(Vec2(0.0, 0.0));
		addChild(map, 0, kTagTileMap);
		closeItem->setPosition(Vec2((map->getMapSize().width + 2.7)*map->getTileSize().width, 26));
		//init score text
		Label* pLabel = Label::create("0", "Arial", 24);
		CC_BREAK_IF(! pLabel);
		Size size = Director::getInstance()->getWinSize();
		pLabel->setPosition(Vec2((map->getMapSize().width + 1.6)*map->getTileSize().width, map->getTileSize().width*6.8));
		this->addChild(pLabel, 1, kTagTileLabel);
		//init high score text kTagTileHighLabel

		Label* pLabel1 = Label::create(ViewUtil::intToString(getHightScoreStr()).c_str(), "Arial", 24);
		CC_BREAK_IF(! pLabel1);
		pLabel1->setPosition(Vec2((map->getMapSize().width + 1.55)*map->getTileSize().width, map->getTileSize().width*2.15));
		this->addChild(pLabel1, 1, kTagTileHighLabel);

		//init sprits
		SpriteBatchNode* spriteSheet = SpriteBatchNode::create("superMatrix/sprits.png"); 
		spriteSheet->setPosition(map->getPosition());
		addChild(spriteSheet, 0, kTagTileSpriteSheet);
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("superMatrix/sprits.plist");
		//init score png
		Sprite* scorePng = Sprite::createWithSpriteFrameName("score.PNG");
		scorePng->setPosition(ccp(map->getMapSize().width*map->getTileSize().width, 0));
		scorePng->setAnchorPoint(Vec2::ZERO);
		spriteSheet->addChild(scorePng, 0, kTagScore);
		
		//init logic
		matrixLogic = new MatrixLogic(map->getMapSize().width, map->getMapSize().height, 7); 
		displayMap(matrixLogic->addSpritInMap(5));

		std::vector<int> blocks;
		for(int i = 1; i<8; i++){
			blocks.push_back(i);
		}
		astar = new AStarLogic(matrixLogic->getMap(), Vec2::ZERO, map->getMapSize(), blocks);
		//addChild(NoTouchLayer::create(),2);
		bRet = true;
	} while (0);

	return bRet;
}

Vec2 SuperMatrix::getConvertPos(TileBean tile){
	TMXTiledMap* map =  (TMXTiledMap*)getChildByTag(kTagTileMap);
	Size size = map->getTileSize();
	return Vec2((tile.getLocation().x + 0.5)*size.height, (tile.getLocation().y + 0.5)*size.width);
}

void SuperMatrix::displayMap(std::vector<TileBean> addTiles){
	char str[20] = {0};
	SpriteBatchNode* spriteSheet = (SpriteBatchNode*)getChildByTag(kTagTileSpriteSheet);
	for(unsigned i=0; i < addTiles.size(); i++)
	{
		sprintf(str, "%d.png", addTiles[i].getVal());
		Sprite* sprite = Sprite::createWithSpriteFrameName(str);
		//sprite ->setAnchorPoint(PointZero);
		sprite->setPosition(getConvertPos(addTiles[i]));
		int tileCount = addTiles[i].getLocation().y*matrixLogic->getWidth() + addTiles[i].getLocation().x;
		spriteSheet->addChild(sprite, 0, tileCount);
		/*ActionInterval *actionScaleTo = ScaleTo::create(0.5, SCALE_TO);
		ActionInterval *actionScaleBack = ScaleTo::create(0.5, 1);*/
		ActionInterval*  actionFade = CCFadeOut::create(0.3f);
		CallFuncN* fun = CallFuncN::create([&](Node* sender){
			//回调动作代码  
			runMatrixLogic(sender);
		});
		sprite->runAction(Sequence::create(actionFade, actionFade->reverse(), fun, NULL));
	}
	if (matrixLogic->getCountSize() == 0){//lose
		int highScore = LoadIntegerFromXML("highScoreString");
		if(highScore < scores){
			SaveIntegerToXML("highScoreString", scores);
			Label* pLabel = (Label*)getChildByTag(kTagTileHighLabel);
			std::string charScore = ViewUtil::intToString(scores);
			pLabel->setString(charScore.c_str());
		}
		NoTouchLayer* touchLayer = NoTouchLayer::create();
		touchLayer->beenCalled(this);
		addChild(touchLayer,2);

	}
}

void SuperMatrix::restart(){	
	this->scores = 0;
	this->sprite = NULL;
	this->selectedTile;
	this->isSelectOne = false;
	Label* pLabel = (Label*)getChildByTag(kTagTileLabel);
	pLabel->setString("0");
	TMXTiledMap* map = (TMXTiledMap*) getChildByTag(kTagTileMap);
	SpriteBatchNode* spriteSheet = (SpriteBatchNode*)getChildByTag(kTagTileSpriteSheet);
	int** maptiles = matrixLogic->getMap();
	for (int i = 0; i< matrixLogic->getWidth(); i++){
		for (int j = 0; j<matrixLogic->getHeight(); j++){
			if(maptiles[j][i] != 0){
				int childTag = ViewUtil::getCountFromPoint(ccp(i, j), map->getMapSize().width);
				spriteSheet->removeChildByTag(childTag, true);
			}
		}
	}
	matrixLogic->reset();
	displayMap(matrixLogic->addSpritInMap(5));
}
void SuperMatrix::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void SuperMatrix::menuCallback(Ref* pSender)
{
	NoTouchLayer* touchLayer = NoTouchLayer::create();
	touchLayer->beenCalled(this);
	addChild(touchLayer,2);
}

Sprite* SuperMatrix::setActionToSelectTile(Vec2 tilePos){
	SpriteBatchNode* spriteSheet = (SpriteBatchNode* )getChildByTag(kTagTileSpriteSheet);
	TMXTiledMap* map = (TMXTiledMap*) getChildByTag(kTagTileMap);
	Sprite* sprite = (Sprite*)spriteSheet->getChildByTag(ViewUtil::getCountFromPoint(tilePos, map->getMapSize().width));

	ActionInterval *actionScaleTo = ScaleTo::create(0.3f, SCALE_TO);
	ActionInterval *actionScaleBack = ScaleTo::create(0.15f, 1.1f);
	Action* action = CCRepeatForever::create(static_cast<Sequence *>(Sequence::create(actionScaleTo, actionScaleBack, NULL)));
	action->setTag(kTagSpriteAct);
	sprite->runAction(action);
	return sprite;
}

void SuperMatrix::stopScaleAction(Node* pSender){
    Action* action = pSender->getActionByTag(kTagSpriteAct);
	if(action != nullptr){
		pSender->stopAction(action);
	}	
	ActionInterval *actionScaleBack = ScaleTo::create(0.15f, 1);
	pSender->runAction(actionScaleBack);
}

void SuperMatrix::removeTileAndUpMap(Node* pSender){
	int** map = matrixLogic->getMap();
	TMXTiledMap* maps = (TMXTiledMap*) getChildByTag(kTagTileMap);
	Vec2 point = ViewUtil::getTheTilePos(pSender->getPosition(), maps->getTileSize());
	map[(int) point.y][(int) point.x] = 0;
	SpriteBatchNode* spriteSheet = (SpriteBatchNode*)pSender->getParent();
	spriteSheet->removeChild(pSender, true);
}

void SuperMatrix::fiveSuccess(std::vector<Vec2> points){
	SpriteBatchNode* spriteSheet = (SpriteBatchNode* )getChildByTag(kTagTileSpriteSheet);
	TMXTiledMap* map = (TMXTiledMap*) getChildByTag(kTagTileMap);
	
	for(unsigned i =0; i<points.size();i++){		
		int childTag = ViewUtil::getCountFromPoint(points[i], map->getMapSize().width);
		Sprite* sprite = (Sprite*)spriteSheet->getChildByTag(childTag);
		ActionInterval *actionScaleTo = ScaleTo::create(0.2f, 0.1f);
		CallFuncN* fun = CallFuncN::create(CC_CALLBACK_1(SuperMatrix::removeTileAndUpMap, this));
		FiniteTimeAction* seq = Sequence::create(actionScaleTo, fun, NULL);
		seq->setTag(kTagActionInterval);
		sprite->runAction(seq);
		//revert the tileInt array
		int* tilesInt = matrixLogic->getTilesInt();
		tilesInt[childTag] = 1;
	}
	matrixLogic->setCountSize(matrixLogic->getCountSize()+points.size());
	sprite = NULL;
	
}
void SuperMatrix::countScore(std::vector<Vec2> points){
	scores += (((float)points.size()-5)/10+1)*points.size()*10;
}

//if is a five successed result then add 3 random tiles
void SuperMatrix::runMatrixLogicAndDisplay(Node* pSender){
	if (!runMatrixLogic(pSender)){
		displayMap(matrixLogic->addSpritInMap(3));
	}
}

//use this fun do judge whether it is a five success
bool SuperMatrix::runMatrixLogic(Node* pSender){
	TMXTiledMap* map = (TMXTiledMap*) getChildByTag(kTagTileMap);
	Size size = map->getTileSize();
	Vec2 point = ViewUtil::getTheTilePos(pSender->getPosition(), map->getTileSize());
	int val = matrixLogic->getMap()[(int)point.y][(int)point.x];
	std::vector<Vec2> points = matrixLogic->judgeFives(TileBean(ViewUtil::getTheTilePos(pSender->getPosition(), size), val));
	if(points.size() > 0){
		fiveSuccess(points);
		//count score
		countScore(points);
		Label* pLabel = (Label*)getChildByTag(kTagTileLabel);
		std::string charScore = ViewUtil::intToString(scores);
		pLabel->setString(charScore.c_str());
		return true;
	}
	return false;
}

//convert the path to ActionArray
Vector<FiniteTimeAction*> SuperMatrix::getActionArray(std::vector<Vec2> path){
	TMXTiledMap* map = (TMXTiledMap*) getChildByTag(kTagTileMap);
	Size size = map->getTileSize();
	Vector<FiniteTimeAction*> actionArray = Vector<FiniteTimeAction*>(path.size());
	CallFuncN* fun = CallFuncN::create(CC_CALLBACK_1(SuperMatrix::stopScaleAction, this));
	actionArray.pushBack(fun);
	for(int i = 1; i<((int)path.size()); i++){
		//Vec2 subPo = Vec2(path[i-1].x - path[i].x, path[i-1].y - path[i].y);
		ActionInterval*  actionTo = CCMoveTo::create(0.2f, getConvertPos(TileBean(path[i], 0)));
		actionArray.pushBack(actionTo);
	}
	CallFuncN* fun1 = CallFuncN::create(CC_CALLBACK_1(SuperMatrix::runMatrixLogicAndDisplay, this));
	actionArray.pushBack(fun1);
	return actionArray;
}