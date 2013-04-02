#include "TableViewTestScene.h"
#include "CustomTableViewCell.h"
#include "VisibleRect.h"

USING_NS_CC;
USING_NS_CC_EXT;

void runTableViewTest()
{
	CCScene *pScene = CCScene::create();
	TableViewTestLayer *pLayer = TableViewTestLayer::create();
	pScene->addChild(pLayer);
	CCDirector::sharedDirector()->replaceScene(pScene);
}
TableViewTestLayer::TableViewTestLayer():
curIdx(0),isScrolling(false),tableCount(3),draggingTime(0)
{}

// on "init" you need to initialize your instance
bool TableViewTestLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	CCString fileName[]={"bg_01.png","bg_02.png","bg_03.png"};
	
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
	
        CCSprite *sprite = CCSprite::create("bg_01.png");
		tableWidth=winSize.width*(1-0.18f);
		cellWidth=sprite->getContentSize().width+(tableWidth-sprite->getContentSize().width)/2;
		tableView = CCTableView::create(this, CCSizeMake(tableWidth, sprite->getContentSize().height));
		for(int i=0;i<tableCount;i++){
		CCSprite* sp=CCSprite::create(fileName[i].getCString());
		_sprites.push_back(sp);
		CCTableViewCell *cell=new CCTableViewCell();
		cell->addChild(sp);
		cell->setIdx(i);
		tableView->insertCellAtIndex(i);//=CCTableViewCell::create();
	}
    tableView->setDirection(kCCScrollViewDirectionHorizontal);
	tableView->setPosition(ccp(winSize.width-tableWidth,winSize.height/2-sprite->getContentSize().height/2));

    tableView->setDelegate(this);
	this->addChild(tableView);
    tableView->reloadData();
	this->scheduleUpdate();
    return true;
}

CCScene* TableViewTestLayer::scene(){
    CCScene *scene = NULL;
    do{
		scene = CCScene::create();
        CC_BREAK_IF(!scene);

        TableViewTestLayer *layer = TableViewTestLayer::create();
        CC_BREAK_IF(!layer);

        scene->addChild(layer);

    }while(0);

    return scene;
}
void TableViewTestLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
	if(!isScrolling) return;//若拖动则调整位置
	endPos=pTouch->getLocationInView();
	CCLog("endPos:%f",endPos.x);
	this->adjustTableView();
}
bool TableViewTestLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	
	return true;
}
void TableViewTestLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	if(!isScrolling){
		isScrolling=true;
		startPos=pTouch->getLocationInView();
		CCLog("startPos:%f",startPos.x);
	}
}
void TableViewTestLayer::update(float dt){
	if(tableView->isDragging()) draggingTime+=dt;
}

void TableViewTestLayer::shakeButton(int idx){
	if(idx<0||idx>=tableCount) return;
	CCLog("shake idx:%d",idx);
}

void TableViewTestLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	if(isScrolling) return;
	if(cell->getIdx()!=curIdx){
		this->shakeButton(curIdx);
	}else
		this->clickedCell(curIdx);
}
void TableViewTestLayer::adjustTableView()  
{  if(!isScrolling) return;//
    // 关闭CCScrollView中的自调整   
    tableView->unscheduleAllSelectors();  
    // 调整位置   
    CCPoint adjustPos;  
    // 调整动画时间   
    float adjustAnimDelay;  
	float adjustAnmiVelocity=fabs(endPos.x-startPos.x)/draggingTime;
	draggingTime=0;
	CCLog("adjustAnmiVelocity:%f",adjustAnmiVelocity);
	if(adjustAnmiVelocity<ADJUST_ANIM_VELOCITY) adjustAnmiVelocity=ADJUST_ANIM_VELOCITY;
	if(adjustAnmiVelocity>1000) adjustAnmiVelocity=1000;
      
    if (startPos.x>endPos.x) {//右移
		curIdx++;
		if(curIdx>=tableCount) curIdx=tableCount-1;
    }  
    else {//左移 
		curIdx--;
		if(curIdx<0) curIdx=0;
    }  
	adjustPos=ccp(-tableWidth*curIdx,0);
	adjustAnimDelay = (float) fabs(tableView->getContentOffset().x-adjustPos.x) / adjustAnmiVelocity;  
    // 调整位置   
    tableView->setContentOffsetInDuration(adjustPos, adjustAnimDelay); 
	this->schedule(schedule_selector(TableViewTestLayer::endScrolling),adjustAnimDelay);
}

void TableViewTestLayer::onEnter(){
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true); 
}
void TableViewTestLayer::onExit()  
{  
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);  
	this->unscheduleUpdate();
    CCLayer::onExit();  
} 

CCSize TableViewTestLayer::cellSizeForTable(CCTableView *table)
{
	CCSprite* sprite=CCSprite::create("bg_01.png");
	return CCSizeMake(cellWidth,sprite->getContentSize().height );
}

CCTableViewCell* TableViewTestLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCLog("%d",idx);
    CCString *string = CCString::createWithFormat("%d", idx);
	CCTableViewCell *cell = table->cellAtIndex(idx);//>dequeueCell();
    if (!cell) {
		CCLog("!cell");
        cell = new CustomTableViewCell();
        cell->autorelease();
		CCSprite* sprite;//=_sprites[idx];
		switch(idx){
		case 0:sprite=CCSprite::create("bg_01.png");break;
		case 1:sprite=CCSprite::create("bg_02.png");break;
		case 2:sprite=CCSprite::create("bg_03.png");break;
		}
	    sprite->setAnchorPoint(ccp(0.5, 0));  
        sprite->setPosition(ccp(tableWidth/2, 35));  
        sprite->setTag(456);
        cell->addChild(sprite);

        CCLabelTTF *label = CCLabelTTF::create(string->getCString(), "Helvetica", 20.0);
        label->setPosition(ccp(1,1));
	//	label->setAnchorPoint(CCPointZero);
        label->setTag(123);
        cell->addChild(label);
    }
    else
    {
        CCLabelTTF *label = (CCLabelTTF*)cell->getChildByTag(123);
        label->setString(string->getCString());
    }
    return cell;
}

unsigned int TableViewTestLayer::numberOfCellsInTableView(CCTableView *table)
{
    return tableCount;
}

void TableViewTestLayer::clickedCell(int idx){//点击编号为idx的图标
	if(idx<0||idx>=tableCount) return;
	CCLog("click cell with idx:%d",idx);

}
void TableViewTestLayer::menuCallBack(CCObject* pSender){
	CCLog("click menu :%d",curIdx);
}

void TableViewTestLayer::endScrolling(float dt){
	CCLog("%f",dt);
	isScrolling=false;
	this->unschedule(schedule_selector(TableViewTestLayer::endScrolling));
}