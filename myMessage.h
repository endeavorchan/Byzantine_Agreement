#ifndef MYMESSAGE_H
#define MYMESSAGE_H

#include "general.h"
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
struct ByztMsgNode{
	ByztMsgNode *next;
	bool tag;
	uint32_t type; // Must be equal to 1
	uint32_t nu_ids; // number of ids 
	uint32_t round; // round number
	uint32_t order; // the order (retreat = 0 and attack = 1)
	uint32_t nu_send; // sent id
	uint32_t ids[]; // id's of the senders of the message
};

struct ByztMsgNodeHead{
	uint32_t round;
	uint32_t count;
	ByztMsgNodeHead *down;
	ByztMsgNode* byzmsgnode;
};



class MsgList{
	ByztMsgNodeHead* head;
public:
	MsgList(){
		head = NULL;
	}
	bool equal(ByztMsgNode* a, ByztMsgNode* b){
		if(a == NULL || b == NULL){
			return false;
		}
		else if(a->nu_ids != b->nu_ids)
			return false;
		else{
			bool ret = true;
			for(int i = 0; i < a->nu_ids; ++i){
				if(a->ids[i] != b->ids[i]){
					ret = false;
					break;
				}
			}
			return ret;
		}
		return false;
	}
	bool insert_bymsg(ByzantineMessage *msg){
		int msgnu = (msg->size - sizeof(ByzantineMessage)) / sizeof(uint32_t);
		ByztMsgNode * bmn = (ByztMsgNode*) malloc(sizeof(ByztMsgNode) + sizeof(uint32_t) * msgnu);
		bmn->tag = false;
		bmn->type = msg->type;
		bmn->nu_ids = msgnu;
		bmn->nu_send = 0;
		bmn->round = msg->round;
		bmn->order = msg->order;
		for(int i = 0; i < msgnu; ++i){  
			bmn->ids[i] = bmn->ids[i];
		}
		ByztMsgNodeHead * temp = head;
		while(temp && temp->round < msg->round){
			temp = temp->down;
		}
		if(temp == NULL){
			temp = (ByztMsgNodeHead*) malloc(sizeof(ByztMsgNodeHead));
			temp->round = msg->round;
			temp->down = NULL;
			temp->byzmsgnode = bmn;
			temp->count = 1;
			return true;
		}
		if(temp->round == msg->round){
			ByztMsgNode *tpnode = temp->byzmsgnode;
			while(!equal(tpnode, bmn)){
				if(tpnode->next == NULL){
					tpnode->next = bmn;
					temp->count++;
					return true;
				}
				tpnode = tpnode->next;
			}
		}
		free(bmn);
		return false;
	}
	bool checkmsgallreceive(int round, int num){
		ByztMsgNodeHead* temp = head;
		while(temp->round != round){
			temp = temp->down;
		}
		if(temp->round == round){
			if(temp->count == num)
				return true;
		}
		return false;
	}
	bool checkmsgallsent(int round){
		if(round <= 0)
			return true;
		ByztMsgNodeHead* temp = head;
		while(temp->round != round){
			temp = temp->down;
		}
		if(temp->round == round){
			ByztMsgNode *tpnode = temp->byzmsgnode;
			while(tpnode && tpnode->tag == true){
				tpnode = tpnode->next;
			}
			if(tpnode == NULL){
				return true;
			}
		}
		return false;
	}

	ByztMsgNode * get_bymsg(int round){
		if(checkmsgallsent(round)){
			return NULL;
		}
		ByztMsgNodeHead* temp = head;
		while(temp->round != round){
			temp = temp->down;
		}
		if(temp->round == round){
			ByztMsgNode *tpnode = temp->byzmsgnode;
			while(tpnode->tag == true){
				tpnode = tpnode->next;
			}
			// tpnode->tag must be false;
			return tpnode;
		}
		return NULL;
	}

	bool set_bymsg(int round){
		ByztMsgNode * temp= get_bymsg(round);
		temp->tag = true;
		return true;
	}
	
	
};
typedef map <int, string>  IPList;
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
		int i = 0;
		for(i = 0; i < idcount-1; ++i);
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
	void sendByzantineMessage(int type, void* p, int dest_id);
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

#endif

