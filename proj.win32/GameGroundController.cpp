#include "GameGroundController.h"

using namespace cocos2d;

GameGroundController::GameGroundController(void)
{
}


GameGroundController::~GameGroundController(void)
{
}

bool GameGroundController::init(){
	bool pRet = false;

    do{

        // 先调用超类的init方法
		CC_BREAK_IF(! CCNode::init());

 

        view = GameBoardView::create();

        this->addChild(view, 0);

 

        pRet = true;
    }while(0)

    return pRet;
}
