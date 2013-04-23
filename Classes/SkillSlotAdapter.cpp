#include "Constant.h"
#include "SkillSlotAdapter.h"
#include "B2EasyBox2D.h"
#include "RandomBy.h"

USING_NS_CC;

const int TAG_SKILL=101;
SkillSlotAdapter::SkillSlotAdapter(){
	CCSprite();
}
SkillSlotAdapter::~SkillSlotAdapter()
{
}

CCSprite* copySprite(CCSprite* sprite){
	CCSprite* copy=CCSprite::createWithSpriteFrame(sprite->displayFrame());
	copy->setAnchorPoint(sprite->getAnchorPoint());
	copy->setPosition(sprite->getPosition());
	copy->setVisible(true);

	return copy;
}

bool SkillSlotAdapter::init(){
	bool pRet=false;
	do{ 	
		CC_BREAK_IF(!CCSprite::init());

		_space=20;
		_count=3;
		_slots=CCArray::create();

		this->setAnchorPoint(CCPointZero);
		CCSprite* slot;
		for(unsigned int i=0;i<_count;i++){
			slot=CCSprite::createWithSpriteFrameName("slot.png");
			slot->setAnchorPoint(ccp(0,0));
			slot->setPosition(ccp((slot->getContentSize().width+_space)*i,0));
			//CCLog("slotWidth:%f",slot->getContentSize().width);
			this->addChild(slot);
			//_slots->addObject(slot);
		}

		this->setContentSize(CCSizeMake(slot->getPositionX()+slot->getContentSize().width,slot->getContentSize().height));
		//	CCLog("slotsWidth:%f",this->getContentSize().width);
		this->setAnchorPoint(ccp(0.5,0.5));

		pRet=true;
	}while(0);

	return pRet;
}


void SkillSlotAdapter::addSkillIfOverLap(CCSprite* skill){
	CCPoint skillPos=this->convertToNodeSpaceAR(skill->getPosition());
	
	float skillWidth=skill->getContentSize().width;
	float skillHeight=skill->getContentSize().height;
	//获得skill Sprite 的rect
	CCRect skillRect=CCRectMake(skillPos.x-skillWidth*skill->getAnchorPoint().x,skillPos.y-skillHeight*skill->getAnchorPoint().y,
		skill->getContentSize().width,skill->getContentSize().height);
	//if(!this->boundingBox().intersectsRect(skillRect)) return;////行不通

	_slots=this->getChildren();
	for(unsigned int i=0;i<_slots->count();i++){
		CCSprite *slot=(CCSprite*)_slots->objectAtIndex(i);
		
		if(slot->boundingBox().intersectsRect(skillRect)){
			if(slot->getChildrenCount()>0)
				slot->removeAllChildrenWithCleanup(true);
			CCSprite* newSkill=copySprite(skill);
			newSkill->setAnchorPoint(CCPointZero);
			newSkill->setPosition(CCPointZero);
			
			slot->addChild(newSkill,1,TAG_SKILL);
			//newSkill->runAction(CCMoveTo::create(0.5,CCPointZero));
			break;
		}
	}
}
