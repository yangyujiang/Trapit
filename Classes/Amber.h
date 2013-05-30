//������
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
	CC_SYNTHESIZE(bool ,enable,Enable);//�Ƿ���ã����Ƿ��ô˳ɾͣ�
	CC_SYNTHESIZE(bool ,used,Used);//�Ƿ���ʹ�ã�
	CC_SYNTHESIZE(char* ,image,ImageName);//ͼƬ����
	CC_SYNTHESIZE(const char* ,descrip,Descrip);//����
	CC_SYNTHESIZE(char* ,imageSkill,SkillImageName);//����ͼ��
	CC_SYNTHESIZE(char* ,descripSkill,SkillDescrip);//���ܽ���
	CC_SYNTHESIZE(char* ,nameSkill,SkillName);//��������
	CC_SYNTHESIZE(char* ,descripHowToGet,DescripHowToGet);//��εõ�
/*	CC_PROPERTY_READONLY(unsigned int,id,Id);
	CC_PROPERTY(bool, enable,Enable);//�Ƿ���ã����Ƿ��ô˳ɾͣ�
	CC_PROPERTY(bool, used,Used);//�Ƿ���ʹ�ã�
	CC_PROPERTY_READONLY(char*,image,IamgeName);//ͼƬ����
	CC_PROPERTY_READONLY(char*,descrip,Descrip);//����
	CC_PROPERTY_READONLY(char*,imageSkill,ImageSkillName);//����ͼ��
	CC_PROPERTY_READONLY(char*, descripSkill,DescripSkill);//���ܽ���
	CC_PROPERTY_READONLY(char*, nameSkill,SkillName);//��������
	CC_PROPERTY_READONLY(char*, descripHowToGet,DescripHowToGet);//��εõ�
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
