#include "GameInsectModel.h"
#include "InsectAction.h"
#include "Constant.h"
#include "B2EasyBox2D.h"
#include "RandomBy.h"

USING_NS_CC;

GameInsectModel::GameInsectModel():_velocity(100),_observerDelegate(NULL),_body(NULL),blood(20)
	,contactStage(0),moveState(0),isAlive(true)
{
}


GameInsectModel::~GameInsectModel()
{
	CCLog("~InsectModel");
}
void GameInsectModel::clean(){
	_world->DestroyBody(_body);
	
}

bool GameInsectModel::init(b2World* world){
	bool pRet=false;
	do{ 		
		_world=world;
		this->setTag(TAG_INSECT);
		this->randomPositionAndAngle();
		

		b2Filter filter;
		filter.groupIndex=k_insectGroup;
		filter.categoryBits=k_insectCategory;
		filter.maskBits=k_insectMask;
		_body=B2EasyBox2D::createBox(world,this->getPositionX(),this->getPositionY(),50,70,this,&filter);
		//this->initVisualField();

		this->setAnchorPoint(ccp(0.5,0.1));//设置虫子的锚点为尾部
		
		//runTuoYuan();
	moveRandom();
	//	runStepBack(5);//退后动作
	//this->schedule(schedule_selector(GameInsectModel::update));

		pRet=true;
	}while(0);

	return pRet;
}

void GameInsectModel::initVisualField(){
	b2PolygonShape box;
	box.SetAsBox(1,1,b2Vec2(0.,1),0);

	b2Filter visualFieldFilter;
	visualFieldFilter.groupIndex=k_visualField;

	b2FixtureDef visualFieldFixtureDef;
	visualFieldFixtureDef.isSensor=true;
	visualFieldFixtureDef.shape=&box;
	visualFieldFixtureDef.filter=visualFieldFilter;

	_body->CreateFixture(&visualFieldFixtureDef);
}

void GameInsectModel::initObserver(GameModelDelegate* pDelegate){
	_observerDelegate=pDelegate;
}

void GameInsectModel::randomPositionAndAngle(){
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	float posX,posY;
	float angle;
	float random=CCRANDOM_0_1();
	if(random<0.25f){
		posX=100;
		posY=CCRANDOM_0_1()*winSize.height-100;
		angle=CCRANDOM_MINUS1_1()*80+5;
	}else if(random<0.5f){
		posX=winSize.width-100;
		posY=CCRANDOM_0_1()*winSize.height-100;
		angle=CCRANDOM_0_1()*160+100;
	}else if(random<0.75f){
		posY=0+100;
		posX=CCRANDOM_0_1()*winSize.width-100;
		angle=CCRANDOM_0_1()*-160-10;
	}else{
		posY=winSize.height-100;
		posX=CCRANDOM_0_1()*winSize.width-100;
		angle=CCRANDOM_0_1()*160+10;
	}
	//this->setPosition(900,300);
	this->setPosition(ccp(posX,posY));
	this->setRotation(angle);
	//CCLog("%f,%f,%f,%f",posX,posY,angle,random);
}

void GameInsectModel::moveRandom(){
	float rotateFirstP=0.1f;//先实行转圈的概率
	if(moveState==1){//如果当前运动状态为碰撞，则把运动状态转到正常运动，并且把先转圈的概率提升至0.9
		moveState=0;
		rotateFirstP=0.9f;
	}
	if(CCRANDOM_0_1()<rotateFirstP){
		this->moveRotateBy();
	}
	else{
		CCFiniteTimeAction* action;
		float p=0.1f;//下一次先开始旋转的概率
		CCCallFunc *funCall=CCCallFunc::create(this,callfunc_selector(GameInsectModel::moveRandom));//回调此函数
		CCActionInterval* randomAction=CCRandomBy::create(10,_velocity);
		action=CCSequence::create(randomAction,funCall,NULL);
		action->setTag(TAG_INSECT_ACTION);
		this->runAction(action);
	}
}
void GameInsectModel::moveStepBack(float duration){
	CCActionInterval* stepBack=CCStepBack::create(duration,_velocity);
	CCCallFunc *funCall=CCCallFunc::create(this,callfunc_selector(GameInsectModel::moveRandom));//回调此函数
	CCFiniteTimeAction *action=CCSequence::create(stepBack,funCall,NULL);
	action->setTag(TAG_INSECT_ACTION);
	this->runAction(action);
}
void GameInsectModel::moveRotateBy(){

	float delta=CCRANDOM_MINUS1_1()*90;//随机化偏转角度
	float duration=fabs(delta)/_velocity;//根据速度计算偏移时间
	CCActionInterval* rotateBy=CCRotateBy::create(duration,delta);

	CCCallFunc *funCall=CCCallFunc::create(this,callfunc_selector(GameInsectModel::moveRandom));//回调此函数
	CCFiniteTimeAction* action=CCSequence::create(rotateBy,funCall,NULL);
	
	action->setTag(TAG_INSECT_ACTION);
	this->runAction(action);
}

void GameInsectModel::handleContact(){
	if(contactStage==1){		
		this->stopActionByTag(TAG_INSECT_ACTION);
		this->setBox2dPosToCos();
	}else if(contactStage==2){//碰撞一结束，马上开启新的运动状态
		contactStage=0;//只执行一次
		this->setBox2dPosToCos();
		if(CCRANDOM_0_1()<0.6f){//0.7的概率先后退
			this->moveStepBack(CCRANDOM_0_1()+0.2);
		}else{
			float p=20;
			this->moveRandom();  //0.3的概率直接回到正常状态
		}
	}
}

void GameInsectModel::update(float dt){
	if(!isAlive) return;
	_body->SetAwake(true);
	_body->SetAngularVelocity(_velocity/PTM_RATIO);
	//this->setPositionAngleToBox2D(getPositionX(),getPositionY(),getRotation());
	CCPoint nowPoint=this->getCenterPoint();
	this->setPositionAngleToBox2D(nowPoint.x,nowPoint.y,getRotation());
	
	this->updateInsectState();

	if(!_observerDelegate) return;
	_observerDelegate->testModelDelegate();
}
void GameInsectModel::draw(){
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	/*if(contactStage==0)
		ccDrawSolidRect(ccp(0,0),ccp(50,50),ccc4f(0,255,0,255));//绿色
	else if(contactStage==1)		
		ccDrawSolidRect(ccp(0,0),ccp(180,180),ccc4f(255,0,0,255));//红色
	else if(contactStage==2)
		ccDrawSolidRect(ccp(0,0),ccp(100,100),ccc4f(0,0,255,255));//蓝色*/
}

void GameInsectModel::beginContact(){
	if(contactStage%2==0){
		contactStage=1;
		moveState=1;//虫子运动状态变为1，表示碰撞
	}
}
void GameInsectModel::endContact(){
	
	if(contactStage==1){
		contactStage=2;//碰撞结束
	}
	
}

void GameInsectModel::runTuoYuan(){
	lrTuoyuanConfig config;
	config.aLength=100;
	config.cLength=70;
	config.centerPosition=ccp(800,200);
	CCActionInterval* tuoyuan=LRTuoyuanBy::create(10,config);
	this->runAction(tuoyuan);
}


bool GameInsectModel::attacked(float attack){
	//CCLog("blood:%f",blood);
	if(!isAlive) return true;
	blood-=attack;
	if(blood<=0){
		updateInsectState();
		return true;
	}
	else return false;
}

void GameInsectModel::updateInsectState(){
	if(isAlive){
		if(blood<=0){
			isAlive=false;//虫子死去
			this->stopAllActions();//停掉所有动作
			
			CCLog("insect dead now!");
			_observerDelegate->testModelDelegate();
		}
	}
}


void GameInsectModel::setPositionAngleToBox2D(float x,float y,float angle){
	b2Vec2 b2Position = b2Vec2(x/PTM_RATIO,y/PTM_RATIO);
	float32 b2Angle =-1* CC_DEGREES_TO_RADIANS(angle+90);
	_body->SetTransform(b2Position, b2Angle);
}
void GameInsectModel::setBox2dPosToCos(){
	CCPoint nowPosition=ccp(_body->GetWorldCenter().x*PTM_RATIO,_body->GetWorldCenter().y*PTM_RATIO);
	float distance=70*(0.5-this->getAnchorPoint().y);
	float angle=this->getRotation()*M_PI/180;
	CCPoint realPosition=ccp(nowPosition.x-distance*cos(angle),nowPosition.y-distance*sin(angle));
	this->setPosition(realPosition);
}

CCPoint GameInsectModel::getCenterPoint(){
	CCPoint anchor=this->getAnchorPoint();
	float distance=70*(0.5-anchor.y);
	float angle=this->getRotation()*M_PI/180;
	CCPoint center=ccp(getPositionX()+distance*cos(angle),getPositionY()+distance*sin(angle));
	return center;
}

float GameInsectModel::getVelocity(){
	return _velocity;
}
void GameInsectModel::setVelocity(float velocity){
	_velocity=velocity;
}
bool GameInsectModel::getAlive(){
	return isAlive;
}