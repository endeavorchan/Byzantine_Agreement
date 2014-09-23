#ifndef MSGLIST_H
#define MSGLIST_H
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