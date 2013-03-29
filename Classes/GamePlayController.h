#ifndef __GAME_PLAY_CONTROLLER_H__
#define __GAME_PLAY_CONTROLLER_H__

#include "cocos2d.h"
#include "GamePlayView.h"
#include "GameInsectView.h"
#include "GameInsectModel.h"
#include "Delegate.h"
#include "vector"
#include "Box2D\Box2D.h"
#include "GameResinBallModel.h"
#include "GameResinBallView.h"
#include "StaticLayer.h"

USING_NS_CC;

class GamePlayController :public CCLayer,public GameViewDelegate,public GameResinBallViewDelegate
{
protected:
	b2MouseJoint *_mouseJoint;//调试用，鼠标关节

public:
	b2World* _world;
	GamePlayView *view;
	StaticLayer* buttonLayer;//按钮层
	MapLayer* mapLayer;//地图层
	GameInsectView *_insectView;
	GameInsectModel *_insectModel;

	GameResinBallModel *_resinBallModel;
	GameResinBallView *_resinBallView;

	vector<GameInsectModel*> _insects;

public:
	GamePlayController();
	virtual ~GamePlayController();
	CREATE_FUNC(GamePlayController);
	virtual bool init();

	virtual void update(float dt);
	void step(float dt);
	void afterStep(float dt);//// process collisions and result from callbacks called by the step

	virtual void draw();
	void createWrapWall();
	vector<b2Body*> bodys; 
public:
	static CCScene* scene();

	void testViewDelegate();

	void initResinBall();//初始化树脂球及其模型
	void initInsect();//初始化虫子

	//重力感应
	void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);
	
	//重写GameResinBallDelegate的方法
	void dealWithTouchesBegan(b2Vec2 locationWorld);
	void dealWithTouchesMoved(b2Vec2 locationWorld);
	void dealWithTouchesEnded();
};

#endif //__GAME_PLAY_CONTROLLER_H__
