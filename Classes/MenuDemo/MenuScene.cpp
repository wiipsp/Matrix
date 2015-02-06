#include "MenuScene.h"

using namespace cocos2d;

Scene * MenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void MenuScene::onEnter(){
	Layer::onEnter();	
}

void handleListener(Sprite* sprite){
	// 创建一个事件监听器 OneByOne 为单点触摸
	auto listener1 = EventListenerTouchOneByOne::create();
	// 设置是否吞没事件，在 onTouchBegan 方法返回 true 时吞没
	listener1->setSwallowTouches(true);



	// 使用 lambda 实现 onTouchBegan 事件回调函数
	listener1->onTouchBegan = [](Touch* touch, Event* event){
		/*// 获取事件所绑定的 target
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		// 获取当前点击点所在相对按钮的位置坐标
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		// 点击范围判断检测
		if (rect.containsPoint(locationInNode))
		{
		log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
		target->setOpacity(180);
		return true;
		}
		return false;*/

		// 获取事件所绑定的 target 
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		TMXTiledMap* map = (TMXTiledMap*)target->getParent();

		Size winSize = Director::getInstance()->getWinSize();
		//map->runAction( CCScaleBy::create(1, 1.05f) ) ;
		Sprite* sprite = (Sprite*)map->getChildByTag(2);
		Vec2 touchPoint = touch->getLocation();//touch->getLocation();
		float x = sprite->getPosition().x + touchPoint.x - winSize.width / 2;
		float y = sprite->getPosition().y + touchPoint.y - winSize.height / 2;
		// Create a label and initialize with string "Hello World".
		std::string a;
		std::stringstream os;
		os << "X :" << x << ", Y: " << y;
		a += os.str();
		LabelTTF* label1 = (LabelTTF*)target->getParent()->getParent()->getChildByTag(1);
		label1->setString(a.c_str());

		ActionInterval*  actionTo = MoveTo::create(2, Vec2(x, y));
		sprite->runAction(actionTo);


		return true;
	};
	// 触摸移动时触发  
	listener1->onTouchMoved = [](Touch* touch, Event* event){
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		// 移动当前按钮精灵的坐标位置  
		//target->setPosition(target->getPosition() + touch->getDelta());

		Vec2 diff = touch->getDelta();
		Node *node = target->getParent();
		Vec2 currentPos = node->getPosition();
		node->setPosition(currentPos + diff);
	};
	auto _eventDispatcher = Director::getInstance()->getEventDispatcher();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, sprite);

}
bool MenuScene::init(){
	bool bReturn = false;

	do{
		CC_BREAK_IF(!Layer::init());

		TMXTiledMap* map = TMXTiledMap::create("map.tmx");
		addChild(map, 0, 10);

		Size ss = map->getContentSize();
		CCLOG("ContentSize: %f, %f", ss.width, ss.height);

		/*Array* pChildrenArray = map->getChildren();
		SpriteBatchNode* child = NULL;
		Object* pObject = NULL;
		CCARRAY_FOREACH(pChildrenArray, pObject)
		{
			child = (SpriteBatchNode*)pObject;

			if (!child)
				break;

			child->getTexture()->setAntiAliasTexParameters();
		}
		*/
		//map->runAction( CCScaleBy::create(1, 2.0f) ) ;

		TMXLayer* layer = map->getLayer("background");
		Size sss = layer->getLayerSize();
		int i, j;
		std::stringstream os;
		for (j = 0; j< sss.height; j++){
			for (i = 0; i < sss.width; i++){

				os << layer->getTileGIDAt(ccp(i, j)) << "\t";
			}
			os << "\n";
		}
		CCLOG("%s", os.str().c_str());
		/*CCLOG("++++++++++++++++++++++++: %d,%d", layer->tileGIDAt(ccp(0,7)),layer->tileGIDAt(ccp(1,6)));
		CCLOG("++++++++++++++++++++++++: %d,%d", layer->tileGIDAt(ccp(0,7)),layer->tileGIDAt(ccp(1,6)));
		CCLOG("++++++++++++++++++++++++: %d,%d", layer->tileGIDAt(ccp(0,7)),layer->tileGIDAt(ccp(1,6)));
		CCLOG("++++++++++++++++++++++++: %d,%d", layer->tileGIDAt(ccp(0,7)),layer->tileGIDAt(ccp(1,6)));*/
		Sprite* sprite;
		sprite = layer->getTileAt(Vec2(1, 0));
		sprite->setScale(2);
		layer->removeChild(sprite, true);
		sprite = layer->getTileAt(Vec2(sss.width - 1, 0));
		sprite->setScale(2);


		Size size = Director::getInstance()->getWinSize();
		MenuItemImage *pDojoItem = MenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			CC_CALLBACK_1(MenuScene::menuClose, this));

		CC_BREAK_IF(!pDojoItem);

		pDojoItem->setPosition(Vec2(Director::getInstance()->getWinSize().width - 20, 15));
		Menu* pMenu = Menu::create(pDojoItem, NULL);
		pMenu->setPosition(Vec2::ZERO);
		this->addChild(pMenu, 1);


		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("test.plist");
		SpriteBatchNode* spriteSheet = SpriteBatchNode::create("test.png");
		addChild(spriteSheet);

		Vector<SpriteFrame*> walkAnimFrames = Vector<SpriteFrame*>(3);
		char str[20] = { 0 };
		for (int i = 0; i < 3; i++)
		{
			sprintf(str, "down%d.png", i);
			walkAnimFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(str));
		}
		Animation * animationDown = Animation::createWithSpriteFrames(walkAnimFrames, 0.2f);

		//CCAnimation* animationUp = CCAnimation::animation();
		//CCAnimation* animationLeft = CCAnimation::animation();
		//CCAnimation* animationRight = CCAnimation::animation();

		Sprite* pSprite = Sprite::createWithSpriteFrameName("down0.png");
		//	CCSprite* pSprite = CCSprite::create("1.jpg");
		
		CC_BREAK_IF(!pSprite);
		pSprite->setAnchorPoint(Vec2(0.5, 0.5));
		pSprite->runAction(RepeatForever::create(Animate::create(animationDown)));

		//spriteSheet->addChild(pSprite);  
		map->addChild(pSprite, 200, 2);

		handleListener(pSprite);
		//// Place the sprite on the center of the screen
		//pSprite->setPosition(ccp(size.width/2, size.height/2));

		//// Add the sprite to HelloWorld layer as a child layer.
		//this->addChild(pSprite, 0);

		//CCSize s =  CCDirector::sharedDirector()->getWinSize();

		//CCActionInterval*  actionTo = CCMoveTo::create(2, ccp(s.width-40, s.height-40));
		//CCActionInterval*  actionBy = CCMoveBy::create(2, ccp(480,320));
		//CCActionInterval*  actionByBack = actionBy->reverse();

		//pSprite->runAction( CCSequence::create(actionBy, actionByBack, NULL));
		//pSprite->runAction(actionTo);

		Label* pLabel = Label::create("Hello World", "Arial", 24);
		CC_BREAK_IF(!pLabel);

		// Get window size and place the label upper. 

		pLabel->setPosition(Vec2(size.width / 2, size.height - 25));

		// Add the label to HelloWorld layer as a child layer.
		this->addChild(pLabel, 1, 1);

		scheduleUpdate();

		bReturn = true;
	} while (0);

	return bReturn;
}

void MenuScene::update(float dt){
	TMXTiledMap* map = (TMXTiledMap*)getChildByTag(10);

	//map->runAction( CCScaleBy::create(1, 1.05f) ) ;
	Sprite* sprite = (Sprite*)map->getChildByTag(2);
	Vec2 herop = sprite->getPosition();
	float deltax = herop.x;
	float deltay = herop.y;
	Size winSize = Director::getInstance()->getWinSize();
	map->setPosition(Vec2(winSize.width / 2 - deltax, winSize.height / 2 - deltay));
}

void MenuScene::menuClose(Ref* pSender)
{
	// "close" menu item clicked
	Director::getInstance()->end();
}
