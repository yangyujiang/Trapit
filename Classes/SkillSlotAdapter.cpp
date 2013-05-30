#include "Constant.h"
#include "SkillSlotAdapter.h"
#include "B2EasyBox2D.h"
#include "RandomBy.h"
#include "GameCollectionController.h"

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
	copy->setTag(sprite->getTag());//这里的tag就是技能的id

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
			char name[20]={0};
			sprintf(name,"used_id_%d",i+1);
			int amberId=CCUserDefault::sharedUserDefault()->getIntegerForKey(name,-1);
			if(amberId>=0){//此技能框里有技能，且技能id为amberId
				CCSprite* spriteSkill=GameCollectionController::getCurSprite(amberId);
				spriteSkill->setScale(1);
				//spriteSkill->setScale(0.8);
				spriteSkill->setPosition(ccp(slot->getContentSize().width/2,slot->getContentSize().height/2));
				slot->addChild(spriteSkill,1,TAG_SKILL);
			}
		}

		this->setContentSize(CCSizeMake(slot->getPositionX()+slot->getContentSize().width,slot->getContentSize().height));
		//	CCLog("slotsWidth:%f",this->getContentSize().width);
		this->setAnchorPoint(ccp(0.5,0.5));

		pRet=true;
	}while(0);

	return pRet;
}

void SkillSlotAdapter::exchangeIfExisted(unsigned int skillId){
	for(unsigned int i=0;i<_slots->count();i++){
		CCNode* slot=(CCNode*)_slots->objectAtIndex(i);
		CCArray* children=slot->getChildren();
		if(children==NULL) continue;
		CCSprite *skill=(CCSprite*)(children->lastObject());
		if(skill!=NULL&&skill->getTag()==skillId){
			slot->removeAllChildrenWithCleanup(true);//清除
			break;
		}
	}
}


int SkillSlotAdapter::addSkillIfOverLap(CCSprite* skill){
	int boxId=0;//
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
			this->exchangeIfExisted(skill->getTag());
			CCSprite* newSkill=copySprite(skill);
			newSkill->setAnchorPoint(ccp(0.5,0.5));
			newSkill->setPosition(ccp(slot->getContentSize().width/2,slot->getContentSize().height/2));
			boxId=i+1;//技能放入了哪个框
			slot->addChild(newSkill,1);
			//newSkill->runAction(CCMoveTo::create(0.5,CCPointZero));
			break;
		}
	}
	return boxId;
}
