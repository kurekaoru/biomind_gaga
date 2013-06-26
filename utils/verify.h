using namespace std;

#include "../utils/IO.h"
#include "../dbi/sql.h"
//#include "../const/const.h"

void verify() {
	cout << " VERIFYING PREREQUISITES\n";
	cout << hline;
	MYSQL mysql;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql,HOST,USER,PASS,"mysql",0,NULL,0)) {
		fprintf(stderr, " OPEN MySQL CONNECTION\t\t\t\t\t\t  [\033[21;31mFAILED\033[21;32m] \n ERROR: %s\n", mysql_error(&mysql));
		exit(0);
	} else {
		cout << " OPEN MySQL CONNECTION\t\t\t\t\t\t [SUCCESS]\n";
		ifstream myfile;
		myfile.open(INFILE,ios::in);
		if (!myfile.is_open()){
			cout << " ATTEMPT TO READ INPUT FILE\t\t\t\t\t  [\033[21;31mFAILED\033[21;32m]\n";
			cout << " VERIFICATION\t\t\t\t\t\t\t  [\033[21;31mFAILED\033[21;32m]\n";
			cout << " \t\t\t\t\t\t\t\t [\033[21;31mABORTED\033[21;32m]\n";
			exit(0);
		}
	}
	cout << " ATTEMPT TO READ INPUT FILE\t\t\t\t\t [SUCCESS]\n";
	cout << " VERIFICATION\t\t\t\t\t\t\t [SUCCESS]\n\n";
}
