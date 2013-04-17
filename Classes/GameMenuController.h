#ifndef __GAME_MENU_CONTROLLER_H__
#define __GAME_MENU_CONTROLLER_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "vector"
#include "../extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameMenuController :public CCLayer
{
protected:
	enum Panel{
	panel_red=0,
	panel_yellow,
	panel_green,
	panel_blue,
}curPanel;
	CCSize winSize;
	CCScrollView* scrollView;
	float countDown;//倒计时
	char *str_time;//倒计时显示的字符串
	CCLabelBMFont *ttf_clock;//倒计时Label
	CCLabelBMFont *ttf_resin;//总树脂量
	unsigned int cur_resin;//当前树脂量
	int curPage;//当前页
	unsigned int space;//空白
	unsigned int nCount;//页数
	int scrollViewWidth_2;//scrollView视图宽的一半
	int tableWidth;//左侧面板宽度

	bool isScrolling;//是否滑动
	bool isMovingMap;//是否在移动地图
	bool isSliding;//是否正在滑动滑动条，调整浓度
	int slideStart;//滑动条初始位置
	int slideEnd;//滑动条最远可滑动

	CCPoint startPoint;//开始点
public:
	GameMenuController();
	virtual ~GameMenuController();
	CREATE_FUNC(GameMenuController);
	virtual bool init();
	//init后会调用; 若是加了CCTransitionScene后，在过渡场景开始后调用  
	void onEnter();
	void onEnterTransitionDidFinish();
	//onEnter以后将会调用此方法 若使用CCTransitionScene,将会在过渡效果结束以后调用此方法 
	void onExitTransitionDidStart();
	void onExit();// 节点调用dealloc方法之前将会调用此方法   
    //如果使用了CCTransitionScene,将会在过渡效果结束以后调用此方法  

	virtual void update(float dt);
	
    void menuGoBackCallback(CCObject* pSender);//点击返回按钮回调
    void menuCollectCallback(CCObject* pSender);//点击收集按钮回调
    void menuClickCallback(CCObject* pSender);//点击某地图回调
    void menuChooseResinCallback(CCObject* pSender);//点击树脂颜色选项卡回调
    void menuShopCallback(CCObject* pSender);//点击商店按钮回调
    void menuStartCallback(CCObject* pSender);//点击开始按钮回调
	void menuWikiCallBack(CCObject* pSender);//点击百科按钮回调

	void step(float dt);


	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	virtual void  keyBackClicked();
	virtual void keyMenuClicked();

	void adjustScrollView(float distance);

	void initMapScrollView();//初始化右侧面板
	void initLeftTable();//初始化左侧面板
	CCLayer* initLeftTopPanel();//初始化左上栏
	CCLayer* initLeftMidPanel();//lefttable的中间部分
	CCMenu* initLeftBottomdPanel();//lefttable的下方部分
	void initResinVol();//初始化树脂量
	void save();//存档

public:
	static CCScene* scene();
};

#endif //__GAME_MENU_CONTROLLER_H__
