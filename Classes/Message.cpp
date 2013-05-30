#include "Message.h"
#include "Constant.h"
#include "GameInsectModel.h"
#include "GameResinBallModel.h"
#include "Enemy.h"
#include "GameInsectView.h"

USING_NS_CC;
using std::vector;
//
//Message.cpp
//

static std::vector<Message> _messages;

void Message::set(void* sender,unsigned int type,void* receiver){
	m_sender=sender;
	m_type=type;
	m_receiver=receiver;
}

std::vector<Message> MessageVector::sharedMessages(){
	return _messages;
}
MessageVector::~MessageVector(){
	_messages.clear();
}

void MessageVector::addMessage(void* sender,unsigned int type,void* receiver){
	Message message;
	message.set(sender,type,receiver);
	_messages.push_back(message);
}

//一定概率进入交流行为，进入则返回true，否则返回false
bool communicateByRandom(BaseInsect* insectA,BaseInsect* insectB){
	float random=CCRANDOM_0_1();
	const float possibility=0.1f;
	if(random<possibility){//0.1的概率进入交流状态
		random=CCRANDOM_0_1();//再产生一个概率，决定是否亲吻
		if(random<possibility){//亲吻
			insectA->setKissEnable(true);
			insectB->setKissEnable(true);
		}
		insectA->replaceAction(ID_ACTION_COMMUNICATE,insectB);
		insectB->replaceAction(ID_ACTION_COMMUNICATED,insectA);
		CCLog("start communicate");
		return true;
	}
	return false;
}

//一定概率逃跑
bool escapeByRandom(BaseInsect* insect){
	float random=CCRANDOM_0_1();
	const float possibility=0.8f;
	if(random<possibility){
		insect->replaceActionIfLevelUp(ID_ACTION_ESCAPE);
		CCLog("start escape");
		return true;
	}
	return false;
}

//一定概率跟踪
bool followInsectByRandom(BaseInsect* insectA,BaseInsect* insectB){
	float random=CCRANDOM_0_1();//0-1的随机数
	const float possibility=0.1f;
	if(random<possibility){//0.1的概率进入跟踪状态
		if(insectB->getActionType()==ID_ACTION_FOLLOW&&insectB->getTarget()->getID()==insectA->getID()){
			//若B正处于跟踪行为，且跟踪目标是A，则A不能跟踪B
		}else if(insectB->getFollowed()){
			//若B正在被人跟，则A不能跟踪B
		}else{
			insectA->replaceAction(ID_ACTION_FOLLOW,insectB);
		}
		CCLog("start follow");
		return true;
	}
	return false;
}

//一定概率螳螂攻击蚂蚁
bool attackByRandom(MantisInsect* mantis,AntInsect* ant){
	float random=CCRANDOM_0_1();//0-1的随机数
	const float possibility=0.1f;//0.1的概率
	if(random<possibility){
		mantis->replaceAction(ID_ACTION_ATTACK,ant);
		CCLog("start mantis attack ant");
		return true;
	}
	return false;
}

//一定概率进入摸索行为
bool gropeByRandom(BaseInsect* insectA,BaseInsect* insectB){
	float random=CCRANDOM_0_1();//0-1的随机数
	const float possibility=0.1f;//0.1的概率
	if(random<possibility){
		insectA->replaceAction(ID_ACTION_GROPE,insectB);
		CCLog("start grope");
		return true;
	}
	return false;
}

void MessageVector::handleMessage(){
	vector<Message>::iterator it = _messages.begin(); 
	while (it != _messages.end()) { 
		Message message=(Message)*it;     
		CCNode* sender=(CCNode*)message.m_sender;
		CCNode* receiver=(CCNode*)message.m_receiver;
		CC_BREAK_IF(sender==NULL||receiver==NULL);
		switch(message.m_type){
		case TYPE_ATTACKED://被攻击
			{
				AntInsect* insectA=(AntInsect*)sender;
				MantisInsect* insectB=(MantisInsect*)receiver;
				if(insectA->getInsectType()==ID_ANT&&insectB->getInsectType()==ID_MANTIS){
					//若蚂蚁被螳螂攻击，有一定几率逃跑
					insectA->attacked(insectB->getAttack());//攻击掉血一次
					CC_BREAK_IF(escapeByRandom(insectA));
				}
				break;
			}
		case TYPE_GROPE://摸索结束
			{
				BaseInsect* insectA=(BaseInsect*)sender;
				BaseInsect* insectB=(BaseInsect*)receiver;
				if(insectA->getInsectType()==ID_ANT&&insectB->getInsectType()==ID_MANTIS){
					//若蚂蚁摸索到螳螂，有一定几率逃跑
					CC_BREAK_IF(escapeByRandom(insectA));
				}
				if(insectA->getInsectType()==insectB->getInsectType()){
					//若两者同种类，则一定几率交流、亲吻
					CC_BREAK_IF(communicateByRandom(insectA,insectB));
				}

				break;
			}
		case TYPE_VIEW_IN://视野进入
			{
				Animal* insectSender=(Animal*)sender;
				if(insectSender->getInsectType()==ID_ENEMY){//若是敌人（只能看到树脂球），
					GameResinBallModel* resin=(GameResinBallModel*)message.m_receiver;
					insectSender->replaceActionIfLevelUp(ID_ACTION_FOLLOW);//敌人开始跟踪树脂球
					CCLog("start follow resin");
					break;
				}
				else if(insectSender->getAnimalType()==ID_FRIEND_ANIMAL){//若是虫子，一定概率进入
					BaseInsect* insectA=(BaseInsect*)insectSender;
					BaseInsect* insectB=(BaseInsect*)receiver;
					if(insectA->getInsectType()==ID_MANTIS&&insectB->getInsectType()==ID_ANT){
						//若螳螂视野看到蚂蚁，一定概率做出攻击行为
						CC_BREAK_IF(attackByRandom((MantisInsect*)insectA,(AntInsect*)insectB));
					}
					if(insectA->getInsectType()==insectB->getInsectType()){
						//若是同种类虫子，则一定概率
					}
				}
				break;
			}
		case TYPE_VIEW_OUT://视野出
			{
				Animal* insectSender=(Animal*)sender;
				if(insectSender->getInsectType()==ID_ENEMY){//若是敌人（只能看到树脂球），
					GameResinBallModel* resin=(GameResinBallModel*)message.m_receiver;

					insectSender->replaceActionIfLevelUp(ID_ACTION_NORMAL);
					CCLog("end follow resin");
				}
				break;
			}
		case TYPE_CONTACT://碰撞
		case TYPE_CONTACT_END:
			{
				if(sender->getTag()==TAG_RESINBALL&&receiver->getTag()==TAG_INSECT){
					//虫子与树脂的碰撞
					Animal* insect=(Animal*)receiver;
					GameResinBallModel* resin=(GameResinBallModel*)sender;
					switch(insect->getAnimalType()){
					case ID_FRIEND_ANIMAL://虫子被吃
						if(message.m_type==TYPE_CONTACT){//若是接触进入，则开始覆盖虫子
							resin->beginCover((BaseInsect*)insect);
							resin->reduceBodyVel(0.1f);//树脂球速度减半
							//若蚂蚁摸索到螳螂，有一定几率逃跑
							CC_BREAK_IF(escapeByRandom((BaseInsect*)insect));
							CCLog("begin cover insect");
						}else{//若是接触离开，则结束覆盖虫子
							resin->endCover();
							CC_BREAK_IF(escapeByRandom((BaseInsect*)insect));
							CCLog("end cover insect");
						}
						break;
					case ID_ENEMY_ANIMAL://树脂球被吃
						EnemyInsect* enemyInsect=((EnemyInsect*)insect);
						if(message.m_type==TYPE_CONTACT){//若是接触进入，则开始吃树脂球
							enemyInsect->startAttack();
							CCLog("start attack resin");
						}
						else{//否则，结束吃树脂球，并先停留N秒(有bug，暂时不停留)							
							enemyInsect->endAttack();
						//	enemyInsect->startAction(ID_ACTION_FOLLOW);
							//enemyInsect->finishMove();
						//	enemyInsect->startAction(ID_ACTION_STAY);
							CCLog("end attack resin");
						}
						break;
					}
				}else if(((BaseInsect*)sender)->getAnimalType()==ID_FRIEND_ANIMAL&&
					((BaseInsect*)sender)->getAnimalType()==ID_FRIEND_ANIMAL){
				//两个虫子(非敌人)相撞 按概率进入某一行为状态（摸索、跟踪，攻击）
					BaseInsect* insectA=(BaseInsect*)sender;
					BaseInsect* insectB=(BaseInsect*)receiver;
					
					if(insectA->getInsectType()==ID_ANT&&insectB->getInsectType()==ID_ANT){
						//若是蚂蚁，一定概率进入跟踪行为
						CC_BREAK_IF(followInsectByRandom(insectA,insectB));
					}
					//一定概率进入摸索行为
					CC_BREAK_IF(gropeByRandom(insectA,insectB));
					
				}
				break;
			}
		} 
		
		it=_messages.erase(it);
	}
}


static std::vector<Message> _messagesView;

std::vector<Message> MessageView::sharedMessages(){
	return _messagesView;
}
MessageView::~MessageView(){
	_messagesView.clear();
}

void MessageView::addMessage(void* sender,unsigned int type,void* receiver){
	Message message;
	message.set(sender,type,receiver);
	_messagesView.push_back(message);
}


void MessageView::handleMessage(){
	vector<Message>::iterator it = _messagesView.begin(); 
	while (it != _messagesView.end()) { 
		Message message=(Message)*it;     
		BaseInsectView* view=(BaseInsectView*)message.m_sender;
		BaseInsect* insect=(BaseInsect*)message.m_receiver;
		switch(message.m_type){
		case VIEW_CHANGE_VEL:
			view->changePlayVel(insect->getVelocity()/insect->getBaseVelocity());
			break;
		case VIEW_ATTACK:
			break;
		case VIEW_KISS:

			break;
		} 
		
		it=_messagesView.erase(it);
	}
}