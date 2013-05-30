#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include "cocos2d.h"
#include "GlobalDef.h"
#include "Constant.h"

USING_NS_CC;


class BaseMove:public CCNode{
protected:
	//当前位置和坐标用CCNode的位置和即可
	CC_SYNTHESIZE(float, baseVelocity,BaseVelocity);//基础速度
	CC_SYNTHESIZE(float, realVelocity,RealVelocity);//实际速度

	CCAction* action;
	CC_SYNTHESIZE(bool,stopped,Stopped);//是否停止
	CC_SYNTHESIZE(bool,finished,Finished);//是否结束
	CC_SYNTHESIZE(BaseMove*, nextMove,NextMove);

	CC_SYNTHESIZE(int,level,Level);//此move的等级

	virtual bool init(float baseVelocity);
public:
	~BaseMove();
	CREATE_FUNC_ONE_PARAM(BaseMove,float,baseVelocity);
	void setPosAngleVel(CCPoint pos,float angle,float vel);
	
	bool isDone();//此运动是否结束？
	void startNextMove();
	virtual void stop();//暂停运动
	virtual void finish();//结束运动
	virtual void start(CCPoint pos,float angle,float vel);
	virtual void initAction(){action=NULL;};//初始化action
	void setAction(CCAction* ac);
};

//以时间结束的基类
class BaseByTimeMove:public BaseMove{
protected:
	CC_SYNTHESIZE(float, duration,Duration);
public:
	CREATE_FUNC_ONE_PARAM(BaseByTimeMove,float,baseVel);
	virtual void stop();
	virtual void finish();//结束运动
	virtual void start(CCPoint pos,float angle,float vel);
	virtual void update(float dt);

};

//正常运动，即随机运动
class NormalMove:public BaseMove
{
protected:
	
public:
	CREATE_FUNC_ONE_PARAM(NormalMove,float,baseVelocity);
	NormalMove(){};
	virtual ~NormalMove();
	virtual void initAction();
};

//
//RotateMove.h 原地旋转运动
//
class RotateMove:public BaseMove
{
protected:
	
public:
	CREATE_FUNC_ONE_PARAM(RotateMove,float,baseVelocity);
	RotateMove(){};

	virtual void initAction();
};

class RotateToMove:public BaseByTimeMove
{
protected:
	CC_SYNTHESIZE(float,m_angle,Angle);
public:
	CREATE_FUNC_ONE_PARAM(RotateToMove,float,baseVelocity);
	RotateToMove(){this->setAngle(this->getRotation());}
	virtual void initAction();
};

class HeadRotateToMove:public BaseByTimeMove{
protected:
	CC_SYNTHESIZE(float,m_angle,Angle);
	CC_SYNTHESIZE(float,m_length,Length);//长度
	virtual bool init(float length);
public:
	CREATE_FUNC_ONE_PARAM(HeadRotateToMove,float,length);
	HeadRotateToMove(){this->setAngle(this->getRotation());}
	virtual void initAction();
};

//
//StepBackMove.h
//
class StepBackMove:public BaseMove
{
protected:
	
public:
	CREATE_FUNC_ONE_PARAM(StepBackMove,float,baseVelocity);
	StepBackMove(){};
	virtual void initAction();
};


//
//FollowForeverMove.h
//
class FollowForeverMove:public BaseMove
{
protected:
	CC_SYNTHESIZE(CCNode*,target,Target);
	virtual bool init(float baseVel,CCNode* target);
public:
	FollowForeverMove():target(NULL){this->setLevel(FOLLOW_LV);};
	LAYER_CREATE_FUNC_DOUBLE_PARAM(FollowForeverMove,float,baseVelocity,CCNode*,target);

	virtual void initAction();
};
//
//FollowMove.h
//
class FollowMove:public FollowForeverMove
{
protected:
	CC_SYNTHESIZE(float, duration,Duration);
public:
	FollowMove():duration(CCRANDOM_0_1()+1){this->setLevel(FOLLOW_LV);};
	LAYER_CREATE_FUNC_DOUBLE_PARAM(FollowMove,float,baseVelocity,CCNode*,target);

	virtual void initAction();
	virtual void stop();
	virtual void start(CCPoint pos,float angle,float vel);
	void update(float dt);
};


//FollowByTimeMove.h
class FollowByTimeMove:public BaseByTimeMove{
protected:
	CC_SYNTHESIZE(CCNode*,_followTarget,Target);
	CC_SYNTHESIZE(int,min_dis,MinDistance);
	virtual bool init(float baseVel,CCNode* target);
public:
	FollowByTimeMove(){this->setDuration(99999);this->setLevel(FOLLOW_LV);};
	LAYER_CREATE_FUNC_DOUBLE_PARAM(FollowByTimeMove,float,baseVelocity,CCNode*,target);
	virtual void initAction();
};

//FollowWhenReachedMove.h
class FollowWhenReachedMove:public FollowByTimeMove
{
protected:
public:
	FollowWhenReachedMove();
	LAYER_CREATE_FUNC_DOUBLE_PARAM(FollowWhenReachedMove,float,baseVelocity,CCNode*,target);
	
	virtual void update(float dt);
};

//LookAtMove.h
class LookAtMove:public FollowByTimeMove{
protected:
public:
	LookAtMove(){this->setDuration(99999);this->setLevel(FOLLOW_LV);};
	LAYER_CREATE_FUNC_DOUBLE_PARAM(LookAtMove,float,baseVelocity,CCNode*,target);

	virtual void initAction();
};


//
//摸索、停留move  t秒
//
class StayMove:public BaseByTimeMove
{
public:
	StayMove(){this->setDuration(CCRANDOM_0_1()+2);this->setLevel(STAY_LV);};
	CREATE_FUNC_ONE_PARAM(StayMove,float,baseVelocity);

	virtual void initAction();
};

class AttackMove:public StayMove{
public:
	CREATE_FUNC_ONE_PARAM(AttackMove,float,baseVelocity);
};
class GropeMove:public StayMove{
public:
	CREATE_FUNC_ONE_PARAM(GropeMove,float,baseVelocity);
};
class KissMove:public StayMove{
public:
	CREATE_FUNC_ONE_PARAM(KissMove,float,baseVelocity);
};

//
//NormalByTimeMove.h  正常运动，包涵随机运动，停留，后退，转圈等
//

class NormalByTimeMove:public BaseByTimeMove{
protected:
	CC_SYNTHESIZE(float,atomDuration,AtomDuration);
public:
	NormalByTimeMove(){this->setDuration(9999);};
	CREATE_FUNC_ONE_PARAM(NormalByTimeMove,float,baseVel);
	virtual void initAction();
	void createActionByRandom();
	void endAtomMove();
	float time;
	virtual void finish();
	virtual void stop();
	virtual void update(float dt);
//	virtual void start(CCPoint pos,float angle,float vel);
};

/*
//以下将是一些组合行为,只关心节点的坐标和方向的改变
//
*/

//
//
//

#endif //__MOVEMENT_H__