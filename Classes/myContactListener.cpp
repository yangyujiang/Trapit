#include "myContactListener.h"
#include "string"
#include "Constant.h"
#include "GameInsectModel.h"
#include "GameResinBallModel.h"
#include "Enemy.h"
#include "Message.h"

using namespace cocos2d;

bool getResinBallAndInsect(b2Contact* contact,GameResinBallModel* &resinBall,BaseInsect* &insect){//检测树脂球与虫子的碰撞
	b2Fixture* fixtureA=contact->GetFixtureA();
	b2Fixture* fixtureB=contact->GetFixtureB();
	CCNode* dataA=(CCNode*)fixtureA->GetBody()->GetUserData();
	CCNode* dataB=(CCNode*)fixtureB->GetBody()->GetUserData();

	if(dataA==NULL||dataB==NULL) return false;//两者必须同时出现
	if(!(fixtureA->IsSensor()^fixtureB->IsSensor())) return false;//两者有且只有有一个sensor

	if(dataA->getTag()==TAG_RESINBALL&&dataB->getTag()==TAG_INSECT) {
		resinBall=(GameResinBallModel*)dataA;
		insect=(BaseInsect*)dataB;
	}else if(dataB->getTag()==TAG_RESINBALL&&dataA->getTag()==TAG_INSECT){
		resinBall=(GameResinBallModel*)dataB;
		insect=(BaseInsect*)dataA;
	}else return false;//
	return true;
}
bool getResinBallAndEnemy(b2Contact* contact,GameResinBallModel* &resinBall,Enemy* &enemy){//检测树脂球与虫子的碰撞
	b2Fixture* fixtureA=contact->GetFixtureA();
	b2Fixture* fixtureB=contact->GetFixtureB();
	CCNode* dataA=(CCNode*)fixtureA->GetBody()->GetUserData();
	CCNode* dataB=(CCNode*)fixtureB->GetBody()->GetUserData();

	if(dataA==NULL||dataB==NULL) return false;//两者必须同时出现
	if(!(fixtureA->IsSensor()^fixtureB->IsSensor())) return false;//两者有且只有有一个sensor

	if(dataA->getTag()==TAG_RESINBALL&&dataB->getTag()==TAG_ENEMY) {
		resinBall=(GameResinBallModel*)dataA;
		enemy=(Enemy*)dataB;
	}else if(dataB->getTag()==TAG_RESINBALL&&dataA->getTag()==TAG_ENEMY){
		resinBall=(GameResinBallModel*)dataB;
		enemy=(Enemy*)dataA;
	}else return false;//
	return true;
}

//边界碰撞，返回与边界重叠的虫子对象
bool getInsectFromWall(b2Contact* contact,BaseInsect* &insect){
	b2Fixture* fixtureA=contact->GetFixtureA();
	b2Fixture* fixtureB=contact->GetFixtureB();
	CCNode* insectA=(CCNode*)fixtureA->GetBody()->GetUserData();
	CCNode* insectB=(CCNode*)fixtureB->GetBody()->GetUserData();

	CCNode* nodeA=(CCNode*)fixtureA->GetUserData();
	CCNode* nodeB=(CCNode*)fixtureB->GetUserData();
	if((nodeA!=NULL&&nodeA->getTag()==ID_MANTIS_VIEW)||(nodeB!=NULL&&nodeB->getTag()==ID_MANTIS_VIEW))
		return false;
	if(insectA==NULL||insectB==NULL) return false;

	if(insectA->getTag()==TAG_INSECT&&insectB->getTag()==TAG_WALL){
		insect=(BaseInsect*)insectA;
		return true;
		}
	if(insectB->getTag()==TAG_INSECT&&insectA->getTag()==TAG_WALL){
		insect=(BaseInsect*)insectB;
		return true;
	}
	return false;
}

//一个昆虫的视野看到另一个昆虫
bool getInsectFromInsectView(b2Contact* contact,BaseInsect* &insect,BaseInsect* &insectViewed){
	b2Fixture* fixtureA=contact->GetFixtureA();
	b2Fixture* fixtureB=contact->GetFixtureB();
	CCNode* insectA=(CCNode*)fixtureA->GetBody()->GetUserData();
	CCNode* insectB=(CCNode*)fixtureB->GetBody()->GetUserData();
	
	if(insectA==NULL||insectB==NULL) return false;
	if(!(fixtureA->IsSensor()^fixtureB->IsSensor())) return false;

	CCNode* nodeA=(CCNode*)fixtureA->GetUserData();
	CCNode* nodeB=(CCNode*)fixtureB->GetUserData();
	if((nodeA!=NULL&&nodeA->getTag()==ID_MANTIS_VIEW)){
		insect=(MantisInsect*)insectA;
		insectViewed=(MantisInsect*)insectB;
		return true;
	}else if(nodeB!=NULL&&nodeB->getTag()==ID_MANTIS_VIEW){
		insect=(MantisInsect*)insectB;
		insectViewed=(MantisInsect*)insectA;
		return true;
	}
	
	return false;
}

//一个昆虫撞到另一个昆虫
bool getInsectFromInsect(b2Contact* contact,BaseInsect* &insect,BaseInsect* &insectRushed){
	b2Fixture* fixtureA=contact->GetFixtureA();
	b2Fixture* fixtureB=contact->GetFixtureB();
	CCNode* insectA=(CCNode*)fixtureA->GetBody()->GetUserData();
	CCNode* insectB=(CCNode*)fixtureB->GetBody()->GetUserData();
	
	CCNode* nodeA=(CCNode*)fixtureA->GetUserData();
	CCNode* nodeB=(CCNode*)fixtureB->GetUserData();
	if((nodeA!=NULL&&nodeA->getTag()==ID_MANTIS_VIEW)||(nodeB!=NULL&&nodeB->getTag()==ID_MANTIS_VIEW))
		return false;
	if(insectA==NULL||insectB==NULL) return false;

	
	if(insectA->getTag()==TAG_INSECT&&insectB->getTag()==TAG_INSECT){
		insect=(MantisInsect*)insectA;
		insectRushed=(MantisInsect*)insectB;
		return true;
		}
	return false;
}

myContactListener::myContactListener():count(0){
}

void myContactListener::BeginContact(b2Contact* contact){
		//CCLog("start contact~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	
		//虫子边界检测
		BaseInsect *insect;
		if(getInsectFromWall(contact,insect)){
			insect->beginContact();
			CCLog("start contact bounding-----------------------------------");
			return;
		}
		//虫子与树脂球碰撞检测
		GameResinBallModel* resinBall;
		if(getResinBallAndInsect(contact,resinBall,insect)){
			//检测到树脂球与虫子有重叠
			resinBall->beginContact(insect);
		CCLog("start contact resin-----------------------------------");
		return;
		}
		BaseInsect *insectRushed;
		if(getInsectFromInsect(contact,insect,insectRushed)){
			insect->beginContact();
			insectRushed->beginContact();
		}

		//视野
		BaseInsect *insectViewed;//被看到的虫子
		if(getInsectFromInsectView(contact,insect,insectViewed)){
			CCLog("start contact insectViewed");
			MessageVector::addMessage(insect,TYPE_VIEW,insectViewed);
		}
		/*Enemy* enemy;
		if(getResinBallAndEnemy(contact,resinBall,enemy)){

			CCLog("start contact enemy");
			enemy->setEating(true);
			return;
		}*/
}

void myContactListener::EndContact(b2Contact* contact){
		//CCLog("end contact~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		//虫子与边界碰撞检测
		BaseInsect *insect;
		if(getInsectFromWall(contact,insect)){
			insect->endContact();
			CCLog("end contact bounding--------------------------------------");
			return;
		}

		//虫子与树脂球碰撞检测
		GameResinBallModel* resinBall;
		if(getResinBallAndInsect(contact,resinBall,insect)){
			//检测到树脂球与虫子有重叠
			resinBall->endContact();
		CCLog("end contact resin--------------------------------------");
		return;
		}

		BaseInsect *insectRushed;
		if(getInsectFromInsect(contact,insect,insectRushed)){
			insect->endContact();
			insectRushed->endContact();
		}

		/*Enemy* enemy;
		if(getResinBallAndEnemy(contact,resinBall,enemy)){

			CCLog("end contact enemy");
			enemy->setEating(false);
			return;
		}*/
}

void myContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){
    //虫子与边界碰撞检测
		BaseInsect *insect;   
		if(getInsectFromWall(contact,insect)){
			insect->preSolve();
			CCLog("PreSolve--");
			return;
		}
		BaseInsect *insectRushed;
		if(getInsectFromInsect(contact,insect,insectRushed)){
			insect->preSolve();
			insectRushed->preSolve();
		}
}

void myContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){
       

}