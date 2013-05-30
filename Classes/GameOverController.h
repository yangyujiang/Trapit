#ifndef __GAME_OVER_CONTROLLER_H__
#define __GAME_OVER_CONTROLLER_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "vector"
#include "StaticLayer.h"

USING_NS_CC;

class GameOverController :public CCLayer
{
public:
	CCSize winSize;

public:
	GameOverController();
	virtual ~GameOverController();
	CREATE_FUNC(GameOverController);
	virtual bool init();
	//init后会调用; 若是加了CCTransitionScene后，在过渡场景开始后调用  
	void onEnter();
	void onEnterTransitionDidFinish();
	//onEnter以后将会调用此方法 若使用CCTransitionScene,将会在过渡效果结束以后调用此方法 
	void onExitTransitionDidStart();
	void onExit();// 节点调用dealloc方法之前将会调用此方法   
    //如果使用了CCTransitionScene,将会在过渡效果结束以后调用此方法  
	
	virtual void  keyBackClicked();

	virtual void update(float dt);
	
    void menuRestartCallback(CCObject* pSender);
	void menuShopCallback(CCObject* pSender);
	void menuCollectionCallback(CCObject* pSender);
	void menuGoBackCallback(CCObject* pSender);
	void menuWikiCallback(CCObject* pSender);

	void initScrollView();
	CCLayer* initTopPanel();
	CCLayer* initBottomPanel();
	CCSprite* initResultPanel();

public:
	static CCScene* scene();
};

#endif //__GAME_OVER_CONTROLLER_H__
