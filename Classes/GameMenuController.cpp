#include "GameMenuController.h"
#include "Constant.h"
#include "VisibleRect.h"
#include "GamePlayController.h"
#include "GameWelcomeController.h"
#include "GameCollectionController.h"
#include "SmartRes.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)   
#include "vld.h"   
#endif 

USING_NS_CC;
USING_NS_CC_EXT;

GameMenuController::GameMenuController():
space(100),curPage(1),nCount(3),isScrolling(false),isSliding(false),
	isMovingMap(false)
{
}


GameMenuController::~GameMenuController()
{	

}

bool GameMenuController::init(){
	bool pRet = false;

    do{
        // �ȵ��ó����init����
		CC_BREAK_IF(! CCLayer::init());
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("leftPanel.plist","leftPanel.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("buttons.plist","buttons.png");
	winSize=CCDirector::sharedDirector()->getWinSize();
	

	tableWidth=winSize.width*0.6f;//��������
	scrollViewWidth_2=(winSize.width-tableWidth)/2;

	CCSprite* sp=CCSprite::create("menu_bg.png");
	sp->setAnchorPoint(ccp(0,0));
	sp->setPosition(ccp(0,0));
	this->addChild(sp);

	this->initMapScrollView();//��ʼ���Ҳ����
	this->initLeftTable();//��ʼ��������

        pRet = true;
    }while(0);
    return pRet;
}
CCMenu* GameMenuController::initLeftBottomdPanel(){
	CCMenuItemImage* pGoBackItem=CCMenuItemImage::create("goback.png","goback.png",this,menu_selector(GameMenuController::menuGoBackCallback));
	pGoBackItem->setAnchorPoint(ccp(0,0));
	pGoBackItem->setScale(CCDirector::sharedDirector()->getContentScaleFactor());
	pGoBackItem->setPosition(_left,_bottom);

    CCMenuItemImage *pCollectItem = CCMenuItemImage::create(
                                        "collections.png",
                                        "collections.png",
                                        this,
                                        menu_selector(GameMenuController::menuCollectCallback));
	pCollectItem->setAnchorPoint(ccp(0.5,0));
	pCollectItem->setScale(CCDirector::sharedDirector()->getContentScaleFactor());
	pCollectItem->setPosition(_left+tableWidth/2,_bottom);


    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pGoBackItem,pCollectItem, NULL);

	
	return pMenu;
}
CCLayer* GameMenuController::initLeftMidPanel(){//��ʼ������м䲿��
	CCLayer* leftPanel=CCLayer::create();

	CCSprite* panel_color=CCSprite::createWithSpriteFrameName("panel_red.png");
	panel_color->setTag(TAG_PANEL_COLOR);
	panel_color->setAnchorPoint(CCPointZero);
	leftPanel->setContentSize(panel_color->getContentSize());
	leftPanel->addChild(panel_color);

	CCSize panelSize=panel_color->getContentSize();//���panel�Ĵ�С
	//��ɫ���갴ť
	CCSprite* sprite=CCSprite::createWithSpriteFrameName("btn_red.png");
	CCMenuItemSprite* btnRed=CCMenuItemSprite::create(
		sprite,sprite,sprite,this,menu_selector(GameMenuController::menuChooseResinCallback));
	btnRed->setTag(panel_red);
	btnRed->setAnchorPoint(ccp(0.5,1));
	btnRed->setPosition(ccp(panelSize.width*10.0f/31,panelSize.height*30.f/31));
	//��ɫ���갴ť
	sprite=CCSprite::createWithSpriteFrameName("btn_yellow.png");
	CCMenuItemSprite* btnYellow=CCMenuItemSprite::create(
		sprite,sprite,sprite,this,menu_selector(GameMenuController::menuChooseResinCallback));
	btnYellow->setTag(panel_yellow);
	btnYellow->setAnchorPoint(ccp(0.5,1));
	btnYellow->setPosition(ccp(panelSize.width*15.0f/31,panelSize.height*30.f/31));
	//��ɫ���갴ť
	sprite=CCSprite::createWithSpriteFrameName("btn_green.png");
	CCMenuItemSprite* btnGreen=CCMenuItemSprite::create(
		sprite,sprite,sprite,this,menu_selector(GameMenuController::menuChooseResinCallback));
	btnGreen->setTag(panel_green);
	btnGreen->setAnchorPoint(ccp(0.5,1));
	btnGreen->setPosition(ccp(panelSize.width*21.0f/31,panelSize.height*30.f/31));
	//��ɫ���갴ť
	sprite=CCSprite::createWithSpriteFrameName("btn_blue.png");
	CCMenuItemSprite* btnBlue=CCMenuItemSprite::create(
		sprite,sprite,sprite,this,menu_selector(GameMenuController::menuChooseResinCallback));
	btnBlue->setTag(panel_blue);
	btnBlue->setAnchorPoint(ccp(0.5,1));
	btnBlue->setPosition(ccp(panelSize.width*27.0f/31,panelSize.height*30.f/31));
	//������ť�˵�
	CCMenu* pMenu=CCMenu::create(btnRed,btnYellow,btnGreen,btnBlue,NULL);
	pMenu->setAnchorPoint(CCPointZero);
	pMenu->setPosition(CCPointZero);
	leftPanel->addChild(pMenu,1);

	//�ʺ�ͼƬ
	CCSprite* help=CCSprite::createWithSpriteFrameName("help.png");
	help->setAnchorPoint(ccp(0,1));
	help->setPosition(ccp(0,panelSize.height));
	leftPanel->addChild(help);

	//����
	CCSprite* stars=CCSprite::createWithSpriteFrameName("stars.png");
	stars->setAnchorPoint(CCPointZero);
	stars->setPosition(ccp(30,30));
	leftPanel->addChild(stars);
	
	//���ܲ�
	CCSprite* skillSlots=CCSprite::createWithSpriteFrameName("skillslots.png");
	skillSlots->setAnchorPoint(ccp(1,0));
	skillSlots->setPosition(ccp(tableWidth+100,30));
	leftPanel->addChild(skillSlots);

	//��֬��ˮŨ�ȵ�����
	CCSprite* slide=CCSprite::createWithSpriteFrameName("slide.png");
	slide->setAnchorPoint(ccp(1,0));
	slide->setPosition(ccp(tableWidth+100,0.6f*panelSize.height));
	leftPanel->addChild(slide);

	//����
	CCSprite* slider=CCSprite::createWithSpriteFrameName("slider.png");
	slider->setTag(TAG_SLIDER);
	slider->setAnchorPoint(ccp(0,0.2f));
	slider->setPosition(ccp(tableWidth/2,0.6f*panelSize.height));
	leftPanel->addChild(slider,1);
	
	//��ʼ��������Сλ�ã��Ϳɻ�����Զ����
	slideStart=slide->getPositionX()-slide->getContentSize().width*21.0f/23;
	slideEnd=slide->getPositionX()-slide->getContentSize().width*3.0f/23-slider->getContentSize().width;

	return leftPanel;

}
CCLayer* GameMenuController::initLeftTopPanel(){	
	CCLayer* leftTopLayer=CCLayer::create();

	CCSprite* column_dollar=CCSprite::createWithSpriteFrameName("column_dollar.png");
	CCSprite* column_amber=CCSprite::createWithSpriteFrameName("column_amber.png");
	CCSprite* column_clock=CCSprite::createWithSpriteFrameName("column_clock.png");

	CCSprite* shop=CCSprite::createWithSpriteFrameName("btn_shop.png");
	CCMenuItemSprite* btnShop=CCMenuItemSprite::create(shop,shop,shop,this,menu_selector(GameMenuController::menuShopCallback));
	

	//��������С
	leftTopLayer->setContentSize(CCSizeMake(tableWidth,column_dollar->getContentSize().height));

	leftTopLayer->addChild(column_dollar);
	column_dollar->setAnchorPoint(CCPointZero);
	column_dollar->setPosition(CCPointZero);
	//leftTopLayer->addChild(column_clock);
	column_clock->setAnchorPoint(ccp(1,1));
	leftTopLayer->addChild(column_amber);
	column_amber->setAnchorPoint(ccp(1,0));
	column_amber->setPosition(ccp(leftTopLayer->getContentSize().width,0));
	
	CCMenu* menuShop=CCMenu::create(btnShop,NULL);
	menuShop->setAnchorPoint(ccp(0,0.5));
	menuShop->setPosition(ccp(column_dollar->getPositionX()+column_dollar->getContentSize().width+100,
		column_dollar->getPositionY()+column_dollar->getContentSize().height/2));
	leftTopLayer->addChild(menuShop);

	return leftTopLayer;
}

void GameMenuController::initLeftTable(){	
	CCLayer* leftLayer=CCLayer::create();
	leftLayer->setTag(TAG_LEFT_TABLE);
	leftLayer->ignoreAnchorPointForPosition(false);
	leftLayer->setContentSize(CCSizeMake(tableWidth,winSize.height));
	leftLayer->setAnchorPoint(ccp(0,0));

	CCLayer* leftMidPanel=this->initLeftMidPanel();
	leftMidPanel->setTag(TAG_LEFT_MID_PANEL);
	leftMidPanel->ignoreAnchorPointForPosition(false);
	leftMidPanel->setAnchorPoint(ccp(0,0.5));
	//leftPanel->setScale(CCDirector::sharedDirector()->getContentScaleFactor());
	leftMidPanel->setPosition(ccp(0,leftLayer->getContentSize().height/2));
	leftLayer->addChild(leftMidPanel);

	CCLayer* leftTopPanel=this->initLeftTopPanel();
	leftTopPanel->ignoreAnchorPointForPosition(false);
	leftTopPanel->setAnchorPoint(ccp(0,1));
	leftTopPanel->setPosition(0,leftLayer->getContentSize().height);
	leftLayer->addChild(leftTopPanel);

	CCMenu* leftBottomPanel=this->initLeftBottomdPanel();
	leftBottomPanel->setAnchorPoint(CCPointZero);
	leftBottomPanel->setPosition(CCPointZero);
	leftLayer->addChild(leftBottomPanel);

	this->addChild(leftLayer,1);
}

void GameMenuController::initMapScrollView(){

	CCLayer* containerLayer=CCLayer::create();
	//containerLayer->setTag(111);
	containerLayer->setContentSize(CCSizeMake(scrollViewWidth_2*2,winSize.height));

	CCSize containerSize=containerLayer->getContentSize();
	CCSprite* sprite=CCSprite::create("bg_01.png");
	sprite->setScale(CCDirector::sharedDirector()->getContentScaleFactor());
	sprite->setTag(1);
	space=(containerSize.width-sprite->getContentSize().width)/2.0f;
	sprite->setPosition(ccp(space+sprite->getContentSize().width/2,containerSize.height/2));
	containerLayer->addChild(sprite);

	CCSprite* sprite2=CCSprite::create("bg_02.png");
	sprite2->setScale(CCDirector::sharedDirector()->getContentScaleFactor());
	sprite2->setTag(2);
	sprite2->setPosition(ccp(sprite->getPositionX()*2,containerSize.height/2));
	containerLayer->addChild(sprite2);
	
	CCSprite* sprite3=CCSprite::create("bg_03.png");
	sprite3->setScale(CCDirector::sharedDirector()->getContentScaleFactor());
	sprite3->setTag(3);
	sprite3->setPosition(ccp(sprite->getPositionX()*3,containerSize.height/2));
	containerLayer->addChild(sprite3);

	scrollView=CCScrollView::create(containerSize,containerLayer);
	scrollView->setDirection(kCCScrollViewDirectionHorizontal);
	scrollView->setTouchEnabled(false);
	scrollView->setPosition(ccp(tableWidth,0));//���ô�scrollView��λ��
	addChild(scrollView);

	CCLabelTTF* pLabel = CCLabelTTF::create("CHOOSE MAP", "Thonburi", 34);
    pLabel->setPosition(ccp(tableWidth+(winSize.width-tableWidth)/2,winSize.height*3.f/4));
    this->addChild(pLabel, 1);
	
	//��ʼ��ť
	CCSprite* start=CCSprite::createWithSpriteFrameName("start.png");
	CCMenuItemSprite* btnStart=CCMenuItemSprite::create(start,start,start,this,menu_selector(GameMenuController::menuStartCallback));
	btnStart->setAnchorPoint(ccp(0.5,0.5));
	btnStart->setPosition(ccp((winSize.width-tableWidth)/2,winSize.height/4));

	CCMenu* pMenu=CCMenu::create(btnStart,NULL);
	pMenu->setAnchorPoint(CCPointZero);
	pMenu->setPosition(ccp(tableWidth,0));
	this->addChild(pMenu,1);
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
	startPoint=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	isScrolling=false;
	if(startPoint.x<tableWidth){
		//�㵽���
		CCSprite* slider=
			(CCSprite*)this->getChildByTag(TAG_LEFT_TABLE)->getChildByTag(TAG_LEFT_MID_PANEL)->getChildByTag(TAG_SLIDER);
		CCSprite* panel=(CCSprite*)this->getChildByTag(TAG_LEFT_TABLE)->getChildByTag(TAG_LEFT_MID_PANEL);

		if(slider->boundingBox().containsPoint(panel->convertToNodeSpace(startPoint))){
			isSliding=true;//ֻҪ���л��飬�ͱ��Ϊ���Կ�ʼ�ƶ�
		}
	}else isMovingMap=true;
	return true;
}
void GameMenuController::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	if(!isScrolling&&!isSliding){//��û���л���
		isScrolling=true;
	}
	if(!isScrolling&&!isSliding) return;//����û���л���Ҳû��ʼ������ͼ�飬�򷵻�

	CCPoint loc=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	CCPoint preLoc=CCDirector::sharedDirector()->convertToGL(pTouch->getPreviousLocationInView());
	CCPoint delta=ccp(loc.x-preLoc.x,0);

	//��ͼ���ƶ�
	if(isScrolling&&isMovingMap){
		CCPoint adjustPos=ccpAdd(scrollView->getContentOffset(),delta);
		if(curPage==nCount&&adjustPos.x<-curPage*scrollViewWidth_2) return;
		if(curPage==1&&adjustPos.x>scrollViewWidth_2) return;
		scrollView->setContentOffset(adjustPos);
	}

	//Ũ�ȵ������ƶ�
	if(isSliding){
		CCSprite* slider=
			(CCSprite*)this->getChildByTag(TAG_LEFT_TABLE)->getChildByTag(TAG_LEFT_MID_PANEL)->getChildByTag(TAG_SLIDER);
		CCPoint slidePos=ccpAdd(slider->getPosition(),delta);

		if(slidePos.x>slideStart&&slidePos.x<slideEnd){
			slider->setPosition(slidePos);
		}
	}
}
void GameMenuController::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
	CCPoint endPoint=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	
	//�ж��Ƿ��ǵ���¼�
	if(!isScrolling&&!isSliding){
		CCSprite* sprite=(CCSprite*)scrollView->getContainer()->getChildByTag(curPage);//��õ�ǰҳ��ͼƬ
		//������ת��ΪGL����ϵ,��ת��Ϊ���container������
		CCPoint touchPoint=((CCLayer*)scrollView->getContainer())->convertToNodeSpace(endPoint);
		if(sprite->boundingBox().containsPoint(touchPoint)){//�������ǰҳ��
			CCLog("clicked:%d",curPage);
		}else{//û�������ǰҳ
			CCLog("shake curPage:%d",curPage);
		}
	}
	if(isScrolling&&isMovingMap){	
		isMovingMap=false;
		float distance=endPoint.x-startPoint.x;
		if(distance!=0){
			adjustScrollView(distance);
		}
	}else if(isSliding){
		isSliding=false;
	}
	
}
void GameMenuController::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){

}

void GameMenuController::adjustScrollView(float distance){

	scrollView->unscheduleAllSelectors();

	if(distance<0){//����
		curPage++;
		if(curPage>nCount) curPage=nCount;
	}else if(distance>0){//����
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
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(2,GameWelcomeController::scene()));
}
void GameMenuController::menuCollectCallback(CCObject* pSender){
	CCLog("go to collections page");
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(2,GameCollectionController::scene()));
}
void GameMenuController::menuClickCallback(CCObject* pSender){
	CCLog("click");
}

void GameMenuController::menuShopCallback(CCObject* pSender){//����̵갴ť�ص�
	CCLog("click shop button");
}
void GameMenuController::menuStartCallback(CCObject* pSender){//�����ʼ��ť�ص�
	CCLog("click start button");
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GamePlayController::scene()));
}

void GameMenuController::menuChooseResinCallback(CCObject* pSender){
	//ѡ������ѡ�
	if(curPanel==((CCNode*)pSender)->getTag()) return;//ѡ�е�ǰҳ
	
	curPanel=(Panel)((CCNode*)pSender)->getTag();
	CCSprite* panel=(CCSprite*)this->getChildByTag(TAG_LEFT_TABLE)->getChildByTag(TAG_LEFT_MID_PANEL)->getChildByTag(TAG_PANEL_COLOR);

	switch(curPanel){//���ݵ������ѡ�
	case panel_red:
		panel->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("panel_red.png"));
		break;
	case panel_yellow:
		panel->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("panel_yellow.png"));
		break;
	case panel_green:
		panel->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("panel_green.png"));
		break;
	case panel_blue:
		panel->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("panel_blue.png"));
		break;
	}
}

void GameMenuController::onEnter(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,1,true);
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