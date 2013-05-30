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
#include "GlobalComponent.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)   
#include "vld.h"   
#endif 

USING_NS_CC;

const int INCREMENT=1;//ÿ��һ��Ҫ���������Ŀ����
const int BASIC_AMBER_NUM=1;//��ʼ����Ҫ��������Ŀ

//���ݳ�����Ϊ��ȡ����id
 int getAmberIdFromActionType(int actionType){
	int amberId;
	switch(actionType){
	case ID_ACTION_NORMAL:amberId=0;break;
	case ID_ACTION_GROPE:amberId=1;break;
	case ID_ACTION_FOLLOW:amberId=2;break;
	case ID_ACTION_COMMUNICATE:
	case ID_ACTION_COMMUNICATED:
		amberId=3;break;
	case ID_ACTION_ATTACK:amberId=4;break;
	case ID_ACTION_ESCAPE:amberId=5;break;
	default:amberId=6;
	}
	return amberId;
}

GamePlayController::GamePlayController():_world(NULL)
	,_resinBallModel(NULL),_resinBallView(NULL),_mouseJoint(NULL),amberCount(0)
{
}


GamePlayController::~GamePlayController()
{	
	CC_SAFE_RELEASE_NULL(_resinBallView);
	CC_SAFE_RELEASE_NULL(_resinBallModel);
	CC_SAFE_RELEASE_NULL(buttonLayer);
	CC_SAFE_RELEASE_NULL(mapLayer);
	this->clean();
	_enemys.clear();
	_insects.clear();

	CC_SAFE_DELETE(contactListener);
	MessageVector::sharedMessages().clear();
	delete _world;
	_world=NULL;
	this->removeAllChildrenWithCleanup(true);
}

void GamePlayController::initResinBall(){//��ʼ����ͼ����ģ��
	_resinBallModel=GameResinBallModel::create(_world);
	_resinBallModel->retain();
	addChild(_resinBallModel,100);
	
	_resinBallView=GameResinBallView::create(this,_resinBallModel);
	_resinBallView->retain();

	addChild(_resinBallView,-1);

	_resinBallModel->initObserver(_resinBallView);
}
void GamePlayController::addEnemyInsect(){
	EnemyInsect* enemy=EnemyInsect::create(_world,_resinBallModel);
	this->addChild(enemy,0);

	EnemyView* view=EnemyView::create(this,enemy);
	this->addChild(view,0);
	view->initSprite();
	view->playAnimation();

	enemy->initObserver(view);
	_enemys.push_back(enemy);
}
void GamePlayController::addInsect(){
	BaseInsect* insectm=MantisInsect::create(_world);
			addChild(insectm,0);
			BaseInsectView* insectv=MantisInsectView::create(this,insectm);
			addChild(insectv,0);
			insectv->initSprite();
			insectv->playAnimation();

			insectm->initObserver(insectv);
			_insects.push_back(insectm);
}
void GamePlayController::addAnt(){
	BaseInsect* insectm=AntInsect::create(_world);
			addChild(insectm,0);
			BaseInsectView* insectv=AntInsectView::create(this,insectm);
			addChild(insectv,0);
			insectv->initSprite();
			insectv->playAnimation();

			insectm->initObserver(insectv);
			_insects.push_back(insectm);
}
void GamePlayController::initInsect(int num){
	for(int i=0;i<num;i++){
			this->addInsect();
		}
}
void GamePlayController::initEnemyInsect(int num){
	for(int i=0;i<num;i++){
		this->addEnemyInsect();
	}
}
void GamePlayController::initAnts(int num){
	for(int i=0;i<num;i++){
		this->addAnt();
	}
}
std::vector<Amber*> GamePlayController::getNewAmbers(){
	return newAmbers;
}

bool GamePlayController::init(){
	bool pRet = false;

    do{
        // �ȵ��ó����init����
		CC_BREAK_IF(! CCLayer::init());

		srand(time(NULL));//���������������
		_world=B2EasyBox2D::createWorld(b2Vec2(0,0));
		B2DebugDrawLayer *dDraw=B2DebugDrawLayer::create(_world,PTM_RATIO);
		//addChild(dDraw,9999);

		contactListener=new myContactListener();
		_world->SetContactListener(contactListener);//��ײ������	
		this->setAccelerometerEnabled(true);//��������������Ӧ

		int consumeResin=CCUserDefault::sharedUserDefault()->getIntegerForKey(CONSUME_RESIN,10);
		int totalResin=CCUserDefault::sharedUserDefault()->getIntegerForKey(RESIN_VOLUME,0);
		CCUserDefault::sharedUserDefault()->setIntegerForKey(RESIN_VOLUME,totalResin-consumeResin);
		CCUserDefault::sharedUserDefault()->flush();
	
		this->initResinBall();//��ʼ����֬��ģ�ͼ���ͼ

		this->createWrapWall();//��������ǽ��һЩ��̬�ϰ�
		this->createEnvir();//��������

		//��ʼ����ť��
		buttonLayer=StaticLayer::create();
		buttonLayer->retain();
		buttonLayer->initControlSlider(_resinBallModel);//�����ã��ֻ���ͨ��������������֬���������Ħ��ϵ��
		addChild(buttonLayer,10);

		//��ʼ����ͼ��
		mapLayer=MapLayer::create();
		mapLayer->retain();
		addChild(mapLayer,-10);

		buttonLayer->innerStage=mapLayer->innerStage;//������


        pRet = true;
    }while(0);
    return pRet;
}
void GamePlayController::test(){
	/*MantisInsect* mantis=MantisInsect::create(_world);
	mantis->setPosition(ccp(100,100));
	addChild(mantis,0);
	BaseInsectView* mantisv=BaseInsectView::create(this,mantis);
	addChild(mantisv,0);
	mantis->initObserver(mantisv);
	_insects.push_back(mantis);
	*/
	AntInsect* ant=AntInsect::create(_world);
	ant->setPosition(ccp(500,500));
	addChild(ant,0);
	BaseInsectView* antv=BaseInsectView::create(this,ant);
	addChild(antv,0);
	ant->initObserver(antv);
	_insects.push_back(ant);

	AntInsect* ant2=AntInsect::create(_world);
	ant2->setPosition(ccp(800,700));
	addChild(ant2,0);
	BaseInsectView* antv2=BaseInsectView::create(this,ant2);
	addChild(antv2,0);
	ant2->initObserver(antv2);
	_insects.push_back(ant2);
	ant->stopMove();
	ant2->stopMove();
	ant->setKissEnable(true);
	ant2->setKissEnable(true);
//	ant->setRotation(0);
//	ant->getMoveForKey(ID_HEAD_ROTATETO_MOVE)->setDuration(5);
//	((HeadRotateToMove*)ant->getMoveForKey(ID_HEAD_ROTATETO_MOVE))->setAngle(190);
	ant2->setRushZone(ID_RIGHT_BODY);
	//	ant->startMove(ID_HEAD_ROTATETO_MOVE);
	ant->replaceAction(ID_ACTION_COMMUNICATE,ant2);//����
	ant2->replaceAction(ID_ACTION_COMMUNICATED,ant);//������
	//ant->replaceActionIfLevelUp(ID_ACTION_ESCAPE);//����
	//mantis->replaceAction(ID_ACTION_ATTACK,ant);//����
}
void GamePlayController::onEnter(){
	this->setKeypadEnabled(true);
	CCLayer::onEnter();
}
void GamePlayController::onEnterTransitionDidFinish(){
	//��ʼ��Ϸ
	this->initGameData();//�����ڰ�ť��֮���ʼ��
	this->schedule(schedule_selector(GamePlayController::update));
	//this->schedule(schedule_selector(GamePlayController::updateLevelIfReached),10);
	CCLayer::onEnterTransitionDidFinish();
}
void GamePlayController::onExitTransitionDidStart(){
	CCLayer::onExitTransitionDidStart();
}
void GamePlayController::onExit(){
	CCLayer::onExit();
	//this->unschedule(schedule_selector(GamePlayController::updateLevelIfReached));
}



void GamePlayController::createEnvir(){
	CCSprite* sp=CCSprite::create("petri_dish.png");
	sp->setPosition(ccp(900,300));
	this->addChild(sp,0,TAG_WALL);
	b2Body* petri_dish=B2EasyBox2D::createCircle(_world,900,300,140,sp);
	b2Fixture* fixture=petri_dish->GetFixtureList();
	fixture->SetDensity(1000);
	fixture->SetFriction(1);
	fixture->SetRestitution(0.2f);
	fixture->SetUserData(sp);


}
 void GamePlayController::createWrapWall(){
	 CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	 winSize=CCSizeMake(winSize.width/PTM_RATIO,winSize.height/PTM_RATIO);
	 b2Vec2 leftTop=b2Vec2(-winSize.width*(MAP_SCALE-1)*0.5f,winSize.height+(MAP_SCALE-1)*0.5f*winSize.height);
	 b2Vec2 leftBottom=b2Vec2(leftTop.x,-(MAP_SCALE-1)*0.5f*winSize.height);
	 b2Vec2 rightTop=b2Vec2(winSize.width+winSize.width*(MAP_SCALE-1)*0.5f,leftTop.y);
	 b2Vec2 rightBottom=b2Vec2(rightTop.x,leftBottom.y);
	// winSize=CCSizeMake(winSize.width*PTM_RATIO,winSize.height*PTM_RATIO);
	// mapArea=CCRect(leftBottom.x,leftBottom.y,winSize.width*MAP_SCALE,winSize.height*MAP_SCALE);

	 CCNode* userDataWall=CCNode::create();
	 userDataWall->setTag(TAG_WALL);
	 this->addChild(userDataWall);

//	 b2Filter filter;
//	 filter.groupIndex=k_wallGroup;
//	 filter.categoryBits=k_wallCategory;
//	 filter.maskBits=k_wallMask;

	 b2BodyDef bodyDef;
	 bodyDef.position.Set(0,0);
	 bodyDef.userData=userDataWall;
	 bodyDef.type=b2_staticBody;
	 b2Body* wall=_world->CreateBody(&bodyDef);

	 b2EdgeShape edgeShape; 
	 edgeShape.Set(leftBottom,rightBottom);  //��
	 
	 b2FixtureDef fixtureDef;
//	 fixtureDef.filter=filter;
	 fixtureDef.shape = &edgeShape;
	 fixtureDef.userData=userDataWall;
	 wall->CreateFixture(&fixtureDef);
	  
	 //��
	 edgeShape.Set(leftTop,rightTop);
	 wall->CreateFixture(&fixtureDef);
	 //��
	 edgeShape.Set(leftTop,leftBottom);
	 wall->CreateFixture(&fixtureDef);
	 //��
	 edgeShape.Set(rightTop,rightBottom);
	 wall->CreateFixture(&fixtureDef);

	 b2CircleShape shape;
	 shape.m_radius=1;

	 b2FixtureDef circleFix;
	 circleFix.userData=userDataWall;
	 circleFix.shape=&shape;

	 shape.m_p.Set(leftTop.x,leftTop.y);
	 wall->CreateFixture(&circleFix);
	 
	 shape.m_p.Set(leftBottom.x,leftBottom.y);
	 wall->CreateFixture(&circleFix);
	 
	 shape.m_p.Set(rightTop.x,rightTop.y);
	 wall->CreateFixture(&circleFix);
	 
	 shape.m_p.Set(rightBottom.x,rightBottom.y);
	 wall->CreateFixture(&circleFix);
	 //wall->SetBullet(true);
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
	
	//CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	/*ccDrawSolidRect(ccp(0,0),ccp(PTM_RATIO/2,winSize.height),ccc4f(0,255,0,255));
	ccDrawSolidRect(ccp(0,0),ccp(winSize.width,PTM_RATIO/2),ccc4f(0,255,0,255));
	ccDrawSolidRect(ccp(winSize.width-PTM_RATIO/2,0),ccp(winSize.width,winSize.height),ccc4f(0,255,0,255));
	ccDrawSolidRect(ccp(0,winSize.height-PTM_RATIO/2),ccp(winSize.width,winSize.height),ccc4f(0,255,0,255));
	*/
	// _resinBallModel->myDraw();
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

void GamePlayController::playInsectDisappear(CCSprite* insect){//������ʧ
	CCSprite* sprite=CCSprite::createWithSpriteFrame(insect->displayFrame());
	sprite->setAnchorPoint(insect->getAnchorPoint());
	sprite->setPosition(insect->getPosition());
	sprite->setRotation(insect->getRotation());
	this->addChild(sprite);

	sprite->runAction(CCFadeOut::create(1));//1������ʧ
}//����
void GamePlayController::intFreezeBox(){
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	CCSprite* freezeBox=CCSprite::createWithSpriteFrameName("slot.png");
	freezeBox->setPosition(ccp(winSize.width/2,freezeBox->getContentSize().height/2));
	this->addChild(freezeBox);

	CCMenuItem* freezeItem=GlobalComponent::createMenuItem(MENU_FREEZE,this,menu_selector(GamePlayController::menuFreezeCallback));

	//freezeItem->setPosition(ccp(winSize.width/2,freezeItem->getContentSize().height/2));
	freezeItem->setPosition(ccp(winSize.width/2,freezeBox->getContentSize().height/2));

	CCMenu* menu=CCMenu::create(freezeItem,NULL);
	menu->setAnchorPoint(ccp(0,0));
	menu->setPosition(ccp(0,0));
	this->addChild(menu);
}
void GamePlayController::menuFreezeCallback(CCObject* pSender){
	this->playFreeze();
}

void GamePlayController::playFreeze(){//����
	this->setFreezing(true);
	CCLog("freeze:%d",_insects.size());
	for(unsigned int i=0;i<_insects.size();i++){
		_insects[i]->setBounding(true);
	}
	for(unsigned int i=0;i<_enemys.size();i++){
		_enemys[i]->setBounding(true);
	}
	CCCallFunc *callfunc=CCCallFunc::create(this,callfunc_selector(GamePlayController::endFreeze));
	this->runAction(CCSequence::create(CCDelayTime::create(3),callfunc,NULL));//��������
}
void GamePlayController::endFreeze(){//��������
	this->setFreezing(false);
	for(unsigned int i=0;i<_insects.size();i++){
		_insects[i]->setBounding(false);
	}
	for(unsigned int i=0;i<_enemys.size();i++){
		_enemys[i]->setBounding(false);
	}
}
void GamePlayController::initGameData(){//��ʼ����Ϸ����
	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("insects.plist","insects.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ants.plist","ants.png");
	batch=CCSpriteBatchNode::create("ants.png",100);
	this->addChild(batch,0,TAG_BATCH);

	/*for(int i=0;i<1000;i++){
		CCSprite* sp=CCSprite::createWithSpriteFrameName("ant_0.png");
		sp->setPosition(ccp(CCRANDOM_0_1()*1280,CCRANDOM_0_1()*720));
		//batch->addChild(sp,0);
		this->addChild(sp,0);
	}*/

	//	this->initInsect(5);
	//	this->initAnts(10);
	//	this->initInsect(3);
		this->initAnts(15);
	//��ʼ������
	//	this->initEnemyInsect(1);
	//this->test();
		nextLevelAmberNum=BASIC_AMBER_NUM;
		cur_level=1;
		this->buttonLayer->playLevelUp(cur_level);

		//���³ɾ��б��ÿ�
		newAmbers.clear();
		this->setFreezing(false);//�Ƿ��ڱ����У�
		this->setTimeCount(0);
	//	this->intFreezeBox();
}
void GamePlayController::updateLevelIfReached(float dt){
	/*this->setTimeCount(this->getTimeCount()+dt);
	if(this->getTimeCount()<10) return;
	this->setTimeCount(0);

	this->initEnemyInsect(1);*/
	if(amberCount<nextLevelAmberNum) return;

	unsigned int newInsectNum=nextLevelAmberNum+1;//ÿ��һ����������+1
	nextLevelAmberNum+=newInsectNum;//��һ������Ҫ������������
	cur_level++;
	
	this->buttonLayer->playLevelUp(cur_level);//������������

	//�������һֻ���˳���
	if(CCRANDOM_0_1()>0.5f){
		this->initEnemyInsect(1);
	}
	//�������Ϻ��������
	unsigned int mantisNum=newInsectNum/3;
	unsigned int antNum=newInsectNum-mantisNum;
	this->initAnts(antNum);
	this->initInsect(mantisNum);
}

void GamePlayController::checkGameOverAndDo(){//����Ƿ������Ϸ
	if(_resinBallModel->usedUp()||_insects.size()==0){
		//CCDelayTime* delay=CCDelayTime::create(1);
		
		this->menuGameOverCallback(this);
	}
}
void GamePlayController::updateInsectNumber(){
	if(_insects.size()>MIN_NUM) return;
	if(CCRANDOM_0_1()>0.8){//0.8�ĸ���
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
	for(unsigned int i=0;i<_enemys.size();i++){
		_enemys[i]->update(dt);
	}

	//���µ�ͼ����
	CCScene* scene=CCDirector::sharedDirector()->getRunningScene();
	CCPoint last=scene->getPosition();
	mapLayer->updateFloor(_resinBallModel->getDeltaPosition());
	mapLayer->updateMap(_resinBallModel->getPosition());
	//���ְ�ťλ�ò���
	buttonLayer->keepStill(last);
//	buttonLayer->setResinCount(_resinBallModel);
	
	MessageVector::handleMessage();
	//MessageView::handleMessage();
	
	this->deleteAmberedInsect();//ɾ���Ѿ����������ĳ���
	//this->updateLevelIfReached();
	
	this->checkGameOverAndDo();
//	this->updateLevelIfReached(dt);

}
void GamePlayController::step(float dt){//box2d����ʱ�䲽 �� cocos2d-x����Ƶ�� ��һ����Ӧ
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
		//CCLog("dt:%f",dt);
	}
	this->afterStep(dt); // process collisions and result from callbacks called by the step
	_world->ClearForces();
	
}
//ÿһ��ʱ�䲽����Ҫִ�еĲ���
void GamePlayController::afterStep(float dt){
	vector<BaseInsect* >::iterator it = _insects.begin(); 
	while (it != _insects.end()) { 
		BaseInsect* insect=(BaseInsect*)*it; 
		if (insect->isOutOfGas()) { 
			insect->clean();
			_amberInsects.push_back(insect);//����Ҫ���б�(�γ�����)
			_deleteBodys.push_back(insect->getBody());//����ݻ��б�
			ambersId.insert(getAmberIdFromActionType(insect->getActionType()));//���뵽����id����
			it = _insects.erase(it);    
			CCLog("after delete insect No.:",_insects.size());
			amberCount++;
		} else {	
			it++;    
		}
	}
	_resinBallModel->coveringInsect(dt);//�����ӿ�Ѫ
}
void GamePlayController::deleteAmberedInsect(){
	for(int i=0;i<_deleteBodys.size();i++){
		if(_deleteBodys[i]!=NULL){
			_world->DestroyBody(_deleteBodys[i]);
			_deleteBodys[i]=NULL;
		}
	}
	_deleteBodys.clear();
	vector<BaseInsect* >::iterator it = _amberInsects.begin(); 
	while (it != _amberInsects.end()) { 
		BaseInsect* insect=(BaseInsect*)*it;
		this->removeChild(insect,true);
		it = _amberInsects.erase(it);
		insect=NULL;
	}
}

void GamePlayController::clean(){//�˳��˳���ǰ������
	this->unschedule(schedule_selector(GamePlayController::update));

	vector<BaseInsect* >::iterator it = _insects.begin(); 
	while (it != _insects.end()) { 
		BaseInsect* insect=(BaseInsect*)*it;
		//����body
		b2Body* body=insect->getBody();
		_world->DestroyBody(body);
		body=NULL;
		
		//
		insect->clean();
		this->removeChild(insect,true);
		it = _insects.erase(it);
		insect=NULL;
	}
	//������˶���
	vector<EnemyInsect* >::iterator itEnemy = _enemys.begin(); 
	while (itEnemy != _enemys.end()) { 
		EnemyInsect* enemy=(EnemyInsect*)*itEnemy;
		//����body
		b2Body* body=enemy->getBody();
		_world->DestroyBody(body);
		body=NULL;
		//
		enemy->clean();
		this->removeChild(enemy,true);
		itEnemy = _enemys.erase(itEnemy);
		enemy=NULL;
	}
}
void GamePlayController::shrinkCallBack(CCObject* pSender){
	_resinBallModel->shrinkResinBallBody(0.99f);
}
void GamePlayController::menuGoBackCallback(CCObject* pSender){
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GameMenuController::scene()));
}
void GamePlayController::menuGameOverCallback(CCObject* pSender){
	this->buttonLayer->playGameOver();
	this->unschedule(schedule_selector(GamePlayController::update));
	CCUserDefault::sharedUserDefault()->setIntegerForKey(GET_AMBER,amberCount);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(GET_SCORE,amberCount*100+CCRANDOM_0_1()*10);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(GET_MONEY,amberCount*10+CCRANDOM_0_1()*10);

	//����Ƿ����³ɾͣ������꣩�������򼤻�
	std::set<int>::iterator it=ambersId.begin();
	while(it!=ambersId.end()){
		int id=*it;
		if(!Amber::getEnableForKey(id)){//��δ��ô˳ɾ�
			Amber::writeEnableForKey(id,true);
			std::vector<Amber*> ambers=Amber::getAmbers();
			newAmbers.push_back(ambers[id]);
		}
		it++;
	}

	CCUserDefault::sharedUserDefault()->flush();
//	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GameOverController::scene()));
}

void GamePlayController::testViewDelegate(){
	CCLog("testViewDelegate");
}

void GamePlayController::didAccelerate(CCAcceleration* pAccelerationValue ){
	if(!_resinBallModel) return;

	_resinBallModel->accelerateCallBack(pAccelerationValue);
}

void GamePlayController::keyBackClicked(){
	CCLog("key back clicked");
	this->menuGoBackCallback(this);
}

void GamePlayController::dealWithTouchesBegan(b2Vec2 locationWorld){
	
if (_mouseJoint!=NULL||!_resinBallModel) return;

b2Body* resinBallBody=_resinBallModel->getBallBody();//�����֬��ģ���е�ʵ��Բ��body����

if (resinBallBody!=NULL&&resinBallBody->GetFixtureList()->TestPoint(locationWorld)) {//����������ʵ��Բ,�򴴽����ؽ�
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

	_mouseJoint->SetTarget(locationWorld);//�ı�λ��
	//_resinBallModel->setPosition(locationWorld.x*PTM_RATIO, locationWorld.y*PTM_RATIO);
	//_resinBallView->update(0);
}
void GamePlayController::dealWithTouchesEnded(){
	if (_mouseJoint) {
		_world->DestroyJoint(_mouseJoint);
		_mouseJoint = NULL;
	}

}



