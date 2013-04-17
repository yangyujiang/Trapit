#ifndef __XML_PARSE_H__
#define __XML_PARSE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class XmlParse :public CCObject, public CCSAXDelegator
{
    
public:
    static XmlParse* parserWithFile(const char* tmxFile);
    bool initXmlParse(const char *xmlName);
    string root_name;
    bool isJumpHeadData;
 /*   ~XmlParse();
    vector< string> allData;//用于记录所有数据
    */
    //  使用 CCSAXDelegator 重写3个回调函数
    //开始解析时回调
    void startElement(void *ctx, const char *name, const char **atts);
    //结束解析回调
    void endElement(void *ctx, const char *name);
    //键值对的结束字段
    void textHandler(void *ctx, const char *ch, int len);
    
private:
    string startXmlElement;//用来记录每个key前字段
	string endXmlElement;//用来记录每个key后字段
    string currString;//记录每个value的值
};

#endif //__XML_PARSE_H__
