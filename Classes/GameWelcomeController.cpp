#include "GameWelcomeController.h"
#include "Constant.h"
#include "VisibleRect.h"
#include "GamePlayController.h"
#include "GameMenuController.h"
#include "GlobalComponent.h"
#include "../extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)   
#include "vld.h"   
#endif 

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;

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
		CCMenuItem* pStartItem=GlobalComponent::createMenuItem(MENU_START,this,menu_selector(GameWelcomeController::menuStartCallback));
		//CCMenuItem* pStartItem=CCMenuItemImage::create("btn_start.png","btn_start.png",this,menu_selector(GameWelcomeController::menuStartCallback));
		CC_BREAK_IF(!pStartItem);
		this->setScale(CCDirector::sharedDirector()->getContentScaleFactor());//屏幕适应，图片缩放
		pStartItem->setPosition(winSize.width/2,winSize.height/4);
		pStartItem->setAnchorPoint(ccp(0.5,0.5));

		CCMenu* menu=CCMenu::create(pStartItem,NULL);
		menu->setAnchorPoint(ccp(0.5,0.5));
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

void GameWelcomeController::menuCloseCallback(CCObject* pSender){
	CCDirector::sharedDirector()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

       exit(0);

    #endif


}

void GameWelcomeController::menuStartCallback(CCObject* pSender){
	//CCScene* gamePlay=GamePlayController::scene();
	//CCTransitionScene *scene=CCTransitionCrossFade::create(2,gamePlay);
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GameMenuController::scene()));
}

void GameWelcomeController::keyBackClicked(){
	CCLog("key back clicked");
	this->menuCloseCallback(this);
}


void GameWelcomeController::onEnter(){
	this->setKeypadEnabled(true);
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/menu.ogg",true);
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