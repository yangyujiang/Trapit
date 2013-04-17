#include "myContactListener.h"
#include "string"
#include "Constant.h"
#include "GameInsectModel.h"
#include "GameResinBallModel.h"
#include "Enemy.h"

using namespace cocos2d;

bool getResinBallAndInsect(b2Contact* contact,GameResinBallModel* &resinBall,GameInsectModel* &insect){//检测树脂球与虫子的碰撞
	b2Fixture* fixtureA=contact->GetFixtureA();
	b2Fixture* fixtureB=contact->GetFixtureB();
	CCNode* dataA=(CCNode*)fixtureA->GetBody()->GetUserData();
	CCNode* dataB=(CCNode*)fixtureB->GetBody()->GetUserData();

	if(dataA==NULL||dataB==NULL) return false;//两者必须同时出现
	if(!(fixtureA->IsSensor()^fixtureB->IsSensor())) return false;//两者有且只有有一个sensor

	if(dataA->getTag()==TAG_RESINBALL&&dataB->getTag()==TAG_INSECT) {
		resinBall=(GameResinBallModel*)dataA;
		insect=(GameInsectModel*)dataB;
	}else if(dataB->getTag()==TAG_RESINBALL&&dataA->getTag()==TAG_INSECT){
		resinBall=(GameResinBallModel*)dataB;
		insect=(GameInsectModel*)dataA;
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

bool getInsect(b2Contact* contact,GameInsectModel* &insect){
	b2Fixture* fixtureA=contact->GetFixtureA();
	b2Fixture* fixtureB=contact->GetFixtureB();
	CCNode* insectA=(CCNode*)fixtureA->GetBody()->GetUserData();
	CCNode* insectB=(CCNode*)fixtureB->GetBody()->GetUserData();

	if(fixtureA->IsSensor()||fixtureB->IsSensor()) return false;
	
	if(insectA!=NULL){
		if(insectA->getTag()==TAG_INSECT&&insectB==NULL){
			insect=(GameInsectModel*)insectA;
		//CCLog("A:%d",insectA->getTag());
			return true;
		}
	}
	if(insectB!=NULL)
		if(insectB->getTag()==TAG_INSECT&&insectA==NULL){
		insect=(GameInsectModel*)insectB;
		//CCLog("B:%d",insectB->getTag());
		return true;
		}
	return false;
	
	/*b2Fixture* fixtureA=contact->GetFixtureA();
	b2Fixture* fixtureB=contact->GetFixtureB();

	bool sensorA=fixtureA->IsSensor();
	bool sensorB=fixtureB->IsSensor();
	if(!(sensorA^sensorB)) return false;
	GameInsectModel* insectA=(GameInsectModel*)fixtureA->GetUserData();
	GameInsectModel* insectB=(GameInsectModel*)fixtureB->GetUserData();
	if(sensorA) insect=insectA;
	else insect=insectB;
	return true;*/
}


myContactListener::myContactListener():count(0){
	b2ContactListener();
}

void myContactListener::BeginContact(b2Contact* contact){
		//CCLog("start contact~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	
		//虫子边界检测
		GameInsectModel *insect;
	/*	if(getInsect(contact,insect)){
			insect->beginContact();
			return;
		}*/
		//虫子与树脂球碰撞检测
		GameResinBallModel* resinBall;
		if(getResinBallAndInsect(contact,resinBall,insect)){
			//检测到树脂球与虫子有重叠
			resinBall->beginContact(insect);
		CCLog("start contact-----------------------------------");
		return;
		}
		Enemy* enemy;
		if(getResinBallAndEnemy(contact,resinBall,enemy)){

			CCLog("start contact enemy");
			enemy->setEating(true);
			return;
		}
}

void myContactListener::EndContact(b2Contact* contact){
		//CCLog("end contact~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		//虫子与边界碰撞检测
		GameInsectModel *insect;
	/*	if(getInsect(contact,insect)){
			insect->endContact();
			return;
		}*/

		//虫子与树脂球碰撞检测
		GameResinBallModel* resinBall;
		if(getResinBallAndInsect(contact,resinBall,insect)){
			//检测到树脂球与虫子有重叠
			resinBall->endContact();
		CCLog("end contact--------------------------------------");
		return;
		}

		Enemy* enemy;
		if(getResinBallAndEnemy(contact,resinBall,enemy)){

			CCLog("end contact enemy");
			enemy->setEating(false);
			return;
		}
}
