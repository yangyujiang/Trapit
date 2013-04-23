#include "Message.h"
#include "Constant.h"
#include "GameInsectModel.h"

USING_NS_CC;
using std::vector;
//
//Message.cpp
//

static vector<Message> messages;

void Message::set(void* sender,unsigned int type,void* receiver){
	m_sender=sender;
	m_type=type;
	m_receiver=receiver;
}

vector<Message> MessageVector::sharedMessages(){
	return messages;
}
MessageVector::~MessageVector(){
	messages.clear();
}

void MessageVector::addMessage(void* sender,unsigned int type,void* receiver){
	Message message;
	message.set(sender,type,receiver);
	messages.push_back(message);
}
void MessageVector::deleteMessage(Message message){
	vector<Message>::iterator it = messages.begin(); 
	while (it != messages.end()) { 
		Message insect=(Message)*it; //CCLog("%d",insect->getAlive());       
	/*	if (!insect->getAlive()) { 
			insect->clean();
			it = _insects.erase(it);    
			CCLog("after delete insect No.:",_insects.size());
			amberCount++;
		} else {	
			//insect->handleContact();
			it++;    
		}*/
	}
}

void MessageVector::handleMessage(){
	vector<Message>::iterator it = messages.begin(); 
	while (it != messages.end()) { 
		Message message=(Message)*it;        
		if(message.m_type==TYPE_VIEW) { 
			((MantisInsect*)message.m_sender)->replaceMovement(ID_FOLLOW_MOVE,(CCNode*)message.m_receiver);
			it=messages.erase(it);
			CCLog("start follow");
			break;
		} 
		it++;
	}
}