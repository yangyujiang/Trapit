#include "GamePlayView.h"

using namespace cocos2d;

GamePlayView::GamePlayView()
{
}


GamePlayView::~GamePlayView()
{
}

bool GamePlayView::init(){
	bool pRet=false;
	do{ 
		// �ȵ��ó����init����
        CC_BREAK_IF(! CCLayer::init());
		
		CCLabelTTF *label = CCLabelTTF::create("Hello World from view", "Marker Felt", 48);

        // ��ȡ����ĳߴ�
        CCSize size = CCDirector::sharedDirector()->getWinSize();

        label->setPosition(ccp(size.width/2, size.height/2));

        this->addChild(label, 0);

		pRet=true;
	}while(0);

	return pRet;
}
