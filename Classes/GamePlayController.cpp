#include "GamePlayController.h"
#include "B2EasyBox2D.h"
#include "B2DebugDrawLayer.h"
#include "DrawUtil.h"
#include "Constant.h"
#include "VisibleRect.h"
#include "cPolySprite.h"
#include "GameMenuController.h"
#include "GameOverController.h"
#include "Message.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)   
#include "vld.h"   
#endif 

USING_NS_CC;

GamePlayController::GamePlayController():_insectView(NULL),_insectModel(NULL),_world(NULL)
	,_resinBallModel(NULL),_resinBallView(NULL),_mouseJoint(NULL),amberCount(0)
{
}


GamePlayController::~GamePlayController()
{	
	CC_SAFE_RELEASE_NULL(_resinBallView);
	CC_SAFE_RELEASE_NULL(_resinBallModel);
	CC_SAFE_RELEASE_NULL(buttonLayer);
	CC_SAFE_RELEASE_NULL(mapLayer);
	enemys.clear();
	_insects.clear();

	CC_SAFE_DELETE(contactListener);

	delete _world;
	_world=NULL;
}

void GamePlayController::initResinBall(){//初始化视图及其模型
	_resinBallModel=GameResinBallModel::create(_world);
	_resinBallModel->retain();
	addChild(_resinBallModel,100);
	
	_resinBallView=GameResinBallView::CREATE(this,_resinBallModel);
	_resinBallView->retain();

	addChild(_resinBallView,-1);

	_resinBallModel->initObserver(_resinBallView);
}
void GamePlayController::addInsect(){
	BaseInsect* insectm=MantisInsect::create(_world);
			addChild(insectm,0);

			GameInsectView* insectv=GameInsectView::CREATE(this,insectm);
			addChild(insectv,0);

			insectm->initObserver(insectv);
			_insects.push_back(insectm);
}
void GamePlayController::initInsect(){
		for(int i=0;i<2;i++){
			this->addInsect();
		}
		
}

bool GamePlayController::init(){
	bool pRet = false;

    do{
        // 先调用超类的init方法
		CC_BREAK_IF(! CCLayer::init());

		srand(time(NULL));//给随机数设置种子
		_world=B2EasyBox2D::createWorld(b2Vec2(0,0));
		B2DebugDrawLayer *dDraw=B2DebugDrawLayer::create(_world,PTM_RATIO);
		//addChild(dDraw,9999);

		contactListener=new myContactListener();
		_world->SetContactListener(contactListener);//碰撞检测监听
		
		this->setAccelerometerEnabled(true);//设置允许重力感应

		int consumeResin=CCUserDefault::sharedUserDefault()->getIntegerForKey(CONSUME_RESIN,10);
		int totalResin=CCUserDefault::sharedUserDefault()->getIntegerForKey(RESIN_VOLUME,0);
		CCUserDefault::sharedUserDefault()->setIntegerForKey(RESIN_VOLUME,totalResin-consumeResin);
		CCUserDefault::sharedUserDefault()->flush();
		this->initResinBall();//初始化树脂球模型及视图
		this->createWrapWall();//创建四周墙和一些静态障碍
		this->createEnvir();//创建环境
		this->initInsect();

		//初始化敌人
	/*	for(unsigned int i=0;i<3;i++){
			Enemy* enemy=Enemy::create(_world);
			enemy->retain();
			enemy->autorelease();
			addChild(enemy);
			EnemyView* enemyView=EnemyView::CREATE(this,enemy);
			enemyView->retain();
			enemyView->autorelease();
			addChild(enemyView);
			enemy->initObserver(enemyView);
			enemys.push_back(enemy);
		}*/

		//初始化按钮层
		buttonLayer=StaticLayer::create();
		buttonLayer->retain();
		buttonLayer->initControlSlider(_resinBallModel);//调试用，手机上通过滑动条控制树脂球的重力和摩擦系数
		addChild(buttonLayer,10);

		//初始化地图层
		mapLayer=MapLayer::create();
		mapLayer->retain();
		addChild(mapLayer,-10);

		buttonLayer->innerStage=mapLayer->innerStage;//调试用

        pRet = true;
    }while(0);
    return pRet;
}
void GamePlayController::onEnter(){

	CCLayer::onEnter();
}
void GamePlayController::onEnterTransitionDidFinish(){
	//开始游戏
	this->schedule(schedule_selector(GamePlayController::update));
	CCLayer::onEnterTransitionDidFinish();
}
void GamePlayController::onExitTransitionDidStart(){
	CCLayer::onExitTransitionDidStart();
}
void GamePlayController::onExit(){
	CCLayer::onExit();
}



void GamePlayController::createEnvir(){
	CCSprite* sp=CCSprite::create("petri_dish.png");
	sp->setPosition(ccp(900,300));
	this->addChild(sp,0,TAG_WALL);
	b2Body* petri_dish=B2EasyBox2D::createCircle(_world,900,300,140,sp);
	b2Fixture* fixture=petri_dish->GetFixtureList();
	fixture->SetDensity(100);
	fixture->SetFriction(1);
	fixture->SetRestitution(0.2);

}
 void GamePlayController::createWrapWall(){
	 CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	 winSize=CCSizeMake(winSize.width/PTM_RATIO,winSize.height/PTM_RATIO);
	 b2Vec2 leftTop=b2Vec2(-winSize.width*(MAP_SCALE-1)*0.5f,winSize.height+(MAP_SCALE-1)*0.5f*winSize.height);
	 b2Vec2 leftBottom=b2Vec2(leftTop.x,-(MAP_SCALE-1)*0.5f*winSize.height);
	 b2Vec2 rightTop=b2Vec2(winSize.width+winSize.width*(MAP_SCALE-1)*0.5f,leftTop.y);
	 b2Vec2 rightBottom=b2Vec2(rightTop.x,leftBottom.y);

	 CCNode* userDataWall=CCNode::create();
	 userDataWall->setTag(TAG_WALL);
	 this->addChild(userDataWall);

	 b2BodyDef bodyDef;
	 bodyDef.position.Set(0,0);
	 bodyDef.userData=userDataWall;
	 bodyDef.type=b2_staticBody;
	 b2Body* wall=_world->CreateBody(&bodyDef);

	 b2EdgeShape edgeShape; 
	 edgeShape.Set(leftBottom,rightBottom);  //下
	 
	 b2FixtureDef fixtureDef;
	 fixtureDef.shape = &edgeShape;
	 wall->CreateFixture(&fixtureDef);
	  
	 //上
	 edgeShape.Set(leftTop,rightTop);
	 wall->CreateFixture(&fixtureDef);
	 //左
	 edgeShape.Set(leftTop,leftBottom);
	 wall->CreateFixture(&fixtureDef);
	 //右
	 edgeShape.Set(rightTop,rightBottom);
	 wall->CreateFixture(&fixtureDef);
}
 void GamePlayController::updateBox2DUserData(){
	 for(b2Body* body=_world->GetBodyList();body!=NULL;body=body->GetNext()){
		 CCNode* sprite=(CCNode*)body->GetUserData();
		 if(sprite!=NULL&&sprite->getTag()==TAG_WALL){
			 sprite->setPosition(body->GetWorldCenter().x*PTM_RATIO,body->GetWorldCenter().y*PTM_RATIO);
			 sprite->setRotation(CC_RADIANS_TO_DEGREES(body->GetAngle()));
		 }
	 }
 }
void GamePlayController::draw(){
	
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	/*ccDrawSolidRect(ccp(0,0),ccp(PTM_RATIO/2,winSize.height),ccc4f(0,255,0,255));
	ccDrawSolidRect(ccp(0,0),ccp(winSize.width,PTM_RATIO/2),ccc4f(0,255,0,255));
	ccDrawSolidRect(ccp(winSize.width-PTM_RATIO/2,0),ccp(winSize.width,winSize.height),ccc4f(0,255,0,255));
	ccDrawSolidRect(ccp(0,winSize.height-PTM_RATIO/2),ccp(winSize.width,winSize.height),ccc4f(0,255,0,255));
	*/
	 _resinBallModel->myDraw();
/*
	for(unsigned int i=0;i<bodys.size();i++){
		DrawUtil::drawSolidCircle(ccp(bodys[i]->GetWorldCenter().x*PTM_RATIO,bodys[i]->GetWorldCenter().y*PTM_RATIO),
		bodys[i]->GetFixtureList()->GetShape()->m_radius*PTM_RATIO,50,ccc4f(0,0,255,255));
	}*/
	//ccDrawSolidRect(ccp(75,25),ccp(125,175),ccc4f(0,255,0,255));
	//ccDrawSolidRect(ccp(575,75),ccp(625,125),ccc4f(0,255,0,255));

}

CCScene* GamePlayController::scene(){
    CCScene *scene = NULL;
    do{
		scene = CCScene::create();
        CC_BREAK_IF(!scene);

        GamePlayController *layer = GamePlayController::create();
        CC_BREAK_IF(!layer);

        scene->addChild(layer);

    }while(0);

    return scene;
}

void GamePlayController::updateInsectNumber(){
	if(_insects.size()>MIN_NUM) return;CCLog("fsdf");
	if(CCRANDOM_0_1()>0.8){//0.8的概率
		this->addInsect();
	}
}

void GamePlayController::update(float dt){
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/

	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
//	world->Step(dt, velocityIterations, positionIterations);
	this->step(dt);
	this->updateBox2DUserData();
	for(unsigned int i=0;i<_insects.size();i++){
		_insects[i]->update(dt);
	}
	_resinBallModel->update(dt);
	for(unsigned int i=0;i<enemys.size();i++){
		enemys[i]->followResin(_resinBallModel->getPosition());
		enemys[i]->update(dt);
	}
	/*if(_mouseJoint==NULL)
		_resinBallModel->shrinkResinBallBody(0.999);*/
	//更新地图坐标
	CCScene* scene=CCDirector::sharedDirector()->getRunningScene();
	CCPoint last=scene->getPosition();
	mapLayer->updateFloor(_resinBallModel->getDeltaPosition());
	mapLayer->updateMap(_resinBallModel->getPosition());
	//保持按钮位置不变
	buttonLayer->keepStill(last);
	buttonLayer->setResinCount(_resinBallModel);

	this->updateInsectNumber();
	//Iterate over the bodies in the physics world
	//CCLog("update:%f",dt);

//	_world->Step(dt, 10, 10); 
}
void GamePlayController::step(float dt){//box2d世界时间步 与 cocos2d-x更新频率 的一种适应
	float32 frameTime = dt;
	int stepsPerformed = 0;
	while ( (frameTime > 0.0) && (stepsPerformed < MAXIMUM_NUMBER_OF_STEPS) ){
		float32 deltaTime = min(frameTime,FIXED_TIMESTEP);
		frameTime -= deltaTime;
		if (frameTime < MINIMUM_TIMESTEP) {
			deltaTime += frameTime;
			frameTime = 0.0f;
		}
		_world->Step(deltaTime,VELOCITY_ITERATIONS,POSITION_ITERATIONS);
		stepsPerformed++;
		this->afterStep(dt); // process collisions and result from callbacks called by the step
		//CCLog("dt:%f",dt);
	}
	_world->ClearForces();
	
}
//每一个时间步后需要执行的操作
void GamePlayController::afterStep(float dt){
	vector<BaseInsect* >::iterator it = _insects.begin(); 
	while (it != _insects.end()) { 
		BaseInsect* insect=(BaseInsect*)*it; //CCLog("%d",insect->getAlive());       
		if (!insect->getAlive()) { 
			insect->clean();
			it = _insects.erase(it);    
			CCLog("after delete insect No.:",_insects.size());
			amberCount++;
		} else {	
			//insect->handleContact();
			it++;    
		}
	}
	MessageVector::handleMessage();
	for(unsigned int i=0;i<enemys.size();i++){
		enemys[i]->eatResin(_resinBallModel,dt);
	}
	_resinBallModel->handleContactWithInsect(dt);//给虫子扣血
	this->checkGameOverAndDo();
}
void GamePlayController::shrinkCallBack(CCObject* pSender){
	_resinBallModel->shrinkResinBallBody(0.99);
}
void GamePlayController::checkGameOverAndDo(){
	if(_resinBallModel->usedUp()||_insects.size()==0){
		//CCDelayTime* delay=CCDelayTime::create(1);
		
		this->menuGameOverCallback(this);
	}
}
void GamePlayController::menuGoBackCallback(CCObject* pSender){
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GameMenuController::scene()));
}
void GamePlayController::menuGameOverCallback(CCObject* pSender){
	CCUserDefault::sharedUserDefault()->setIntegerForKey(GET_AMBER,amberCount);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(GET_SCORE,amberCount*100+CCRANDOM_0_1()*10);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(GET_MONEY,199);
	CCUserDefault::sharedUserDefault()->flush();
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GameOverController::scene()));
}

void GamePlayController::testViewDelegate(){
	CCLog("testViewDelegate");
}

void GamePlayController::didAccelerate(CCAcceleration* pAccelerationValue ){
	if(!_resinBallModel) return;

	_resinBallModel->accelerateCallBack(pAccelerationValue);
}



void GamePlayController::dealWithTouchesBegan(b2Vec2 locationWorld){
	
if (_mouseJoint!=NULL||!_resinBallModel) return;

b2Body* resinBallBody=_resinBallModel->getBallBody();//获得树脂球模型中的实心圆的body刚体

if (resinBallBody!=NULL&&resinBallBody->GetFixtureList()->TestPoint(locationWorld)) {//如果鼠标点击到实心圆,则创建鼠标关节
	b2MouseJointDef md;
	md.bodyA =_world->GetBodyList();
	md.bodyB =resinBallBody;
	md.target =resinBallBody->GetWorldCenter();
	md.collideConnected =true;
	md.maxForce =1000.0f* resinBallBody->GetMass();
	_mouseJoint = (b2MouseJoint *)_world->CreateJoint(&md);
	resinBallBody->SetAwake(true);
}
}
void GamePlayController::dealWithTouchesMoved(b2Vec2 locationWorld){	
	if (_mouseJoint == NULL) return;

	_mouseJoint->SetTarget(locationWorld);//改变位置
	//_resinBallModel->setPosition(locationWorld.x*PTM_RATIO, locationWorld.y*PTM_RATIO);
	//_resinBallView->update(0);
}
void GamePlayController::dealWithTouchesEnded(){
	if (_mouseJoint) {
		_world->DestroyJoint(_mouseJoint);
		_mouseJoint = NULL;
	}

}



