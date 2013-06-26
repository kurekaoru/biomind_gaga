#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <boost/regex.hpp>

using namespace std;

int getdir (string dir, vector<string> &files) {

	FILE *in;
	char buff[512];
	struct dirent *dirp;

	if(!(in = popen(("ls "+string(dir)+"/t*.tab").c_str(),"r")))
		return 1;

	while(fgets(buff, sizeof(buff), in)!=NULL)
		files.push_back(buff);

	pclose(in);
	return 0;
}


const char* shexec(const char* cmd){
	FILE *lsofFile_p = popen(cmd, "r");
	if (!lsofFile_p) 
		return "";
	char buffer[100000];
	const char* out = fgets(buffer, sizeof(buffer), lsofFile_p);
	pclose(lsofFile_p);
	return out;
}
