#ifndef __GAME_PLAY_CONTROLLER_H__
#define __GAME_PLAY_CONTROLLER_H__

#include "cocos2d.h"
#include "GamePlayView.h"

USING_NS_CC;

class GamePlayController :public CCLayer
{
public:
	GamePlayController(void);
	~GamePlayController(void);
	CREATE_FUNC(GamePlayController);
	virtual bool init();
public:
	static CCScene* scene();

public:
	GamePlayView *view;
};

#endif //__GAME_PLAY_CONTROLLER_H__
