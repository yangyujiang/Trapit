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
	virtual void initSprite();//初始化所有精灵图片
	void playAnimation();//根据速度播放虫子行走的动画
	void initAnimation();
	virtual void initEyes(){};
	void createSpriteByInsectType(int type);//创建身体底色
	void changePlayVel(float speed);//改变动画速度
	void playAttack(CCPoint attacker,CCPoint target);//播放攻击动画
	void playKiss(CCPoint source,CCPoint des);//播放亲吻动画
	void initAttack();//初始化攻击动画帧
	void initKiss();//初始化亲吻动画帧
	void end();
	void endFinish();
	void endKiss();
	void finish();//结束
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
	CCAnimation *feelerNormalAnimation;//正常触角动画
	CCAnimation *feelerCommunicateAnimation;//交流触角动画
public:
	virtual ~AntInsectView();
	virtual void initSprite();//初始化所有精灵图片
	LAYER_CREATE_FUNC_DOUBLE_PARAM(AntInsectView,GameViewDelegate*,pViewDelegate,Animal*,gameInsect);
	void initFeeler();
	void initEyes();
	void blink(float dt);//眨眼
};

class MantisInsectView : public BaseInsectView{
protected:
	CCAnimation *feelerNormalAnimation;//正常触角动画
	CCAnimation *eyeAnimation;
public:
	virtual ~MantisInsectView();
	virtual void initSprite();//初始化所有精灵图片
	LAYER_CREATE_FUNC_DOUBLE_PARAM(MantisInsectView,GameViewDelegate*,pViewDelegate,Animal*,gameInsect);
	void initFeeler();
	void initEyes();
	void blink(float dt);//眨眼
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
