#ifndef __GAME_PLAY_CONTROLLER_H__
#define __GAME_PLAY_CONTROLLER_H__

#include "cocos2d.h"
#include "GamePlayView.h"
#include "GameInsectView.h"
#include "GameInsectModel.h"
#include "Delegate.h"
#include "vector"
#include "Box2D\Box2D.h"
#include "GameResinBallModel.h"
#include "Enemy.h"
#include "GameResinBallView.h"
#include "StaticLayer.h"
#include "myContactListener.h"

USING_NS_CC;


//AndroidKeyLayer.h
class AndroidKeyLayer:public CCLayer{
public :
	virtual void keyBackClicked()//Android ���ؼ�
	{
		CCDirector::sharedDirector()->end();
		CCLayer::keyBackClicked();
	}
	virtual void keyMenuClicked()//Android �˵���
	{
		CCDirector::sharedDirector()->pause();
		CCLayer::keyMenuClicked();
	}
};

class GamePlayController :public GameViewDelegate,public GameResinBallViewDelegate,public AndroidKeyLayer
{
protected:
	b2MouseJoint *_mouseJoint;//�����ã����ؽ�

protected:
	void createEnvir();
	void updateBox2DUserData();
public:
	b2World* _world;//box2d����
	myContactListener* contactListener;
	StaticLayer* buttonLayer;//��ť��
	MapLayer* mapLayer;//��ͼ��
	GameInsectView *_insectView;
	GameInsectModel *_insectModel;

	GameResinBallModel *_resinBallModel;
	GameResinBallView *_resinBallView;
	vector<Enemy*> enemys;
	vector<BaseInsect*> _insects;
	unsigned int amberCount;//�������õ�������
	const static int MIN_NUM=0;//����������

public:
	GamePlayController();
	virtual ~GamePlayController();
	CREATE_FUNC(GamePlayController);
	virtual bool init();

	void addInsect();//����һֻ����
	void updateInsectNumber();//�������������������������������ֵʱ����������
	virtual void update(float dt);
	void step(float dt);
	void afterStep(float dt);//// process collisions and result from callbacks called by the step

	virtual void draw();
	void createWrapWall();
	vector<b2Body*> bodys; 
public:
	static CCScene* scene();
	/*init������; ���Ǽ���CCTransitionScene���ڹ��ɳ�����ʼ����� 
     */  
	void onEnter();
	void onEnterTransitionDidFinish();
	/*onEnter�Ժ󽫻���ô˷��� 
     ��ʹ��CCTransitionScene,�����ڹ���Ч�������Ժ���ô˷��� 
     */  
	void onExitTransitionDidStart();
	void onExit();// �ڵ����dealloc����֮ǰ������ô˷���   
    //���ʹ����CCTransitionScene,�����ڹ���Ч�������Ժ���ô˷���   


	void testViewDelegate();

	void initResinBall();//��ʼ����֬����ģ��
	void initInsect();//��ʼ������

	//������Ӧ
	void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);
	
	//��дGameResinBallDelegate�ķ���
	void dealWithTouchesBegan(b2Vec2 locationWorld);
	void dealWithTouchesMoved(b2Vec2 locationWorld);
	void dealWithTouchesEnded();

	void shrinkCallBack(CCObject* pSender);

public:
	void checkGameOverAndDo();
	void menuGoBackCallback(CCObject* pSender);
	void menuGameOverCallback(CCObject* pSender);
};


#endif //__GAME_PLAY_CONTROLLER_H__
