#ifndef __HELP_SCENE_H__
#define __HELP_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "../extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


class GalleryLayer : public cocos2d::CCLayer ,public CCScrollViewDelegate
{
public:
	GalleryLayer():isScrolling(false),m_nCurPage(1),m_nCount(3),space(100){};
    virtual bool init();  

    void menuCloseCallback(CCObject* pSender);

    CREATE_FUNC(GalleryLayer);

public:
	//scrollview������ʱ������
	void scrollViewDidScroll(CCScrollView* view);
	//scrollview���ŵ�ʱ������
	void scrollViewDidZoom(CCScrollView* view);

	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	void menuCallBack(CCObject* pSender);
	void endScroll();//��������

private:
	//�������ƻ����ľ���ͷ������ͼ��
	 void adjustScrollView(float offset);
	 CCScrollView *m_pScrollView;
	 CCPoint m_touchPoint;
	 CCPoint m_nextPoint;
	 unsigned int m_nCurPage;//��ǰ����ҳ
	 bool isScrolling;//�Ƿ����ڹ�����
	 unsigned int m_nCount;//ҳ��
	 unsigned int space;//�հ�
	 unsigned int scrollWidth;//scrollView�Ŀ��
	 unsigned int buttonWidth;//ͼ����
	 CCSize visibleSize;
};

class MyMenu:public CCMenu{
	virtual void registerWithTouchDispatcher(){
        //�������ȼ���Ϊ1��ֻҪ��CCScrollView�;Ϳ���
         CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, true);
     }
public:
	MyMenu(){CCMenu();};
	static MyMenu* MyMenu::create(CCMenuItem* item, ...)
{
    va_list args;
    va_start(args,item);
    MyMenu *pRet = new MyMenu();
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

	 bool MyMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){ 
      moved_=false;
      return CCMenu::ccTouchBegan(pTouch,pEvent);
 }
  
 void MyMenu::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
     moved_=true;
     CCMenu::ccTouchMoved(pTouch,pEvent);
 }
 
 void MyMenu::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){CCLog("end?%d",moved_);
     if(!moved_){
         CCMenu::ccTouchEnded(pTouch,pEvent);
	 }else
         m_eState = kCCMenuStateWaiting;
 }
public:
	bool moved_;//�Ƿ��ǻ���
};

#endif  