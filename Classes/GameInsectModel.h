#ifndef __GAME_INSECT_MODEL_H__
#define __GAME_INSECT_MODEL_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "Box2D\Box2D.h"
#include "GlobalDef.h"
#include "Movement.h"

USING_NS_CC;
//����ģ���࣬��������˶���
class BaseInsect :public cocos2d::CCNode
{
private:
	GameModelDelegate* _observerDelegate;//����ģ�͵Ĺ۲��ߣ�һ��Ϊ���ӵ�view����
	
protected:
	unsigned int moveLevel;//�����ȼ���Խ��Խ��
	int pauseMoveId;//��ͣ��move���ȴ��ָ�
	vector<BaseMove*> moves;
	unsigned int moveId;
	b2Body* _body;//���ӵ�����
	b2World* _world;
	CC_PROPERTY(float,_velocity,Velocity);//�����ٶ�
protected:
	float blood;//Ѫ��
	CC_PROPERTY_READONLY(bool,isAlive,Alive);//�����Ƿ���ţ�

private://��ʼ����һЩ����
	void randomPositionAndAngle();//������ӵ�λ�úͽǶ�
	bool isHigherLevel(int id1,int id2);//��id�Ƿ�ȵ�ǰmove����
	bool outOfSize(int id);//���ش�id�Ƿ�Խ��
public:
	BaseInsect();
	virtual ~BaseInsect();
	virtual bool init(b2World* world);
	CREATE_FUNC_ONE_PARAM(BaseInsect,b2World*,world);
	void clean();
	void initObserver(GameModelDelegate* pDelegate);//��ʼ���۲��ߣ�һ��Ϊ��Model��Ӧ��View
	void initMovement();
	void replaceMovement(int id);//�滻�˶�
	void stopMove();//ֹͣ�˶�
	void startMove();//��ʼ�˶�
	bool isMoveDone();//�Ƿ�����Ȼֹͣmove
	
	CCPoint getCenterPoint();//��ȡ��body������
	void setBox2dPosToCos();//��box2d�����bodyλ�ø���cocos2d��ע��ת��ê��λ��
	void setPositionAngleToBox2D(float x,float y,float angle);//����λ�úͽǶȵ�Box2D����
	void setPositionAngle();
	void setVelocityToBox2D();

	void update(float dt);

public://�й���ײ����һЩ����
	void beginContact();//���ÿ�ʼ��ײ
	void endContact();//���ý�����ײ
	void preSolve();
	
	void handleContact();//����contact�¼�
};

//mantisInsect.h
//
class MantisInsect:public BaseInsect{
protected:
	
public:
	virtual bool init(b2World* world);
	CREATE_FUNC_ONE_PARAM(MantisInsect,b2World*,world);

	void replaceMovement(int id,CCNode* node);
};


//
//GameInsectModel.cpp
//
class GameInsectModel :public BaseInsect
{
private:
	GameModelDelegate* _observerDelegate;//����ģ�͵Ĺ۲��ߣ�һ��Ϊ���ӵ�view����
protected:
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
	GameInsectModel();
	virtual ~GameInsectModel();
	virtual bool init(b2World* world);
	CREATE_FUNC_ONE_PARAM(GameInsectModel,b2World*,world)
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
	void moveRandom();//����˶���Ϊ
	void moveStepBack(float duration);//�����˶���Ϊ
	void moveRotateBy();//��ת�˶���Ϊ
	void runTuoYuan();
	void handleContact();//����contact�¼�
	void stepBackInstant(float delta);//˲�����
	bool stop;
private:
	int16 contactStage;//��ײ�׶Σ�0��ʾδ��ʼ��1��ʾ��ײ�У�2��ʾ��ײ����
	int16 moveState;//�˶�״̬��Ŀǰ������ײ1������0
};

#endif //__GAME_INSECT_MODEL_H__
