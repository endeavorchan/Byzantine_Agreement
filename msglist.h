#ifndef MSGLIST_H
#define MSGLIST_H
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

using namespace std;
class mysort;

typedef map <string, uint32_t, mysort>  MSGList;

class mysort{
public:
	bool operator()(const string &k1, const string &k2) const {  
    if (atoi(k1.c_str()) < atoi(k2.c_str()))  
      return true;  
    else  
      return false;  
  }  
};

#endif



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
