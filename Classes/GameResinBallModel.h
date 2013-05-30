#ifndef __GAME_RESIN_BALL_MODEL_H__
#define __GAME_RESIN_BALL_MODEL_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "Box2D\Box2D.h"
#include "GlobalDef.h"
#include "GameInsectModel.h"


const float32 LIMIT_ANGLE=CC_DEGREES_TO_RADIANS(30);//������ת�ؽڵ���ת�Ƕ�
 //float32 BALL_FRICTION=0.1f;//ʵ����������Ħ��ϵ��
 //float32 BALL_GRAVITY=150;//ʵ����������N��,�̶�����
 //float32 BLOCK_FRICTION=2.f;//0.6f;//ÿһ��С����������Ħ��ϵ��
 //float32 BLOCK_GRAVITY=0.8f;//ÿһ��С�����������N�����̶�����
const float32 g=10;//ģ������ϵ��
 //float32 MAX_VELOCITY=18;//������֬�������ٶȣ���/�룩

USING_NS_CC;
//����ģ���࣬��������˶���
class GameResinBallModel :public cocos2d::CCNode
{
public:
	
 float32 BALL_FRICTION;//ʵ����������Ħ��ϵ��
 float32 BALL_GRAVITY;//ʵ����������N��,�̶�����
 float32 BLOCK_FRICTION;//0.6f;//ÿһ��С����������Ħ��ϵ��
 float32 BLOCK_GRAVITY;//ÿһ��С�����������N�����̶�����
 float32 MAX_VELOCITY;//������֬�������ٶȣ���/�룩
private:
	GameModelDelegate* _observerDelegate;//��֬��ģ�͵Ĺ۲��ߣ�һ��Ϊ��֬���view����
protected:
	b2World* _world;
	vector<b2Body*> _blocks;//Բ�������ķ�������
	vector<b2Joint*> _blockJoints;//����֮�����ת�ؽڵĴ洢����
	CC_PROPERTY_READONLY(b2Body*,_ballBody,BallBody);//ʵ�������
	//cocos2d::CCPoint _position;//��������λ��,��ʵ��Բ��Բ�����꣬��λ����,��Ϊ��Nodeλ��,�ʲ����ٶ���
	float32 _currentBlockHeight;//С���鳤��
	float32 _currentBlockWidth;//��ǰС������
	CC_PROPERTY_READONLY(float32,_radius,Radius);//��Χ�뾶
	CC_PROPERTY_READONLY(float,_ballRadius,BallRadius);//ʵ����뾶
	CCPoint lastPosition;//��һ֡��λ��

	b2Fixture* _sensorBall;//������֬������ӵ���ײ
protected:
	float attack;//��������ÿ�룩
	//float coverInsectTimeCount;//����ס���ӵ�ʱ���ۼ�
	//float isCoverInsect;//�Ƿ���ס���ӣ�
	BaseInsect* coveredInsect;//�����ǵĳ���
//����ӵĽ���
public:
	void attackedByEnemy(int attack);

public:
	CC_SYNTHESIZE(int ,reached,Reached);//����֬��С�ڴ���ʱ������
	CC_SYNTHESIZE(float,blood,Blood);
	int countBlood;//�ۼ����ĵ���֬
	bool usedUp();//�Ƿ�Ĺ���֬
	void shrinkResinByBlood();//���ݵ�ǰ��֬��������֬
	GameResinBallModel();
	virtual ~GameResinBallModel();

	virtual bool init(b2World* world);
	CREATE_FUNC_ONE_PARAM(GameResinBallModel,b2World*,world);
	void clean();

	void initBallBody(float32 radius);//ʵ��Բ�뾶
	void createCircleBridge(int number,float32 radius);//��������
	void shrinkResinBallBody(float32 scale);//��С��֬��ģ�͸���

	void initPolygonTexture();
	void updatePolygonTexture();
	
	void accelerateCallBack(cocos2d::CCAcceleration* pAccelerationValue);//�������仯ʱ����б�ֻ���Ļ���Ļص�����
	void reduceBodyVel(float scale);//�����ٶȼ���Ϊԭ����scale

	void update(float dt);
	CCPoint getDeltaPosition();//����λ�ñ仯��ƫ��

	void initObserver(GameModelDelegate* pDelegate);//��ʼ���۲��ߣ�һ��Ϊ��Model��Ӧ��View

public:
	void attackInsect(BaseInsect* insect,float dt);//����������
	void myDraw();//������
	
public:
	void beginCover(BaseInsect* insect);
	void endCover();
	void coveringInsect(float dt);//��������ӵ���ײ�¼�
};

#endif //__GAME_RESIN_BALL_MODEL_H__
