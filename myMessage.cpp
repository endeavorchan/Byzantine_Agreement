#include "general.h"
#define MAXBUFLEN 2048
#define MYPORT "4950"
#define F 1
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
	this->round = 1;
	this->f = 1;
	// initialize iplist
	string file_name = "ip.conf";
	ifstream infile(file_name.c_str(),ios::in);
	string textline;
	int index = 0;
	while(getline(infile,textline,'\n')){    
		iplist[index++] = textline;
	}  
	infile.close();
	nodeid = 0;
	msglist.fortest();
	//printmsgtable();
/*	
	IPList::iterator pos;

	for(pos = iplist.begin(); pos != iplist.end(); ++pos){
		cout << pos->first << " " << pos->second << endl;
	}

	pos = iplist.find(2);
	cout << "here" << pos->second << endl;
*/
	// get a list of all the other nodes and the corresponding address.
   	
}

void Messages::mainLoop(){
	fd_set set;
  
    while (round <= F+1) {    /*need to continue to next round f*/  
	cout << "Iterate" << endl;
	struct timeval timeout={1,0}; 
	FD_ZERO(&set);
	FD_SET(sockfd, &set);

	select(sockfd+1, &set, NULL, NULL, &timeout);

	if (FD_ISSET(sockfd, &set)) {   
	    cout << "receive message" << endl;
	    void *p = NULL;
	    int type = recvByzantineMessage(p);
	    /*insert the messages received in this round(discard messages not for this round), 
				must be a way to sort the messages */
		/*Send ACK*/
	    if(type = BYZANTINE){
			ByzantineMessage* rc_byzmsg = (ByzantineMessage*)p;
			cout << rc_byzmsg->round << "my round is " << round << endl;
			if(rc_byzmsg->round == round){
				//cout << "received a byzantine msg in main loop\n" <<endl;  
				ByztMsgNode* byznode = msglist.insert_bymsg(rc_byzmsg);
				if(byznode != NULL){
					makeAck(rc_byzmsg->round);
					sendByzantineMessage(ACK, (void*)ack, byznode->ids[byznode->nu_ids-1]);
					//free(ack);
				}
  		   		
  		   	}
	
		}
		else if(type = ACK){
			Ack* rc_ack = (Ack*)p;
			//int round = ntohl(rc_ack->round);
			cout << "it's the ack from the " << round << "round" << endl;
			if(rc_ack->size == sizeof(Ack)){
				//set_bymsg(round);
				/*tell send this one has be received*/
				ByztMsgNode * byztnode = msglist.get_bymsg(f-1);
				byztnode->nu_send++;
				if(byztnode->nu_send == (byztnode->nu_ids - 1)){
					byztnode->tag = true;
				}
			}
		}
		else{
			cout << "receiving error"<< endl;
		}
	    
	}
	//cout << "here it si" <<endl;
	/*forword (to all others who should receive) messages in the last round received. if not contine*/
	if(!msglist.checkmsgallsent(round - 1)){
		ByztMsgNode * byztnode = msglist.get_bymsg(round-1);
		if(byztnode->nu_send >= byztnode->needsendcount) cout << "nu_send > needsendcount" << endl;
		int dest_id = byztnode->needsendqueue[byztnode->nu_send];
		makeByzantineMessage(byztnode,f,nodeid);     //(byztnode->order, f, byztnode->nu_ids+1, nodeid);
		sendByzantineMessage(BYZANTINE, (void*)byzmsg, dest_id);   
		//free(byzmsg);
	}
	/*all messages in the last round had be succedly forwarded and
		 all messages should be received in this round has received enter next round*/
	// if so{ f++; }
	if(msglist.checkmsgallreceive(round, 3-round/**/) && msglist.checkmsgallsent(round-1)){  //suppose its f+1
		round++;
	}
    }
}


void Messages::sendByzantineMessage(int type, void* p, int dest_id){

	struct addrinfo hints, *servinfo;
	int numbytes;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	IPList::iterator pos = iplist.find(dest_id);
	cout << "the ip of the dest is "<< pos->second << endl;
	getaddrinfo(pos->second.c_str(), "6441", &hints, &servinfo); //shoud send to the dest_id
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
		freebyz(byzmsg);
	}
	else if(type == ACK){
		Ack* ack = (Ack*)p;
		char *mp = (char*)p;
		cout << "ack size "<< ntohl(ack->size) << endl;
		for(int i = 0; i < ntohl(ack->size); ++i){
			buf[i] = mp[i];
		}
		if ((numbytes = sendto(sockfd, buf, ntohl(ack->size), 0, 
					servinfo->ai_addr, servinfo->ai_addrlen)) == -1) {
			perror("talker: sendto");
			exit(1);
		}
		freeack(ack);
	}
	else{
		cout << "message content error\n";
	}
	
	freeaddrinfo(servinfo);
	printf("talker: sent %d bytes to %s\n", numbytes, pos->second.c_str());
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
		byzmsg->type = ntohl(byzmsg->type);
		byzmsg->size = ntohl(byzmsg->size);
		byzmsg->round = ntohl(byzmsg->round);
		//cout << "round is " << byzmsg->round << endl;
		byzmsg->order = ntohl(byzmsg->order);
		//cout << "order is " << byzmsg->order << endl;
		int idcounts = (byzmsg->size - sizeof(ByzantineMessage)) / sizeof(uint32_t);
		cout << "idcounts is " << idcounts << endl;
		for(int i = 0; i < idcounts; ++i){
			byzmsg->ids[i] = ntohl(byzmsg->ids[i]);
		}
  		cout << "received byzantine msg from " << byzmsg->ids[idcounts-1] <<endl;
		//printByzantineMessageids(nids);
		pmsg = (void*)buf; //(ByzantineMessage*)pmsg = byzmsg;
		return BYZANTINE;
		
	}
	else if(ntohl(*ptype) == ACK){
		Ack* ack = (Ack*)buf;
		ack->type = ntohl(ack->type);
		ack->size = ntohl(ack->size);
		ack->round = ntohl(ack->round);
		//uint32_t round = ntohl(ack->round);
		//cout << "it's the ack from the " << round << "round" << endl;
		pmsg = (void*)buf; //(Ack*)pmsg = ack;
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

