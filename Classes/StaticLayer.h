#ifndef __STATIC_LAYER_H__
#define __STATIC_LAYER_H__

#include "cocos2d.h"
#include "Constant.h"
USING_NS_CC;

//这是一个静止层，此层的物体相对于屏幕一般静止不动
class StaticLayer :public cocos2d::CCLayer
{
protected:
	CCMenuItemImage *pCloseItem;
	CCMenuItemImage *pPauseItem;//暂停按钮
public:
	InnerStage innerStage;//调试用
public:
	StaticLayer();
	virtual ~StaticLayer();

	virtual bool init();
	CREATE_FUNC(StaticLayer);
	
	// a selector callback
    void menuCloseCallback(CCObject* pSender);
	void menuPauseCallBack(CCObject* pSender);
	void keepStill(CCPoint lastPositionOfScene);//保持不动
	void draw();
};

#endif //__STATIC_LAYER_H__
