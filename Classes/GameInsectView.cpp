#include "GameInsectView.h"

using namespace cocos2d;

GameInsectView::GameInsectView():_insect(NULL),_insectModel(NULL),pViewDelegate(NULL),animationAction(NULL)
{
}


GameInsectView::~GameInsectView()
{
	CC_SAFE_RELEASE(_insect);
//	CC_SAFE_RELEASE(_insectModel);
}

bool GameInsectView::init(GameViewDelegate* pViewDelegate,GameInsectModel* gameInsect){
	bool pRet=false;
	do{ 
		// �ȵ��ó����init����
        CC_BREAK_IF(! CCLayer::init());
		
		CCLabelTTF *label = CCLabelTTF::create("Hello World from view", "Marker Felt", 48);
        // ��ȡ����ĳߴ�
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        label->setPosition(ccp(size.width/2, size.height/2));
        this->addChild(label, 0);

		_insectModel=gameInsect;
		this->pViewDelegate=pViewDelegate;
		_insect=CCSprite::create("ant_0.png");
		_insect->retain();
		update(0);
		addChild(_insect);

		this->initAnimation();
		this->playAnimation(_insectModel->getVelocity());

		pRet=true;
	}while(0);

	return pRet;
}

void GameInsectView::update(float dt){
	//CCLog("InsectView:update");
	if(_insect==NULL){
		return;
	}

	_insect->setPosition(_insectModel->getCenterPoint());
	_insect->setRotation(_insectModel->getRotation()+90);
	//_insect->setRotation(90);
//	CCLog("%f,%f",_insect->getPosition().x,_insect->getPosition().y);
}

void GameInsectView::testModelDelegate(){
	//CCLog("testModelDelegate");
	update(0);
}


void GameInsectView::initAnimation(){
	animation=CCAnimation::create();
	char* imageNames[]={"ant_0.png","ant_1.png","ant_2.png","ant_3.png"};
	for(int i=0;i<4;i++){
		CCTexture2D *texture=CCTextureCache::sharedTextureCache()->addImage(imageNames[i]);
		CCSpriteFrame *frame=CCSpriteFrame::createWithTexture(texture,CCRectMake(0,0,texture->getPixelsWide(),texture->getPixelsHigh())); 
		animation->addSpriteFrame(frame);
	}
}
void GameInsectView::playAnimation(float velocity){
	if(animationAction!=NULL){
		this->stopAction(animationAction);
		animationAction=NULL;
	}
	float var=25.0/velocity;
	animation->setDelayPerUnit(var);
	animation->setLoops(-1);
	animationAction=CCAnimate::create(animation);
	_insect->runAction(animationAction);
}