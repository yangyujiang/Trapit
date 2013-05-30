#ifndef __TABLEVIEWTESTSCENE_H__
#define __TABLEVIEWTESTSCENE_H__

#include "cocos2d.h"
#include "../extensions/cocos-ext.h"
#include "Amber.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ListViewLayer : public cocos2d::CCLayer, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
protected:
	CC_PROPERTY(unsigned int,_idx,Idx);
	unsigned int count;
	CCSize cellSize;
	CCSprite* selectedSprite;//被选中

	CC_SYNTHESIZE(CCPoint, scrollPoint,ScrollPoint);
protected:
	void getColumnById(unsigned int id, CCTableViewCell *pCell);
	void selectedItem(unsigned int id, CCTableViewCell *pCell );
	void selected();
public:
	ListViewLayer();
    virtual bool init();  
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);

    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view);

	//处理触摸事件
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	//每一项的宽度和高度
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
	//生成列表每一项的内容
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	//一共多少项
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

    CREATE_FUNC(ListViewLayer);
	void initList(int width,int height,int count,CCSize cellSize);
};


class AmberList : public ListViewLayer
{
protected:
	vector<Amber*> newAmbers;
	int amberId[20];
public:
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	
	void initList(int width,int height,int count,CCSize cellSize);
	void getColumnById(unsigned int id, CCTableViewCell *pCell);
	CREATE_FUNC(AmberList);
};


//ListMapVertical.h
class ListMapVertical:public ListViewLayer{
public:
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	
	CREATE_FUNC(ListMapVertical);
protected:
};


class CCTableViewAdvanced:public CCTableView{
private:
	CCPoint beginPoint;
	bool readyMoved;//准备好移动
public:
	 static CCTableViewAdvanced* create(CCTableViewDataSource* dataSource, CCSize size);

    static CCTableViewAdvanced* create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container);
	/** override functions */
    // optional
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
  //  virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
  //  virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);


};


#endif 