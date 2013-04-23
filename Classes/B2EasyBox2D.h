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
		 * @param	userData box�����
		 * @param	isStatic box�Ƿ�ֹ����
		 * @return b2Body
		 */
	static b2Body* createBox(b2World* world,float posX,float posY,float boxWidth,float boxHeight,void *userData=NULL
		,b2Filter *filter=NULL,bool isSensor=false);
	static b2Body* createStaticBox(b2World* world,float posX,float posY,float boxWidth,float boxHeight,void *userData=NULL
		,b2Filter *filter=NULL,bool isSensor=false);

	static b2Body* createCircle(b2World *world, float posX, float posY, float radius,void *userData=NULL
		,b2Filter *filter=NULL,bool isSensor=false);
	
	static b2Body* createStaticCircle(b2World *world, float posX, float posY, float radius,void *userData=NULL
		,b2Filter *filter=NULL,bool isSensor=false);
	static void shrinkBox(b2Body* body,float32 width,float32 height,float32 scale);

	static void createFixture(b2Body* body,float posX,float posY,float );
	//static void createWrapWall(b2World* world);//�������ܱ߽�ǽ

};

#endif // __B2EASY_BOX2D_H__

