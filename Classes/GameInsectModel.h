#ifndef __GAME_INSECT_MODEL_H__
#define __GAME_INSECT_MODEL_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "Box2D\Box2D.h"
#include "GlobalDef.h"
#include "Movement.h"

USING_NS_CC;

//���ж���ĸ��� 
class Animal :public cocos2d::CCNode
{
public:
	void distributeID();
private:
	CC_SYNTHESIZE(GameModelDelegate*,_observerDelegate,Observer);//����ģ�͵Ĺ۲��ߣ�һ��Ϊ���ӵ�view����
	
protected:
	CC_SYNTHESIZE(int,_id,ID);//ÿ�����Ӷ����Լ���һ�޶���id�����֤��
	CC_SYNTHESIZE(bool,_bounding,Bounding);//�Ƿ����ڴ����ص���
	CC_SYNTHESIZE(int,_insectType,InsectType);//��������
	CC_SYNTHESIZE(int,_animalType,AnimalType);//�������ͣ����˺���ʳ����
	CCDictionary *moves;//����ԭ�Ӷ�������
	CC_SYNTHESIZE(unsigned int, moveId,MoveId);//���ӵ�ǰԭ�Ӷ���
	CC_SYNTHESIZE(unsigned int,_actionType,ActionType);//���ӵ�ǰ��Ϊ����

	//��������ṹ
	CC_SYNTHESIZE(b2Body*,_body,Body);//���ӵ����� ����
	b2World* _world;
	CC_SYNTHESIZE(float,_bodyWidth,BodyWidth);//������
	CC_SYNTHESIZE(float,_bodyHeight,BodyHeight);//����߶�
	CC_SYNTHESIZE(int,rushZone,RushZone);//��ײ��λ
	//��������
	CC_PROPERTY(float,_velocity,Velocity);//�����ٶ�
	CC_SYNTHESIZE(float,_baseVelocity,BaseVelocity);
	CC_SYNTHESIZE(float,blood,Blood);//Ѫ��
	CC_SYNTHESIZE(bool,isAlive,IsAlive);//�����Ƿ���ţ�

private://��ʼ���������Եķ���
	void randomPositionAndAngle();//������ӵ�λ�úͽǶ�
	virtual void setBodySize(){};//���������С
public:
	Animal();
	virtual ~Animal();
	void clean();//�ڴ�����
	void deleteSelf();//�������ۼ������������ط�����������ĵط�ȫ����ֵΪNULL
	virtual bool init(b2World* world);
	virtual void setIdentify(){ this->distributeID(); };//���ñ�ʶ,����������
	CREATE_FUNC_ONE_PARAM(Animal,b2World*,world);
	void initObserver(GameModelDelegate* pDelegate);//��ʼ���۲��ߣ�һ��Ϊ��Model��Ӧ��View
protected:
	//������Ϊ�����ķ���
	virtual void initMovement();//��ʼ�������ԭ�Ӷ�������
	void addMove(BaseByTimeMove* move,int key);
	BaseByTimeMove* getCurMove();//���ص�ǰԭ�Ӷ���
	bool outOfSize(int id);//���ش�id�Ƿ�Խ��
	void replaceMovement(int id);//�滻ԭ�Ӷ���
	bool isMoveFinished();//�Ƿ�����Ȼֹͣmove
	bool isHigher(int id);//�Ƚ�������Ϊ�ȼ����µĸ��򷵻�true
	
	virtual bool changeMoveWhenDone();//��һ��move����ʱ������һ��

	void setBox2dPosToCos();//��box2d�����bodyλ�ø���cocos2d��ע��ת��ê��λ��
	void setPositionAngle();//����λ�úͽǶȣ�ͬʱ����cocos��box2d
	void setVelocityToBox2D();//��cocos���ٶ�ͬ����box2d
public:
	virtual void update(float dt);
	CCPoint getCenterPoint();//��ȡ��body������
	BaseByTimeMove* getMoveForKey(int key);//��key�õ���Ӧԭ�Ӷ���
	void stopMove();//ֹͣ��ǰԭ�Ӷ���
	void startMove(int id);//��ʼid��Ӧԭ�Ӷ���
	virtual bool startAction(int actionId);//��ʼĳ����Ϊ
	bool replaceActionIfLevelUp(int actionId);//������Ϊ�ȼ����,�ɹ�����true��ʧ�ܷ���false
	bool isOutOfBlood();//�Ƿ����
	virtual void finishMove();//����move
	void finishCurAction();
public://�й���ײ����һЩ����
	void beginContact();//���ÿ�ʼ��ײ
	void endContact();//���ý�����ײ
	void preSolve();
	
};


//����ģ���࣬��������˶���
class BaseInsect :public Animal
{
protected:
	CC_SYNTHESIZE(bool,_isFollowed,Followed);//�˳����Ƿ����ڱ��˸���
	CC_SYNTHESIZE(bool,_kissEnable,KissEnable);//�Ƿ���������
	CC_SYNTHESIZE(BaseInsect*,_target,Target);//Ŀ�꣨����Ŀ�꣬����Ŀ�꣬��˭��������˭̽���ȣ�
	CC_SYNTHESIZE(float,gas,Gas);//��
	CC_SYNTHESIZE(bool,isCovered,Covered);//�Ƿ���֬����
	CC_SYNTHESIZE(int,nextActionId,NextActionID);//������Ϊִ�н�������һ��Ϊid
public:
	BaseInsect();
	virtual ~BaseInsect();
	virtual bool init(b2World* world);
	virtual void setIdentify(){Animal::setIdentify();this->setAnimalType(ID_FRIEND_ANIMAL);};//���ñ�ʶ
	CREATE_FUNC_ONE_PARAM(BaseInsect,b2World*,world);
protected:
	void turnToCenter(float dt);//ת����Ļ����
	virtual void initMovement();
	virtual bool changeMoveWhenDone();//��һ��move����ʱ������һ��
	virtual bool startAction(int actionId);//��ʼĳ����Ϊ
	void doAfterGrope();//������Ϊ������ִ��
public:
	virtual void update(float dt);
	bool replaceAction(int id,BaseInsect* node);
	void covered(float attack);
	bool isOutOfGas();


};

//mantisInsect.h
//
class MantisInsect:public BaseInsect{
private:
	virtual void setBodySize();
protected:	
	CC_SYNTHESIZE(float ,attack,Attack);//������

public:
	MantisInsect();
	virtual bool init(b2World* world);
	virtual void setIdentify(){BaseInsect::setIdentify();this->setInsectType(ID_MANTIS);}
	CREATE_FUNC_ONE_PARAM(MantisInsect,b2World*,world);
	
	virtual void initMovement();
	virtual bool changeMoveWhenDone();//��һ��move����ʱ������һ��
	virtual bool startAction(int actionId);//��ʼĳ����Ϊ
};


//AntInsect.h ����
class AntInsect:public BaseInsect{
private:
	virtual void setBodySize();
public:
	AntInsect();
	virtual void setIdentify(){BaseInsect::setIdentify();this->setInsectType(ID_ANT);}
	CREATE_FUNC_ONE_PARAM(AntInsect,b2World*,world);

	virtual bool changeMoveWhenDone();//��һ��move����ʱ������һ��
	virtual bool startAction(int actionId);//��ʼĳ����Ϊ
	void attacked(float attack);//���ϱ�����һ��
};


#endif //__GAME_INSECT_MODEL_H__
