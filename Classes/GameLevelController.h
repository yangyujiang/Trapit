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
	StaticLayer* buttonLayer;//��ť��

public:
	GameLevelController();
	virtual ~GameLevelController();
	CREATE_FUNC(GameLevelController);
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

#endif //__GAME_LEVEL_CONTROLLER_H__
