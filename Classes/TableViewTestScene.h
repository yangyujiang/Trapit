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
	int tableWidth;//tableView�Ŀ��
	int cellWidth;//cell�Ŀ��
	int curIdx;//��ǰ����
	bool isScrolling;//�Ƿ����ڻ���
	CCPoint startPos;//��ʼλ��
	CCPoint endPos;//����λ��
	float draggingTime;//�϶�ʱ��

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
