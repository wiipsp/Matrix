#include "NoTouchLayer.h"

USING_NS_CC;

MenuItem* itemForTouch(Touch *touch, Vector<Node*> m_pChildren)
{
	Point touchLocation = touch->getLocation();

	if (m_pChildren.size() > 0)
	{
		Ref* pObject = nullptr;
		for (auto pObject : m_pChildren)
		{
			MenuItem* pChild = dynamic_cast<MenuItem*>(pObject);
			if (pChild && pChild->isVisible() && pChild->isEnabled())
			{
				Point local = pChild->convertToNodeSpace(touchLocation);
				Rect r = pChild->rect();
				r.origin = Vec2::ZERO;

				if (r.containsPoint(local))
				{
					return pChild;
				}
			}
		}
	}

	return nullptr;
}


bool NoTouchLayer::init(){  
	if (!Layer::init() )  
	{  
		return false;  
	}  

	LayerColor *color = LayerColor::create(Color4B(0, 0, 0, 175));
	this->addChild(color);

	// ����һ���¼������� OneByOne Ϊ���㴥��
	auto listener2 = EventListenerTouchOneByOne::create();
	// �����Ƿ���û�¼����� onTouchBegan �������� true ʱ��û
	listener2->setSwallowTouches(true);
	
	// ʹ�� lambda ʵ�� onTouchBegan �¼��ص�����
	listener2->onTouchBegan = [this](Touch* touch, Event* event){
		Menu* menu = (Menu*)getChildByTag(100);
		Vector<Node*> m_pChildren = menu->getChildren();

		if (itemForTouch(touch, m_pChildren) != nullptr){
			return false;
		}
		return true;
	};

	auto _eventDispatcher = Director::getInstance()->getEventDispatcher();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);

	return true;  
}  
void NoTouchLayer::restartSM(Ref* pSender){
	superMatrix->restart();
	superMatrix->removeChild(this, true);
}

void NoTouchLayer::exitSM(Ref* pSender){
    superMatrix->menuCloseCallback(pSender);
}

void NoTouchLayer::beenCalled(SuperMatrix* layer){
	setSuperMatrix(layer);
	
	Sprite* repeatPng = Sprite::createWithSpriteFrameName("repeat.PNG");
	Sprite* exitPng = Sprite::createWithSpriteFrameName("exit.PNG");
	MenuItemSprite* item1 = MenuItemSprite::create(repeatPng, repeatPng, repeatPng, CC_CALLBACK_1(NoTouchLayer::restartSM, this));
	MenuItemSprite* item2 = MenuItemSprite::create(exitPng, exitPng, exitPng, CC_CALLBACK_1(NoTouchLayer::exitSM, this));
	Menu* menu = Menu::create( item1, item2, NULL);
	menu->alignItemsVertically();
	addChild(menu, 0, 100);
}

