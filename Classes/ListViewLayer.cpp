#include "ListViewLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;


bool ListViewLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !CCLayer::init() );
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();

		CCTableView* pTableView = CCTableView::create(this, CCSizeMake(winSize.width*4.0f/5, winSize.height));
		pTableView->setDirection(kCCScrollViewDirectionVertical);
		pTableView->setPosition(CCPointZero);
		pTableView->setDelegate(this);
		pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
		this->addChild(pTableView);
		pTableView->reloadData();

		bRet = true;
	}while(0);

	return bRet;
}



void ListViewLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLog("cell touched at index: %i", cell->getIdx());
}

CCSize ListViewLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(960, 120);
}

CCTableViewCell* ListViewLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	_idx=idx;
    CCString *pString = CCString::createWithFormat("%d", idx);
	CCString *string;  
    switch(idx)  
    {  
        case 0: string = CCString::createWithFormat("%s", "CITY"); break;  
        case 1: string = CCString::createWithFormat("%s", "WORLD"); break;  
        case 2: string = CCString::createWithFormat("%s", "BATTLE"); break;  
        case 3: string = CCString::createWithFormat("%s", "CityInfo"); break;  
        case 4: string = CCString::createWithFormat("%s", "Commander"); break;  
        case 5: string = CCString::createWithFormat("%s", "Equipment"); break;  
        case 6: string = CCString::createWithFormat("%s", "Quest"); break;  
        case 7: string = CCString::createWithFormat("%s", "Items"); break;  
        case 8: string = CCString::createWithFormat("%s", "Military"); break;  
        case 9: string = CCString::createWithFormat("%s", "Legion"); break;  
        case 10: string = CCString::createWithFormat("%s", "Rank"); break;  
        case 11: string = CCString::createWithFormat("%s", "Report"); break;  
        case 12: string = CCString::createWithFormat("%s", "News"); break;  
        case 13: string = CCString::createWithFormat("%s", "System"); break;  
        case 14: string = CCString::createWithFormat("%s", "Shop"); break;  
        case 15: string = CCString::createWithFormat("%s", "Friend"); break;  
        default:string = CCString::createWithFormat("%s", "Error");  
    }  

    CCTableViewCell *pCell = table->dequeueCell();
    if (!pCell) {
        pCell = new CCTableViewCell();
        pCell->autorelease();
        CCSprite *sprite;
		switch(idx){
		case 0:sprite= CCSprite::createWithSpriteFrameName("mantis_1.png");break;
		case 1:sprite= CCSprite::createWithSpriteFrameName("mantis_2.png");break;
		case 2:sprite= CCSprite::createWithSpriteFrameName("mantis_3.png");break;
		case 3:sprite= CCSprite::createWithSpriteFrameName("mantis_4.png");break;
		default:sprite= CCSprite::create("CloseNormal.png");break;
		}
		sprite->setTag(777);
        sprite->setAnchorPoint(ccp(0,0.5));
		sprite->setPosition(CCPointZero);
        pCell->addChild(sprite);

		CCSprite *pSprite = CCSprite::create("listitem.png");
		pSprite->setTag(666);
        pSprite->setAnchorPoint(ccp(0,0.5));
		pSprite->setPosition(CCPointZero);
        pCell->addChild(pSprite,-1);

        CCLabelTTF *pLabel = CCLabelTTF::create(pString->getCString(), "Arial", 20.0);
        pLabel->setAnchorPoint(ccp(0,0.5));
		pLabel->setPosition(ccp(sprite->getPositionX()+sprite->getContentSize().width,0));
        pLabel->setTag(123);
        pCell->addChild(pLabel);
    }
    else
    {
		pCell->removeChildByTag(666,true);
		pCell->removeChildByTag(777,true);
		pCell->removeChildByTag(123,true);
        CCSprite *sprite;
		switch(idx){
		case 0:sprite= CCSprite::createWithSpriteFrameName("mantis_1.png");break;
		case 1:sprite= CCSprite::createWithSpriteFrameName("mantis_2.png");break;
		case 2:sprite= CCSprite::createWithSpriteFrameName("mantis_3.png");break;
		case 3:sprite= CCSprite::createWithSpriteFrameName("mantis_4.png");break;
		default:sprite= CCSprite::create("CloseNormal.png");break;
		}
		sprite->setTag(777);
        sprite->setAnchorPoint(ccp(0,0.5));
		sprite->setPosition(CCPointZero);
        pCell->addChild(sprite);

		 CCSprite *pSprite = CCSprite::create("listitem.png");
		pSprite->setTag(666);
        pSprite->setAnchorPoint(ccp(0,0.5));
		pSprite->setPosition(CCPointZero);
        pCell->addChild(pSprite,-1);

        CCLabelTTF *pLabel = CCLabelTTF::create(pString->getCString(), "Arial", 20.0);
		pLabel->setAnchorPoint(ccp(0,0.5));
		pLabel->setPosition(ccp(sprite->getPositionX()+sprite->getContentSize().width,0));
        pLabel->setTag(123);
        pCell->addChild(pLabel);
		
    }


    return pCell;
}

unsigned int ListViewLayer::numberOfCellsInTableView(CCTableView *table)
{
    return 20;
}


void ListViewLayer::scrollViewDidScroll(CCScrollView *view)
{
}

void ListViewLayer::scrollViewDidZoom(CCScrollView *view)
{
}

unsigned int ListViewLayer::getIdx(){
	return _idx;
}