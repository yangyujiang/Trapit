#ifndef __GAME_COLLECTION_CONTROLLER_H__
#define __GAME_COLLECTION_CONTROLLER_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "vector"
#include "StaticLayer.h"
#include "Constant.h"
#include "SkillSlotAdapter.h"
#include "Amber.h"

USING_NS_CC;

const int TAG_LIST=100;//列表层
const int TAG_CUR_AMBER=101;//
const int TAG_CUR_AMBER_MOVABLE=102;
const int TAG_MOVETO=103;//moveTo action

class GameCollectionController :public CCLayer
{
public:
	//StaticLayer* buttonLayer;//按钮层
	int listWidth;
	CCSize winSize;
	unsigned int preIdx;
	bool isMoving;//是否正在拖动琥珀
	CCSprite* amberAddLayer;
	CCArray *_slots;//三个技能槽
	SkillSlotAdapter* skillSlot;//技能槽适配类对象
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
	
	void menuGoBackCallback(CCObject* pSender);

	CCSprite* initbottomLayer();
	CCSprite* initTopTitle();//初始化顶部标题栏
	CCSprite* initAmplificationIcon(const char *name,float amplification);//初始化增幅图标

	CCSprite* getCurSprite(unsigned int idx);

public:
	static CCScene* scene();
};

#endif //__GAME_COLLECTION_CONTROLLER_H__
