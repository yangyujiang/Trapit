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
	float countDown;//����ʱ
	char *str_time;//����ʱ��ʾ���ַ���
	CCLabelBMFont *ttf_clock;//����ʱLabel
	CCLabelBMFont *ttf_resin;//����֬��
	unsigned int cur_resin;//��ǰ��֬��
	int curPage;//��ǰҳ
	unsigned int space;//�հ�
	unsigned int nCount;//ҳ��
	int scrollViewWidth_2;//scrollView��ͼ���һ��
	int tableWidth;//��������

	bool isScrolling;//�Ƿ񻬶�
	bool isMovingMap;//�Ƿ����ƶ���ͼ
	bool isSliding;//�Ƿ����ڻ���������������Ũ��
	int slideStart;//��������ʼλ��
	int slideEnd;//��������Զ�ɻ���

	CCPoint startPoint;//��ʼ��
public:
	GameMenuController();
	virtual ~GameMenuController();
	CREATE_FUNC(GameMenuController);
	virtual bool init();
	//init������; ���Ǽ���CCTransitionScene���ڹ��ɳ�����ʼ�����  
	void onEnter();
	void onEnterTransitionDidFinish();
	//onEnter�Ժ󽫻���ô˷��� ��ʹ��CCTransitionScene,�����ڹ���Ч�������Ժ���ô˷��� 
	void onExitTransitionDidStart();
	void onExit();// �ڵ����dealloc����֮ǰ������ô˷���   
    //���ʹ����CCTransitionScene,�����ڹ���Ч�������Ժ���ô˷���  

	virtual void update(float dt);
	
    void menuGoBackCallback(CCObject* pSender);//������ذ�ť�ص�
    void menuCollectCallback(CCObject* pSender);//����ռ���ť�ص�
    void menuClickCallback(CCObject* pSender);//���ĳ��ͼ�ص�
    void menuChooseResinCallback(CCObject* pSender);//�����֬��ɫѡ��ص�
    void menuShopCallback(CCObject* pSender);//����̵갴ť�ص�
    void menuStartCallback(CCObject* pSender);//�����ʼ��ť�ص�
	void menuWikiCallBack(CCObject* pSender);//����ٿư�ť�ص�

	void step(float dt);


	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	virtual void  keyBackClicked();
	virtual void keyMenuClicked();

	void adjustScrollView(float distance);

	void initMapScrollView();//��ʼ���Ҳ����
	void initLeftTable();//��ʼ��������
	CCLayer* initLeftTopPanel();//��ʼ��������
	CCLayer* initLeftMidPanel();//lefttable���м䲿��
	CCMenu* initLeftBottomdPanel();//lefttable���·�����
	void initResinVol();//��ʼ����֬��
	void save();//�浵

public:
	static CCScene* scene();
};

#endif //__GAME_MENU_CONTROLLER_H__
