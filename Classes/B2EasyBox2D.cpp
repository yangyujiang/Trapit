/**
 * API list:
 * 
 * createWorld 		����������һ������Ϊ10��Box2D����
 * createDebug 		����Box2D Debug���󣬵���Box2DӦ��
 * createBox 		����������һ�����ε�b2Body�������
 * createCircle 	����������һ��Բ�θ��壬ͬ�������漰������Ĳ�������������Ϊ��λ
 * createPolygon 	����һ�鶥�����ݣ���������θ��壬������˳ʱ����ƣ�Ҳ������ʱ����ƣ������ܳ��ֽ���
 * createWrapWall 	��Box2D�����д���Χ��canvas���ܵ�ǽ�壬
 * getBodyAtMouse	��ȡ�����λ�õĸ���
 * startDragBody	�����ؽ��϶�����
 * stopDragBody		ֹͣ�϶�����
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
	
	//3.˯�ŵĶ����Ƿ�ģ��
	bool doSleep = true;
	//4.����b2World����
	b2World *_world = new b2World(gravity);
	
	_world->SetAllowSleeping(doSleep);

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
/*
b2Body* B2EasyBox2D::createBox(b2World* world,float posX,float posY,float boxWidth,float boxHeight,bool isStatic){

	b2BodyDef bodyRequest;
	if(!isStatic) bodyRequest.type=b2_dynamicBody;
	bodyRequest.position.Set(posX/PTM_RATIO,posY/PTM_RATIO);
	b2Body *body=world->CreateBody(&bodyRequest);

	b2PolygonShape shapeRequest;
	shapeRequest.SetAsBox(boxWidth/2.0f/PTM_RATIO,boxHeight/2.0f/PTM_RATIO);

	b2FixtureDef fixtureRequest;
	fixtureRequest.shape=&shapeRequest;
	if(!isStatic) fixtureRequest.density=3.f;
	fixtureRequest.friction=0.3f;
	fixtureRequest.restitution=0.f;

	b2Fixture *fixture=body->CreateFixture(&fixtureRequest);

	return body;
}
b2Body* B2EasyBox2D::createBox(b2World* world,float posX,float posY,float boxWidth,float boxHeight,bool isStatic,float angle){


	return B2EasyBox2D::createBox(world,posX,posY,boxWidth,boxHeight,isStatic,angle,3.f,0.3f,0.3f);
}*/
b2Body* B2EasyBox2D::createBox(b2World* world,float posX,float posY,float boxWidth,float boxHeight,bool isStatic,void *userData,
	float angle,float density,float friction,float restitution,b2Filter *filter,bool isSensor){

	b2BodyDef bodyRequest;
	if(!isStatic) bodyRequest.type=b2_dynamicBody;
	bodyRequest.position.Set(posX/PTM_RATIO,posY/PTM_RATIO);
	
	if ( userData != NULL) {
		bodyRequest.userData = userData;
	}
	b2Body *body=world->CreateBody(&bodyRequest);

	b2PolygonShape shapeRequest;
	shapeRequest.SetAsBox(boxWidth/2.0f/PTM_RATIO,boxHeight/2.0f/PTM_RATIO,b2Vec2(0,0),angle);

	b2FixtureDef fixtureRequest;
	fixtureRequest.shape=&shapeRequest;
	if(!isStatic) fixtureRequest.density=density;
	fixtureRequest.friction=friction;
	fixtureRequest.restitution=restitution;
	fixtureRequest.isSensor=isSensor;
	if(filter!=NULL) fixtureRequest.filter=*filter;
	b2Fixture *fixture=body->CreateFixture(&fixtureRequest);

	return body;
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
	shapeRequest.SetAsBox(width*scale/2.0f/PTM_RATIO,height*scale/2.0f/PTM_RATIO,b2Vec2(0,0),angle);
	
	b2FixtureDef fixtureRequest;
	fixtureRequest.shape=&shapeRequest;
	fixtureRequest.density=density;
	fixtureRequest.friction=friction;
	fixtureRequest.restitution=restitution;
	body->CreateFixture(&fixtureRequest);
}

/**
		 * ����������һ��Բ�θ��壬ͬ�������漰������Ĳ�������������Ϊ��λ
		 * @param	world �������и����Box2D����
		 * @param	posX	�����x���꣬������Ϊ��λ
		 * @param	posY	�����y���꣬������Ϊ��λ
		 * @param	radius	����İ뾶��������Ϊ��λ
		 * @param	isStatic	�����Ƿ�Ϊ��ֹ����Ĭ��Ϊfalse
		 * @param	userData	�������ۣ�Ĭ��Ϊnull
		 * @return ����һ��Բ�θ���
		 */
b2Body* B2EasyBox2D::createCircle(b2World *world, float posX, float posY, float radius, bool isStatic,void *userData
	,float density,float friction,float restitution,bool isRotation,b2Filter *filter,bool isSensor) {
			//1.������������b2BodyDef
			b2BodyDef bodyRequest;
			if(!isStatic) bodyRequest.type=b2_dynamicBody;
			if(!isRotation) bodyRequest.fixedRotation=true;
			bodyRequest.position.Set(posX / PTM_RATIO, posY / PTM_RATIO);//�ǵ��׺����ص�ת����ϵ
		
			if ( userData != NULL) {
				//((CCNode*)userData)->setPosition(ccp(posX,posY));
				 bodyRequest.userData = userData;
			}
			
			//2.Box2D���繤���������󴴽�createBody()��������
			b2Body *circle = world->CreateBody(&bodyRequest);

			//2.������״
			b2CircleShape shapeCircle;
			shapeCircle.m_radius=radius/PTM_RATIO;
			
			//3.����������״����b2FixtureDef������
			b2FixtureDef fixtureRequest;
			if(!isStatic) fixtureRequest.density=density;
			fixtureRequest.friction = friction;
			fixtureRequest.restitution = restitution;
			fixtureRequest.isSensor=isSensor;
			fixtureRequest.shape = &shapeCircle;
			if(filter!=NULL)	fixtureRequest.filter=*filter;
			
			//4.b2Body���幤����������CreateFixture������״			
			circle->CreateFixture(&fixtureRequest);
			
			return circle;
		}


void B2EasyBox2D::createWrapWall(b2World* _world){
	
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();

	B2EasyBox2D::createBox(_world,winSize.width/2,winSize.height,winSize.width,PTM_RATIO,true,NULL,0,0,0);
	B2EasyBox2D::createBox(_world,0,winSize.height/2,PTM_RATIO,winSize.height,true,0,1,0);
	B2EasyBox2D::createBox(_world,winSize.width,winSize.height/2,PTM_RATIO,winSize.height,true,NULL,0,0,0);
	B2EasyBox2D::createBox(_world,winSize.width/2,0,winSize.width,PTM_RATIO,true,NULL,0,0,0);
	//������̬�ϰ���
	B2EasyBox2D::createBox(_world,100,100,50,150,true,NULL,0,0,0);
	B2EasyBox2D::createBox(_world,600,100,50,50,true,NULL,0,0,0);
	B2EasyBox2D::createBox(_world,300,300,100,100,true,NULL,0,0,0);
	B2EasyBox2D::createCircle(_world,500,500,50,true,NULL,0,0,0);
}

/**
		 * ����һ�鶥�����ݣ���������θ��壬������˳ʱ����ƣ�Ҳ������ʱ����ƣ������ܳ��ֽ���
		 * @param	world Box2D����
		 * @param	vertices �������飬����֮�䲻���н���
		 * @param	isStatic	�Ƿ�Ϊ��ֹ�ĸ���
		 * @param	fillData	������������һ��BitmapData������ȷ������BitmapData�ĳߴ������̨�ĳߴ�
		 * @param	stage	���userData����̨,����ָ��������,���޷�������������
		 * @return ����һ������θ���
		 * @link	http://www.ladeng6666.com/blog/index.php/2012/08/10/box2d%e5%a4%9a%e8%be%b9%e5%bd%a2%e5%88%9a%e4%bd%93%e8%b4%b4%e5%9b%be/
		 */
/*b2Body* B2EasyBox2D::createPolygon(b2World *world, Array<b2Vec2> vertices, bool isStatic = false, fillData:BitmapData = null,stage:DisplayObjectContainer=null):b2Body {
			//1.������������b2BodyDef
			var bodyRequest:b2BodyDef = new b2BodyDef();
			bodyRequest.type = isStatic? b2Body.b2_staticBody:b2Body.b2_dynamicBody;
			bodyRequest.position.Set(0 , 0);//�ǵ��׺����ص�ת����ϵ
			
			//3.����������״����b2ShapeDef������	
				//�������θ�����״����
			var fixtureRequest:b2FixtureDef = new b2FixtureDef();
			fixtureRequest.density = 3;
			fixtureRequest.friction = 0.3;
			fixtureRequest.restitution = 0.2;
			//����һ��Separator����
			var separator:b2Separator = new b2Separator();
			//��֤�����Ƿ���ϴ�������εı�׼
			var validate:int = separator.Validate(vertices);
			//����Ƕ������˳ʱ�벻����׼����ת�����еĶ���˳��
			if (validate == 2) {
				vertices.reverse();
			}else if (validate != 0) {
				//��������϶���α�׼������
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
			//2.Box2D���繤���������󴴽�createBody()��������
			var body:b2Body=world.CreateBody(bodyRequest);
			//������ֽ�ɶ��͹����Σ���ϳɸ��ӵĶ����
			separator.Separate(body, fixtureRequest, vertices);
			
			return body;
		}*/