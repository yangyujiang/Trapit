#include "GameCollectionController.h"
#include "VisibleRect.h"
#include "GamePlayController.h"
#include "GameMenuController.h"
#include "../extensions/cocos-ext.h"
#include "ListViewLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)   
#include "vld.h"   
#endif 

USING_NS_CC;
USING_NS_CC_EXT;


void  WStrToUTF8(std::string& dest, const std::wstring& src){
	dest.clear();
	for (size_t i = 0; i < src.size(); i++){
		wchar_t w = src[i];
		if (w <= 0x7f)
			dest.push_back((char)w);
		else if (w <= 0x7ff){
			dest.push_back(0xc0 | ((w >> 6)& 0x1f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0xffff){
			dest.push_back(0xe0 | ((w >> 12)& 0x0f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (sizeof(wchar_t) > 2 && w <= 0x10ffff){
			dest.push_back(0xf0 | ((w >> 18)& 0x07)); // wchar_t 4-bytes situation
			dest.push_back(0x80| ((w >> 12) & 0x3f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else
			dest.push_back('?');
	}
}

//! simple warpper
std::string WStrToUTF8(const std::wstring& str){
	std::string result;
	WStrToUTF8(result, str);
	return result;
}

GameCollectionController::GameCollectionController():preIdx(0),isMoving(false)
{
}

GameCollectionController::~GameCollectionController()
{	
	
}

bool GameCollectionController::init(){
	bool pRet = false;

    do{
        // 先调用超类的init方法
		CC_BREAK_IF(! CCLayer::init());
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("insects.plist","insects.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("leftPanel.plist","leftPanel.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ambers.plist","ambers.png");
		winSize=CCDirector::sharedDirector()->getWinSize();
		
		listWidth=winSize.width*4.0f/5;

		CCSprite* Collection_bg=CCSprite::create("map.png");
		Collection_bg->setAnchorPoint(ccp(0,0));
		Collection_bg->setPosition(ccp(listWidth,0));
		addChild(Collection_bg,-1);

		CCSprite* topTitle=this->initTopTitle();
		topTitle->setAnchorPoint(ccp(0.5,1));
		topTitle->setPosition(ccp(winSize.width/2,winSize.height));
		this->addChild(topTitle,1);
		
		//下侧琥珀加成栏
		amberAddLayer=this->initbottomLayer();
		amberAddLayer->setAnchorPoint(ccp(0.5,0));
		amberAddLayer->setPosition(ccp(winSize.width/2,0));
		this->addChild(amberAddLayer,0);

		//垂直滑动列表
		ListViewLayer* listViewLayer=ListViewLayer::create();
		listViewLayer->initList(listWidth,winSize.height-topTitle->getContentSize().height-amberAddLayer->getContentSize().height);
		listViewLayer->setTag(TAG_LIST);
	//	listViewLayer->getChildByTag(TAG_COLLECTIONLIST)->setContentSize(CCSizeMake(listWidth,winSize.height-topTitle->getContentSize().height));
		listViewLayer->setPosition(ccp(0,amberAddLayer->getContentSize().height));
		addChild(listViewLayer);

		//技能图标
		CCSprite* sprite=this->getCurSprite(preIdx);
		sprite->setTag(TAG_CUR_AMBER);
		sprite->setAnchorPoint(ccp(0.5,1));
		sprite->setPosition(ccp((winSize.width-listWidth)/2+listWidth,winSize.height*0.8f));
		addChild(sprite);
		CCSprite* sprite_movable=this->getCurSprite(preIdx);
		sprite_movable->setTag(TAG_CUR_AMBER_MOVABLE);
		sprite_movable->setAnchorPoint(ccp(0.5,1));
		sprite_movable->setPosition(ccp((winSize.width-listWidth)/2+listWidth,winSize.height*0.8f));
		addChild(sprite_movable,2);
		sprite_movable->setVisible(false);

		//技能说明文字
		   std::string pString = WStrToUTF8(L"技能介绍：\n  使用后，所有昆虫\n会进入攻击状态。\n当心你的手指！");
		//CCString *pString = CCString::createWithFormat("据说在map2中经常见到");
		CCLabelTTF *pLabel = CCLabelTTF::create(pString.c_str(), "AppleGothic", 20.0);
        pLabel->setAnchorPoint(ccp(0.5,1));
		pLabel->setPosition(ccp((winSize.width-listWidth)/2+listWidth,winSize.height*0.5f));
        pLabel->setTag(123);
        this->addChild(pLabel);

		//Amber::getAmbersFromSQLite();
		
		//返回按钮
		CCSprite* goback=CCSprite::createWithSpriteFrameName("goback.png");
		CCMenuItemSprite* btnGoBack=CCMenuItemSprite::create(goback,goback,goback,this,menu_selector(GameCollectionController::menuGoBackCallback));
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
CCSprite* GameCollectionController::initTopTitle(){
	CCSprite* title=CCSprite::create("listitem_title.png");

	CCLabelTTF* ttf_title=CCLabelTTF::create("Collections","Thonburi",40.0);
	ttf_title->setAnchorPoint(ccp(0.5,0.5));
	ttf_title->setPosition(ccp(title->getContentSize().width/2,title->getContentSize().height/2));
	title->addChild(ttf_title);

	return title;
}

CCSprite* GameCollectionController::initAmplificationIcon(const char* name,float amplification){
	CCSprite* bg=CCSprite::create();
	bg->setContentSize(CCSizeMake(200,50));

	CCLabelTTF* ttf_name=CCLabelTTF::create(name,"AppleGothic",40);
	ttf_name->setAnchorPoint(ccp(0,0.5));
	ttf_name->setPosition(ccp(0,bg->getContentSize().height/2));
	bg->addChild(ttf_name);
	CCSprite* star=CCSprite::createWithSpriteFrameName("stars_.png");
	CCSprite* stars=CCSprite::create("stars_.png",CCRect(0,0,star->getContentSize().width*amplification/3,star->getContentSize().height));//:createWithSpriteFrameName("stars_.png");
	stars->setAnchorPoint(ccp(0,0.5));
	stars->setPosition(ccp(ttf_name->getPositionX()+ttf_name->getContentSize().width,bg->getContentSize().height/2));
	bg->addChild(stars);

	bg->setContentSize(CCSizeMake(ttf_name->getContentSize().width+star->getContentSize().width,50));

	return bg;
}

CCSprite* GameCollectionController::getCurSprite(unsigned int idx){
	CCSprite* sprite;
	switch(idx){
		case 0:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 1:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 2:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 3:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 4:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 5:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 6:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 7:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 8:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 9:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 10:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 11:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 12:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 13:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 14:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 15:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 16:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 17:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 18:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		default:sprite= CCSprite::create("amber_1.png");break;
	}
	return sprite;
}

CCSprite* GameCollectionController::initbottomLayer(){//初始化下侧的layer,即琥珀加成槽
	
	//底色
	CCSprite* bg=CCSprite::create("listitem2.png");

	//琥珀加成四个字
	 std::string pString = WStrToUTF8(L"琥珀\n加成");
	CCLabelTTF *pLabel = CCLabelTTF::create(pString.c_str(), "AppleGothic", 50.0);
    pLabel->setAnchorPoint(ccp(0,0.5));
	pLabel->setPosition(ccp(20,bg->getContentSize().height*0.5f));
    bg->addChild(pLabel);

	skillSlot=SkillSlotAdapter::create();
	skillSlot->setAnchorPoint(ccp(0,0.5));
	skillSlot->setPosition(ccp(pLabel->getPositionX()+pLabel->getContentSize().width+20,bg->getContentSize().height/2));
	bg->addChild(skillSlot);

	std::string name=WStrToUTF8(L"速度：");
	CCSprite* veliIcon=this->initAmplificationIcon(name.c_str(),1.5);
	veliIcon->setAnchorPoint(ccp(0,0.5));
	veliIcon->setPosition(ccp(skillSlot->getPositionX()+skillSlot->getContentSize().width,bg->getContentSize().height*0.75));
	bg->addChild(veliIcon);

	name=WStrToUTF8(L"控制：");
	CCSprite* conIcon=this->initAmplificationIcon(name.c_str(),2.5);
	conIcon->setAnchorPoint(ccp(0,0.5));
	conIcon->setPosition(ccp(skillSlot->getPositionX()+skillSlot->getContentSize().width,bg->getContentSize().height*0.25));
	bg->addChild(conIcon);

	name=WStrToUTF8(L"黏性：");
	CCSprite* nianIcon=this->initAmplificationIcon(name.c_str(),3);
	nianIcon->setAnchorPoint(ccp(0,0.5));
	nianIcon->setPosition(ccp(conIcon->getPositionX()+conIcon->getContentSize().width,bg->getContentSize().height*0.25));
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
	unsigned int idx=((ListViewLayer*)this->getChildByTag(TAG_LIST))->getIdx();
	if(idx!=preIdx){
		preIdx=idx;
		CCSprite* sprite=(CCSprite*)this->getChildByTag(TAG_CUR_AMBER);
		sprite->setDisplayFrame(this->getCurSprite(idx)->displayFrame());
		
		sprite=(CCSprite*)this->getChildByTag(TAG_CUR_AMBER_MOVABLE);
		sprite->setDisplayFrame(this->getCurSprite(idx)->displayFrame());
	}
	
}
bool GameCollectionController::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	CCPoint startPoint=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());

	if(startPoint.x<listWidth) return false;

	CCSprite* amber=(CCSprite*)this->getChildByTag(TAG_CUR_AMBER_MOVABLE);
	if(amber->boundingBox().containsPoint(startPoint)){
		isMoving=true;
		amber->setVisible(true);
	}
	
	return true;
}
void GameCollectionController::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	if(!isMoving) return;
	CCPoint loc=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	
	CCSprite* amber=(CCSprite*)this->getChildByTag(TAG_CUR_AMBER_MOVABLE);
	if(loc.x<winSize.width&&loc.x>amber->getContentSize().width/2){
		/*if(CCAction* moveTo=amber->getActionByTag(TAG_MOVETO)){//设置平滑移动
			if(!moveTo->isDone()) return;
			else CCLog("runACtion");
		}
		float distance=ccpDistance(amber->getPosition(),loc);
		if(distance>50){
			CCAction* moveTo=CCMoveTo::create(distance/1000,loc);
			moveTo->setTag(TAG_MOVETO);
			amber->runAction(moveTo);
		}else*/
		amber->setPosition(ccp(loc.x,loc.y+amber->getContentSize().height/2));
		
	}else if(loc.y<winSize.height-amber->getContentSize().height){
		if(loc.x>winSize.width) amber->setPositionX(winSize.width-amber->getContentSize().width/2);
		else if(loc.x<amber->getContentSize().width/2) amber->setPositionX(amber->getContentSize().width/2);
		amber->setPositionY(loc.y+amber->getContentSize().height/2);
	}
	
}
void GameCollectionController::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
	CCPoint endPoint=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	isMoving=false;
	
	CCSprite *amber=(CCSprite*)this->getChildByTag(TAG_CUR_AMBER_MOVABLE);
	amber->setVisible(false);

	//若是拖到了技能栏，则加入到技能栏中
	skillSlot->addSkillIfOverLap(amber);

	CCSprite *amber_=(CCSprite*)this->getChildByTag(TAG_CUR_AMBER);
	amber->setPosition(amber_->getPosition());

}
void GameCollectionController::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){

}


void GameCollectionController::menuGoBackCallback(CCObject* pSender){
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GameMenuController::scene()));
}


void GameCollectionController::onEnter(){
	
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,1,true);
	CCLayer::onEnter();
}
void GameCollectionController::onEnterTransitionDidFinish(){
	CCLayer::onEnterTransitionDidFinish();
}
void GameCollectionController::onExitTransitionDidStart(){
	CCLayer::onExitTransitionDidStart();
}
void GameCollectionController::onExit(){
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}