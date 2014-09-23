#include "general.h"


int main(){

	Servers servers;
	string file_name = "ip.conf";
	ifstream infile(file_name.c_str(),ios::in);
 
	string textline;
 
	while(getline(infile,textline,'\n')){    //遇到换行结束这一行的读取
		servers.push_back(textline);
	}  
	infile.close();
	Servers::iterator pos;
	for(pos = servers.begin(); pos != servers.end(); ++pos){
		cout << *pos << endl;
	}

	return 0;
}