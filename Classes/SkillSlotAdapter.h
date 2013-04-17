//���ܲ۵������࣬�̳�CCSprite
//
#ifndef __SKILL_SLOT__
#define __SKILL_SLOT__

#include "cocos2d.h"
#include "GlobalDef.h"

USING_NS_CC;

class SkillSlotAdapter :public cocos2d::CCSprite
{
private:
	CCArray *_slots;//���ܲ�����
	unsigned int _count;//���ܲ�����
	unsigned int _space;//�������ܲ�֮��ļ��

public: 
	SkillSlotAdapter();
	virtual ~SkillSlotAdapter();
	virtual bool init();
	CREATE_FUNC(SkillSlotAdapter);
	
	void addSkillIfOverLap(CCSprite* skill);
};

#endif //__SKILL_SLOT__
