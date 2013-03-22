#ifndef __GAME_INSECT_VIEW_H__
#define __GAME_INSECT_VIEW_H__

#include "cocos2d.h"

USING_NS_CC;

class GameInsectView :public cocos2d::CCLayer
{
public:
	GameInsectView(void);
	~GameInsectView(void);

	virtual bool init();
	CREATE_FUNC(GameInsectView);
};

#endif //__GAME_INSECT_VIEW_H__
