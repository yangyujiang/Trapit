#ifndef __GAME_INSECT_VIEW_H__
#define __GAME_INSECT_VIEW_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "GlobalDef.h"
#include "GameInsectModel.h"
#include "Enemy.h"

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
	void playAnimation(float velocity);//�����ٶȲ��ų������ߵĶ���
	void initAnimation();
public:
	GameInsectView();
	virtual ~GameInsectView();

	virtual bool init(GameViewDelegate* pViewDelegate,GameInsectModel* gameInsect);
	LAYER_CREATE_FUNC_DOUBLE_PARAM(GameInsectView,GameViewDelegate*,pViewDelegate,GameInsectModel*,gameInsect);

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
	void playAnimation(float velocity);//�����ٶȲ��ų������ߵĶ���
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
