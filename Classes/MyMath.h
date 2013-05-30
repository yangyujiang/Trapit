#ifndef __MYMATH_H__
#define __MYMATH_H__

#include "cocos2d.h"

USING_NS_CC;

class MyMath{
public:
	/*
	//获取直线p1,p2与y轴正方向的夹角
	*/
    static float getDirection(CCPoint p1,CCPoint p2);
	/*获取当前系统时间，单位毫秒*/
	static long  millisecondNow();
	/*获取当前系统时间，单位秒*/
	static long secondNow();
	
	/*中文转化*/
	//static void  WStrToUTF8(std::string& dest, const std::wstring& src);
	//static std::string WStrToUTF8(const std::wstring& str);
};

#endif // __MYMATH_H__
