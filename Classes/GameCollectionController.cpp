#include "GameCollectionController.h"
#include "Constant.h"
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

void  WStrToUTF8(std::string& dest, const wstring& src){
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
		winSize=CCDirector::sharedDirector()->getWinSize();

		CCMenuItemImage* pStartItem=CCMenuItemImage::create("start.png","start.png",this,menu_selector(GameCollectionController::menuStartCallback));
		CC_BREAK_IF(!pStartItem);
		pStartItem->setPosition(winSize.width/2,winSize.height/4);

		CCMenu* menu=CCMenu::create(pStartItem,NULL);
		menu->setPosition(CCPointZero);
		//addChild(menu);

		CCSprite* Collection_bg=CCSprite::create("map.png");
		Collection_bg->setAnchorPoint(ccp(1,0));
		Collection_bg->setPosition(ccp(winSize.width/5,0));
		addChild(Collection_bg,-1);

		rightWidth=winSize.width*4.0f/5;

		//垂直滑动列表
		ListViewLayer* listViewLayer=ListViewLayer::create();
		listViewLayer->setTag(TAG_LIST);
		listViewLayer->setContentSize(CCSizeMake(rightWidth,winSize.height));
		listViewLayer->setPosition(ccp(winSize.width/5,0));
		addChild(listViewLayer);
		
		//下侧琥珀加成栏
		amberAddLayer=this->initbottomLayer();
		this->addChild(amberAddLayer,1);

		//左侧图标
		CCSprite* sprite=this->getCurSprite(preIdx);
		sprite->setTag(TAG_CUR_AMBER);
		sprite->setAnchorPoint(ccp(0.5,1));
		sprite->setPosition(ccp((winSize.width-rightWidth)/2,winSize.height*0.8f));
		addChild(sprite);
		CCSprite* sprite_movable=this->getCurSprite(preIdx);
		sprite_movable->setTag(TAG_CUR_AMBER_MOVABLE);
		sprite_movable->setAnchorPoint(ccp(0.5,1));
		sprite_movable->setPosition(ccp((winSize.width-rightWidth)/2,winSize.height*0.8f));
		amberAddLayer->addChild(sprite_movable,2);
		sprite_movable->setVisible(false);
		//左侧说明文字
		   std::string pString = WStrToUTF8(L"据说在map2中经常见到");
		//CCString *pString = CCString::createWithFormat("据说在map2中经常见到");
		CCLabelTTF *pLabel = CCLabelTTF::create(pString.c_str(), "AppleGothic", 20.0);
        pLabel->setAnchorPoint(ccp(0.5,1));
		pLabel->setPosition(ccp((winSize.width-rightWidth)/2,winSize.height*0.5f));
        pLabel->setTag(123);
        this->addChild(pLabel);


		this->scheduleUpdate();
        pRet = true;
    }while(0);
    return pRet;
}
CCSprite* GameCollectionController::getCurSprite(unsigned int idx){
	CCSprite* sprite;
	switch(idx){
		case 0:
			sprite=CCSprite::createWithSpriteFrameName("mantis_1.png");break;
		case 1:
			sprite=CCSprite::createWithSpriteFrameName("mantis_2.png");break;
		case 2:
			sprite=CCSprite::createWithSpriteFrameName("mantis_3.png");break;
		case 3:
			sprite=CCSprite::createWithSpriteFrameName("mantis_4.png");break;
		default:
			sprite=CCSprite::create("CloseNormal.png");break;
	}
	return sprite;
}

CCLayer* GameCollectionController::initbottomLayer(){//初始化下侧的layer,即琥珀加成槽
	CCLayer* amberAddLayer=CCLayer::create();

	//底色
	CCSprite* bg=CCSprite::create("listitem2.png");
	bg->setAnchorPoint(ccp(0.5,0));
	bg->setPosition(ccp(amberAddLayer->getContentSize().width/2,0));
	amberAddLayer->addChild(bg);
	//琥珀加成四个字
	 std::string pString = WStrToUTF8(L"琥珀");
	CCLabelTTF *pLabel = CCLabelTTF::create(pString.c_str(), "AppleGothic", 20.0);
    pLabel->setAnchorPoint(CCPointZero);
	pLabel->setPosition(ccp(200,pLabel->getContentSize().height*2));
    pLabel->setTag(123);
    amberAddLayer->addChild(pLabel);
	
	pString = WStrToUTF8(L"加成");
	CCLabelTTF *pLabel2 = CCLabelTTF::create(pString.c_str(), "AppleGothic", 20.0);
    pLabel2->setAnchorPoint(CCPointZero);
	pLabel2->setPosition(ccp(200,pLabel->getContentSize().height));
    pLabel2->setTag(123);
    amberAddLayer->addChild(pLabel2);

	//三个槽
	CCSprite* slots=CCSprite::createWithSpriteFrameName("skillslots.png");
	slots->setAnchorPoint(CCPointZero);
	slots->setPosition(ccp(pLabel->getPositionX()+pLabel->getContentSize().width,0));
	amberAddLayer->addChild(slots);

	return amberAddLayer;
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
		
		sprite=(CCSprite*)amberAddLayer->getChildByTag(TAG_CUR_AMBER_MOVABLE);
		sprite->setDisplayFrame(this->getCurSprite(idx)->displayFrame());
	}
	
}
bool GameCollectionController::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	CCPoint startPoint=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());

	if(startPoint.x>winSize.width-rightWidth) return false;

	CCSprite* amber=(CCSprite*)amberAddLayer->getChildByTag(TAG_CUR_AMBER_MOVABLE);
	if(amber->boundingBox().containsPoint(startPoint)){
		isMoving=true;
		amber->setVisible(true);
	}
	
	return true;
}
void GameCollectionController::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	if(!isMoving) return;
	CCPoint loc=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	
	CCSprite* amber=(CCSprite*)amberAddLayer->getChildByTag(TAG_CUR_AMBER_MOVABLE);
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
	
	//判断是否是点击事件
	/*if(!isScrolling&&!isSliding){
		CCSprite* sprite=(CCSprite*)scrollView->getContainer()->getChildByTag(curPage);//获得当前页的图片
		//把坐标转换为GL坐标系,并转换为相对container的坐标
		CCPoint touchPoint=((CCLayer*)scrollView->getContainer())->convertToNodeSpace(endPoint);
		if(sprite->boundingBox().containsPoint(touchPoint)){//点击到当前页面
			CCLog("clicked:%d",curPage);
		}else{//没点击到当前页
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
	*/
}
void GameCollectionController::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){

}

void GameCollectionController::menuStartCallback(CCObject* pSender){
	//CCScene* gamePlay=GamePlayController::scene();
	//CCTransitionScene *scene=CCTransitionCrossFade::create(2,gamePlay);
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(2,GameMenuController::scene()));
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