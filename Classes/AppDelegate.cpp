#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "GamePlayController.h"
#include "GameMenuController.h"
#include "GameWelcomeController.h"
#include "GameCollectionController.h"
#include "GameOverController.h"
#include "SmartRes.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MENU_MUSIC);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(GAME_MUSIC);
}

AppDelegate::~AppDelegate()
{
	CCLog("~AppDelegate");
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("buttons.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("menu.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("ambers.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("collections.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("menuMap.plist");
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
	CCLog("applicationDidFinishLaunching");
  // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	/*CCFileUtils::sharedFileUtils()->setResourceDirectory("hd");
    SmartRes::sharedRes()->setVirtualScreenWidth(960);
	*/
    // Set the design resolution
    //pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
    CCSize frameSize = pEGLView->getFrameSize();

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

    // if the frame's height is larger than the height of medium resource size, select large resource.
   if (frameSize.height > mediumResource.size.height)
   { 
	   CCFileUtils::sharedFileUtils()->setResourceDirectory(largeResource.directory);
        pDirector->setContentScaleFactor(largeResource.size.height/designResolutionSize.height);
    }
    // if the frame's height is larger than the height of small resource size, select medium resource.
   else if (frameSize.height > smallResource.size.height)
    { 
        CCFileUtils::sharedFileUtils()->setResourceDirectory(largeResource.directory);
        pDirector->setContentScaleFactor(mediumResource.size.height/designResolutionSize.height);
   }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
    else
    { 
        CCFileUtils::sharedFileUtils()->setResourceDirectory(largeResource.directory);
        pDirector->setContentScaleFactor(smallResource.size.height/designResolutionSize.height);
    }

	

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("buttons.plist","buttons.png");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("menu.plist","menu.png");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("collections.plist","collections.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("menuMap.plist","menuMap.png");
    // create a scene. it's an autorelease object
   //CCScene *pScene = GamePlayController::scene();
	//CCScene *pScene = GameMenuController::scene();pScene->setTag(1111);
    CCScene *pScene = GameWelcomeController::scene();
	 //CCScene *pScene = GameOverController::scene();
	//CCScene *pScene = GameCollectionController::scene();
	//CCScene *pScene = TableViewTestLayer::scene();
	/*CCScene *pScene = CCScene::create();
	CCLayer *pLayer = GalleryLayer::create();
	pScene->addChild(pLayer);*/
    // run
    pDirector->runWithScene(pScene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	CCLog("applicationDidEnterBackground");

	CCScene* scene=CCDirector::sharedDirector()->getRunningScene();
	if(scene->getTag()==1111){
		((GameMenuController*)(scene->getChildren()->lastObject()))->save();
	}
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	CCLog("applicationWillEnterForeground");
	
	CCScene* scene=CCDirector::sharedDirector()->getRunningScene();
	if(scene!=NULL&&scene->getTag()==1111){
		((GameMenuController*)(scene->getChildren()->lastObject()))->initResinVol();
	}
}
