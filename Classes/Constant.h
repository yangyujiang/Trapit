#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#include "cocos2d.h"

#define PTM_RATIO 32.0f //Box2D��������Ļ����Ļ��������1��=32���ص�
#define MAP_SCALE 1.8f //��ͼ����Ļ�ߴ�ı���
#define WORLD_SCALE 2.0f //��������Ļ�ߴ�ı���

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

const unsigned int ORIGIN_DOLLAR=100;//��ʼ���Ϊ100
const unsigned int ORIGIN_RESIN=100;//��ʼ��֬��Ϊ100

#define DOLLAR "DOLLAR"
#define RESIN_VOLUME "RESIN_VOLUME" //��ǰʣ����֬����
#define GET_SCORE "GET_SCORE" //��ǰ��Ϸ�÷�
#define GET_AMBER "GET_AMBER" //���������
#define GET_MONEY "GET_MONEY" //��ý�Ǯ��
#define CONSUME_RESIN "CONSUME_RESIN"
//�˾����ĵ���֬
//const char *DOLLAR="DOLLAR";//key �������
//const char *RESIN_VOLUME="RESIN_VOLUME";//key ��֬����



/*
//��Ϸ������Ԫ��id
*/
#define ID_MANTIS_VIEW 111
#define ID_MANTIS 112


/*
//��Ϣ�����е���Ϣ����
*/
#define TYPE_VIEW 0   //��Ұ
#define TYPE_BOUNDING 1  //�߽���ײ
#define TYPE_ 2



const unsigned int ID_NORMAL_MOVE=0;
const unsigned int ID_STEPBACK_MOVE=1;
const unsigned int ID_ROTATE_MOVE=2;
const unsigned int ID_FOLLOW_MOVE=3;//����

#define ID_DEFAULT -1

//�˶��ȼ� 
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