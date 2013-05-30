#include "Amber.h"
#include "B2EasyBox2D.h"
#include "RandomBy.h"
#include "sqlite3.h"
#include "Constant.h"
#include "MyMath.h"

USING_NS_CC;

const char* COLLECTION="collection";
//��COLLECTION���е�����
const unsigned int _id=0;
const unsigned int __enable=1;
const unsigned int _used=2;
const unsigned int _imageName=3;
const unsigned int _descrip=4;
const unsigned int _imageSkillName=5;
const unsigned int _nameSkill=6;
const unsigned int _descripSkill=7;
const unsigned int _descripHowToGet=8;
	
static vector<Amber*> AMBERS;

//const char* descrip[]={MyMath::WStrToUTF8(L"��ŭ����������").c_str(),""};

Amber::Amber()
{
	nameSkill=new char[50];
}


Amber::~Amber()
{
	AMBERS.clear();
}


bool Amber::init(){
	bool pRet=false;
	do{ 	
		CC_BREAK_IF(!CCSprite::init());

		pRet=true;
	}while(0);

	return pRet;
}

int loadRecord( void * para, int n_column, char ** column_value, char ** column_name ) 
{ 
            CCLog("ID=%s,enable=%s,descripSkill=%s",column_value[0],column_value[__enable],column_value[_descripSkill]); 
            Amber *amber=(Amber*)para;
			amber->setId(CCString::createWithFormat(column_value[_id])->intValue());
			//CCLog("%d",amber->id);
			amber->setEnable(CCString::createWithFormat(column_value[__enable])->intValue()?1:0);
			amber->setUsed(CCString::createWithFormat(column_value[_used])->intValue()?1:0);
			amber->setImageName(column_value[_imageName]);
			amber->setDescrip(column_value[_descrip]);
			amber->setSkillImageName(column_value[_imageSkillName]);
			amber->setSkillDescrip(column_value[_descripSkill]);
			amber->setSkillName(column_value[_nameSkill]);
			amber->setDescripHowToGet(column_value[_descripHowToGet]);
			CCLog("id=%d",amber->getId()); 
			CCLog("enable=%d",amber->getEnable()); 
			CCLog("used=%d",amber->getUsed()); 
			CCLog("imageName=%s",amber->getImageName()); 
			CCLog("descrip=%s",amber->getDescrip()); 
			CCLog("imageSkill=%s",amber->getSkillImageName()); 
			CCLog("descripSkill=%s",amber->getSkillDescrip()); 
			CCLog("nameSkill=%s",amber->getSkillName()); 
			CCLog("descripHowToGet=%s",amber->getDescripHowToGet());
	AMBERS.push_back(amber);
			return 0; 
} 
int loadRecordCount( void * para, int n_column, char ** column_value, char ** column_name ) 
{
            int *count=(int*)para; 
			*count+=1;
           // *count=n_column; 
			CCLog("column_value:%s,%s",column_value[0],column_value[1]);
			bool enable=column_value[__enable]?1:0;
			Amber *amber=Amber::create();
			amber->setId(CCString::createWithFormat(column_value[_id])->intValue());
			//CCLog("%d",amber->id);
			amber->setEnable(CCString::createWithFormat(column_value[__enable])->intValue()?1:0);
			amber->setUsed(CCString::createWithFormat(column_value[_used])->intValue()?1:0);
			amber->setImageName(column_value[_imageName]);
			amber->setDescrip(column_value[_descrip]);
			amber->setSkillImageName(column_value[_imageSkillName]);
			amber->setSkillDescrip(column_value[_descripSkill]);
			amber->setSkillName(column_value[_nameSkill]);
		//	char name[50];
		//	strcpy(name,column_value[_nameSkill]);
		//	amber->setSkillName(name);
			amber->setDescripHowToGet(column_value[_descripHowToGet]);
			AMBERS.push_back(amber);
			CCLog("id=%d",amber->getId()); 
			CCLog("enable=%d",amber->getEnable()); 
			CCLog("used=%d",amber->getUsed()); 
			CCLog("imageName=%s",amber->getImageName()); 
			CCLog("descrip=%s",amber->getDescrip()); 
			CCLog("imageSkill=%s",amber->getSkillImageName()); 
			CCLog("descripSkill=%s",amber->getSkillDescrip()); 
			CCLog("nameSkill=%s",amber->getSkillName()); 
			CCLog("descripHowToGet=%s",amber->getDescripHowToGet());
            return 0; 
} 
/*
vector<Amber*> Amber::getAmbersFromSQLite(){
	//vector<Amber*> ambers;
	
	sqlite3 *pDB = NULL;//���ݿ�ָ�� 
	char * errMsg = NULL;//������Ϣ 
	std::string sqlstr;//SQLָ�� 
	char sql_str[1000]={0};
	int result;//sqlite3_exec����ֵ 
 
	//��һ�����ݿ⣬��������ݿⲻ���ڣ��򴴽�һ�����ݿ��ļ� 
	std::string sqlpath= CCFileUtils::sharedFileUtils()->getWriteablePath()+"data.db";
	CCLog("%s",sqlpath.c_str());
	result = sqlite3_open(sqlpath.c_str(), &pDB); 
	if( result != SQLITE_OK ) 
	      CCLog( "�����ݿ�ʧ�ܣ�������:%d ������ԭ��:%s\n" , result, errMsg ); 
	else CCLog("�����ݿ�ɹ�");
  
	sprintf(sql_str, "create table %s( ID integer primary key autoincrement,enable integer,used integer, image nvarchar(32),descrip nvarchar(32),imageSkill nvarchar(32),nameSkill nvarchar(32),descripSkill nvarchar(64),descripHowToGet nvarchar(32) )",COLLECTION);
	 //����������IDΪ���������Զ����� 
	result=sqlite3_exec( pDB,sql_str, NULL, NULL, &errMsg );
	if( result != SQLITE_OK ) 
	      CCLog( "������ʧ�ܣ�������:%d ������ԭ��:%s\n" , result, errMsg ); 
	else CCLog("������ɹ�");
	//std::string str=MyMath::WStrToUTF8(L"insert into collection(enable,used,image,descrip,imageSkill,nameSkill,descripSkill,descripHowToGet) values(1,0,'amber_1.png','��ŭ����������','amber_1.png','����֮ŭ','ʹ�ú�,����������빥��״̬.���������ָ','����빥��ʱ���׳���')");
	sprintf(sql_str,"insert into %s(enable,used,image,descrip,imageSkill,nameSkill,descripSkill,descripHowToGet) values(1,0,'amber_1.png','��ŭ����������','amber_1.png','����֮ŭ','ʹ�ú�,����������빥��״̬.���������ָ','����빥��ʱ���׳���')",COLLECTION);
	//sprintf(sql_str,"%s",str.c_str());
	//�������� 
	//sqlstr=" insert into Collection( name ) values ( '����' ) "; 
	result = sqlite3_exec( pDB, sql_str , NULL, NULL, &errMsg ); 
	if(result != SQLITE_OK ) 
	      CCLog( "�����¼ʧ�ܣ�������:%d ������ԭ��:%s\n" , result, errMsg ); 
	  
	int count=0;
	sprintf(sql_str,"select * from %s",COLLECTION);
	CCLog("%s",sql_str);
	sqlite3_exec( pDB, sql_str , loadRecordCount, &count, &errMsg );
	CCLog("count:%d",count);
	CCLog("%d",AMBERS.size());

	 //�ر����ݿ� 
	sqlite3_close(pDB); 
	//return ambers;
	return AMBERS;
}
*/
bool Amber::getEnableForKey(int id){
	char key[20]={0};
	sprintf(key,"%d_enable",id);
	return CCUserDefault::sharedUserDefault()->getBoolForKey(key,false);
}
bool Amber::getUsedForKey(int id){
	char key[20]={0};
	sprintf(key,"%d_used",id);
	return CCUserDefault::sharedUserDefault()->getBoolForKey(key,false);
}
void Amber::writeEnableForKey(int id,bool p){
	char key[20]={0};
	sprintf(key,"%d_enable",id);
	CCUserDefault::sharedUserDefault()->setBoolForKey(key,p);
}
void Amber::writeUsedForKey(int id,bool p){
	char key[20]={0};
	sprintf(key,"%d_used",id);
	CCUserDefault::sharedUserDefault()->setBoolForKey(key,p);
}

vector<Amber*> Amber::getAmbers(int count){
	if(AMBERS.size()>0) return AMBERS;

	for(int i=0;i<count;i++){
		Amber* amber=Amber::create();
		amber->setEnable(getEnableForKey(i));
		amber->setUsed(getUsedForKey(i));
		AMBERS.push_back(amber);
	}
	return AMBERS;
}
void Amber::writeAmbers(){
	for(int i=0;i<AMBERS.size();i++){
		Amber::writeEnableForKey(i,AMBERS[i]->getEnable());
		Amber::writeUsedForKey(i,AMBERS[i]->getUsed());
	}
	CCUserDefault::sharedUserDefault()->flush();
}