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
	GameResinBallViewDelegate* pViewDelegate;//����
protected:
	CCSprite* _ResinBall;//��֬��ͼƬ
	GameResinBallModel* _ResinBallModel;
	vector<CCSprite*> blocks;//С����ͼƬ��Դ

	CCMotionStreak* streak;//β����Ч

public:
	GameResinBallView();
	virtual ~GameResinBallView();

	virtual bool init(GameResinBallViewDelegate* pViewDelegate,GameResinBallModel* gameResinBall);
	LAYER_CREATE_FUNC_DOUBLE_PARAM(GameResinBallView,GameResinBallViewDelegate*,pViewDelegate,GameResinBallModel*,gameResinBall);

	virtual void testModelDelegate();

	void update(float dt);

	//���������¼�
	void ccTouchesBegan(cocos2d::CCSet *touched,cocos2d::CCEvent *event);
	void ccTouchesMoved(cocos2d::CCSet *touches,cocos2d::CCEvent *event);
	void ccTouchesCancelled(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
	void ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event);

	void onEnter();
	void onExit();

};

const float scaleVToMap=0.1f;//�ذ��ƶ��ٶ����ͼ�ƶ��ٶȵı���
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

	void updateMap(CCPoint nextPosition);//���µ�ͼλ��
	void updateFloor(CCPoint delta);//���µ�ͼλ�ã�����Ϊ��֬��λ�ñ仯��
};



#endif //__GAME_RESIN_BALL_VIEW_H__
