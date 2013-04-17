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

		this->setAnchorPoint(ccp(0.5,0.1));//���ó��ӵ�ê��Ϊβ��
		
		//runTuoYuan();
	moveRandom();
	//	runStepBack(5);//�˺���
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
	float rotateFirstP=0.1f;//��ʵ��תȦ�ĸ���
	if(moveState==1){//�����ǰ�˶�״̬Ϊ��ײ������˶�״̬ת�������˶������Ұ���תȦ�ĸ���������0.9
		moveState=0;
		rotateFirstP=0.9f;
	}
	if(CCRANDOM_0_1()<rotateFirstP){
		this->moveRotateBy();
	}
	else{
		CCFiniteTimeAction* action;
		float p=0.1f;//��һ���ȿ�ʼ��ת�ĸ���
		CCCallFunc *funCall=CCCallFunc::create(this,callfunc_selector(GameInsectModel::moveRandom));//�ص��˺���
		CCActionInterval* randomAction=CCRandomBy::create(10,_velocity);
		action=CCSequence::create(randomAction,funCall,NULL);
		action->setTag(TAG_INSECT_ACTION);
		this->runAction(action);
	}
}
void GameInsectModel::moveStepBack(float duration){
	CCActionInterval* stepBack=CCStepBack::create(duration,_velocity);
	CCCallFunc *funCall=CCCallFunc::create(this,callfunc_selector(GameInsectModel::moveRandom));//�ص��˺���
	CCFiniteTimeAction *action=CCSequence::create(stepBack,funCall,NULL);
	action->setTag(TAG_INSECT_ACTION);
	this->runAction(action);
}
void GameInsectModel::moveRotateBy(){

	float delta=CCRANDOM_MINUS1_1()*90;//�����ƫת�Ƕ�
	float duration=fabs(delta)/_velocity;//�����ٶȼ���ƫ��ʱ��
	CCActionInterval* rotateBy=CCRotateBy::create(duration,delta);

	CCCallFunc *funCall=CCCallFunc::create(this,callfunc_selector(GameInsectModel::moveRandom));//�ص��˺���
	CCFiniteTimeAction* action=CCSequence::create(rotateBy,funCall,NULL);
	
	action->setTag(TAG_INSECT_ACTION);
	this->runAction(action);
}

void GameInsectModel::handleContact(){
	if(contactStage==1){		
		this->stopActionByTag(TAG_INSECT_ACTION);
		this->setBox2dPosToCos();
	}else if(contactStage==2){//��ײһ���������Ͽ����µ��˶�״̬
		contactStage=0;//ִֻ��һ��
		this->setBox2dPosToCos();
		if(CCRANDOM_0_1()<0.6f){//0.7�ĸ����Ⱥ���
			this->moveStepBack(CCRANDOM_0_1()+0.2);
		}else{
			float p=20;
			this->moveRandom();  //0.3�ĸ���ֱ�ӻص�����״̬
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
		ccDrawSolidRect(ccp(0,0),ccp(50,50),ccc4f(0,255,0,255));//��ɫ
	else if(contactStage==1)		
		ccDrawSolidRect(ccp(0,0),ccp(180,180),ccc4f(255,0,0,255));//��ɫ
	else if(contactStage==2)
		ccDrawSolidRect(ccp(0,0),ccp(100,100),ccc4f(0,0,255,255));//��ɫ*/
}

void GameInsectModel::beginContact(){
	if(contactStage%2==0){
		contactStage=1;
		moveState=1;//�����˶�״̬��Ϊ1����ʾ��ײ
	}
}
void GameInsectModel::endContact(){
	
	if(contactStage==1){
		contactStage=2;//��ײ����
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
			isAlive=false;//������ȥ
			this->stopAllActions();//ͣ�����ж���
			
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