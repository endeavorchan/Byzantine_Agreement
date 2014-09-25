#include "myMessage.h"
#define MAXBUFLEN 2048
#define MYPORT "4950"


Messages::Messages(){
	struct addrinfo hints, *servinfo, *p;
	int rv;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, "6441", &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	}
	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
	}

	freeaddrinfo(servinfo);

   	/* make message */
	byzmsg = makeByzantineMessage(1, 0, 4);
	//cout << "sec" <<endl;
}

void Messages::mainLoop(){
	fd_set set;
  
    while (1) {
	//cout << "Iterate" << endl;
	struct timeval timeout={10,0}; 
	FD_SET(sockfd, &set);

	select(sockfd+1, &set, NULL, NULL, &timeout);

	if (FD_ISSET(sockfd, &set)) {
	    cout << "receive message\n";
	    void *p = NULL;
	    int type = recvByzantineMessage(p);
	    	if(type = BYZANTINE){
			ByzantineMessage* byzmsg = (ByzantineMessage*)p;
		    	int nids = (ntohl(byzmsg->size) - sizeof(ByzantineMessage)) / sizeof(uint32_t);
  		    	cout << "receiveing in main loop\n" <<endl;  
		    	printByzantineMessageids(nids);  
		}
		else if(type = ACK){
			Ack* ackmsg = (Ack*)p;
			int round = ntohl(ackmsg->round);
			cout << "it's the ack from the " << round << "round" << endl;
		}
		else{
			cout << "receiving error"<< endl;
		}
	    
	}
	cout << "1 " << byzmsg << endl;
	sendByzantineMessage(BYZANTINE, (void*)byzmsg);
    }
}


void Messages::sendByzantineMessage(int type, void* p){

	struct addrinfo hints, *servinfo;
	int numbytes;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	getaddrinfo("172.16.238.160", "6441", &hints, &servinfo);
        char buf[MAXBUFLEN];
	
	if(type == BYZANTINE){
		ByzantineMessage* byzmsg = (ByzantineMessage*)p;
		char *mp = (char*) p;
		//cout << "2 " << mp << endl;


		for(int i = 0; i < ntohl(byzmsg->size); ++i){
			buf[i] = mp[i];
		}
		cout << "size :"<< ntohl(byzmsg->size ) << endl;
		if ((numbytes = sendto(sockfd, buf, ntohl(byzmsg->size), 0, 
					servinfo->ai_addr, servinfo->ai_addrlen)) == -1) {
			perror("talker: sendto");
			exit(1);
		}
	}
	else if(type == ACK){
		Ack* ack = (Ack*)p;
		char *mp = (char*)p;
		for(int i = 0; i < ack->size; ++i){
			buf[i] = mp[i];
		}
		if ((numbytes = sendto(sockfd, buf, ack->size, 0, 
					servinfo->ai_addr, servinfo->ai_addrlen)) == -1) {
			perror("talker: sendto");
			exit(1);
		}
	}
	else{
		cout << "message content error\n";
	}

	freeaddrinfo(servinfo);
	printf("talker: sent %d bytes to %s\n", numbytes, "172.16.238.160");
	//close(sockfd);

}

int Messages::recvByzantineMessage(void *&pmsg){
	struct sockaddr_storage their_addr;
	static char buf[MAXBUFLEN];
	char s[INET6_ADDRSTRLEN];
	socklen_t addr_len;
	int numbytes;
	printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof(their_addr);
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		return -1;
	}

	/* Collect message */
	uint32_t *ptype = (uint32_t*)buf;
	if(ntohl(*ptype) == BYZANTINE){
		ByzantineMessage* byzmsg = (ByzantineMessage*)buf;
		//int nids = (ntohl(byzmsg->size) - sizeof(ByzantineMessage)) / sizeof(uint32_t);
  		//cout << "receiveing\n" <<endl;
		//printByzantineMessageids(nids);
		pmsg = (void*)byzmsg; //(ByzantineMessage*)pmsg = byzmsg;
		return BYZANTINE;
		
	}
	else if(ntohl(*ptype) == ACK){
		Ack* ack = (Ack*)buf;
		//uint32_t round = ntohl(ack->round);
		//cout << "it's the ack from the " << round << "round" << endl;
		pmsg = (void*)ack; //(Ack*)pmsg = ack;
		return ACK;
	}
	else{
		cout << "received message error" << endl;
		return -1;
	}
	return -1;
/*
	printf("listener: got packet from %s\n",
		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s));
	printf("listener: packet is %d bytes long\n", numbytes);
	buf[numbytes] = '\0';
*/
	//printf("listener: packet contains \"%s\"\n", buf);

}

/*
void makeAck(){

}

void printAck(){

}

void sendAck(){

}

void recvAck(){

}
*/

