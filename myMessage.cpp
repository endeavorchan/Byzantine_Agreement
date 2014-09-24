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


}

void Messages::mainLoop(){
	fd_set set;
  
    while (1) {
	//cout << "Iterate" << endl;
	struct timeval timeout={1,0}; 
	FD_SET(sockfd, &set);

	select(sockfd+1, &set, NULL, NULL, &timeout);

	if (FD_ISSET(sockfd, &set)) {
	    //cout << "receive message\n";
	    recvByzantineMessage();
	}
	sendByzantineMessage();
    }
}


void Messages::sendByzantineMessage(){

	struct addrinfo hints, *servinfo;
	int numbytes;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	getaddrinfo("172.16.238.158", "6441", &hints, &servinfo);
	if ((numbytes = sendto(sockfd, "hello cc", strlen("hello cc"), 0,
			servinfo->ai_addr, servinfo->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}
	freeaddrinfo(servinfo);
	printf("talker: sent %d bytes to %s\n", numbytes, "172.16.238.158");
	//close(sockfd);

}



void *Messages::get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


void Messages::recvByzantineMessage(){
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	char s[INET6_ADDRSTRLEN];
	socklen_t addr_len;
	int numbytes;
	printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	printf("listener: got packet from %s\n",
		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s));
	printf("listener: packet is %d bytes long\n", numbytes);
	buf[numbytes] = '\0';
	printf("listener: packet contains \"%s\"\n", buf);

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

