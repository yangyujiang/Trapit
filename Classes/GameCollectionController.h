#ifndef __GAME_COLLECTION_CONTROLLER_H__
#define __GAME_COLLECTION_CONTROLLER_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "vector"
#include "StaticLayer.h"

USING_NS_CC;

const int TAG_LIST=100;//列表层
const int TAG_CUR_AMBER=101;//
const int TAG_CUR_AMBER_MOVABLE=102;
const int TAG_MOVETO=103;//moveTo action

class GameCollectionController :public CCLayer
{
public:
	//StaticLayer* buttonLayer;//按钮层
	int rightWidth;
	CCSize winSize;
	unsigned int preIdx;
	bool isMoving;//是否正在拖动琥珀
	CCLayer* amberAddLayer;
public:
	GameCollectionController();
	virtual ~GameCollectionController();
	CREATE_FUNC(GameCollectionController);
	virtual bool init();
	//init后会调用; 若是加了CCTransitionScene后，在过渡场景开始后调用  
	void onEnter();
	void onEnterTransitionDidFinish();
	//onEnter以后将会调用此方法 若使用CCTransitionScene,将会在过渡效果结束以后调用此方法 
	void onExitTransitionDidStart();
	void onExit();// 节点调用dealloc方法之前将会调用此方法   
    //如果使用了CCTransitionScene,将会在过渡效果结束以后调用此方法  

	virtual void update(float dt);
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
    void menuStartCallback(CCObject* pSender);

	CCLayer* initbottomLayer();

	CCSprite* getCurSprite(unsigned int idx);

public:
	static CCScene* scene();
};

#endif //__GAME_COLLECTION_CONTROLLER_H__
