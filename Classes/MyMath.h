#ifndef __MYMATH_H__
#define __MYMATH_H__

#include "cocos2d.h"

USING_NS_CC;

class MyMath{
public:
	/*
	//��ȡֱ��p1,p2��y��������ļн�
	*/
    static float getDirection(CCPoint p1,CCPoint p2);
	/*��ȡ��ǰϵͳʱ�䣬��λ����*/
	static long  millisecondNow();
	/*��ȡ��ǰϵͳʱ�䣬��λ��*/
	static long secondNow();
	
	/*����ת��*/
	//static void  WStrToUTF8(std::string& dest, const std::wstring& src);
	//static std::string WStrToUTF8(const std::wstring& str);
};

#endif // __MYMATH_H__
