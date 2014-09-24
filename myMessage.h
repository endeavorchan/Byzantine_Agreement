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
private:
	ByzantineMessage* byzmsg;
	Ack* ack;
	int sockfd;
	uint16_t my_port;
	uint32_t my_ip;
	uint32_t my_id;
	void *get_in_addr(struct sockaddr *sa)
	{
		if (sa->sa_family == AF_INET) {
			return &(((struct sockaddr_in*)sa)->sin_addr);
		}

		return &(((struct sockaddr_in6*)sa)->sin6_addr);
	}

public:
	Messages();
	Ack* makeAck(int round){
		ack = (Ack*)malloc(sizeof(Ack));
		ack->type = htonl(ACK);
		ack->size = htonl(sizeof(Ack));
		ack->round = htonl(round);
	}
	ByzantineMessage * makeByzantineMessage(int order, int round, int idcount){
		byzmsg = (ByzantineMessage *)malloc(sizeof(ByzantineMessage) + sizeof(uint32_t) * idcount);
		byzmsg->type = htonl(BYZANTINE);
		byzmsg->size = htonl(sizeof(ByzantineMessage) + sizeof(uint32_t) * idcount);
		byzmsg->round = htonl(round);
		byzmsg->order = htonl(order);
		//	byzmsg->order = 2;
		for(int i = 0; i < idcount; ++i){
			byzmsg->ids[i] = htonl(2);
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
			cout << ntohl(byzmsg->ids[i])<< endl;
		}
	}
	void sendByzantineMessage(int type, void* p);
	void recvByzantineMessage();
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
