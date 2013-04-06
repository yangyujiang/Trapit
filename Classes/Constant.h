#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#include "cocos2d.h"

#define PTM_RATIO 32.0f //Box2D世界与屏幕坐标的换算比例，1米=32像素点
#define MAP_SCALE 1.8f //地图与屏幕尺寸的比例
#define WORLD_SCALE 2.0f //世界与屏幕尺寸的比例

#define k_blocksGroup  2
#define k_resinBallBodyGroup -2
#define k_insectGroup 4
#define k_visualField -4
#define k_wallGroup 0

#define k_defaultCategory 0x0001
#define k_blocksCategory 0x0002
#define k_resinBallRealCategory 0x0004
#define k_resinBallSensorCategory 0x0010
#define k_insectCategory 0x0008
#define k_wallCategory 0x0020

#define k_blocksMask 0x0005
#define k_resinBallRealMask 0x0003
#define k_resinBallSensorMask 0x0008
#define k_insectMask 0x0039
#define k_wallMask 0x0008


const int TAG_RESINBALL=1;//树脂球
const int TAG_INSECT=2;//虫子
const int TAG_WALL=4;//墙

//tag标记
const int TAG_INSECT_ACTION=9; //虫子的运动
const int TAG_STEPBACK=10; //后退action
const int TAG_RANDOM=11; //随机运动action
const int TAG_ROTATERANDOM=12;//旋转并重新随机运动




const float FIXED_TIMESTEP = 1.0f / 60.0f;

// Minimum remaining time to avoid box2d unstability caused by very small delta times
// if remaining time to simulate is smaller than this, the rest of time will be added to the last step,
// instead of performing one more single step with only the small delta time.
const float MINIMUM_TIMESTEP = 1.0f / 300.0f;  

const int VELOCITY_ITERATIONS = 8;
const int POSITION_ITERATIONS = 8;

// maximum number of steps per tick to avoid spiral of death
const int MAXIMUM_NUMBER_OF_STEPS = 25;
class Constant{
/*public:
//! convert from wstring to UTF8 using self-coding-converting
static void  WStrToUTF8(std::string& dest, const wstring& src){
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
}*/
};
class InnerStage{//内边界结构体
public:
		float top;
		float bottom;
		float left;
		float right;//与屏幕右边界的距离（像素）
};



#endif //__CONSTANT_H__