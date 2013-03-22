#include "GameInsectModel.h"

USING_NS_CC;

GameInsectModel::GameInsectModel(void)
{
}


GameInsectModel::~GameInsectModel(void)
{
}

bool GameInsectModel::init(){
	bool pRet=false;
	do{ 		
		

		pRet=true;
	}while(0);

	return pRet;
}






float GameInsectModel::getVelocity(){
	return _velocity;
}
void GameInsectModel::setVelocity(float velocity){
	_velocity=velocity;
}
