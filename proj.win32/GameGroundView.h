#ifndef __GAME_GROUND_VIEW_H__
#define __GAME_GROUND_VIEW_H__

#include "cocos2d.h"

class GameGroundView :public cocos2d::CCLayer
{
public:
	GameGroundView(void);
	~GameGroundView(void);

	virtual bool init();
	CREATE_FUNC(GameGroundView);
};

#endif //__GAME_GROUND_VIEW_H__
