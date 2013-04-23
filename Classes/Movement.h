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
	CC_SYNTHESIZE(bool,isStop,IsStop);//是否停止
	CC_SYNTHESIZE(bool,isRunning,IsRunning);//是否停止
	CC_SYNTHESIZE(BaseMove*, nextMove,NextMove);

	CC_SYNTHESIZE(int,level,Level);//此move的等级

	virtual bool init(CCPoint pos,float angle,float baseVelocity);
public:
	static BaseMove* create(CCPoint pos,float angle,float baseVelocity);
	void setPosAngle(CCPoint pos,float angle);
	
	bool isDone();//此运动是否结束？
	virtual void start();//开始运动
	void startNextMove();
	virtual void stop();//停止运动
	void restart(CCPoint pos,float angle);
	virtual void initAction(){action=NULL;};//初始化action
	void setAction(CCAction* ac);
};

//正常运动，即随机运动
class NormalMove:public BaseMove
{
protected:
//	CCPoint pos;//当前位置
//	float angle;//当前方向,与x轴正向顺时针夹角
	
public:
	CREATE_FUNC_THREE_PARAM(NormalMove,CCPoint,pos,float,angle,float,baseVelocity);
	NormalMove(){this->setLevel(NORMAL_LV);};
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
//以下将是一些组合行为,只关心节点的坐标和方向的改变
//
*/

//
//
//

#endif //__MOVEMENT_H__