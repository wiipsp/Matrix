#include "Player.h"

Player * Player::create(char* plist){
	Player* player = new Player(plist);
	player->autorelease();
	return player;
}

Player::Player(char* plist)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist);
	Vector<SpriteFrame*> walkAnimFrames = Vector<SpriteFrame*>(3);
	char str[20] = {0}; 
	for(int i = 0; i < 3; i++)  
	{
		sprintf(str, "down%d.png", i);
		walkAnimFrames.pushBack(SpriteFrameCache::getInstance()->spriteFrameByName(str));
	}
	m_animateDown = Animate::create(CCAnimation::createWithSpriteFrames(walkAnimFrames, 0.2f));
	m_animateDown->retain();

	walkAnimFrames = Vector<SpriteFrame*>(3);
	for(int i = 0; i < 3; i++)  
	{
		sprintf(str, "up%d.png", i);
		walkAnimFrames.pushBack(SpriteFrameCache::getInstance()->spriteFrameByName(str));
	}
	m_animateUp = Animate::create(CCAnimation::createWithSpriteFrames(walkAnimFrames, 0.2f));
	m_animateUp->retain();

	walkAnimFrames = Vector<SpriteFrame*>(3);
	for(int i = 0; i < 3; i++)  
	{
		sprintf(str, "left%d.png", i);
		walkAnimFrames.pushBack(SpriteFrameCache::getInstance()->spriteFrameByName(str));
	}
	m_animateLeft = Animate::create(CCAnimation::createWithSpriteFrames(walkAnimFrames, 0.2f));
	m_animateLeft->retain();

	walkAnimFrames = Vector<SpriteFrame*>(3);
	for(int i = 0; i < 3; i++)  
	{
		sprintf(str, "right%d.png", i);
		walkAnimFrames.pushBack(SpriteFrameCache::getInstance()->spriteFrameByName(str));
	}
	m_animateRight = Animate::create(CCAnimation::createWithSpriteFrames(walkAnimFrames, 0.2f));
	m_animateRight->retain();
}

int Player::getDirection(int absX, int absY){
	if (absX == 0 && absY == 0){
		return flagStay;
	}
	if(absY == 0 && absX < 0){
		return flagRight;
	}
	if(absY < 0 && absX == 0){
		return flagUp;
	}
	if(absY > 0 && absX == 0){
		return flagDown;
	}
	if(absY == 0 && absX > 0){
		return flagLeft;
	}
	return flagNull;
}

Player::~Player(){
	
}