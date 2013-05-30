#ifndef __ENEMY_INSECT_H__
#define __ENEMY_INSECT_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "Box2D\Box2D.h"
#include "GlobalDef.h"
#include "GameResinBallModel.h"
#include "GameInsectModel.h"

USING_NS_CC;
//����ģ���࣬��������˶���
//EnemyInsect.h
//
class EnemyInsect:public Animal 
{
protected:
	CC_SYNTHESIZE(CCNode*,_target,Target);//����Ŀ��
	CC_SYNTHESIZE(int,attack,Attack);//������
private:
	virtual void setBodySize();
public:
	EnemyInsect();
	virtual bool init(b2World* world,CCNode* target);
	virtual void setIdentify(){Animal::setIdentify();this->setAnimalType(ID_ENEMY_ANIMAL);this->setInsectType(ID_ENEMY);};
	LAYER_CREATE_FUNC_DOUBLE_PARAM(EnemyInsect,b2World*,world,CCNode*,target);
	
	virtual bool changeMoveWhenDone();//��һ��move����ʱ������һ��
	virtual bool startAction(int actionId);//��ʼĳ����Ϊ
	virtual void initMovement();

	//������֬����ط���
	void startAttack();//��ʼ����
	void endAttack();//��������
	void attacking(float dt);//����һ��
};

#endif //__ENEMY_INSECT_H__
