// void function example
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "../params/pars.h"
#include "../params/dirs.h"
#include "../utils/utils.h"
#include "../const/const.h"
using namespace std;

static const char* INFILE;

void progressBar(unsigned int maxLen, float progress){
	int pLen = maxLen*progress;
	cout << " [" << string(pLen,'=')<< string(tLen-pLen,' ') << "] "<< fixed << setprecision(2) << progress*100 << "% \r" << flush;
	if (progress == 1.0)
		cout << fixed << setprecision(8) << " [" << string(tLen,'=') << "\t\t [SUCCESS]\n";
}

void transform(const char* infile, string suuid) {
		cout << " USING PARTIAL DATA | ";
		system(("head -qn 2 " + string(infile) + " > temp/temp_"+suuid+".tab").c_str());
		if (!SHUFFLE) {
			cout << "SHUFFLE OFF\n";
			system(("head -n "+i2s(SAMPLE_SIZE)+" "+string(infile)+" >> temp/temp_"+suuid+".tab").c_str());
		} else {
			cout << "SHUFFLE ON\n";
			system(("tail -n +3 "+string(infile)+" | shuf -n "+i2s(SAMPLE_SIZE)+" >> temp/temp_"+suuid+".tab").c_str());
		}
		INFILE = ("temp/temp"+suuid+".tab").c_str();
		cout << " GENERATE INPUT FILE \t"+string(INFILE)+"\t\t\t\t [SUCCESS]\n";
		//INFILE = infile;
}

void transformSNP(const char* infile) {
	//open infile
	ofstream datafile;
	datafile.open(infile, ios::in);
	cout << datafile << endl;
}

void fold(const char* infile) {
		//Fold and convert to SNP pseudovalues
		//system(("java -Xmx2048m -cp ./ob/:openbiomind.jar task.FoldSelectSNPs -d "+string(infile)+" -o "+string(INPUT_DIR)+" -targetCategory "+string(targetCategory)+" -numberOfFolds "+i2s(numberOfFolds)+" -numberOfSelectedFeatures "+i2s(numberOfSelectedFeatures)+ " > "+LOG_DIR+"/FoldSelectSNPs.log").c_str());
		system(("python pyutils/foldBinarizeSNP.py " + string(infile) + " " + string(INPUT_DIR) + " " + string(targetCategory) + " " + i2s(numberOfFolds)).c_str());
}

void prepare(const char* infile, string suuid) {
	cout << " PREPROCESSING DATA \n";
	cout << hline;
	system(("rm -f temp/temp_"+suuid+".tab").c_str());
	INFILE = infile;
	//clearCache();
	if (!FULLDATA) {
		transform(infile, suuid);
	} else cout << " USING FULL DATA FROM INFILE: "+string(INFILE)+"\n";

	//Permutate labels if flag is on
	if (PERMUTE) {
		cout << " PERMUTE CASE DESIGNATION\t\t\t\t\t [SUCCESS]\n";
		//system(("bash/TP.sh " + string(infile)+" "+suuid).c_str());
		system(("bash/TP.sh " + string(infile)+" "+suuid+" "+"temp/temp_"+suuid+".tab").c_str());
		INFILE = ("temp/temp_"+suuid+".tab").c_str();
	}

	ifstream myfile;
	myfile.open(INFILE,ios::in);
	char output[512];
	if (myfile.is_open()) {
		cout << " READ INPUT FILE \t" << INFILE << "\t\t\t\t [SUCCESS]\n";
	} else {
		cout << " READ INPUT FILE \t" << INFILE << "\t\t\t\t  [\033[21;31mFAILED\033[21;32m]\n";
		exit(0);
	}
	fold(INFILE);
	myfile.close();
}
