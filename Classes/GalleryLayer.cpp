#include "GalleryLayer.h"
#include "ListViewLayer.h"

using namespace cocos2d;
using namespace cocos2d::extension;

bool GalleryLayer::init()
{
	bool bRet = false;
	do
	{
       CC_BREAK_IF( !CCLayer::init() );

	   visibleSize = CCDirector::sharedDirector()->getWinSize();//得到可视范围
	  // visibleSize.width=visibleSize.width*(CCDirector::sharedDirector()->getContentScaleFactor());//屏幕适应
	  // visibleSize.height=visibleSize.height*(CCDirector::sharedDirector()->getContentScaleFactor());//屏幕适应
	   CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	   CCLayer *pLayer = CCLayer::create();
  
	   CCSprite* sprite=CCSprite::create("map.png");
	   sprite->setAnchorPoint(ccp(1,1));
	   sprite->setPosition(ccp(visibleSize.width*0.68f,visibleSize.height));
	   addChild(sprite,-1);
	   char helpstr[30] = {0};
	   float scale=0;
	   buttonWidth=0;
	   for (unsigned int i = 1; i <= m_nCount; ++ i)
	   {
		   memset(helpstr, 0, sizeof(helpstr));
		   sprintf(helpstr,"bg_%02d.png",i);
		   CCSprite *pSprite = CCSprite::create(helpstr);
		   if(buttonWidth==0){
			   scrollWidth=visibleSize.width*(1-0.68f);//scrollView置于屏幕右侧，与左侧黄金比例
			   space=0.3f*scrollWidth/2;//两图标之间的宽度
			   buttonWidth=0.7f*scrollWidth/2;//图标宽度(适应屏幕)
			   float sp_width=pSprite->getContentSize().width;//图标原始宽度
			   scale=buttonWidth/sp_width;
		   }
		   CCMenuItemSprite* item=CCMenuItemSprite::create(pSprite,pSprite,pSprite,this,menu_selector(GalleryLayer::menuCallBack));
		   item->setTag(i);//设置tag标识
		   MyMenu *menu=MyMenu::create(item,NULL);

		   menu->setPosition(ccp((i+1)*(scrollWidth/2)+space/2, visibleSize.height / 2));
		   pSprite->setAnchorPoint(ccp(0,0.5));
		   pSprite->setScale(scale);
		   pLayer->addChild(menu);
	   }
	   CCLog("buttonWidth:%d",buttonWidth);
	   CCLog("scrollWidth:%d",scrollWidth);
	  // this->addChild(pLayer,3);
	   m_pScrollView = CCScrollView::create(CCSizeMake(scrollWidth*(m_nCount-1), visibleSize.height), pLayer);
	   m_pScrollView->setContentOffset(CCPointZero);
	   m_pScrollView->setAnchorPoint(CCPointZero);CCLog("scrollView.pos:%f",m_pScrollView->getPosition().x);
	   m_pScrollView->setPosition(ccp(visibleSize.width-scrollWidth,0));
	  // m_pScrollView->setTouchEnabled(true);
	   m_pScrollView->setBounceable(true);
	   m_pScrollView->setDelegate(this);
	   m_pScrollView->setDirection(kCCScrollViewDirectionHorizontal);
	   CCLog("scrollView.viewSize:%f,f",m_pScrollView->getViewSize().width,m_pScrollView->getViewSize().height);
	
	   this->addChild(m_pScrollView);

	/*   CCSpriteFrameCache *pCache =  CCSpriteFrameCache::sharedSpriteFrameCache();
   
	   pCache->addSpriteFrame(CCSpriteFrame::create("button_normal.png",CCRectMake(0, 0, 95, 135)),"button_normal.png");
	   pCache->addSpriteFrame(CCSpriteFrame::create("button_selected.png",CCRectMake(0, 0, 95, 135)),"button_selected.png");
	   for (int i = 1; i <= 3; ++ i)
	   {
		   CCSprite *pPoint = CCSprite::createWithSpriteFrameName("button_normal.png");
		   pPoint->setTag(i);
		   pPoint->setPosition(ccp( origin.x + (visibleSize.width - 3 * pPoint->getContentSize().width)/2 + pPoint->getContentSize().width * (i-1), 
			   origin.y + 30));
		   this->addChild(pPoint);
	   }
		CCSprite *pPoint = (CCSprite *)this->getChildByTag(1);
		pPoint->setDisplayFrame(pCache->spriteFrameByName("button_selected.png"));
	*/
		bRet = true;
	}while(0);

	return bRet;

}

void GalleryLayer::menuCloseCallback(CCObject* pSender)
{

}

void GalleryLayer::scrollViewDidScroll(cocos2d::extension::CCScrollView *view)
{
	if(!isScrolling) CCLOG("scroll");
}

void GalleryLayer::scrollViewDidZoom(cocos2d::extension::CCScrollView *view)
{
	CCLOG("zoom");
}

void GalleryLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void GalleryLayer::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
}


bool GalleryLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	m_nextPoint=m_touchPoint;
	return true;
}

void GalleryLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if(!isScrolling) isScrolling=true;
	CCPoint nextPosition=CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());CCLog("next:%f",nextPosition.x);
	m_pScrollView->setContentOffset(ccp(m_pScrollView->getContentOffset().x+nextPosition.x-m_nextPoint.x,0),true);
	//CCLog("contentOffSet:%f",m_pScrollView->getContentOffset().x);
	m_nextPoint=nextPosition;
}

void GalleryLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint endPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > 5)
	{
		adjustScrollView(distance);
	}
}

void GalleryLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCPoint endPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > 5)
	{
		adjustScrollView(distance);
	}
}

void GalleryLayer::adjustScrollView(float offset)
{
	/*CCSpriteFrameCache *pCache =  CCSpriteFrameCache::sharedSpriteFrameCache();
	CCSprite *pPoint = (CCSprite *)this->getChildByTag(m_nCurPage);
	pPoint->setDisplayFrame(pCache->spriteFrameByName("button_normal.png"));*/
	m_pScrollView->unscheduleAllSelectors();CCLog("%d",m_nCurPage);
	if (offset<0)
	{
		m_nCurPage ++;
	}else
	{
		m_nCurPage --;
	}

	if (m_nCurPage <1)
	{
		m_nCurPage = 1;
	}

	if(m_nCurPage > m_nCount)
	{
		m_nCurPage=m_nCount;
	}CCLog("%d",m_nCurPage);
		/*pPoint = (CCSprite *)this->getChildByTag(m_nCurPage);
		pPoint->setDisplayFrame(pCache->spriteFrameByName("button_selected.png"));
		//CCPoint  adjustPos = ccp(origin.x - visibleSize.width * (m_nCurPage-1), 0);*/

		CCLog("before:%f",m_pScrollView->getContentOffset().x);
		CCPoint  adjustPos = ccp(-(scrollWidth/2.0f*(m_nCurPage-1)) , 0);
		CCLog("adjustPos:%f",adjustPos.x);
		float adjustAnimDelay = (float) fabs(m_pScrollView->getContentOffset().x-adjustPos.x) / 800;  
		m_pScrollView->setContentOffsetInDuration(adjustPos,adjustAnimDelay);//setContentOffset(adjustPos, true);
	isScrolling=false;
		/*CCFiniteTimeAction *action=CCSequence::create(CCDelayTime::create(adjustAnimDelay),
			CCCallFunc::create(this,callfunc_selector(GalleryLayer::endScroll)),NULL);
		this->runAction(action);*/
}

void GalleryLayer::menuCallBack(CCObject* sender){
	CCLog("%d",((CCNode*)sender)->getTag());
	CCLog("menuPos:(%f,%f)",((CCNode*)sender)->getParent()->getPosition().x,((CCNode*)sender)->getParent()->getPosition().y);
	if(!isScrolling&&m_nCurPage==((CCNode*)sender)->getTag())
	CCLog("clicked");
}
void GalleryLayer::endScroll(){
	isScrolling=false;
}