#include "GlobalComponent.h"

USING_NS_CC;

const float scale=1.1f;

CCMenuItem* GlobalComponent::createMenuItem(int id,CCObject* target,SEL_MenuHandler selector){
		CCSprite* normal;
		CCSprite* selected;
		switch(id){
		case MENU_START:
			normal=CCSprite::createWithSpriteFrameName("btn_start.png");
			selected=CCSprite::createWithSpriteFrameName("btn_start.png");
			break;
		case MENU_GOBACK:
			normal=CCSprite::createWithSpriteFrameName("btn_goback.png");
			selected=CCSprite::createWithSpriteFrameName("btn_goback.png");
			break;
		case MENU_COLLECTION:
			normal=CCSprite::createWithSpriteFrameName("btn_collection.png");
			selected=CCSprite::createWithSpriteFrameName("btn_collection.png");
			break;
		case MENU_WIKI:
			normal=CCSprite::createWithSpriteFrameName("btn_wiki.png");
			selected=CCSprite::createWithSpriteFrameName("btn_wiki.png");
			break;
		case MENU_PLAY:
			normal=CCSprite::createWithSpriteFrameName("btn_play.png");
			selected=CCSprite::createWithSpriteFrameName("btn_play.png");
			break;
		case MENU_PAUSE:
			normal=CCSprite::createWithSpriteFrameName("btn_pause.png");
			selected=CCSprite::createWithSpriteFrameName("btn_pause.png");
			break;
		case MENU_SHOP:
			normal=CCSprite::createWithSpriteFrameName("btn_shop.png");
			selected=CCSprite::createWithSpriteFrameName("btn_shop.png");
			break;
		case MENU_END:
			normal=CCSprite::createWithSpriteFrameName("btn_pause.png");
			selected=CCSprite::createWithSpriteFrameName("btn_pause.png");
			break;
		case MENU_RESTART:
			normal=CCSprite::createWithSpriteFrameName("result_restart.png");
			selected=CCSprite::createWithSpriteFrameName("result_restart.png");
			break;
		case MENU_NEW_COLLECTION:
			normal=CCSprite::createWithSpriteFrameName("btn_new_collection.png");
			selected=CCSprite::createWithSpriteFrameName("btn_new_collection.png");
			break;
		case MENU_FREEZE:
			normal=CCSprite::createWithSpriteFrameName("skill_2.png");
			selected=CCSprite::createWithSpriteFrameName("skill_2.png");
			break;
		}
		//selected->setScale(scale);
		//CCMenuItem* btnItem=CCMenuItemSprite::create(normal,selected,target,selector);
		CCMenuItem* btnItem=CCMenuItemSpriteAdvanced::create(normal,selected,target,selector);

		return btnItem;
}


void CCMenuItemSpriteAdvanced::selected(){
	CCSprite* selected=(CCSprite*)this->getSelectedImage();
	selected->runAction(CCScaleBy::create(0.1,1.1));
	CCMenuItemSprite::selected();
}
void CCMenuItemSpriteAdvanced::unselected(){
	this->getSelectedImage()->stopAllActions();
	this->getSelectedImage()->setScale(1);
	CCMenuItemSprite::unselected();
}

CCMenuItemSpriteAdvanced * CCMenuItemSpriteAdvanced::create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector)
{
    CCMenuItemSpriteAdvanced *pRet = new CCMenuItemSpriteAdvanced();
    pRet->initWithNormalSprite(normalSprite, selectedSprite, NULL, target, selector); 
    pRet->autorelease();
    return pRet;
}
void CCMenuItemSpriteAdvanced::setSelectedImage(CCNode* pImage)
{
	CCMenuItemSprite::setSelectedImage(pImage);
	this->getSelectedImage()->setAnchorPoint(ccp(0.5,0.5));
	this->getSelectedImage()->ignoreAnchorPointForPosition(true);
}
