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

	char name[50];
	CC_SYNTHESIZE(unsigned int, id,Id);
	CC_SYNTHESIZE(bool ,enable,Enable);//是否可用（即是否获得此成就）
	CC_SYNTHESIZE(bool ,used,Used);//是否已使用？
	CC_SYNTHESIZE(char* ,image,ImageName);//图片名称
	CC_SYNTHESIZE(const char* ,descrip,Descrip);//描述
	CC_SYNTHESIZE(char* ,imageSkill,SkillImageName);//技能图标
	CC_SYNTHESIZE(char* ,descripSkill,SkillDescrip);//技能介绍
	CC_SYNTHESIZE(char* ,nameSkill,SkillName);//技能名称
	CC_SYNTHESIZE(char* ,descripHowToGet,DescripHowToGet);//如何得到
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
	
//	static vector<Amber*> getAmbersFromSQLite();
	static vector<Amber*> getAmbers(int count=20);
	static bool getEnableForKey(int id);
	static bool getUsedForKey(int id);
	static void writeEnableForKey(int id,bool p);
	static void writeUsedForKey(int id,bool p);
	static void writeAmbers();

};

#endif //__AMBER__
