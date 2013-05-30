#ifndef __GLOBAL_COMPONENT_H__
#define __GLOBAL_COMPONENT_H__

#include "cocos2d.h"

USING_NS_CC;

const int MENU_START=1;//��ʼ��ť
const int MENU_GOBACK=2;//���ذ�ť
const int MENU_COLLECTION=3;//�ɾͰ�ť
const int MENU_WIKI=4;//�ٿư�ť
const int MENU_PLAY=5;//play��ť
const int MENU_PAUSE=6;//��ͣ��ť
const int MENU_END=7;//������ť
const int MENU_SHOP=8;//�̵갴ť
const int MENU_RESTART=9;//���¿�ʼ��ť
const int MENU_NEW_COLLECTION=10;//�³ɾͰ�ť
const int MENU_FREEZE=11;//��������

//������һЩ��ť������
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
 