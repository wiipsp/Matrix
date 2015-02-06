#ifndef __MATRIX_NOTOUCH_LAYER_H__
#define __MATRIX_NOTOUCH_LAYER_H__
#include "MatrixScene.h"

class NoTouchLayer: public cocos2d::Layer
{
private:
	SuperMatrix *superMatrix;
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone 
	virtual bool init();   

	void setSuperMatrix(SuperMatrix* superMatrix){this->superMatrix = superMatrix;}
	// implement the "static node()" method manually 
	CREATE_FUNC(NoTouchLayer); 

	void beenCalled(SuperMatrix* layer);
	void restartSM(Ref* pSender);
	void exitSM(Ref* pSender);

	
};

#endif