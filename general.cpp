#include "general.h"
#include <iostream>

int processname[4] = {0, 1, 2, 3};

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
/*
	ByzantineMessage * bzmsg;
	int count = 4;
	makeByzantineMessage(bzmsg, count);
	printByzantineMessageids(bzmsg, count);
	sendByzantineMessage();
	recvByzantineMessage();
*/
	
	Messages msg;
	//msg.makeByzantineMessage(4);
	//msg.printByzantineMessageids(4);
	msg.mainLoop();
	return 0;
}
