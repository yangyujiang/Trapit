#include "StaticLayer.h"
#include "VisibleRect.h"
#include "Constant.h"
#include "GameMenuController.h"
#include "GlobalComponent.h"
#include "GamePlayController.h"
#include "GameOverController.h"

USING_NS_CC;
USING_NS_CC_EXT;

const int TAG_LEVELUP=12345;
const int TAG_GAMEOVER=12234;

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
	ball_g->setAnchorPoint(ccp(0,0.5));
	ball_g->setPosition(50,winSize.height*0.9f);
	ball_g->setMaximumValue(500);
	ball_g->setMinimumValue(10);
	ball_g->setValue(model->BALL_GRAVITY);
	ball_g->addTargetWithActionForControlEvents(this,cccontrol_selector(StaticLayer::ballGValueChanged),CCControlEventValueChanged);
	addChild(ball_g,0,921);

	CCLabelTTF *str_ball_g=CCLabelTTF::create("","Helvetica",20);
	str_ball_g->setAnchorPoint(ccp(0,0.5));
	str_ball_g->setPosition(ccp(50,winSize.height*0.95f));
	str_ball_g->setString(CCString::createWithFormat("ball_G:%.02f",ball_g->getValue())->getCString());
	addChild(str_ball_g,0,922);
	//球摩擦系数
	CCControlSlider* ball_u=CCControlSlider::create("sliderBg.png","sliderProgress.png","sliderThumb.png");
	ball_u->setPosition(50,winSize.height*0.8f);
	ball_u->setAnchorPoint(ccp(0,0.5));
	ball_u->setMaximumValue(1);
	ball_u->setMinimumValue(0);
	ball_u->setValue(model->BALL_FRICTION);
	ball_u->addTargetWithActionForControlEvents(this,cccontrol_selector(StaticLayer::ballUValueChanged),CCControlEventValueChanged);
	addChild(ball_u,0,821);

	CCLabelTTF *str_ball_u=CCLabelTTF::create("","Helvetica",20);
	str_ball_u->setAnchorPoint(ccp(0,0.5));
	str_ball_u->setPosition(ccp(50,winSize.height*0.85f));
	str_ball_u->setString(CCString::createWithFormat("ball_u:%.02f",ball_u->getValue())->getCString());
	addChild(str_ball_u,0,822);
	//球最大速度
	CCControlSlider* ball_maxV=CCControlSlider::create("sliderBg.png","sliderProgress.png","sliderThumb.png");
	ball_maxV->setPosition(50,winSize.height*0.7f);
	ball_maxV->setAnchorPoint(ccp(0,0.5));
	ball_maxV->setMaximumValue(100);
	ball_maxV->setMinimumValue(1);
	ball_maxV->setValue(model->MAX_VELOCITY);
	ball_maxV->addTargetWithActionForControlEvents(this,cccontrol_selector(StaticLayer::ballMaxVValueChanged),CCControlEventValueChanged);
	addChild(ball_maxV,0,721);

	CCLabelTTF *str_ball_maxV=CCLabelTTF::create("","Helvetica",20);
	str_ball_maxV->setAnchorPoint(ccp(0,0.5));
	str_ball_maxV->setPosition(ccp(50,winSize.height*0.75f));
	str_ball_maxV->setString(CCString::createWithFormat("ball_maxV:%.02f",ball_maxV->getValue())->getCString());
	addChild(str_ball_maxV,0,722);
	//小方块重力
	CCControlSlider* block_g=CCControlSlider::create("sliderBg.png","sliderProgress.png","sliderThumb.png");
	block_g->setPosition(50,winSize.height*0.6f);
	block_g->setAnchorPoint(ccp(0,0.5));
	block_g->setMaximumValue(10);
	block_g->setMinimumValue(0);
	block_g->setValue(model->BLOCK_GRAVITY);
	block_g->addTargetWithActionForControlEvents(this,cccontrol_selector(StaticLayer::blockGValueChanged),CCControlEventValueChanged);
	addChild(block_g,0,621);

	CCLabelTTF *str_block_g=CCLabelTTF::create("","Helvetica",20);
	str_block_g->setAnchorPoint(ccp(0,0.5));
	str_block_g->setPosition(ccp(50,winSize.height*0.65f));
	str_block_g->setString(CCString::createWithFormat("block_G:%.02f",block_g->getValue())->getCString());
	addChild(str_block_g,0,622);
	//小方块摩擦系数
	CCControlSlider* block_u=CCControlSlider::create("sliderBg.png","sliderProgress.png","sliderThumb.png");
	block_u->setPosition(50,winSize.height*0.5f);
	block_u->setAnchorPoint(ccp(0,0.5));
	block_u->setMaximumValue(3);
	block_u->setMinimumValue(0);
	block_u->setValue(model->BLOCK_FRICTION);
	block_u->addTargetWithActionForControlEvents(this,cccontrol_selector(StaticLayer::blockUValueChanged),CCControlEventValueChanged);
	addChild(block_u,0,521);

	CCLabelTTF *str_block_u=CCLabelTTF::create("","Helvetica",20);
	str_block_u->setAnchorPoint(ccp(0,0.5));
	str_block_u->setPosition(ccp(50,winSize.height*0.55f));
	str_block_u->setString(CCString::createWithFormat("block_u:%.02f",block_g->getValue())->getCString());
	addChild(str_block_u,0,522);
}

bool StaticLayer::init(){
	bool pRet=false;
	do{ 
		// 先调用超类的init方法
        CC_BREAK_IF(! CCLayer::init());
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		 // Create a "close" menu item with close icon, it's an auto release object.
        pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(StaticLayer::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);
        // Place the menu item bottom-right conner.
		pCloseItem->setPosition(ccpSub(VisibleRect::rightBottom(),ccp(pCloseItem->getContentSize().width,-pCloseItem->getContentSize().height)));

	/*	CCSprite* freezeBox=CCSprite::createWithSpriteFrameName("slot.png");
		freezeBox->setPosition(ccp(winSize.width/2,freezeBox->getContentSize().height/2));
		this->addChild(freezeBox);

		CCMenuItem* freezeItem=GlobalComponent::createMenuItem(MENU_FREEZE,this,menu_selector(StaticLayer::menuFreezeCallBack));
		CC_BREAK_IF(!freezeItem);
		//freezeItem->setPosition(ccp(winSize.width/2,freezeItem->getContentSize().height/2));
		freezeItem->setPosition(ccp(freezeBox->getContentSize().width/2,freezeBox->getContentSize().height/2));

		CCMenu* menu=CCMenu::create(freezeItem,NULL);
		menu->setAnchorPoint(ccp(0,0));
		menu->setPosition(ccp(0,0));
		freezeBox->addChild(menu);*/

	//	pPauseItem=CCMenuItemImage::create("pause_normal.png","pause_selected.png",this,menu_selector(StaticLayer::menuPauseCallBack));
		pPauseItem=GlobalComponent::createMenuItem(MENU_PAUSE,this,menu_selector(StaticLayer::menuPauseCallBack));
		CC_BREAK_IF(!pPauseItem);
		//pPauseItem->setPosition(ccp(VisibleRect::bottom().x-pPauseItem->getContentSize().width/2,
		//	VisibleRect::bottom().y+pPauseItem->getContentSize().height/2));
		pPauseItem->setPosition(ccp(winSize.width-pPauseItem->getContentSize().width/2,pPauseItem->getContentSize().height/2));

		//CCMenuItemImage *pGoBackItem=CCMenuItemImage::create("goback.png","goback.png",this,menu_selector(StaticLayer::menuGoBackCallBack));
		CCMenuItem* pGoBackItem=GlobalComponent::createMenuItem(MENU_GOBACK,this,menu_selector(StaticLayer::menuGoBackCallBack));
		CC_BREAK_IF(!pGoBackItem);
		pGoBackItem->setScale(CCDirector::sharedDirector()->getContentScaleFactor());
		pGoBackItem->setPosition(ccp(VisibleRect::leftBottom().x+pGoBackItem->getContentSize().width/2,
			VisibleRect::leftBottom().y+pGoBackItem->getContentSize().height/2));

        // Create a menu with the "close" menu item, it's an auto release object.
       // CCMenu* pMenu = CCMenu::create(pCloseItem,pPauseItem,pGoBackItem,NULL);
		CCMenu* pMenu=CCMenu::create(pPauseItem,NULL);
		pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

		ttf_resin=CCLabelBMFont::create("300", "fonts/bitmapFontTest3.fnt");
		ttf_resin->setAnchorPoint(ccp(0.5,1));
		ttf_resin->setPosition(ccp(winSize.width/2,winSize.height));
		//addChild(ttf_resin,1);

		
		pRet=true;
	}while(0);

	return pRet;
}

void StaticLayer::playLevelUp(int curLevel){//播放升级动画
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	char str_level[10]={0};
	sprintf(str_level,"Level %d",curLevel);
	CCLabelBMFont *ttf_level=CCLabelBMFont::create(str_level, "fonts/bitmapFontTest3.fnt");
	ttf_level->setAnchorPoint(ccp(0.5,0.5));
	ttf_level->setPosition(ccp(winSize.width/2,winSize.height*9.f/11));
	ttf_level->setOpacity(0);
	this->addChild(ttf_level,0,TAG_LEVELUP);

	ttf_level->setScale(2);
	CCActionInterval* fadeIn=CCFadeIn::create(0.5f);
	CCActionInterval* scaleBy=CCEaseBounceOut::create(CCScaleBy::create(1,1.5));
	CCFiniteTimeAction* spawn=CCSpawn::create(fadeIn,scaleBy,NULL);
	
//	CCActionInterval* fadeOut=CCFadeOut::create(0.5f);
//	CCActionInterval* scaleBy2=CCScaleBy::create(1,2);
//	CCFiniteTimeAction* spawn_reverse=CCSpawn::create(fadeOut,scaleBy2,NULL);
	CCCallFunc* callFunc=CCCallFunc::create(this,callfunc_selector(StaticLayer::endPlayLevelUp));
	CCFiniteTimeAction* sequence=CCSequence::create(spawn,callFunc,NULL);
	ttf_level->runAction(sequence);
}
void StaticLayer::endPlayLevelUp(){
	this->removeChildByTag(TAG_LEVELUP,false);
}
void StaticLayer::endPlayGameOver(){
	this->removeChildByTag(TAG_GAMEOVER,false);
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GameOverController::scene()));
}
void StaticLayer::playGameOver(){
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();

	CCLabelBMFont *ttf_gameOver=CCLabelBMFont::create("Game Over", "fonts/bitmapFontTest3.fnt");
	ttf_gameOver->setAnchorPoint(ccp(0.5,0.5));
	ttf_gameOver->setPosition(ccp(winSize.width/2,winSize.height*5.5f/11));
	ttf_gameOver->setOpacity(0);
	this->addChild(ttf_gameOver,0,TAG_GAMEOVER);

	ttf_gameOver->setScale(2);
	CCActionInterval* fadeIn=CCFadeIn::create(0.5f);
	CCActionInterval* scaleBy=CCEaseBounceOut::create(CCScaleBy::create(1,1.5));
	CCFiniteTimeAction* spawn=CCSpawn::create(fadeIn,scaleBy,NULL);
	
	CCCallFunc* callFunc=CCCallFunc::create(this,callfunc_selector(StaticLayer::endPlayGameOver));
	CCFiniteTimeAction* sequence=CCSequence::create(spawn,callFunc,NULL);
	ttf_gameOver->runAction(sequence);
}
void StaticLayer::setResinCount(GameResinBallModel* resin){
	char str[10]={0};
	sprintf(str,"%d",(int)resin->getBlood());
	ttf_resin->setString(str);
}
void StaticLayer::draw(){
/*	ccDrawLine(ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.left,innerStage.top)),
		ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.right,innerStage.top)));
	ccDrawLine(ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.left,innerStage.bottom)),
		ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.right,innerStage.bottom)));
	ccDrawLine(ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.left,innerStage.top)),
		ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.left,innerStage.bottom)));
	ccDrawLine(ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.right,innerStage.top)),
		ccpAdd(VisibleRect::leftBottom(),ccp(innerStage.right,innerStage.bottom)));*/
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
void StaticLayer::menuGoBackCallBack(CCObject* pSender)
{
   CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,GameMenuController::scene()));
}
void StaticLayer::menuFreezeCallBack(CCObject* pSender){//使用冰冻器
	GamePlayController* game=(GamePlayController*)CCDirector::sharedDirector()->getRunningScene();
	game->playFreeze();
}
void StaticLayer::menuPauseCallBack(CCObject* pSender){
	if(!CCDirector::sharedDirector()->isPaused()){
		CCDirector::sharedDirector()->pause();
		CCSprite* resume=CCSprite::createWithSpriteFrameName("btn_resume.png");
		CCSprite* resume_selected=CCSprite::createWithSpriteFrameName("btn_resume.png");
		resume_selected->setScale(1.1f);
		((CCMenuItemSprite*)pPauseItem)->setNormalImage(resume);
		((CCMenuItemSprite*)pPauseItem)->setSelectedImage(resume_selected);
		
	}else{
		CCDirector::sharedDirector()->resume();
		
		CCSprite* pause=CCSprite::createWithSpriteFrameName("btn_pause.png");
		CCSprite* pause_selected=CCSprite::createWithSpriteFrameName("btn_pause.png");
		pause_selected->setScale(1.1f);
		((CCMenuItemSprite*)pPauseItem)->setNormalImage(pause);
		((CCMenuItemSprite*)pPauseItem)->setSelectedImage(pause_selected);
	}
}