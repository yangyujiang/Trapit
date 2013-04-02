#ifndef __TABLEVIEWTESTSCENE_H__
#define __TABLEVIEWTESTSCENE_H__

#include "cocos2d.h"
#include "../extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

const int ADJUST_ANIM_VELOCITY=500;

class TableViewTestLayer : public cocos2d::CCLayer, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
public:
	vector<cocos2d::CCSprite*> _sprites;
	CCTableView* tableView;
	int tableCount;
	int tableWidth;//tableView的宽度
	int cellWidth;//cell的宽度
	int curIdx;//当前引用
	bool isScrolling;//是否正在滑动
	CCPoint startPos;//起始位置
	CCPoint endPos;//结束位置
	float draggingTime;//拖动时间

public:
	TableViewTestLayer();
	static CCScene* scene();
    virtual bool init();  
	void onEnter();
	void onExit();
   void adjustTableView();

    CREATE_FUNC(TableViewTestLayer);

	void update(float dt);
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){};

void menuCallBack(CCObject* pSender);
void endScrolling(float dt);
void clickedCell(int idx);
void shakeButton(int idx);
};

#endif // __TABLEVIEWTESTSCENE_H__
