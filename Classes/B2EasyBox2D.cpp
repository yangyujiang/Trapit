/**
 * API list:
 * 
 * createWorld 		创建并返回一个重力为10的Box2D世界
 * createDebug 		创建Box2D Debug对象，调试Box2D应用
 * createBox 		创建并返回一个矩形的b2Body刚体对象
 * createCircle 	创建并返回一个圆形刚体，同样所有涉及到坐标的参数都是以像素为单位
 * createPolygon 	根据一组顶点数据，创建多边形刚体，可以是顺时针绘制，也可以逆时针绘制，但不能出现交叉
 * createWrapWall 	在Box2D世界中创建围绕canvas四周的墙体，
 * getBodyAtMouse	获取鼠标点击位置的刚体
 * startDragBody	用鼠标关节拖动刚体
 * stopDragBody		停止拖动刚体
 */

#include "B2EasyBox2D.h"
#include "B2DebugDrawLayer.h"

#define PTM_RATIO 32.0f

using namespace cocos2d;

B2EasyBox2D::B2EasyBox2D(void)
{
}


B2EasyBox2D::~B2EasyBox2D(void)
{
}

b2World* B2EasyBox2D::createWorld(){
	return B2EasyBox2D::createWorld(b2Vec2(0,-10));
}
b2World* B2EasyBox2D::createWorld(b2Vec2 gravity){
	
	//3.睡着的对象是否模拟
	bool doSleep = true;
	//4.创建b2World世界
	b2World *_world = new b2World(gravity);
	
	_world->SetAllowSleeping(doSleep);
	
	_world->SetAutoClearForces(false);
	_world->SetAllowSleeping(false);//设置世界是否允许睡眠

	return _world;
}

CCSprite* B2EasyBox2D::createDebugDraw(b2World* world){
	CCSprite *debugSprite = CCSprite::create();
	/*b2Draw debugDraw;
			debugDraw.m_sprite = debugSprite;
			debugDraw.m_drawScale = 30.0;
			debugDraw.m_fillAlpha = 0.5;
			debugDraw.m_lineThickness = 1.0;
			debugDraw.m_drawFlags = b2DebugDraw.e_shapeBit | b2DebugDraw.e_jointBit;
			world.SetDebugDraw(debugDraw);
		*/	
	return debugSprite;
}
b2Body* B2EasyBox2D::createStaticBox(b2World* world,float posX,float posY,float boxWidth,float boxHeight,
	void *userData,b2Filter *filter,bool isSensor){
	b2BodyDef bodyRequest;
	bodyRequest.type=b2_staticBody;
	bodyRequest.position.Set(posX/PTM_RATIO,posY/PTM_RATIO);
	bodyRequest.userData = userData;
	
	b2Body *body=world->CreateBody(&bodyRequest);

	b2PolygonShape shapeRequest;
	shapeRequest.SetAsBox(boxWidth/2.0f/PTM_RATIO,boxHeight/2.0f/PTM_RATIO);

	b2FixtureDef fixtureRequest;
	fixtureRequest.shape=&shapeRequest;
	fixtureRequest.density=1;
	fixtureRequest.friction=0.3f;
	fixtureRequest.restitution=0;
	fixtureRequest.isSensor=isSensor;
	if(filter!=NULL) fixtureRequest.filter=*filter;
	b2Fixture *fixture=body->CreateFixture(&fixtureRequest);//创建定制器
	
	return body;

}
b2Body* B2EasyBox2D::createBox(b2World* world,float posX,float posY,float boxWidth,float boxHeight,
	void *userData,b2Filter *filter,bool isSensor){
	b2BodyDef bodyRequest;
	bodyRequest.type=b2_dynamicBody;
	bodyRequest.position.Set(posX/PTM_RATIO,posY/PTM_RATIO);
	bodyRequest.userData = userData;
	b2Body *body=world->CreateBody(&bodyRequest);
	b2PolygonShape shapeRequest;
	shapeRequest.SetAsBox(boxWidth/2.0f/PTM_RATIO,boxHeight/2.0f/PTM_RATIO);

	b2FixtureDef fixtureRequest;
	fixtureRequest.shape=&shapeRequest;
	fixtureRequest.density=1;
	fixtureRequest.friction=0.f;
	fixtureRequest.restitution=1.f;
	fixtureRequest.isSensor=isSensor;
	if(filter!=NULL) fixtureRequest.filter=*filter;
	b2Fixture *fixture=body->CreateFixture(&fixtureRequest);//创建定制器

	return body;
}

/**
		 * 创建并返回一个圆形刚体，同样所有涉及到坐标的参数都是以像素为单位
		 * @param	world 承载所有刚体的Box2D世界
		 * @param	posX	刚体的x坐标，以像素为单位
		 * @param	posY	刚体的y坐标，以像素为单位
		 * @param	radius	刚体的半径，以像素为单位
		 * @param	userData	刚体的外观，默认为null
		 * @param	isStatic	刚体是否为静止对象，默认为false
		 * @return 返回一个圆形刚体
		 */
b2Body* B2EasyBox2D::createCircle(b2World *world, float posX, float posY, float radius,
	void *userData,b2Filter *filter,bool isSensor) {
			//1.创建刚体需求b2BodyDef
			b2BodyDef bodyRequest;
			bodyRequest.type=b2_dynamicBody;
			bodyRequest.position.Set(posX / PTM_RATIO, posY / PTM_RATIO);//记得米和像素的转换关系
			bodyRequest.userData = userData;
					
			//2.Box2D世界工厂根据需求创建createBody()生产刚体
			b2Body *circle = world->CreateBody(&bodyRequest);

			//2.创建形状
			b2CircleShape shapeCircle;
			shapeCircle.m_radius=radius/PTM_RATIO;
			
			//3.创建刚体形状需求b2FixtureDef的子类
			b2FixtureDef fixtureRequest;
			fixtureRequest.density=1;
			fixtureRequest.friction = 0.3f;
			fixtureRequest.restitution = 0;
			fixtureRequest.isSensor=isSensor;
			fixtureRequest.shape = &shapeCircle;
			if(filter!=NULL)	fixtureRequest.filter=*filter;
			
			//4.b2Body刚体工厂根据需求CreateFixture生产形状			
			circle->CreateFixture(&fixtureRequest);
			
			return circle;
		}

b2Body* B2EasyBox2D::createStaticCircle(b2World *world, float posX, float posY, float radius,
	void *userData,b2Filter *filter,bool isSensor){
		//1.创建刚体需求b2BodyDef
			b2BodyDef bodyRequest;
			bodyRequest.type=b2_staticBody;
			bodyRequest.position.Set(posX / PTM_RATIO, posY / PTM_RATIO);//记得米和像素的转换关系
			bodyRequest.userData = userData;			
			//2.Box2D世界工厂根据需求创建createBody()生产刚体
			b2Body *circle = world->CreateBody(&bodyRequest);

			//2.创建形状
			b2CircleShape shapeCircle;
			shapeCircle.m_radius=radius/PTM_RATIO;
			//3.创建刚体形状需求b2FixtureDef的子类
			b2FixtureDef fixtureRequest;
			fixtureRequest.density=1;
			fixtureRequest.friction = 0.3f;
			fixtureRequest.restitution = 0;
			fixtureRequest.isSensor=isSensor;
			fixtureRequest.shape = &shapeCircle;
			if(filter!=NULL)	fixtureRequest.filter=*filter;
			
			//4.b2Body刚体工厂根据需求CreateFixture生产形状			
			circle->CreateFixture(&fixtureRequest);
			
			return circle;
}

void B2EasyBox2D::shrinkBox(b2Body* body,float32 width,float32 height,float32 scale){
	float32 angle=body->GetAngle();
	b2Vec2 currentPosition=body->GetWorldCenter();
	b2Fixture* fixture=body->GetFixtureList();
	float32 friction=fixture->GetFriction();
	float32 density=fixture->GetDensity();
	float32 restitution=fixture->GetRestitution();
	body->DestroyFixture(fixture);

	b2PolygonShape shapeRequest;
	float32 widthScale=scale*2>1?1:scale*2;
	CCLog("widthScale:%f",widthScale);
	shapeRequest.SetAsBox(width*widthScale/2.0f/PTM_RATIO,height*scale/2.0f/PTM_RATIO,b2Vec2(0,0),angle);
	
	b2FixtureDef fixtureRequest;
	fixtureRequest.shape=&shapeRequest;
	fixtureRequest.density=density;
	fixtureRequest.friction=friction;
	fixtureRequest.restitution=restitution;
	body->CreateFixture(&fixtureRequest);
}

/*
void B2EasyBox2D::createWrapWall(b2World* _world){
	
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();

	B2EasyBox2D::createStaticBox(_world,winSize.width/2,winSize.height,winSize.width,PTM_RATIO);
	B2EasyBox2D::createStaticBox(_world,0,winSize.height/2,PTM_RATIO,winSize.height);
	B2EasyBox2D::createStaticBox(_world,winSize.width,winSize.height/2,PTM_RATIO,winSize.height);
	B2EasyBox2D::createStaticBox(_world,winSize.width/2,0,winSize.width,PTM_RATIO);
	//创建静态障碍物
	B2EasyBox2D::createStaticBox(_world,100,100,50,150);
	B2EasyBox2D::createStaticBox(_world,600,100,50,50);
	B2EasyBox2D::createStaticBox(_world,300,300,100,100);
	B2EasyBox2D::createStaticCircle(_world,500,500,50);
}*/

/**
		 * 根据一组顶点数据，创建多边形刚体，可以是顺时针绘制，也可以逆时针绘制，但不能出现交叉
		 * @param	world Box2D世界
		 * @param	vertices 顶点数组，顶点之间不能有交叉
		 * @param	isStatic	是否为静止的刚体
		 * @param	fillData	刚体的填充纹理，一个BitmapData对象，请确保整个BitmapData的尺寸大于舞台的尺寸
		 * @param	stage	添加userData的舞台,若不指定该属性,将无法看到刚体的外观
		 * @return 返回一个多边形刚体
		 * @link	http://www.ladeng6666.com/blog/index.php/2012/08/10/box2d%e5%a4%9a%e8%be%b9%e5%bd%a2%e5%88%9a%e4%bd%93%e8%b4%b4%e5%9b%be/
		 */
/*b2Body* B2EasyBox2D::createPolygon(b2World *world, Array<b2Vec2> vertices, bool isStatic = false, fillData:BitmapData = null,stage:DisplayObjectContainer=null):b2Body {
			//1.创建刚体需求b2BodyDef
			var bodyRequest:b2BodyDef = new b2BodyDef();
			bodyRequest.type = isStatic? b2Body.b2_staticBody:b2Body.b2_dynamicBody;
			bodyRequest.position.Set(0 , 0);//记得米和像素的转换关系
			
			//3.创建敢提形状需求b2ShapeDef的子类	
				//创建矩形刚体形状需求
			var fixtureRequest:b2FixtureDef = new b2FixtureDef();
			fixtureRequest.density = 3;
			fixtureRequest.friction = 0.3;
			fixtureRequest.restitution = 0.2;
			//创建一个Separator对象
			var separator:b2Separator = new b2Separator();
			//验证顶点是否符合创建多边形的标准
			var validate:int = separator.Validate(vertices);
			//如果是顶点因非顺时针不符标准，则反转数组中的顶点顺序
			if (validate == 2) {
				vertices.reverse();
			}else if (validate != 0) {
				//如果不符合多边形标准，跳出
				return null;
			}
			if (fillData != null && stage!=null) {
				var userData:Sprite = new Sprite();
				var commandVector:Vector.<int> = new Vector.<int>();
				var posVector:Vector.<Number> = new Vector.<Number>();
				
				commandVector.push(1);
				posVector.push(vertices[0].x * pixelPerMeter);
				posVector.push(vertices[0].y * pixelPerMeter);
				for (var i:int = 1; i < vertices.length;i++ ) {
					commandVector.push(2)
					posVector.push(vertices[i].x * pixelPerMeter);
					posVector.push(vertices[i].y * pixelPerMeter);
				}
				userData.graphics.beginBitmapFill(fillData);
				userData.graphics.drawPath(commandVector, posVector);
				userData.graphics.endFill();
				stage.addChild(userData);
				
				bodyRequest.userData = userData;
			}
			//2.Box2D世界工厂更具需求创建createBody()生产刚体
			var body:b2Body=world.CreateBody(bodyRequest);
			//将顶点分解成多个凸多边形，组合成复杂的多边形
			separator.Separate(body, fixtureRequest, vertices);
			
			return body;
		}*/