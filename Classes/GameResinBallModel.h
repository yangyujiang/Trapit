#ifndef __GAME_RESIN_BALL_MODEL_H__
#define __GAME_RESIN_BALL_MODEL_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "Box2D\Box2D.h"
#include "GlobalDef.h"
#include "GameInsectModel.h"

USING_NS_CC;
//虫子模型类，处理虫子运动等
class GameResinBallModel :public cocos2d::CCNode
{
private:
	GameModelDelegate* _observerDelegate;//树脂球模型的观察者，一般为树脂球的view对象
protected:
	b2World* _world;
	vector<b2Body*> _blocks;//圆环链条的方块数组
	vector<b2Joint*> _blockJoints;//方块之间的旋转关节的存储数组
	CC_PROPERTY_READONLY(b2Body*,_ballBody,BallBody);//实心球刚体
	//cocos2d::CCPoint _position;//物体所在位置,即实心圆的圆心坐标，单位像素,即为此Node位置,故不需再定义
	float32 _currentBlockHeight;//小方块长度
	float32 _currentBlockWidth;//当前小方块宽度
	CC_PROPERTY_READONLY(float32,_radius,Radius);//外围半径
	CC_PROPERTY_READONLY(float,_ballRadius,BallRadius);//实心球半径
	CCPoint lastPosition;//上一帧的位置

	b2Fixture* _sensorBall;//代表树脂球与虫子的碰撞
protected:
	float attack;//攻击力（每秒）
	//float coverInsectTimeCount;//覆盖住虫子的时间累加
	//float isCoverInsect;//是否碾住虫子？
	GameInsectModel* coveredInsect;//被覆盖的虫子

public:
	GameResinBallModel();
	virtual ~GameResinBallModel();

	virtual bool init(b2World* world);
	CREATE_FUNC_ONE_PARAM(GameResinBallModel,b2World*,world);

	void initBallBody(float32 radius);//实心圆半径
	void createCircleBridge(int number,float32 radius);//创建环链
	void shrinkResinBallBody(float32 scale);//缩小树脂球模型刚体
	
	void accelerateCallBack(cocos2d::CCAcceleration* pAccelerationValue);//加速器变化时（倾斜手机屏幕）的回调函数

	void update(float dt);
	CCPoint getDeltaPosition();//两次位置变化的偏移

	void initObserver(GameModelDelegate* pDelegate);//初始化观察者，一般为与Model对应的View

public:
	void attackInsect(GameInsectModel* insect,float dt);//攻击到虫子
	void myDraw();//调试用
	
public:
	void beginContact(GameInsectModel* insect);
	void endContact();
	void handleContactWithInsect(float dt);//处理与虫子的碰撞事件
};

#endif //__GAME_RESIN_BALL_MODEL_H__
