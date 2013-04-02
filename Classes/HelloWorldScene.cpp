#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
USING_NS_CC_EXT;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    pLabel = CCLabelTTF::create("Hello World", "Thonburi", 34);

    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    this->addChild(pLabel, 1);


    // CCScrollView
    scrollView = CCScrollView::create();
    CCLayer *layer = CCLayer::create();
    
    CCSprite *sprite1 = CCSprite::create("HelloWorld.png");
    CCSprite *sprite2 = CCSprite::create("HelloWorld.png");
    CCSprite *sprite3 = CCSprite::create("button.png");
	sprite3->setPosition(ccp(size.width/2,size.height/2));
	addChild(sprite3);
    layer->setAnchorPoint(CCPointZero);
    layer->setPosition(CCPointZero);
    
    // Menu
   // CCMenuItemSprite *menuItem1 = CCMenuItemSprite::create(sprite1, sprite1, this, menu_selector(HelloWorld::menu1Callback));
	CCMenuItemImage *menuItem1 = CCMenuItemImage::create("HelloWorld.png", "HelloWorld.png", this, menu_selector(HelloWorld::menu1Callback));
    menuItem1->setPosition(ccpAdd(CCPointZero, ccp(size.width / 2, size.height / 2)));
   // menuItem1->setScale(0.4f);
   //CCMenuItemSprite *menuItem2 = CCMenuItemSprite::create(sprite2, sprite2, this, menu_selector(HelloWorld::menu2Callback));
	 CCMenuItemImage *menuItem2 = CCMenuItemImage::create("HelloWorld.png", "HelloWorld.png", this, menu_selector(HelloWorld::menu1Callback));
   menuItem2->setPosition(ccpAdd(ccp(size.width, 0), ccp(size.width / 2, size.height / 2)));
 //   menuItem2->setScale(0.4f);
    CCMenu *menu = CCMenu::create(menuItem1, menuItem2, NULL);
    
    menu->setPosition(CCPointZero);
    layer->addChild(menu);
    
    scrollView->setPosition(CCPointZero);
    scrollView->setContentOffset(CCPointZero);
	layer->setContentSize(CCSizeMake(size.width, size.height));
    scrollView->setContentSize(CCSizeMake(size.width, size.height));
    scrollView->setContainer(layer);
    
    // ���ù�������
    scrollView->setDirection(kCCScrollViewDirectionVertical);
    scrollView->setDelegate(this);

    
    this->addChild(scrollView,10);
    
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::scrollViewDidScroll(cocos2d::extension::CCScrollView *view)
{
}

void HelloWorld::scrollViewDidZoom(cocos2d::extension::CCScrollView *view)
{
}

void HelloWorld::onEnter()
{
    CCLayer::onEnter();
    // ����ĵ���������һ��Ҫ���ó�false��
    // true ��HelloWorld�����ɵ������¼�
    // false ��HelloWorld����CCScrollView�����Ⱥ������¼�
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void HelloWorld::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool HelloWorld::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void HelloWorld::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCLOG("move");
}

void HelloWorld::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    adjustScrollView();
}

void HelloWorld::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    adjustScrollView();
}

void HelloWorld::adjustScrollView()
{
    // �ر�CCScrollView�е��Ե���
    scrollView->unscheduleAllSelectors();
    
    int x = scrollView->getContentOffset().x;
    int offset = (int) x % 480;
    // ����λ��
    CCPoint adjustPos;
    // ��������ʱ��
    float adjustAnimDelay;
    
    if (offset < -240) {
        // ������һҳλ�ã�ʱ��
        adjustPos = ccpSub(scrollView->getContentOffset(), ccp(480 + offset, 0));
        adjustAnimDelay = (float) (480 + offset) / ADJUST_ANIM_VELOCITY;
    }
    else {
        // ���㵱ǰҳλ�ã�ʱ��
        adjustPos = ccpSub(scrollView->getContentOffset(), ccp(offset, 0));
        // ����Ҫȡ����ֵ�������ڵ�һҳ���󷭶���ʱ����֤adjustAnimDelayΪ����
        adjustAnimDelay = (float) abs(offset) / ADJUST_ANIM_VELOCITY;
    }
    
    // ����λ��
    scrollView->setContentOffsetInDuration(adjustPos, adjustAnimDelay);
}

void HelloWorld::menu1Callback(cocos2d::CCObject *pSender)
{
    CCLOG("menu1Callback");
}

void HelloWorld::menu2Callback(cocos2d::CCObject *pSender)
{
    CCLOG("menu2Callback");
}


