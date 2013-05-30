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
#include "Amber.h"

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

static vector<Amber*> newAmbers;//新成就
class GamePlayController :public GameViewDelegate,public GameResinBallViewDelegate,public AndroidKeyLayer
{
protected:
	b2MouseJoint *_mouseJoint;//调试用，鼠标关节
	CCSpriteBatchNode* batch;
protected:
	void createEnvir();
	void updateBox2DUserData();
public:
	//static CCRect mapArea;
	b2World* _world;//box2d世界
	myContactListener* contactListener;
	StaticLayer* buttonLayer;//按钮层
	MapLayer* mapLayer;//地图层


	GameResinBallModel *_resinBallModel;
	GameResinBallView *_resinBallView;
	vector<EnemyInsect*> _enemys;
	vector<BaseInsect*> _insects;
	vector<BaseInsect*> _amberInsects;
	vector<b2Body*> _deleteBodys;
	
	//游戏数据
	unsigned int amberCount;//计数，得到琥珀数
	const static int MIN_NUM=0;//最少蚂蚁数
	unsigned int nextLevelAmberNum;//下一关需要的琥珀数
	unsigned int cur_level;//当前等级

	std::set<int> ambersId;//本局游戏获得的琥珀id列表（不记重复）

	//计时
	CC_SYNTHESIZE(int,_timeCount,TimeCount);//计时

	//道具
	CC_SYNTHESIZE(bool ,_freezing,Freezing);

public:
	GamePlayController();
	virtual ~GamePlayController();
	CREATE_FUNC(GamePlayController);
	virtual bool init();
	void clean();//清理内存等

	void addAnt();//新增一只蚂蚁
	void addInsect();//新增一只虫子
	void addEnemyInsect();
	void initGameData();//初始化游戏数据
	void intFreezeBox();
	void updateLevelIfReached(float dt);//升级
	void playInsectDisappear(CCSprite* insect);//播放虫子消失动画
	void playFreeze();//冰冻
	void endFreeze();//结束冰冻
	static std::vector<Amber*> getNewAmbers();
	void updateInsectNumber();//更新蚂蚁数量，若蚂蚁数量少于最低值时，新增蚂蚁
	virtual void update(float dt);
	void step(float dt);
	void afterStep(float dt);//// process collisions and result from callbacks called by the step

	virtual void draw();
	void createWrapWall();
	vector<b2Body*> bodys; 
public:
	void test();//测试
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
	void initInsect(int num);//初始化虫子
	void initEnemyInsect(int num);
	void initAnts(int num);//初始化蚂蚁

	virtual void  keyBackClicked();

	//删除已变成琥珀的虫子
	void deleteAmberedInsect();

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
	void menuFreezeCallback(CCObject* pSender);
};


#endif //__GAME_PLAY_CONTROLLER_H__
