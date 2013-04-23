#include "Amber.h"
#include "B2EasyBox2D.h"
#include "RandomBy.h"
#include "sqlite3.h"
#include "Constant.h"

USING_NS_CC;

const char* COLLECTION="collection";
//在COLLECTION表中的列数
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

Amber::Amber()
{
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
			//amber=Amber::create();
			//amber->id=1;//=column_value[_id];
			amber->id=CCString::createWithFormat(column_value[_id])->intValue();
			//CCLog("%d",amber->id);
			amber->enable=CCString::createWithFormat(column_value[__enable])->boolValue();
			amber->used=CCString::createWithFormat(column_value[_used])->boolValue();
			amber->image=column_value[_imageName];
			amber->descrip=column_value[_descrip];
			amber->imageSkill=column_value[_imageSkillName];
			amber->descripSkill=column_value[_descripSkill];
			amber->nameSkill=column_value[_nameSkill];
			amber->descripHowToGet=column_value[_descripHowToGet];
			CCLog("id=%d",amber->id); 
	CCLog("enable=%d",amber->enable); 
	CCLog("used=%d",amber->used); 
	CCLog("imageName=%s",amber->image); 
	CCLog("descrip=%s",amber->descrip); 
	CCLog("imageSkill=%s",amber->imageSkill); 
	CCLog("descripSkill=%s",amber->descripSkill); 
	CCLog("nameSkill=%s",amber->nameSkill); 
	CCLog("descripHowToGet=%s",amber->descripHowToGet);
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
			amber->id=CCString::createWithFormat(column_value[_id])->intValue();
			//CCLog("%d",amber->id);
			amber->enable=CCString::createWithFormat(column_value[__enable])->intValue()?1:0;
			amber->used=CCString::createWithFormat(column_value[_used])->intValue()?1:0;
			amber->image=column_value[_imageName];
			amber->descrip=column_value[_descrip];
			amber->imageSkill=column_value[_imageSkillName];
			amber->descripSkill=column_value[_descripSkill];
			amber->nameSkill=column_value[_nameSkill];
			amber->descripHowToGet=column_value[_descripHowToGet];
			AMBERS.push_back(amber);
            return 0; 
} 

vector<Amber*> Amber::getAmbersFromSQLite(){
	//vector<Amber*> ambers;
	
	sqlite3 *pDB = NULL;//数据库指针 
	char * errMsg = NULL;//错误信息 
	std::string sqlstr;//SQL指令 
	char sql_str[1000]={0};
	int result;//sqlite3_exec返回值 
 
	//打开一个数据库，如果该数据库不存在，则创建一个数据库文件 
	std::string sqlpath= CCFileUtils::sharedFileUtils()->getWriteablePath()+"data.db";
	CCLog("%s",sqlpath.c_str());
	result = sqlite3_open(sqlpath.c_str(), &pDB); 
	if( result != SQLITE_OK ) 
	      CCLog( "打开数据库失败，错误码:%d ，错误原因:%s\n" , result, errMsg ); 
	else CCLog("成功");
  
	sprintf(sql_str, "create table %s( ID integer primary key autoincrement,enable integer,used integer, image nvarchar(32),descrip nvarchar(32),imageSkill nvarchar(32),nameSkill nvarchar(32),descripSkill nvarchar(64),descripHowToGet nvarchar(32) )",COLLECTION);
	 //创建表，设置ID为主键，且自动增加 
	result=sqlite3_exec( pDB,sql_str, NULL, NULL, &errMsg );
	if( result != SQLITE_OK ) 
	      CCLog( "创建表失败，错误码:%d ，错误原因:%s\n" , result, errMsg ); 
	sprintf(sql_str,"insert into %s(enable,used,image,descrip,imageSkill,nameSkill,descripSkill,descripHowToGet) values(1,0,'amber_1.png','愤怒的蚂蚁蓝珀','amber_1.png','昆虫之怒','使用后，所有昆虫进入攻击状态。当心你的手指','受螳螂攻击时容易出现')",COLLECTION);
	//插入数据 
	//sqlstr=" insert into Collection( name ) values ( '克塞' ) "; 
	result = sqlite3_exec( pDB, sql_str , NULL, NULL, &errMsg ); 
	if(result != SQLITE_OK ) 
	      CCLog( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, errMsg ); 
	  
	/*//插入数据 
	sqlstr=" insert into Collection( name ) values ( '葫芦娃' ) "; 
	result = sqlite3_exec( pDB, sqlstr.c_str() , NULL, NULL, &errMsg ); 
	if(result != SQLITE_OK ) 
	      CCLog( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, errMsg ); 
 
	//插入数据 
	sqlstr=" insert into Collection( name ) values ( '擎天柱' ) "; 
	result = sqlite3_exec( pDB, sqlstr.c_str() , NULL, NULL, &errMsg ); 
	if(result != SQLITE_OK ) 
	      CCLog( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, errMsg ); 
	 */
	int count=0;
	sprintf(sql_str,"select * from %s",COLLECTION);
	CCLog("%s",sql_str);
	sqlite3_exec( pDB, sql_str , loadRecordCount, &count, &errMsg );
	CCLog("count:%d",count);
	CCLog("%d",AMBERS.size());

	 //关闭数据库 
	sqlite3_close(pDB); 
	//return ambers;
	return AMBERS;
}

//一些封装
/*
char* Amber::getDescrip(){
	return descrip;
}
char* Amber::getDescripHowToGet(){
	return descripHowToGet;
}
char* Amber::getDescripSkill(){
	return descripSkill;
}
char* Amber::getIamgeName(){
	return image;
}
char* Amber::getImageSkillName(){
	return imageSkill;
}
unsigned int Amber::getId(){
	return id;
}
char* Amber::getSkillName(){
	return nameSkill;
}
void Amber::setUsed(bool used){
	this->used=used;
}
bool Amber::getUsed(){
	return used;
}
void Amber::setEnable(bool enable){
	this->enable=enable;
}
bool Amber::getEnable(){
	return enable;
}*/