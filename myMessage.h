#ifndef MYMESSAGE_H
#define MYMESSAGE_H
#endif
#include "msglist.h"
#include <iostream>
#define PORT 10088
using namespace std;

enum msg_type {BYZANTINE = 1, ACK = 2};
enum orders {RETREAT = 0, ATTACK = 1};

/* The structure of the Byzantine message */
typedef struct {
	uint32_t type; // Must be equal to 1
	uint32_t size; // size of message in bytes
	uint32_t round; // round number
	uint32_t order; // the order (retreat = 0 and attack = 1)
	uint32_t ids[]; // id's of the senders of the message
} ByzantineMessage;

/* The sturcture of the ACK */
typedef struct {
	uint32_t type; // Must be equal to 2
	uint32_t size; // size of message in bytes
	uint32_t round; // round number
} Ack;

/*
void makeByzantineMessage (ByzantineMessage * &byzmsg, int idcount);
void printByzantineMessageids(ByzantineMessage * byzmsg, int idcount);
void sendByzantineMessage ();
void *get_in_addr(struct sockaddr *sa);
void recvByzantineMessage();
void makeAck();
*/

class Messages{
private:
	ByzantineMessage* byzmsg;
	Ack* ack;
	MsgList msglist;
	int sockfd;
	uint16_t my_port;
	uint32_t my_ip;
	uint32_t my_id;
	uint32_t nodeid;
	uint32_t round;
	uint32_t f;
	IPList iplist;


	void *get_in_addr(struct sockaddr *sa)
	{
		if (sa->sa_family == AF_INET) {
			return &(((struct sockaddr_in*)sa)->sin_addr);
		}

		return &(((struct sockaddr_in6*)sa)->sin6_addr);
	}
	
	ByzantineMessage* allocatebyz(int idcount){
		ByzantineMessage* p = (ByzantineMessage *)malloc(sizeof(ByzantineMessage) + sizeof(uint32_t) * idcount);
		return p;
	}
	Ack* allocateAck(){
		Ack* p = (Ack*)malloc(sizeof(Ack));
		return p;
	}

public:
	Messages();
	void makeAck(int round){
		ack = (Ack*)malloc(sizeof(Ack));
		ack->type = htonl(ACK);
		ack->size = htonl(sizeof(Ack));
		ack->round = htonl(round);
	}
	void makeByzantineMessage(int order, int round, int idcount, int id){
		byzmsg = allocatebyz(idcount);
		byzmsg->type = htonl(BYZANTINE);
		byzmsg->size = htonl(sizeof(ByzantineMessage) + sizeof(uint32_t) * idcount);
		byzmsg->round = htonl(round);
		byzmsg->order = htonl(order);
		//	byzmsg->order = 2;
		for(int i = 0; i < idcount-1; ++i);
		byzmsg->ids[i] = htonl(id);  // may change
	}
	
	
	void printByzantineMessageids(int idcount, ByzantineMessage *msg){
		//	cout << byzmsg->order  << " this is order "<< endl;
		for(int i = 0; i < idcount; ++i){
			cout << msg->ids[i]<< endl;
		}
	}
	
	void freeack(Ack* p){
		free(p);
	}

	void freebyz(ByzantineMessage *pbyzmsg){
		free(pbyzmsg);
	}
	void sendByzantineMessage(int type, void* p);
	int recvByzantineMessage(void* &);
	void mainLoop();

};


/*
class myMessage{
	uint32_t type;
	ByzantineMessage bzmsg;
	Ack ack;

	myMessage(){

	}
	myMessage(ByzantineMessage* msg){
		type = BYZANTINE;
		bzmsg->size = msg->size;

	}

};
*/



