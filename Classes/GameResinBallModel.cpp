#include "GameResinBallModel.h"
#include "Constant.h"
#include "B2EasyBox2D.h"
#include "DrawUtil.h"

USING_NS_CC;

const float32 LIMIT_ANGLE=CC_DEGREES_TO_RADIANS(30);//������ת�ؽڵ���ת�Ƕ�
const float32 BALL_FRICTION=0.1f;//ʵ����������Ħ��ϵ��
const float32 BALL_GRAVITY=150;//ʵ����������N��,�̶�����
const float32 BLOCK_FRICTION=2.f;//0.6f;//ÿһ��С����������Ħ��ϵ��
const float32 BLOCK_GRAVITY=0.8f;//ÿһ��С�����������N�����̶�����
const float32 g=10;//ģ������ϵ��
const float32 MAX_VELOCITY=18;//������֬�������ٶȣ���/�룩

//ȫ�ֺ���
b2Joint* createRevoluteJoint(b2World* world,b2Body* body1,b2Body* body2,b2Vec2 anchor){//��������body��һ���ڵ㴴��һ����ת�ؽڲ�����
		//����ؽ�����
		b2RevoluteJointDef revoluteJoint;
		revoluteJoint.Initialize( body1, body2, anchor);
		revoluteJoint.collideConnected = false;//�˹ؽڵ����������Ƿ���ײ���
		revoluteJoint.lowerAngle=-LIMIT_ANGLE;
		revoluteJoint.upperAngle=LIMIT_ANGLE;
		revoluteJoint.enableLimit=true;
		//�������
		revoluteJoint.enableMotor = false;
		//�������������ٶȣ���λΪ ����/�룬������ΪMath.PI����ÿ����ת180��
		revoluteJoint.motorSpeed =10;
		revoluteJoint.maxMotorTorque = 500;//��������Ť��ֵ
		return world->CreateJoint(&revoluteJoint);
}

//
//GameResinBallModel.cpp
//
GameResinBallModel::GameResinBallModel():_ballBody(NULL),_world(NULL)
	,_currentBlockHeight(0),_currentBlockWidth(0),attack(10)
	,coveredInsect(NULL),lastPosition(ccp(0,0)),_radius(0)
{
}


GameResinBallModel::~GameResinBallModel()
{
	_world=NULL;
}
void GameResinBallModel::initBallBody(float32 radius){
	//const float32 radius=20;//ʵ��Բ�뾶
	//const float32 magnification=1.35f;//�������뾶��ԲԲ���ķŴ���

	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	float centerX = winSize.width / 2.f;
	float centerY = winSize.height / 2.f;
	this->setPosition(centerX,centerY);
	lastPosition=this->getPosition();
//	CCLog("resinBallBody:%f,%f",centerX,centerY);

	b2Filter filter;
	filter.groupIndex=k_resinBallBodyGroup;
	filter.categoryBits=k_resinBallRealCategory;
	filter.maskBits=k_resinBallRealMask;
	_ballBody=B2EasyBox2D::createCircle(_world,centerX,centerY,radius,this,&filter);
	b2Fixture* fixture=_ballBody->GetFixtureList();
	fixture->SetRestitution(0);
	_ballBody->SetFixedRotation(true);
	
	//����һ��sensorFixture, ��������Ӽ����ص�����ײ
	b2CircleShape sensorShape;
	sensorShape.m_radius=0.5;
	b2Filter sensorFilter;
	sensorFilter.groupIndex=k_resinBallBodyGroup;
	sensorFilter.categoryBits=k_resinBallSensorCategory;
	sensorFilter.maskBits=k_resinBallSensorMask;

	b2FixtureDef sensorDef;
	sensorDef.shape=&sensorShape;
	sensorDef.filter=sensorFilter;
	sensorDef.isSensor=true;
	_sensorBall=_ballBody->CreateFixture(&sensorDef);

	//_delegateBall=B2EasyBox2D::createCircle(_world,centerX,centerY,radius+1,false,this,10.0f,0.8f,0,false,NULL,true);
}

bool GameResinBallModel::init(b2World* world){
	bool pRet=false;
	do{
		_world=world;
		this->setTag(TAG_RESINBALL);
		_ballRadius=30;
		float factor=CCDirector::sharedDirector()->getContentScaleFactor();
		//_ballRadius*=factor;
		this->initBallBody(_ballRadius);//�뾶

		const float32 magnification=1.35f;//�������뾶��ԲԲ���ķŴ���
		_radius=magnification*_ballRadius;
		this->createCircleBridge(20,_radius);

		//this->setPosition(ccp(10,0));
		//this->schedule(schedule_selector(GameResinBallModel::update));

		pRet=true;
	}while(0);
	return pRet;
}
void GameResinBallModel::myDraw(){
	CCPoint filledVertives[100];//={ccp(100,100),ccp(500,100),ccp(500,500),ccp(250,250),ccp(100,500)};
	for(unsigned int i=0;i<_blockJoints.size();i++){
		filledVertives[i]=ccp(this->_blockJoints[i]->GetAnchorA().x*PTM_RATIO,
			this->_blockJoints[i]->GetAnchorA().y*PTM_RATIO);
	}
	
 	ccDrawSolidPoly(filledVertives,this->_blockJoints.size(),ccc4f(255,165,0,100));
	ccDrawColor4B(255,69,0,255);
	b2Body *ballBody=_ballBody;
	ccDrawCircle(ccp(ballBody->GetWorldCenter().x*PTM_RATIO,ballBody->GetWorldCenter().y*PTM_RATIO),
		ballBody->GetFixtureList()->GetShape()->m_radius*PTM_RATIO,M_PI_2,50,false);
	ccDrawPoint(ccp(ballBody->GetWorldCenter().x*PTM_RATIO,ballBody->GetWorldCenter().y*PTM_RATIO));
	DrawUtil::drawSolidCircle(ccp(ballBody->GetWorldCenter().x*PTM_RATIO,ballBody->GetWorldCenter().y*PTM_RATIO),
		ballBody->GetFixtureList()->GetShape()->m_radius*PTM_RATIO,50,ccc4f(255,0,0,1));

		for(unsigned int i=0;i<_blocks.size();i++){
//ccDrawSolidRect(ccp(0,0),ccp(50,50),ccc4f(0,255,0,255));//��ɫ
		}
	//DrawUtil::drawSolidCircle(ccp(ballBody->GetWorldCenter().x*PTM_RATIO,ballBody->GetWorldCenter().y*PTM_RATIO),
		//(ballBody->GetFixtureList()->GetShape()->m_radius*0.8)*PTM_RATIO,50,ccc4f(255,65,0,1));
	
}

void GameResinBallModel::createCircleBridge(int number,float32 radius){//����Բ�����뾶������С������������Բ��
	double alta=b2_pi/number;//ÿ��С����Ƕ�֮��
	double r_small=radius*cos(alta);//radiusΪ�Խڵ���ɵ�Բ�İ뾶��r_smallΪ��С�������ĵ���ɵ�Բ
	//���ݰ뾶�͸��������߶εĳ���
	double segmentHeight = 2.f*radius * sin(alta);//ÿ��С����ĳ���
	double segmentWidth =segmentHeight*0.3f;//ÿ��С����Ŀ��
	_currentBlockHeight=segmentHeight;
	_currentBlockWidth=segmentWidth;
	int i=0;
	//����ÿ���߶εĽǶȡ�����
	double angle = i*1.f/ number *b2_pi*2;//���Ҳ����м��λ���𣬼�VisibleRect.right
	double bx = r_small * cos(angle)+getPositionX();
	double by = r_small * sin(angle)+getPositionY();
	
	b2Filter filter;    //filter��ײ��������
	filter.groupIndex=k_blocksGroup;
	filter.categoryBits=k_blocksCategory;
	filter.maskBits=k_blocksMask;

	CCTexture2D *texture=CCTextureCache::sharedTextureCache()->addImage("amber_wai.png");
	CCSprite* block=CCSprite::createWithTexture(texture,CCRectMake(0,0,_currentBlockWidth,_currentBlockHeight));
	block->setPosition(ccpSub(ccp(bx,by),this->getPosition()));
	block->setRotation(-angle*180/M_PI);
	//CCLog("sprite:%f,%f",block->getPositionX(),block->getPositionY());
	//CCLog("CCNode:%f,%f",this->getPositionX(),this->getPositionY());
	addChild(block,10);

	b2Body *firstBody=B2EasyBox2D::createBox(_world,bx,by,segmentWidth,segmentHeight,block,&filter);
	firstBody->SetTransform(firstBody->GetWorldCenter(),angle);
	firstBody->GetFixtureList()->SetRestitution(0);
	_blocks.push_back(firstBody);//����_blocks������

	b2Body *body;//��ǰbody
	b2Body *preBody=firstBody;//��һ��body

	//forѭ������number���߶Σ��ϳ�Բ�α߽�
	for (i = 1; i < number; i++) {
		//����ÿ���߶εĽǶȡ�����
		angle = i*1.f/number *b2_pi*2;
		float bx = r_small * cos(angle)+getPositionX();
		float by = r_small * sin(angle)+getPositionY();

		//�����з���ľ��θ��壬�ϳ��ܵ�Բ�θ���
	CCSprite* block=CCSprite::createWithTexture(texture,CCRectMake(0,0,_currentBlockWidth,_currentBlockHeight));
	block->setPosition(ccpSub(ccp(bx,by),this->getPosition()));
	block->setRotation(-angle*180/M_PI);
	addChild(block,10);
		body=B2EasyBox2D::createBox(_world,bx,by,segmentWidth,segmentHeight,block,&filter);
		body->SetTransform(body->GetWorldCenter(),angle);//���ýǶ�
		body->GetFixtureList()->SetRestitution(0);
		_blocks.push_back(body);//����_blocks������
		//����ڵ㣬��С�����ཻ��������Ĺ�ʽΪ��r*cos(0.5*2*pi/n+k*2*pi/n),r*sin(0.5*2*pi/n+k*2*pi/n)��+centerXY
		b2Vec2 anchor=b2Vec2((radius*cos((2*i-1)*alta)+getPositionX())/PTM_RATIO,(radius*sin((2*i-1)*alta)+getPositionY())/PTM_RATIO);		
		b2Joint* revoluteJoint=createRevoluteJoint(_world,preBody,body,anchor);//������ת�ؽ�
		
		_blockJoints.push_back(revoluteJoint);//����ת�ؽڼ��뵽_blockJoint������
		
		preBody = body;//����preBody���õ�ǰ�ĸ���
	}//for
	//����ڵ�
	b2Vec2 anchor=b2Vec2((radius*cos((2*i-1)*alta)+getPositionX())/PTM_RATIO,(radius*sin((2*i-1)*alta)+getPositionY())/PTM_RATIO);
	b2Joint* revoluteJoint=createRevoluteJoint(_world,body,firstBody,anchor);//�����һ���ڵ����ӵ�firstBody
		
	_blockJoints.push_back(revoluteJoint);
}

void GameResinBallModel::initObserver(GameModelDelegate* pDelegate){
	_observerDelegate=pDelegate;
}

void GameResinBallModel::accelerateCallBack(CCAcceleration* pAccelerationValue){//�������仯ʱ����б�ֻ���Ļ���Ļص�����
	double x=pAccelerationValue->x;
	double y=pAccelerationValue->y;
	double z=pAccelerationValue->z;

	//ʵ�����������Ħ����ģ��
	float32 zz=b2Vec2(x,y).Length();
	float32 F=BALL_GRAVITY*zz;//����
	float32 f=fabs(BALL_FRICTION*BALL_GRAVITY*z);//Ħ����
	float FF=F-f;//����(N)
	b2Vec2 force=b2Vec2(x*fabs(FF)/zz,y*fabs(FF)/zz);
	if(_ballBody->GetLinearVelocity().Length()>0||FF>0){//body���ٶȣ���������Ϊ����
		if(_ballBody->GetLinearVelocity().Length()<MAX_VELOCITY||FF<0)//���û�дﵽ����ٶȻ�������ΪĦ������
			_ballBody->ApplyForceToCenter(force);//�������ʩ�ӵ�body��
	}
	
	//ÿ��С�����Ħ������ģ��
	for(unsigned int i=0;i<_blocks.size();i++){
		float32 zz=b2Vec2(x,y).Length();
		float32 F=BLOCK_GRAVITY*zz;//����
		float32 f=fabs(BLOCK_FRICTION*BLOCK_GRAVITY*z);//Ħ����
		/*float FF=F-f;//����(N)*/
		//b2Vec2 force=b2Vec2(x*fabs(FF)/zz,y*fabs(FF)/zz);
		b2Vec2 nowVelocity=_blocks[i]->GetLinearVelocity();//����С������ٶ�
		float32 V=nowVelocity.Length();//��ǰС������ٶ�
		b2Vec2 force=b2Vec2(-nowVelocity.x*f/V,-nowVelocity.y*f/V);//�õ�Ħ���������뵱ǰ�ٶȷ���,���ԼӸ���
		if(V>1){//�����ǰ�ٶȲ�Ϊ0��ʩ�ӷ���Ħ����������ʩ����
			_blocks[i]->ApplyForceToCenter(force);
		}else{
			_blocks[i]->ApplyForceToCenter(b2Vec2(0,0));
			_blocks[i]->SetLinearVelocity(b2Vec2(0,0));
		}
	}
}


void GameResinBallModel::shrinkResinBallBody(float32 scale){
	_ballBody->GetFixtureList()->GetShape()->m_radius=0.1f;

	vector<b2Vec2> anchors;
	for(unsigned int i=0;i<_blocks.size();i++){
	B2EasyBox2D::shrinkBox(_blocks[i],_currentBlockHeight,_currentBlockWidth,0.5f);
	b2Vec2 currentPosition=_blocks[i]->GetWorldCenter();
	b2Vec2 center=b2Vec2(getPositionX()/PTM_RATIO,getPositionY()/PTM_RATIO);
	b2Vec2 newPosition=b2Vec2(center.x+(currentPosition.x-center.x)*scale,center.y+(currentPosition.y-center.y)*scale);
	_blocks[i]->SetTransform(newPosition,0);

	b2Vec2 position=b2Vec2(center.x+(_blockJoints[i]->GetAnchorA().x-center.x)*scale,
		center.y+(_blockJoints[i]->GetAnchorA().y-center.y)*scale);
	anchors.push_back(position);
	}
	for(unsigned int i=0;i<_blockJoints.size();i++){
		int nexti=i+1;
		if(nexti==_blockJoints.size()) nexti=0;
	//	b2Joint* newJoint=createRevoluteJoint(_world,_blocks[i],_blocks[nexti],anchors[i]);
		_world->DestroyJoint(_blockJoints[i]);
		//_blockJoints[i]=newJoint;
	}
}

void GameResinBallModel::update(float dt){
	lastPosition=this->getPosition();
	this->setPosition(_ballBody->GetWorldCenter().x*PTM_RATIO,_ballBody->GetWorldCenter().y*PTM_RATIO);
	//CCLog("ballPos:%f,%f",this->getPositionX(),this->getPositionY());
	//_delegateBall->SetTransform(_ballBody->GetWorldCenter(),0);
	
	for(unsigned int i=0;i<_blocks.size();i++){//ʵʱ����λ�úͽǶ�
		b2Body* body=_blocks[i];
		if(body->GetUserData()!=NULL){
			CCNode* userData=(CCNode*)body->GetUserData();
			userData->setPosition(ccpSub(ccp(body->GetPosition().x*PTM_RATIO,body->GetPosition().y*PTM_RATIO),
				this->getPosition()));//Ҫ��ȥ�������꣬�Ա��box2dһ��
			userData->setRotation(-body->GetAngle()*180/M_PI);
		}
	}

	if(!_observerDelegate) return;

	_observerDelegate->testModelDelegate();
}

void GameResinBallModel::attackInsect(GameInsectModel* insect,float reducedBlood){
	if(insect->attacked(reducedBlood)){
		coveredInsect=NULL;
	}
}

//
//����ӵ���ײ���
//
void GameResinBallModel::beginContact(GameInsectModel* insect){
	coveredInsect=insect;
	//isCoverInsect=true;
}
void GameResinBallModel::endContact(){
//	isCoverInsect=false;
	coveredInsect=NULL;
}
void GameResinBallModel::handleContactWithInsect(float dt){
	if(coveredInsect!=NULL){
		float reducedBlood=attack*dt;
		this->attackInsect(coveredInsect,reducedBlood);
	}
}


b2Body* GameResinBallModel::getBallBody(){
	return _ballBody;
}

CCPoint GameResinBallModel::getDeltaPosition(){
	return ccpSub(this->getPosition(),lastPosition);
}
float GameResinBallModel::getRadius(){
	return _radius;
}
float GameResinBallModel::getBallRadius(){
	return _ballRadius;
}