#ifndef __STATIC_LAYER_H__
#define __STATIC_LAYER_H__

#include "cocos2d.h"
#include "Constant.h"
#include "../extensions/cocos-ext.h"
#include "GameResinBallModel.h"
#include "SkillSlotAdapter.h"
USING_NS_CC;
USING_NS_CC_EXT;

//这是一个静止层，此层的物体相对于屏幕一般静止不动
class StaticLayer :public cocos2d::CCLayer
{
protected:
	CCMenuItem *pCloseItem;
	CCMenuItem *pPauseItem;//暂停按钮
	GameResinBallModel* model;
	CCLabelBMFont* ttf_resin;
	CCLabelBMFont* ttf_insectNum;
	SkillSlotAdapter* skills;//技能栏
public:
	InnerStage innerStage;//调试用
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
	
	void playLevelUp(int curLevel);//播放升级动画
	void endPlayLevelUp();
	void playGameOver();//播放gameOver动画
	void endPlayGameOver();//结束播放gameOver动画

	// a selector callback
    void menuCloseCallback(CCObject* pSender);
	void menuPauseCallBack(CCObject* pSender);
	void menuGoBackCallBack(CCObject* pSender);
	void menuFreezeCallBack(CCObject* pSender);
	void keepStill(CCPoint lastPositionOfScene);//保持不动
	void draw();

	void setResinCount(GameResinBallModel* resin);
};

#endif //__STATIC_LAYER_H__
