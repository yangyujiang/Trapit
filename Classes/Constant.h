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
#define k_visualFieldCategory 0x0040

#define k_blocksMask 0x0005
#define k_resinBallRealMask 0x0003
#define k_resinBallSensorMask 0x0038
#define k_insectMask 0x0079  //view,resinBall,default,enemy
#define k_wallMask 0x0008
#define k_enemyMask 0x0010
#define k_visualFieldMask 0x0008

const int TAG_RESINBALL=1;//树脂球
const int TAG_INSECT=2;//虫子
const int TAG_WALL=4;//墙
const int TAG_ENEMY=5;//敌人
const int TAG_COLLECTIONLIST=6;//成就列表
const int TAG_BLOCK=7;//小方块图片

//tag标记
const int TAG_INSECT_ACTION=9; //虫子的运动
const int TAG_STEPBACK=10; //后退action
const int TAG_RANDOM=11; //随机运动action
const int TAG_ROTATERANDOM=12;//旋转并重新随机运动

const unsigned int ORIGIN_DOLLAR=100;//初始金币为100
const unsigned int ORIGIN_RESIN=100;//初始树脂设为100

#define DOLLAR "DOLLAR"
#define RESIN_VOLUME "RESIN_VOLUME" //当前剩余树脂数量
#define GET_SCORE "GET_SCORE" //当前游戏得分
#define GET_AMBER "GET_AMBER" //获得琥珀数
#define GET_MONEY "GET_MONEY" //获得金钱数
#define CONSUME_RESIN "CONSUME_RESIN"
//此局消耗的树脂
//const char *DOLLAR="DOLLAR";//key 金币数量
//const char *RESIN_VOLUME="RESIN_VOLUME";//key 树脂容量



/*
//游戏中所有元素id
*/
#define ID_MANTIS_VIEW 111
#define ID_MANTIS 112


/*
//消息队列中的消息类型
*/
#define TYPE_VIEW 0   //视野
#define TYPE_BOUNDING 1  //边界碰撞
#define TYPE_ 2



const unsigned int ID_NORMAL_MOVE=0;
const unsigned int ID_STEPBACK_MOVE=1;
const unsigned int ID_ROTATE_MOVE=2;
const unsigned int ID_FOLLOW_MOVE=3;//跟踪

#define ID_DEFAULT -1

//运动等级 
#define LV0 0
#define LV1 1
#define LV2 2
#define LV3 3
#define LV4 4
#define LV5 5
#define LV6 6

#define NORMAL_LV (LV0)
#define FOLLOW_LV (LV2)
#define ROTATE_LV (LV1)
#define STEPBACK_LV (LV1)

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