#include "GameInsectModel.h"
#include "Constant.h"
#include "B2EasyBox2D.h"
#include "RandomBy.h"
#include "GlobalDef.h"
#include "GamePlayController.h"
#include "MyMath.h"
#include "Message.h"

USING_NS_CC;

//�ɵ�ǰ��Ϊ���ͷ�����ȼ�
unsigned int getLevel(unsigned int actionType){
	unsigned int level=0;
	switch(actionType){
	case ID_ACTION_NORMAL:
		level=NORMAL_LV;
		break;
	case ID_ACTION_GROPE:
		level=GROPE_LV;
		break;
	case ID_ACTION_COMMUNICATE:
	case ID_ACTION_COMMUNICATED:
		level=COMMUNICATE_LV;
		break;
	case ID_ACTION_FOLLOW:
		level=FOLLOW_LV;
		break;
	case ID_ACTION_ATTACK:
		level=ATTACK_LV;
		break;
	case ID_ACTION_STAY:
		level=STAY_LV;
		break;
	case ID_ACTION_ESCAPE:
		level=ESCAPE_LV;
		break;
	}
	return level;
}


static int IDCOUNT=0;
//Animal.cpp
Animal::Animal():_velocity(100),_observerDelegate(NULL),_body(NULL),blood(20)
	,isAlive(true),moveId(ID_NORMAL_MOVE),_bounding(false),_baseVelocity(_velocity)
{
}

Animal::~Animal()
{
	CCLog("~Animal");
}
void Animal::clean(){
	this->finishCurAction();
	this->stopAllActions();
	this->getObserver()->finish();
	CCDictElement* pElement = NULL;
	CCDICT_FOREACH(moves, pElement)
{
    int key = pElement->getIntKey();
    BaseMove* move = (BaseMove*)pElement->getObject();
	this->removeChild(move,true);
}
	moves->removeAllObjects();
}
void Animal::deleteSelf(){
	//����Щ��Ϊ����ҪĿ��
	if(this->getActionType()==ID_ACTION_ATTACK||
		this->getActionType()==ID_ACTION_COMMUNICATE||
		this->getActionType()==ID_ACTION_COMMUNICATED||
		this->getActionType()==ID_ACTION_FOLLOW||
		this->getActionType()==ID_ACTION_GROPE){
			
	}
}
void Animal::distributeID(){
	if(IDCOUNT>1000) IDCOUNT=0;
	int id=IDCOUNT++;
	this->setID(id);
}
void Animal::initMovement(){
	moveId=ID_NORMAL_MOVE;
	moves=CCDictionary::create();
	moves->retain();
	BaseByTimeMove* move=NormalByTimeMove::create(this->getVelocity());
	this->addMove(move,ID_NORMAL_MOVE);

	move=FollowWhenReachedMove::create(this->getVelocity(),CCNode::create());
	this->addMove(move,ID_FOLLOW_REACHED_MOVE);

	move=StayMove::create(this->getVelocity());
	this->addMove(move,ID_STAY_MOVE);
}
void Animal::addMove(BaseByTimeMove* move,int id){
	if(move==NULL) return;
	this->addChild(move);
	moves->setObject(move,id);
}
BaseByTimeMove* Animal::getMoveForKey(int id){
	BaseByTimeMove* move=(BaseByTimeMove*)(moves->objectForKey((int)id));
	return move;
}
BaseByTimeMove* Animal::getCurMove(){
	return this->getMoveForKey(moveId);
}
bool Animal::outOfSize(const int id){
	if(moves->objectForKey((int)id)==NULL) return true;
	return false;
}
void Animal::replaceMovement(int id){//����ԭ�Ӷ��������ԭ�Ӷ���
	this->getCurMove()->finish();//������ǰԭ�Ӷ���
	this->startMove(id);
}
void Animal::finishMove(){
	if(!this->isMoveFinished()){
		this->setVelocity(this->getBaseVelocity());
		this->getCurMove()->finish();
	}
}
void Animal::startMove(int id){
	if(moves->count()<=0) return;//��û��ԭ�Ӷ���
	this->setMoveId(id);
	//if(this->getBounding()) return;
	CCLog("startmove:%d",moveId);
	this->getCurMove()->start(this->getPosition(),this->getRotation(),this->getVelocity());
}
void Animal::stopMove(){
	this->getCurMove()->stop();
}
bool Animal::isMoveFinished(){//��ǰԭ�Ӷ����Ƿ��ѽ���
	if(this->outOfSize(moveId)) return true;
	return this->getCurMove()->getFinished();
}
bool Animal::startAction(int actionId){//�ɹ���������true�����򷵻�false
	this->setActionType(actionId);
	switch(actionId){
	case ID_ACTION_NORMAL:
		this->startMove(ID_NORMAL_MOVE);
		return true;
	}
	return false;
}
bool Animal::isHigher(int id){
	return getLevel(id)>getLevel(this->getActionType());
}
bool Animal::replaceActionIfLevelUp(int actionId){
	//���ȼ��߹���ǰ�ȼ�
	if(this->isHigher(actionId)){
		this->finishCurAction();
		this->startAction(actionId);
		return true;
	}
	return false;
}
bool Animal::changeMoveWhenDone(){//ִ���ˣ�����true�����򷵻�false
	if(!this->isMoveFinished()) return true;
	
	switch(this->getActionType()){
	case ID_ACTION_NORMAL://����������Ϊ
		this->getCurMove()->setDuration(9999);
		this->startAction(ID_ACTION_NORMAL);
		return true;
	}
	return false;
}
void Animal::finishCurAction(){
	this->setActionType(ID_DEFAULT);
	this->finishMove();
}

bool Animal::init(b2World* world){
	bool pRet=false;
	do{ 		
		_world=world;
		this->setIdentify();
		this->setTag(TAG_INSECT);
		this->randomPositionAndAngle();
		this->setBodySize();

		b2Filter filter;
		filter.groupIndex=k_insectGroup;
		filter.categoryBits=k_insectCategory;
		filter.maskBits=k_insectMask;
		_body=B2EasyBox2D::createBox(world,this->getPositionX(),this->getPositionY(),this->getBodyWidth(),
			this->getBodyHeight(),this,&filter);
		_body->GetFixtureList()->SetSensor(true);
		//����
			CCNode* body=CCNode::create();
			body->setTag(ID_BODY);
			this->addChild(body);
			_body->GetFixtureList()->SetUserData(body);
		b2CircleShape shapeCircle;
		shapeCircle.m_radius=this->getBodyWidth()/2/PTM_RATIO;
			shapeCircle.m_p.Set(0,0.8f);
			
			//ͷ
			CCNode* head=CCNode::create();
			head->setTag(ID_HEAD_BODY);
			this->addChild(head);
			//3.����������״����b2FixtureDef������
			b2FixtureDef fixtureRequest;
			fixtureRequest.density=1;
			fixtureRequest.friction = 0.f;
			fixtureRequest.restitution = 1;
			fixtureRequest.shape = &shapeCircle;
			fixtureRequest.userData=head;
			fixtureRequest.filter=filter;
			_body->CreateFixture(&fixtureRequest);//ͷ
			//β��
			CCNode* tail=CCNode::create();
			tail->setTag(ID_TAIL_BODY);
			this->addChild(tail);
			shapeCircle.m_radius=this->getBodyWidth()/2/PTM_RATIO;
			shapeCircle.m_p.Set(0,-0.8f);
			fixtureRequest.shape=&shapeCircle;
			fixtureRequest.userData=tail;
			_body->CreateFixture(&fixtureRequest);
			
			//�����
			b2PolygonShape shape;
			shape.SetAsBox(this->getBodyWidth()/4/PTM_RATIO,this->getBodyHeight()/2/PTM_RATIO,b2Vec2(-0.25f,0),0);
			CCNode* left=CCNode::create();
			left->setTag(ID_LEFT_BODY);
			this->addChild(left);
			fixtureRequest.shape=&shape;
			fixtureRequest.userData=left;
			_body->CreateFixture(&fixtureRequest);
			//�Ұ���
			shape.SetAsBox(this->getBodyWidth()/4/PTM_RATIO,this->getBodyHeight()/2/PTM_RATIO,b2Vec2(0.25f,0),0);
			CCNode* right=CCNode::create();
			right->setTag(ID_RIGHT_BODY);
			this->addChild(right);
			fixtureRequest.shape=&shape;
			fixtureRequest.userData=right;
			_body->CreateFixture(&fixtureRequest);

			this->initMovement();
			this->setVelocity(this->getVelocity());
			this->setVelocityToBox2D();
			this->startAction(ID_ACTION_NORMAL);
			
		pRet=true;
	}while(0);

	return pRet;
}

void Animal::initObserver(GameModelDelegate* pDelegate){
	_observerDelegate=pDelegate;
}

void Animal::randomPositionAndAngle(){
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


void Animal::update(float dt){
	if(!this->getIsAlive()) return;
	
//	CCLog("actionType:%d",this->getActionType());
//	CCLog("move:%d",this->getMoveId());
	this->setPositionAngle();//λ�øı�
	this->changeMoveWhenDone();//��Ϊ�ı�

	if(!_observerDelegate) return;
	_observerDelegate->testModelDelegate();
}

void Animal::beginContact(){
	if(!this->getBounding()){
		this->stopMove();//��ͣ��ǰԭ�Ӷ���
		this->setBounding(true);
	}
}
void Animal::endContact(){
	if(this->getBounding()){
		this->setBox2dPosToCos();
		this->setBounding(false);
		this->startMove(moveId);//������ǰԭ�Ӷ���
	}
}
void Animal::preSolve(){
	this->setBox2dPosToCos();
}

void Animal::setPositionAngle(){
	//���ֽ׶����ڴ����ص����򷵻�
	if(this->getBounding()) return;
	BaseByTimeMove* curMove=this->getCurMove();
	
	CCPoint pos=curMove->getPosition();
	float angle=curMove->getRotation();
	this->setPosition(pos.x,pos.y);
	this->setRotation(angle);
	b2Vec2 b2Pos=b2Vec2(pos.x/PTM_RATIO,pos.y/PTM_RATIO);
	float32 b2Angle=CC_COCOSDEGREES_TO_BOX2DRADIANS(angle);
	_body->SetTransform(b2Pos,b2Angle);
	this->setVelocity(curMove->getRealVelocity());
	this->setVelocityToBox2D();
//	CCLog("%f,%f",pos.x,pos.y);
}
void Animal::setBox2dPosToCos(){
	CCPoint nowPosition=ccp(_body->GetWorldCenter().x*PTM_RATIO,_body->GetWorldCenter().y*PTM_RATIO);
	
	this->setPosition(nowPosition);
	this->setRotation(CC_BOX2DRADIANS_TO_COCOSDEGREES(_body->GetAngle()));
}

CCPoint Animal::getCenterPoint(){
//	CCPoint anchor=this->getAnchorPoint();
//	float distance=70*(0.5-anchor.y);
//	float angle=this->getRotation()*M_PI/180;
//	CCPoint center=ccp(getPositionX(),getPositionY()+distance*sin(angle));
	return this->getPosition();
}

float Animal::getVelocity(){
	return _velocity;
}
void Animal::setVelocity(float velocity){
	if(_velocity==velocity) return;
	_velocity=velocity;
//	MessageView::addMessage(this->_observerDelegate,VIEW_CHANGE_VEL,this);
	this->getObserver()->changePlayVel(this->getVelocity()/this->getBaseVelocity());
}
void Animal::setVelocityToBox2D(){
	if(_body!=NULL){
		float angle=CC_DEGREES_TO_RADIANS(this->getRotation());
		float velocity=this->getVelocity()/PTM_RATIO;
		//velocity*=3;
		b2Vec2 velInBox2D=b2Vec2(velocity*cos(angle),0-velocity*sin(angle));
		_body->SetLinearVelocity(velInBox2D);
	}
}

bool Animal::isOutOfBlood(){//�Ƿ�Ѫ�ľ�
	return this->getBlood()<=0;
}


const int COMMUNICATE_1_DURATION=5;//������һ�׶�
const int COMMUNICATE_3_DURATION=1;//���������׶�
const int KISS_DURATION=3;//����ʱ��
//BaseInsect.cpp
BaseInsect::BaseInsect(){
	this->setTarget(NULL);
	this->setGas(100);//����
	
	this->setNextActionID(ID_ACTION_NORMAL);
	this->setFollowed(false);
	this->setKissEnable(false);
}

BaseInsect::~BaseInsect()
{
	CCLog("~BaseInsect");
}
void BaseInsect::turnToCenter(float dt){
	if(CCRANDOM_0_1()>0.5f){
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		CCRect area=CCRect(winSize.width*0.3f,winSize.height*0.3f,winSize.width*0.5f,winSize.height*0.5f);
		float deltaX=CCRANDOM_0_1()*area.size.width;
		float deltaY=CCRANDOM_0_1()*area.size.height;
		CCPoint desPos=ccp(area.origin.x+deltaX,area.origin.y+deltaY);
		float angle=MyMath::getDirection(this->getPosition(),desPos);
		((RotateToMove*)this->getMoveForKey(ID_ROTATETO_MOVE))->setAngle(angle);
		this->replaceMovement(ID_ROTATETO_MOVE);
		CCLog("turn to center");
		CCLog("desPos:%f,%f",desPos.x,desPos.y);
		CCLog("locPos:%f,%f",this->getPositionX(),this->getPositionY());
		CCLog("angle:%f",angle);
	}
}
void BaseInsect::update(float dt){
	
	//�ж�Ŀ��target�Ƿ�����
	if((this->getActionType()==ID_ACTION_ATTACK||
		this->getActionType()==ID_ACTION_COMMUNICATE||
		this->getActionType()==ID_ACTION_COMMUNICATED||
		this->getActionType()==ID_ACTION_FOLLOW||
		this->getActionType()==ID_ACTION_GROPE)&&this->getTarget()==NULL){
			this->finishCurAction();
			this->startAction(ID_ACTION_NORMAL);
	}

		Animal::update(dt);
}

void BaseInsect::initMovement(){
	Animal::initMovement();

	BaseByTimeMove* move=FollowByTimeMove::create(this->getVelocity(),CCNode::create());
	this->addMove(move,ID_FOLLOW_BYTIME_MOVE);

	move=RotateToMove::create(this->getVelocity());
	this->addMove(move,ID_ROTATETO_MOVE);

	move=GropeMove::create(this->getVelocity());
	this->addMove(move,ID_GROPE_MOVE);
	move=KissMove::create(this->getVelocity());
	this->addMove(move,ID_KISS_MOVE);

	move=HeadRotateToMove::create(20);
	this->addMove(move,ID_HEAD_ROTATETO_MOVE);
}

bool BaseInsect::replaceAction(int id,BaseInsect* node){
	if(this->isHigher(id)){//����Ϊ�ȼ����ڵ�ǰ��Ϊ
		this->finishMove();
		this->setTarget(node);
		this->startAction(id);
		return true;
	}
	return false;
}
bool BaseInsect::startAction(int actionId){
	if(Animal::startAction(actionId)) return true;
	this->setActionType(actionId);
	switch(actionId){
	case ID_ACTION_ESCAPE://����
		this->setVelocity(this->getBaseVelocity()*2);//����
		this->getMoveForKey(ID_NORMAL_MOVE)->setDuration(5);//��ʱ������5����
		this->startMove(ID_NORMAL_MOVE);//������
		return true;
		break;
	case ID_ACTION_GROPE://����
		this->setVelocity(0);
		this->getMoveForKey(ID_STAY_MOVE)->setDuration(1);//����һ��
		this->startMove(ID_STAY_MOVE);
		return true;
	case ID_ACTION_COMMUNICATE://��������
		this->setVelocity(0);
		this->getMoveForKey(ID_STAY_MOVE)->setDuration(COMMUNICATE_1_DURATION);
		this->startMove(ID_STAY_MOVE);//ͣ��N��
		return true;
	case ID_ACTION_COMMUNICATED://������
		HeadRotateToMove* move=((HeadRotateToMove*)this->getMoveForKey(ID_HEAD_ROTATETO_MOVE));
		//float angle=MyMath::getDirection(this->getPosition(),this->getTarget()->getPosition());
		float angle;
		if(this->getRushZone()==ID_LEFT_BODY){//ײ��������壬��ʱ����ת
			angle=this->getTarget()->getRotation()+(-180+CCRANDOM_0_1()*10);
		}else {
			angle=this->getTarget()->getRotation()+(180-CCRANDOM_0_1()*10);
		}
		CCLog("angle:%f",angle);
		move->setAngle(angle);
		move->setDuration(COMMUNICATE_1_DURATION);//��ת��ĳ���Ƕ�,��2������
		this->startMove(ID_HEAD_ROTATETO_MOVE);//��ʼִ����ת�Ƕȵ�ԭ�Ӷ���
		return true;
	}
	return false;
}

bool BaseInsect::changeMoveWhenDone(){//ĳһԭ�Ӷ���������������һԭ�Ӷ���
	if(Animal::changeMoveWhenDone()) return true;
	
	switch(this->getActionType()){
	case ID_ACTION_ESCAPE://������Ϊ
		{
			switch(moveId){
			case ID_NORMAL_MOVE:
				this->setVelocity(this->getBaseVelocity());//����
				this->startAction(ID_ACTION_NORMAL);
				break;
			}
			return true;
		}
	case ID_ACTION_GROPE://������Ϊ
		this->setVelocity(this->getBaseVelocity());
		MessageVector::addMessage(this,TYPE_GROPE,this->getTarget());//��������������Ϣ
		this->startAction(ID_ACTION_NORMAL);
		return true;
	case ID_ACTION_COMMUNICATE://��������
		{	
			switch(this->getMoveId()){
			case ID_STAY_MOVE://ͣ��
				{
					this->setVelocity(this->getBaseVelocity());
					FollowWhenReachedMove* move=(FollowWhenReachedMove*)this->getMoveForKey(ID_FOLLOW_REACHED_MOVE);
					move->setTarget(this->getTarget());
					move->setMinDistance(this->getBodyHeight()/2+this->getTarget()->getBodyHeight()/2);
					this->startMove(ID_FOLLOW_REACHED_MOVE);
					break;
				}
			case ID_FOLLOW_REACHED_MOVE:
				this->setVelocity(0);
				((GropeMove*)this->getMoveForKey(ID_GROPE_MOVE))->setDuration(COMMUNICATE_3_DURATION);
				this->startMove(ID_GROPE_MOVE);
				break;
			case ID_GROPE_MOVE:
				if(this->getKissEnable()){
					this->setVelocity(0);
					((KissMove*)this->getMoveForKey(ID_KISS_MOVE))->setDuration(KISS_DURATION);
					this->startMove(ID_KISS_MOVE);
					this->getObserver()->playKiss(this->getPosition(),this->getTarget()->getPosition());
				}else{
					this->setVelocity(this->getBaseVelocity());
					this->startAction(ID_ACTION_NORMAL);
				}
				break;
			case ID_KISS_MOVE:
				this->setVelocity(this->getBaseVelocity());
				this->setKissEnable(false);
				this->startAction(ID_ACTION_NORMAL);
				break;
			}

			return true;
		}
	case ID_ACTION_COMMUNICATED://������
		{
			switch(moveId){
			case ID_HEAD_ROTATETO_MOVE:
				{
				this->setVelocity(this->getBaseVelocity());
				FollowWhenReachedMove* move=(FollowWhenReachedMove*)this->getMoveForKey(ID_FOLLOW_REACHED_MOVE);
				move->setTarget(this->getTarget());
				move->setMinDistance(this->getBodyHeight()/2+this->getTarget()->getBodyHeight()/2);
				this->startMove(ID_FOLLOW_REACHED_MOVE);
				break;
				}
			case ID_FOLLOW_REACHED_MOVE:
				this->getMoveForKey(ID_GROPE_MOVE)->setDuration(COMMUNICATE_3_DURATION);
				this->startMove(ID_GROPE_MOVE);//ͣ��1��
				break;
			case ID_GROPE_MOVE:
				if(this->getKissEnable()){
					this->getMoveForKey(ID_KISS_MOVE)->setDuration(KISS_DURATION);
					this->startMove(ID_KISS_MOVE);
					this->getObserver()->playKiss(this->getPosition(),this->getTarget()->getPosition());
				}else{
					this->startAction(ID_ACTION_NORMAL);
				}
				break;
			case ID_KISS_MOVE:
				this->setKissEnable(false);
				this->startAction(ID_ACTION_NORMAL);
				break;
			}
			return true;
		}
	}
	return false;
}

void BaseInsect::doAfterGrope(){
	//������Ϊ������һ�����ʴ���������Ϊ 
}

bool BaseInsect::init(b2World* world){
	bool pRet=false;
	do{ 	
		CC_BREAK_IF(!Animal::init(world));
	/*	_world=world;
		this->setIdentify();
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
			
			//3.����������״����b2FixtureDef������
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
			this->startAction(ID_ACTION_NORMAL);*/
			
		pRet=true;
	}while(0);

	return pRet;
}

void BaseInsect::covered(float attacked){
	this->setGas(this->getGas()-attacked);
}
bool BaseInsect::isOutOfGas(){
	return this->getGas()<=0;
}


//
//MantisInsect.cpp
//

const int ATTACK_TIME=1;//����ʱ��
MantisInsect::MantisInsect(){
	this->setAttack(50);
	this->setGas(60);
}
void MantisInsect::setBodySize(){
	CCSprite* mantis=CCSprite::createWithSpriteFrameName("mantis_0.png");
	this->setBodyWidth(mantis->getContentSize().width*0.5f);
	this->setBodyHeight(mantis->getContentSize().height*0.7f);
}
bool MantisInsect::init(b2World* world){
	bool pRet=false;
	do{
		CC_BREAK_IF(!BaseInsect::init(world));
		
		b2Filter filter;
		filter.groupIndex=k_visualField;
		filter.categoryBits=k_visualFieldCategory;
		filter.maskBits=k_visualFieldMask;
		
		b2PolygonShape shape;
		shape.SetAsBox(3,3.8f,b2Vec2(0,4.8f),0);

		CCNode* node=CCNode::create();
		this->addChild(node,0,ID_MANTIS_VIEW);
			
			//3.����������״����b2FixtureDef������
			b2FixtureDef fixtureRequest;
			fixtureRequest.isSensor=true;
			fixtureRequest.shape = &shape;
			fixtureRequest.filter=filter;
			fixtureRequest.userData=node;
		this->_body->CreateFixture(&fixtureRequest);
		//this->schedule(schedule_selector(BaseInsect::turnToCenter),5);
		CCLog("init mamtis");
		pRet=true;
	}while(0);
	return pRet;
}

void MantisInsect::initMovement(){
	BaseInsect::initMovement();

	BaseByTimeMove* move=LookAtMove::create(this->getVelocity(),NULL);
	((LookAtMove*)move)->setDuration(100);
	this->addMove(move,ID_LOOKAT_MOVE);
	
	move=AttackMove::create(this->getVelocity());
	this->addMove(move,ID_ATTACK_MOVE);
	
}
bool MantisInsect::startAction(int actionId){
	if(BaseInsect::startAction(actionId)) return true;
	this->setActionType(actionId);
	switch(actionId){
	case ID_ACTION_ATTACK://������Ϊ
		{
		LookAtMove* move=((LookAtMove*)this->getMoveForKey(ID_LOOKAT_MOVE));
		move->setTarget(this->getTarget());
		move->setDuration(CCRANDOM_0_1()*2+1);//2-3��
		this->startMove(ID_LOOKAT_MOVE);
		return true;
		break;
		}
	}
	return false;
}
bool MantisInsect::changeMoveWhenDone(){
	if(BaseInsect::changeMoveWhenDone()) return true;
	
	switch(this->getActionType()){
	case ID_ACTION_ATTACK://������Ϊ
		{
			switch(moveId){
			case ID_LOOKAT_MOVE:
				{
				this->setVelocity(this->getBaseVelocity()*2);//����
				FollowWhenReachedMove* move=(FollowWhenReachedMove*)this->getMoveForKey(ID_FOLLOW_REACHED_MOVE);
				move->setTarget(this->getTarget());
				move->setMinDistance(this->getBodyHeight()/2+this->getTarget()->getBodyHeight()/2);
				this->startMove(ID_FOLLOW_REACHED_MOVE);//����ֱ������Ϊֹ
				break;
				}
			case ID_FOLLOW_REACHED_MOVE:
				{
					this->setVelocity(0);
					AttackMove* move=(AttackMove*)this->getMoveForKey(ID_ATTACK_MOVE);
					move->setDuration(ATTACK_TIME);//����N��
					this->startMove(ID_ATTACK_MOVE);//
					this->getObserver()->playAttack(this->getPosition(),this->getTarget()->getPosition());
					break;
				}
			case ID_ATTACK_MOVE:
				{
					this->getTarget()->replaceAction(ID_ACTION_ESCAPE,this);
					MessageVector::addMessage(this->getTarget(),TYPE_ATTACKED,this);//������������Ϣ
					///MessageView::addMessage(this->getObserver(),VIEW_ATTACK,this);
					this->setVelocity(this->getBaseVelocity());//����
					this->startAction(ID_ACTION_NORMAL);
					break;
				}
			}
			return true;
			break;
		}
	}
	return false;
}

//
//AntInsect.cpp
//
AntInsect::AntInsect(){
	this->setGas(50);
	this->setBlood(50+CCRANDOM_MINUS1_1()*10);//40-60
}
void AntInsect::setBodySize(){
	CCSprite* ant=CCSprite::createWithSpriteFrameName("ant_0.png");
	this->setBodyWidth(ant->getContentSize().width*0.5f);
	this->setBodyHeight(ant->getContentSize().height*0.7f);
}
bool AntInsect::startAction(int actionId){
	if(BaseInsect::startAction(actionId)) return true;
	this->setActionType(actionId);
	switch(actionId){
	case ID_ACTION_FOLLOW:// ����
		{
			this->getTarget()->setFollowed(true);//����Ŀ����Ϊ������
			FollowByTimeMove* move=((FollowByTimeMove*)this->getMoveForKey(ID_FOLLOW_BYTIME_MOVE));
			move->setTarget(this->getTarget());
			move->setMinDistance(this->getBodyHeight()/2+this->getTarget()->getBodyHeight()/2);
			move->setDuration(10);//���Ӹ��ٳ���10����
			this->startMove(ID_FOLLOW_BYTIME_MOVE);
			return true;
			break;
		}
	}
	return false;
}
bool AntInsect::changeMoveWhenDone(){
	if(BaseInsect::changeMoveWhenDone()) return true;
	
	switch(this->getActionType()){
	case ID_ACTION_FOLLOW://������Ϊ����
		this->getTarget()->setFollowed(false);
		this->startAction(ID_ACTION_NORMAL);
		return true;
		break;
	}
	return false;
}

void AntInsect::attacked(float attack){
	this->setBlood(this->getBlood()-attack);
}