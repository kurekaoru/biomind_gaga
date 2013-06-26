#include "../settings/db_sock.h"
#include <mysql/mysql.h>

void sql(){
	MYSQL mysql;
	MYSQL_ROW row;
	MYSQL_RES *result;
	unsigned int num_fields;
	unsigned int i;
	mysql_init(&mysql);
	mysql_real_connect(&mysql,HOST,USER,PASS,DB,PORT,NULL,0);
	if(mysql_query(&mysql, "SELECT * FROM SOLS")) {
		fprintf(stderr, " Failed to query database: \n Error: %s\n", mysql_error(&mysql));
	} else {
			result = mysql_store_result(&mysql);
			num_fields = mysql_num_fields(result);
			while ((row = mysql_fetch_row(result))) {
				unsigned long *lengths;
				lengths = mysql_fetch_lengths(result);
				for(i = 0; i < num_fields; i++) {
					printf(" %.*s \t", (int) lengths[i], row[i] ? row[i] : "NULL");
				}   
				printf("\n");
		}
	}
	mysql_close(&mysql);
}

vector<char*> getCol(const char* query){
	vector<char*> output;
	MYSQL mysql;
	MYSQL_ROW row;
	MYSQL_RES *result;
	unsigned int num_fields;
	unsigned int i;
	mysql_init(&mysql);
	mysql_real_connect(&mysql,HOST,USER,PASS,DB,PORT,NULL,0);
	if(mysql_query(&mysql, query)) {
		fprintf(stderr, " Failed to query database: \n Error: %s\n", mysql_error(&mysql));
	} else {
		result = mysql_store_result(&mysql);
		num_fields = mysql_num_fields(result);
		while ((row = mysql_fetch_row(result))) {
			unsigned long *lengths;
			lengths = mysql_fetch_lengths(result);
			for(i = 0; i < num_fields; i++) {
				output.push_back(row[0]);
			}   
		}
	}
	mysql_close(&mysql);
	return output;
}

vector<vector< char* > > getCols(const char* query){
	vector<vector< char* > > output;
	MYSQL mysql;
	MYSQL_ROW row;
	MYSQL_RES *result;
	unsigned int num_fields;
	unsigned int i;
	mysql_init(&mysql);
	mysql_real_connect(&mysql,HOST,USER,PASS,DB,PORT,NULL,0);
	if(mysql_query(&mysql, query)) {
		fprintf(stderr, " Failed to query database: \n Error: %s\n", mysql_error(&mysql));
	} else {
			result = mysql_store_result(&mysql);
			num_fields = mysql_num_fields(result);
			while ((row = mysql_fetch_row(result))) {
				unsigned long *lengths;
				lengths = mysql_fetch_lengths(result);
				vector<char*> object;
				for(i = 0; i < num_fields; i++) {
					object.push_back(row[i]);
				}
				output.push_back(object);
		}
	}
	mysql_close(&mysql);
	return output;
}

void Q(const char* query){
	MYSQL mysql;
	mysql_init(&mysql);
	mysql_real_connect(&mysql,HOST,USER,PASS,DB,PORT,NULL,0);
	if(mysql_query(&mysql, query))
		fprintf(stderr, " Failed to insert into MySQL database: \n Error: %s\n", mysql_error(&mysql));
	mysql_close(&mysql);
}

//-------------------------------------------------------------------------------------------------------GENERIC

vector<char*> getGeneIds(vector<string> genes, int pid){
	string idString;
	for(int i = 0; i < genes.size(); i++)
		idString+=("\""+genes[i]+"\",");
	idString = idString.substr(0,idString.size()-1);
	return getCol(("SELECT id FROM GA.GENES WHERE name IN ("+idString+")" +" AND pid = "+i2s(pid)).c_str());
}

vector<char*> getGeneNames(vector<int> genes, int pid){
	string idString;
	for(int i = 0; i < genes.size(); i++)
		idString+=(i2s(genes[i])+",");
	idString = idString.substr(0,idString.size()-1);
	//cout << " " << ("SELECT name FROM GA.GENES WHERE id IN ("+idString+") AND pid = "+i2s(pid)).c_str() << endl;
	return getCol(("SELECT name FROM GA.GENES WHERE id IN ("+idString+") AND pid = "+i2s(pid)).c_str());
}

vector<vector< char* > > getSolutions(int fold, int pid){
	return getCols(("SELECT id,sol,score FROM GA.SOLS WHERE pid = "+i2s(pid)+" AND fold ="+i2s(fold)).c_str());
}

void insertGene(string name, int pid){
	if (getCol(("SELECT id FROM GA.GENES where pid = "+i2s(pid)+" AND name = \""+name+"\"").c_str()).size() == 0)
		Q(("INSERT INTO GA.GENES(name, pid) VALUES(\""+name+"\","+i2s(pid)+")").c_str());
}

void insertNode(char* node, string score,int pid){
	if (getCol((string("SELECT node FROM GA.NODES where pid = "+i2s(pid)+" AND node = ")+node).c_str()).size() == 0)
		Q((string("INSERT  INTO GA.NODES(node,visits,score,pid) VALUES(")+node+",0,"+score+","+i2s(pid)+")").c_str());
}


void updateNode(int node, float score){
		Q(("UPDATE GA.NODES SET score = (score+"+f2s(score)+")/2,visits = visits + 1 WHERE node = "+i2s(node)).c_str());
}

void insertFold(string fold, string gene, int pid){
	Q((string("INSERT INTO GA.FOLDS(fold, gene, pid) VALUES(")+fold+","+gene+","+i2s(pid)+")").c_str());
}

vector<char*> getAllGenes(int pid){
	return getCol(("SELECT id FROM GA.GENES WHERE pid = "+i2s(pid)+" ORDER BY id").c_str());
}

string c2s(vector<int> cluster){
	string output = "|";
	for(unsigned int i=0; i<cluster.size(); i++)
		output+=(i2s(cluster[i])+"|");
	return output;
}

int getMinFoldId(int fold, int pid){
	return atoi(getCol(("SELECT min(id) FROM GA.SOLS WHERE pid = "+i2s(pid)+" AND fold = " + i2s(fold)).c_str())[0]);
}

void insertSolution(vector<int> cluster, int fold, float score, float tscore, int pid){
	Q(("INSERT INTO GA.SOLS(sol,fold,score,tscore,pid) VALUES(\'"+c2s(cluster)+"\',"+i2s(fold)+","+f2s(score)+","+f2s(tscore)+","+i2s(pid)+")").c_str());
}

void updateSolution(int solid, vector<int> cluster, float score, float tscore, int sid){
	Q(("UPDATE GA.SOLS SET sol = \'"+c2s(cluster)+"\', score = " + f2s(score)+", tscore = " + f2s(tscore) + " WHERE id = "+i2s(solid)).c_str());
}

void insertEvid(vector<int> cluster, float score, float tscore, int pid, string model){
	Q(("INSERT INTO GA.GA(clus,score,tscore, pid, model) VALUES(\'"+c2s(cluster)+"\',"+f2s(score)+","+f2s(tscore)+","+i2s(pid)+",\'"+model+"\')").c_str());
}

void registerBatch(string batchId, string server, string infile, int repeats, int &id){	
	Q((string("INSERT INTO GA_sessions.batch(batchId, inputFile, totalTasks) VALUES(\'")+batchId+"\',\'"+infile+"\',"+i2s(repeats)+")").c_str());
	id = atoi(getCol(("SELECT id FROM GA_sessions.batch WHERE batchId =\'"+batchId+'\'').c_str())[0]);
}

void registerSession(string UUID, int batchId){
	Q((string("INSERT INTO GA_sessions.sessions(batch, uuid) VALUES(")+i2s(batchId)+",\'"+UUID+"\')").c_str());
}

void getNewSession(string buuid, int &sid, string &suuid){
	std::vector<char*> _sid = getCol(("SELECT id FROM GA_sessions.sessions WHERE batch = (SELECT id FROM GA_sessions.batch WHERE batchId =\'"+buuid+"\') AND started = 0").c_str());
	if (_sid.size() != 0) {
		sid = atoi(_sid[0]);
	} else {
		//cout << " ALL DAUGHTER SESSIONS COMPLETED\t\t\t\t [SKIPPED]\n";
		exit(0);
	}
	std::vector<char*> _suuid = getCol(("SELECT uuid FROM GA_sessions.sessions WHERE id = "+i2s(sid)).c_str());
	if (_suuid.size() != 0){
		suuid = _suuid[0];
	} else {
		//cout << " ALL DAUGHTER SESSIONS COMPLETED\t\t\t\t [SKIPPED]\n";
		exit(0);
	}	
}

void updateSession(int sid, float complete){
	Q(("UPDATE GA_sessions.sessions SET complete = "+i2s(complete)+" WHERE id = "+i2s(sid)).c_str());
}

void closeSession(int sid){
	Q(("UPDATE GA_sessions.sessions SET completed = CURRENT_TIMESTAMP WHERE id = "+i2s(sid)).c_str());
	Q(("UPDATE GA_sessions.sessions SET complete = 100 WHERE id = "+i2s(sid)).c_str());
}

void startSession(int pid){
	Q(("UPDATE GA_sessions.sessions SET started = CURRENT_TIMESTAMP, server = replace(\'"+string(shexec("hostname | awk /./"))+"\',\'\\n\',\'\')  WHERE id = "+i2s(pid)).c_str());
}

void registerStats(string UUID, string labels, int pid, bool PERMUTATE){
	Q((string("INSERT INTO GA_sessions.stats(uuid, permutation, labels) VALUES(\'")+UUID+"\',"+((PERMUTE==0) ? "0" : "1")+",\'"+labels+"\')").c_str());
}

vector<vector< char* > > getEvid(vector<int> cluster, int pid){
	string query = "SELECT clus, score FROM GA.GA WHERE pid = "+i2s(pid)+" AND clus LIKE \'%|";
	for(unsigned int i = 0; i < cluster.size(); i++) {
		if (i>0) query += " AND clus like \'%|";	
		query += (i2s(cluster[i]) + "|%\'");
	}
	query += ';';
	vector<vector< char* > > out = getCols(query.c_str());
	return out;
}

float getGlobalMean(int pid){
	return atof(getCol(("SELECT AVG(score) from GA.GA WHERE pid = "+i2s(pid)).c_str())[0]);
}

template<class T>
string v2s(vector<T> V, string openstr, string endstr, string delim) {
	string out = openstr;
	for(unsigned int i = 0; i < V.size(); i++) {
		out += i2s(V[i]);
		out += delim;
	}
	out = out.substr(0,out.length()-1);
	out += endstr;
	return out;
}

map<int, float> getComplementNodes(vector<int> cluster, int fold, int pid){
	map<int, float> out;
	vector<vector<char*> > obj = getCols(("SELECT NODES.node,NODES.score FROM GA.NODES,GA.FOLDS WHERE NODES.pid = "+i2s(pid)+" AND NODES.node = FOLDS.gene AND FOLDS.fold = "+i2s(fold)+" AND FOLDS.gene NOT IN "+v2s(cluster,"(",")",",")).c_str());
	for(unsigned int i = 0; i < obj.size(); i++)
		out[atoi(obj[i][0])]=atof(obj[i][1]);
	return out;

}

float getScoreBySolution(const char* cluster, int pid){
	return atof(getCol(("SELECT AVG(score) FROM GA.SOLS WHERE pid = "+i2s(pid)+" AND sol = \'" + string(cluster) +"\'").c_str())[0]);
}

void createFoldSet(int fold, int pid){
	system(("mkdir -p "+string(GENE_TUPLET_OUTPUT_DIR)).c_str());
	string trainSet = "train"+i2s(fold)+".tab";
	string testSet = "test"+i2s(fold)+".tab";
	//Copy over Test set
	system(("cp " + string(INPUT_DIR)+"/"+testSet +" "+ string(GENE_TUPLET_OUTPUT_DIR)).c_str());
}

void filterCluster(vector<int> cluster, int fold, int pid){
	string trainSet = "train"+i2s(fold)+".tab";
	string testSet = "test"+i2s(fold)+".tab";
	vector<char*> G = getGeneNames(cluster, pid);

	//Get header
	system(("head -n2 " + string(INPUT_DIR)+"/"+trainSet+" > "+string(GENE_TUPLET_OUTPUT_DIR)+"/"+trainSet).c_str());
	//Create Train subset
	for (unsigned int i = 0; i < G.size(); i++) {
		system(("grep "+string((HAS_SNPS ? "\\#" : ""))+string(G[i])+"$w "+string(INPUT_DIR)+"/"+trainSet+" >> "+string(GENE_TUPLET_OUTPUT_DIR)+"/"+trainSet).c_str());
	}
	//Copy over Test set
	//system(("cp " + string(INPUT_DIR)+"/"+testSet +" "+ string(GENE_TUPLET_OUTPUT_DIR)).c_str());
	//cout << " copied " << string(INPUT_DIR)<<"/"<<testSet<<" to "<< string(GENE_TUPLET_OUTPUT_DIR)<<endl;
	if (cluster.size()!=G.size()) { cout << " PAUSED"; cin.ignore(); }
}
