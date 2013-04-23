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
#include "Enemy.h"
#include "GameResinBallView.h"
#include "StaticLayer.h"
#include "myContactListener.h"

USING_NS_CC;


//AndroidKeyLayer.h
class AndroidKeyLayer:public CCLayer{
public :
	virtual void keyBackClicked()//Android 返回键
	{
		CCDirector::sharedDirector()->end();
		CCLayer::keyBackClicked();
	}
	virtual void keyMenuClicked()//Android 菜单键
	{
		CCDirector::sharedDirector()->pause();
		CCLayer::keyMenuClicked();
	}
};

class GamePlayController :public GameViewDelegate,public GameResinBallViewDelegate,public AndroidKeyLayer
{
protected:
	b2MouseJoint *_mouseJoint;//调试用，鼠标关节

protected:
	void createEnvir();
	void updateBox2DUserData();
public:
	b2World* _world;//box2d世界
	myContactListener* contactListener;
	StaticLayer* buttonLayer;//按钮层
	MapLayer* mapLayer;//地图层
	GameInsectView *_insectView;
	GameInsectModel *_insectModel;

	GameResinBallModel *_resinBallModel;
	GameResinBallView *_resinBallView;
	vector<Enemy*> enemys;
	vector<BaseInsect*> _insects;
	unsigned int amberCount;//计数，得到琥珀数
	const static int MIN_NUM=0;//最少蚂蚁数

public:
	GamePlayController();
	virtual ~GamePlayController();
	CREATE_FUNC(GamePlayController);
	virtual bool init();

	void addInsect();//新增一只虫子
	void updateInsectNumber();//更新蚂蚁数量，若蚂蚁数量少于最低值时，新增蚂蚁
	virtual void update(float dt);
	void step(float dt);
	void afterStep(float dt);//// process collisions and result from callbacks called by the step

	virtual void draw();
	void createWrapWall();
	vector<b2Body*> bodys; 
public:
	static CCScene* scene();
	/*init后会调用; 若是加了CCTransitionScene后，在过渡场景开始后调用 
     */  
	void onEnter();
	void onEnterTransitionDidFinish();
	/*onEnter以后将会调用此方法 
     若使用CCTransitionScene,将会在过渡效果结束以后调用此方法 
     */  
	void onExitTransitionDidStart();
	void onExit();// 节点调用dealloc方法之前将会调用此方法   
    //如果使用了CCTransitionScene,将会在过渡效果结束以后调用此方法   


	void testViewDelegate();

	void initResinBall();//初始化树脂球及其模型
	void initInsect();//初始化虫子

	//重力感应
	void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);
	
	//重写GameResinBallDelegate的方法
	void dealWithTouchesBegan(b2Vec2 locationWorld);
	void dealWithTouchesMoved(b2Vec2 locationWorld);
	void dealWithTouchesEnded();

	void shrinkCallBack(CCObject* pSender);

public:
	void checkGameOverAndDo();
	void menuGoBackCallback(CCObject* pSender);
	void menuGameOverCallback(CCObject* pSender);
};


#endif //__GAME_PLAY_CONTROLLER_H__
