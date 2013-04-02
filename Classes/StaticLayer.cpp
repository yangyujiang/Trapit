#include "StaticLayer.h"
#include "VisibleRect.h"

using namespace cocos2d;

USING_NS_CC;

StaticLayer::StaticLayer()
{
}


StaticLayer::~StaticLayer()
{
}

bool StaticLayer::init(){
	bool pRet=false;
	do{ 
		// 先调用超类的init方法
        CC_BREAK_IF(! CCLayer::init());
		 // Create a "close" menu item with close icon, it's an auto release object.
        pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(StaticLayer::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);
        // Place the menu item bottom-right conner.
		pCloseItem->setPosition(ccpSub(VisibleRect::rightBottom(),ccp(pCloseItem->getContentSize().width,-pCloseItem->getContentSize().height)));


		pPauseItem=CCMenuItemImage::create("pause_normal.png","pause_selected.png",this,menu_selector(StaticLayer::menuPauseCallBack));
		CC_BREAK_IF(!pPauseItem);
		pPauseItem->setPosition(ccp(VisibleRect::bottom().x-pPauseItem->getContentSize().width/2,
			VisibleRect::bottom().y+pPauseItem->getContentSize().height/2));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem,pPauseItem,NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

		pRet=true;
	}while(0);

	return pRet;
}
void StaticLayer::draw(){
	ccDrawLine(ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.left,innerStage.top)),
		ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.right,innerStage.top)));
	ccDrawLine(ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.left,innerStage.bottom)),
		ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.right,innerStage.bottom)));
	ccDrawLine(ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.left,innerStage.top)),
		ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.left,innerStage.bottom)));
	ccDrawLine(ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.right,innerStage.top)),
		ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.right,innerStage.bottom)));
}

void StaticLayer::keepStill(CCPoint lastPos){
	CCScene* scene=CCDirector::sharedDirector()->getRunningScene();
	CCPoint scenePos=scene->getPosition();
	CCPoint delta=ccpSub(scenePos,lastPos);
	this->setPosition(ccpSub(this->getPosition(),delta));
}

void StaticLayer::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}
void StaticLayer::menuPauseCallBack(CCObject* pSender){
	if(!CCDirector::sharedDirector()->isPaused()){
		CCDirector::sharedDirector()->pause();
	}else CCDirector::sharedDirector()->resume();
}