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

	unsigned int id;
	bool enable;//�Ƿ���ã����Ƿ��ô˳ɾͣ�
	bool used;//�Ƿ���ʹ�ã�
	const char* image;//ͼƬ����
	const char* descrip;//����
	const char* imageSkill;//����ͼ��
	const char* descripSkill;//���ܽ���
	const char* nameSkill;//��������
	const char* descripHowToGet;//��εõ�
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
	
	static vector<Amber*> getAmbersFromSQLite();

};

#endif //__AMBER__
