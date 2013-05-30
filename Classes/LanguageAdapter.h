#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__

static void  WStrToUTF8(std::string& dest, const std::wstring& src){
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
static std::string WStrToUTF8(const std::wstring& str){
	std::string result;
	WStrToUTF8(result, str);
	return result;
}




static const char* variable_vel="速度:";
static const char* variable_nian="黏度:";
static const char* variable_contro="控制:";

static const char* skillDescrip[]={
	"技能介绍：\n  使用后,所有昆虫 \n 会进入攻击状态. \n 当心你的手指",
	"技能介绍:",
	"技能介绍:"
};
static const char* __descrip[]={
	"黄色螳螂琥珀",
	"红色蚂蚁琥珀",
	"蓝色蚂蚁琥珀",
	"暂未开放"
};

static const char* __imageName[]={"amber_yellow.png","amber_red.png","amber_blue.png","amber_unknown.png"};
static const char* __skillName[]={
	"加速",
	"冰冻",
	"食物",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放",
	"暂未开放"
};
 static const char* __skillDescrip[]={
	 "对树脂球瞬间加速",
	 "把所有虫子冻住5秒",
	 "食物可以吸引附\n近虫子聚集",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放",
	 "暂未开放"
 };
 static const char* __howToGet[]={
	  "攻击时的螳螂有一种特殊的魅力",
	  "逃跑时的蚂蚁潜力无限",
	  "恋爱中的蚂蚁总是充满干劲",
	  "暂未开放"
 };


#endif //__LANGUAGE_H__