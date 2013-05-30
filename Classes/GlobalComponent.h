#ifndef __GLOBAL_COMPONENT_H__
#define __GLOBAL_COMPONENT_H__

#include "cocos2d.h"

USING_NS_CC;

const int MENU_START=1;//开始按钮
const int MENU_GOBACK=2;//返回按钮
const int MENU_COLLECTION=3;//成就按钮
const int MENU_WIKI=4;//百科按钮
const int MENU_PLAY=5;//play按钮
const int MENU_PAUSE=6;//暂停按钮
const int MENU_END=7;//结束按钮
const int MENU_SHOP=8;//商店按钮
const int MENU_RESTART=9;//重新开始按钮
const int MENU_NEW_COLLECTION=10;//新成就按钮
const int MENU_FREEZE=11;//冰冻技能

//公共的一些按钮生成器
class GlobalComponent{
public:
	static CCMenuItem* createMenuItem(int name,CCObject* target,SEL_MenuHandler selector);
	
};

class CCMenuItemSpriteAdvanced:public CCMenuItemSprite{
public:
	virtual void selected();
	virtual void unselected();

	
    /** creates a menu item with a normal and selected image with target/selector */
    static CCMenuItemSpriteAdvanced * create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector);
	void setSelectedImage(CCNode* pImage);
};


#endif  //__GLOBAL_COMPONENT_H__
 