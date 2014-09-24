#include "general.h"
#include <iostream>
#include <pthread.h>
#define NUM_THREADS 2
int processname[4] = {0, 1, 2, 3};
void* recev(void *args){
	while(1){
		recvByzantineMessage();	
	}
}

void* sendm(void *args){
	while(1){
		sendByzantineMessage();
	}
}
int main(){

	int process = 0;


/* Read ips from a external file 
	Servers servers;
	string file_name = "ip.conf";
	ifstream infile(file_name.c_str(),ios::in);
	string textline;
	while(getline(infile,textline,'\n')){    
		servers.push_back(textline);
	}  
	infile.close();
	Servers::iterator pos;
	for(pos = servers.begin(); pos != servers.end(); ++pos){
		cout << *pos << endl;
	}

	uint16_t myport = 6440;
	uint32_t server_ip;
*/

	ByzantineMessage * bzmsg;
	int count = 4;
	makeByzantineMessage(bzmsg, count);
	printByzantineMessageids(bzmsg, count);
	
	
	
	
	
	pthread_t tids[NUM_THREADS];
	int ret = pthread_create(&tids[0], NULL, recev, NULL);
	if(ret !=0 ){
		cout << "pthread creat error" << endl;	
	}
	int ret2 = pthread_create(&tids[1], NULL, sendm, NULL);
	if(ret2 !=0 ){
		cout << "pthread creat error" << endl;	
	}	

	return 0;
}
