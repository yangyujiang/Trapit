#include "GameMenuController.h"
#include "Constant.h"
#include "VisibleRect.h"
#include "GamePlayController.h"
#include "GameWelcomeController.h"
#include "GameCollectionController.h"
#include "SmartRes.h"
#include "ListViewLayer.h"
#include "cPolySprite.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)   
#include "vld.h"   
#endif 

USING_NS_CC;
USING_NS_CC_EXT;

const int TAG_LEFT_TABLE=100;
const int TAG_LEFT_MID_PANEL=101;
const int TAG_PANEL_COLOR=102;
const int TAG_SLIDE=103;//滑块
const int TAG_MENUBG=104;//menuBg
const int TAG_MAPBG=105;
const int TAG_MAP_LIST=106;//小地图图标列表（垂直）
const int TAG_LEVEL_SELECT=107;

const unsigned int DELTA_RESIN=10;//每到时间新产生的树脂量
const unsigned int INTERVAL=10*60;//*60;//倒计时设为10分钟

const char *LAST_TIME="LAST_TIME";//key 上次退出游戏时系统时间

long millisecondNow()  
{ 
struct cc_timeval now; 
CCTime::gettimeofdayCocos2d(&now, NULL); 
return (now.tv_sec * 1000 + now.tv_usec / 1000); 
}
long secondNow()  
{ 
struct cc_timeval now; 
CCTime::gettimeofdayCocos2d(&now, NULL); 
return (now.tv_sec); 
}

GameMenuController::GameMenuController():
space(100),curPage(1),nCount(3),isScrolling(false),isSliding(false),
	isMovingMap(false),str_time(NULL),needToChange(false)
{
	str_time=new char[10];
	countDown=INTERVAL;
}


GameMenuController::~GameMenuController()
{	
	CC_SAFE_DELETE(str_time);
}

void timeToChar(float time,char* &str){//把float型的时间转化为xx:xx形式的字符串
	unsigned int hour=time/60;
	float second=time-hour*60;

	//char str[10]={0};
	if(hour<10&&second<10)
		sprintf(str, "0%d:0%.0f", hour,second);	
	else if(hour<10&&second>=10)
		sprintf(str, "0%d:%.0f", hour,second);
	else if(hour>=10&&second<10)
		sprintf(str, "%d:0%.0f", hour,second);
	else if(hour>=10&&second>=10)
		sprintf(str, "%d:%.0f", hour,second);
	CCLog("%s",str);
}

bool GameMenuController::init(){
	bool pRet = false;

    do{
        // 先调用超类的init方法
		CC_BREAK_IF(! CCLayer::init());
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("menu.plist","menu.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("menuMap.plist","menuMap.png");
	winSize=CCDirector::sharedDirector()->getWinSize();
	

	CCSprite* menuBg=CCSprite::create("menu_bg.png");
	menuBg->setTag(TAG_MENUBG);
	menuBg->setAnchorPoint(ccp(0,0));
	menuBg->setPosition(ccp(0,0));
	this->addChild(menuBg);

	//this->initMapScrollView();//初始化右侧面板
	this->initResinVol();
	CCLayer* leftTable=this->initLeftTable();//初始化左侧面板
	menuBg->addChild(leftTable);
	CCSprite* rightTable=this->initRightTable();
	rightTable->setTag(TAG_MAPBG);
	menuBg->addChild(rightTable);
	rightTable->setAnchorPoint(CCPointZero);
	rightTable->setPosition(ccp(tableWidth,0));
	focusView=resin;//当前聚焦在调节树脂浓度的页面

	schedule(schedule_selector(GameMenuController::step), 1.0f);

	this->setKeypadEnabled(true);

        pRet = true;
    }while(0);
    return pRet;
}
CCMenu* GameMenuController::initLeftBottomdPanel(){
	CCSprite* goback=CCSprite::createWithSpriteFrameName("btn_goback.png");
	CCMenuItem* pGoBackItem=CCMenuItemSprite::create(goback,goback,goback,this,menu_selector(GameMenuController::menuGoBackCallback));
	pGoBackItem->setAnchorPoint(ccp(0,0));
	pGoBackItem->setScale(CCDirector::sharedDirector()->getContentScaleFactor());
	pGoBackItem->setPosition(0,0);

	CCSprite* collection=CCSprite::createWithSpriteFrameName("btn_collection.png");
    CCMenuItem *pCollectItem = CCMenuItemSprite::create(collection,collection,collection,
                                        this,
                                        menu_selector(GameMenuController::menuCollectCallback));
	pCollectItem->setAnchorPoint(ccp(0.5,0));
	pCollectItem->setScale(CCDirector::sharedDirector()->getContentScaleFactor());
	pCollectItem->setPosition(tableWidth/2,0);

	CCSprite* wiki=CCSprite::createWithSpriteFrameName("btn_wiki.png");
	CCMenuItem *btnWiki=CCMenuItemSprite::create(wiki,wiki,wiki,this,menu_selector(GameMenuController::menuWikiCallback));
	btnWiki->setAnchorPoint(ccp(1,0));
	btnWiki->setPosition(tableWidth,0);

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pGoBackItem,pCollectItem,btnWiki, NULL);

	
	return pMenu;
}
CCLayer* GameMenuController::initLeftMidPanel(){//初始化左侧中间部分
	CCLayer* leftPanel=CCLayer::create();

	CCSprite* panel_color=CCSprite::createWithSpriteFrameName("panel_red.png");
	panel_color->setTag(TAG_PANEL_COLOR);
	panel_color->setAnchorPoint(CCPointZero);
	leftPanel->setContentSize(panel_color->getContentSize());
	leftPanel->addChild(panel_color);

	CCSize panelSize=panel_color->getContentSize();//获得panel的大小
	//红色琥珀按钮
	CCSprite* sprite=CCSprite::createWithSpriteFrameName("btn_red.png");
	CCMenuItemSprite* btnRed=CCMenuItemSprite::create(
		sprite,sprite,sprite,this,menu_selector(GameMenuController::menuChooseResinCallback));
	btnRed->setTag(panel_red);
	btnRed->setAnchorPoint(ccp(0.5,1));
	btnRed->setPosition(ccp(panelSize.width*10.0f/31,panelSize.height*30.f/31));
	//黄色琥珀按钮
	sprite=CCSprite::createWithSpriteFrameName("btn_yellow.png");
	CCMenuItemSprite* btnYellow=CCMenuItemSprite::create(
		sprite,sprite,sprite,this,menu_selector(GameMenuController::menuChooseResinCallback));
	btnYellow->setTag(panel_yellow);
	btnYellow->setAnchorPoint(ccp(0.5,1));
	btnYellow->setPosition(ccp(panelSize.width*15.0f/31,panelSize.height*30.f/31));
	//绿色琥珀按钮
	sprite=CCSprite::createWithSpriteFrameName("btn_green.png");
	CCMenuItemSprite* btnGreen=CCMenuItemSprite::create(
		sprite,sprite,sprite,this,menu_selector(GameMenuController::menuChooseResinCallback));
	btnGreen->setTag(panel_green);
	btnGreen->setAnchorPoint(ccp(0.5,1));
	btnGreen->setPosition(ccp(panelSize.width*21.0f/31,panelSize.height*30.f/31));
	//蓝色琥珀按钮
	sprite=CCSprite::createWithSpriteFrameName("btn_blue.png");
	CCMenuItemSprite* btnBlue=CCMenuItemSprite::create(
		sprite,sprite,sprite,this,menu_selector(GameMenuController::menuChooseResinCallback));
	btnBlue->setTag(panel_blue);
	btnBlue->setAnchorPoint(ccp(0.5,1));
	btnBlue->setPosition(ccp(panelSize.width*27.0f/31,panelSize.height*30.f/31));
	//创建按钮菜单
	CCMenu* pMenu=CCMenu::create(btnRed,btnYellow,btnGreen,btnBlue,NULL);
	pMenu->setAnchorPoint(CCPointZero);
	pMenu->setPosition(CCPointZero);
	leftPanel->addChild(pMenu,1);

	//星星
	CCSprite* stars=CCSprite::createWithSpriteFrameName("stars_three.png");
	stars->setAnchorPoint(ccp(0,0.5));
	stars->setPosition(ccp(panelSize.width*9.5f/31,panelSize.height*7.2f/17));
	panel_color->addChild(stars);

	CCSprite* stars2=CCSprite::createWithSpriteFrameName("stars_three.png");
	stars2->setAnchorPoint(ccp(0,0.5));
	stars2->setPosition(ccp(panelSize.width*9.5f/31,panelSize.height*5.3f/17));
	panel_color->addChild(stars2);

	CCSprite* stars3=CCSprite::createWithSpriteFrameName("stars_three.png");
	stars3->setAnchorPoint(ccp(0,0.5));
	stars3->setPosition(ccp(panelSize.width*9.5f/31,panelSize.height*3.4f/17));
	panel_color->addChild(stars3);
	
	//技能槽

	//树脂、水浓度调整条
	slide_resin=CCSprite::createWithSpriteFrameName("slide_resin.png");
	slide_resin->setAnchorPoint(ccp(0.5,0.5));
	slide_resin->setPosition(ccp(panelSize.width*18.f/31,panelSize.height*11.f/17));
	panel_color->addChild(slide_resin);
	
	slide_water=CCSprite::createWithSpriteFrameName("slide_water.png");
	slide_water->setAnchorPoint(CCPointZero);
	slide_water->setPosition(CCPointZero);
	slide_resin->addChild(slide_water);

	//滑块
	slider=CCSprite::createWithSpriteFrameName("slider_ant.png");
	slider->setAnchorPoint(ccp(0.5,0.5f));
	slider->setPosition(ccp(slide_resin->getContentSize().width/2,slide_resin->getContentSize().height/2));
	slide_resin->addChild(slider,1);
	slide_resin->setTag(TAG_SLIDE);
	
	//初始化滑块最小位置，和可滑动最远距离
	slideStart=slider->getContentSize().width/2;
	slideEnd=slide_resin->getContentSize().width-slider->getContentSize().width/2;

	this->updateSlideWater();

	return leftPanel;

}
void GameMenuController::updateSlideWater(){//更新滑动条
	CCRect textureRect          = slide_water->getTextureRect();
	textureRect                 = CCRectMake(textureRect.origin.x, textureRect.origin.y, slider->getPositionX(), textureRect.size.height);
    slide_water->setTextureRect(textureRect, slide_water->isTextureRectRotated(), textureRect.size);
}

CCLayer* GameMenuController::initLeftTopPanel(){	
	CCLayer* leftTopLayer=CCLayer::create();

	CCSprite* column_dollar=CCSprite::createWithSpriteFrameName("col_dollar.png");
	CCSprite* column_amber=CCSprite::createWithSpriteFrameName("col_resin.png");
	CCSprite* column_clock=CCSprite::createWithSpriteFrameName("col_clock.png");

	CCSprite* shop=CCSprite::createWithSpriteFrameName("btn_shop.png");
	CCMenuItemSprite* btnShop=CCMenuItemSprite::create(shop,shop,shop,this,menu_selector(GameMenuController::menuShopCallback));
	
	//左上面板大小
	leftTopLayer->setContentSize(CCSizeMake(winSize.width,column_dollar->getContentSize().height));

	leftTopLayer->addChild(column_dollar);
	column_dollar->setAnchorPoint(CCPointZero);
	column_dollar->setPosition(CCPointZero);
	unsigned int dollar=CCUserDefault::sharedUserDefault()->getIntegerForKey(DOLLAR,ORIGIN_DOLLAR);
	char str_dollar[10]={0};
	sprintf(str_dollar,"%d",dollar);
	CCLabelBMFont *ttf_dollar=CCLabelBMFont::create(str_dollar, "fonts/bitmapFontTest3.fnt");//CCLabelTTF::create(CCString::createWithFormat("498")->getCString(),"AppleGothic", 30.0);
	ttf_dollar->setAnchorPoint(ccp(0.5,0.5));
	ttf_dollar->setPosition(ccp(column_dollar->getContentSize().width*2.2f/4,column_dollar->getContentSize().height/2));
	column_dollar->addChild(ttf_dollar);

	leftTopLayer->addChild(column_amber);
	column_amber->setAnchorPoint(ccp(1,0));
	column_amber->setPosition(ccp(leftTopLayer->getContentSize().width,0));
	
	ttf_resin=CCLabelBMFont::create("0", "fonts/bitmapFontTest3.fnt");//CCLabelTTF::create(CCString::createWithFormat("100")->getCString(),"AppleGothic", 30.0);
	ttf_resin->setAnchorPoint(ccp(0.5,0.5));
	char str[10] = {0};
    sprintf(str, "%d", cur_resin);
	ttf_resin->setString(str);
	ttf_resin->setPosition(ccp(column_amber->getContentSize().width*2.2f/4,column_amber->getContentSize().height/2));
	column_amber->addChild(ttf_resin);

	leftTopLayer->addChild(column_clock);
	column_clock->setAnchorPoint(ccp(1,0));
	column_clock->setPosition(ccp(column_amber->getPositionX()-column_amber->getContentSize().width,0));
	
	ttf_clock=CCLabelBMFont::create(str_time, "fonts/bitmapFontTest3.fnt");//CCLabelTTF::create(CCString::createWithFormat("10:00")->getCString(),"AppleGothic", 30.0);
	ttf_clock->setAnchorPoint(ccp(0,0.5));
	ttf_clock->setPosition(ccp(column_clock->getContentSize().width*2.2f/4-ttf_clock->getContentSize().width/2,column_clock->getContentSize().height/2));
	column_clock->addChild(ttf_clock);
	
	CCMenu* menuShop=CCMenu::create(btnShop,NULL);
	menuShop->setAnchorPoint(ccp(0,0.5));
	menuShop->setPosition(ccp(column_dollar->getPositionX()+column_dollar->getContentSize().width+100,
		column_dollar->getPositionY()+column_dollar->getContentSize().height/2));
	leftTopLayer->addChild(menuShop);

	return leftTopLayer;
}

void GameMenuController::initResinVol(){
	long now=secondNow();
	//上次退出时系统时间，若没有，则返回当前系统时间
	long last_time=CCUserDefault::sharedUserDefault()->getIntegerForKey(LAST_TIME,now);
	//获取上次存档时的树脂量，若没有则初始化为基础量
	cur_resin=CCUserDefault::sharedUserDefault()->getIntegerForKey(RESIN_VOLUME,ORIGIN_RESIN);
	
	//计算上次存档到此时的时间间隔，并据此计算出 应新产生的树脂量 和 当前倒计时的初始值
	long duration=now-last_time;
	if(duration>0){//非第一次进入
		unsigned int deltaCount=duration/INTERVAL;
		cur_resin+=DELTA_RESIN*deltaCount;//当前总树脂量
		CCUserDefault::sharedUserDefault()->setIntegerForKey(RESIN_VOLUME,cur_resin);
		CCUserDefault::sharedUserDefault()->flush();
		countDown=INTERVAL-(duration-INTERVAL*deltaCount);//当前倒计时初始值
	}else{//第一次进入
		countDown=INTERVAL;
	}
	timeToChar(countDown,str_time);
}

CCLayer* GameMenuController::initLeftTable(){	
	CCLayer* leftLayer=CCLayer::create();
	
	CCLayer* leftMidPanel=this->initLeftMidPanel();


	tableWidth=leftMidPanel->getContentSize().width;//左侧面板宽度
	scrollViewWidth_2=(winSize.width-tableWidth)/2;

	leftLayer->setTag(TAG_LEFT_TABLE);
	leftLayer->ignoreAnchorPointForPosition(false);
	leftLayer->setContentSize(CCSizeMake(tableWidth,winSize.height));
	leftLayer->setAnchorPoint(ccp(0,0));

	leftMidPanel->setTag(TAG_LEFT_MID_PANEL);
	leftMidPanel->ignoreAnchorPointForPosition(false);
	leftMidPanel->setAnchorPoint(ccp(0,0.5));
	//leftPanel->setScale(CCDirector::sharedDirector()->getContentScaleFactor());
	leftMidPanel->setPosition(ccp(0,leftLayer->getContentSize().height/2));
	leftLayer->addChild(leftMidPanel);

	leftTopPanel=this->initLeftTopPanel();
	leftTopPanel->ignoreAnchorPointForPosition(false);
	leftTopPanel->setAnchorPoint(ccp(0,1));
	leftTopPanel->setPosition(0,leftLayer->getContentSize().height);
	this->addChild(leftTopPanel,1);

	CCMenu* leftBottomPanel=this->initLeftBottomdPanel();
	leftBottomPanel->setAnchorPoint(CCPointZero);
	leftBottomPanel->setPosition(CCPointZero);
	leftLayer->addChild(leftBottomPanel);

    return leftLayer;
}

CCSprite* GameMenuController::initRightTable(){
	CCSprite* menuMapBg=CCSprite::create("menuMap_bg.png");
	CCSize mapBgSize=menuMapBg->getContentSize();

	CCSprite* panel_levelSelect=CCSprite::createWithSpriteFrameName("panel_levelselect.png");
	panel_levelSelect->setTag(TAG_LEVEL_SELECT);
	/*CCMenuItem* btn_levelSelect=CCMenuItemSprite::create(panel_levelSelect,panel_levelSelect,panel_levelSelect,
		this,menu_selector(GameMenuController::menuLevelSelectCallback));
	btn_levelSelect->setAnchorPoint(ccp(0,0.5));
	btn_levelSelect->setPosition(ccp(0,mapBgSize.height/2));
	CCMenu* menu=CCMenu::create(btn_levelSelect,NULL);
	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);
	menuMapBg->addChild(menu);*/
	panel_levelSelect->setAnchorPoint(ccp(0,0.5));
	panel_levelSelect->setPosition(ccp(0,mapBgSize.height/2));
	menuMapBg->addChild(panel_levelSelect);

	//场景选择小窗口（垂直列表）
	CCSize cellSize=CCSizeMake(254,313);
	ListMapVertical* listMapVertical=ListMapVertical::create();
	listMapVertical->initList(cellSize.width,winSize.height,3,cellSize);
	listMapVertical->setTag(TAG_MAP_LIST);
	listMapVertical->setPosition(ccp(panel_levelSelect->getContentSize().width,0));
	menuMapBg->addChild(listMapVertical);

	//场景选择Gallery
	scrollViewWidth_2=menuMapBg->getContentSize().width-panel_levelSelect->getContentSize().width-cellSize.width;

	CCLayer* containerLayer=CCLayer::create();
	CCSize containerSize=CCSizeMake(scrollViewWidth_2,winSize.height*0.95f);
	containerLayer->setContentSize(containerSize);

	CCSprite* scene_1=CCSprite::createWithSpriteFrameName("scene_1.png");
	space=(containerSize.width-scene_1->getContentSize().width);
	scene_1->setAnchorPoint(ccp(0,1));
	scene_1->setPosition(ccp(panel_levelSelect->getContentSize().width+cellSize.width+space/2,containerSize.height));
	
	const int sceneCount=3;
	CCSprite* scenes[sceneCount];
	for(int i=0;i<sceneCount;i++){
		char fileName[20]={0};
		sprintf(fileName,"scene_%d.png",(i+1));
		scenes[i]=CCSprite::createWithSpriteFrameName(fileName);
		scenes[i]->setTag(i+1);
		scenes[i]->setAnchorPoint(ccp(0.5,0.5));
		scenes[i]->setPosition(ccp(space/2+(space+scenes[i]->getContentSize().width)*i+scenes[i]->getContentSize().width/2,
			containerSize.height-scenes[i]->getContentSize().height/2));
		containerLayer->addChild(scenes[i]);
	}
	scrollView=CCScrollView::create(containerSize,containerLayer);
	scrollView->setDirection(kCCScrollViewDirectionHorizontal);
	scrollView->setTouchEnabled(false);
	scrollView->setPosition(ccp(panel_levelSelect->getContentSize().width+cellSize.width,0));//设置此scrollView的位置
	menuMapBg->addChild(scrollView);

	CCSprite* toleft=CCSprite::create("toleft.png");
	btn_toleft=CCMenuItemSprite::create(toleft,toleft,toleft,this,menu_selector(GameMenuController::menuToLeftCallback));
	btn_toleft->setAnchorPoint(ccp(0,0.5));
	btn_toleft->setPosition(ccp(panel_levelSelect->getContentSize().width+cellSize.width,
		containerSize.height-scenes[0]->getContentSize().height/2));
	btn_toleft->setEnabled(false);
	btn_toleft->setVisible(false);

	CCSprite* toright=CCSprite::create("toright.png");
	btn_toright=CCMenuItemSprite::create(toright,toright,toright,this,menu_selector(GameMenuController::menuToRightCallback));
	btn_toright->setAnchorPoint(ccp(1,0.5));
	btn_toright->setPosition(ccp(menuMapBg->getContentSize().width,
		containerSize.height-scenes[0]->getContentSize().height/2));

	CCMenu* menu_leftRight=CCMenu::create(btn_toleft,btn_toright,NULL);
	menu_leftRight->setAnchorPoint(CCPointZero);
	menu_leftRight->setPosition(CCPointZero);
	menuMapBg->addChild(menu_leftRight);

	CCSprite* play=CCSprite::create("btn_play.png");
	CCMenuItem* btn_play=CCMenuItemSprite::create(play,play,play,this,menu_selector(GameMenuController::menuStartCallback));
	btn_play->setAnchorPoint(ccp(0.5,0));
	btn_play->setPosition(ccp(panel_levelSelect->getContentSize().width+cellSize.width+containerSize.width/2,0));

	CCMenu* menu2=CCMenu::create(btn_play,NULL);
	menu2->setAnchorPoint(CCPointZero);
	menu2->setPosition(CCPointZero);
	menuMapBg->addChild(menu2);

	return menuMapBg;
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
	scrollView->setPosition(ccp(tableWidth,0));//设置此scrollView的位置
	addChild(scrollView);

	CCLabelTTF* pLabel = CCLabelTTF::create("CHOOSE MAP", "Thonburi", 34);
    pLabel->setPosition(ccp(tableWidth+(winSize.width-tableWidth)/2,winSize.height*3.f/4));
    this->addChild(pLabel, 1);
	
	//开始按钮
	CCSprite* start=CCSprite::createWithSpriteFrameName("btn_start.png");
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
	switch(focusView){
	case resin:
		if(startPoint.x<tableWidth){
			//点到左边
			if(slider->boundingBox().containsPoint(slide_resin->convertToNodeSpace(startPoint))){
				isSliding=true;//只要点中滑块，就标记为可以开始移动
			}
		}else needToChange=true;
		break;
	case map:
		CCSprite* mapBg=(CCSprite*)this->getChildByTag(TAG_MENUBG)->getChildByTag(TAG_MAPBG);
		CCPoint pos=mapBg->convertToNodeSpace(startPoint);
		if(pos.x>mapBg->getContentSize().width-scrollView->getContainer()->getContentSize().width){
			isMovingMap=true;
		}else if(pos.x<mapBg->getChildByTag(TAG_LEVEL_SELECT)->getContentSize().width) needToChange=true;
		break;
	}
	return true;
}
void GameMenuController::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	if(!isScrolling&&!isSliding){//若没点中滑块
		isScrolling=true;
	}
	if(!isScrolling&&!isSliding) return;//若既没点中滑块也没开始滑动地图块，则返回

	CCPoint loc=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	CCPoint preLoc=CCDirector::sharedDirector()->convertToGL(pTouch->getPreviousLocationInView());
	CCPoint delta=ccp(loc.x-preLoc.x,0);

	//地图块移动
	if(isScrolling&&isMovingMap){
		CCPoint adjustPos=ccpAdd(scrollView->getContentOffset(),delta);
		if(curPage==nCount&&adjustPos.x<-curPage*scrollViewWidth_2) return;
		if(curPage==1&&adjustPos.x>scrollViewWidth_2) return;
		scrollView->setContentOffset(adjustPos);
	}

	//浓度调整块移动
	if(isSliding){
		CCPoint slidePos=ccpAdd(slider->getPosition(),delta);

		if(slidePos.x>slideStart&&slidePos.x<slideEnd){
			slider->setPosition(slidePos);
			this->updateSlideWater();
		}
	}
}
void GameMenuController::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
	CCPoint endPoint=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	
	//判断是否是点击事件
	switch(focusView){
	case map:
		if(needToChange){
			needToChange=false;
			this->menuLevelSelectCallback(this);
		}
		else{
			if(!isScrolling&&!isSliding){
				CCSprite* sprite=(CCSprite*)scrollView->getContainer()->getChildByTag(curPage);//获得当前页的图片
				//把坐标转换为GL坐标系,并转换为相对container的坐标
				CCPoint touchPoint=((CCLayer*)scrollView->getContainer())->convertToNodeSpace(endPoint);
				if(sprite->boundingBox().containsPoint(touchPoint)){//点击到当前页面
					CCLog("clicked:%d",curPage);
				}else{//没点击到当前页
					CCLog("shake curPage:%d",curPage);
					CCActionInterval* shake=CCWaves::create(1,10,true,true,ccg(24,24),0.5);
						//CCShaky3D::create(15,false,ccg(24,24),0.5);
					CCActionInterval* shake_back=shake->reverse();
					CCCallFuncN *funCall=CCCallFuncN::create(this,callfuncN_selector(GameMenuController::afterShakeCallback));
					sprite->runAction(CCSequence::create(shake,shake_back,funCall,NULL));
				}
			}
			if(isScrolling&&isMovingMap){
				isMovingMap=false;
				float distance=endPoint.x-startPoint.x;
				if(distance!=0){
					adjustScrollView(distance);
				}
			}
		}
		break;
	case resin:
		if(needToChange){
			needToChange=false;
			this->menuLevelSelectCallback(this);
		}
		else if(isSliding){
			isSliding=false;
		}
		break;
	}
}
void GameMenuController::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){

}
void GameMenuController::afterShakeCallback(CCNode* pSender){	
	//CCSprite* map=(CCSprite*)(((CCAction*)pSender)->getTarget());
	CCSprite* map=(CCSprite*)scrollView->getContainer()->getChildByTag(curPage);//获得当前页的图片
	if(map->getGrid()!=NULL){
		map->setGrid(NULL);
	}
}
void GameMenuController::keyBackClicked(){
	CCLog("key back clicked");
	this->menuGoBackCallback(this);
}
void GameMenuController::keyMenuClicked(){
	CCLog("key menu clicked");
}

void GameMenuController::adjustScrollView(float distance){

	scrollView->unscheduleAllSelectors();

	if(distance<0){//右移
		curPage++;
		if(curPage>=nCount){
			curPage=nCount;
			btn_toright->setEnabled(false);
			btn_toright->setVisible(false);
		}else if(!btn_toleft->isEnabled()){
			btn_toleft->setEnabled(true);
			btn_toleft->setVisible(true);
		}
	}else if(distance>0){//左移
		curPage--;
		if(curPage<=1){
			curPage=1;
			btn_toleft->setEnabled(false);
			btn_toleft->setVisible(false);
		}else if(!btn_toright->isEnabled()){
			btn_toright->setEnabled(true);
			btn_toright->setVisible(true);
		}
	}

	CCPoint adjustPos=ccp(-(curPage-1)*(scrollViewWidth_2),0);
	
	scrollView->setContentOffset(adjustPos,true);
}

void GameMenuController::update(float dt){
	
}
void GameMenuController::step(float dt){
	if(countDown-dt<=0){//时间到
		countDown=INTERVAL;
		cur_resin+=DELTA_RESIN;
		char str[10]={0};
		sprintf(str,"%d",cur_resin);
		ttf_resin->setString(str);
	}
	else countDown-=dt;

	unsigned int hour=countDown/60;
	float second=countDown-hour*60;

	timeToChar(countDown,str_time);
    ttf_clock->setString( str_time );
}
void GameMenuController::menuToLeftCallback(CCObject* pSender){
	CCLog("click left");
	this->adjustScrollView(10);
}
void GameMenuController::menuToRightCallback(CCObject* pSender){
	CCLog("click right");
	this->adjustScrollView(-10);

}
void GameMenuController::menuGoBackCallback(CCObject* pSender){
	CCLog("go Back to welcome page");
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GameWelcomeController::scene()));
}
void GameMenuController::menuCollectCallback(CCObject* pSender){
	CCLog("go to collections page");
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GameCollectionController::scene()));
}
void GameMenuController::menuClickCallback(CCObject* pSender){
	CCLog("click");
}
void GameMenuController::menuWikiCallback(CCObject* pSender){
	CCLog("click wiki button");
}

void GameMenuController::menuShopCallback(CCObject* pSender){//点击商店按钮回调
	CCLog("click shop button");
}
void GameMenuController::menuStartCallback(CCObject* pSender){//点击开始按钮回调
	CCLog("click start button");
	int consumeResin=10;//此局消耗树脂
	if(this->cur_resin<consumeResin) return;
	this->save();
	CCUserDefault::sharedUserDefault()->setIntegerForKey(CONSUME_RESIN,10);
	CCUserDefault::sharedUserDefault()->flush();
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GamePlayController::scene()));
}
void GameMenuController::menuLevelSelectCallback(CCObject* pSender){//点击开始按钮回调
	CCLog("click LevelSelect button");
	CCSprite* menuBg=(CCSprite*)this->getChildByTag(TAG_MENUBG);
	CCSprite* menuMapBg=(CCSprite*)menuBg->getChildByTag(TAG_MAPBG);
	CCPoint mapPos=this->convertToNodeSpace(menuMapBg->getPosition());
	float width=mapPos.x+menuMapBg->getContentSize().width-winSize.width;
	CCPoint desPos;
	if(menuBg->getPositionX()<0){
		desPos=ccp(0,0);
		focusView=resin;
	}else{
		desPos=ccp(0-width,0);
		focusView=map;
	}
	menuBg->runAction(CCMoveTo::create(0.5,desPos));
	
}

void GameMenuController::menuChooseResinCallback(CCObject* pSender){
	//选择琥珀选项卡
	if(curPanel==((CCNode*)pSender)->getTag()) return;//选中当前页
	
	curPanel=(Panel)((CCNode*)pSender)->getTag();
	CCSprite* panel=(CCSprite*)this->getChildByTag(TAG_MENUBG)->getChildByTag(TAG_LEFT_TABLE)->getChildByTag(TAG_LEFT_MID_PANEL)->getChildByTag(TAG_PANEL_COLOR);

	switch(curPanel){//根据点击更换选项卡
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
	CCLog("MENU:onEnter");
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,1,true);
	CCLayer::onEnter();
}
void GameMenuController::onEnterTransitionDidFinish(){
	CCLog("MENU:onEnterTransitionDidFinish");
	CCLayer::onEnterTransitionDidFinish();
}
void GameMenuController::onExitTransitionDidStart(){
	CCLog("MENU:onExitTransitionDidStart");
	CCLayer::onExitTransitionDidStart();
}
void GameMenuController::onExit(){
	CCLog("MENU:onExit");
	this->save();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("menu.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("menuMap.plist");
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}
void GameMenuController::save(){
	//退出时把当前系统时间存档,减去当前倒计时已经消耗的时间
	CCUserDefault::sharedUserDefault()->setIntegerForKey(LAST_TIME,secondNow()-(INTERVAL-countDown));
	CCUserDefault::sharedUserDefault()->setIntegerForKey(RESIN_VOLUME,cur_resin);//当前树脂量存档
	CCUserDefault::sharedUserDefault()->flush();
}