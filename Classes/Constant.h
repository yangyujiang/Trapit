#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#include "cocos2d.h"

#define PTM_RATIO 32.0f //Box2D世界与屏幕坐标的换算比例，1米=32像素点
#define MAP_SCALE 1.8f //地图与屏幕尺寸的比例
#define WORLD_SCALE 2.0f //世界与屏幕尺寸的比例

//16进制 碰撞过滤
#define k_blocksGroup  2
#define k_resinBallBodyGroup -2
#define k_insectGroup 4  //
#define k_visualField -4  //虫子视野
#define k_wallGroup 0  //墙、障碍物
#define k_enemyGroup 8  //敌人虫子视野

#define k_defaultCategory 0x0001 
#define k_blocksCategory 0x0002 
#define k_resinBallRealCategory 0x0004  //实体球
#define k_resinBallSensorCategory 0x0010  //感应球
#define k_insectCategory 0x0008  //
#define k_wallCategory 0x0020  //墙、障碍物
#define k_visualFieldCategory 0x0040 //虫子视野
#define k_enemyViewCategory 0x0080  //敌人虫子视野

#define k_blocksMask 0x0027    //resinReal 4,blocks 2,wall 20,default 1
#define k_resinBallRealMask 0x00a3 // block 2, wall 20 ,enemyView 80,default 1
#define k_resinBallSensorMask 0x0008 //insect 8,
#define k_insectMask 0x0079  //InsectView 40,resinBallSensor 10, insect 8 wall 20 default 1
#define k_wallMask 0x000e   //墙、障碍物 block 2,resinReal 4, insect 8 
#define k_visualFieldMask 0x0008  //虫子视野 insect 8
#define k_enemyViewMask 0x0004  //敌人虫子视野 resinReal 4

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
const int TAG_BATCH=12333;//CCSpriteNodeBatch

const unsigned int ORIGIN_DOLLAR=100;//初始金币为100
const unsigned int ORIGIN_RESIN=100;//初始树脂设为100

#define DOLLAR "DOLLAR"
#define RESIN_VOLUME "RESIN_VOLUME" //当前剩余树脂数量
#define GET_SCORE "GET_SCORE" //当前游戏得分
#define GET_AMBER "GET_AMBER" //获得琥珀数
#define GET_MONEY "GET_MONEY" //获得金钱数
#define CONSUME_RESIN "CONSUME_RESIN" //每局消耗树脂数
//此局消耗的树脂
//const char *DOLLAR="DOLLAR";//key 金币数量
//const char *RESIN_VOLUME="RESIN_VOLUME";//key 树脂容量

/*
//游戏中所有元素id
*/
#define ID_LEFT_BODY 301
#define ID_RIGHT_BODY 302
#define ID_HEAD_BODY 303
#define ID_TAIL_BODY 304
#define ID_BODY 305 
//树脂球fixtureUserDataID
#define ID_REAL_BALL 310
#define ID_SENSOR_BALL 311 //树脂球的感应区域
#define ID_BLOCK 320 //小方块fixture

#define ID_MANTIS_VIEW 111
#define ID_MANTIS 112   //螳螂类型
#define ID_ENEMY 113
#define ID_ENEMY_VIEW 114
#define ID_ANT 115  //蚂蚁类型
#define ID_A 116

#define ID_ENEMY_ANIMAL 201 //敌人动物
#define ID_FRIEND_ANIMAL 202 //友人动物，即需要捕捉的动物

/*
//消息队列中的消息类型
*/
#define TYPE_VIEW_IN 0   //视野进
#define TYPE_VIEW_OUT 1 //视野出
#define TYPE_BOUNDING 2  //边界碰撞
#define TYPE_CONTACT 3 //接触
#define TYPE_CONTACT_END 4 //接触结束
#define TYPE_ATTACKED  5//被攻击
#define TYPE_GROPE 6  //摸索结束

/*视野消息队列*/
#define VIEW_ATTACK 10 //播放砍动画
#define VIEW_CHANGE_VEL 11 //速度变化 
#define VIEW_KISS 12  //播放亲吻动画


//原子动作ID
const int ID_NORMAL_MOVE=0;
const int ID_LOOKAT_MOVE =1;
const int ID_ROTATE_MOVE =2;
const int ID_FOLLOW_REACHED_MOVE =3;  //跟踪 到达 停止
const int ID_FOLLOW_BYTIME_MOVE=4;  //跟踪 时间到 停止
const int ID_ATTACK_MOVE=5;
const int ID_ROTATETO_MOVE =6;//旋转到某角度
const int ID_STAY_MOVE=7;//停留、摸索
const int ID_GROPE_MOVE=8;//摸索
const int ID_KISS_MOVE=9;//亲吻
const int ID_HEAD_ROTATETO_MOVE=10;//绕头部旋转


#define ID_DEFAULT -1

//行为层ID
#define ID_ACTION_NORMAL 10  //正常运动
#define ID_ACTION_ESCAPE 11  //逃跑，惊恐
#define ID_ACTION_ATTACK 12  //攻击
#define ID_ACTION_COMMUNICATE 13  //交流
#define ID_ACTION_COMMUNICATED 14 //被交流
#define ID_ACTION_FOLLOW 15  //跟踪
#define ID_ACTION_GROPE 16 //摸索
#define ID_ACTION_STAY 18  //原地停留，只有敌人有

//运动等级 
#define LV0 0
#define LV1 1
#define LV2 2
#define LV3 3
#define LV4 4
#define LV5 5
#define LV6 6

#define NORMAL_LV (LV0)
#define GROPE_LV (LV1)
#define FOLLOW_LV (LV2)
#define ESCAPE_LV (LV3)
#define ATTACK_LV (LV3)
#define ROTATE_LV (LV1)
#define STAY_LV (LV1)
#define COMMUNICATE_LV (LV2)

//字体名字
const static char* FNTFILE="fonts/markerFelt.fnt";
const static char* FONTNAME="Helvetica-Bold";
//音乐与音效
const static char* MENU_MUSIC="music/menu.ogg";
const static char* GAME_MUSIC="music/game.ogg";

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