#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include "cocos2d.h"
#include "GlobalDef.h"
#include "Constant.h"

USING_NS_CC;

class BaseMove:public CCNode{
protected:
	//��ǰλ�ú�������CCNode��λ�úͼ���
	CC_SYNTHESIZE(float, baseVelocity,BaseVelocity);//�����ٶ�
	CC_SYNTHESIZE(float, realVelocity,RealVelocity);//ʵ���ٶ�

	CCAction* action;
	CC_SYNTHESIZE(bool,isStop,IsStop);//�Ƿ�ֹͣ
	CC_SYNTHESIZE(bool,isRunning,IsRunning);//�Ƿ�ֹͣ
	CC_SYNTHESIZE(BaseMove*, nextMove,NextMove);

	CC_SYNTHESIZE(int,level,Level);//��move�ĵȼ�

	virtual bool init(CCPoint pos,float angle,float baseVelocity);
public:
	static BaseMove* create(CCPoint pos,float angle,float baseVelocity);
	void setPosAngle(CCPoint pos,float angle);
	
	bool isDone();//���˶��Ƿ������
	virtual void start();//��ʼ�˶�
	void startNextMove();
	virtual void stop();//ֹͣ�˶�
	void restart(CCPoint pos,float angle);
	virtual void initAction(){action=NULL;};//��ʼ��action
	void setAction(CCAction* ac);
};

//�����˶���������˶�
class NormalMove:public BaseMove
{
protected:
//	CCPoint pos;//��ǰλ��
//	float angle;//��ǰ����,��x������˳ʱ��н�
	
public:
	CREATE_FUNC_THREE_PARAM(NormalMove,CCPoint,pos,float,angle,float,baseVelocity);
	NormalMove(){this->setLevel(NORMAL_LV);};
	virtual ~NormalMove();
	virtual void initAction();
};

//
//RotateMove.h ԭ����ת�˶�
//
class RotateMove:public BaseMove
{
protected:
	
public:
	CREATE_FUNC_THREE_PARAM(RotateMove,CCPoint,pos,float,angle,float,baseVelocity);
	RotateMove(){this->setLevel(ROTATE_LV);};

	virtual void initAction();
};

//
//StepBackMove.h
//
class StepBackMove:public BaseMove
{
protected:
	
public:
	CREATE_FUNC_THREE_PARAM(StepBackMove,CCPoint,pos,float,angle,float,baseVelocity);
	StepBackMove(){this->setLevel(STEPBACK_LV);};

	virtual void initAction();
};

//
//FollowMove.h
//
class FollowMove:public BaseMove
{
protected:
	CC_SYNTHESIZE(CCNode*,target,Target);
	CC_SYNTHESIZE(float, duration,Duration);
public:
	FollowMove():target(NULL),duration(0){this->setLevel(FOLLOW_LV);};
	CREATE_FUNC_THREE_PARAM(FollowMove,CCPoint,pos,float,angle,float,baseVelocity);

	virtual void initAction();
	void initTarget(CCNode* target);
	void start();
	void stop();

};




/*
//���½���һЩ�����Ϊ,ֻ���Ľڵ������ͷ���ĸı�
//
*/

//
//
//

#endif //__MOVEMENT_H__