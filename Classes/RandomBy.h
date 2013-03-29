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
	int _moveState;//�˶�״̬��0��ʾֱ�ߣ�1��ʾ����
	float _sita;//΢���ı�ֵ
	float _countTime;
	float _preChangeTime;
	const float ODDS;//�ı��˶�״̬�ļ���
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


// ����һ���ṹ������ȷ����Բ�Ĳ���   
typedef struct _lrTuoyuanConfig {  
    //���ĵ�����   
    CCPoint centerPosition;  
    //��Բa��������б��   
    float aLength;  
    //��Բc�������ǵױ�   
    float cLength;  
} lrTuoyuanConfig;  

class   LRTuoyuanBy : public CCActionInterval  
{  
public:  
    //�á���������ʱ�䡱�͡���Բ���Ʋ�������ʼ������   
    bool initWithDuration(float t, const lrTuoyuanConfig& c);  
    virtual void startWithTarget(cocos2d::CCNode *pTarget);
    virtual void update(float time);//����update���������ϵ��趨����   
public:  
    //�á���������ʱ�䡱�͡���Բ���Ʋ�������������   
    static LRTuoyuanBy *create(float t, const lrTuoyuanConfig& c);  
  
protected:  
    lrTuoyuanConfig m_sConfig;  
    CCPoint m_startPosition;  
    CCPoint s_startPosition;  

	CCPoint m_lastPosition;//��ʼλ��
};  

static inline float tuoyuanXat( float a, float bx, float c, float t )//����X����   
{  
    //��������   
    return -a*cos(2*3.1415926*t)+a;  
}  
static inline float tuoyuanYat( float a, float by, float c, float t )//����Y����   
{  
    float b = sqrt(powf(a, 2) - powf(c, 2));//��Ϊ֮ǰ����Ĳ����ǽ���c�����Ƕ̰���b��������Ҫ�����b   
    //��������   
    return b*sin(2*3.1415926*t);  
}  



#endif //__RANDOM_BY_H__