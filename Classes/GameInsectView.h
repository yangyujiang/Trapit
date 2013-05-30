#ifndef __GAME_INSECT_VIEW_H__
#define __GAME_INSECT_VIEW_H__

#include "cocos2d.h"
#include "Delegate.h"
#include "GlobalDef.h"
#include "GameInsectModel.h"
#include "Enemy.h"

USING_NS_CC;

class BaseInsectView :public CCLayer,public GameModelDelegate
{
private:
	GameViewDelegate* pViewDelegate;
protected:
	CC_SYNTHESIZE(int,_insectType,InsectType);
	char* insectFrames[10];
	unsigned int frameCount;
	CCSprite* _insect;
	Animal* _insectModel;

	CCRect winRect;

	CCAnimation *animation;
	CCSpeed* animationAction;
	CCAnimation *kissAnimation;
protected:
	void cleanFrames();
public:
	CCSprite* getSprite();
	virtual void initSprite();//��ʼ�����о���ͼƬ
	void playAnimation();//�����ٶȲ��ų������ߵĶ���
	void initAnimation();
	virtual void initEyes(){};
	void createSpriteByInsectType(int type);//���������ɫ
	void changePlayVel(float speed);//�ı䶯���ٶ�
	void playAttack(CCPoint attacker,CCPoint target);//���Ź�������
	void playKiss(CCPoint source,CCPoint des);//�������Ƕ���
	void initAttack();//��ʼ����������֡
	void initKiss();//��ʼ�����Ƕ���֡
	void end();
	void endFinish();
	void endKiss();
	void finish();//����
public:
	BaseInsectView();
	virtual ~BaseInsectView();
	virtual bool init(GameViewDelegate* pViewDelegate,Animal* gameInsect);
	LAYER_CREATE_FUNC_DOUBLE_PARAM(BaseInsectView,GameViewDelegate*,pViewDelegate,Animal*,gameInsect);
	virtual void testModelDelegate();
	void update(float dt);
};

class AntInsectView : public BaseInsectView{
protected:
	CCAnimation *feelerNormalAnimation;//�������Ƕ���
	CCAnimation *feelerCommunicateAnimation;//�������Ƕ���
public:
	virtual ~AntInsectView();
	virtual void initSprite();//��ʼ�����о���ͼƬ
	LAYER_CREATE_FUNC_DOUBLE_PARAM(AntInsectView,GameViewDelegate*,pViewDelegate,Animal*,gameInsect);
	void initFeeler();
	void initEyes();
	void blink(float dt);//գ��
};

class MantisInsectView : public BaseInsectView{
protected:
	CCAnimation *feelerNormalAnimation;//�������Ƕ���
	CCAnimation *eyeAnimation;
public:
	virtual ~MantisInsectView();
	virtual void initSprite();//��ʼ�����о���ͼƬ
	LAYER_CREATE_FUNC_DOUBLE_PARAM(MantisInsectView,GameViewDelegate*,pViewDelegate,Animal*,gameInsect);
	void initFeeler();
	void initEyes();
	void blink(float dt);//գ��
};










class EnemyView :public BaseInsectView
{
public:
	virtual ~EnemyView();
	virtual void initSprite();
	virtual void initEyes();
	void blink(float dt);

	LAYER_CREATE_FUNC_DOUBLE_PARAM(EnemyView,GameViewDelegate*,pViewDelegate,Animal*,gameInsect);

	void update(float dt);
};

#endif //__GAME_INSECT_VIEW_H__
