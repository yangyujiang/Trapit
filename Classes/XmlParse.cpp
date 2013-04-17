#include "XmlParse.h"

USING_NS_CC;


XmlParse* XmlParse::parserWithFile(const char* tmxFile){
	XmlParse* pRet=new XmlParse();
	if(pRet->initXmlParse(tmxFile)){
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool XmlParse::initXmlParse(const char* xmlName){
	CCSAXParser _par;
	if(_par.init("UTF-8")==false){
		CCLog("please use utf-8");
		return false;
	}

	_par.setDelegator(this);
	//获取文件的完整相对路径
	const char* _path=CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(xmlName);
	return _par.parse(_path);
}


/*
bool XmlParse::initXmlParse(const char* xmlName)

{    
    CCSAXParser _par;
    if (false == _par.init("UTF-8") )
    {
        CCLog("请使用utf-8格式!");
        return false;
    }
    _par.setDelegator(this);
    //获取文件的完整相对路径
    const char* _path =CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(xmlName);
    return _par.parse(_path);
    
}


//开始解析时回调
void XmlParse::startElement(void *ctx, const char *name, const char **atts)

{
    CC_UNUSED_PARAM(ctx);
    
    startXmlElement = (char*)name;
    if(!isJumpHeadData){//跳过数据头
        CCLog("------跳过root name");
        isJumpHeadData=true;
        root_name=startXmlElement;
        return;
    }
}
//结束解析回调
void XmlParse::endElement(void *ctx, const char *name)

{
    CC_UNUSED_PARAM(ctx);
    
    endXmlElement = (char*)name;
    if(endXmlElement==root_name){//数据尾
        CCLog("读取xml结束");
        isJumpHeadData=false;
        root_name="";
        return;
    }
}
//键值对的结束字段
void XmlParse::textHandler(void *ctx, const char *ch, int len)
{
    
    CC_UNUSED_PARAM(ctx);
    currString=string((char*)ch,0,len);
    if(root_name!=""){
         allData.push_back(currString);
    }
    
}

//析构函数释放内存
XmlParse::~XmlParse()
{
    for (int i=0; i<allData.size(); i++) {
        allData[i].clear();
    }
    allData.clear();
}


*/
void XmlParse::startElement(void *ctx, const char *name, const char **atts){

}

void XmlParse::endElement(void *ctx, const char *name){

}

void XmlParse::textHandler(void *ctx, const char *ch, int len){

}