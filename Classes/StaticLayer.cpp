#include "StaticLayer.h"
#include "VisibleRect.h"
#include "Constant.h"

USING_NS_CC;
USING_NS_CC_EXT;

StaticLayer::StaticLayer()
{
}


StaticLayer::~StaticLayer()
{
}
//调试用
void StaticLayer::ballGValueChanged(CCObject* pSender,CCControlEvent event){
	CCControlSlider* slider=(CCControlSlider*)this->getChildByTag(921);
	CCLabelTTF* ttf=(CCLabelTTF*)this->getChildByTag(922);
	ttf->setString(CCString::createWithFormat("ball_G:%.02f",slider->getValue())->getCString());
	model->BALL_GRAVITY=slider->getValue();
}
void StaticLayer::ballUValueChanged(CCObject* pSender,CCControlEvent event){
	CCControlSlider* slider=(CCControlSlider*)this->getChildByTag(821);
	CCLabelTTF* ttf=(CCLabelTTF*)this->getChildByTag(822);
	ttf->setString(CCString::createWithFormat("ball_u:%.02f",slider->getValue())->getCString());
	model->BALL_FRICTION=slider->getValue();
}
void StaticLayer::ballMaxVValueChanged(CCObject* pSender,CCControlEvent event){
	CCControlSlider* slider=(CCControlSlider*)this->getChildByTag(721);
	CCLabelTTF* ttf=(CCLabelTTF*)this->getChildByTag(722);
	ttf->setString(CCString::createWithFormat("ball_MaxV:%.02f",slider->getValue())->getCString());
	model->MAX_VELOCITY=slider->getValue();
}
void StaticLayer::blockGValueChanged(CCObject* pSender,CCControlEvent event){
	CCControlSlider* slider=(CCControlSlider*)this->getChildByTag(621);
	CCLabelTTF* ttf=(CCLabelTTF*)this->getChildByTag(622);
	ttf->setString(CCString::createWithFormat("block_G:%.02f",slider->getValue())->getCString());
	model->BLOCK_GRAVITY=slider->getValue();
}
void StaticLayer::blockUValueChanged(CCObject* pSender,CCControlEvent event){
	CCControlSlider* slider=(CCControlSlider*)this->getChildByTag(521);
	CCLabelTTF* ttf=(CCLabelTTF*)this->getChildByTag(522);
	ttf->setString(CCString::createWithFormat("block_u:%.02f",slider->getValue())->getCString());
	model->BLOCK_FRICTION=slider->getValue();
}
void StaticLayer::initControlSlider(GameResinBallModel* model){
	this->model=model;
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	CCControlSlider* ball_g=CCControlSlider::create("sliderBg.png","sliderProgress.png","sliderThumb.png");
	ball_g->setPosition(winSize.width/2,winSize.height*0.95f);
	ball_g->setMaximumValue(500);
	ball_g->setMinimumValue(10);
	ball_g->setValue(model->BALL_GRAVITY);
	ball_g->addTargetWithActionForControlEvents(this,cccontrol_selector(StaticLayer::ballGValueChanged),CCControlEventValueChanged);
	addChild(ball_g,0,921);

	CCLabelTTF *str_ball_g=CCLabelTTF::create("","Helvetica",20);
	str_ball_g->setPosition(ccp(winSize.width/2,winSize.height*0.98f));
	str_ball_g->setString(CCString::createWithFormat("ball_G:%.02f",ball_g->getValue())->getCString());
	addChild(str_ball_g,0,922);
	//球摩擦系数
	CCControlSlider* ball_u=CCControlSlider::create("sliderBg.png","sliderProgress.png","sliderThumb.png");
	ball_u->setPosition(winSize.width/2,winSize.height*0.9f);
	ball_u->setMaximumValue(3);
	ball_u->setMinimumValue(0);
	ball_u->setValue(model->BALL_FRICTION);
	ball_u->addTargetWithActionForControlEvents(this,cccontrol_selector(StaticLayer::ballUValueChanged),CCControlEventValueChanged);
	addChild(ball_u,0,821);

	CCLabelTTF *str_ball_u=CCLabelTTF::create("","Helvetica",20);
	str_ball_u->setPosition(ccp(winSize.width/2,winSize.height*0.93f));
	str_ball_u->setString(CCString::createWithFormat("ball_u:%.02f",ball_u->getValue())->getCString());
	addChild(str_ball_u,0,822);
	//球最大速度
	CCControlSlider* ball_maxV=CCControlSlider::create("sliderBg.png","sliderProgress.png","sliderThumb.png");
	ball_maxV->setPosition(winSize.width/2,winSize.height*0.85f);
	ball_maxV->setMaximumValue(100);
	ball_maxV->setMinimumValue(1);
	ball_maxV->setValue(model->MAX_VELOCITY);
	ball_maxV->addTargetWithActionForControlEvents(this,cccontrol_selector(StaticLayer::ballMaxVValueChanged),CCControlEventValueChanged);
	addChild(ball_maxV,0,721);

	CCLabelTTF *str_ball_maxV=CCLabelTTF::create("","Helvetica",20);
	str_ball_maxV->setPosition(ccp(winSize.width/2,winSize.height*0.88f));
	str_ball_maxV->setString(CCString::createWithFormat("ball_maxV:%.02f",ball_maxV->getValue())->getCString());
	addChild(str_ball_maxV,0,722);
	//小方块重力
	CCControlSlider* block_g=CCControlSlider::create("sliderBg.png","sliderProgress.png","sliderThumb.png");
	block_g->setPosition(winSize.width/2,winSize.height*0.80f);
	block_g->setMaximumValue(100);
	block_g->setMinimumValue(0);
	block_g->setValue(model->BLOCK_GRAVITY);
	block_g->addTargetWithActionForControlEvents(this,cccontrol_selector(StaticLayer::blockGValueChanged),CCControlEventValueChanged);
	addChild(block_g,0,621);

	CCLabelTTF *str_block_g=CCLabelTTF::create("","Helvetica",20);
	str_block_g->setPosition(ccp(winSize.width/2,winSize.height*0.83f));
	str_block_g->setString(CCString::createWithFormat("block_G:%.02f",block_g->getValue())->getCString());
	addChild(str_block_g,0,622);
	//小方块摩擦系数
	CCControlSlider* block_u=CCControlSlider::create("sliderBg.png","sliderProgress.png","sliderThumb.png");
	block_u->setPosition(winSize.width/2,winSize.height*0.75f);
	block_u->setMaximumValue(3);
	block_u->setMinimumValue(0);
	block_u->setValue(model->BLOCK_FRICTION);
	block_u->addTargetWithActionForControlEvents(this,cccontrol_selector(StaticLayer::blockUValueChanged),CCControlEventValueChanged);
	addChild(block_u,0,521);

	CCLabelTTF *str_block_u=CCLabelTTF::create("","Helvetica",20);
	str_block_u->setPosition(ccp(winSize.width/2,winSize.height*0.78f));
	str_block_u->setString(CCString::createWithFormat("block_u:%.02f",block_g->getValue())->getCString());
	addChild(str_block_u,0,522);
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

	//	this->initControlSlider();

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