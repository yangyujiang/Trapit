#ifndef __GAME_INSECT_MODEL_H__
#define __GAME_INSECT_MODEL_H__

#include "cocos2d.h"

class GameInsectModel :public cocos2d::CCObject
{
protected:
	CC_PROPERTY(float,_velocity,Velocity);
	float posX,posY;
	float lastX,lastY;
public:
	GameInsectModel(void);
	~GameInsectModel(void);

	virtual bool init();
	CREATE_FUNC(GameInsectModel);
};

#endif //__GAME_INSECT_MODEL_H__
