#ifndef __B2EASY_BOX2D_H__
#define __B2EASY_BOX2D_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"


class B2EasyBox2D
{
public:
	B2EasyBox2D(void);
	virtual ~B2EasyBox2D(void);

	/**
	* ����������һ������Ϊ10��Box2D����
	* @return b2World
	*/
	static b2World* createWorld();
	static b2World* createWorld(b2Vec2 gravity);

	static cocos2d::CCSprite* createDebugDraw(b2World* world);
	/**
		 * ����������һ�����ε�b2Body�������
		 * @param	world
		 * @param	posX box��x���꣬������Ϊ��λ
		 * @param	posY box��y���꣬������Ϊ��λ
		 * @param	boxWidth box�Ŀ�ȣ�������Ϊ��λ
		 * @param	boxHeight box�ĸ߶ȣ�������Ϊ��λ
		 * @param	isStatic box�Ƿ�ֹ����
		 * @param	userData box�����
		 * @return b2Body
		 */
//	static b2Body* createBox(b2World* world,float posX,float posY,float boxWidth,float boxHeight,bool isStatic);
//	static b2Body* createBox(b2World* world,float posX,float posY,float boxWidth,float boxHeight,bool isStatic,float angle);
	static b2Body* createBox(b2World* world,float posX,float posY,float boxWidth,float boxHeight,bool isStatic,
		float angle=0,float density=1,float friction=0.3f,float restitution=0.0f,b2Filter *filter=NULL);
	static b2Body* createCircle(b2World *world, float posX, float posY, float radius, bool isStatic=false,void *userData=NULL
		,float density=10,float friction=0.3f,float restitution=0.0f,bool isRotation=false,b2Filter *filter=NULL);
	static void shrinkBox(b2Body* body,float32 width,float32 height,float32 scale);

};

#endif // __B2EASY_BOX2D_H__

