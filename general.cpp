#include "general.h"

int main(){
	MSGList Msglist;
	Msglist["0"] = 1;
	Msglist["10"] = 1;
	Msglist["320"] = 0;
	
	
	
	Msglist["230"] = 1;
	Msglist["30"] = 1;
	Msglist["20"] = 0;
	MSGList::iterator pos;
	for(pos = Msglist.begin(); pos != Msglist.end(); ++pos){
		cout << "key: " << pos->first << " Value: " << pos->second; 
		cout << endl;
	}

	
	return 0;
}