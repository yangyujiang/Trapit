#include "Enemy.h"
#include "InsectAction.h"
#include "Constant.h"
#include "B2EasyBox2D.h"
#include "RandomBy.h"

USING_NS_CC;

const float PAUSETIME=3;

Enemy::Enemy():_velocity(150),_observerDelegate(NULL),_body(NULL),blood(20)
	,contactStage(0),moveState(0),isAlive(true),isEating(false),eatVelocity(30),countTime(0),isCounting(false)
{
}


Enemy::~Enemy()
{
}
void Enemy::clean(){
	_world->DestroyBody(_body);
	
}

bool Enemy::init(b2World* world){
	bool pRet=false;
	do{ 		
		_world=world;
		this->setTag(TAG_ENEMY);
		this->randomPositionAndAngle();
		

		b2Filter filter;
		filter.groupIndex=k_enemyGroup;
		filter.categoryBits=k_enemyCategory;
		filter.maskBits=k_enemyMask;
		_body=B2EasyBox2D::createBox(world,this->getPositionX(),this->getPositionY(),50,70,this,&filter);
		//this->initVisualField();

		this->setAnchorPoint(ccp(0.5,0.1));//设置虫子的锚点为尾部
		
		//runTuoYuan();
	//moveRandom();
	//	runStepBack(5);//退后动作
	//this->schedule(schedule_selector(Enemy::update));

		pRet=true;
	}while(0);

	return pRet;
}

void Enemy::initVisualField(){
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

void Enemy::initObserver(GameModelDelegate* pDelegate){
	_observerDelegate=pDelegate;
}

void Enemy::randomPositionAndAngle(){
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

void Enemy::moveRandom(){
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
		CCCallFunc *funCall=CCCallFunc::create(this,callfunc_selector(Enemy::moveRandom));//回调此函数
		CCActionInterval* randomAction=CCRandomBy::create(10,_velocity);
		action=CCSequence::create(randomAction,funCall,NULL);
		action->setTag(TAG_INSECT_ACTION);
		this->runAction(action);
	}
}
void Enemy::moveStepBack(float duration){
	CCActionInterval* stepBack=CCStepBack::create(duration,_velocity);
	CCCallFunc *funCall=CCCallFunc::create(this,callfunc_selector(Enemy::moveRandom));//回调此函数
	CCFiniteTimeAction *action=CCSequence::create(stepBack,funCall,NULL);
	action->setTag(TAG_INSECT_ACTION);
	this->runAction(action);
}
void Enemy::moveRotateBy(){

	float delta=CCRANDOM_MINUS1_1()*90;//随机化偏转角度
	float duration=fabs(delta)/_velocity;//根据速度计算偏移时间
	CCActionInterval* rotateBy=CCRotateBy::create(duration,delta);

	CCCallFunc *funCall=CCCallFunc::create(this,callfunc_selector(Enemy::moveRandom));//回调此函数
	CCFiniteTimeAction* action=CCSequence::create(rotateBy,funCall,NULL);
	
	action->setTag(TAG_INSECT_ACTION);
	this->runAction(action);
}

void Enemy::handleContact(){
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
void Enemy::eatResin(GameResinBallModel* resin,float dt){
	if(isEating&&!isCounting){
		resin->blood-=100;
		this->setEating(false);
		isCounting=true;
		resin->shrinkResinBallBody(0.8);
	}
}
void Enemy::setEating(bool eating){
	isEating=eating;
}
float getDirection(CCPoint p1,CCPoint p2){
	float angle=asin((p2.x-p1.x)/(ccpDistance(p1,p2)))*180.f/M_PI;
	if(p1.x<=p2.x&&p1.y>=p2.y) angle=180-angle;
	else if(p1.x>=p2.x&&p1.y>=p2.y) angle=-180-angle;

	return angle;
}
void Enemy::followResin(CCPoint position){
	if(isCounting) return;
	CCPoint locPos=this->getPosition();
	if(ccpDistance(locPos,position)<40) return;
	float degree=getDirection(locPos,position);
	this->setRotation(degree);
	CCAction* ac=this->getActionByTag(1111);
	if(ac==NULL||ac->isDone()){
		CCLog("enemy");
		float angle=degree*M_PI/180.0f;
		CCPoint delta=ccp(_velocity*sin(angle)*0.5,_velocity*cos(angle)*0.5);
		CCMoveBy* move=CCMoveBy::create(0.5,delta);
		CCFiniteTimeAction* action=CCSequence::create(move,NULL);
		action->setTag(1111);
		this->runAction(action);
	}
}

void Enemy::update(float dt){
	if(isCounting){
			countTime+=dt;
			if(countTime>=PAUSETIME){
				countTime=0;
				isCounting=false;
			}
		}

	if(!isAlive) return;
	_body->SetAwake(true);
	_body->SetAngularVelocity(_velocity/PTM_RATIO);
	//this->setPositionAngleToBox2D(getPositionX(),getPositionY(),getRotation());
	CCPoint nowPoint=this->getCenterPoint();
	this->setPositionAngleToBox2D(nowPoint.x,nowPoint.y,getRotation());
	
//	this->updateInsectState();

	if(!_observerDelegate) return;
	_observerDelegate->testModelDelegate();

	
		
}
void Enemy::draw(){
	//CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	/*if(contactStage==0)
		ccDrawSolidRect(ccp(0,0),ccp(50,50),ccc4f(0,255,0,255));//绿色
	else if(contactStage==1)		
		ccDrawSolidRect(ccp(0,0),ccp(180,180),ccc4f(255,0,0,255));//红色
	else if(contactStage==2)
		ccDrawSolidRect(ccp(0,0),ccp(100,100),ccc4f(0,0,255,255));//蓝色*/
}

void Enemy::beginContact(){
	if(contactStage%2==0){
		contactStage=1;
		moveState=1;//虫子运动状态变为1，表示碰撞
	}
}
void Enemy::endContact(){
	
	if(contactStage==1){
		contactStage=2;//碰撞结束
	}
	
}

void Enemy::runTuoYuan(){
	lrTuoyuanConfig config;
	config.aLength=100;
	config.cLength=70;
	config.centerPosition=ccp(800,200);
	CCActionInterval* tuoyuan=LRTuoyuanBy::create(10,config);
	this->runAction(tuoyuan);
}


bool Enemy::attacked(float attack){
	//CCLog("blood:%f",blood);
	if(!isAlive) return true;
	blood-=attack;
	if(blood<=0){
		updateInsectState();
		return true;
	}
	else return false;
}

void Enemy::updateInsectState(){
	if(isAlive){
		if(blood<=0){
			isAlive=false;//虫子死去
			this->stopAllActions();//停掉所有动作
			
			CCLog("insect dead now!");
			_observerDelegate->testModelDelegate();
		}
	}
}


void Enemy::setPositionAngleToBox2D(float x,float y,float angle){
	b2Vec2 b2Position = b2Vec2(x/PTM_RATIO,y/PTM_RATIO);
	float32 b2Angle =-1* CC_DEGREES_TO_RADIANS(angle);
	_body->SetTransform(b2Position, b2Angle);
}
void Enemy::setBox2dPosToCos(){
	CCPoint nowPosition=ccp(_body->GetWorldCenter().x*PTM_RATIO,_body->GetWorldCenter().y*PTM_RATIO);
	float distance=70*(0.5-this->getAnchorPoint().y);
	float angle=this->getRotation()*M_PI/180;
	CCPoint realPosition=ccp(nowPosition.x-distance*cos(angle),nowPosition.y-distance*sin(angle));
	this->setPosition(realPosition);
}

CCPoint Enemy::getCenterPoint(){
	CCPoint anchor=this->getAnchorPoint();
	float distance=70*(0.5-anchor.y);
	float angle=this->getRotation()*M_PI/180;
	CCPoint center=ccp(getPositionX()+distance*cos(angle),getPositionY()+distance*sin(angle));
	return center;
}

float Enemy::getVelocity(){
	return _velocity;
}
void Enemy::setVelocity(float velocity){
	_velocity=velocity;
}
bool Enemy::getAlive(){
	return isAlive;
}