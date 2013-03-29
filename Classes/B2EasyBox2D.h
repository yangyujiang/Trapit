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
	* 创建并返回一个重力为10的Box2D世界
	* @return b2World
	*/
	static b2World* createWorld();
	static b2World* createWorld(b2Vec2 gravity);

	static cocos2d::CCSprite* createDebugDraw(b2World* world);
	/**
		 * 创建并返回一个矩形的b2Body刚体对象
		 * @param	world
		 * @param	posX box的x坐标，以像素为单位
		 * @param	posY box的y坐标，以像素为单位
		 * @param	boxWidth box的宽度，以像素为单位
		 * @param	boxHeight box的高度，以像素为单位
		 * @param	userData box的外观
		 * @param	isStatic box是否静止不动
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
	static void createWrapWall(b2World* world);//创建四周边界墙

};

#endif // __B2EASY_BOX2D_H__

