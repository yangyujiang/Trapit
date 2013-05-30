#ifndef __STATIC_LAYER_H__
#define __STATIC_LAYER_H__

#include "cocos2d.h"
#include "Constant.h"
#include "../extensions/cocos-ext.h"
#include "GameResinBallModel.h"
#include "SkillSlotAdapter.h"
USING_NS_CC;
USING_NS_CC_EXT;

//����һ����ֹ�㣬�˲�������������Ļһ�㾲ֹ����
class StaticLayer :public cocos2d::CCLayer
{
protected:
	CCMenuItem *pCloseItem;
	CCMenuItem *pPauseItem;//��ͣ��ť
	GameResinBallModel* model;
	CCLabelBMFont* ttf_resin;
	CCLabelBMFont* ttf_insectNum;
	SkillSlotAdapter* skills;//������
public:
	InnerStage innerStage;//������
public:
	StaticLayer();
	virtual ~StaticLayer();

	virtual bool init();
	CREATE_FUNC(StaticLayer);

	void initControlSlider(GameResinBallModel* model);
	void ballGValueChanged(CCObject* pSender,CCControlEvent event);
	void ballUValueChanged(CCObject* pSender,CCControlEvent event);
	void blockGValueChanged(CCObject* pSender,CCControlEvent event);
	void blockUValueChanged(CCObject* pSender,CCControlEvent event);
	void ballMaxVValueChanged(CCObject* pSender,CCControlEvent event);
	
	void playLevelUp(int curLevel);//������������
	void endPlayLevelUp();
	void playGameOver();//����gameOver����
	void endPlayGameOver();//��������gameOver����

	// a selector callback
    void menuCloseCallback(CCObject* pSender);
	void menuPauseCallBack(CCObject* pSender);
	void menuGoBackCallBack(CCObject* pSender);
	void menuFreezeCallBack(CCObject* pSender);
	void keepStill(CCPoint lastPositionOfScene);//���ֲ���
	void draw();

	void setResinCount(GameResinBallModel* resin);
};

#endif //__STATIC_LAYER_H__
