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

//һ�����ʽ��뽻����Ϊ�������򷵻�true�����򷵻�false
bool communicateByRandom(BaseInsect* insectA,BaseInsect* insectB){
	float random=CCRANDOM_0_1();
	const float possibility=0.1f;
	if(random<possibility){//0.1�ĸ��ʽ��뽻��״̬
		random=CCRANDOM_0_1();//�ٲ���һ�����ʣ������Ƿ�����
		if(random<possibility){//����
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

//һ����������
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

//һ�����ʸ���
bool followInsectByRandom(BaseInsect* insectA,BaseInsect* insectB){
	float random=CCRANDOM_0_1();//0-1�������
	const float possibility=0.1f;
	if(random<possibility){//0.1�ĸ��ʽ������״̬
		if(insectB->getActionType()==ID_ACTION_FOLLOW&&insectB->getTarget()->getID()==insectA->getID()){
			//��B�����ڸ�����Ϊ���Ҹ���Ŀ����A����A���ܸ���B
		}else if(insectB->getFollowed()){
			//��B���ڱ��˸�����A���ܸ���B
		}else{
			insectA->replaceAction(ID_ACTION_FOLLOW,insectB);
		}
		CCLog("start follow");
		return true;
	}
	return false;
}

//һ��������빥������
bool attackByRandom(MantisInsect* mantis,AntInsect* ant){
	float random=CCRANDOM_0_1();//0-1�������
	const float possibility=0.1f;//0.1�ĸ���
	if(random<possibility){
		mantis->replaceAction(ID_ACTION_ATTACK,ant);
		CCLog("start mantis attack ant");
		return true;
	}
	return false;
}

//һ�����ʽ���������Ϊ
bool gropeByRandom(BaseInsect* insectA,BaseInsect* insectB){
	float random=CCRANDOM_0_1();//0-1�������
	const float possibility=0.1f;//0.1�ĸ���
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
		case TYPE_ATTACKED://������
			{
				AntInsect* insectA=(AntInsect*)sender;
				MantisInsect* insectB=(MantisInsect*)receiver;
				if(insectA->getInsectType()==ID_ANT&&insectB->getInsectType()==ID_MANTIS){
					//�����ϱ���빥������һ����������
					insectA->attacked(insectB->getAttack());//������Ѫһ��
					CC_BREAK_IF(escapeByRandom(insectA));
				}
				break;
			}
		case TYPE_GROPE://��������
			{
				BaseInsect* insectA=(BaseInsect*)sender;
				BaseInsect* insectB=(BaseInsect*)receiver;
				if(insectA->getInsectType()==ID_ANT&&insectB->getInsectType()==ID_MANTIS){
					//��������������룬��һ����������
					CC_BREAK_IF(escapeByRandom(insectA));
				}
				if(insectA->getInsectType()==insectB->getInsectType()){
					//������ͬ���࣬��һ�����ʽ���������
					CC_BREAK_IF(communicateByRandom(insectA,insectB));
				}

				break;
			}
		case TYPE_VIEW_IN://��Ұ����
			{
				Animal* insectSender=(Animal*)sender;
				if(insectSender->getInsectType()==ID_ENEMY){//���ǵ��ˣ�ֻ�ܿ�����֬�򣩣�
					GameResinBallModel* resin=(GameResinBallModel*)message.m_receiver;
					insectSender->replaceActionIfLevelUp(ID_ACTION_FOLLOW);//���˿�ʼ������֬��
					CCLog("start follow resin");
					break;
				}
				else if(insectSender->getAnimalType()==ID_FRIEND_ANIMAL){//���ǳ��ӣ�һ�����ʽ���
					BaseInsect* insectA=(BaseInsect*)insectSender;
					BaseInsect* insectB=(BaseInsect*)receiver;
					if(insectA->getInsectType()==ID_MANTIS&&insectB->getInsectType()==ID_ANT){
						//�������Ұ�������ϣ�һ����������������Ϊ
						CC_BREAK_IF(attackByRandom((MantisInsect*)insectA,(AntInsect*)insectB));
					}
					if(insectA->getInsectType()==insectB->getInsectType()){
						//����ͬ������ӣ���һ������
					}
				}
				break;
			}
		case TYPE_VIEW_OUT://��Ұ��
			{
				Animal* insectSender=(Animal*)sender;
				if(insectSender->getInsectType()==ID_ENEMY){//���ǵ��ˣ�ֻ�ܿ�����֬�򣩣�
					GameResinBallModel* resin=(GameResinBallModel*)message.m_receiver;

					insectSender->replaceActionIfLevelUp(ID_ACTION_NORMAL);
					CCLog("end follow resin");
				}
				break;
			}
		case TYPE_CONTACT://��ײ
		case TYPE_CONTACT_END:
			{
				if(sender->getTag()==TAG_RESINBALL&&receiver->getTag()==TAG_INSECT){
					//��������֬����ײ
					Animal* insect=(Animal*)receiver;
					GameResinBallModel* resin=(GameResinBallModel*)sender;
					switch(insect->getAnimalType()){
					case ID_FRIEND_ANIMAL://���ӱ���
						if(message.m_type==TYPE_CONTACT){//���ǽӴ����룬��ʼ���ǳ���
							resin->beginCover((BaseInsect*)insect);
							resin->reduceBodyVel(0.1f);//��֬���ٶȼ���
							//��������������룬��һ����������
							CC_BREAK_IF(escapeByRandom((BaseInsect*)insect));
							CCLog("begin cover insect");
						}else{//���ǽӴ��뿪����������ǳ���
							resin->endCover();
							CC_BREAK_IF(escapeByRandom((BaseInsect*)insect));
							CCLog("end cover insect");
						}
						break;
					case ID_ENEMY_ANIMAL://��֬�򱻳�
						EnemyInsect* enemyInsect=((EnemyInsect*)insect);
						if(message.m_type==TYPE_CONTACT){//���ǽӴ����룬��ʼ����֬��
							enemyInsect->startAttack();
							CCLog("start attack resin");
						}
						else{//���򣬽�������֬�򣬲���ͣ��N��(��bug����ʱ��ͣ��)							
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
				//��������(�ǵ���)��ײ �����ʽ���ĳһ��Ϊ״̬�����������٣�������
					BaseInsect* insectA=(BaseInsect*)sender;
					BaseInsect* insectB=(BaseInsect*)receiver;
					
					if(insectA->getInsectType()==ID_ANT&&insectB->getInsectType()==ID_ANT){
						//�������ϣ�һ�����ʽ��������Ϊ
						CC_BREAK_IF(followInsectByRandom(insectA,insectB));
					}
					//һ�����ʽ���������Ϊ
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