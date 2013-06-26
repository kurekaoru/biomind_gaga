#include <iostream>
#include <string>

//Data type
const char* DATA_TYPE 		= "SNP_raw";
const char* targetCategory 	= "WELL";
const char* classificationMethod= "boolsimple";

//Switches
bool PLOT		= false;
bool FULLDATA		= true;
bool SHUFFLE		= false;
bool CLEAR_CACHE	= true;
bool HAS_SNPS		= true;
bool DIRECTED		= false;
bool PERMUTE		= false;
bool BATCH		= true;
bool MOSES		= true;

//Parameters
int REPEATS			= 2;
int SAMPLE_SIZE			= 100;
int NUM_TASKS			= 10;
int CLUS_TOPO			= 3;
int numberOfFolds		= 3;
int numberOfSelectedFeatures	= 50;
int populationSize 		= 5;
int numOfSteps			= 5;
float alpha 			= 10;	// Alpha peanalizes bad-performing nodes
float beta			= 10;	// Beta penalizes bad-performing solutions
float decay			= 0.99;
float recombination		= 0.00;
float mutation 			= 1.0 - recombination;
