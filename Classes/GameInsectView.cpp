#include "GameInsectView.h"

using namespace cocos2d;


const int TAG_ANIMATION=12333;//行走动作
const int TAG_ATTACK=133;//攻击动作 
const int TAG_KISS=144;//亲吻动作
const int TAG_INSECTVIEW=155;//
const int TAG_EYE=156;//眼睛

BaseInsectView::BaseInsectView():_insect(NULL),_insectModel(NULL),pViewDelegate(NULL),animationAction(NULL)
{
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	winRect=CCRect(-100,-100,winSize.width+200,winSize.height+200);
}


BaseInsectView::~BaseInsectView()
{
//	CC_SAFE_RELEASE(animation);
	//CC_SAFE_RELEASE(animationAction);
	CC_SAFE_RELEASE(_insect);
	for(unsigned int i=0;i<frameCount;i++){
		//CC_SAFE_FREE(insectFrames[i]);
		//delete []insectFrames[i];
	}
}
CCSprite* BaseInsectView::getSprite(){
	return _insect;
}
void BaseInsectView::cleanFrames(){
	for(unsigned int i=0;i<frameCount;i++){
		//CC_SAFE_FREE(insectFrames[i]);
		delete [](insectFrames[i]);
	}
}

bool BaseInsectView::init(GameViewDelegate* pViewDelegate,Animal* gameInsect){
	bool pRet=false;
	do{ 
		// 先调用超类的init方法
        CC_BREAK_IF(! CCLayer::init());

		_insectModel=gameInsect;
		this->pViewDelegate=pViewDelegate;

		//this->initSprite();
		//this->playAnimation();
		pRet=true;
	}while(0);

	return pRet;
}
void BaseInsectView::initSprite(){
	this->createSpriteByInsectType(_insectModel->getInsectType());
	_insect->retain();
//	this->addChild(_insect,0,TAG_INSECTVIEW);
	this->getParent()->getChildByTag(TAG_BATCH)->addChild(_insect,0,TAG_INSECTVIEW);

	this->initAnimation();
	this->initKiss();
	this->initEyes();
}
void BaseInsectView::playAttack(CCPoint attacker,CCPoint target){
	CCLog("play attack animation");
	CCSprite* attack=(CCSprite*)(_insect->getChildByTag(TAG_ATTACK));
	attack->setVisible(true);

	CCActionInterval*  rotate=CCRotateBy::create(1,360*2);
	CCActionInterval* moveBy=CCMoveBy::create(1,ccpSub(_insect->convertToNodeSpace(target),_insect->convertToNodeSpace(attacker)));
	CCFiniteTimeAction* spawn=CCSpawn::create(rotate,moveBy,NULL);
	CCCallFunc* callfun=CCCallFunc::create(this,callfunc_selector(BaseInsectView::end));
	attack->runAction(CCSequence::create(spawn,callfun,NULL));
}
void BaseInsectView::playKiss(CCPoint source,CCPoint des){
	CCLog("play kiss animation");
	CCSprite* kiss=(CCSprite*)(_insect->getChildByTag(TAG_KISS));
	kiss->setVisible(true);

	CCActionInterval* scaleBy=CCScaleBy::create(1,1);
	CCActionInterval* moveBy=CCMoveBy::create(1,ccpSub(_insect->convertToNodeSpace(source),_insect->convertToNodeSpace(des)));
	CCFiniteTimeAction* spawn=CCSpawn::create(scaleBy,moveBy,NULL);
	CCCallFunc* callfun=CCCallFunc::create(this,callfunc_selector(BaseInsectView::endKiss));
	kiss->runAction(CCSequence::create(spawn,callfun,NULL));
}
void BaseInsectView::initAttack(){
	CCSprite* attack=CCSprite::create("star.png");
	attack->setAnchorPoint(ccp(0.5,0.5));
	attack->setPosition(ccp(_insect->getContentSize().width/2,_insect->getContentSize().height));
	_insect->addChild(attack,0,TAG_ATTACK);
	attack->setVisible(false);
}
void BaseInsectView::initKiss(){
	CCSprite* kiss=CCSprite::createWithSpriteFrameName("love_big.png");
	kiss->setAnchorPoint(ccp(0.5,0.5));
	kiss->setPosition(ccp(_insect->getContentSize().width/2,_insect->getContentSize().height));
	_insect->addChild(kiss,0,TAG_KISS);
	kiss->setVisible(false);
}
void BaseInsectView::endKiss(){
	CCSprite* kiss=(CCSprite*)(_insect->getChildByTag(TAG_KISS));
	kiss->setVisible(false);
	kiss->setScale(0.5f);
	kiss->setPosition(ccp(_insect->getContentSize().width/2,_insect->getContentSize().height));
}
void BaseInsectView::end(){
	CCSprite* attack=(CCSprite*)(_insect->getChildByTag(TAG_ATTACK));
	attack->setVisible(false);
	attack->setPosition(ccp(_insect->getContentSize().width/2,_insect->getContentSize().height));
}
void BaseInsectView::changePlayVel(float speed){
	CCSpeed* speedAction=(CCSpeed*)_insect->getActionByTag(TAG_ANIMATION);
	if(speedAction!=NULL){
		//if(speed==0) speed=0.1f;
		speedAction->setSpeed(speed);
	}
}

void BaseInsectView::update(float dt){
	//CCLog("InsectView:update");
	if(_insect==NULL){
		return;
	}
	_insect->setPosition(_insectModel->getCenterPoint());
	_insect->setRotation(_insectModel->getRotation()+90);

	CCPoint pos=this->convertToWorldSpace(_insect->getPosition());
//	CCLog("pos:%f,%f",pos.x,pos.y);
	if(!winRect.containsPoint(pos)){
		//CCLog("insectVisible:%d",_insect->isVisible());
		if(_insect->isVisible()){
			_insect->setVisible(false);
		//	CCLog("hide");
		}
	}else if(!_insect->isVisible()){
		_insect->setVisible(true);
		//CCLog("show");
	}
	
//	CCLog("%f,%f",_insect->getPosition().x,_insect->getPosition().y);
}
void BaseInsectView::finish(){
	CCLog("remove sprite");
	this->stopAllActions();
	_insect->stopAllActions();
	
	CCCallFunc* callfunc=CCCallFunc::create(this,callfunc_selector(BaseInsectView::endFinish));
	_insect->runAction(CCSequence::create(CCFadeOut::create(1),callfunc,NULL));
	CCArray* plist=_insect->getChildren();
	for(int i=0;i<plist->count();i++){
		CCSprite* p=(CCSprite*)plist->objectAtIndex(i);
		p->runAction(CCFadeOut::create(1));
	}
}
void BaseInsectView::endFinish(){
	this->getParent()->getChildByTag(TAG_BATCH)->removeChild(_insect,true);
}

void BaseInsectView::testModelDelegate(){
	//CCLog("testModelDelegate");
	update(0);
}


void BaseInsectView::initAnimation(){
	animation=CCAnimation::create();
	//char* imageNames[]={"ant_0.png","ant_1.png","ant_2.png","ant_3.png"};
//	char* imageNames[]={"mantis_0.png","mantis_1.png","mantis_2.png","mantis_3.png"};
	for(unsigned int i=0;i<frameCount;i++){
	//	CCLog("i=%d",i);
	//	CCTexture2D *texture=CCTextureCache::sharedTextureCache()->addImage(insectFrames[i]);
	//	CCSpriteFrame *frame=CCSpriteFrame::createWithTexture(texture,
	//		CCRectMake(0,0,texture->getContentSize().width,texture->getContentSize().height));//>getPixelsWide(),texture->getPixelsHigh())); 
		CCSprite* sprite=CCSprite::createWithSpriteFrameName(insectFrames[i]);
		CCSpriteFrame* frame=sprite->displayFrame();
		animation->addSpriteFrame(frame);
	}
	this->cleanFrames();
}
void BaseInsectView::playAnimation(){
	if(animationAction!=NULL){
		this->stopAction(animationAction);
		animationAction=NULL;
	}
	float var=5.0/_insectModel->getVelocity();
	animation->setDelayPerUnit(var);
	animation->setLoops(-1);
	animationAction=CCSpeed::create(CCAnimate::create(animation),1);
	animationAction->setTag(TAG_ANIMATION);
	_insect->runAction(animationAction);
}
void BaseInsectView::createSpriteByInsectType(int type){//根据虫子种类获得相应图片
	this->setInsectType(type);
	char name[10]={0};
	switch(type){
	case ID_MANTIS:
		frameCount=8;
		sprintf(name,"mantis");
		_insect=CCSprite::createWithSpriteFrameName("mantis_0.png");
		break;
	case ID_ENEMY:
		frameCount=9;
		sprintf(name,"uang");
		_insect=CCSprite::createWithSpriteFrameName("uang_0.png");
		break;
	case ID_ANT:
		frameCount=9;
		sprintf(name,"ant");
		_insect=CCSprite::createWithSpriteFrameName("ant_0.png");
		break;
	}

	for(unsigned int i=0;i<frameCount;i++){
		insectFrames[i]=new char[20];
		sprintf(insectFrames[i],"%s_%d.png",name,i);
		CCLog("%s",insectFrames[i]);
	}
}


//
//AntInsectView.cpp
//
AntInsectView::~AntInsectView(){
	this->unschedule(schedule_selector(AntInsectView::blink));
}
void AntInsectView::initSprite(){
	BaseInsectView::initSprite();
	this->initFeeler();
}
void AntInsectView::initFeeler(){//触角
	feelerNormalAnimation=CCAnimation::create();
	feelerCommunicateAnimation=CCAnimation::create();
	char* imageNames[]={"feeler_0.png","feeler_1.png","feeler_2.png","feeler_3.png","feeler_4.png","feeler_5.png","feeler_4.png","feeler_3.png","feeler_2.png","feeler_1.png","feeler_a.png","feeler_b.png"};
	CCSprite* feeler=CCSprite::createWithSpriteFrameName(imageNames[0]);
//	feelerNormalAnimation->addSpriteFrame(feeler->displayFrame());
	feelerCommunicateAnimation->addSpriteFrame(feeler->displayFrame());
	for(int i=1;i<12;i++){
		CCSprite* feel=CCSprite::createWithSpriteFrameName(imageNames[i]);
		if(i<10){
			feelerNormalAnimation->addSpriteFrame(feel->displayFrame());
		}
		if(i>9)
			feelerCommunicateAnimation->addSpriteFrame(feel->displayFrame());

	}
	feeler->setAnchorPoint(ccp(0.5,0.5));
	feeler->setPosition(ccp(_insect->getContentSize().width/2,_insect->getContentSize().height*6.5f/7.5f));
	_insect->addChild(feeler);

	feelerNormalAnimation->setDelayPerUnit(10.0/_insectModel->getVelocity());
	feelerNormalAnimation->setLoops(-1);
	feeler->runAction(CCAnimate::create(feelerNormalAnimation));
}
void AntInsectView::initEyes(){
	CCSprite* eye=CCSprite::createWithSpriteFrameName("eye_mid.png");
	eye->setAnchorPoint(ccp(0.5,0.5));
	eye->setPosition(ccp(_insect->getContentSize().width/2,_insect->getContentSize().height*5.6f/7.9f));
	_insect->addChild(eye,0,TAG_EYE);

	this->schedule(schedule_selector(AntInsectView::blink),5);//5秒钟一次
}
void AntInsectView::blink(float dt){//眨眼
	if(CCRANDOM_0_1()<0.5f) return;

	CCSprite* eye=(CCSprite*)_insect->getChildByTag(TAG_EYE);

	CCAnimation* animation=CCAnimation::create();

	int id=((int)(CCRANDOM_0_1()*10))%5;//随机一个帧
	
	char* images[]={"eye_mid.png","eye_left.png","eye_right.png","eye_down.png","eye_up.png"};
	animation->addSpriteFrame(CCSprite::createWithSpriteFrameName(images[id])->displayFrame());
	animation->addSpriteFrame(eye->displayFrame());

	animation->setDelayPerUnit(0.8f);
	animation->setLoops(2);

	eye->runAction(CCAnimate::create(animation));
}

//
//MantisInsectView.cpp
//

MantisInsectView::~MantisInsectView(){
	CCLog("~MantisInsectView");
}
void MantisInsectView::initSprite(){
	BaseInsectView::initSprite();
	this->initFeeler();
	this->initAttack();
}

void MantisInsectView::initFeeler(){//触角
	feelerNormalAnimation=CCAnimation::create();
	char* imageNames[]={"hand_down1.png","hand_down2.png","hand_down3.png","hand_mid.png","hand_up1.png","hand_up2.png","hand_up3.png"};
	CCSprite* feeler=CCSprite::createWithSpriteFrameName(imageNames[0]);
	feelerNormalAnimation->addSpriteFrame(feeler->displayFrame());
	for(int i=1;i<7;i++){
		CCSprite* feel=CCSprite::createWithSpriteFrameName(imageNames[i]);
		feelerNormalAnimation->addSpriteFrame(feel->displayFrame());
	}
	feeler->setAnchorPoint(ccp(0.5,0.5));
	feeler->setPosition(ccp(_insect->getContentSize().width/2,_insect->getContentSize().height*12.f/15.f));
	_insect->addChild(feeler);

	feelerNormalAnimation->setDelayPerUnit(10.0/_insectModel->getVelocity());
	feelerNormalAnimation->setLoops(-1);
	feeler->runAction(CCAnimate::create(feelerNormalAnimation));
}

void MantisInsectView::initEyes(){
	CCSprite* eye=CCSprite::createWithSpriteFrameName("mantis_eye.png");
	eye->setAnchorPoint(ccp(0.5,0.5));
	eye->setPosition(ccp(_insect->getContentSize().width/2,_insect->getContentSize().height*13.f/15.f));
	_insect->addChild(eye,0,TAG_EYE);

//	this->schedule(schedule_selector(AntInsectView::blink),5);//5秒钟一次
}
void MantisInsectView::blink(float dt){//眨眼
	if(CCRANDOM_0_1()<0.5f) return;

	CCSprite* eye=(CCSprite*)_insect->getChildByTag(TAG_EYE);

	CCAnimation* animation=CCAnimation::create();

	int id=((int)(CCRANDOM_0_1()*10))%5;//随机一个帧
	
	char* images[]={"eye_mid.png","eye_left.png","eye_right.png","eye_down.png","eye_up.png"};
	animation->addSpriteFrame(CCSprite::createWithSpriteFrameName(images[id])->displayFrame());
	animation->addSpriteFrame(eye->displayFrame());

	animation->setDelayPerUnit(0.8f);
	animation->setLoops(2);

	eye->runAction(CCAnimate::create(animation));
}




//
//enemyView.cpp
//


EnemyView::~EnemyView()
{
	this->unschedule(schedule_selector(EnemyView::blink));
}

void EnemyView::initSprite(){
	BaseInsectView::initSprite();
}

void EnemyView::update(float dt){
	//CCLog("InsectView:update");
	if(_insect==NULL){
		return;
	}

	_insect->setPosition(_insectModel->getCenterPoint());
	_insect->setRotation(_insectModel->getRotation()+90);
	if(_insectModel->isOutOfBlood()){
		this->stopAllActions();
		_insect->stopAllActions();
		this->removeChild(_insect,true);
	}
//	CCLog("%f,%f",_insect->getPosition().x,_insect->getPosition().y);
}
void EnemyView::initEyes(){
	CCSprite* eye=CCSprite::createWithSpriteFrameName("eye_mid.png");
	eye->setAnchorPoint(ccp(0.5,0.5));
	eye->setPosition(ccp(_insect->getContentSize().width/2,_insect->getContentSize().height*16.5f/26.f));
	_insect->addChild(eye,0,TAG_EYE);

	this->schedule(schedule_selector(EnemyView::blink),5);//5秒钟一次
}
void EnemyView::blink(float dt){//眨眼
	if(CCRANDOM_0_1()<0.5f) return;

	CCSprite* eye=(CCSprite*)_insect->getChildByTag(TAG_EYE);

	CCAnimation* animation=CCAnimation::create();

	int id=((int)(CCRANDOM_0_1()*10))%5;//随机一个帧
	
	char* images[]={"eye_mid.png","eye_left.png","eye_right.png","eye_down.png","eye_up.png"};
	animation->addSpriteFrame(CCSprite::createWithSpriteFrameName(images[id])->displayFrame());
	animation->addSpriteFrame(eye->displayFrame());

	animation->setDelayPerUnit(0.8f);
	animation->setLoops(2);

	eye->runAction(CCAnimate::create(animation));
}