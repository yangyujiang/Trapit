#ifndef __GAME_LEVEL_CONTROLLER_H__
#define __GAME_LEVEL_CONTROLLER_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "vector"
#include "StaticLayer.h"

USING_NS_CC;

class GameLevelController :public CCLayer
{
public:
	StaticLayer* buttonLayer;//按钮层

public:
	GameLevelController();
	virtual ~GameLevelController();
	CREATE_FUNC(GameLevelController);
	virtual bool init();
	//init后会调用; 若是加了CCTransitionScene后，在过渡场景开始后调用  
	void onEnter();
	void onEnterTransitionDidFinish();
	//onEnter以后将会调用此方法 若使用CCTransitionScene,将会在过渡效果结束以后调用此方法 
	void onExitTransitionDidStart();
	void onExit();// 节点调用dealloc方法之前将会调用此方法   
    //如果使用了CCTransitionScene,将会在过渡效果结束以后调用此方法  

	virtual void update(float dt);
	
    void menuStartCallback(CCObject* pSender);

	void initScrollView();

public:
	static CCScene* scene();
};

#endif //__GAME_LEVEL_CONTROLLER_H__
