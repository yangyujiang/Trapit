#include "Movement.h"
#include "RandomBy.h"
#include "MyMath.h"

USING_NS_CC;

//BaseMove.cpp
BaseMove::~BaseMove(){
	this->finish();
}
bool BaseMove::init(float baseVelocity){
	bool pRet=false;
	do{
		this->setBaseVelocity(baseVelocity);
		this->setRealVelocity(baseVelocity);
		this->setStopped(true);
		this->setFinished(false);
		this->nextMove=NULL;
		this->action=NULL;
		pRet=true;
	}while(0);
	return pRet;
}
void BaseMove::setPosAngleVel(CCPoint pos,float angle,float vel){
	this->setPosition(pos);
	this->setRotation(angle);
	this->setRealVelocity(vel);
}

bool BaseMove::isDone(){
	if(nextMove!=NULL)//若存在下一个move，即是组合move，则
		return nextMove->isDone();
	if(this->getFinished()){
		return true;
	}
	return false;
}
void BaseMove::startNextMove(){
	if(nextMove!=NULL){
		nextMove->start(this->getPosition(),this->getRotation(),this->getRealVelocity());
	}
}
void BaseMove::stop(){
	if(action!=NULL){
		this->stopActionByTag(111);//>stopAction(action);
		this->setStopped(true);
		CCLog("stop action");
	}
}
void BaseMove::finish(){
	this->stop();
	this->setFinished(true);
}
void BaseMove::start(CCPoint pos,float angle,float vel){
	CCLog("start action");
	this->setPosAngleVel(pos,angle,vel);
	this->initAction();
	if(action!=NULL){
		action->setTag(111);
		this->runAction(action);
		this->setStopped(false);
		this->setFinished(false);

	}
}
void BaseMove::setAction(CCAction* ac){
	if(nextMove!=NULL){
		CCCallFunc *funCall=CCCallFunc::create(this,callfunc_selector(BaseMove::startNextMove));//回调此函数
		action=CCSequence::create((CCActionInterval*)ac,funCall,NULL);
	}else{
		action=ac;
	}
}

//
//BaseByTimeMove.cpp
//

void BaseByTimeMove::stop(){
	BaseMove::stop();
	this->unschedule(schedule_selector(BaseByTimeMove::update));
}
void BaseByTimeMove::finish(){
	BaseMove::finish();
	this->setDuration(9999);
	this->unschedule(schedule_selector(BaseByTimeMove::update));
}
void BaseByTimeMove::start(CCPoint pos,float angle,float vel){
	BaseMove::start(pos,angle,vel);
	this->schedule(schedule_selector(BaseByTimeMove::update));
}
void BaseByTimeMove::update(float dt){
	duration-=dt;
	if(duration<=0){
		this->finish();
	}
}


//
//NormalMove.cpp
//

NormalMove::~NormalMove(void)
{
}

void NormalMove::initAction(){
	this->setAction(CCRandomBy::create(1000,realVelocity));
	CCLog("init normal action");
}

//
//RotateMove.cpp
//
void RotateMove::initAction(){
	float delta=CCRANDOM_MINUS1_1()*90;//随机化偏转角度-90到90度
	float duration=fabs(delta)/this->getRealVelocity();//根据速度计算偏移时间
	CCActionInterval* rotateBy=CCRotateBy::create(duration,delta);
	this->setAction(rotateBy);
	CCLog("init rotate action");
}
//
//RotateToMove.cpp
//
void RotateToMove::initAction(){
	CCActionInterval* rotateTo=CCRotateTo::create(this->getDuration(),this->getRotation());
	this->setAction(rotateTo);
	CCLog("delAngle:%f",this->getAngle());
	CCLog("duration:%f",duration);
	CCLog("init rotateTo action");
}

//HeadRotateToMove.cpp
bool HeadRotateToMove::init(float length){
	bool pRet=false;
	do{
		CC_BREAK_IF(!BaseByTimeMove::init(0));
		this->setLength(length);
		pRet=true;
	}while(0);
	return pRet;
}
void HeadRotateToMove::initAction(){
	CCPoint originPos=this->getPosition();
	CCPoint desPos;
	float originAngle=this->getRotation();
	float originRadians=CC_DEGREES_TO_RADIANS(originAngle);
	float desAngle=this->getAngle();
	float desRadians=CC_DEGREES_TO_RADIANS(desAngle);
	float length=this->getLength();
	CCPoint center=ccp(originPos.x+length*sin(originRadians),originPos.y+length*cos(originRadians));
	desPos=ccp(center.x-length*sin(desRadians),center.y-length*cos(desRadians));

	this->setAction(CCHeadRotateTo::create(this->getDuration(),this->getLength(),this->getAngle()));
	CCLog("start headRotateToMove");
	CCLog("originPos:%f,%f",originPos.x,originPos.y);
	CCLog("desPos:%f,%f",desPos.x,desPos.y);
	CCLog("centerPos:%f,%f",center.x,center.y);
	CCLog("originAngle:%f,desAngle:%f",originAngle,desAngle);
	CCLog("length:%f",length);
}

//
//StepBackMove.cpp 退后1-2秒
//
void StepBackMove::initAction(){
	CCActionInterval* stepBack=CCStepBack::create(CCRANDOM_0_1()+1,this->getRealVelocity());
	this->setAction(stepBack);
	CCLog("init stepBack action");
}

//
//FollowForeverMove.cpp 永远跟踪
//
bool FollowForeverMove::init(float baseVel,CCNode* target){
	bool pRet=false;
	do{
		this->setTarget(target);
		CC_BREAK_IF(!BaseMove::init(baseVel));
		pRet=true;
	}while(0);
	return pRet;
}
void FollowForeverMove::initAction(){
	if(this->getTarget()==NULL) return;
	CCActionInterval* follow=CCFollowSprite::create(1000,this->getRealVelocity(),this->getTarget(),0);//2-7秒
	//CCActionInterval* follows=CCRepeatForever::create(follow);
	
	this->setAction(follow);
	CCLog("init follow forever");
}

//
//FollowByTimeMove.cpp 跟踪到时间t耗完为止
//
bool FollowByTimeMove::init(float baseVel,CCNode* target){
	bool pRet=false;
	do{
		this->setTarget(target);
		CC_BREAK_IF(!BaseByTimeMove::init(baseVel));
		pRet=true;
	}while(0);
	return pRet;
}
void FollowByTimeMove::initAction(){
	float rotateDuration=CCRANDOM_0_1()+0.1f;
	CCActionInterval* rotateTo=CCRotateTo::create(rotateDuration,MyMath::getDirection(this->getPosition(),this->getTarget()->getPosition()));
	CCActionInterval* follow=CCFollowSprite::create(duration-rotateDuration,this->getRealVelocity(),this->getTarget(),this->getMinDistance());//2-7秒
	CCAction* action=CCSequence::create(rotateTo,follow,NULL);
	
	this->setAction(action);
	CCLog("init FollowByTimeMove action");
}


//FollowWhenReachedMove.cpp
//

FollowWhenReachedMove::FollowWhenReachedMove(){
	this->setDuration(99999);
	this->setMinDistance(100);
}
void FollowWhenReachedMove::update(float dt){
	if(this->getTarget()==NULL) return;
	if(ccpDistance(this->getPosition(),this->getTarget()->getPosition())<this->getMinDistance()){
		this->finish();
		CCLog("finish follow when reached move");
	}
}


//
//LookAtMove.cpp
//
void LookAtMove::initAction(){
	CCActionInterval* action=CCLookAt::create(duration,this->getTarget());
	this->setAction(action);
}

//
//FollowMove.cpp 跟踪N时间
//
void FollowMove::initAction(){
	if(this->getTarget()==NULL) return;
	if(duration<=0) this->setDuration(CCRANDOM_0_1()+1);

	CCActionInterval* follow=CCFollowSprite::create(duration,this->getRealVelocity(),this->getTarget(),0);//2-7秒
	this->setAction(follow);
	CCLog("init follow action");
}
void FollowMove::stop(){
	if(action!=NULL&&!this->isDone()){
		this->setDuration(this->getDuration()-((CCActionInterval*)action)->getElapsed());
		BaseMove::stop();
	}
}
void FollowMove::update(float dt){
		CCLog("2222222222");
	if(ccpDistance(this->getPosition(),this->getTarget()->getPosition())<100){
		//距离到一定程度，结束跟踪
		BaseMove::stop();
		this->unschedule(schedule_selector(FollowMove::update));
		CCLog("5555555555");
	}
}
void FollowMove::start(CCPoint pos,float angle,float vel){
	BaseMove::start(pos,angle,vel);
	this->schedule(schedule_selector(FollowMove::update),0.1f);
	CCLog("333333333");
}


//
//StayMove.cpp
//
void StayMove::initAction(){
	this->setRealVelocity(0);
	this->setAction(CCMoveBy::create(this->getDuration(),CCPointZero));
}


//
//NormalByTimeMove.cpp
//
void NormalByTimeMove::initAction(){
	//CCAction* sequence=CCSequence::create(CCCallFunc::create(this,callfunc_selector(NormalByTimeMove::createActionByRandom)),NULL);
	//this->setAction(sequence);
	float possibility=0.5f;
	CCActionInterval* atomAction=NULL;
	float tag=CCRANDOM_0_1();
	float durationTime;
	if(tag<possibility){//normal
		durationTime=CCRANDOM_0_1()*10+2;
		this->setAtomDuration(durationTime);
		this->setRealVelocity(this->getBaseVelocity());
		atomAction=CCRandomBy::create(durationTime,realVelocity+CCRANDOM_MINUS1_1()*10);
		CCLog("randomBy");
	}else if(tag<0.1f+possibility){
		durationTime=CCRANDOM_0_1()+1;
		this->setAtomDuration(durationTime);
		this->setRealVelocity(0);
		atomAction=CCMoveBy::create(durationTime,CCPointZero);
		CCLog("stay");
	}else if(tag<0.2f+possibility){
		durationTime=CCRANDOM_0_1();
		this->setAtomDuration(durationTime);
		this->setRealVelocity(this->getBaseVelocity());
		atomAction=CCRotateBy::create(durationTime,CCRANDOM_MINUS1_1()*90);//左右转90度范围内的随机角度
		CCLog("rotate");
	}else {
		durationTime=CCRANDOM_0_1();
		this->setAtomDuration(durationTime);
		this->setRealVelocity(this->getBaseVelocity());
		atomAction=CCStepBack::create(durationTime,this->getRealVelocity());
		CCLog("stepback");
	}
	this->setAction(atomAction);
	CCLog("init action NormalByTime");
}
void NormalByTimeMove::update(float dt){
	duration-=dt;
	atomDuration-=dt;
	if(atomDuration<=0&&duration>0){
		this->initAction();
		//this->unschedule(schedule_selector(NormalByTimeMove::update));
		BaseMove::start(this->getPosition(),this->getRotation(),this->getRealVelocity());
	}else if(duration<=0){
		this->finish();
	}

}
/*void NormalByTimeMove::start(CCPoint pos,float angle,float vel){
	BaseMove::start(pos,angle,vel);
	this->schedule(schedule_selector(NormalByTimeMove::update),0.1f);
}*/
void NormalByTimeMove::endAtomMove(){
	CCLog("end atom move");
	CCActionInterval* action=(CCActionInterval*)this->getActionByTag(111);
	if(action!=NULL)CCLog("duration:%f",action->getDuration());
	CCLog("time:%f",(MyMath::millisecondNow()-time)/1000.f);
}
void NormalByTimeMove::createActionByRandom(){
	float possibility=0.5f;
	CCActionInterval* atomAction=NULL;
	float tag=CCRANDOM_0_1();
	float durationTime;
	if(tag<possibility){//normal
		durationTime=CCRANDOM_0_1()*10+2;
		this->setRealVelocity(this->getBaseVelocity());
		atomAction=CCRandomBy::create(durationTime,realVelocity+CCRANDOM_MINUS1_1()*10);
		CCLog("randomBy");
	}else if(tag<0.1f+possibility){
		durationTime=CCRANDOM_0_1()+1;
		this->setRealVelocity(0);
		atomAction=CCMoveBy::create(durationTime,CCPointZero);
		CCLog("stay");
	}else if(tag<0.2f+possibility){
		durationTime=CCRANDOM_0_1();
		this->setRealVelocity(this->getBaseVelocity());
		atomAction=CCRotateBy::create(durationTime,CCRANDOM_MINUS1_1()*90);//左右转90度范围内的随机角度
		CCLog("rotate");
	}else {
		durationTime=CCRANDOM_0_1();
		this->setRealVelocity(this->getBaseVelocity());
		atomAction=CCStepBack::create(durationTime,this->getRealVelocity());
		CCLog("stepback");
	}
	atomAction->setTag(111);
	CCCallFunc* callFunc=CCCallFunc::create(this,callfunc_selector(NormalByTimeMove::createActionByRandom));
	//CCCallFunc* callFunc2=CCCallFunc::create(this,callfunc_selector(NormalByTimeMove::endAtomMove));
	CCAction* action=CCSequence::create(atomAction,callFunc,NULL);
	action->setTag(123);	
	this->runAction(action);
//	time=MyMath::millisecondNow();
}
void NormalByTimeMove::finish(){
	this->stopAllActions();
	BaseByTimeMove::finish();
}
void NormalByTimeMove::stop(){
	this->stopAllActions();
	BaseByTimeMove::stop();
}

