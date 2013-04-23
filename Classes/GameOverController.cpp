#include "GameOverController.h"
#include "Constant.h"
#include "VisibleRect.h"
#include "GamePlayController.h"
#include "GameMenuController.h"
#include "GameCollectionController.h"
#include "../extensions/cocos-ext.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)   
#include "vld.h"   
#endif 

USING_NS_CC;
USING_NS_CC_EXT;

GameOverController::GameOverController()
{
}


GameOverController::~GameOverController()
{	

}

bool GameOverController::init(){
	bool pRet = false;

    do{
        // 先调用超类的init方法
		CC_BREAK_IF(! CCLayer::init());
		winSize=CCDirector::sharedDirector()->getWinSize();
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("gameover.plist","gameover.png");

		CCSprite* panel_result=this->initResultPanel();
		panel_result->setAnchorPoint(ccp(0,0.5));
		panel_result->setPosition(ccp(0,winSize.height/2));
		this->addChild(panel_result);

		CCLayer* topPanel=this->initTopPanel();
		topPanel->ignoreAnchorPointForPosition(false);
		topPanel->setAnchorPoint(ccp(0,1));
		topPanel->setPosition(ccp(0,winSize.height));
		this->addChild(topPanel);

		CCLayer* bottomPanel=this->initBottomPanel();
		bottomPanel->ignoreAnchorPointForPosition(false);
		bottomPanel->setAnchorPoint(CCPointZero);
		bottomPanel->setPosition(CCPointZero);
		this->addChild(bottomPanel);

		CCSprite* gameOver_bg=CCSprite::create("gameover_bg.png");
		gameOver_bg->setPosition(ccp(winSize.width/2,winSize.height/2));
		addChild(gameOver_bg,-1);

		CCSprite* newCollection=CCSprite::createWithSpriteFrameName("new_collection.png");
		CCMenuItem* btn_collection=CCMenuItemSprite::create(newCollection,newCollection,newCollection,this,
			menu_selector(GameOverController::menuCollectionCallback));
		btn_collection->setAnchorPoint(ccp(1,1));
		btn_collection->setPosition(ccpFromSize(winSize));
		CCMenu* menu=CCMenu::create(btn_collection,NULL);
		menu->setAnchorPoint(CCPointZero);
		menu->setPosition(CCPointZero);
		this->addChild(menu);

        pRet = true;
    }while(0);
    return pRet;
}

CCSprite* GameOverController::initResultPanel(){
	CCSprite* panel_result=CCSprite::createWithSpriteFrameName("result_panel.png");
	CCSize panelSize=panel_result->getContentSize();

	unsigned int score=CCUserDefault::sharedUserDefault()->getIntegerForKey(GET_SCORE,0);
	char str_score[10]={0};
	sprintf(str_score,"%d",score);
	CCLabelBMFont *ttf_score=CCLabelBMFont::create(str_score, "fonts/bitmapFontTest3.fnt");
	ttf_score->setAnchorPoint(ccp(0.5,0.5));
	ttf_score->setPosition(ccp(panelSize.width/2,panelSize.height*10.f/17));
	panel_result->addChild(ttf_score);

	unsigned int amber=CCUserDefault::sharedUserDefault()->getIntegerForKey(GET_AMBER,0);
	char str_amber[10]={0};
	sprintf(str_amber,"%d",amber);
	CCLabelBMFont *ttf_amber=CCLabelBMFont::create(str_amber, "fonts/bitmapFontTest3.fnt");
	ttf_amber->setAnchorPoint(ccp(0.5,0.5));
	ttf_amber->setPosition(ccp(panelSize.width/2,panelSize.height*7.f/17));
	panel_result->addChild(ttf_amber);

	unsigned int money=CCUserDefault::sharedUserDefault()->getIntegerForKey(GET_MONEY,0);
	char str_money[10]={0};
	sprintf(str_money,"%d",money);
	CCLabelBMFont *ttf_money=CCLabelBMFont::create(str_money, "fonts/bitmapFontTest3.fnt");
	ttf_money->setAnchorPoint(ccp(0.5,0.5));
	ttf_money->setPosition(ccp(panelSize.width/2,panelSize.height*3.5f/17));
	panel_result->addChild(ttf_money);

	CCLabelBMFont *ttf_level=CCLabelBMFont::create("LEVEL 12", "fonts/bitmapFontTest3.fnt");
	ttf_level->setAnchorPoint(ccp(0.5,0.5));
	ttf_level->setPosition(ccp(panelSize.width/2,panelSize.height*15.f/17));
	panel_result->addChild(ttf_level);

	CCSprite* newRecord=CCSprite::createWithSpriteFrameName("newrecord.png");
	newRecord->setPosition(ccp(panelSize.width*24.0f/32,ttf_score->getPositionY()));
	panel_result->addChild(newRecord);

	return panel_result;
}

CCLayer* GameOverController::initTopPanel(){	
	CCLayer* topLayer=CCLayer::create();

	CCSprite* column_dollar=CCSprite::createWithSpriteFrameName("col_dollar.png");
	CCSprite* column_amber=CCSprite::createWithSpriteFrameName("col_resin.png");
	//CCSprite* column_clock=CCSprite::createWithSpriteFrameName("col_clock.png");

	CCSprite* shop=CCSprite::createWithSpriteFrameName("btn_shop.png");
	CCMenuItemSprite* btnShop=CCMenuItemSprite::create(shop,shop,shop,this,menu_selector(GameMenuController::menuShopCallback));
	
	//左上面板大小
	topLayer->setContentSize(CCSizeMake(winSize.width*0.7,column_dollar->getContentSize().height));

	topLayer->addChild(column_dollar);
	column_dollar->setAnchorPoint(CCPointZero);
	column_dollar->setPosition(CCPointZero);
	unsigned int dollar=CCUserDefault::sharedUserDefault()->getIntegerForKey(DOLLAR,ORIGIN_DOLLAR);
	char str_dollar[10]={0};
	sprintf(str_dollar,"%d",dollar);
	CCLabelBMFont *ttf_dollar=CCLabelBMFont::create(str_dollar, "fonts/bitmapFontTest3.fnt");
	ttf_dollar->setAnchorPoint(ccp(0.5,0.5));
	ttf_dollar->setPosition(ccp(column_dollar->getContentSize().width*2.2f/4,column_dollar->getContentSize().height/2));
	column_dollar->addChild(ttf_dollar);

	topLayer->addChild(column_amber);
	column_amber->setAnchorPoint(ccp(1,0));
	column_amber->setPosition(ccp(topLayer->getContentSize().width,0));
	
	unsigned int resin=CCUserDefault::sharedUserDefault()->getIntegerForKey(RESIN_VOLUME,ORIGIN_RESIN);
	char str_resin[10]={0};
	sprintf(str_resin,"%d",resin);
	CCLabelBMFont *ttf_resin=CCLabelBMFont::create(str_resin, "fonts/bitmapFontTest3.fnt");
	ttf_resin->setAnchorPoint(ccp(0.5,0.5));
	ttf_resin->setPosition(ccp(column_amber->getContentSize().width*2.2f/4,column_amber->getContentSize().height/2));
	column_amber->addChild(ttf_resin);
	
	CCMenu* menuShop=CCMenu::create(btnShop,NULL);
	menuShop->setAnchorPoint(ccp(0,0.5));
	menuShop->setPosition(ccp(column_dollar->getPositionX()+column_dollar->getContentSize().width+100,
		column_dollar->getPositionY()+column_dollar->getContentSize().height/2));
	topLayer->addChild(menuShop);

	return topLayer;
}
CCLayer* GameOverController::initBottomPanel(){	
	CCLayer* bottomLayer=CCLayer::create();

	CCSprite* goback=CCSprite::createWithSpriteFrameName("result_goback.png");
	CCSprite* restart=CCSprite::createWithSpriteFrameName("result_restart.png");
	CCSprite* wiki=CCSprite::createWithSpriteFrameName("btn_wiki.png");
	
	CCSize size=CCSizeMake(winSize.width,goback->getContentSize().height);
	bottomLayer->setContentSize(size);

	CCMenuItem* btn_goback=CCMenuItemSprite::create(goback,goback,goback,this,menu_selector(GameOverController::menuGoBackCallback));
	btn_goback->setAnchorPoint(CCPointZero);
	btn_goback->setPosition(CCPointZero);

	CCMenuItem* btn_restart=CCMenuItemSprite::create(restart,restart,restart,this,menu_selector(GameOverController::menuRestartCallback));
	btn_restart->setAnchorPoint(ccp(0.5,0));
	btn_restart->setPosition(ccp(size.width*15.5f/40,0));

	CCMenuItem* btn_wiki=CCMenuItemSprite::create(wiki,wiki,wiki,this,menu_selector(GameOverController::menuWikiCallback));
	btn_wiki->setAnchorPoint(ccp(0.5,0));
	btn_wiki->setPosition(ccp(size.width*27.5f/40,0));
	
	CCMenu* menuShop=CCMenu::create(btn_goback,btn_restart,btn_wiki,NULL);
	menuShop->setAnchorPoint(ccp(0,0.5));
	menuShop->setPosition(CCPointZero);
	bottomLayer->addChild(menuShop);

	return bottomLayer;
}

void GameOverController::initScrollView(){
	
}

CCScene* GameOverController::scene(){
    CCScene *scene = NULL;
    do{
		scene = CCScene::create();
        CC_BREAK_IF(!scene);

        GameOverController *layer = GameOverController::create();
        CC_BREAK_IF(!layer);

        scene->addChild(layer);

    }while(0);

    return scene;
}

void GameOverController::update(float dt){
	
}

void GameOverController::menuRestartCallback(CCObject* pSender){
	//CCScene* gamePlay=GamePlayController::scene();
	//CCTransitionScene *scene=CCTransitionCrossFade::create(2,gamePlay);
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GamePlayController::scene()));
}

void GameOverController::menuShopCallback(CCObject* pSender){
	CCLog("click shop button");
}
void GameOverController::menuCollectionCallback(CCObject* pSender){
	CCLog("go to collections page");
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GameCollectionController::scene()));
}
void GameOverController::menuGoBackCallback(CCObject* pSender){
	CCLog("go to menu page");
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GameMenuController::scene()));
}
void GameOverController::menuWikiCallback(CCObject* pSender){
	CCLog("go to wiki page");
	//CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GameCollectionController::scene()));
}


void GameOverController::onEnter(){

	CCLayer::onEnter();
}
void GameOverController::onEnterTransitionDidFinish(){
	CCLayer::onEnterTransitionDidFinish();
}
void GameOverController::onExitTransitionDidStart(){
	CCLayer::onExitTransitionDidStart();
}
void GameOverController::onExit(){
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("gameover.plist");
	CCLayer::onExit();
}