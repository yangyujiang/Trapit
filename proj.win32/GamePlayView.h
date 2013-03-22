#ifndef __GAME_PLAY_VIEW_H__
#define __GAME_PLAY_VIEW_H__

#include "cocos2d.h"

class GamePlayView :public cocos2d::CCLayer
{
public:
	GamePlayView(void);
	~GamePlayView(void);

	virtual bool init();
	CREATE_FUNC(GamePlayView);
};

#endif //__GAME_PLAY_VIEW_H__
