#ifndef __GAME_RESIN_BALL_MODEL_H__
#define __GAME_RESIN_BALL_MODEL_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "Box2D\Box2D.h"
#include "GlobalDef.h"
#include "GameInsectModel.h"

USING_NS_CC;
//����ģ���࣬��������˶���
class GameResinBallModel :public cocos2d::CCNode
{
private:
	GameModelDelegate* _observerDelegate;//��֬��ģ�͵Ĺ۲��ߣ�һ��Ϊ��֬���view����
protected:
	b2World* _world;
	vector<b2Body*> _blocks;//Բ�������ķ�������
	vector<b2Joint*> _blockJoints;//����֮�����ת�ؽڵĴ洢����
	CC_PROPERTY_READONLY(b2Body*,_ballBody,BallBody);//ʵ�������
	//cocos2d::CCPoint _position;//��������λ��,��ʵ��Բ��Բ�����꣬��λ����,��Ϊ��Nodeλ��,�ʲ����ٶ���
	float32 _currentBlockHeight;//С���鳤��
	float32 _currentBlockWidth;//��ǰС������
	CC_PROPERTY_READONLY(float32,_radius,Radius);//��Χ�뾶
	CC_PROPERTY_READONLY(float,_ballRadius,BallRadius);//ʵ����뾶
	CCPoint lastPosition;//��һ֡��λ��

	b2Fixture* _sensorBall;//������֬������ӵ���ײ
protected:
	float attack;//��������ÿ�룩
	//float coverInsectTimeCount;//����ס���ӵ�ʱ���ۼ�
	//float isCoverInsect;//�Ƿ���ס���ӣ�
	GameInsectModel* coveredInsect;//�����ǵĳ���

public:
	GameResinBallModel();
	virtual ~GameResinBallModel();

	virtual bool init(b2World* world);
	CREATE_FUNC_ONE_PARAM(GameResinBallModel,b2World*,world);

	void initBallBody(float32 radius);//ʵ��Բ�뾶
	void createCircleBridge(int number,float32 radius);//��������
	void shrinkResinBallBody(float32 scale);//��С��֬��ģ�͸���
	
	void accelerateCallBack(cocos2d::CCAcceleration* pAccelerationValue);//�������仯ʱ����б�ֻ���Ļ���Ļص�����

	void update(float dt);
	CCPoint getDeltaPosition();//����λ�ñ仯��ƫ��

	void initObserver(GameModelDelegate* pDelegate);//��ʼ���۲��ߣ�һ��Ϊ��Model��Ӧ��View

public:
	void attackInsect(GameInsectModel* insect,float dt);//����������
	void myDraw();//������
	
public:
	void beginContact(GameInsectModel* insect);
	void endContact();
	void handleContactWithInsect(float dt);//��������ӵ���ײ�¼�
};

#endif //__GAME_RESIN_BALL_MODEL_H__
