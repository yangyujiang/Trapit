#ifndef __GAME_INSECT_VIEW_H__
#define __GAME_INSECT_VIEW_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "GlobalDef.h"
#include "GameInsectModel.h"

USING_NS_CC;

class GameInsectView :public CCLayer,public GameModelDelegate
{
private:
	GameViewDelegate* pViewDelegate;
protected:
	CCSprite* _insect;
	GameInsectModel* _insectModel;

	CCAnimation *animation;
	CCAction* animationAction;	
public:
	void playAnimation(float velocity);//根据速度播放虫子行走的动画
	void initAnimation();
public:
	GameInsectView();
	virtual ~GameInsectView();

	virtual bool init(GameViewDelegate* pViewDelegate,GameInsectModel* gameInsect);
	LAYER_CREATE_FUNC_DOUBLE_PARAM(GameInsectView,GameViewDelegate*,pViewDelegate,GameInsectModel*,gameInsect);

	virtual void testModelDelegate();

	void update(float dt);
};

#endif //__GAME_INSECT_VIEW_H__
