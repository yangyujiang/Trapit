#ifndef __GAME_RESIN_BALL_VIEW_H__
#define __GAME_RESIN_BALL_VIEW_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "GlobalDef.h"
#include "GameResinBallModel.h"

USING_NS_CC;

class GameResinBallView :public CCLayer,public GameModelDelegate
{
private:
	GameResinBallViewDelegate* pViewDelegate;//代理
protected:
	CCSprite* _ResinBall;//树脂球图片
	GameResinBallModel* _ResinBallModel;


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

};

const float scaleVeToMap=0.001f;//地板移动速度与地图移动速度的比例
class MapLayer:public CCLayer
{
protected:
	CCSprite* _map;
	struct innerStage{//内边界
		float top;
		float bottom;
		float left;
		float right;//与屏幕右边界的距离（像素）
		innerStage(){
		top=bottom=left=right=0;//默认全为零
		}	
	}innerStage;

	CCSprite* _floor[4];

public:
	MapLayer();
	virtual ~MapLayer();

	virtual bool init();
	CREATE_FUNC(MapLayer);

	void updateMap(CCPoint nextPosition);//更新地图位置

};



#endif //__GAME_RESIN_BALL_VIEW_H__
