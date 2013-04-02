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
	int curPage;//��ǰҳ
	unsigned int space;//�հ�
	unsigned int nCount;//ҳ��
	int scrollViewWidth_2;//scrollView��ͼ���һ��
	int tableWidth;//��������

	bool isScrolling;//�Ƿ񻬶�

	CCPoint startPoint;//��ʼ��
public:
	GameMenuController();
	virtual ~GameMenuController();
	CREATE_FUNC(GameMenuController);
	virtual bool init();
	//init������; ���Ǽ���CCTransitionScene���ڹ��ɳ�����ʼ�����  
	void onEnter();
	void onEnterTransitionDidFinish();
	//onEnter�Ժ󽫻���ô˷��� ��ʹ��CCTransitionScene,�����ڹ���Ч�������Ժ���ô˷��� 
	void onExitTransitionDidStart();
	void onExit();// �ڵ����dealloc����֮ǰ������ô˷���   
    //���ʹ����CCTransitionScene,�����ڹ���Ч�������Ժ���ô˷���  

	virtual void update(float dt);
	
    void menuGoBackCallback(CCObject* pSender);
    void menuCollectCallback(CCObject* pSender);
    void menuClickCallback(CCObject* pSender);


	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	void adjustScrollView(float distance);

	void initMapScrollView();//��ʼ���Ҳ����
	void initLeftTable();//��ʼ��������

public:
	static CCScene* scene();
};

//ScrollMenu
class ScrollMenu:public CCMenu{
private:
	virtual void registerWithTouchDispatcher(){
        //�������ȼ���Ϊ1��ֻҪ��CCScrollView�;Ϳ���
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
	bool moved_;//�Ƿ��ǻ���
};


#endif //__GAME_MENU_CONTROLLER_H__
