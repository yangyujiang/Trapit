#include "GameLevelController.h"
#include "Constant.h"
#include "VisibleRect.h"
#include "GamePlayController.h"
#include "../extensions/cocos-ext.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)   
#include "vld.h"   
#endif 

USING_NS_CC;
USING_NS_CC_EXT;

GameLevelController::GameLevelController()
{
}


GameLevelController::~GameLevelController()
{	

}

bool GameLevelController::init(){
	bool pRet = false;

    do{
        // 先调用超类的init方法
		CC_BREAK_IF(! CCLayer::init());

		CCMenuItemImage* pStartItem=CCMenuItemImage::create("start.png","start_selected.png",this,menu_selector(GameLevelController::menuStartCallback));
		CC_BREAK_IF(!pStartItem);
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		pStartItem->setPosition(winSize.width/2,winSize.height/2);

		CCMenu* menu=CCMenu::create(pStartItem,NULL);
		menu->setPosition(CCPointZero);
		//addChild(menu);
		this->initScrollView();
		//this->schedule(schedule_selector(GameMenuController::update));

        pRet = true;
    }while(0);
    return pRet;
}
void GameLevelController::initScrollView(){
	CCScrollView* scrollView=CCScrollView::create();

	CCLayer *layer = CCLayer::create();  
      
    CCSprite *sprite1 = CCSprite::create("HelloWorld.png");  
    CCSprite *sprite2 = CCSprite::create("HelloWorld.png");  
      
    layer->setAnchorPoint(CCPointZero);  
    layer->setPosition(CCPointZero);  
	CCSize size=CCDirector::sharedDirector()->getVisibleSize();
      
    // Menu   
    CCMenuItemSprite *menuItem1 = CCMenuItemSprite::create(sprite1, sprite1, this, menu_selector(GameLevelController::menuStartCallback));  
    menuItem1->setPosition(ccpAdd(CCPointZero, ccp(size.width / 2, size.height / 2)));  
   // menuItem1->setScale(0.4f);  
    CCMenuItemSprite *menuItem2 = CCMenuItemSprite::create(sprite2, sprite2, this, menu_selector(GameLevelController::menuStartCallback));  
	menuItem2->setPosition(ccpAdd(ccp(size.width, 0), ccp(size.width / 2, size.height / 2)));  
   // menuItem2->setScale(0.4f);  
    CCMenu *menu = CCMenu::create(menuItem1, menuItem2, NULL);  
      
    menu->setPosition(CCPointZero);  
    layer->addChild(menu);
	scrollView->setContentSize(CCSizeMake(size.width, size.height));  
    scrollView->setContainer(layer);
	scrollView->setPosition(CCPointZero);
	addChild(scrollView,1);
}

CCScene* GameLevelController::scene(){
    CCScene *scene = NULL;
    do{
		scene = CCScene::create();
        CC_BREAK_IF(!scene);

        GameLevelController *layer = GameLevelController::create();
        CC_BREAK_IF(!layer);

        scene->addChild(layer);

    }while(0);

    return scene;
}

void GameLevelController::update(float dt){
	
}

void GameLevelController::menuStartCallback(CCObject* pSender){
	//CCScene* gamePlay=GamePlayController::scene();
	//CCTransitionScene *scene=CCTransitionCrossFade::create(2,gamePlay);
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(2,GamePlayController::scene()));
}


void GameLevelController::onEnter(){

	CCLayer::onEnter();
}
void GameLevelController::onEnterTransitionDidFinish(){
	CCLayer::onEnterTransitionDidFinish();
}
void GameLevelController::onExitTransitionDidStart(){
	CCLayer::onExitTransitionDidStart();
}
void GameLevelController::onExit(){
	CCLayer::onExit();
}