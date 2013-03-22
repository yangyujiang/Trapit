#include "GamePlayModel.h"
#include "B2EasyBox2D.h"

USING_NS_CC;


GamePlayModel::~GamePlayModel(void)
{
	CC_SAFE_DELETE(_world);
}

bool GamePlayModel::init(){
	bool pRet=false;
	do{ 
		_world=B2EasyBox2D::createWorld(b2Vec2(0,0));

		pRet=true;
	}while(0);

	return pRet;
}

b2World* GamePlayModel::getWorld(){
	return _world;
}