#ifndef MVCprototype_GlobalDef_h
#define MVCprototype_GlobalDef_h

#include "cocos2d.h"
#include "Box2D\Box2D.h"

USING_NS_CC;

#define CREATE_FUNC_NO_PARAM(modelName) \
static modelName* create() \
{ \
modelName* pModel = new modelName(); \
if(pModel&&pModel->init()) \
{\
return pModel; \
}\
else\
{\
delete pModel;\
pModel=NULL;\
return NULL;\
}\
};

#define CREATE_FUNC_ONE_PARAM(modelName,__PARAMTYPE__,__PARAM__) \
static modelName* create(__PARAMTYPE__ __PARAM__) \
{ \
modelName* pModel = new modelName(); \
if(pModel&&pModel->init(__PARAM__)) \
{\
pModel->autorelease(); \
pModel->retain();\
return pModel; \
}\
else\
{\
delete pModel;\
pModel=NULL;\
return NULL;\
}\
};

#define LAYER_CREATE_FUNC_DOUBLE_PARAM(layer,__PARAMTYPE1__,__PARAM1__,__PARAMTYPE2__,__PARAM2__) \
static layer* CREATE(__PARAMTYPE1__ __PARAM1__,__PARAMTYPE2__ __PARAM2__) \
{ \
layer *pRet = new layer(); \
if (pRet && pRet->init(__PARAM1__,__PARAM2__)) \
{ \
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}; 

#define VIEW_FUNC_ONEXIT \
virtual void onExit() { \
CCSpriteFrameCache::sharedSpriteFrameCache()->purgeSharedSpriteFrameCache(); \
CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames(); \
CCLayer::onExit(); \
}; 

#endif
