#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "../extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

// У�����������ٶ�
#define ADJUST_ANIM_VELOCITY 800;

class HelloWorld : public cocos2d::CCLayer, public CCScrollViewDelegate
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
public:
    void scrollViewDidScroll(CCScrollView* view);
    void scrollViewDidZoom(CCScrollView* view);
    
public:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    // �������¼�����ʱ��У��CCScrollView��λ��
    void adjustScrollView();
    
private:
    // �˵��ص�
    void menu1Callback(CCObject *pSender);
    void menu2Callback(CCObject *pSender);
    
private:
    // ��ʾ��ǰҳ
    CCLabelTTF* pLabel;
    
    CCScrollView *scrollView;
    
};

#endif // __HELLOWORLD_SCENE_H__
