#ifndef __GAME_INSECT_MODEL_H__
#define __GAME_INSECT_MODEL_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "Box2D\Box2D.h"
#include "GlobalDef.h"

USING_NS_CC;
//虫子模型类，处理虫子运动等
class GameInsectModel :public cocos2d::CCNode
{
private:
	GameModelDelegate* _observerDelegate;//虫子模型的观察者，一般为虫子的view对象
protected:
	b2Body* _body;//虫子的身体
	//b2Body* _world;
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
	void stepBack();//退回一步
	void moveRandom();//随机运动行为
	void moveStepBack(float duration);//后退运动行为
	void moveRotateBy();//旋转运动行为
	void runTuoYuan();
	void handleContact();//处理contact事件

private:
	int16 contactStage;//碰撞阶段，0表示未开始，1表示碰撞中，2表示碰撞结束
	int16 moveState;//运动状态，目前仅有碰撞1、正常0
};

#endif //__GAME_INSECT_MODEL_H__
