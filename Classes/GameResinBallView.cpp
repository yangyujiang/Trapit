#include "GameResinBallView.h"
#include "VisibleRect.h"
#include "SmartRes.h"

USING_NS_CC;

//ȫ�ֺ���
b2Vec2 getLocationWorld(CCSet *touches){//��������������Box2D���������(�����)
	CCTouch *myTouch =(CCTouch*) touches->anyObject();
	CCPoint location = myTouch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	CCScene* scene=CCDirector::sharedDirector()->getRunningScene();
//	CCLog("touch:%f,%f",location.x,location.y);
//	CCLog("scene:%f,%f",scene->getPosition().x,scene->getPosition().y);
	location=ccpSub(location,scene->getPosition());
	b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);

	return locationWorld;
}

//GameReainBallView.cpp
//
GameResinBallView::GameResinBallView():_ResinBall(NULL),_ResinBallModel(NULL),pViewDelegate(NULL)
{
}


GameResinBallView::~GameResinBallView()
{

}

bool GameResinBallView::init(GameResinBallViewDelegate* pViewDelegate,GameResinBallModel* gameResinBall){
	bool pRet=false;
	do{ 
		// �ȵ��ó����init����
        CC_BREAK_IF(! CCLayer::init());
		
		_ResinBallModel=gameResinBall;
		this->pViewDelegate=pViewDelegate;

		_ResinBall=CCSprite::create("amber.png");
		_ResinBall->setScale(_ResinBallModel->getBallRadius()/_ResinBall->getContentSize().width);
		_ResinBall->retain();
		addChild(_ResinBall,2);
		_ResinBall->setPosition(_ResinBallModel->getPosition());

		this->setTouchEnabled(true);

		pRet=true;
	}while(0);

	return pRet;
}
void GameResinBallView::onEnter(){
	CCLayer::onEnter();	
	
	streak = CCMotionStreak::create(2, 3, _ResinBallModel->getRadius()*1.5, ccWHITE, "yellow.png");
	//streak->setScale(0.5);
	addChild(streak,5);
	CCActionInterval *colorAction = CCRepeatForever::create((CCActionInterval *)CCSequence::create(
        CCTintTo::create(0.2f, 255, 0, 0),
        CCTintTo::create(0.2f, 0, 255, 0),
        CCTintTo::create(0.2f, 0, 0, 255),
        CCTintTo::create(0.2f, 0, 255, 255),
        CCTintTo::create(0.2f, 255, 255, 0),
        CCTintTo::create(0.2f, 255, 0, 255),
        CCTintTo::create(0.2f, 255, 255, 255),
        NULL));

    //streak->runAction(colorAction);
}
void GameResinBallView::onExit(){
	CCLayer::onExit();
}

void GameResinBallView::update(float dt){
	//CCLog("ResinBallView:update");
	if(_ResinBall==NULL){
		return;
	}
	
	streak->setPosition(_ResinBallModel->getPosition());//���ø���Ŀ�����
	_ResinBall->setPosition(_ResinBallModel->getPosition());
}

void GameResinBallView::testModelDelegate(){
	//CCLog("testModelDelegate");
	update(0);
}


void GameResinBallView::ccTouchesBegan(CCSet *touches,CCEvent *event){	
	//����������λ�ò�����ת��ΪBox2D���������
	b2Vec2 locationWorld =getLocationWorld(touches);

	pViewDelegate->dealWithTouchesBegan(locationWorld);
}
 

void GameResinBallView::ccTouchesMoved(CCSet *touches,CCEvent *event) {
	//����������λ�ò�����ת��ΪBox2D���������
	b2Vec2 locationWorld =getLocationWorld(touches);

	pViewDelegate->dealWithTouchesMoved(locationWorld);
}

void GameResinBallView::ccTouchesCancelled(CCSet *touches, CCEvent *event) {
	pViewDelegate->dealWithTouchesEnded();
}

void GameResinBallView::ccTouchesEnded(CCSet *touches, CCEvent *event) {
	pViewDelegate->dealWithTouchesEnded();
}


//
//MapLayer.cpp
//
MapLayer::MapLayer()
{}
MapLayer::~MapLayer(){};

bool MapLayer::init(){
	bool pRet=false;
	do{ 
		// �ȵ��ó����init����
        CC_BREAK_IF(! CCLayer::init());

		_map=CCSprite::create("playBg.png");
	//	CCLog("factor:%f",CCDirector::sharedDirector()->getContentScaleFactor());
		_map->setScale(CCDirector::sharedDirector()->getContentScaleFactor());
		CC_BREAK_IF(!_map);
		addChild(_map,-1);

		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		float width=50;
		innerStage.left=winSize.width/2-width;
		innerStage.right=winSize.width/2+width;
		innerStage.top=winSize.height/2+width;
		innerStage.bottom=winSize.height/2-width;
		
		_map->setPosition(ccp(winSize.width/2,winSize.height/2));

		float deltaX=winSize.width*(WORLD_SCALE-1)/2;
		float deltaY=winSize.height*(WORLD_SCALE-1)/2;

		//�ذ�
		_floor[0]=CCSprite::create("floor1.png");
		//_floor[0]->setAnchorPoint(ccp(0,0));
		_floor[0]->setPosition(ccp(winSize.width/2,-deltaY));//��
		_floor[1]=CCSprite::create("floor1.png");
		//_floor[1]->setAnchorPoint(ccp(0,1));
		_floor[1]->setPosition(ccp(winSize.width/2,winSize.height+deltaY));//��
		_floor[2]=CCSprite::create("floor2.png");
		//_floor[2]->setAnchorPoint(ccp(1,0));
		_floor[2]->setPosition(ccp(-deltaX,winSize.height/2));//��
		_floor[3]=CCSprite::create("floor2.png");
		//_floor[3]->setAnchorPoint(ccp(1,1));
		_floor[3]->setPosition(ccp(deltaX+winSize.width,winSize.height/2));//��

		addChild(_floor[0],-10);
		addChild(_floor[1],-10);
		addChild(_floor[2],-10);
		addChild(_floor[3],-10);

		pRet=true;
	}while(0);

	return pRet;
}

void MapLayer::updateFloor(CCPoint delta){
	delta=ccpMult(delta,-scaleVToMap);
//	CCLog("%f,%f",delta.x,delta.y);
	_floor[0]->setPosition(ccp(_floor[0]->getPositionX()-delta.x,_floor[0]->getPositionY()-delta.y));
	_floor[1]->setPosition(ccp(_floor[1]->getPositionX()-delta.x,_floor[1]->getPositionY()-delta.y));
	//_floor[2]->setPosition(ccp(_floor[2]->getPositionX()-delta.x,_floor[2]->getPositionY()-delta.y));
	//_floor[3]->setPosition(ccp(_floor[3]->getPositionX()-delta.x,_floor[3]->getPositionY()-delta.y));
}

void MapLayer::updateMap(CCPoint nextPosition){
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	CCScene* scene=CCDirector::sharedDirector()->getRunningScene();//��õ�ǰ����
	CCPoint locPosition=ccpAdd(scene->getPosition(),nextPosition);//�����֬������ڳ���������
	CCPoint newPosition=scene->getPosition();//������������ͼ����һ֡������

	if(locPosition.y>innerStage.top){//�����ڱ߽���Ͻ�
		float delta=innerStage.top-locPosition.y;
		newPosition.y+=delta;
		if(newPosition.y<=(winSize.height-winSize.height*WORLD_SCALE)*0.5f){
			newPosition.y=(winSize.height-winSize.height*WORLD_SCALE)*0.5f;
		}
	}else if(locPosition.y<innerStage.bottom){//�����ڱ߽���½�
		float delta=innerStage.bottom-locPosition.y;
		newPosition.y+=delta;

		if(newPosition.y>=(winSize.height*WORLD_SCALE-winSize.height)*0.5f){
			newPosition.y=(winSize.height*WORLD_SCALE-winSize.height)*0.5f;
		}
	}

	if(locPosition.x<innerStage.left){//�����ڱ߽���Ͻ�
		newPosition.x+=innerStage.left-locPosition.x;
		if(newPosition.x>=(winSize.width*WORLD_SCALE-winSize.width)*0.5f){
			newPosition.x=(winSize.width*WORLD_SCALE-winSize.width)*0.5f;
		}
		
	}else if(locPosition.x>innerStage.right){//�����ڱ߽���½�
		newPosition.x+=innerStage.right-locPosition.x;
		if(newPosition.x<=(winSize.width-winSize.width*WORLD_SCALE)*0.5f){
			newPosition.x=(winSize.width-winSize.width*WORLD_SCALE)*0.5f;
		}
	}
	//CCLog("%f,%f",newPosition.x,newPosition.y);

	scene->setPosition(newPosition);
}