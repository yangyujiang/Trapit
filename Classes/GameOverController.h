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
	//init������; ���Ǽ���CCTransitionScene���ڹ��ɳ�����ʼ�����  
	void onEnter();
	void onEnterTransitionDidFinish();
	//onEnter�Ժ󽫻���ô˷��� ��ʹ��CCTransitionScene,�����ڹ���Ч�������Ժ���ô˷��� 
	void onExitTransitionDidStart();
	void onExit();// �ڵ����dealloc����֮ǰ������ô˷���   
    //���ʹ����CCTransitionScene,�����ڹ���Ч�������Ժ���ô˷���  
	
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
