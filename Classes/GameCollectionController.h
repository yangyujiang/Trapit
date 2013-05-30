#ifndef __GAME_COLLECTION_CONTROLLER_H__
#define __GAME_COLLECTION_CONTROLLER_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "vector"
#include "StaticLayer.h"
#include "Constant.h"
#include "SkillSlotAdapter.h"
#include "Amber.h"

USING_NS_CC;

const int TAG_LIST=100;//�б��
const int TAG_CUR_AMBER=101;//
const int TAG_CUR_AMBER_MOVABLE=102;
const int TAG_MOVETO=103;//moveTo action

class GameCollectionController :public CCLayer
{
public:
	int listWidth;
	CCSize winSize;
	unsigned int preIdx;
	bool isMoving;//�Ƿ������϶�����
	CCSprite* amberAddLayer;
	CCArray *_slots;//�������ܲ�
	SkillSlotAdapter* skillSlot;//���ܲ����������
	CCSprite* movableSkill;//���ƶ�����
public:
	GameCollectionController();
	virtual ~GameCollectionController();
	CREATE_FUNC(GameCollectionController);
	virtual bool init();
	//init������; ���Ǽ���CCTransitionScene���ڹ��ɳ�����ʼ�����  
	void onEnter();
	void onEnterTransitionDidFinish();
	//onEnter�Ժ󽫻���ô˷��� ��ʹ��CCTransitionScene,�����ڹ���Ч�������Ժ���ô˷��� 
	void onExitTransitionDidStart();
	void onExit();// �ڵ����dealloc����֮ǰ������ô˷���   
    //���ʹ����CCTransitionScene,�����ڹ���Ч�������Ժ���ô˷���  
	
	virtual void  keyBackClicked();

	virtual void update(float dt);
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	void menuGoBackCallback(CCObject* pSender);

	CCSprite* initbottomLayer();
	CCSprite* initTopTitle();//��ʼ������������
	void initSkillColumn();//��ʼ������չʾ��
	CCSprite* initAmplificationIcon(const char *name,float amplification);//��ʼ������ͼ��

	static CCSprite* getCurSprite(unsigned int idx);

public:
	static CCScene* scene();
};

#endif //__GAME_COLLECTION_CONTROLLER_H__
