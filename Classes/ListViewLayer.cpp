#include "ListViewLayer.h"
#include "Constant.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool ListViewLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !CCLayer::init() );	

		bRet = true;
	}while(0);

	return bRet;
}
void ListViewLayer::initList(int width,int height,int count,CCSize cellSize){
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	this->count=count;
	this->cellSize=cellSize;

		CCTableView* pTableView = CCTableView::create(this, CCSizeMake(width, height));
		pTableView->setDirection(kCCScrollViewDirectionVertical);
		pTableView->setPosition(CCPointZero);
		pTableView->setDelegate(this);
		pTableView->setTag(TAG_COLLECTIONLIST);
		pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
		this->addChild(pTableView);
		pTableView->reloadData();
}

void ListViewLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLog("cell touched at index: %i", cell->getIdx());
	_idx=cell->getIdx();
}

CCSize ListViewLayer::cellSizeForTable(CCTableView *table)
{
    return cellSize;
}

CCTableViewCell* ListViewLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	_idx=idx;
	CCLog("_idx:%d",idx);
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
		
		CCSprite *listitem = CCSprite::create("listitem.png");
		listitem->setTag(666);
        listitem->setAnchorPoint(ccp(0,0));
		listitem->setPosition(CCPointZero);
        pCell->addChild(listitem,-1);

        CCSprite *sprite;
		switch(idx){
		case 0:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 1:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 2:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 3:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 4:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 5:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 6:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 7:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 8:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 9:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 10:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 11:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 12:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 13:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 14:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 15:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 16:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 17:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 18:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		default:sprite= CCSprite::create("amber_1.png");break;
		}
		sprite->setTag(777);
        sprite->setAnchorPoint(ccp(0,0.5));
		sprite->setPosition(ccp(0,listitem->getContentSize().height/2));
       // pCell->addChild(sprite);
		listitem->addChild(sprite);

        CCLabelTTF *pLabel = CCLabelTTF::create(pString->getCString(), "Arial", 20.0);
        pLabel->setAnchorPoint(ccp(0,0.5));
		pLabel->setPosition(ccp(sprite->getPositionX()+sprite->getContentSize().width,listitem->getContentSize().height/2));
        pLabel->setTag(123);
       // pCell->addChild(pLabel);
		listitem->addChild(pLabel);
    }
    else
    {
		pCell->removeChildByTag(666,true);
		//pCell->removeChildByTag(777,true);
		//pCell->removeChildByTag(123,true);
       
		CCSprite *listitem = CCSprite::create("listitem.png");
		listitem->setTag(666);
        listitem->setAnchorPoint(ccp(0,0));
		listitem->setPosition(CCPointZero);
        pCell->addChild(listitem,-1);

        CCSprite *sprite;
		switch(idx){
		case 0:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 1:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 2:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 3:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 4:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 5:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 6:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 7:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 8:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 9:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 10:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 11:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 12:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 13:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 14:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 15:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		case 16:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		case 17:sprite= CCSprite::createWithSpriteFrameName("amber_2.png");break;
		case 18:sprite= CCSprite::createWithSpriteFrameName("amber_3.png");break;
		default:sprite= CCSprite::create("amber_1.png");break;
		}
		sprite->setTag(777);
        sprite->setAnchorPoint(ccp(0,0.5));
		sprite->setPosition(ccp(0,listitem->getContentSize().height/2));
       // pCell->addChild(sprite);
		listitem->addChild(sprite);

        CCLabelTTF *pLabel = CCLabelTTF::create(pString->getCString(), "Arial", 20.0);
        pLabel->setAnchorPoint(ccp(0,0.5));
		pLabel->setPosition(ccp(sprite->getPositionX()+sprite->getContentSize().width,listitem->getContentSize().height/2));
        pLabel->setTag(123);
       // pCell->addChild(pLabel);
		listitem->addChild(pLabel);
		
    }

    return pCell;
}

unsigned int ListViewLayer::numberOfCellsInTableView(CCTableView *table)
{
    return count;
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


//
//ListMapVetical.cpp

CCTableViewCell* ListMapVertical::tableCellAtIndex(CCTableView *table,unsigned int idx){
	_idx=idx;
	CCLog("_idx:%d",idx);
    CCString *pString = CCString::createWithFormat("%d", idx);
	
    CCTableViewCell *pCell = table->dequeueCell();
    if (!pCell) {
        pCell = new CCTableViewCell();
        pCell->autorelease();

        CCSprite *sprite;
		switch(idx){
		case 0:sprite= CCSprite::create("level_1.png");break;
		case 1:sprite= CCSprite::create("level_2.png");break;
		case 2:sprite= CCSprite::create("level_3.png");break;
		default:sprite= CCSprite::create("amber_1.png");break;
		}
		sprite->setTag(666);
        sprite->setAnchorPoint(ccp(0,0));
		sprite->setPosition(ccp(0,0));
        pCell->addChild(sprite);
    }
    else
    {
		pCell->removeChildByTag(666,true);
       
		
        CCSprite *sprite;
		switch(idx){
		case 0:sprite= CCSprite::create("level_1.png");break;
		case 1:sprite= CCSprite::create("level_2.png");break;
		case 2:sprite= CCSprite::create("level_3.png");break;
		default:sprite= CCSprite::create("amber_1.png");break;
		}
		sprite->setTag(666);
        sprite->setAnchorPoint(ccp(0,0));
		sprite->setPosition(ccp(0,0));
        pCell->addChild(sprite);
		
    }

    return pCell;
}
