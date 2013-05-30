#ifndef __GAME_INSECT_MODEL_H__
#define __GAME_INSECT_MODEL_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "Box2D\Box2D.h"
#include "GlobalDef.h"
#include "Movement.h"

USING_NS_CC;

//所有动物的父类 
class Animal :public cocos2d::CCNode
{
public:
	void distributeID();
private:
	CC_SYNTHESIZE(GameModelDelegate*,_observerDelegate,Observer);//虫子模型的观察者，一般为虫子的view对象
	
protected:
	CC_SYNTHESIZE(int,_id,ID);//每个虫子都有自己独一无二的id（身份证）
	CC_SYNTHESIZE(bool,_bounding,Bounding);//是否正在处理重叠？
	CC_SYNTHESIZE(int,_insectType,InsectType);//虫子类型
	CC_SYNTHESIZE(int,_animalType,AnimalType);//动物类型（敌人和扑食对象）
	CCDictionary *moves;//虫子原子动作集合
	CC_SYNTHESIZE(unsigned int, moveId,MoveId);//虫子当前原子动作
	CC_SYNTHESIZE(unsigned int,_actionType,ActionType);//虫子当前行为类型

	//虫子身体结构
	CC_SYNTHESIZE(b2Body*,_body,Body);//虫子的身体 刚体
	b2World* _world;
	CC_SYNTHESIZE(float,_bodyWidth,BodyWidth);//身体宽度
	CC_SYNTHESIZE(float,_bodyHeight,BodyHeight);//身体高度
	CC_SYNTHESIZE(int,rushZone,RushZone);//被撞部位
	//虫子属性
	CC_PROPERTY(float,_velocity,Velocity);//虫子速度
	CC_SYNTHESIZE(float,_baseVelocity,BaseVelocity);
	CC_SYNTHESIZE(float,blood,Blood);//血量
	CC_SYNTHESIZE(bool,isAlive,IsAlive);//虫子是否活着？

private://初始化虫子属性的方法
	void randomPositionAndAngle();//随机虫子的位置和角度
	virtual void setBodySize(){};//设置身体大小
public:
	Animal();
	virtual ~Animal();
	void clean();//内存清理
	void deleteSelf();//清除自身痕迹，即把其他地方有引用自身的地方全都赋值为NULL
	virtual bool init(b2World* world);
	virtual void setIdentify(){ this->distributeID(); };//设置标识,即动物种类
	CREATE_FUNC_ONE_PARAM(Animal,b2World*,world);
	void initObserver(GameModelDelegate* pDelegate);//初始化观察者，一般为与Model对应的View
protected:
	//动物行为操作的方法
	virtual void initMovement();//初始化动物的原子动作序列
	void addMove(BaseByTimeMove* move,int key);
	BaseByTimeMove* getCurMove();//返回当前原子动作
	bool outOfSize(int id);//返回此id是否越界
	void replaceMovement(int id);//替换原子动作
	bool isMoveFinished();//是否已自然停止move
	bool isHigher(int id);//比较两个行为等级，新的高则返回true
	
	virtual bool changeMoveWhenDone();//当一个move结束时开启下一个

	void setBox2dPosToCos();//把box2d世界的body位置赋给cocos2d，注意转化锚点位置
	void setPositionAngle();//设置位置和角度，同时赋给cocos和box2d
	void setVelocityToBox2D();//把cocos的速度同步给box2d
public:
	virtual void update(float dt);
	CCPoint getCenterPoint();//获取此body的中心
	BaseByTimeMove* getMoveForKey(int key);//由key得到相应原子动作
	void stopMove();//停止当前原子动作
	void startMove(int id);//开始id相应原子动作
	virtual bool startAction(int actionId);//开始某个行为
	bool replaceActionIfLevelUp(int actionId);//根据行为等级替代,成功返回true，失败返回false
	bool isOutOfBlood();//是否活着
	virtual void finishMove();//结束move
	void finishCurAction();
public://有关碰撞检测的一些方法
	void beginContact();//设置开始碰撞
	void endContact();//设置结束碰撞
	void preSolve();
	
};


//虫子模型类，处理虫子运动等
class BaseInsect :public Animal
{
protected:
	CC_SYNTHESIZE(bool,_isFollowed,Followed);//此虫子是否正在被人跟踪
	CC_SYNTHESIZE(bool,_kissEnable,KissEnable);//是否允许亲吻
	CC_SYNTHESIZE(BaseInsect*,_target,Target);//目标（跟踪目标，攻击目标，被谁攻击，被谁探索等）
	CC_SYNTHESIZE(float,gas,Gas);//气
	CC_SYNTHESIZE(bool,isCovered,Covered);//是否被树脂覆盖
	CC_SYNTHESIZE(int,nextActionId,NextActionID);//摸索行为执行结束后下一行为id
public:
	BaseInsect();
	virtual ~BaseInsect();
	virtual bool init(b2World* world);
	virtual void setIdentify(){Animal::setIdentify();this->setAnimalType(ID_FRIEND_ANIMAL);};//设置标识
	CREATE_FUNC_ONE_PARAM(BaseInsect,b2World*,world);
protected:
	void turnToCenter(float dt);//转向屏幕中心
	virtual void initMovement();
	virtual bool changeMoveWhenDone();//当一个move结束时开启下一个
	virtual bool startAction(int actionId);//开始某个行为
	void doAfterGrope();//摸索行为结束后执行
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
	CC_SYNTHESIZE(float ,attack,Attack);//攻击力

public:
	MantisInsect();
	virtual bool init(b2World* world);
	virtual void setIdentify(){BaseInsect::setIdentify();this->setInsectType(ID_MANTIS);}
	CREATE_FUNC_ONE_PARAM(MantisInsect,b2World*,world);
	
	virtual void initMovement();
	virtual bool changeMoveWhenDone();//当一个move结束时开启下一个
	virtual bool startAction(int actionId);//开始某个行为
};


//AntInsect.h 蚂蚁
class AntInsect:public BaseInsect{
private:
	virtual void setBodySize();
public:
	AntInsect();
	virtual void setIdentify(){BaseInsect::setIdentify();this->setInsectType(ID_ANT);}
	CREATE_FUNC_ONE_PARAM(AntInsect,b2World*,world);

	virtual bool changeMoveWhenDone();//当一个move结束时开启下一个
	virtual bool startAction(int actionId);//开始某个行为
	void attacked(float attack);//蚂蚁被攻击一次
};


#endif //__GAME_INSECT_MODEL_H__
