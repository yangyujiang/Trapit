#ifndef __GAME_GROUND_CONTROLLER_H__
#define __GAME_GROUND_CONTROLLER_H__

#include "cocos2d.h"
#include "GameGroundView.h"

class GameGroundController :public cocos2d::CCNode
{
public:
	GameGroundController(void);
	~GameGroundController(void);
	CREATE_FUNC(GameGroundController);
	virtual bool init();


public:
	GameGroundView *view;
};

#endif //__GAME_GROUND_CONTROLLER_H__
