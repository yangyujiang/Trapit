#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "cocos2d.h"


USING_NS_CC;
using std::vector;  

class Message{
public:
	void* m_sender;//主动者
	unsigned int m_type;//动作类型
	void* m_receiver;//被动接受者

public:
	void set(void* sender,unsigned int type,void* receiver);
};

class MessageVector{
public:
	~MessageVector();
	static vector<Message> sharedMessages();
	static void addMessage(void* sender,unsigned int type,void* receiver);
	static void handleMessage();
};


class MessageView{
public:
	~MessageView();
	static vector<Message> sharedMessages();
	static void addMessage(void* sender,unsigned int type,void* receiver);
	static void handleMessage();
};




#endif //__MESSAGE_H__