#ifndef __GAME_WELCOME_CONTROLLER_H__
#define __GAME_WELCOME_CONTROLLER_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "vector"
#include "StaticLayer.h"

USING_NS_CC;

class GameWelcomeController :public CCLayer
{
public:
	//StaticLayer* buttonLayer;//��ť��

public:
	GameWelcomeController();
	virtual ~GameWelcomeController();
	CREATE_FUNC(GameWelcomeController);
	virtual bool init();
	//init������; ���Ǽ���CCTransitionScene���ڹ��ɳ�����ʼ�����  
	void onEnter();
	void onEnterTransitionDidFinish();
	//onEnter�Ժ󽫻���ô˷��� ��ʹ��CCTransitionScene,�����ڹ���Ч�������Ժ���ô˷��� 
	void onExitTransitionDidStart();
	void onExit();// �ڵ����dealloc����֮ǰ������ô˷���   
    //���ʹ����CCTransitionScene,�����ڹ���Ч�������Ժ���ô˷���  

	virtual void update(float dt);
	
    void menuStartCallback(CCObject* pSender);

	void initScrollView();

public:
	static CCScene* scene();
};

#endif //__GAME_WELCOME_CONTROLLER_H__
