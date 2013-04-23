#include "GameInsectModel.h"
#include "Constant.h"
#include "B2EasyBox2D.h"
#include "RandomBy.h"
#include "GlobalDef.h"

USING_NS_CC;



BaseInsect::BaseInsect():_velocity(100),_observerDelegate(NULL),_body(NULL),blood(20)
	,isAlive(true),moveId(ID_NORMAL_MOVE),moveLevel(NORMAL_LV),pauseMoveId(ID_DEFAULT)
{
}

BaseInsect::~BaseInsect()
{
	CCLog("~InsectModel");
}
void BaseInsect::clean(){
	this->stopAllActions();
	_world->DestroyBody(_body);
}
void BaseInsect::initMovement(){
	moveId=ID_NORMAL_MOVE;
	BaseMove* move=NormalMove::create(this->getPosition(),this->getRotation(),this->getVelocity());
/*	BaseMove* move=FollowMove::create(this->getPosition(),this->getRotation(),this->getVelocity());
	CCSprite* node=CCSprite::create("ant_1.png");node->setPosition(ccp(500,100));this->addChild(node);
	node->runAction(CCRandomBy::create(1000,this->getVelocity()*0.1));
	((FollowMove*)move)->initTarget(node);*/
	this->addChild(move);
	moves.push_back(move);
	move=StepBackMove::create(this->getPosition(),this->getRotation(),this->getVelocity());
	this->addChild(move);
	moves.push_back(move);
	move=RotateMove::create(this->getPosition(),this->getRotation(),this->getVelocity());
	this->addChild(move);
	moves.push_back(move);
	CCLog("size:%d",moves.size());
}
bool BaseInsect::isHigherLevel(int id1,int id2){
	if(this->outOfSize(id1)||this->outOfSize(id2)) return false;
	if(moves[id1]->getLevel()>moves[id2]->getLevel()){
		return true;
	}
	return false;
}
bool BaseInsect::outOfSize(int id){
	return id<0||id>moves.size()-1;
}
void BaseInsect::replaceMovement(int id){
	//若当前move已经结束，并且有move压栈,则判断压栈的move和新move的优先级高低，以决定启用的move
	if(this->isMoveDone()&&pauseMoveId!=ID_DEFAULT){
		if(!this->isHigherLevel(id,pauseMoveId))
			moveId=pauseMoveId;
	}
	else if(this->isHigherLevel(id,moveId)){
		moveId=id;
	}
	this->startMove();
}
void BaseInsect::startMove(){
	moves[moveId]->restart(this->getPosition(),this->getRotation());
}
void BaseInsect::stopMove(){
	moves[moveId]->stop();
}
bool BaseInsect::isMoveDone(){
	if(this->outOfSize(moveId)) return false;
	return moves[moveId]->isDone();
}

bool BaseInsect::init(b2World* world){
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
		b2CircleShape shapeCircle;
			shapeCircle.m_radius=1;
			shapeCircle.m_p.Set(0,1);
			
			//3.创建刚体形状需求b2FixtureDef的子类
			b2FixtureDef fixtureRequest;
			fixtureRequest.density=1;
			fixtureRequest.friction = 0.f;
			fixtureRequest.restitution = 1;
			fixtureRequest.shape = &shapeCircle;
			fixtureRequest.filter=filter;
			_body->CreateFixture(&fixtureRequest);
			shapeCircle.m_radius=0.8f;
			shapeCircle.m_p.Set(0,-1);
			fixtureRequest.shape=&shapeCircle;
			_body->CreateFixture(&fixtureRequest);

			this->initMovement();
			this->setVelocity(this->getVelocity());
			this->setVelocityToBox2D();
			this->startMove();
			
		pRet=true;
	}while(0);

	return pRet;
}

void BaseInsect::initObserver(GameModelDelegate* pDelegate){
	_observerDelegate=pDelegate;
}

void BaseInsect::randomPositionAndAngle(){
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

void BaseInsect::handleContact(){
	
}

void BaseInsect::update(float dt){
	if(!isAlive) return;
	
	this->setPositionAngle();
	if(this->isMoveDone()){
		if(pauseMoveId==ID_DEFAULT){
		switch(moveId){
		case ID_NORMAL_MOVE:
			break;
		case ID_STEPBACK_MOVE:
			if(CCRANDOM_0_1()>0.5f)
				this->replaceMovement(ID_NORMAL_MOVE);
			else 
				this->replaceMovement(ID_ROTATE_MOVE);
			break;
		case ID_ROTATE_MOVE:
			this->replaceMovement(ID_NORMAL_MOVE);
			break;
		}
		}
		else{
			this->replaceMovement(pauseMoveId);
			pauseMoveId=ID_DEFAULT;
		}
	}

	if(!_observerDelegate) return;
	_observerDelegate->testModelDelegate();
}


void BaseInsect::beginContact(){
	this->stopMove();
	//this->setBox2dPosToCos();
}
void BaseInsect::endContact(){
	this->setBox2dPosToCos();
	if(CCRANDOM_0_1()>0.7f){
		this->replaceMovement(ID_STEPBACK_MOVE);
	}else{
		this->replaceMovement(ID_NORMAL_MOVE);
	}
}
void BaseInsect::preSolve(){
	this->setBox2dPosToCos();
}

void BaseInsect::setPositionAngle(){
	if(moves[moveId]->getIsStop()) return;
	
	CCPoint pos=moves[moveId]->getPosition();
	float angle=moves[moveId]->getRotation();
	this->setPosition(pos.x,pos.y);
	this->setRotation(angle);
	b2Vec2 b2Pos=b2Vec2(pos.x/PTM_RATIO,pos.y/PTM_RATIO);
	float32 b2Angle=CC_COCOSDEGREES_TO_BOX2DRADIANS(angle);
	_body->SetTransform(b2Pos,b2Angle);
	this->setVelocityToBox2D();
}
void BaseInsect::setPositionAngleToBox2D(float x,float y,float angle){
	b2Vec2 b2Position = b2Vec2(x/PTM_RATIO,y/PTM_RATIO);
	float32 b2Angle=CC_COCOSDEGREES_TO_BOX2DRADIANS(angle);
	_body->SetTransform(b2Position, b2Angle);
	this->setVelocity(this->getVelocity());
}
void BaseInsect::setBox2dPosToCos(){
	CCPoint nowPosition=ccp(_body->GetWorldCenter().x*PTM_RATIO,_body->GetWorldCenter().y*PTM_RATIO);
	float distance=70*(0.5-this->getAnchorPoint().y);
	float angle=this->getRotation()*M_PI/180;
	CCPoint realPosition=ccp(nowPosition.x-distance*cos(angle),nowPosition.y-distance*sin(angle));
	//this->setPosition(realPosition);
	this->setPosition(nowPosition);
	this->setRotation(CC_RADIANS_TO_DEGREES(-_body->GetAngle())-90);
}

CCPoint BaseInsect::getCenterPoint(){
	CCPoint anchor=this->getAnchorPoint();
	float distance=70*(0.5-anchor.y);
	float angle=this->getRotation()*M_PI/180;
	CCPoint center=ccp(getPositionX(),getPositionY()+distance*sin(angle));
	return this->getPosition();
}

float BaseInsect::getVelocity(){
	return _velocity;
}
void BaseInsect::setVelocity(float velocity){
	if(_velocity==velocity) return;
	_velocity=velocity;
	
}
void BaseInsect::setVelocityToBox2D(){
	//if(moveId>=0&&moveId<moves.size())
	//	moves[moveId]->setRealVelocity(this->getVelocity());
	if(_body!=NULL){
		float angle=this->getRotation()*M_PI/180;
		float velocity=this->getVelocity()/PTM_RATIO;
		//velocity*=3;
		b2Vec2 velInBox2D=b2Vec2(velocity*cos(angle),0-velocity*sin(angle));
		_body->SetLinearVelocity(velInBox2D);
	}
}
bool BaseInsect::getAlive(){
	return isAlive;
}


//
//MantisInsect.cpp
//
bool MantisInsect::init(b2World* world){
	bool pRet=false;
	do{
		CC_BREAK_IF(!BaseInsect::init(world));
		
		b2Filter filter;
		filter.groupIndex=k_visualField;
		filter.categoryBits=k_visualFieldCategory;
		filter.maskBits=k_visualFieldMask;
		
		b2PolygonShape shape;
		shape.SetAsBox(3,3.8,b2Vec2(0,4.8),0);

		CCNode* node=CCNode::create();
		this->addChild(node,0,ID_MANTIS_VIEW);
			
			//3.创建刚体形状需求b2FixtureDef的子类
			b2FixtureDef fixtureRequest;
			fixtureRequest.isSensor=true;
			fixtureRequest.shape = &shape;
			fixtureRequest.filter=filter;
			fixtureRequest.userData=node;
		this->_body->CreateFixture(&fixtureRequest);
		CCLog("init mamtis");
		pRet=true;
	}while(0);
	return pRet;
}

void MantisInsect::replaceMovement(int id,CCNode* node){
	if(id==ID_FOLLOW_MOVE){
		moveId=id;
		if(moves.size()<id){
			BaseMove* move=FollowMove::create(this->getPosition(),this->getRotation(),this->getVelocity());
			moves.push_back(move);
		}
		((FollowMove*)moves[moveId])->setTarget(node);
		moves[moveId]->restart(this->getPosition(),this->getRotation());
	}
}


//
//GameInsectModel.cpp
//

GameInsectModel::GameInsectModel():_velocity(100),_observerDelegate(NULL),_body(NULL),blood(20)
	,contactStage(0),moveState(0),isAlive(true),stop(false)
{
}


GameInsectModel::~GameInsectModel()
{
	CCLog("~InsectModel");
}
void GameInsectModel::clean(){
	this->stopAllActions();
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
		b2CircleShape shapeCircle;
			shapeCircle.m_radius=1;
			shapeCircle.m_p.Set(0,1);
			
			//3.创建刚体形状需求b2FixtureDef的子类
			b2FixtureDef fixtureRequest;
			fixtureRequest.density=1;
			fixtureRequest.friction = 0.f;
			fixtureRequest.restitution = 1;
			fixtureRequest.shape = &shapeCircle;
			fixtureRequest.filter=filter;
			_body->CreateFixture(&fixtureRequest);
			shapeCircle.m_radius=0.8f;
			shapeCircle.m_p.Set(0,-1);
			fixtureRequest.shape=&shapeCircle;
			_body->CreateFixture(&fixtureRequest);
	
			this->setVelocity(100);
		//this->initVisualField();

	//	this->setAnchorPoint(ccp(0.5,0.1));//设置虫子的锚点为尾部

		//runTuoYuan();
	//moveRandom();
		moveStepBack(5);//退后动作
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
	_body->SetAwake(true);
	this->setVelocity(this->getVelocity());
	//_body->SetAngularVelocity(_velocity*2/PTM_RATIO);
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
	//this->setPositionAngleToBox2D(getPositionX(),getPositionY(),getRotation());
	CCPoint nowPoint=this->getCenterPoint();
	CCLog("-----------");
	CCLog("box2d:(%f,%f)",_body->GetWorldCenter().x*PTM_RATIO,_body->GetWorldCenter().y*PTM_RATIO);
	CCLog("cocos:(%f,%f)",nowPoint.x,nowPoint.y);
	if(!stop)this->setPositionAngleToBox2D(nowPoint.x,nowPoint.y,getRotation());
	CCLog("stop:%d",stop);
	CCLog("boxVel:%f",_body->GetLinearVelocity().Length());
	if(this->_body->GetContactList()!=NULL){
		CCLog("isTouching:%d",this->_body->GetContactList()->contact->IsTouching());
		/*this->stepBackInstant(10);
		if(CCRANDOM_0_1()<0.99f){//0.7的概率先后退
			this->moveStepBack(2);//CCRANDOM_0_1()+0.2);
			CCLog("step Back");
		}else{
			float p=20;
			this->moveRandom();  //0.3的概率直接回到正常状态
		}*/
	}
	CCLog("-----------");
	this->updateInsectState();

	if(!_observerDelegate) return;
	_observerDelegate->testModelDelegate();
}
void GameInsectModel::draw(){
	/*CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	if(contactStage==0)
		ccDrawSolidRect(ccp(0,0),ccp(50,50),ccc4f(0,255,0,255));//绿色
	else if(contactStage==1)		
		ccDrawSolidRect(ccp(0,0),ccp(180,180),ccc4f(255,0,0,255));//红色
	else if(contactStage==2)
		ccDrawSolidRect(ccp(0,0),ccp(100,100),ccc4f(0,0,255,255));//蓝色*/
}

void GameInsectModel::beginContact(){
	/*if(contactStage%2==0){
		contactStage=1;
		moveState=1;//虫子运动状态变为1，表示碰撞
	}*/
	stop=true;
	CCLog("box2d:(%f,%f)",_body->GetWorldCenter().x*PTM_RATIO,_body->GetWorldCenter().y*PTM_RATIO);
	CCLog("cocos:(%f,%f)",this->getPositionX(),this->getPositionY());
		CCLog("isTouching:%d",this->_body->GetContactList()->contact->IsTouching());
	CCLog("boxVel:%f",_body->GetLinearVelocity().Length());
	
	this->stopActionByTag(TAG_INSECT_ACTION);
	this->setBox2dPosToCos();
}
void GameInsectModel::endContact(){
	stop=false;
	//_body->SetLinearVelocity(b2Vec2(0,0));
	this->setBox2dPosToCos();
	CCLog("isTouching:%d",this->_body->GetContactList()->contact->IsTouching());
	CCLog("box2d:(%f,%f)",_body->GetWorldCenter().x*PTM_RATIO,_body->GetWorldCenter().y*PTM_RATIO);
	CCLog("cocos:(%f,%f)",this->getPositionX(),this->getPositionY());
		if(CCRANDOM_0_1()<0.2f){//0.7的概率先后退
			this->moveStepBack(CCRANDOM_0_1()+2);
			CCLog("step Back");
		}else{
			float p=20;
			this->moveRandom();  //0.3的概率直接回到正常状态
		}
		
		CCLog("%f",_body->GetLinearVelocity().Length());
/*	if(contactStage==1){
		contactStage=2;//碰撞结束
	}
	*/
}

void GameInsectModel::stepBackInstant(float delta){
	float angle=this->getRotation()*M_PI/180;
	this->setPosition(ccpSub(this->getPosition(),ccpMult(ccp(cos(angle),-sin(angle)),delta)));
	this->setPositionAngleToBox2D(this->getPositionX(),this->getPositionY(),this->getRotation());
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
	this->setVelocity(this->getVelocity());
}
void GameInsectModel::setBox2dPosToCos(){
	CCPoint nowPosition=ccp(_body->GetWorldCenter().x*PTM_RATIO,_body->GetWorldCenter().y*PTM_RATIO);
	float distance=70*(0.5-this->getAnchorPoint().y);
	float angle=this->getRotation()*M_PI/180;
	CCPoint realPosition=ccp(nowPosition.x-distance*cos(angle),nowPosition.y-distance*sin(angle));
	//this->setPosition(realPosition);
	this->setPosition(nowPosition);
	this->setRotation(CC_RADIANS_TO_DEGREES(-_body->GetAngle())-90);
}

CCPoint GameInsectModel::getCenterPoint(){
	CCPoint anchor=this->getAnchorPoint();
	float distance=70*(0.5-anchor.y);
	float angle=this->getRotation()*M_PI/180;
	CCPoint center=ccp(getPositionX(),getPositionY()+distance*sin(angle));
	return this->getPosition();
}

float GameInsectModel::getVelocity(){
	return _velocity;
}
void GameInsectModel::setVelocity(float velocity){
	_velocity=velocity;
	if(_body!=NULL){
		float angle=this->getRotation()*M_PI/180;
		velocity/=PTM_RATIO;
		//velocity*=3;
		b2Vec2 velInBox2D=b2Vec2(velocity*cos(angle),0-velocity*sin(angle));
		_body->SetLinearVelocity(velInBox2D);
	}
}
bool GameInsectModel::getAlive(){
	return isAlive;
}