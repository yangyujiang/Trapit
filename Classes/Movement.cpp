#include "Movement.h"
#include "RandomBy.h"

USING_NS_CC;

bool BaseMove::init(CCPoint pos,float angle,float baseVelocity){
	bool pRet=false;
	do{
		this->setPosAngle(pos,angle);
		this->setBaseVelocity(baseVelocity);
		this->setRealVelocity(baseVelocity);
		this->initAction();
		this->setIsStop(true);
		this->setIsRunning(false);
		this->nextMove=NULL;
		pRet=true;
	}while(0);
	return pRet;
}

BaseMove* BaseMove::create(CCPoint pos,float angle,float baseVelocity){
	BaseMove* baseMove=new BaseMove;
	if(baseMove&&baseMove->init(pos,angle,baseVelocity)){
		baseMove->autorelease();
		return baseMove;
	}
	CC_SAFE_DELETE(baseMove);
	return NULL;
}

void BaseMove::setPosAngle(CCPoint pos,float angle){
	this->setPosition(pos);
	this->setRotation(angle);
}

bool BaseMove::isDone(){
	if(nextMove!=NULL)//��������һ��move���������move����
		return nextMove->isDone();
	if(this->getIsRunning()&&this->getActionByTag(111)==NULL){
		return true;
	}
	return false;
}

void BaseMove::start(){
	if(action!=NULL){
		action->setTag(111);
		this->runAction(action);
		this->setIsStop(false);
		this->setIsRunning(true);
		CCLog("start action");
	}
}
void BaseMove::startNextMove(){
	if(nextMove!=NULL){
		nextMove->start();
	}
}
void BaseMove::stop(){
	if(action!=NULL){
		this->stopAction(action);
		action=NULL;
		this->setIsStop(true);
		CCLog("stop action");
	}
}
void BaseMove::restart(CCPoint pos,float angle){
	this->setPosAngle(pos,angle);
	this->initAction();
	this->start();
}
void BaseMove::setAction(CCAction* ac){
	if(nextMove!=NULL){
		CCCallFunc *funCall=CCCallFunc::create(this,callfunc_selector(BaseMove::startNextMove));//�ص��˺���
		action=CCSequence::create((CCActionInterval*)ac,funCall,NULL);
	}else 
		action=ac;
}

//
//NormalMove.cpp
//

NormalMove::~NormalMove(void)
{
}

void NormalMove::initAction(){
	this->setAction(CCRandomBy::create(1000,realVelocity));
}

//
//RotateMove.cpp
//
void RotateMove::initAction(){
	float delta=CCRANDOM_MINUS1_1()*90;//�����ƫת�Ƕ�-90��90��
	float duration=fabs(delta)/this->getRealVelocity();//�����ٶȼ���ƫ��ʱ��
	CCActionInterval* rotateBy=CCRotateBy::create(duration,delta);
	this->setAction(rotateBy);
}

//
//StepBackMove.cpp �˺�1-2��
//
void StepBackMove::initAction(){
	CCActionInterval* stepBack=CCStepBack::create(CCRANDOM_0_1()+1,this->getRealVelocity());
	this->setAction(stepBack);
	CCLog("init stepBack");
}

//
//FollowMove.cpp ����Nʱ��
//
void FollowMove::initAction(){
	if(this->getTarget()==NULL) return;
	CCActionInterval* follow=CCFollowSprite::create(duration,this->getRealVelocity(),this->getTarget());//2-7��
	this->setAction(follow);
	CCLog("init follow");
}
void FollowMove::initTarget(CCNode* target){
	this->setTarget(target);
	this->setDuration(CCRANDOM_0_1()*5+5);
}
void FollowMove::start(){//��дstart����
	this->initAction();
	BaseMove::start();
}
void FollowMove::stop(){
	if(action!=NULL&&!this->isDone()){
		this->setDuration(this->getDuration()-((CCActionInterval*)action)->getElapsed());
		BaseMove::stop();
	}
}


