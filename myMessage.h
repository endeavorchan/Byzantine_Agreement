#ifndef MYMESSAGE_H
#define MYMESSAGE_H
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

	ByzantineMessage* byzmsg;
	Ack* ack;
	int sockfd;
	uint16_t my_port;
	uint32_t my_ip;
	uint32_t my_id;
public:
	Messages();
	Ack* makeAck(int round){
		ack = (Ack*)malloc(sizeof(Ack));
		ack->type = ACK;
		ack->size = sizeof(Ack);
		ack->round = round;
	}
	ByzantineMessage * makeByzantineMessage(int order, int round, int idcount){
		byzmsg = (ByzantineMessage *)malloc(sizeof(ByzantineMessage) + sizeof(uint32_t) * idcount);
		byzmsg->type = BYZANTINE;
		byzmsg->size = sizeof(ByzantineMessage) + sizeof(uint32_t) * idcount;
		byzmsg->round = round;
		byzmsg->order = order;
		//	byzmsg->order = 2;
		for(int i = 0; i < idcount; ++i){
			byzmsg->ids[i] = 2;
		}
		return byzmsg;
	}
	void deAllocatemessage(){
		free(byzmsg);
	}
	void deAllocateack(){
		free(ack);
	}
	void printByzantineMessageids(int idcount){
		//	cout << byzmsg->order  << " this is order "<< endl;
		for(int i = 0; i < idcount; ++i){
			cout << byzmsg->ids[i]<< endl;
		}
	}
	void sendByzantineMessage();
	void recvByzantineMessage();
	void *get_in_addr(struct sockaddr *sa);
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


#endif
