#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#include "cocos2d.h"

#define PTM_RATIO 32.0 //Box2D��������Ļ����Ļ��������1��=32���ص�

#define k_blocksGroup  2
#define k_resinBallBodyGroup -2
#define k_insectGroup 4
#define k_visualField -4

#define k_defaultCategory 0x0001
#define k_blocksCategory 0x0002
#define k_resinBallRealCategory 0x0004
#define k_resinBallSensorCategory 0x0010
#define k_insectCategory 0x0008

#define k_blocksMask 0x0005
#define k_resinBallRealMask 0x0003
#define k_resinBallSensorMask 0x0008
#define k_insectMask 0x0019

const int TAG_RESINBALL=1;//��֬��
const int TAG_INSECT=2;//����
const int TAG_WALL=4;//ǽ

//tag���
const int TAG_INSECT_ACTION=9; //���ӵ��˶�
const int TAG_STEPBACK=10; //����action
const int TAG_RANDOM=11; //����˶�action
const int TAG_ROTATERANDOM=12;//��ת����������˶�




const float32 FIXED_TIMESTEP = 1.0f / 60.0f;

// Minimum remaining time to avoid box2d unstability caused by very small delta times
// if remaining time to simulate is smaller than this, the rest of time will be added to the last step,
// instead of performing one more single step with only the small delta time.
const float32 MINIMUM_TIMESTEP = 1.0f / 300.0f;  

const int32 VELOCITY_ITERATIONS = 8;
const int32 POSITION_ITERATIONS = 8;

// maximum number of steps per tick to avoid spiral of death
const int32 MAXIMUM_NUMBER_OF_STEPS = 25;


#endif //__CONSTANT_H__