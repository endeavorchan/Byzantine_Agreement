#ifndef MSGLIST_H
#define MSGLIST_H
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <map>
#include <iostream>

typedef struct {
	bool tag;
	uint32_t size;
	uint32_t round;
	uint32_t order;
}ByztMsgNode;

using namespace std;
class mysort;


typedef map <int, string>  IPList;

/*
class mysort{
public:
	bool operator()(const string &k1, const string &k2) const {  
    if (atoi(k1.c_str()) < atoi(k2.c_str()))  
      return true;  
    else  
      return false;  
  }  
};
*/


typedef struct {
	uint32_t round;
	uint32_t count;
	ByztMsgNodeHead *down;
	ByztMsgNode* byzmsgnode;
}ByztMsgNodeHead;

typedef struct {
	ByztMsgNode *next;
	bool tag;
	uint32_t type; // Must be equal to 1
	uint32_t nu_ids; // number of ids 
	uint32_t round; // round number
	uint32_t order; // the order (retreat = 0 and attack = 1)
	uint32_t ids[]; // id's of the senders of the message
}ByztMsgNode;

Class MsgList{
	ByztMsgNodeHead* head;
public:
	Msglist(){
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
		int msgnu = (byzmsg->size - sizeof(ByzantineMessage)) / sizeof(uint32_t);
		ByztMsgNode * bmn = (ByztMsgNode*) malloc(sizeof(ByztMsgNode) + sizeof(uint32_t) * msgnu);
		bmn->tag = false;
		bmn->type = msg->type;
		bmn->nu_ids = msgnu;
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
			while(!equal(tpnode, bnm)){
				if(tpnode->next == NULL){
					tpnode->next = bnm;
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
			if(temp->cound == num)
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
		ByzantineMessage * temp= get_bymsg(round);
		temp->tag = true;
		return true;
	}
	
	
};


/*
	MSGList Msglist;

	Msglist["0"] = 1;
	Msglist["10"] = 1;
	Msglist["320"] = 0;
	Msglist["230"] = 1;
	Msglist["30"] = 1;
	Msglist["20"] = 0;
	//Msglist["230"] = 0;

	MSGList::iterator pos;
	for(pos = Msglist.begin(); pos != Msglist.end(); ++pos){
		cout << "key: " << pos->first << " Value: " << pos->second; 
		cout << endl;
	}

	*/
