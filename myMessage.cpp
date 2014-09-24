#include "myMessage.h"
#define MAXBUFLEN 2048
#define MYPORT "4950"

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

void sendByzantineMessage (ByzantineMessage *msg, int idcount){

	//struct  sockaddr_in saddr;
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo("172.16.238.161", MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		//return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to bind socket\n");
		//return 2;
	}

	if ((numbytes = sendto(sockfd, "hello cc", strlen("hello cc"), 0,
			 p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}

	freeaddrinfo(servinfo);

	printf("talker: sent %d bytes to %s\n", numbytes, "172.16.238.161");
	close(sockfd);

//	return 0;
	
}

void recvByzantineMessage(){

}


void makeAck(){

}

void printAck(){

}

void sendAck(){

}

void recvAck(){

}
