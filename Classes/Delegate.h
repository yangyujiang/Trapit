#ifndef __DELEGATE_H__
#define __DELEGATE_H__

#include "Box2D\Box2D.h"

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