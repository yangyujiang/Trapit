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
#define k_enemyGroup 8

#define k_defaultCategory 0x0001
#define k_blocksCategory 0x0002
#define k_resinBallRealCategory 0x0004
#define k_resinBallSensorCategory 0x0010
#define k_insectCategory 0x0008
#define k_wallCategory 0x0020
#define k_enemyCategory 0x0030

#define k_blocksMask 0x0005
#define k_resinBallRealMask 0x0003
#define k_resinBallSensorMask 0x0038
#define k_insectMask 0x0039
#define k_wallMask 0x0008
#define k_enemyMask 0x0010

const int TAG_RESINBALL=1;//树脂球
const int TAG_INSECT=2;//虫子
const int TAG_WALL=4;//墙
const int TAG_ENEMY=5;//敌人
const int TAG_COLLECTIONLIST=6;//成就列表

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
class Constant{};
class InnerStage{//内边界结构体
public:
		float top;
		float bottom;
		float left;
		float right;//与屏幕右边界的距离（像素）
};
class CollectionDef{
private:
	unsigned int id;
	bool enable;//是否可用（即是否获得此成就）
	bool used;//是否已使用？
	char* image;//图片名称
	char* descrip;//描述
	char* imageSkill;//技能图标
	char* descripSkill;//技能介绍
	char* nameSkill;//技能名称
	char* descripHowToGet;//如何得到
public:
	CollectionDef();
	CollectionDef(unsigned int id,bool enable,bool used,char* image,char* descrip){
		this->id=id;
		this->enable=enable;
		this->used=used;
		this->image=image;
	};
};
/*
class StrToUTF8{
public:
	static void  WStrToUTF8(std::string& dest, const std::wstring& src);
	static std::string WStrToUTF8(const std::wstring& str);

};
*/


#endif //__CONSTANT_H__