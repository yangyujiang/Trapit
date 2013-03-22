#include "GamePlayController.h"

USING_NS_CC;

GamePlayController::GamePlayController(void)
{
}


GamePlayController::~GamePlayController(void)
{
}

bool GamePlayController::init(){
	bool pRet = false;

    do{
        // �ȵ��ó����init����
		CC_BREAK_IF(! CCLayer::init());
		
		view = GamePlayView::create();

        this->addChild(view, 0);

        pRet = true;
    }while(0);
    return pRet;
}

CCScene* GamePlayController::scene(){
    CCScene *scene = NULL;
    do{
        scene = CCScene::create();
        CC_BREAK_IF(!scene);

        GamePlayController *layer = GamePlayController::create();
        CC_BREAK_IF(!layer);

        scene->addChild(layer);
    }while(0);

    return scene;
}