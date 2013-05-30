#ifndef __ENEMY_INSECT_H__
#define __ENEMY_INSECT_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "Box2D\Box2D.h"
#include "GlobalDef.h"
#include "GameResinBallModel.h"
#include "GameInsectModel.h"

USING_NS_CC;
//虫子模型类，处理虫子运动等
//EnemyInsect.h
//
class EnemyInsect:public Animal 
{
protected:
	CC_SYNTHESIZE(CCNode*,_target,Target);//跟踪目标
	CC_SYNTHESIZE(int,attack,Attack);//攻击力
private:
	virtual void setBodySize();
public:
	EnemyInsect();
	virtual bool init(b2World* world,CCNode* target);
	virtual void setIdentify(){Animal::setIdentify();this->setAnimalType(ID_ENEMY_ANIMAL);this->setInsectType(ID_ENEMY);};
	LAYER_CREATE_FUNC_DOUBLE_PARAM(EnemyInsect,b2World*,world,CCNode*,target);
	
	virtual bool changeMoveWhenDone();//当一个move结束时开启下一个
	virtual bool startAction(int actionId);//开始某个行为
	virtual void initMovement();

	//攻击树脂球相关方法
	void startAttack();//开始攻击
	void endAttack();//结束攻击
	void attacking(float dt);//攻击一次
};

#endif //__ENEMY_INSECT_H__
