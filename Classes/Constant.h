#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#include "cocos2d.h"

#define PTM_RATIO 32.0f //Box2D��������Ļ����Ļ��������1��=32���ص�
#define MAP_SCALE 1.8f //��ͼ����Ļ�ߴ�ı���
#define WORLD_SCALE 2.0f //��������Ļ�ߴ�ı���

//16���� ��ײ����
#define k_blocksGroup  2
#define k_resinBallBodyGroup -2
#define k_insectGroup 4  //
#define k_visualField -4  //������Ұ
#define k_wallGroup 0  //ǽ���ϰ���
#define k_enemyGroup 8  //���˳�����Ұ

#define k_defaultCategory 0x0001 
#define k_blocksCategory 0x0002 
#define k_resinBallRealCategory 0x0004  //ʵ����
#define k_resinBallSensorCategory 0x0010  //��Ӧ��
#define k_insectCategory 0x0008  //
#define k_wallCategory 0x0020  //ǽ���ϰ���
#define k_visualFieldCategory 0x0040 //������Ұ
#define k_enemyViewCategory 0x0080  //���˳�����Ұ

#define k_blocksMask 0x0027    //resinReal 4,blocks 2,wall 20,default 1
#define k_resinBallRealMask 0x00a3 // block 2, wall 20 ,enemyView 80,default 1
#define k_resinBallSensorMask 0x0008 //insect 8,
#define k_insectMask 0x0079  //InsectView 40,resinBallSensor 10, insect 8 wall 20 default 1
#define k_wallMask 0x000e   //ǽ���ϰ��� block 2,resinReal 4, insect 8 
#define k_visualFieldMask 0x0008  //������Ұ insect 8
#define k_enemyViewMask 0x0004  //���˳�����Ұ resinReal 4

const int TAG_RESINBALL=1;//��֬��
const int TAG_INSECT=2;//����
const int TAG_WALL=4;//ǽ
const int TAG_ENEMY=5;//����
const int TAG_COLLECTIONLIST=6;//�ɾ��б�
const int TAG_BLOCK=7;//С����ͼƬ

//tag���
const int TAG_INSECT_ACTION=9; //���ӵ��˶�
const int TAG_STEPBACK=10; //����action
const int TAG_RANDOM=11; //����˶�action
const int TAG_ROTATERANDOM=12;//��ת����������˶�
const int TAG_BATCH=12333;//CCSpriteNodeBatch

const unsigned int ORIGIN_DOLLAR=100;//��ʼ���Ϊ100
const unsigned int ORIGIN_RESIN=100;//��ʼ��֬��Ϊ100

#define DOLLAR "DOLLAR"
#define RESIN_VOLUME "RESIN_VOLUME" //��ǰʣ����֬����
#define GET_SCORE "GET_SCORE" //��ǰ��Ϸ�÷�
#define GET_AMBER "GET_AMBER" //���������
#define GET_MONEY "GET_MONEY" //��ý�Ǯ��
#define CONSUME_RESIN "CONSUME_RESIN" //ÿ��������֬��
//�˾����ĵ���֬
//const char *DOLLAR="DOLLAR";//key �������
//const char *RESIN_VOLUME="RESIN_VOLUME";//key ��֬����

/*
//��Ϸ������Ԫ��id
*/
#define ID_LEFT_BODY 301
#define ID_RIGHT_BODY 302
#define ID_HEAD_BODY 303
#define ID_TAIL_BODY 304
#define ID_BODY 305 
//��֬��fixtureUserDataID
#define ID_REAL_BALL 310
#define ID_SENSOR_BALL 311 //��֬��ĸ�Ӧ����
#define ID_BLOCK 320 //С����fixture

#define ID_MANTIS_VIEW 111
#define ID_MANTIS 112   //�������
#define ID_ENEMY 113
#define ID_ENEMY_VIEW 114
#define ID_ANT 115  //��������
#define ID_A 116

#define ID_ENEMY_ANIMAL 201 //���˶���
#define ID_FRIEND_ANIMAL 202 //���˶������Ҫ��׽�Ķ���

/*
//��Ϣ�����е���Ϣ����
*/
#define TYPE_VIEW_IN 0   //��Ұ��
#define TYPE_VIEW_OUT 1 //��Ұ��
#define TYPE_BOUNDING 2  //�߽���ײ
#define TYPE_CONTACT 3 //�Ӵ�
#define TYPE_CONTACT_END 4 //�Ӵ�����
#define TYPE_ATTACKED  5//������
#define TYPE_GROPE 6  //��������

/*��Ұ��Ϣ����*/
#define VIEW_ATTACK 10 //���ſ�����
#define VIEW_CHANGE_VEL 11 //�ٶȱ仯 
#define VIEW_KISS 12  //�������Ƕ���


//ԭ�Ӷ���ID
const int ID_NORMAL_MOVE=0;
const int ID_LOOKAT_MOVE =1;
const int ID_ROTATE_MOVE =2;
const int ID_FOLLOW_REACHED_MOVE =3;  //���� ���� ֹͣ
const int ID_FOLLOW_BYTIME_MOVE=4;  //���� ʱ�䵽 ֹͣ
const int ID_ATTACK_MOVE=5;
const int ID_ROTATETO_MOVE =6;//��ת��ĳ�Ƕ�
const int ID_STAY_MOVE=7;//ͣ��������
const int ID_GROPE_MOVE=8;//����
const int ID_KISS_MOVE=9;//����
const int ID_HEAD_ROTATETO_MOVE=10;//��ͷ����ת


#define ID_DEFAULT -1

//��Ϊ��ID
#define ID_ACTION_NORMAL 10  //�����˶�
#define ID_ACTION_ESCAPE 11  //���ܣ�����
#define ID_ACTION_ATTACK 12  //����
#define ID_ACTION_COMMUNICATE 13  //����
#define ID_ACTION_COMMUNICATED 14 //������
#define ID_ACTION_FOLLOW 15  //����
#define ID_ACTION_GROPE 16 //����
#define ID_ACTION_STAY 18  //ԭ��ͣ����ֻ�е�����

//�˶��ȼ� 
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

//��������
const static char* FNTFILE="fonts/markerFelt.fnt";
const static char* FONTNAME="Helvetica-Bold";
//��������Ч
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
class InnerStage{//�ڱ߽�ṹ��
public:
		float top;
		float bottom;
		float left;
		float right;//����Ļ�ұ߽�ľ��루���أ�
};
class CollectionDef{
private:
	unsigned int id;
	bool enable;//�Ƿ���ã����Ƿ��ô˳ɾͣ�
	bool used;//�Ƿ���ʹ�ã�
	char* image;//ͼƬ����
	char* descrip;//����
	char* imageSkill;//����ͼ��
	char* descripSkill;//���ܽ���
	char* nameSkill;//��������
	char* descripHowToGet;//��εõ�
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