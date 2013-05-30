#include "Enemy.h"
#include "Constant.h"
#include "B2EasyBox2D.h"
#include "RandomBy.h"

USING_NS_CC;

const float PAUSETIME=5;//咬一口后停留时间
const int ATTACK_RATE=10;//攻击频率3秒钟
const int TAG_ATTACK_ACTION=1234;//攻击action

EnemyInsect::EnemyInsect(){
	this->setAttack(300);
	this->setBaseVelocity(120);//速度120
	_velocity=this->getBaseVelocity();
}
void EnemyInsect::setBodySize(){
	CCSprite* uang=CCSprite::createWithSpriteFrameName("uang_0.png");
	this->setBodyWidth(uang->getContentSize().width*0.5f);
	this->setBodyHeight(uang->getContentSize().height*0.7f);
}
bool EnemyInsect::init(b2World* world,CCNode* target){
	bool pRet=false;
	do{
		_target=target;
		CC_BREAK_IF(!Animal::init(world));

		b2Filter filter;
		filter.groupIndex=k_visualField;
		filter.categoryBits=k_enemyViewCategory;
		filter.maskBits=k_enemyViewMask;
		
		b2CircleShape shape;
		shape.m_p=b2Vec2(0,0);
		shape.m_radius=10;

		CCNode* node=CCNode::create();
		this->addChild(node,0,ID_ENEMY_VIEW);
			
			//3.创建刚体形状需求b2FixtureDef的子类
			b2FixtureDef fixtureRequest;
			fixtureRequest.isSensor=true;
			fixtureRequest.shape = &shape;
			fixtureRequest.filter=filter;
			fixtureRequest.userData=node;
		this->_body->CreateFixture(&fixtureRequest);
		pRet=true;
	}while(0);
	return pRet;
}

bool EnemyInsect::startAction(int actionId){//成功启动返回true，否则返回false
	if(Animal::startAction(actionId)) return true;
	this->setActionType(actionId);
	switch(actionId){
	case ID_ACTION_FOLLOW:
		{
		//	FollowWhenReachedMove* move=((FollowWhenReachedMove*)this->getMoveForKey(ID_FOLLOW_REACHED_MOVE));
			FollowByTimeMove* move=(FollowByTimeMove*)this->getMoveForKey(ID_FOLLOW_BYTIME_MOVE);
			move->setTarget(this->getTarget());
			move->setMinDistance(this->getBodyHeight()/2);
			this->startMove(ID_FOLLOW_BYTIME_MOVE);
			return true;
		}
/*	case ID_ACTION_STAY:
		this->setVelocity(0);
		CCLog("locate1:%f,%f",this->getPositionX(),this->getPositionY());
		this->getMoveForKey(ID_STAY_MOVE)->setDuration(PAUSETIME);
		this->startMove(ID_STAY_MOVE);
		return true;*/
	}
	return false;
}
bool EnemyInsect::changeMoveWhenDone(){//执行了，返回true，否则返回false
	if(Animal::changeMoveWhenDone()) return true;
	
	switch(this->getActionType()){
	case ID_ACTION_FOLLOW://追踪树脂球
		this->getCurMove()->setDuration(9999);
		this->startAction(ID_ACTION_FOLLOW);
		return true;
/*	case ID_ACTION_STAY://若停留状态结束，返回正常运动状态
		this->setVelocity(this->getBaseVelocity());
		CCLog("locate2:%f,%f",this->getPositionX(),this->getPositionY());
		this->startAction(ID_ACTION_NORMAL);
		return true;*/
	}
	return false;
}
void EnemyInsect::initMovement(){
	Animal::initMovement();
	((FollowWhenReachedMove*)(this->getMoveForKey(ID_FOLLOW_REACHED_MOVE)))->setTarget(this->getTarget());

	FollowByTimeMove* move=FollowByTimeMove::create(this->getVelocity(),this->getTarget());
	this->addMove(move,ID_FOLLOW_BYTIME_MOVE);

}


//攻击树脂相关方法
void EnemyInsect::startAttack(){
	((GameResinBallModel*)_target)->attackedByEnemy(this->getAttack());//攻击血量
	this->schedule(schedule_selector(EnemyInsect::attacking),ATTACK_RATE);//每ATTACK_RATE秒钟扣一滴血
}
void EnemyInsect::endAttack(){
	this->unschedule(schedule_selector(EnemyInsect::attacking));
}
void EnemyInsect::attacking(float dt){
	((GameResinBallModel*)_target)->attackedByEnemy(this->getAttack());//攻击血量
}