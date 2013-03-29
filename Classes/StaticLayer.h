#ifndef __STATIC_LAYER_H__
#define __STATIC_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;

//����һ����ֹ�㣬�˲�������������Ļһ�㾲ֹ����
class StaticLayer :public cocos2d::CCLayer
{
protected:
	CCMenuItemImage *pCloseItem;
	CCMenuItemImage *pPauseItem;//��ͣ��ť
public:
	StaticLayer();
	virtual ~StaticLayer();

	virtual bool init();
	CREATE_FUNC(StaticLayer);
	
	// a selector callback
    void menuCloseCallback(CCObject* pSender);
	void menuPauseCallBack(CCObject* pSender);
	void keepStill();//���ֲ���
};

#endif //__STATIC_LAYER_H__
