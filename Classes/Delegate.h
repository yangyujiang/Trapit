#ifndef __DELEGATE_H__
#define __DELEGATE_H__

#include "Box2D\Box2D.h"
#include "cocos2d.h"
USING_NS_CC;

class GameViewDelegate{
public:
	GameViewDelegate(){};
	virtual ~GameViewDelegate(){};

	virtual void testViewDelegate(){};
};

class GameModelDelegate{
public:
	GameModelDelegate(){};
	virtual ~GameModelDelegate(){};

	virtual void testModelDelegate(){};
	virtual void playAttack(CCPoint attacker,CCPoint target){};
	virtual void changePlayVel(float speed){};
	virtual void playKiss(CCPoint source,CCPoint des){};
	virtual void finish(){};
	virtual CCSprite* getSprite(){return NULL;};
};

class GameResinBallViewDelegate{
public:
	GameResinBallViewDelegate(){};
	virtual ~GameResinBallViewDelegate(){};

	virtual void dealWithTouchesBegan(b2Vec2 locationWorld){};
	virtual void dealWithTouchesMoved(b2Vec2 locationWorld){};
	virtual void dealWithTouchesEnded(){};
};
#endif //__DELEGATE_H__