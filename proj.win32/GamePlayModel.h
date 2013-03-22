#ifndef __GAME_PLAY_MODEL_H__
#define __GAME_PLAY_MODEL_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"

USING_NS_CC;

class GamePlayModel :public cocos2d::CCObject
{
protected:
	CC_PROPERTY_READONLY(b2World*,_world,World);
public:
	GamePlayModel(void):_world(NULL){};
	~GamePlayModel(void);

	virtual bool init();
	CREATE_FUNC(GamePlayModel);
};

#endif //__GAME_PLAY_MODEL_H__
