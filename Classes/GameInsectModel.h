#ifndef __GAME_INSECT_MODEL_H__
#define __GAME_INSECT_MODEL_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "Box2D\Box2D.h"
#include "GlobalDef.h"
#include "Movement.h"

USING_NS_CC;
//虫子模型类，处理虫子运动等
class BaseInsect :public cocos2d::CCNode
{
private:
	GameModelDelegate* _observerDelegate;//虫子模型的观察者，一般为虫子的view对象
	
protected:
	unsigned int moveLevel;//动作等级，越大越高
	int pauseMoveId;//暂停的move，等待恢复
	vector<BaseMove*> moves;
	unsigned int moveId;
	b2Body* _body;//虫子的身体
	b2World* _world;
	CC_PROPERTY(float,_velocity,Velocity);//虫子速度
protected:
	float blood;//血量
	CC_PROPERTY_READONLY(bool,isAlive,Alive);//虫子是否活着？

private://初始化的一些方法
	void randomPositionAndAngle();//随机虫子的位置和角度
	bool isHigherLevel(int id1,int id2);//此id是否比当前move更高
	bool outOfSize(int id);//返回此id是否越界
public:
	BaseInsect();
	virtual ~BaseInsect();
	virtual bool init(b2World* world);
	CREATE_FUNC_ONE_PARAM(BaseInsect,b2World*,world);
	void clean();
	void initObserver(GameModelDelegate* pDelegate);//初始化观察者，一般为与Model对应的View
	void initMovement();
	void replaceMovement(int id);//替换运动
	void stopMove();//停止运动
	void startMove();//开始运动
	bool isMoveDone();//是否已自然停止move
	
	CCPoint getCenterPoint();//获取此body的中心
	void setBox2dPosToCos();//把box2d世界的body位置赋给cocos2d，注意转化锚点位置
	void setPositionAngleToBox2D(float x,float y,float angle);//设置位置和角度到Box2D世界
	void setPositionAngle();
	void setVelocityToBox2D();

	void update(float dt);

public://有关碰撞检测的一些方法
	void beginContact();//设置开始碰撞
	void endContact();//设置结束碰撞
	void preSolve();
	
	void handleContact();//处理contact事件
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
	GameModelDelegate* _observerDelegate;//虫子模型的观察者，一般为虫子的view对象
protected:
	b2Body* _body;//虫子的身体
	b2World* _world;
	CC_PROPERTY(float,_velocity,Velocity);//虫子速度
	b2Fixture *visualField;//视野fixture
protected:
	float blood;//血量
	CC_PROPERTY_READONLY(bool,isAlive,Alive);//虫子是否活着？

private://初始化的一些方法
	void randomPositionAndAngle();//随机虫子的位置和角度
public:
	GameInsectModel();
	virtual ~GameInsectModel();
	virtual bool init(b2World* world);
	CREATE_FUNC_ONE_PARAM(GameInsectModel,b2World*,world)
	void clean();
	void initObserver(GameModelDelegate* pDelegate);//初始化观察者，一般为与Model对应的View
	void initVisualField();//初始化视野fixture
	
	CCPoint getCenterPoint();//获取此body的中心
	void setBox2dPosToCos();//把box2d世界的body位置赋给cocos2d，注意转化锚点位置
	void setPositionAngleToBox2D(float x,float y,float angle);//设置位置和角度到Box2D世界

	void update(float dt);
	void updateInsectState();//更新虫子的一些状态

	bool attacked(float attack);//扣除attack的血量,若血量扣除后低于0，则返回true，否则false
	void draw();

public://有关碰撞检测的一些方法
	void beginContact();//设置开始碰撞
	void endContact();//设置结束碰撞
	void moveRandom();//随机运动行为
	void moveStepBack(float duration);//后退运动行为
	void moveRotateBy();//旋转运动行为
	void runTuoYuan();
	void handleContact();//处理contact事件
	void stepBackInstant(float delta);//瞬间后退
	bool stop;
private:
	int16 contactStage;//碰撞阶段，0表示未开始，1表示碰撞中，2表示碰撞结束
	int16 moveState;//运动状态，目前仅有碰撞1、正常0
};

#endif //__GAME_INSECT_MODEL_H__
