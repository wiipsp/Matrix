#include "AStarViewScene.h"

enum {
	kTagTileMap,
	kTagTileSprite,
	kTagTileSpriteSheet,
	kTagTileLabel,
	kTagSpriteAct,
	kTagSpriteSeq
};


AStarView::~AStarView(){
	player->release();
	delete astar;
}

Scene* AStarView::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = AStarView::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool AStarView::init()
{
	bool bRet = false;
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! Layer::init());

		this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(AStarView::touchBegan, this);
		listener->onTouchMoved = CC_CALLBACK_2(AStarView::touchMoved, this);
		listener->onTouchEnded = CC_CALLBACK_2(AStarView::touchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		//////////////////////////////////////////////////////////////////////////
		// add your codes below...
		//////////////////////////////////////////////////////////////////////////

		// 1. Add a menu item with "X" image, which is clicked to quit the program.

		// Create a "close" menu item with close icon, it's an auto release object.
		MenuItemImage *pCloseItem = MenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			CC_CALLBACK_1(AStarView::menuCloseCallback, this));
		CC_BREAK_IF(! pCloseItem);

		// Place the menu item bottom-right conner.
		pCloseItem->setPosition(Vec2(Director::getInstance()->getWinSize().width - 20, 20));

		// Create a menu with the "close" menu item, it's an auto release object.
		Menu* pMenu = Menu::create(pCloseItem, nullptr);
		pMenu->setPosition(Vec2::ZERO);
		CC_BREAK_IF(! pMenu);

		// Add the menu to AStarView layer as a child layer.
		this->addChild(pMenu, 1);

		Label* pLabel = Label::create("Hello World", "Arial", 24);
		CC_BREAK_IF(! pLabel);

		// Get window size and place the label upper. 
		Size size = Director::getInstance()->getWinSize();
		pLabel->setPosition(Vec2(size.width / 2, size.height - 50));

		// Add the label to HelloWorld layer as a child layer.
		this->addChild(pLabel, 1,kTagTileLabel);

		TMXTiledMap* map = TMXTiledMap::create("map.tmx");
		map->setPosition(Vec2(0.0,0.0));
		addChild(map, 0, kTagTileMap);		
		TMXLayer* layer = map->getLayer("background");
	//	layer->getTexture()->setAntiAliasTexParameters();
		
		
		
		SpriteBatchNode* spriteSheet = SpriteBatchNode::create("test.png"); 
		spriteSheet->setPosition(map->getPosition());
		addChild(spriteSheet, 0, kTagTileSpriteSheet);
		this->player = Player::create("test.plist");
		this->player->retain();

		Sprite* sprite = Sprite::createWithSpriteFrameName("down0.png");
		CC_BREAK_IF(! sprite);
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(Vec2(map->getTileSize().width, map->getTileSize().height));
		Action* actionDown = RepeatForever::create( player->getAnimateDown());
		actionDown->setTag(kTagSpriteAct);
		sprite->runAction(actionDown);
		spriteSheet->addChild(sprite, 0, kTagTileSprite);

		Size  mapSize = map->getTileSize();
		Point startTile = ViewUtil::getTheTilePos(sprite->getPosition(), mapSize);
		std::vector<int> blocks;
		blocks.push_back(1);
		//int* matrix = getMapMatrix(map);
		astar = new AStarLogic(getMapMatrix(map), startTile, map->getMapSize(), blocks);
		//delete []matrix;
		scheduleUpdate();
		bRet = true;
	} while (0);

	return bRet;
}

//get the right map position
Point AStarView::getRightPoint(Size mapSize, Size winSize, Point spritePos){
	float x = 0;
	float y =0;
	if((mapSize.width - winSize.width) < 0){
		x = 0;
	}else{
	   if (spritePos.x < winSize.width/2){
		   x = 0;
	   }else if(spritePos.x > (mapSize.width - winSize.width/2)){
		   x = winSize.width - mapSize.width;//set the map pos to -
	   } else{
		   x = winSize.width/2 - spritePos.x;
	   }
	}
	if((mapSize.height - winSize.height) < 0){
		y = 0;
	}else{
		if(spritePos.y < winSize.height/2){
			y = 0;
		}else if(spritePos.y > (mapSize.height - winSize.height/2)){
			y = winSize.height - mapSize.height;
		}else{
			y = winSize.height/2 - spritePos.y;
		}
	}
	return Vec2(x, y);
}

//to make the sprite in the screen center
void AStarView::update(float dt){
	TMXTiledMap* map = (TMXTiledMap*) getChildByTag(kTagTileMap);
	SpriteBatchNode* spriteSheet = (SpriteBatchNode* )getChildByTag(kTagTileSpriteSheet);
	Sprite* sprite = (Sprite*)spriteSheet->getChildByTag(kTagTileSprite);
	//to judge the sprite is or not move if yes then update the position
	FiniteTimeAction* seq = (FiniteTimeAction*)sprite->getActionByTag(kTagSpriteSeq);
	if(seq == nullptr){
		return;
	}else{
		if(seq->isDone()){
			return;
		}
	}
	
	//get the correct map position to adjust the screen
	Size winSize = Director::getInstance()->getWinSize();
	Point position = getRightPoint(map->getContentSize(), winSize, sprite->getPosition());
	map->setPosition(position);
	//make sure the map and the sprite location are same
	spriteSheet->setPosition(map->getPosition());
}

//get the Matrix about tile map, the value is the tile's parameter
int** AStarView::getMapMatrix(TMXTiledMap *map){
	TMXLayer* layer = map->getLayer("background");
	Size size = layer->getLayerSize();
	int height = size.height;
	int width = size.width;
	int **mapMatrix;
	mapMatrix = new int*[height];
	for (int i = 0; i < height; i++)
		mapMatrix[i] = new int[width];
	//std::stringstream os1;
	for(int i = (size.height-1); i >= 0;i--){
		for(int j = 0; j < size.width ; j++){
			int tilegid = layer->getTileGIDAt(Vec2(j, i));
			//os1<<tilegid<<"\t";
			Value tiledic = map->getPropertiesForGID(tilegid);
			auto value = tiledic.asValueMap().find("block")->second;
			int block = value.asInt();
			int mapI = size.height - i - 1;
			mapMatrix[mapI][j] = block;
		}
		//os1<<"\n";
	}
	//CCLOG("%s", os1.str().c_str());
	/*
	std::stringstream os;
	for(int i = 0; i < size.height;i++){
		for(int j = 0; j < size.width ; j++){
			os<<mapMatrix[i][j]<<"\t";
		}
		os<<"\n";
	}
	CCLOG("%s", os.str().c_str());*/
	return mapMatrix;
}

void AStarView::menuCloseCallback(Ref* pSender)
{
	// "close" menu item clicked
	Director::getInstance()->end();
}

void AStarView::touchMoved(Touch* touch, Event* event){
	Point diff = touch->getDelta();
	Node *node = getChildByTag(kTagTileMap);
	node->setPosition( node->getPosition() + diff);
	SpriteBatchNode* spriteSheet = (SpriteBatchNode* )getChildByTag(kTagTileSpriteSheet);	
	spriteSheet->setPosition(node->getPosition());
}


//through the subPosition to judge the direction
void AStarView::changeSpriteDir(Vec2 subPo){
	int x = subPo.x;
	int y = subPo.y;
	int direct = Player::getDirection(x, y);
	SpriteBatchNode* spriteSheet = (SpriteBatchNode* )getChildByTag(kTagTileSpriteSheet);
	Sprite* sprite = (Sprite*)spriteSheet->getChildByTag(kTagTileSprite);
	Action* action = sprite->getActionByTag(kTagSpriteAct);
	sprite->stopActionByTag(kTagSpriteAct);
	
	switch(direct){
		case 3:
			action = RepeatForever::create( player->getAnimateDown());
			break;
		case 4:
			action = RepeatForever::create( player->getAnimateLeft());
			break;
		case 5:
			action = RepeatForever::create( player->getAnimateRight());
			break;
		case 2:
			action = RepeatForever::create( player->getAnimateUp());
			break;
		default:
			action = RepeatForever::create( player->getAnimateUp());
			break;
	}
	action->setTag(kTagSpriteAct);
	sprite->runAction(action);
}

//convert the path to ActionArray
Vector<FiniteTimeAction*> AStarView::getActionArray(std::vector<Point> path){
	TMXTiledMap* map = (TMXTiledMap*) getChildByTag(kTagTileMap);
	Size size = map->getTileSize();
	Vector<FiniteTimeAction*> actionArray;
	for(int i = 1; i<((int)path.size()); i++){
		Vec2 subPo = Vec2(path[i-1].x - path[i].x, path[i-1].y - path[i].y);
		auto fun = CallFunc::create(CC_CALLBACK_0(AStarView::changeSpriteDir, this, subPo));
		actionArray.pushBack(fun);
		ActionInterval*  actionTo = CCMoveTo::create(1, ViewUtil::getThePixPos(path[i], size));
		actionArray.pushBack(actionTo);
	}
	return actionArray;
}

bool AStarView::touchBegan(Touch* touch, Event* event){
	TMXTiledMap* map = (TMXTiledMap*) getChildByTag(kTagTileMap);
	SpriteBatchNode* spriteSheet = (SpriteBatchNode* )getChildByTag(kTagTileSpriteSheet);
	Size winSize = Director::getInstance()->getWinSize();
	//map->runAction( CCScaleBy::create(1, 1.05f) ) ;
	Sprite* sprite = (Sprite*)spriteSheet->getChildByTag(kTagTileSprite);
	//Sprite* sprite = (Sprite*)map->getChildByTag(kTagTileSprite);
	Point touchPoint = map->convertTouchToNodeSpace(touch);
	float x = touchPoint.x;
	float y = touchPoint.y;
	// Create a label and initialize with string "Hello World".
	std::string a;
	std::stringstream os;
	os << "X :"<<x<<", Y: "<<y;
	a +=os.str();
	LabelTTF* label1 = (LabelTTF*)getChildByTag(kTagTileLabel);
	label1->setString(a.c_str());
	
	//AStar logic
	std::vector<Point> path = astar->moveTo(ViewUtil::getTheTilePos(touchPoint, map->getTileSize()));
	if(path.size() < 2){
		return true;
	}
	path = ViewUtil::getThinPath(path);
	Vector<FiniteTimeAction*> actionArray = getActionArray(path);
	FiniteTimeAction* seq = Sequence::create(actionArray);
	seq->setTag(kTagSpriteSeq);
	sprite->runAction(seq);
	return true;
}

void AStarView::touchEnded(Touch* touch, Event* event){

}