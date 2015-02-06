#include "cocos2d.h"
#include <string>

using namespace cocos2d;
class MenuScene : public Layer
{
private:
	LabelTTF* pLabel;
	void update(float dt);
public:
	virtual void onEnter();
	virtual bool init();
	static cocos2d::Scene* createScene();

	void menuClose(Ref* pSender);
	CREATE_FUNC(MenuScene);
};

