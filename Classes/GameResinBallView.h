#ifndef __GAME_RESIN_BALL_VIEW_H__
#define __GAME_RESIN_BALL_VIEW_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "GlobalDef.h"
#include "GameResinBallModel.h"
#include "Constant.h"

USING_NS_CC;

class GameResinBallView :public CCLayer,public GameModelDelegate
{
private:
	GameResinBallViewDelegate* pViewDelegate;//代理
protected:
	CCSprite* _ResinBall;//树脂球图片
	GameResinBallModel* _ResinBallModel;
	vector<CCSprite*> blocks;//小方块图片资源

	CCMotionStreak* streak;//尾巴特效

public:
	GameResinBallView();
	virtual ~GameResinBallView();

	virtual bool init(GameResinBallViewDelegate* pViewDelegate,GameResinBallModel* gameResinBall);
	LAYER_CREATE_FUNC_DOUBLE_PARAM(GameResinBallView,GameResinBallViewDelegate*,pViewDelegate,GameResinBallModel*,gameResinBall);

	virtual void testModelDelegate();

	void update(float dt);

	//监听触屏事件
	void ccTouchesBegan(cocos2d::CCSet *touched,cocos2d::CCEvent *event);
	void ccTouchesMoved(cocos2d::CCSet *touches,cocos2d::CCEvent *event);
	void ccTouchesCancelled(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
	void ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event);

	void onEnter();
	void onExit();

};

const float scaleVToMap=0.1f;//地板移动速度与地图移动速度的比例
class MapLayer:public CCLayer
{
protected:
	CCSprite* _map;

	CCSprite* _floor[4];
public:
	InnerStage innerStage;
public:
	MapLayer();
	virtual ~MapLayer();

	virtual bool init();
	CREATE_FUNC(MapLayer);

	void updateMap(CCPoint nextPosition);//更新地图位置
	void updateFloor(CCPoint delta);//更新地图位置，参数为树脂球位置变化量
};



#endif //__GAME_RESIN_BALL_VIEW_H__
