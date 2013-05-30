#include "MyMath.h"

USING_NS_CC;

	/*
	//获取直线p1,p2与y轴正方向的夹角
	*/
float MyMath::getDirection(CCPoint p1,CCPoint p2){
	float dis=ccpDistance(p1,p2);
	if(dis<=0) dis=1;
	float angle=asin((p2.x-p1.x)/(dis))*180.f/M_PI;
	//if(p1.x<=p2.x&&p1.y>=p2.y) angle=180-angle;//左下右上 angle
	if(p1.x>=p2.x&&p1.y>=p2.y) angle=-180-angle;//右上左下 -180-angle
	else if(p1.x<=p2.x&&p1.y>=p2.y) angle=180-angle;   //左上右下 180-angle
	//右下左上 angle

	return angle-90;
}


long MyMath::millisecondNow()  //毫秒
{ 
struct cc_timeval now; 
CCTime::gettimeofdayCocos2d(&now, NULL); 
return (now.tv_sec * 1000 + now.tv_usec / 1000); 
}
long MyMath::secondNow()  //秒
{ 
struct cc_timeval now; 
CCTime::gettimeofdayCocos2d(&now, NULL); 
return (now.tv_sec); 
}



/*
void  MyMath::WStrToUTF8(std::string& dest, const std::wstring& src){
	dest.clear();
	for (size_t i = 0; i < src.size(); i++){
		wchar_t w = src[i];
		if (w <= 0x7f)
			dest.push_back((char)w);
		else if (w <= 0x7ff){
			dest.push_back(0xc0 | ((w >> 6)& 0x1f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0xffff){
			dest.push_back(0xe0 | ((w >> 12)& 0x0f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (sizeof(wchar_t) > 2 && w <= 0x10ffff){
			dest.push_back(0xf0 | ((w >> 18)& 0x07)); // wchar_t 4-bytes situation
			dest.push_back(0x80| ((w >> 12) & 0x3f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else
			dest.push_back('?');
	}
}

//! simple warpper
std::string MyMath::WStrToUTF8(const std::wstring& str){
	std::string result;
	WStrToUTF8(result, str);
	return result;
}
*/

