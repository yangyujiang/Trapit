//琥珀类
//
#ifndef __AMBER__
#define __AMBER__

#include "cocos2d.h"
#include "Delegate.h"
#include "GlobalDef.h"

USING_NS_CC;

class Amber :public cocos2d::CCSprite
{
public:

	unsigned int id;
	bool enable;//是否可用（即是否获得此成就）
	bool used;//是否已使用？
	const char* image;//图片名称
	const char* descrip;//描述
	const char* imageSkill;//技能图标
	const char* descripSkill;//技能介绍
	const char* nameSkill;//技能名称
	const char* descripHowToGet;//如何得到
/*	CC_PROPERTY_READONLY(unsigned int,id,Id);
	CC_PROPERTY(bool, enable,Enable);//是否可用（即是否获得此成就）
	CC_PROPERTY(bool, used,Used);//是否已使用？
	CC_PROPERTY_READONLY(char*,image,IamgeName);//图片名称
	CC_PROPERTY_READONLY(char*,descrip,Descrip);//描述
	CC_PROPERTY_READONLY(char*,imageSkill,ImageSkillName);//技能图标
	CC_PROPERTY_READONLY(char*, descripSkill,DescripSkill);//技能介绍
	CC_PROPERTY_READONLY(char*, nameSkill,SkillName);//技能名称
	CC_PROPERTY_READONLY(char*, descripHowToGet,DescripHowToGet);//如何得到
	*/
public:
	Amber();
	virtual ~Amber();
	virtual bool init();
	CREATE_FUNC_NO_PARAM(Amber);
	
	static vector<Amber*> getAmbersFromSQLite();

};

#endif //__AMBER__
