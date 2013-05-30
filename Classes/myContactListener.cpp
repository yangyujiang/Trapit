#include "myContactListener.h"
#include "string"
#include "Constant.h"
#include "GameInsectModel.h"
#include "GameResinBallModel.h"
#include "Enemy.h"
#include "Message.h"

using namespace cocos2d;
//检测树脂球与虫子的碰撞
bool getResinBallAndInsect(b2Contact* contact,GameResinBallModel* &resinBall,Animal* &insect){
	b2Fixture* fixtureA=contact->GetFixtureA();
	b2Fixture* fixtureB=contact->GetFixtureB();
	CCNode* dataA=(CCNode*)fixtureA->GetBody()->GetUserData();
	CCNode* dataB=(CCNode*)fixtureB->GetBody()->GetUserData();

	if(dataA==NULL||dataB==NULL) return false;//两者必须同时出现
	if(!(fixtureA->IsSensor()&&fixtureB->IsSensor())) return false;//两者都是sensor
	CCNode* nodeA=(CCNode*)fixtureA->GetUserData();
	CCNode* nodeB=(CCNode*)fixtureB->GetUserData();
	if(nodeA==NULL||nodeB==NULL) return false;//必须两个fixture都是sensor并且存在userData
	if(nodeA->getTag()==ID_SENSOR_BALL&&nodeB->getTag()==ID_BODY){
		resinBall=(GameResinBallModel*)dataA;
		insect=(Animal*)dataB;
	}else if(nodeB->getTag()==ID_SENSOR_BALL&&nodeA->getTag()==ID_BODY){
		resinBall=(GameResinBallModel*)dataB;
		insect=(Animal*)dataA;
	}else return false;//
	if(insect->getAnimalType()==ID_FRIEND_ANIMAL&&((BaseInsect*)insect)->isOutOfGas()) return false;
	return true;
}

//边界碰撞，返回与边界重叠的虫子对象
bool getInsectFromWall(b2Contact* contact,BaseInsect* &insect){
	b2Fixture* fixtureA=contact->GetFixtureA();
	b2Fixture* fixtureB=contact->GetFixtureB();
	CCNode* insectA=(CCNode*)fixtureA->GetBody()->GetUserData();
	CCNode* insectB=(CCNode*)fixtureB->GetBody()->GetUserData();

	if((fixtureA->IsSensor()|fixtureB->IsSensor())) return false;
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
	if(insectA->getTag()!=TAG_INSECT||insectB->getTag()!=TAG_INSECT) return false;
	if(!(fixtureA->IsSensor()&&fixtureB->IsSensor())) return false;//必须两个fixture都为sensor并且存在userData

	CCNode* nodeA=(CCNode*)fixtureA->GetUserData();
	CCNode* nodeB=(CCNode*)fixtureB->GetUserData();
	if(nodeA==NULL||nodeB==NULL) return false;
	if((nodeA->getTag()==ID_MANTIS_VIEW)&&nodeB->getTag()==ID_BODY){
		insect=(BaseInsect*)insectA;
		insectViewed=(BaseInsect*)insectB;
		return true;
	}else if(nodeA->getTag()==ID_BODY&&nodeB->getTag()==ID_MANTIS_VIEW){
		insect=(BaseInsect*)insectB;
		insectViewed=(BaseInsect*)insectA;
		return true;
	}
	
	return false;
}
//敌人昆虫的视野看到树脂
bool getInsectAndResinViewed(b2Contact* contact,EnemyInsect* &insect,GameResinBallModel* &resin){
	b2Fixture* fixtureA=contact->GetFixtureA();
	b2Fixture* fixtureB=contact->GetFixtureB();
	CCNode* insectA=(CCNode*)fixtureA->GetBody()->GetUserData();
	CCNode* insectB=(CCNode*)fixtureB->GetBody()->GetUserData();
	
	if(insectA==NULL||insectB==NULL) return false;
	if(!(fixtureA->IsSensor()^fixtureB->IsSensor())) return false;

	CCNode* nodeA=(CCNode*)fixtureA->GetUserData();
	CCNode* nodeB=(CCNode*)fixtureB->GetUserData();
	if(nodeA==NULL||nodeB==NULL) return false;
	if((nodeA!=NULL&&nodeA->getTag()==ID_ENEMY_VIEW)&&nodeB->getTag()==ID_REAL_BALL){
		insect=(EnemyInsect*)insectA;
		resin=(GameResinBallModel*)insectB;
		return true;
	}else if(nodeB!=NULL&&nodeB->getTag()==ID_ENEMY_VIEW&&nodeA->getTag()==ID_REAL_BALL){
		insect=(EnemyInsect*)insectB;
		resin=(GameResinBallModel*)insectA;
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
	
	if(insectA==NULL||insectB==NULL) return false;
	if((fixtureA->IsSensor()|fixtureB->IsSensor())) return false;
	
	if(insectA->getTag()==TAG_INSECT&&insectB->getTag()==TAG_INSECT){

		CCNode* nodeA=(CCNode*)fixtureA->GetUserData();
		CCNode* nodeB=(CCNode*)fixtureB->GetUserData();
		if(nodeB->getTag()==ID_HEAD_BODY){//B为主动者
			insect=(BaseInsect*)insectB;
			insectRushed=(BaseInsect*)insectA;
			insect->setRushZone(nodeB->getTag());
			insectRushed->setRushZone(nodeA->getTag());
		}else{
			insect=(BaseInsect*)insectA;
			insectRushed=(BaseInsect*)insectB;
			insect->setRushZone(nodeA->getTag());
			insectRushed->setRushZone(nodeB->getTag());
		}
		
		return true;
	}
	return false;
}

myContactListener::myContactListener():count(0){
}

void myContactListener::BeginContact(b2Contact* contact){
//	CCLog("begin contact");
	/*if(((CCNode*)contact->GetFixtureA()->GetUserData())->getTag()==ID_BLOCK||
		((CCNode*)contact->GetFixtureB()->GetUserData())->getTag()==ID_BLOCK){//树脂球的小方块链条不做处理
			return;
	}*/
		//虫子边界检测
		BaseInsect *insect;
		if(getInsectFromWall(contact,insect)){
			insect->beginContact();
			CCLog("start contact bounding-----------------------------------");
			return;
		}
		//虫子与树脂球碰撞检测
		GameResinBallModel* resinBall;
		Animal* animal;
		if(getResinBallAndInsect(contact,resinBall,animal)){
			//检测到树脂球与虫子有重叠
			//resinBall->beginContact(insect);
			MessageVector::addMessage(resinBall,TYPE_CONTACT,animal);
			CCLog("start contact resin-----------------------------------");
			return;
		}
		//两个虫子相撞
		BaseInsect *insectRushed;
		if(getInsectFromInsect(contact,insect,insectRushed)){
			CCLog("insects contact");
			insect->beginContact();
			insectRushed->beginContact();
			return;
		}

		//视野
		BaseInsect *insectViewed;//被看到的虫子
		if(getInsectFromInsectView(contact,insect,insectViewed)){
			CCLog("start contact insectViewed");
			MessageVector::addMessage(insect,TYPE_VIEW_IN,insectViewed);
			return;
		}
		//敌人视野看到树脂球
		EnemyInsect* enemy;
		if(getInsectAndResinViewed(contact,enemy,resinBall)){
			CCLog("enemyInsect see resin");
			MessageVector::addMessage(enemy,TYPE_VIEW_IN,resinBall);
		//	insect->replaceMovement(ID_FOLLOW_FOREVER_MOVE);
			return;
		}
}

void myContactListener::EndContact(b2Contact* contact){
		//虫子与边界碰撞检测
		BaseInsect *insect;
		if(getInsectFromWall(contact,insect)){
			insect->endContact();
			CCLog("end contact bounding--------------------------------------");
			return;
		}

		//虫子与树脂球碰撞检测
		GameResinBallModel* resinBall;
		Animal* animal;
		if(getResinBallAndInsect(contact,resinBall,animal)){
			//检测到树脂球与虫子有重叠
			//resinBall->endContact();
			MessageVector::addMessage(resinBall,TYPE_CONTACT_END,animal);
			CCLog("end contact resin--------------------------------------");
		return;
		}
		//两个虫子相撞
		BaseInsect *insectRushed;
		if(getInsectFromInsect(contact,insect,insectRushed)){
			insect->endContact();
			insectRushed->endContact();
			if(insect->getRushZone()==ID_HEAD_BODY){//有一个主动者
				MessageVector::addMessage(insect,TYPE_CONTACT_END,insectRushed);
			}
			CCLog("end contact two insects");
			return;
		}

		//敌人视野离开树脂球
		EnemyInsect* enemy;
		if(getInsectAndResinViewed(contact,enemy,resinBall)){
			CCLog("enemyInsect leave resin");
			MessageVector::addMessage(enemy,TYPE_VIEW_OUT,resinBall);
		//	insect->stopMove();
		//	insect->replaceMovement(ID_NORMAL_MOVE);
			return;
		}

}

void myContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){
    //虫子与边界碰撞检测
		BaseInsect *insect;   
		if(getInsectFromWall(contact,insect)){
			insect->preSolve();
			CCLog("PreSolve--");
			CCLog("insect.pos,%f,%f",insect->getPositionX(),insect->getPositionY());
			return;
		}
		BaseInsect *insectRushed;
		if(getInsectFromInsect(contact,insect,insectRushed)){
			insect->preSolve();
			insectRushed->preSolve();
			return;
		}
}

void myContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){
       

}