#ifndef __ASTAR_PLAYER_H__
#define __ASTAR_PLAYER_H__
#include "cocos2d.h"
#include "Player.h"
#include "base/CCVector.h"
USING_NS_CC;

class Player : public Ref
{
private:
	Animate * m_animateDown;
	Animate * m_animateUp;
	Animate * m_animateLeft;
	Animate * m_animateRight;
public:
	enum directFlag{
		flagNull,//0
		flagStay,//1
		flagUp,//2
		flagDown,//3
		flagLeft,//4
		flagRight//5
	};
	/** Create an Player */
	static Player* create(char* plist);
	Player(char* plist);
	static int getDirection(int absX, int absY);
	Animate* getAnimateDown(){return m_animateDown;}
	Animate* getAnimateUp(){return m_animateUp;}
	Animate* getAnimateLeft(){return m_animateLeft;}
	Animate* getAnimateRight(){return m_animateRight;}
	~Player();
};

#endif
