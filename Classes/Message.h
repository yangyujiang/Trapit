#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "cocos2d.h"


USING_NS_CC;
using std::vector;  

class Message{
public:
	void* m_sender;//������
	unsigned int m_type;//��������
	void* m_receiver;//����������

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