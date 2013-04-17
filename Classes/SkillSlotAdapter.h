//技能槽的适配类，继承CCSprite
//
#ifndef __SKILL_SLOT__
#define __SKILL_SLOT__

#include "cocos2d.h"
#include "GlobalDef.h"

USING_NS_CC;

class SkillSlotAdapter :public cocos2d::CCSprite
{
private:
	CCArray *_slots;//技能槽数组
	unsigned int _count;//技能槽数量
	unsigned int _space;//两个技能槽之间的间隔

public: 
	SkillSlotAdapter();
	virtual ~SkillSlotAdapter();
	virtual bool init();
	CREATE_FUNC(SkillSlotAdapter);
	
	void addSkillIfOverLap(CCSprite* skill);
};

#endif //__SKILL_SLOT__
