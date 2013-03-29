#ifndef __RANDOM_BY_H__
#define __RANDOM_BY_H__

#include "cocos2d.h"

USING_NS_CC;

class CCRandomBy : public cocos2d::CCActionInterval
{
	protected:
    cocos2d::CCPoint m_endPosition;
    cocos2d::CCPoint m_startPosition;
    cocos2d::CCPoint m_delta;
	float m_velocity;
	int _moveState;//运动状态，0表示直线，1表示曲线
	float _sita;//微量改变值
	float _countTime;
	float _preChangeTime;
	const float ODDS;//改变运动状态的几率
public:
    /** initializes the action */
    bool initWithDuration(float duration, const float velocity);

    virtual CCObject* copyWithZone(cocos2d::CCZone* pZone);
    virtual void startWithTarget(cocos2d::CCNode *pTarget);
    virtual void update(float time);
	CCRandomBy():_moveState(0),_countTime(0),_preChangeTime(0),ODDS(0.8f){};
	void changeState();

public:
    /** creates the action */
    static CCRandomBy* create(float duration,  const float velocity);

};


//CCStepBack.h
class CCStepBack : public cocos2d::CCActionInterval
{
	protected:
    cocos2d::CCPoint m_endPosition;
    cocos2d::CCPoint m_startPosition;
    cocos2d::CCPoint m_delta;
	float m_velocity;
public:
    /** initializes the action */
    bool initWithDuration(float duration, const float velocity);

    virtual CCObject* copyWithZone(cocos2d::CCZone* pZone);
    virtual void startWithTarget(cocos2d::CCNode *pTarget);
    virtual void update(float time);
	CCStepBack(){};

public:
    /** creates the action */
    static CCStepBack* create(float duration,  const float velocity);

};


// 定义一个结构来包含确定椭圆的参数   
typedef struct _lrTuoyuanConfig {  
    //中心点坐标   
    CCPoint centerPosition;  
    //椭圆a长，三角斜边   
    float aLength;  
    //椭圆c长，三角底边   
    float cLength;  
} lrTuoyuanConfig;  

class   LRTuoyuanBy : public CCActionInterval  
{  
public:  
    //用“动作持续时间”和“椭圆控制参数”初始化动作   
    bool initWithDuration(float t, const lrTuoyuanConfig& c);  
    virtual void startWithTarget(cocos2d::CCNode *pTarget);
    virtual void update(float time);//利用update函数来不断的设定坐标   
public:  
    //用“动作持续时间”和“椭圆控制参数”创建动作   
    static LRTuoyuanBy *create(float t, const lrTuoyuanConfig& c);  
  
protected:  
    lrTuoyuanConfig m_sConfig;  
    CCPoint m_startPosition;  
    CCPoint s_startPosition;  

	CCPoint m_lastPosition;//开始位置
};  

static inline float tuoyuanXat( float a, float bx, float c, float t )//返回X坐标   
{  
    //参数方程   
    return -a*cos(2*3.1415926*t)+a;  
}  
static inline float tuoyuanYat( float a, float by, float c, float t )//返回Y坐标   
{  
    float b = sqrt(powf(a, 2) - powf(c, 2));//因为之前定义的参数是焦距c而不是短半轴b，所以需要计算出b   
    //参数方程   
    return b*sin(2*3.1415926*t);  
}  



#endif //__RANDOM_BY_H__