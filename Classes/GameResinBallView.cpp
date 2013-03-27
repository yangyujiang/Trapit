#include "GameResinBallView.h"
#include "Constant.h"

USING_NS_CC;

//全局函数
b2Vec2 getLocationWorld(CCSet *touches){//根据鼠标点击获得在Box2D世界的坐标(点击点)
	CCTouch *myTouch =(CCTouch*) touches->anyObject();
	CCPoint location = myTouch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);

	return locationWorld;
}

GameResinBallView::GameResinBallView():_ResinBall(NULL),_ResinBallModel(NULL),pViewDelegate(NULL)
{
}


GameResinBallView::~GameResinBallView()
{

}

bool GameResinBallView::init(GameResinBallViewDelegate* pViewDelegate,GameResinBallModel* gameResinBall){
	bool pRet=false;
	do{ 
		// 先调用超类的init方法
        CC_BREAK_IF(! CCLayer::init());
		
		
		_ResinBallModel=gameResinBall;
		this->pViewDelegate=pViewDelegate;
		_ResinBall=CCSprite::create("CloseNormal.png");
		_ResinBall->retain();
		update(0);
		addChild(_ResinBall);

		this->setTouchEnabled(true);

		pRet=true;
	}while(0);

	return pRet;
}

void GameResinBallView::update(float dt){
	//CCLog("ResinBallView:update");
	if(_ResinBall==NULL){
		return;
	}

	_ResinBall->setPosition(_ResinBallModel->getPosition());
	//CCLog("%f,%f",_ResinBall->getPosition().x,_ResinBall->getPosition().y);
}

void GameResinBallView::testModelDelegate(){
	//CCLog("testModelDelegate");
	update(0);
}







void GameResinBallView::ccTouchesBegan(CCSet *touches,CCEvent *event){	
	//获得鼠标点击的位置并把其转化为Box2D世界的坐标
	b2Vec2 locationWorld =getLocationWorld(touches);

	pViewDelegate->dealWithTouchesBegan(locationWorld);
}
 

void GameResinBallView::ccTouchesMoved(CCSet *touches,CCEvent *event) {
	//获得鼠标点击的位置并把其转化为Box2D世界的坐标
	b2Vec2 locationWorld =getLocationWorld(touches);

	pViewDelegate->dealWithTouchesMoved(locationWorld);
}

void GameResinBallView::ccTouchesCancelled(CCSet *touches, CCEvent *event) {
	pViewDelegate->dealWithTouchesEnded();
}

void GameResinBallView::ccTouchesEnded(CCSet *touches, CCEvent *event) {
	pViewDelegate->dealWithTouchesEnded();
}