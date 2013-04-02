#include "GameMenuController.h"
#include "Constant.h"
#include "VisibleRect.h"
#include "GamePlayController.h"
#include "SmartRes.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)   
#include "vld.h"   
#endif 

USING_NS_CC;
USING_NS_CC_EXT;

GameMenuController::GameMenuController():space(100),curPage(1),nCount(3),isScrolling(false)
{
}


GameMenuController::~GameMenuController()
{	

}

bool GameMenuController::init(){
	bool pRet = false;

    do{
        // 先调用超类的init方法
		CC_BREAK_IF(! CCLayer::init());
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();

	tableWidth=winSize.width*0.68f;//左侧面板宽度
	scrollViewWidth_2=(winSize.width-tableWidth)/2;

	CCSprite* sp=CCSprite::create("map.png");
	sp->setAnchorPoint(ccp(1,0));
	sp->setPosition(ccp(tableWidth,0));
	this->addChild(sp);

	this->initMapScrollView();//初始化右侧面板
	this->initLeftTable();//初始化左侧面板

        pRet = true;
    }while(0);
    return pRet;
}
void GameMenuController::initLeftTable(){	
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	CCMenuItemImage* pGoBackItem=CCMenuItemImage::create("goback.png","goback.png",this,menu_selector(GameMenuController::menuGoBackCallback));
	
	pGoBackItem->setPosition(_left+pGoBackItem->getContentSize().width/2,pGoBackItem->getContentSize().height/2);

    CCMenuItemImage *pCollectItem = CCMenuItemImage::create(
                                        "collections.png",
                                        "collections.png",
                                        this,
                                        menu_selector(GameMenuController::menuCollectCallback));
    
	pCollectItem->setPosition(_left+tableWidth/2,pCollectItem->getContentSize().height/2);

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pGoBackItem,pCollectItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
}

void GameMenuController::initMapScrollView(){
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();

	CCLayer* containerLayer=CCLayer::create();
	//containerLayer->setTag(111);
	containerLayer->setContentSize(CCSizeMake(scrollViewWidth_2*2,winSize.height));
	CCSize containerSize=containerLayer->getContentSize();
	CCSprite* sprite=CCSprite::create("bg_01.png");
	sprite->setTag(1);
	space=(containerSize.width-sprite->getContentSize().width)/2.0f;
	sprite->setPosition(ccp(space+sprite->getContentSize().width/2,containerSize.height/2));
	containerLayer->addChild(sprite);

	CCSprite* sprite2=CCSprite::create("bg_02.png");
	sprite2->setTag(2);
	sprite2->setPosition(ccp(sprite->getPositionX()*2,containerSize.height/2));
	containerLayer->addChild(sprite2);
	
	CCSprite* sprite3=CCSprite::create("bg_03.png");
	sprite3->setTag(3);
	sprite3->setPosition(ccp(sprite->getPositionX()*3,containerSize.height/2));
	containerLayer->addChild(sprite3);

	scrollView=CCScrollView::create(containerSize,containerLayer);
	scrollView->setDirection(kCCScrollViewDirectionHorizontal);
	scrollView->setTouchEnabled(false);
	scrollView->setPosition(ccp(tableWidth,0));//设置此scrollView的位置
	addChild(scrollView);
}

CCScene* GameMenuController::scene(){
    CCScene *scene = NULL;
    do{
		scene = CCScene::create();
        CC_BREAK_IF(!scene);

        GameMenuController *layer = GameMenuController::create();
        CC_BREAK_IF(!layer);

        scene->addChild(layer);

    }while(0);

    return scene;
}

bool GameMenuController::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	isScrolling=false;
	startPoint=pTouch->getLocationInView();
	return true;
}
void GameMenuController::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	if(!isScrolling) isScrolling=true;
	CCPoint delta=ccp(pTouch->getLocationInView().x-pTouch->getPreviousLocationInView().x,0);
	CCPoint adjustPos=ccpAdd(scrollView->getContentOffset(),delta);

	if(curPage==nCount&&adjustPos.x<-curPage*scrollViewWidth_2) return;
	if(curPage==1&&adjustPos.x>scrollViewWidth_2) return;
	scrollView->setContentOffset(adjustPos);
}
void GameMenuController::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
	CCPoint endPoint=pTouch->getLocationInView();
	float distance=endPoint.x-startPoint.x;
	if(distance!=0){
		adjustScrollView(distance);
	}

	//判断是否是点击事件
	if(!isScrolling){
		CCSprite* sprite=(CCSprite*)scrollView->getContainer()->getChildByTag(curPage);//获得当前页的图片
		//把坐标转换为GL坐标系,并转换为相对container的坐标
	/*	CCPoint touchPoint=ccpAdd(
			ccpSub(CCDirector::sharedDirector()->convertToGL(endPoint),scrollView->getPosition()),
			ccp(scrollViewWidth_2*(curPage-1),0));*/
		CCPoint touchPoint=CCDirector::sharedDirector()->convertToGL(endPoint);
		touchPoint=((CCLayer*)scrollView->getContainer())->convertToNodeSpace(touchPoint);
		if(sprite->boundingBox().containsPoint(touchPoint)){//点击到当前页面
			CCLog("clicked:%d",curPage);
		}else{//没点击到当前页
			CCLog("shake curPage:%d",curPage);
		}
	}
	
}
void GameMenuController::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){

}

void GameMenuController::adjustScrollView(float distance){

	scrollView->unscheduleAllSelectors();

	if(distance<0){//右移
		curPage++;
		if(curPage>nCount) curPage=nCount;
	}else if(distance>0){//左移
		curPage--;
		if(curPage<1)curPage=1;
	}

	CCPoint adjustPos=ccp(-(curPage-1)*(scrollViewWidth_2),0);
	
	scrollView->setContentOffset(adjustPos,true);
}

void GameMenuController::update(float dt){
	
}

void GameMenuController::menuGoBackCallback(CCObject* pSender){
	CCLog("go Back to welcome page");
	//CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(2,GameWelcomeController::scene()));
}
void GameMenuController::menuCollectCallback(CCObject* pSender){
	CCLog("go to collections page");
	//CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(2,GameCollectionController::scene()));
}
void GameMenuController::menuClickCallback(CCObject* pSender){
	CCLog("click");
}

void GameMenuController::onEnter(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,1,true);
	CCLog("%d",this->isTouchEnabled());
	CCLayer::onEnter();
}
void GameMenuController::onEnterTransitionDidFinish(){
	CCLayer::onEnterTransitionDidFinish();
}
void GameMenuController::onExitTransitionDidStart(){
	CCLayer::onExitTransitionDidStart();
}
void GameMenuController::onExit(){
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}