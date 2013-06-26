//LIBARIES
#include <cstdlib>
#include <map>
#include <stdexcept>
#include <string>
#include <stdio.h>
#include <iomanip>
#include <algorithm>

//LOCAL PACKAGES
#include "dataHandler/loadData.h"
#include "dataHandler/interpreter.h"
#include "utils/verify.h"
#include "utils/combination.h"
#include "utils/batch.h"
#include "boost/foreach.hpp"
#include <boost/algorithm/string.hpp>


//uuid
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace std;

//FIELD VAR
int bid, sid;
string SUUID, BUUID;
int totalSteps = (numOfSteps+populationSize)*numberOfFolds;
int doneSteps = 0;

void updateDir() {
	LOG_DIR			= "sessions/"+BUUID+"/"+SUUID+"/"+LOG_DIR;
	INPUT_DIR		= "sessions/"+BUUID+"/"+SUUID+"/"+INPUT_DIR;
	GENE_TUPLET_OUTPUT_DIR	= "sessions/"+BUUID+"/"+SUUID+"/"+GENE_TUPLET_OUTPUT_DIR;
	TASK_OUTPUT_DIR		= "sessions/"+BUUID+"/"+SUUID+"/"+TASK_OUTPUT_DIR;
}

void clearCache() {
	updateDir();
	system(("mkdir -p "+LOG_DIR).c_str());
	system(("mkdir -p "+INPUT_DIR).c_str());
	system(("mkdir -p "+GENE_TUPLET_OUTPUT_DIR).c_str());
	system(("mkdir -p "+INPUT_DIR).c_str());
}

void dt(float &T, float dt){
	// DECREASE TEMPREATURE
	T *= dt;
}

void rejectAction(){
	// GIVE UP ON SOLUTION
}

void acceptAction(int solid, vector<int> cluster, float score, float tscore, int fold, int sid){
	updateSolution(solid, cluster, score, tscore, sid);
	system(("rm -Rf " + string(TASK_OUTPUT_DIR) +"/history/fold_"+i2s(fold)+"/sol_"+i2s(solid-getMinFoldId(fold,sid))).c_str());
	system(("cp -R " + string(TASK_OUTPUT_DIR) +"/current/fold_"+i2s(fold)+"/sol_"+i2s(solid-getMinFoldId(fold,sid))+" "+TASK_OUTPUT_DIR +"/history/fold_"+i2s(fold)+"/sol_"+i2s(solid-getMinFoldId(fold,sid))).c_str());
}

bool accept(float ds, float temp){

	if (ds >= 0) {
		// ACCEPT SOLUTION
		return true;
	} else {
		float prob = exp(ds/temp);
		if (rDouble(0,1) <= prob) {
			// ACCEPT SOLUTION
			return true;
		} else {
			// REJECT SOLUTION
			return false;
		}
	}
}

//Stop criterion
bool stop(int& step, float &temp, float oldScore, float newScore, int solid, vector<int> cluster, float score, float tscore, int fold, int sid){
	int S = step;
	if (accept(newScore-oldScore, temp)){
		step = 0;
		dt(temp, decay);
		acceptAction(solid, cluster, score, tscore, fold, sid);
		return true;
	} else if (S > 20){
		step = 0;
		rejectAction();
		return true;
	} else {
		step++;
		return false;
	}
}

void swaptGene(vector<int> &Cluster, int before, int after){
	for(unsigned int i=0; i<Cluster.size(); i++){
		if (Cluster[i] == before)
			Cluster[i] = after;
	}
	if (!DIRECTED) std::sort(Cluster.begin(), Cluster.end());
}

void pickGenes(vector<vector< char*> > S, int &S1, int &S2, vector<int> &NS1, vector<int> &NS2, int &SID1, int &SID2){
	S1 = sampleI(S.size()); 
	S2 = sampleI(S.size());
	NS1 = s2c(mgetCol(S,1)[S1]);
	NS2 = s2c(mgetCol(S,1)[S2]); 
	SID1 = atoi(mgetCol(S,0)[S1]);
	SID2 = atoi(mgetCol(S,0)[S2]);
}

void shuffle(vector<int> &NS1, vector<int> &NS2){
	int Gene1, Gene2, index1, index2 = 0;
		index1 = sampleI(NS1.size());
		index2 = sampleI(NS2.size());
		Gene1 = NS1[index1];
		Gene2 = NS2[index2];
	while (Gene1 == Gene2){
		index1 = sampleI(NS1.size());
		index2 = sampleI(NS2.size());
		Gene1 = NS1[index1];
		Gene2 = NS2[index2];
	}
	NS1[index1] = Gene2;
	NS2[index2] = Gene1;
	
}	

void recombine(vector<vector< char*> > S, int fold, int &S1, int &S2, vector<int> &NS1, vector<int> &NS2, int &SID1, int &SID2, int sid){

	pickGenes(S, S1, S2, NS1, NS2, SID1, SID2);
	while (NS1 == NS2){
		pickGenes(S, S1, S2, NS1, NS2, SID1, SID2);
	}
	shuffle(NS1, NS2);
	while (!isUniq(NS1) or !isUniq(NS2)) {
		shuffle(NS1,NS2);
	}
	if (!DIRECTED) {
		std::sort(NS1.begin(), NS1.end());
		std::sort(NS2.begin(), NS2.end());
	}

	filterCluster(NS1,fold, sid); filterCluster(NS2,fold, sid);
	MetaTask(fold,atoi(mgetCol(S,0)[S1]));
	MetaTask(fold,atoi(mgetCol(S,0)[S2]));
	float score1 = getTrainAcc(fold,SID1);
	float score2 = getTrainAcc(fold,SID2);
	float tscore1 = getTestAcc(fold,SID1);
	float tscore2 = getTestAcc(fold,SID2);
	updateSolution(atoi(mgetCol(S,0)[S1]), NS1, score1, tscore1, sid);
	updateSolution(atoi(mgetCol(S,0)[S2]), NS2, score2, tscore2, sid);
	system(("rm -Rf " + string(TASK_OUTPUT_DIR) +"/history/fold_"+i2s(fold)+"/sol_"+i2s(SID1-getMinFoldId(fold,sid))).c_str());
	system(("cp -R " + string(TASK_OUTPUT_DIR) +"/current/fold_"+i2s(fold)+"/sol_"+i2s(SID1-getMinFoldId(fold,sid))+" "+TASK_OUTPUT_DIR +"/history/fold_"+i2s(fold)+"/sol_"+i2s(SID1-getMinFoldId(fold,sid))).c_str());
	system(("rm -Rf " + string(TASK_OUTPUT_DIR) +"/history/fold_"+i2s(fold)+"/sol_"+i2s(SID2-getMinFoldId(fold,sid))).c_str());
	system(("cp -R " + string(TASK_OUTPUT_DIR) +"/current/fold_"+i2s(fold)+"/sol_"+i2s(SID2-getMinFoldId(fold,sid))+" "+TASK_OUTPUT_DIR +"/history/fold_"+i2s(fold)+"/sol_"+i2s(SID2-getMinFoldId(fold,sid))).c_str());
}

void core(int fold, int steps, int sid){
	cout << " EVOLVING POPULATION" << "\n";
	progressBar(tLen, 0.0);
	int elapsedSteps = 0;
	float temp = 1.0;
	vector<float> v = make_vector<float>() << mutation << recombination;
	for(unsigned int S=0; S<steps; S++){
		float globalMean = getGlobalMean(sid);
		int action = weighted_choice(v);
		vector<vector< char* > > solutions = getSolutions(fold,sid);

		if (action == 0){
			vector<float> solSelector = vc2f(mgetCol(solutions,2));
			sigmoidTransform(solSelector,beta);
			normalize(solSelector);
			int choice = weighted_choice(solSelector);
			vector<char*> solution = solutions[choice];
			float oldScore = atof(solution[2]);
			float meanSolScore = getScoreBySolution(solution[1],sid);
			vector<int> cluster = s2c(solution[1]);
			bool STOP = false;
			while (!STOP){
				vector<vector<int> > subsets = combinations(cluster, cluster.size());
				map<int,float> localHazards;
				BOOST_FOREACH(int x, cluster) localHazards[x]=0;
				BOOST_FOREACH(vector<int> x, subsets) {
					map<vector<int>,float> localEvids;
					vector<vector<char*> > z = getEvid(x,sid);
					BOOST_FOREACH(vector<char*> a, z) 
						localEvids[s2c(a[0])] = atof(a[1]);
					deviationTransform(localEvids,globalMean);
					if (x.size()==1) {
						localHazards[x[0]] = mean(getVals(localEvids));
						updateNode(x[0],localHazards[x[0]]);
					}
				}
				vector<int> localKeys = getKeys(localHazards);
				for ( unsigned int i = 0; i < localKeys.size(); i++)
					localHazards[localKeys[i]] = sigmoid(localHazards[localKeys[i]],alpha);
				vector<float> localVals = getVals(localHazards);
				normalize(localVals);
				int mutateFrom = localKeys[weighted_choice(localVals)];
				map<int, float> compset = getComplementNodes(cluster,fold,sid);
				vector<float> compvals = getVals(compset);
				sigmoidTransform(compvals,-alpha);
				normalize(compvals);
				int mutateTo = getKeys(compset)[weighted_choice(compvals)];
				swaptGene(cluster, mutateFrom, mutateTo);
				filterCluster(cluster,fold, sid);
				int FSID = atoi(solution[0])-getMinFoldId(fold,sid);

				// Execute Metatask
				//MetaTask(fold,FSID);
				if (MOSES) {
					Moses(fold,FSID);
					cout << " MOSES EXECUTION...TERMINATE" << endl;
					system("pause");
					exit(0);
				} else {
					MetaTask(fold,FSID);
				}	

				float newScore = getTrainAcc(fold,FSID);
				insertEvid(cluster,getTrainAcc(fold,FSID),getTestAcc(fold,FSID),sid);
				STOP = stop(elapsedSteps, temp, oldScore, newScore, atoi(solution[0]), cluster, newScore, getTestAcc(fold,FSID), fold, sid);
			}
			progressBar(tLen, (float)(S+1)/(float)steps);
		} else {
			int sol1; vector<int> newSol1; int solid1;
			int sol2; vector<int> newSol2; int solid2;
			recombine(solutions, fold, sol1, sol2, newSol1, newSol2, solid1, solid2, sid);  //DEBUGGING
			progressBar(tLen, (float)(S+1)/(float)steps);
		}
	doneSteps ++;
	updateSession(sid, (float)doneSteps/(float)totalSteps*100.0);
	}
	cout << endl;
}

void MOSES(

int main(int argc, char *argv[]){

	// Seed random number generator
	srand(time(NULL));
	INFILE = argv[1];
	// Verify that everything works
	verify();

	// Pick and initiate daughter process
	BUUID = argv[2]; boost::algorithm::trim(BUUID);

	getNewSession(BUUID, sid, SUUID);
	cout << " BATCH REGISTLATION" << endl << hline;
	cout << " OPENING BATCH " << "\t\t\t    [" << BUUID << "]" << endl; 
	cout << " INHERIT DAUGHTER PROCESS " << "\t    [" << SUUID << "]" << endl << endl; 
	startSession(sid);
	clearCache();

	// Prepare input data
	prepare(argv[1], SUUID);
	registerStats(SUUID,((PERMUTE==0) ? "" : shexec(("more temp/"+SUUID+".tab").c_str())),sid,PERMUTE); 
	if (PERMUTE) system(("rm temp/"+SUUID+".tab").c_str());
	// Build sql data structure
	system(("mysql -u "+string(USER)+" -p"+string(PASS)+" < const/schema.sql").c_str());
	cout << " CREATING MySQL DATA FRAME\t\t\t\t\t [SUCCESS]\n";

	// For each fold get list of genes
	for (unsigned int FOLD = 0; FOLD < numberOfFolds; FOLD++) {
		cout << " PROCESSING FOLD " << FOLD+1 << " OF " << numberOfFolds << endl;
		cout << hline;
		vector<string> testFile, trainFile;
		vector<string> foldGenes;
		foldGenes.clear();
		readlines(("./"+string(INPUT_DIR)+"/train"+i2s(FOLD)+".tab").c_str(),trainFile);

		// Populate GENES
		for (unsigned int i = 2; i < trainFile.size(); i++) {
			vector <string> testVector, trainVector;
			boost::split(trainVector, trainFile[i], boost::is_any_of("\t"));
			foldGenes.push_back(getGene(trainVector[0]));
			insertGene(getGene(trainVector[0]),sid);
		}
		
		// Populate NODES
		vector<char*> genes = getAllGenes(sid);
		for (unsigned int i = 0; i < genes.size(); i++) insertNode(genes[i],f2s(smallnum),sid);

		// Populate FOLDS
		vector<char*> foldIds = getGeneIds(foldGenes, sid);
		for (unsigned int i = 0; i < foldIds.size(); i++) insertFold(i2s(FOLD),foldIds[i],sid);

		//Generate G(0) solution population 
		cout << " GENERATING POPULATION G(0)" << "\n";
		progressBar(tLen, 0.0);
		for (unsigned int indiv = 0; indiv < populationSize; indiv++) {
			system(("mkdir -p "+string(TASK_OUTPUT_DIR) +"/history/fold_"+i2s(FOLD)+"/sol_"+i2s(indiv)).c_str());		
			
			//Randomly choose N Nodes
			vector<int> cluster = subset(foldIds,sample(foldIds.size(),CLUS_TOPO));

			//Sort if network undirected
			if (!DIRECTED) std::sort(cluster.begin(), cluster.end());
			filterCluster(cluster,FOLD, sid);
			
			//Execute Metatask
			//MetaTask(FOLD,indiv);
			if (MOSES) {
				Moses(FOLD,indiv);
				exit(0);
			} else {
				MetaTask(FOLD,indiv);
			}			

			//Copy over MetaTask result to history
			system(("cp -R " + string(TASK_OUTPUT_DIR) +"/current/fold_"+i2s(FOLD)+"/sol_"+i2s(indiv)+" "+string(TASK_OUTPUT_DIR) +"/history/fold_"+i2s(FOLD)).c_str());
			insertSolution(cluster,FOLD,getTrainAcc(FOLD,indiv),getTestAcc(FOLD,indiv),sid);
			insertEvid(cluster,getTrainAcc(FOLD,indiv),getTestAcc(FOLD,indiv),sid);
			progressBar(tLen, (float)(indiv+1)/(float)populationSize);
			doneSteps ++;
			updateSession(sid, (float)doneSteps/(float)totalSteps*100.0);
		}
		core(FOLD,numOfSteps, sid);
	}
	cout << hline;
	closeSession(sid);
	cout << "\t\t\t\t\t\t\t       [COMPLETED]\n";
	return 0;
}

