#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "Box2D\Box2D.h"
#include "GlobalDef.h"
#include "GameResinBallModel.h"


USING_NS_CC;
//����ģ���࣬��������˶���
class Enemy :public cocos2d::CCNode
{
private:
	GameModelDelegate* _observerDelegate;//����ģ�͵Ĺ۲��ߣ�һ��Ϊ���ӵ�view����
protected:
	//CCSprite* sprite;
	float eatVelocity;
	bool isEating;
	float countTime;
	bool isCounting;
	b2Body* _body;//���ӵ�����
	b2World* _world;
	CC_PROPERTY(float,_velocity,Velocity);//�����ٶ�
	b2Fixture *visualField;//��Ұfixture
protected:
	float blood;//Ѫ��
	CC_PROPERTY_READONLY(bool,isAlive,Alive);//�����Ƿ���ţ�

private://��ʼ����һЩ����
	void randomPositionAndAngle();//������ӵ�λ�úͽǶ�
public:
	Enemy();
	virtual ~Enemy();
	virtual bool init(b2World* world);
	CREATE_FUNC_ONE_PARAM(Enemy,b2World*,world)
	void clean();
	void initObserver(GameModelDelegate* pDelegate);//��ʼ���۲��ߣ�һ��Ϊ��Model��Ӧ��View
	void initVisualField();//��ʼ����Ұfixture
	
	CCPoint getCenterPoint();//��ȡ��body������
	void setBox2dPosToCos();//��box2d�����bodyλ�ø���cocos2d��ע��ת��ê��λ��
	void setPositionAngleToBox2D(float x,float y,float angle);//����λ�úͽǶȵ�Box2D����

	void update(float dt);
	void updateInsectState();//���³��ӵ�һЩ״̬

	bool attacked(float attack);//�۳�attack��Ѫ��,��Ѫ���۳������0���򷵻�true������false
	void draw();

public://�й���ײ����һЩ����
	void beginContact();//���ÿ�ʼ��ײ
	void endContact();//���ý�����ײ
	void stepBack();//�˻�һ��
	void moveRandom();//����˶���Ϊ
	void moveStepBack(float duration);//�����˶���Ϊ
	void moveRotateBy();//��ת�˶���Ϊ
	void runTuoYuan();
	void handleContact();//����contact�¼�

	void followResin(CCPoint position);//������֬��
	void eatResin(GameResinBallModel* resin,float dt);
	void setEating(bool isEating);

private:
	int16 contactStage;//��ײ�׶Σ�0��ʾδ��ʼ��1��ʾ��ײ�У�2��ʾ��ײ����
	int16 moveState;//�˶�״̬��Ŀǰ������ײ1������0
};

#endif //__ENEMY_H__
