#include "GameWelcomeController.h"
#include "Constant.h"
#include "VisibleRect.h"
#include "GamePlayController.h"
#include "GameMenuController.h"
#include "../extensions/cocos-ext.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)   
#include "vld.h"   
#endif 

USING_NS_CC;
USING_NS_CC_EXT;

GameWelcomeController::GameWelcomeController()
{
}


GameWelcomeController::~GameWelcomeController()
{	

}

bool GameWelcomeController::init(){
	bool pRet = false;

    do{
        // 先调用超类的init方法
		CC_BREAK_IF(! CCLayer::init());
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();

		CCMenuItemImage* pStartItem=CCMenuItemImage::create("start.png","start.png",this,menu_selector(GameWelcomeController::menuStartCallback));
		CC_BREAK_IF(!pStartItem);
		this->setScale(CCDirector::sharedDirector()->getContentScaleFactor());//屏幕适应，图片缩放
		pStartItem->setPosition(winSize.width/2,winSize.height/4);

		CCMenu* menu=CCMenu::create(pStartItem,NULL);
		menu->setPosition(CCPointZero);
		addChild(menu);

		CCSprite* welcome_bg=CCSprite::create("welcome_bg.png");
		welcome_bg->setPosition(ccp(winSize.width/2,winSize.height/2));
		addChild(welcome_bg,-1);

        pRet = true;
    }while(0);
    return pRet;
}
void GameWelcomeController::initScrollView(){
	
}

CCScene* GameWelcomeController::scene(){
    CCScene *scene = NULL;
    do{
		scene = CCScene::create();
        CC_BREAK_IF(!scene);

        GameWelcomeController *layer = GameWelcomeController::create();
        CC_BREAK_IF(!layer);

        scene->addChild(layer);

    }while(0);

    return scene;
}

void GameWelcomeController::update(float dt){
	
}

void GameWelcomeController::menuStartCallback(CCObject* pSender){
	//CCScene* gamePlay=GamePlayController::scene();
	//CCTransitionScene *scene=CCTransitionCrossFade::create(2,gamePlay);
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(2,GameMenuController::scene()));
}


void GameWelcomeController::onEnter(){

	CCLayer::onEnter();
}
void GameWelcomeController::onEnterTransitionDidFinish(){
	CCLayer::onEnterTransitionDidFinish();
}
void GameWelcomeController::onExitTransitionDidStart(){
	CCLayer::onExitTransitionDidStart();
}
void GameWelcomeController::onExit(){
	CCLayer::onExit();
}