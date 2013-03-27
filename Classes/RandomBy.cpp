#include "RandomBy.h"

USING_NS_CC;

//CCRandomBy.cpp
//
CCRandomBy* CCRandomBy::create(float duration,  const float velocity)
{
    CCRandomBy *pRandomBy = new CCRandomBy();
    pRandomBy->initWithDuration(duration, velocity);
    pRandomBy->autorelease();

    return pRandomBy;
}

bool CCRandomBy::initWithDuration(float duration,  const float velocity)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_velocity = velocity;
        return true;
    }

    return false;
}

CCObject* CCRandomBy::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    CCRandomBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCRandomBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCRandomBy();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_velocity);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCRandomBy::startWithTarget(CCNode *pTarget)
{
	//CCLog("RandomBy:startWithTarget");
    CCActionInterval::startWithTarget(pTarget);
    m_startPosition = pTarget->getPosition();
	this->changeState();
	/*double degree=pTarget->getRotation()/180*M_PI;
	double distance=m_velocity*m_fDuration;
	m_delta = ccp(distance*cos(degree), distance*sin(degree));*/
}

void CCRandomBy::update(float time)
{
//	CCLog("%dRandomBy:running",m_pTarget->getTag());
//	CCLog("(%f,%f)",m_pTarget->getPositionX(),m_pTarget->getPositionY());
    if (m_pTarget)
    {
		if(time*m_fDuration-_preChangeTime>1){
			_preChangeTime+=1;
			this->changeState();
		}
		float newAngle=0;
		switch(_moveState){
		case 1:
			newAngle=m_pTarget->getRotation()+_sita;
			newAngle=(int)newAngle%360;
			//if(newAngle>180) newAngle=360-newAngle;
			//if(newAngle<-180) newAngle=360+newAngle;
			m_pTarget->setRotation(newAngle);//设置新的角度
	//		CCLog("newAngle:%f",newAngle);
		case 0:
			double degree=(m_pTarget->getRotation())/180*M_PI;
			double distance=m_velocity*(m_fDuration*time-_countTime);
			m_delta = ccp(distance*cos(degree), -distance*sin(degree));
			CCPoint newPosition=ccp(m_startPosition.x + m_delta.x ,m_startPosition.y + m_delta.y);
			m_pTarget->setPosition(newPosition);
			m_startPosition=newPosition;
			//CCLog("(%f)",distance);
			//CCLog("degree:%f",degree);
			//CCLog("(%f,%f)",m_delta.x,m_delta.y);
			_countTime=m_fDuration*time;
			break;
		}//switch
    }else CCLog("target is NULL");
}

void CCRandomBy::changeState(){
	
	if(CCRANDOM_0_1()>ODDS) return;

	_moveState=(_moveState+1)%2;
	switch(_moveState){
	case 0:

		break;
	case 1:
		_sita=180*CCRANDOM_MINUS1_1()*0.01f;
		break;
	default:
		break;
	}
}


//
//CCStepBack.cpp
//
CCStepBack* CCStepBack::create(float duration,  const float velocity)
{
    CCStepBack *pStepBack = new CCStepBack();
    pStepBack->initWithDuration(duration, velocity);
    pStepBack->autorelease();

    return pStepBack;
}

bool CCStepBack::initWithDuration(float duration,  const float velocity)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_velocity = velocity;
        return true;
    }

    return false;
}

CCObject* CCStepBack::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    CCStepBack* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCStepBack*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCStepBack();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_velocity);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCStepBack::startWithTarget(CCNode *pTarget)
{
	//CCLog("RandomBy:startWithTarget");
    CCActionInterval::startWithTarget(pTarget);
    m_startPosition = pTarget->getPosition();
	
	double degree=(-pTarget->getRotation())/180*M_PI;
	double distance=m_velocity*0.3*m_fDuration;//原有速度的0.3倍后退
	//CCLog("%f,%f",distance,degree);
	m_delta = ccp(distance*cos(degree), distance*sin(degree));
}

void CCStepBack::update(float time)
{
//	CCLog("%dCCStepBack:running",m_pTarget->getTag());
	//CCLog("(%f,%f)",m_pTarget->getPositionX(),m_pTarget->getPositionY());
	
    if (m_pTarget)
    {
		CCPoint newPosition=ccp(m_startPosition.x - m_delta.x*time ,m_startPosition.y - m_delta.y*time);
		m_pTarget->setPosition(newPosition);		
    }else CCLog("target is NULL");
}  

//
//TuoyuanBy
//
LRTuoyuanBy* LRTuoyuanBy::create(float t, const lrTuoyuanConfig& c)//利用之前定义的椭圆的三个参数初始化椭圆
{
	LRTuoyuanBy *pTuoyuanBy = new LRTuoyuanBy();
	pTuoyuanBy->initWithDuration(t, c);
	pTuoyuanBy->autorelease();

	return pTuoyuanBy;
}

bool LRTuoyuanBy::initWithDuration(float t, const lrTuoyuanConfig& c)
{
	if (CCActionInterval::initWithDuration(t))
	{
        m_sConfig = c;
		return true;
	}

	return false;
}
void LRTuoyuanBy::startWithTarget(CCNode* pTarget){
	 CCActionInterval::startWithTarget(pTarget);
    m_startPosition = pTarget->getPosition();
}

void LRTuoyuanBy::update(float time)
{
	if (m_pTarget)
	{
		m_lastPosition=m_pTarget->getPosition();

		CCPoint s_startPosition =m_sConfig.centerPosition;//中心点坐标
		float a = m_sConfig.aLength;
		float bx = m_sConfig.centerPosition.x;
		float by = m_sConfig.centerPosition.y;
		float c = m_sConfig.cLength;
		float x = tuoyuanXat(a, bx, c, time);//调用之前的坐标计算函数来计算出坐标值
		float y = tuoyuanYat(a, by, c, time);
		m_pTarget->setPosition(ccpAdd(s_startPosition, ccp(x-a, y)));//由于我们画计算出的椭圆你做值是以原点为中心的，所以需要加上我们设定的中心点坐标
	
		//计算当前角度
		CCPoint nowPosition=m_pTarget->getPosition();
		float angle=atan((nowPosition.y-m_startPosition.y)/(nowPosition.x-m_startPosition.x));
		CCLog("angle:%f",angle);
		m_pTarget->setRotation(angle*180/M_PI);
	}
}
