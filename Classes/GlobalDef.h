#ifndef MVCprototype_GlobalDef_h
#define MVCprototype_GlobalDef_h

#include "cocos2d.h"
#include "Box2D\Box2D.h"

USING_NS_CC;

/** @def CC_COCOSDEGREES_TO_BOX2DRADIANS
 converts cocos2d degrees to box2d radians
 */
#define CC_COCOSDEGREES_TO_BOX2DRADIANS(__ANGLE__) (-1*CC_DEGREES_TO_RADIANS(__ANGLE__+90)) 
/** @def CC_BOX2DRADIANS_TO_COCOSDEGREES
 converts box2d radians to cocos2d degrees
 */
#define CC_BOX2DRADIANS_TO_COCOSDEGREES(__ANGLE__) (CC_RADIANS_TO_DEGREES(-1*__ANGLE__)-90) 

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
return pModel; \
}\
else\
{\
delete pModel;\
pModel=NULL;\
return NULL;\
}\
};

#define CREATE_FUNC_THREE_PARAM(Type,__PARAMTYPE1__,__PARAM1__,__PARAMTYPE2__,__PARAM2__,__PARAMTYPE3__,__PARAM3__) \
static Type* create(__PARAMTYPE1__ __PARAM1__,__PARAMTYPE2__ __PARAM2__,__PARAMTYPE3__ __PARAM3__) \
{ \
Type *pRet = new Type(); \
if (pRet && pRet->init(__PARAM1__,__PARAM2__,__PARAM3__)) \
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

#define  LAYER_CREATE_FUNC_DOUBLE_PARAM(layer,__PARAMTYPE1__,__PARAM1__,__PARAMTYPE2__,__PARAM2__) \
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
