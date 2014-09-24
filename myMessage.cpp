#include "myMessage.h"

void makeByzantineMessage (ByzantineMessage * &byzmsg, int idcount ){
	byzmsg = (ByzantineMessage *)malloc(sizeof(ByzantineMessage) + sizeof(uint32_t) * idcount);
//	byzmsg->order = 2;
	for(int i = 0; i < idcount; ++i){
		byzmsg->ids[i] = 22;
	}
}

void printByzantineMessageids(ByzantineMessage * byzmsg, int idcount){
//	cout << byzmsg->order  << " this is order "<< endl;
	for(int i = 0; i < idcount; ++i){
		cout << byzmsg->ids[i]<< endl;
	}
}



//void makeAck();