#include "ListViewLayer.h"
#include "Constant.h"
#include "MyMath.h"
#include "GamePlayController.h"
#include "LanguageAdapter.h"

USING_NS_CC;
USING_NS_CC_EXT;

const unsigned int TAG_SELECTED=12321;
const CCRect selectedRect=CCRect(0,0,1000,1000);

CCString* getAmberDescrip(int id){
	CCString *pString;
	std::string str;
	switch(id){
	case 0:
		{
		//str=MyMath::WStrToUTF8(L"愤怒的蚂蚁蓝珀");
		str="愤怒的蚂蚁蓝珀";
		pString=CCString::createWithFormat("%s",str.c_str());
		break;
		}
	default:
		pString=CCString::createWithFormat("%s","暂未获得");
		break;
	}
	return pString;
}

//
//ListViewLayer.cpp
//

ListViewLayer::ListViewLayer():
_idx(0),
count(0),
cellSize(CCSizeMake(0,0)),
selectedSprite(NULL),
scrollPoint(ccp(0,0))
{
}

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

		//CCTableView* pTableView = CCTableView::create(this, CCSizeMake(width, height));
	CCTableViewAdvanced* pTableView = CCTableViewAdvanced::create(this, CCSizeMake(width, height));	
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
	this->selectedItem(_idx,cell);
}

CCSize ListViewLayer::cellSizeForTable(CCTableView *table)
{
    return cellSize;
}
void ListViewLayer::selectedItem(unsigned int id, CCTableViewCell *pCell ){
/*	CCPoint offSet=((CCTableViewAdvanced*)(pCell->getParent()))->getContentOffset();
	CCPoint pos=this->convertToNodeSpace(offSet);
	CCLog("%f,%f",id,offSet.x,offSet.y);
	CCLog("id:%d,%f,%f",id,pos.x,pos.y);*/
	CCSprite* selected=(CCSprite*)pCell->getChildByTag(TAG_SELECTED);
	if(selectedSprite!=NULL){
		selectedSprite->setVisible(false);
		selectedSprite=selected;
		selectedSprite->setVisible(true);
	}
}
void ListViewLayer::selected(){
	if(count==0) return;
	CCTableView* tableView=(CCTableView*)this->getChildByTag(TAG_COLLECTIONLIST);
	CCTableViewCell* cell=tableView->cellAtIndex(_idx);
	this->selectedItem(_idx,cell);
}
void ListViewLayer::getColumnById(unsigned int id, CCTableViewCell *pCell ){	
    CCString *pString = CCString::createWithFormat("%d", id);
	//CCString *descrip=getAmberDescrip(id);
	if(id>3) id=3;

	//选中状态
	CCSprite* selected=CCSprite::create("old/selected_2.png");
	selected->setAnchorPoint(CCPointZero);
	selected->setPosition(CCPointZero);
	selected->setVisible(false);
	pCell->addChild(selected,-2,TAG_SELECTED);

	if(selectedSprite==NULL&&id==0){
		selectedSprite=selected;
		selectedSprite->setVisible(true);
	}
	//this->selectedItem(_idx,pCell);
	

	CCSprite *listitem = CCSprite::createWithSpriteFrameName("selected.png");
	listitem->setTag(666);
    listitem->setAnchorPoint(ccp(0,0));
	listitem->setPosition(CCPointZero);
    pCell->addChild(listitem,-1);

        CCSprite *sprite;
		switch(id){
		case 0:sprite= CCSprite::createWithSpriteFrameName("amber_yellow.png");break;
		case 1:sprite= CCSprite::createWithSpriteFrameName("amber_red.png");break;
		case 2:sprite= CCSprite::createWithSpriteFrameName("amber_blue.png");break;
		default:sprite= CCSprite::createWithSpriteFrameName("amber_unknown.png");break;
		}
		sprite->setTag(777);
        sprite->setAnchorPoint(ccp(0,0.5));
		sprite->setPosition(ccp(0,listitem->getContentSize().height/2));
       // pCell->addChild(sprite);
		listitem->addChild(sprite);

		CCSprite* dotted_line=CCSprite::createWithSpriteFrameName("dottedline.png");
		dotted_line->setAnchorPoint(ccp(0,0));
		dotted_line->setPosition(ccp(0,0));
		listitem->addChild(dotted_line);

		//琥珀描述
		CCLabelTTF *label_descrip=CCLabelTTF::create(__descrip[id],"Arial",30.f);
		label_descrip->setAnchorPoint(ccp(0.5,0.5));
		label_descrip->setPosition(ccp(sprite->getPositionX()+sprite->getContentSize().width+label_descrip->getContentSize().width/2,
			listitem->getContentSize().height*0.8f));
		listitem->addChild(label_descrip);

		//获取提示
		CCLog("howToGet:%s",__howToGet[id]);
		CCLabelTTF *label_howToGet=CCLabelTTF::create(__howToGet[id],"Arial",30.f);
		label_howToGet->setAnchorPoint(ccp(0.5,0.5));
		label_howToGet->setPosition(ccp(sprite->getPositionX()+sprite->getContentSize().width+label_howToGet->getContentSize().width/2,
			listitem->getContentSize().height*0.5f));
		listitem->addChild(label_howToGet);

		//技能名称		
		CCLog("skillName:%s",__skillName[id]);
		CCLabelTTF *label_skillName=CCLabelTTF::create(__skillName[id],"Arial",30.f);
		label_skillName->setAnchorPoint(ccp(0.5,0.5));
		label_skillName->setPosition(ccp(sprite->getPositionX()+sprite->getContentSize().width+label_skillName->getContentSize().width/2,
			listitem->getContentSize().height*0.2f));
		listitem->addChild(label_skillName);
		

        CCLabelTTF *pLabel = CCLabelTTF::create(pString->getCString(), "Arial", 40.0f);
        pLabel->setAnchorPoint(ccp(0,0));
		pLabel->setPosition(ccp(0,0));
        pLabel->setTag(123);
		listitem->addChild(pLabel);
}

CCTableViewCell* ListViewLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	//_idx=idx;
	CCLog("_idx:%d",idx);

    CCTableViewCell *pCell = table->dequeueCell();
    if (!pCell) {
        pCell = new CCTableViewCell();
        pCell->autorelease();
		this->getColumnById(idx,pCell);
    }
    else
    {
		pCell->removeChildByTag(666,true);
		
		this->getColumnById(idx,pCell);		
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
void ListViewLayer::setIdx(unsigned int id){
	if(_idx!=id){
		_idx=id;
		this->selected();
	}
}





//
//AmberList.cpp
//
void AmberList::initList(int width,int height,int count,CCSize cellSize){
	ListViewLayer::initList(width,height,count,cellSize);
	std::vector<Amber*> newAmberId=GamePlayController::getNewAmbers();
	
	for(unsigned int i=0;i<newAmberId.size();i++){
		amberId[i]=newAmberId[i]->getId();	
	}
}
void AmberList::getColumnById(unsigned int id, CCTableViewCell *pCell ){	
    CCString *pString = CCString::createWithFormat("%d", id);
	CCString *descrip=getAmberDescrip(id);

	CCSprite *listitem = CCSprite::createWithSpriteFrameName("amberlist_listitem.png");
	listitem->setTag(666);
    listitem->setAnchorPoint(ccp(0,0));
	listitem->setPosition(CCPointZero);
    pCell->addChild(listitem,-1);

        CCSprite *sprite;
		switch(amberId[id]){
		case 0:sprite= CCSprite::createWithSpriteFrameName("amber_yellow.png");break;
		case 1:sprite= CCSprite::createWithSpriteFrameName("amber_red.png");break;
		case 2:sprite= CCSprite::createWithSpriteFrameName("amber_blue.png");break;
		default:sprite= CCSprite::createWithSpriteFrameName("amber_unknown.png");break;
		}
		sprite->setTag(777);
        sprite->setAnchorPoint(ccp(0,0.5));
		sprite->setPosition(ccp(0,listitem->getContentSize().height/2));
		listitem->addChild(sprite);

	
		CCLabelTTF *label_descrip=CCLabelTTF::create(descrip->getCString(),"Arial",30.f);
		label_descrip->setAnchorPoint(ccp(0.5,0.5));
		label_descrip->setPosition(ccp(sprite->getPositionX()+sprite->getContentSize().width+label_descrip->getContentSize().width/2,
			listitem->getContentSize().height*0.8f));
		listitem->addChild(label_descrip);

        CCLabelTTF *pLabel = CCLabelTTF::create(pString->getCString(), "Arial", 40.0f);
        pLabel->setAnchorPoint(ccp(0,0));
		pLabel->setPosition(ccp(0,0));
        pLabel->setTag(123);
		listitem->addChild(pLabel);
}

CCTableViewCell* AmberList::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	_idx=idx;
	CCLog("AmberList:_idx:%d",idx);

    CCTableViewCell *pCell = table->dequeueCell();
    if (!pCell) {
        pCell = new CCTableViewCell();
        pCell->autorelease();
		this->getColumnById(idx,pCell);
    }
    else
    {
		pCell->removeChildByTag(666,true);
		
		this->getColumnById(idx,pCell);		
    }

    return pCell;
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
		case 0:sprite= CCSprite::createWithSpriteFrameName("level_1.png");break;
		case 1:sprite= CCSprite::createWithSpriteFrameName("level_2.png");break;
		case 2:sprite= CCSprite::createWithSpriteFrameName("level_3.png");break;
		default:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
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
		case 0:sprite= CCSprite::createWithSpriteFrameName("level_1.png");break;
		case 1:sprite= CCSprite::createWithSpriteFrameName("level_2.png");break;
		case 2:sprite= CCSprite::createWithSpriteFrameName("level_3.png");break;
		default:sprite= CCSprite::createWithSpriteFrameName("amber_1.png");break;
		}
		sprite->setTag(666);
        sprite->setAnchorPoint(ccp(0,0));
		sprite->setPosition(ccp(0,0));
        pCell->addChild(sprite);
		
    }

    return pCell;
}



//
//CCTableViewAdvanced.cpp
//

CCTableViewAdvanced* CCTableViewAdvanced::create(CCTableViewDataSource* dataSource, CCSize size)
{
    return CCTableViewAdvanced::create(dataSource, size, NULL);
}

CCTableViewAdvanced* CCTableViewAdvanced::create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container)
{
    CCTableViewAdvanced *table = new CCTableViewAdvanced();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
    table->_updateContentSize();

    return table;
}

bool CCTableViewAdvanced::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (!this->isVisible())
    {
        return false;
    }
    CCRect frame;
    CCPoint frameOriginal = this->getParent()->convertToWorldSpace(this->getPosition());
    frame = CCRectMake(frameOriginal.x, frameOriginal.y, m_tViewSize.width, m_tViewSize.height);
    
		//杨添加
		readyMoved=false;

    //dispatcher does not know about clipping. reject touches outside visible bounds.
    if (m_pTouches->count() > 2 ||
        m_bTouchMoved          ||
        !frame.containsPoint(m_pContainer->convertToWorldSpace(m_pContainer->convertTouchToNodeSpace(touch))))
    {
        return false;
    }

    if (!m_pTouches->containsObject(touch))
    {
        m_pTouches->addObject(touch);
    }

    if (m_pTouches->count() == 1)
    { // scrolling
        m_tTouchPoint     = this->convertTouchToNodeSpace(touch);
        m_bTouchMoved     = false;
        m_bDragging     = true; //dragging started
        m_tScrollDistance = ccp(0.0f, 0.0f);
        m_fTouchLength    = 0.0f;

		//yang添加
		beginPoint=touch->getLocation();
		m_bDragging=false;
		readyMoved=true;
    }
    else if (m_pTouches->count() == 2)
    {
        m_tTouchPoint  = ccpMidpoint(this->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
                                   this->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
        m_fTouchLength = ccpDistance(m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
                                   m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
        m_bDragging  = false;
    } 
    return true;
}

void CCTableViewAdvanced::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if (!this->isVisible())
    {
        return;
    }

    if (m_pTouches->containsObject(touch))
    {
       // if (m_pTouches->count() == 1 && m_bDragging)
         if (m_pTouches->count() == 1 && readyMoved)
		{ 
			if(fabs(touch->getLocation().y-beginPoint.y)<10){//如果移动的距离达到一定程度，才开始移动
				return;
			}else if(m_bDragging==false){
				m_bDragging=true;
		       m_tTouchPoint     = this->convertTouchToNodeSpace(touch);
			}

			// scrolling
            CCPoint moveDistance, newPoint, maxInset, minInset;
            CCRect  frame;
            float newX, newY;
            
            m_bTouchMoved  = true;
            CCPoint frameOriginal = this->getParent()->convertToWorldSpace(this->getPosition());
            frame = CCRectMake(frameOriginal.x, frameOriginal.y, m_tViewSize.width, m_tViewSize.height);

            newPoint     = this->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0));
            moveDistance = ccpSub(newPoint, m_tTouchPoint);
            m_tTouchPoint  = newPoint;
            
            if (frame.containsPoint(this->convertToWorldSpace(newPoint)))
            {
                switch (m_eDirection)
                {
                    case kCCScrollViewDirectionVertical:
                        moveDistance = ccp(0.0f, moveDistance.y);
                        break;
                    case kCCScrollViewDirectionHorizontal:
                        moveDistance = ccp(moveDistance.x, 0.0f);
                        break;
                    default:
                        break;
                }

                m_pContainer->setPosition(ccpAdd(m_pContainer->getPosition(), moveDistance));
                
                maxInset = m_fMaxInset;
                minInset = m_fMinInset;
                
                
                //check to see if offset lies within the inset bounds
                newX     = MIN(m_pContainer->getPosition().x, maxInset.x);
                newX     = MAX(newX, minInset.x);
                newY     = MIN(m_pContainer->getPosition().y, maxInset.y);
                newY     = MAX(newY, minInset.y);
                
                m_tScrollDistance     = ccpSub(moveDistance, ccp(newX - m_pContainer->getPosition().x, newY - m_pContainer->getPosition().y));
                this->setContentOffset(ccp(newX, newY));
            }
        }
        else if (m_pTouches->count() == 2 && !m_bDragging)
        {
            const float len = ccpDistance(m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
                                            m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
            this->setZoomScale(this->getZoomScale()*len/m_fTouchLength);
        }
    }
}
