#include "GameResinBallModel.h"
#include "Constant.h"
#include "B2EasyBox2D.h"
#include "DrawUtil.h"
#include "cPolySprite.h"

USING_NS_CC;

const int Origin_blood=1000;//初始量
const int times=5;//缩放次数
const int tag_PolySprite=123;//多边形图片的tag索引
//全局函数
//根据两个body和一个节点创建一个旋转关节并返回
b2Joint* createRevoluteJoint(b2World* world,b2Body* body1,b2Body* body2,b2Vec2 anchor){
		//定义关节需求
		b2RevoluteJointDef revoluteJoint;
		revoluteJoint.Initialize( body1, body2, anchor);
		revoluteJoint.collideConnected = false;//此关节的两个刚体是否碰撞检测
		revoluteJoint.lowerAngle=-LIMIT_ANGLE;
		revoluteJoint.upperAngle=LIMIT_ANGLE;
		revoluteJoint.enableLimit=true;
		//开启马达
		revoluteJoint.enableMotor = false;
		//设置马达的最大角速度，单位为 弧度/秒，如设置为Math.PI，即每秒钟转180度
		revoluteJoint.motorSpeed =10;
		revoluteJoint.maxMotorTorque = 500;//设置最大的扭力值
		return world->CreateJoint(&revoluteJoint);
}

//
//GameResinBallModel.cpp
//
GameResinBallModel::GameResinBallModel():_ballBody(NULL),_world(NULL)
	,_currentBlockHeight(0),_currentBlockWidth(0),attack(50)
	,coveredInsect(NULL),lastPosition(ccp(0,0)),_radius(0),blood(Origin_blood),countBlood(0)
{
  BALL_FRICTION=0.1f;//实心球与地面的摩擦系数
  BALL_GRAVITY=150*2;//实心球重力（N）,固定不变
  BLOCK_FRICTION=2.f;//0.6f;//每一个小方块与地面的摩擦系数
  BLOCK_GRAVITY=0.8f;//每一个小方块的重力（N），固定不变
  MAX_VELOCITY=18;//主角树脂球的最大速度（米/秒）

  this->setReached(Origin_blood*(times-1)*1.f/times);
}


GameResinBallModel::~GameResinBallModel()
{
	this->clean();
	_world=NULL;
}
void GameResinBallModel::clean(){
	_blockJoints.clear();
	_blocks.clear();
}
//初始化实心球的刚体
void GameResinBallModel::initBallBody(float32 radius){
	//const float32 radius=20;//实心圆半径
	//const float32 magnification=1.35f;//环形链半径比圆圆环的放大倍数

	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	float centerX = winSize.width / 2.f;
	float centerY = winSize.height / 2.f;
	this->setPosition(centerX,centerY);
	lastPosition=this->getPosition();
//	CCLog("resinBallBody:%f,%f",centerX,centerY);

	CCNode* realBall=CCNode::create();
	this->addChild(realBall,0,ID_REAL_BALL);

	b2Filter filter;
	filter.groupIndex=k_resinBallBodyGroup;
	filter.categoryBits=k_resinBallRealCategory;
	filter.maskBits=k_resinBallRealMask;
	_ballBody=B2EasyBox2D::createCircle(_world,centerX,centerY,radius,this,&filter);
	b2Fixture* fixture=_ballBody->GetFixtureList();
	fixture->SetRestitution(0);
	fixture->SetUserData(realBall);
	_ballBody->SetFixedRotation(true);
	
	//创建一个sensorFixture, 用来与虫子检测可重叠的碰撞
	b2CircleShape sensorShape;
	sensorShape.m_radius=1;
	b2Filter sensorFilter;
	sensorFilter.groupIndex=k_resinBallBodyGroup;
	sensorFilter.categoryBits=k_resinBallSensorCategory;
	sensorFilter.maskBits=k_resinBallSensorMask;

	CCNode* sensorId=CCNode::create();
	this->addChild(sensorId,0,ID_SENSOR_BALL);

	b2FixtureDef sensorDef;
	sensorDef.shape=&sensorShape;
	sensorDef.filter=sensorFilter;
	sensorDef.isSensor=true;
	sensorDef.userData=sensorId;
	_sensorBall=_ballBody->CreateFixture(&sensorDef);
	
	//_delegateBall=B2EasyBox2D::createCircle(_world,centerX,centerY,radius+1,false,this,10.0f,0.8f,0,false,NULL,true);
}

bool GameResinBallModel::init(b2World* world){
	bool pRet=false;
	do{
		_world=world;
		this->setTag(TAG_RESINBALL);
		_ballRadius=40;//实心球的半径30
		float factor=CCDirector::sharedDirector()->getContentScaleFactor();
		//_ballRadius*=factor;
		this->initBallBody(_ballRadius);//半径

		const float32 magnification=1.35f;//环形链半径比圆圆环的放大倍数
		_radius=magnification*_ballRadius;
		this->createCircleBridge(20,_radius);

		this->initPolygonTexture();
		//this->shrinkResinBallBody(0.5);

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
//ccDrawSolidRect(ccp(0,0),ccp(50,50),ccc4f(0,255,0,255));//绿色
		}
	//DrawUtil::drawSolidCircle(ccp(ballBody->GetWorldCenter().x*PTM_RATIO,ballBody->GetWorldCenter().y*PTM_RATIO),
		//(ballBody->GetFixtureList()->GetShape()->m_radius*0.8)*PTM_RATIO,50,ccc4f(255,65,0,1));
	
}

void GameResinBallModel::createCircleBridge(int number,float32 radius){//根据圆环链半径和所需小方块数量创建圆环
	double alta=b2_pi/number;//每个小方块角度之半
	double r_small=radius*cos(alta);//radius为以节点组成的圆的半径，r_small为以小方块中心点组成的圆
	//根据半径和个数计算线段的长度
	double segmentHeight = 2.f*radius * sin(alta);//每个小方块的长度
	double segmentWidth =segmentHeight*0.3f;//每个小方块的宽度
	_currentBlockHeight=segmentHeight;
	_currentBlockWidth=segmentWidth;
	int i=0;
	//计算每个线段的角度、坐标
	double angle = i*1.f/ number *b2_pi*2;//从右侧最中间的位置起，即VisibleRect.right
	double bx = r_small * cos(angle)+getPositionX();
	double by = r_small * sin(angle)+getPositionY();
	
	b2Filter filter;    //filter碰撞过滤设置
	filter.groupIndex=k_blocksGroup;
	filter.categoryBits=k_blocksCategory;
	filter.maskBits=k_blocksMask;

	CCTexture2D *texture=CCTextureCache::sharedTextureCache()->addImage("amber_wai.png");
	CCSprite* block=CCSprite::createWithTexture(texture,CCRectMake(0,0,_currentBlockWidth,_currentBlockHeight));
	//block->setTag(TAG_BLOCK);
	block->setPosition(ccpSub(ccp(bx,by),this->getPosition()));
	block->setRotation(-angle*180/M_PI);
	//CCLog("sprite:%f,%f",block->getPositionX(),block->getPositionY());
	//CCLog("CCNode:%f,%f",this->getPositionX(),this->getPositionY());
	addChild(block,10,ID_BLOCK);

	b2Body *firstBody=B2EasyBox2D::createBox(_world,bx,by,segmentWidth,segmentHeight,block,&filter);
	firstBody->SetTransform(firstBody->GetWorldCenter(),angle);
	firstBody->GetFixtureList()->SetRestitution(0);
	firstBody->GetFixtureList()->SetUserData(block);
	_blocks.push_back(firstBody);//存入_blocks数组中

	b2Body *body;//当前body
	b2Body *preBody=firstBody;//上一个body

	//for循环创建number个线段，合成圆形边界
	for (i = 1; i < number; i++) {
		//计算每个线段的角度、坐标
		angle = i*1.f/number *b2_pi*2;
		float bx = r_small * cos(angle)+getPositionX();
		float by = r_small * sin(angle)+getPositionY();

		//创建有方向的矩形刚体，合成总的圆形刚体
	CCSprite* block=CCSprite::createWithTexture(texture,CCRectMake(0,0,_currentBlockWidth,_currentBlockHeight));
//	block->setTag(TAG_BLOCK);
	block->setPosition(ccpSub(ccp(bx,by),this->getPosition()));
	block->setRotation(-angle*180/M_PI);
	addChild(block,10,ID_BLOCK);
		body=B2EasyBox2D::createBox(_world,bx,by,segmentWidth,segmentHeight,block,&filter);
		body->SetTransform(body->GetWorldCenter(),angle);//设置角度
		body->GetFixtureList()->SetRestitution(0);
		firstBody->GetFixtureList()->SetUserData(block);
		_blocks.push_back(body);//存入_blocks数组中
		//定义节点，两小方块相交处点坐标的公式为（r*cos(0.5*2*pi/n+k*2*pi/n),r*sin(0.5*2*pi/n+k*2*pi/n)）+centerXY
		b2Vec2 anchor=b2Vec2((radius*cos((2*i-1)*alta)+getPositionX())/PTM_RATIO,(radius*sin((2*i-1)*alta)+getPositionY())/PTM_RATIO);		
		b2Joint* revoluteJoint=createRevoluteJoint(_world,preBody,body,anchor);//创建旋转关节
		
		_blockJoints.push_back(revoluteJoint);//将旋转关节加入到_blockJoint数组中
		
		preBody = body;//设置preBody引用当前的刚体
	}//for
	//定义节点
	b2Vec2 anchor=b2Vec2((radius*cos((2*i-1)*alta)+getPositionX())/PTM_RATIO,(radius*sin((2*i-1)*alta)+getPositionY())/PTM_RATIO);
	b2Joint* revoluteJoint=createRevoluteJoint(_world,body,firstBody,anchor);//将最后一个节点链接到firstBody
		
	_blockJoints.push_back(revoluteJoint);
}

void GameResinBallModel::initObserver(GameModelDelegate* pDelegate){
	_observerDelegate=pDelegate;
}

void GameResinBallModel::reduceBodyVel(float scale){
	b2Vec2 vel=_ballBody->GetLinearVelocity();
	b2Vec2 newVel=b2Vec2(vel.x*scale,vel.y*scale);
	CCLog("velBeforeReduce:%f",vel.Length());
	_ballBody->SetLinearVelocity(newVel);
	CCLog("velAfterReduce:%f",newVel.Length());
}

void GameResinBallModel::accelerateCallBack(CCAcceleration* pAccelerationValue){//加速器变化时（倾斜手机屏幕）的回调函数
	double x=pAccelerationValue->x;
	double y=pAccelerationValue->y;
	double z=pAccelerationValue->z;

	//实心球的重力、摩擦力模拟
	float32 zz=b2Vec2(x,y).Length();
	float32 F=BALL_GRAVITY*zz;//动力
	float32 f=fabs(BALL_FRICTION*BALL_GRAVITY*z);//摩擦力
	float FF=F-f;//总力(N)
	b2Vec2 force=b2Vec2(x*fabs(FF)/zz,y*fabs(FF)/zz);
	if(_ballBody->GetLinearVelocity().Length()>0||FF>0){//body有速度，或者总力为动力
		if(_ballBody->GetLinearVelocity().Length()<MAX_VELOCITY||FF<0)//如果没有达到最大速度或者总力为摩擦阻力
			_ballBody->ApplyForceToCenter(force);//则把总力施加到body上
	}
	
	//每个小方块的摩擦力的模拟
	for(unsigned int i=0;i<_blocks.size();i++){
		float32 zz=b2Vec2(x,y).Length();
		float32 F=BLOCK_GRAVITY*zz;//动力
		float32 f=fabs(BLOCK_FRICTION*BLOCK_GRAVITY*z);//摩擦力
		/*float FF=F-f;//总力(N)*/
		//b2Vec2 force=b2Vec2(x*fabs(FF)/zz,y*fabs(FF)/zz);
		b2Vec2 nowVelocity=_blocks[i]->GetLinearVelocity();//现在小方块的速度
		float32 V=nowVelocity.Length();//当前小方块的速度
		b2Vec2 force=b2Vec2(-nowVelocity.x*f/V,-nowVelocity.y*f/V);//得到摩擦力向量与当前速度反向,所以加负号
		if(V>1){//如果当前速度不为0才施加反向摩擦力；否则不施加力
			_blocks[i]->ApplyForceToCenter(force);
		}else{
			_blocks[i]->ApplyForceToCenter(b2Vec2(0,0));
			_blocks[i]->SetLinearVelocity(b2Vec2(0,0));
		}
	}
}

void GameResinBallModel::updatePolygonTexture(){
	if(_blockJoints.size()>2){
		cPolySprite *sprite=(cPolySprite*)this->getChildByTag(tag_PolySprite);
		const int count=_blockJoints.size();
		const int indexCount=(count-2)*3;

		float width=200;//图片宽度
		float height=200;//图片高度

		width=sprite->getContentSize().width;
		height=sprite->getContentSize().height;
		CCPoint points[100];
		int index[100];
		for(unsigned int i=0;i<count;i++){
			CCPoint p=ccp(_blockJoints[i]->GetAnchorA().x*PTM_RATIO,_blockJoints[i]->GetAnchorA().y*PTM_RATIO);
		//	CCLog("anchor:%f,%f",p.x,p.y);
			points[i]=ccp(p.x-(this->getPositionX()-width/2),p.y-(this->getPositionY()-height/2));
			//CCLog("%f,%f",points[i].x,points[i].y);
			points[i]=ccp(points[i].x/width,points[i].y/height);
			//CCLog("%f,%f",points[i].x,points[i].y);
			points[i].y=1-points[i].y;
			//CCLog("%f,%f",points[i].x,points[i].y);
		}
		int k=0;
		for(unsigned int i=1;i<count-1;i++){
			if(k+3>indexCount) break;

			index[k++]=0;
			index[k++]=i;
			index[k++]=i+1;
		}
		sprite->initWithUV(points,index,count);
	}
}
void GameResinBallModel::initPolygonTexture(){
	if(_blockJoints.size()>2){
		const int count=_blockJoints.size();
		const int indexCount=(count-2)*3;

		const char* fileName="nei.png";
		float width=200;//图片宽度
		float height=200;//图片高度
		CCSprite* p=CCSprite::create(fileName);
		width=p->getContentSize().width;
		height=p->getContentSize().height;
		CCPoint points[100];
		int index[100];
		for(unsigned int i=0;i<count;i++){
			CCPoint p=ccp(_blockJoints[i]->GetAnchorA().x*PTM_RATIO,_blockJoints[i]->GetAnchorA().y*PTM_RATIO);
		//	CCLog("anchor:%f,%f",p.x,p.y);
			points[i]=ccp(p.x-(this->getPositionX()-width/2),p.y-(this->getPositionY()-height/2));
			//CCLog("%f,%f",points[i].x,points[i].y);
			points[i]=ccp(points[i].x/width,points[i].y/height);
			//CCLog("%f,%f",points[i].x,points[i].y);
			points[i].y=1-points[i].y;
			//CCLog("%f,%f",points[i].x,points[i].y);
		}
		int k=0;
		for(unsigned int i=1;i<count-1;i++){
			if(k+3>indexCount) break;

			index[k++]=0;
			index[k++]=i;
			index[k++]=i+1;
		}
		cPolySprite *csp = cPolySprite::create(fileName, points, count, index);
		csp->setTag(tag_PolySprite);
		this->addChild(csp);
		//csp->setAnchorPoint(ccp(0.5,0.5));
		/*
		CCActionInterval *colorAction = CCRepeatForever::create((CCActionInterval *)CCSequence::create(
        CCTintTo::create(0.2f, 255, 0, 0),
        CCTintTo::create(0.2f, 0, 255, 0),
        CCTintTo::create(0.2f, 0, 0, 255),
        CCTintTo::create(0.2f, 0, 255, 255),
        CCTintTo::create(0.2f, 255, 255, 0),
        CCTintTo::create(0.2f, 255, 0, 255),
        CCTintTo::create(0.2f, 255, 255, 255),
        NULL));
		csp->runAction(colorAction);
		csp->runAction(CCMoveBy::create(4,ccp(100,100)));
		csp->runAction( CCRepeatForever::create((CCActionInterval *)CCSequence::create(
        CCTintTo::create(0.2f, 255, 0, 0),
        CCTintTo::create(0.2f, 0, 255, 0),
        CCTintTo::create(0.2f, 0, 0, 255),
        CCTintTo::create(0.2f, 0, 255, 255),
        CCTintTo::create(0.2f, 255, 255, 0),
        CCTintTo::create(0.2f, 255, 0, 255),
        CCTintTo::create(0.2f, 255, 255, 255),
        NULL)));*/
		//csp->runAction(CCFadeOut::create(4));

	//	MYCCSprite* p2=MYCCSprite::create("cup.png");
	//	p2->setPosition(ccp(0,0));
	//	this->addChild(p2);
	//	p2->runAction(colorAction);
	}
}

void GameResinBallModel::shrinkResinBallBody(float32 scale){
//	cPolySprite* polySp=(cPolySprite*)this->getChildByTag(tag_PolySprite);
//	polySp->runAction(CCShakyTiles3D::create(10,true,ccg(polySp->getContentSize().width,polySp->getContentSize().height),2));
	_ballRadius*=scale;
	//_world->DestroyBody(_ballBody);
	_ballBody->GetFixtureList()->GetShape()->m_radius=_ballRadius/PTM_RATIO;
	_ballBody->GetFixtureList()->GetNext()->GetShape()->m_radius*=scale;
	
	for(unsigned int i=0;i<_blockJoints.size();i++){
		_world->DestroyJoint(_blockJoints[i]);
	}
	for(unsigned int i=0;i<_blocks.size();i++){
		CCNode* userData=(CCNode*)_blocks[i]->GetUserData();
		_world->DestroyBody(_blocks[i]);
		this->removeChild(userData,true);
	//	_blocks[i]->GetFixtureList()->GetShape()->set
	}
	_blockJoints.clear();
	_blocks.clear();
	CCLog("_blocks.size():%d",_blocks.size());
	
	//this->initBallBody(_ballRadius);
	const float32 magnification=1.35f;//环形链半径比圆圆环的放大倍数
	_radius=magnification*_ballRadius;
	this->createCircleBridge(20,_radius);
	
	CCLog("_ballRadius:%f",_ballRadius);
	CCLog("_radius:%f",_radius);
}
void GameResinBallModel::shrinkResinByBlood(){
	if(blood<=this->getReached()){
		this->setReached(this->getReached()-Origin_blood/times);
		this->shrinkResinBallBody(0.8f);
	}
}

void GameResinBallModel::update(float dt){
	lastPosition=this->getPosition();
	this->setPosition(_ballBody->GetWorldCenter().x*PTM_RATIO,_ballBody->GetWorldCenter().y*PTM_RATIO);
	//CCLog("ballPos:%f,%f",this->getPositionX(),this->getPositionY());
	//_delegateBall->SetTransform(_ballBody->GetWorldCenter(),0);
	
	for(unsigned int i=0;i<_blocks.size();i++){//实时更新位置和角度
		b2Body* body=_blocks[i];
		if(body->GetUserData()!=NULL){
			CCNode* userData=(CCNode*)body->GetUserData();
			userData->setPosition(ccpSub(ccp(body->GetPosition().x*PTM_RATIO,body->GetPosition().y*PTM_RATIO),
				this->getPosition()));//要减去父亲坐标，以便跟box2d一致
			userData->setRotation(-body->GetAngle()*180/M_PI);
		}
	}
	//this->updatePolygonTexture();

	if(!_observerDelegate) return;

	_observerDelegate->testModelDelegate();

	this->shrinkResinByBlood();
}

void GameResinBallModel::attackInsect(BaseInsect* insect,float reducedBlood){
	insect->covered(reducedBlood);
}
bool GameResinBallModel::usedUp(){
	return blood<=0;
}

//与虫子的交互
void GameResinBallModel::attackedByEnemy(int attack){
	this->setBlood(this->getBlood()-attack);
}

//
//与虫子的碰撞检测
//
void GameResinBallModel::beginCover(BaseInsect* insect){
	coveredInsect=insect;
	//isCoverInsect=true;
}
void GameResinBallModel::endCover(){
//	isCoverInsect=false;
	coveredInsect=NULL;
}
void GameResinBallModel::coveringInsect(float dt){
	if(coveredInsect!=NULL&&!coveredInsect->isOutOfGas()){
		float reducedBlood=attack*dt;
		this->attackInsect(coveredInsect,reducedBlood);
	}else coveredInsect=NULL;
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