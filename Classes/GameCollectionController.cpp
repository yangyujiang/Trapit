#include "GameCollectionController.h"
#include "VisibleRect.h"
#include "GamePlayController.h"
#include "GameMenuController.h"
#include "../extensions/cocos-ext.h"
#include "ListViewLayer.h"
#include "MyMath.h"
#include "GlobalComponent.h"
#include "LanguageAdapter.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)   
#include "vld.h"   
#endif 

USING_NS_CC;
USING_NS_CC_EXT;

const int TAG_SKILL_DESCRIP=123321;//技能说明文字
const int TAG_SKILLNAME=321321;//技能名称
const int TAG_SKILLBOX=12345;//技能框

GameCollectionController::GameCollectionController():preIdx(0),isMoving(false)
{
}

GameCollectionController::~GameCollectionController()
{	
	//_slots->removeAllObjects();
}

bool GameCollectionController::init(){
	bool pRet = false;

	do{
		// 先调用超类的init方法
		CC_BREAK_IF(! CCLayer::init());
		winSize=CCDirector::sharedDirector()->getWinSize();

		listWidth=winSize.width*4.0f/5;

		CCSprite* Collection_bg=CCSprite::create("menu_bg.png");
		Collection_bg->setAnchorPoint(ccp(0,0));
		Collection_bg->setPosition(ccp(0,0));
		this->addChild(Collection_bg);

		CCSprite* topTitle=this->initTopTitle();
		topTitle->setAnchorPoint(ccp(0.5,1));
		topTitle->setPosition(ccp(winSize.width/2,winSize.height));
		this->addChild(topTitle,1);

		//下侧琥珀加成栏
		amberAddLayer=this->initbottomLayer();
		amberAddLayer->setAnchorPoint(ccp(0.5,0));
		amberAddLayer->setPosition(ccp(winSize.width/2,0));
		this->addChild(amberAddLayer,2);

		//垂直滑动列表
		ListViewLayer* listViewLayer=ListViewLayer::create();
		listViewLayer->initList(listWidth,winSize.height-topTitle->getContentSize().height-amberAddLayer->getContentSize().height,20,CCSizeMake(960,180));
		listViewLayer->setTag(TAG_LIST);
		//	listViewLayer->getChildByTag(TAG_COLLECTIONLIST)->setContentSize(CCSizeMake(listWidth,winSize.height-topTitle->getContentSize().height));
		listViewLayer->setPosition(ccp(0,amberAddLayer->getContentSize().height));
		addChild(listViewLayer);

		this->initSkillColumn();//初始化技能展示栏

		/*	std::vector<Amber*> ambers=Amber::getAmbersFromSQLite();
		const char* skillName=ambers[0]->getDescrip();
		CCLog("%s",skillName);
		CCString *pString = CCString::createWithFormat("%s",skillName);
		CCLabelTTF *pLabel = CCLabelTTF::create(pString->getCString(), "AppleGothic", 60.0);
		pLabel->setAnchorPoint(ccp(0.5,1));
		pLabel->setPosition(ccp((winSize.width)/2,winSize.height*0.5f));
		pLabel->setTag(123);
		this->addChild(pLabel,10);*/

		//返回按钮
		CCMenuItem* btnGoBack=GlobalComponent::createMenuItem(MENU_GOBACK,this,menu_selector(GameCollectionController::menuGoBackCallback));
		//CCSprite* goback=CCSprite::createWithSpriteFrameName("btn_goback.png");
		//CCMenuItemSprite* btnGoBack=CCMenuItemSprite::create(goback,goback,goback,this,menu_selector(GameCollectionController::menuGoBackCallback));
		btnGoBack->setAnchorPoint(CCPointZero);
		btnGoBack->setPosition(CCPointZero);
		CCMenu* menu=CCMenu::create(btnGoBack,NULL);
		menu->setPosition(CCPointZero);
		this->addChild(menu);

		this->scheduleUpdate();
		pRet = true;
	}while(0);
	return pRet;
}
void GameCollectionController::initSkillColumn(){//技能栏 
	//技能、成就分隔栏
	CCSprite* skill_split=CCSprite::createWithSpriteFrameName("skill_split.png");
	skill_split->setAnchorPoint(ccp(0.7f,0.5f));
	skill_split->setPosition(ccp(listWidth,winSize.height/2));
	this->addChild(skill_split);

	//技能栏（竖栏）x轴起始位置
	const int skillColumn_start=skill_split->getPositionX()+skill_split->getContentSize().width*(1-skill_split->getAnchorPoint().x);
	CCLog("skillColumn_start:%d",skillColumn_start);
	CCLog("listWidth:%d",listWidth);
	//技能框
	CCSprite* skill_box=CCSprite::createWithSpriteFrameName("slot.png");
	skill_box->setScale(1.5f);//放大1.5倍
	skill_box->setAnchorPoint(ccp(0.5,0.5));
	skill_box->setPosition(ccp((winSize.width-skillColumn_start)/2+skillColumn_start,winSize.height*0.75f));
	this->addChild(skill_box,0,TAG_SKILLBOX);

	//技能图标
	CCSprite* sprite=this->getCurSprite(preIdx);
	sprite->setTag(TAG_CUR_AMBER);
	sprite->setAnchorPoint(ccp(0.5,0.5));
	sprite->setPosition(skill_box->getPosition());
	addChild(sprite);
/*	CCSprite* sprite_movable=this->getCurSprite(preIdx);
	sprite_movable->setTag(TAG_CUR_AMBER_MOVABLE);
	sprite_movable->setAnchorPoint(sprite->getAnchorPoint());
	sprite_movable->setPosition(sprite->getPosition());
	addChild(sprite_movable,2);
	sprite_movable->setVisible(false);*/
	movableSkill=this->getCurSprite(preIdx);
	movableSkill->setAnchorPoint(sprite->getAnchorPoint());
	movableSkill->setPosition(sprite->getPosition());
	addChild(movableSkill,2);
	movableSkill->setVisible(false);

	CCLabelTTF *label_skillName=CCLabelTTF::create(__skillName[0],"Arial",30.f);
	label_skillName->setAnchorPoint(ccp(0.5f,0.5f));
	label_skillName->setPosition(ccp(skill_box->getContentSize().width/2,-label_skillName->getContentSize().height/2));
	skill_box->addChild(label_skillName,2,TAG_SKILLNAME);

	//技能说明文字
	std::string pString =__skillDescrip[0];//skillDescrip[0];//"技能介绍：\n  使用后，所有昆虫\n会进入攻击状态。\n当心你的手指";
	//"技能介绍：\n  使用后，所有昆虫\n会进入攻击状态。\n当心你的手指！";
	//   std::string pString = MyMath::WStrToUTF8(L"技能介绍：\n  使用后，所有昆虫\n会进入攻击状态。\n当心你的手指！");
	//CCString *pString = CCString::createWithFormat("据说在map2中经常见到");
	CCLabelTTF *pLabel = CCLabelTTF::create(pString.c_str(), "Arial", 20.0);
	pLabel->setAnchorPoint(ccp(0.5,1));
	pLabel->setPosition(ccp((winSize.width-listWidth)/2+listWidth,winSize.height*0.5f));
	pLabel->setTag(TAG_SKILL_DESCRIP);
	this->addChild(pLabel);
}
CCSprite* GameCollectionController::initTopTitle(){
	/*	CCSprite* title=CCSprite::create("listitem_title.png");

	CCLabelTTF* ttf_title=CCLabelTTF::create("Collections","Thonburi",40.0);
	ttf_title->setAnchorPoint(ccp(0.5,0.5));
	ttf_title->setPosition(ccp(title->getContentSize().width/2,title->getContentSize().height/2));
	title->addChild(ttf_title);*/
	CCSprite* title=CCSprite::createWithSpriteFrameName("collection_top.png");

	return title;
}

CCSprite* GameCollectionController::initAmplificationIcon(const char* name,float amplification){
	CCSprite* bg=CCSprite::create();
	bg->setContentSize(CCSizeMake(200,50));

	CCLabelTTF* ttf_name=CCLabelTTF::create(name,"AppleGothic",20);
	ttf_name->setAnchorPoint(ccp(0,0.5));
	ttf_name->setPosition(ccp(0,bg->getContentSize().height/2));
	bg->addChild(ttf_name);
	CCSprite* star=CCSprite::createWithSpriteFrameName("stars_three_small.png");
	
	CCSprite* stars=CCSprite::create("stars_three_small.png",CCRect(0,0,star->getContentSize().width*amplification/3,star->getContentSize().height));//:createWithSpriteFrameName("stars_.png");
	stars->setAnchorPoint(ccp(0,0.5));
	stars->setPosition(ccp(ttf_name->getPositionX()+ttf_name->getContentSize().width,bg->getContentSize().height/2));
	bg->addChild(stars);

	bg->setContentSize(CCSizeMake(ttf_name->getContentSize().width+star->getContentSize().width,50));

	return bg;
}

CCSprite* GameCollectionController::getCurSprite(unsigned int idx){
	CCSprite* sprite;
	switch(idx){
	case 0:sprite= CCSprite::createWithSpriteFrameName("skill_1.png");break;
	case 1:sprite= CCSprite::createWithSpriteFrameName("skill_2.png");break;
	case 2:sprite= CCSprite::createWithSpriteFrameName("skill_3.png");break;
	default:sprite= CCSprite::createWithSpriteFrameName("skill_1.png");break;
	}
	sprite->setScale(1.5f);
	return sprite;
}

CCSprite* GameCollectionController::initbottomLayer(){//初始化下侧的layer,即琥珀加成槽

	//底色
	CCSprite* bg=CCSprite::createWithSpriteFrameName("collection_bottom.png");

	//琥珀加成四个字
	/*	 std::string pString = WStrToUTF8(L"琥珀\n加成");
	CCLabelTTF *pLabel = CCLabelTTF::create(pString.c_str(), "AppleGothic", 50.0);
	pLabel->setAnchorPoint(ccp(0,0.5));
	pLabel->setPosition(ccp(20,bg->getContentSize().height*0.5f));
	bg->addChild(pLabel);*/

	skillSlot=SkillSlotAdapter::create();
	skillSlot->setAnchorPoint(ccp(0,0.5));
	skillSlot->setPosition(ccp(20,bg->getContentSize().height*0.5f));
	bg->addChild(skillSlot);

	std::string name=variable_vel;//"速度：";
	//std::string name=MyMath::WStrToUTF8(L"速度：");
	CCSprite* veliIcon=this->initAmplificationIcon(name.c_str(),1.5);
	veliIcon->setAnchorPoint(ccp(0,0.5));
	veliIcon->setPosition(ccp(skillSlot->getPositionX()+skillSlot->getContentSize().width+50,bg->getContentSize().height*0.75f));
	bg->addChild(veliIcon);

	name=variable_contro;//"控制";
	//	name=MyMath::WStrToUTF8(L"控制：");
	CCSprite* conIcon=this->initAmplificationIcon(name.c_str(),2.5);
	conIcon->setAnchorPoint(ccp(0,0.5));
	conIcon->setPosition(ccp(skillSlot->getPositionX()+skillSlot->getContentSize().width+50,bg->getContentSize().height*0.5f));
	bg->addChild(conIcon);

	name=variable_nian;//"黏性";
	//name=MyMath::WStrToUTF8(L"黏性：");
	CCSprite* nianIcon=this->initAmplificationIcon(name.c_str(),3);
	nianIcon->setAnchorPoint(ccp(0,0.5));
	nianIcon->setPosition(ccp(skillSlot->getPositionX()+skillSlot->getContentSize().width+50,bg->getContentSize().height*0.25f));
	bg->addChild(nianIcon);

	/*pString = WStrToUTF8(L"速度+");
	CCLabelTTF *pLabelV = CCLabelTTF::create(pString.c_str(), "AppleGothic", 40.0);
	pLabelV->setAnchorPoint(ccp(0,0.5));
	pLabelV->setPosition(ccp(((CCSprite*)_slots->lastObject())->getPositionX()+
	((CCSprite*)_slots->lastObject())->getContentSize().width+20,bg->getContentSize().height*0.75f));
	bg->addChild(pLabelV);*/

	return bg;
	//this->addChild(amberAddLayer,1);
}

CCScene* GameCollectionController::scene(){
	CCScene *scene = NULL;
	do{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		GameCollectionController *layer = GameCollectionController::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);

	}while(0);

	return scene;
}

void GameCollectionController::update(float dt){
	ListViewLayer* list=((ListViewLayer*)this->getChildByTag(TAG_LIST));
	CCTableViewAdvanced* listview=((CCTableViewAdvanced*)list->getChildByTag(TAG_COLLECTIONLIST));
	CCPoint offSet=listview->getContentOffset();
	float dis=offSet.y-list->getScrollPoint().y;
	if(dis!=0){//在滚动中计算选中的item
		int id=list->numberOfCellsInTableView(NULL)+offSet.y/list->cellSizeForTable(NULL).height-2;
		list->setScrollPoint(offSet);
		list->setIdx(id);//设置选中的itemID并标记选中
	}

	unsigned int idx=list->getIdx();
	if(idx!=preIdx){
		preIdx=idx;
		CCSprite* sprite=(CCSprite*)this->getChildByTag(TAG_CUR_AMBER);
		sprite->setDisplayFrame(this->getCurSprite(idx)->displayFrame());

		//sprite=(CCSprite*)this->getChildByTag(TAG_CUR_AMBER_MOVABLE);
		movableSkill->setDisplayFrame(this->getCurSprite(idx)->displayFrame());

		//技能说明文字改变
		CCLabelTTF *label_skillDescrip=(CCLabelTTF*)this->getChildByTag(TAG_SKILL_DESCRIP);
		label_skillDescrip->setString(__skillDescrip[idx]);

		CCLabelTTF *label_skillName=(CCLabelTTF*)this->getChildByTag(TAG_SKILLBOX)->getChildByTag(TAG_SKILLNAME);
		label_skillName->setString(__skillName[idx]);
	}

}
bool GameCollectionController::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	CCPoint startPoint=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());

	if(startPoint.x<listWidth) return false;

	//CCSprite* amber=(CCSprite*)this->getChildByTag(TAG_CUR_AMBER_MOVABLE);
	if(movableSkill->boundingBox().containsPoint(startPoint)){
		isMoving=true;
		movableSkill->setVisible(true);
	}

	return true;
}
void GameCollectionController::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	if(!isMoving) return;
	CCPoint loc=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());

//	CCSprite* amber=(CCSprite*)this->getChildByTag(TAG_CUR_AMBER_MOVABLE);
	if(loc.x<winSize.width&&loc.x>movableSkill->getContentSize().width/2){
		movableSkill->setPosition(loc);

	}else if(loc.y<winSize.height-movableSkill->getContentSize().height){
		if(loc.x>winSize.width) movableSkill->setPositionX(winSize.width-movableSkill->getContentSize().width/2);
		else if(loc.x<movableSkill->getContentSize().width/2) movableSkill->setPositionX(movableSkill->getContentSize().width/2);
		movableSkill->setPositionY(loc.y);
	}

}
void GameCollectionController::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
	CCPoint endPoint=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	isMoving=false;

//	CCSprite *amber=(CCSprite*)this->getChildByTag(TAG_CUR_AMBER_MOVABLE);
	movableSkill->setVisible(false);
	movableSkill->setTag(((ListViewLayer*)this->getChildByTag(TAG_LIST))->getIdx());

	//若是拖到了技能栏，则加入到技能栏中
	int amberId=skillSlot->addSkillIfOverLap(movableSkill);
	if(amberId>0){
		char name[20]={0};
		sprintf(name,"used_id_%d",amberId);
		ListViewLayer* list=(ListViewLayer*)this->getChildByTag(TAG_LIST);
		CCUserDefault::sharedUserDefault()->setIntegerForKey(name,list->getIdx());
		CCUserDefault::sharedUserDefault()->flush();
	}
	//位置复原
	movableSkill->setPosition(this->getChildByTag(TAG_CUR_AMBER)->getPosition());

}
void GameCollectionController::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){

}


void GameCollectionController::menuGoBackCallback(CCObject* pSender){
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GameMenuController::scene()));
}

void GameCollectionController::keyBackClicked(){
	CCLog("key back clicked");
	this->menuGoBackCallback(this);
}


void GameCollectionController::onEnter(){
	CCLog("Collection:onEnter");
	this->setKeypadEnabled(true);

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,1,true);
	CCLayer::onEnter();
}
void GameCollectionController::onEnterTransitionDidFinish(){
	CCLog("Collection:onEnterTransitionDidFinish");
	CCLayer::onEnterTransitionDidFinish();
}
void GameCollectionController::onExitTransitionDidStart(){
	CCLog("Collection:onExitTransitionDidStart");
	CCLayer::onExitTransitionDidStart();
}
void GameCollectionController::onExit(){
	CCLog("Collection:onExit");
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
//	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("menu.plist");
	//CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("insects.plist");
	//CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("ambers.plist");
	//CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("collections.plist");
	CCLayer::onExit();
}