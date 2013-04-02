#ifndef __GAME_MENU_CONTROLLER_H__
#define __GAME_MENU_CONTROLLER_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "vector"
#include "StaticLayer.h"
#include "../extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameMenuController :public CCLayer
{
protected:
	CCScrollView* scrollView;
	int curPage;//当前页
	unsigned int space;//空白
	unsigned int nCount;//页数
	int scrollViewWidth_2;//scrollView视图宽的一半
	int tableWidth;//左侧面板宽度

	bool isScrolling;//是否滑动

	CCPoint startPoint;//开始点
public:
	GameMenuController();
	virtual ~GameMenuController();
	CREATE_FUNC(GameMenuController);
	virtual bool init();
	//init后会调用; 若是加了CCTransitionScene后，在过渡场景开始后调用  
	void onEnter();
	void onEnterTransitionDidFinish();
	//onEnter以后将会调用此方法 若使用CCTransitionScene,将会在过渡效果结束以后调用此方法 
	void onExitTransitionDidStart();
	void onExit();// 节点调用dealloc方法之前将会调用此方法   
    //如果使用了CCTransitionScene,将会在过渡效果结束以后调用此方法  

	virtual void update(float dt);
	
    void menuGoBackCallback(CCObject* pSender);
    void menuCollectCallback(CCObject* pSender);
    void menuClickCallback(CCObject* pSender);


	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	void adjustScrollView(float distance);

	void initMapScrollView();//初始化右侧面板
	void initLeftTable();//初始化左侧面板

public:
	static CCScene* scene();
};

//ScrollMenu
class ScrollMenu:public CCMenu{
private:
	virtual void registerWithTouchDispatcher(){
        //这里优先级设为1，只要比CCScrollView低就可以
         CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, true);
     }
public:
	ScrollMenu(){CCMenu();};

	static ScrollMenu* ScrollMenu::create(){
	    return ScrollMenu::create(NULL, NULL);
	}
	
	static ScrollMenu* ScrollMenu::create(CCMenuItem* item, ...){
	  va_list args;
		va_start(args,item);
		ScrollMenu *pRet = new ScrollMenu();
		if (pRet && pRet->initWithItems(item, args))
		{
		    pRet->autorelease();
		    va_end(args);
		    return pRet;
		}
		va_end(args);
		CC_SAFE_DELETE(pRet);
		return NULL;
	}

	bool ScrollMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){ 
		moved_=false;
		return CCMenu::ccTouchBegan(pTouch,pEvent);
	}
  
	void ScrollMenu::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
		moved_=true;
		CCMenu::ccTouchMoved(pTouch,pEvent);
	}
 
	void ScrollMenu::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){CCLog("end?%d",moved_);
		if(!moved_){
			CCMenu::ccTouchEnded(pTouch,pEvent);
		}else
			m_eState = kCCMenuStateWaiting;
	}
protected:
	bool moved_;//是否是滑动
};


#endif //__GAME_MENU_CONTROLLER_H__
