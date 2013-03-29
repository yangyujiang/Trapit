#ifndef __APP_DELEGATE_H__
#define __APP_DELEGATE_H__

#include "cocos2d.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
USING_NS_CC;

class  AppDelegate : private cocos2d::CCApplication
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
};
typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
}Resource;

static Resource smallResource  =  { cocos2d::CCSizeMake(854, 480),   "Android Phone 2" };
static Resource mediumResource =  { cocos2d::CCSizeMake(1280, 720),  "Android Phone 3"   };
static Resource largeResource  =  { cocos2d::CCSizeMake(1920, 1080), "Android Phone 3" };
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(1280, 720);

#endif  // __APP_DELEGATE_H__

