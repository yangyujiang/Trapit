#ifndef __GAME_INSECT_VIEW_H__
#define __GAME_INSECT_VIEW_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "GlobalDef.h"
#include "GameInsectModel.h"
#include "Enemy.h"

USING_NS_CC;

class BaseInsectView :public CCLayer,public GameModelDelegate
{
private:
	GameViewDelegate* pViewDelegate;
protected:
	CCSprite* _insect;
	BaseInsect* _insectModel;

	CCAnimation *animation;
	CCAction* animationAction;
public:
	void playAnimation(float velocity);//根据速度播放虫子行走的动画
	void initAnimation();
public:
	BaseInsectView();
	virtual ~BaseInsectView();

	virtual bool init(GameViewDelegate* pViewDelegate,BaseInsect* gameInsect);
	LAYER_CREATE_FUNC_DOUBLE_PARAM(BaseInsectView,GameViewDelegate*,pViewDelegate,BaseInsect*,gameInsect);

	virtual void testModelDelegate();

	void update(float dt);
};


class GameInsectView :public CCLayer,public GameModelDelegate
{
private:
	GameViewDelegate* pViewDelegate;
protected:
	CCSprite* _insect;
	BaseInsect* _insectModel;

	CCAnimation *animation;
	CCAction* animationAction;
public:
	void playAnimation(float velocity);//根据速度播放虫子行走的动画
	void initAnimation();
public:
	GameInsectView();
	virtual ~GameInsectView();

	virtual bool init(GameViewDelegate* pViewDelegate,BaseInsect* gameInsect);
	LAYER_CREATE_FUNC_DOUBLE_PARAM(GameInsectView,GameViewDelegate*,pViewDelegate,BaseInsect*,gameInsect);

	virtual void testModelDelegate();

	void update(float dt);
};

class EnemyView :public CCLayer,public GameModelDelegate
{
private:
	GameViewDelegate* pViewDelegate;
protected:
	CCSprite* _insect;
	Enemy* _insectModel;

	CCAnimation *animation;
	CCAction* animationAction;	
public:
	void playAnimation(float velocity);//根据速度播放虫子行走的动画
	void initAnimation();
public:
	EnemyView();
	virtual ~EnemyView();

	virtual bool init(GameViewDelegate* pViewDelegate,Enemy* gameInsect);
	LAYER_CREATE_FUNC_DOUBLE_PARAM(EnemyView,GameViewDelegate*,pViewDelegate,Enemy*,gameInsect);

	virtual void testModelDelegate();

	void update(float dt);
};

#endif //__GAME_INSECT_VIEW_H__
