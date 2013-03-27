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
	GameResinBallViewDelegate* pViewDelegate;//´úÀí
protected:
	CCSprite* _ResinBall;//Ê÷Ö¬ÇòÍ¼Æ¬
	GameResinBallModel* _ResinBallModel;
public:
	GameResinBallView();
	virtual ~GameResinBallView();

	virtual bool init(GameResinBallViewDelegate* pViewDelegate,GameResinBallModel* gameResinBall);
	LAYER_CREATE_FUNC_DOUBLE_PARAM(GameResinBallView,GameResinBallViewDelegate*,pViewDelegate,GameResinBallModel*,gameResinBall);

	virtual void testModelDelegate();

	void update(float dt);

	//¼àÌý´¥ÆÁÊÂ¼þ
	void ccTouchesBegan(cocos2d::CCSet *touched,cocos2d::CCEvent *event);
	void ccTouchesMoved(cocos2d::CCSet *touches,cocos2d::CCEvent *event);
	void ccTouchesCancelled(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
	void ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event);

};

#endif //__GAME_RESIN_BALL_VIEW_H__
